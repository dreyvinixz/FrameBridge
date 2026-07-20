#include <pch.h>
#include <Config.h>
#include <Util.h>

#include <proxies/FfxApi_Proxy.h>

#include "FSR31Feature_Dx12.h"
#include <chrono>
#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER
#include <vector>
#include <algorithm>
#endif
#include "../../runtime/RuntimeConfiguration.h"

NVSDK_NGX_Parameter* FSR31FeatureDx12::SetParameters(NVSDK_NGX_Parameter* InParameters)
{
    InParameters->Set("OptiScaler.SupportsUpscaleSize", true);
    return InParameters;
}

FSR31FeatureDx12::FSR31FeatureDx12(unsigned int InHandleId, NVSDK_NGX_Parameter* InParameters)
    : FSR31Feature(InHandleId, InParameters), IFeature_Dx12(InHandleId, InParameters),
      IFeature(InHandleId, SetParameters(InParameters))
{
    FfxApiProxy::InitFfxDx12();

    _moduleLoaded = FfxApiProxy::IsSRReady();

    if (_moduleLoaded)
        LOG_INFO("amd_fidelityfx_dx12.dll methods loaded!");
    else
        LOG_ERROR("can't load amd_fidelityfx_dx12.dll methods!");
}

bool FSR31FeatureDx12::Init(ID3D12Device* InDevice, ID3D12GraphicsCommandList* InCommandList,
                            NVSDK_NGX_Parameter* InParameters)
{
    LOG_DEBUG("FSR31FeatureDx12::Init");

    if (IsInited())
        return true;

    Device = InDevice;

    if (InitFSR3(InParameters))
    {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (!Config::Instance()->OverlayMenu.value_or_default() && (Imgui == nullptr || Imgui.get() == nullptr))
#else
        if (!config.fsr31.overlayMenu && (Imgui == nullptr || Imgui.get() == nullptr))
#endif
            Imgui = std::make_unique<Menu_Dx12>(Util::GetProcessWindow(), InDevice);

        OutputScaler = std::make_unique<OS_Dx12>("Output Scaling", InDevice, (TargetWidth() < DisplayWidth()));
        RCAS = std::make_unique<RCAS_Dx12>("RCAS", InDevice);
        Bias = std::make_unique<Bias_Dx12>("Bias", InDevice);

        return true;
    }

    return false;
}

#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER
struct BenchmarkTimer
{
    std::chrono::high_resolution_clock::time_point start;
    BenchmarkTimer() { start = std::chrono::high_resolution_clock::now(); }
    ~BenchmarkTimer()
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        static std::vector<long long> samples;
        samples.push_back(duration);
        if (samples.size() >= 1000)
        {
            std::sort(samples.begin(), samples.end());
            long long sum = 0;
            for (auto s : samples)
                sum += s;
            double avg = sum / 1000.0;
            long long p50 = samples[500];
            long long p95 = samples[950];
            long long p99 = samples[990];
            long long max_val = samples[999];
            LOG_INFO("[FrameBridge Benchmark] Evaluate samples: 1000 | Avg: {:.3f} ms | P50: {:.3f} ms | P95: {:.3f} "
                     "ms | P99: {:.3f} ms | Max: {:.3f} ms | Path: {}",
                     avg / 1000.0, p50 / 1000.0, p95 / 1000.0, p99 / 1000.0, max_val / 1000.0,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                     "Legacy"
#else
                     "Snapshot"
#endif
            );
            samples.clear();
        }
    }
};
#endif
bool FSR31FeatureDx12::Evaluate(ID3D12GraphicsCommandList* InCommandList, NVSDK_NGX_Parameter* InParameters)
{
#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER
    BenchmarkTimer __timer;
#endif

    LOG_FUNC();
#ifndef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    auto config = RuntimeConfiguration::Instance().GetSnapshot();
#endif

    if (!IsInited())
        return false;

    if (!RCAS->IsInit())
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
        Config::Instance()->RcasEnabled.set_volatile_value(false);
    config.fsr31.rcasEnabled = false;
#endif

    if (!OutputScaler->IsInit())
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
#else
        Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
    config.fsr31.outputScalingEnabled = false;
#endif

    if (Config::Instance()->DADepthIsLinear.value_for_config_ignore_default() == std::nullopt)
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->DADepthIsLinear.set_volatile_value(false);
#else
        Config::Instance()->DADepthIsLinear.set_volatile_value(false);
    config.fsr31.daDepthIsLinear = false;
#endif

    struct ffxDispatchDescUpscale params = { 0 };
    params.header.type = FFX_API_DISPATCH_DESC_TYPE_UPSCALE;

    params.flags = 0;

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->FsrDebugView.value_or_default() &&
#else
    if (config.fsr31.debugView &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        (Version() < feature_version { 4, 0, 0 } || Config::Instance()->Fsr4EnableDebugView.value_or_default()))
#else
        (Version() < feature_version { 4, 0, 0 } || config.fsr31.enableDebugViewForFsr4))
#endif
    {
        params.flags |= FFX_UPSCALE_FLAG_DRAW_DEBUG_VIEW;
    }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->FsrNonLinearPQ.value_or_default())
#else
    if (config.fsr31.nonLinearPQ)
#endif
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_PQ;
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else if (Config::Instance()->FsrNonLinearSRGB.value_or_default())
#else
    else if (config.fsr31.nonLinearSRGB)
#endif
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_SRGB;

    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_X, &params.jitterOffset.x);
    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_Y, &params.jitterOffset.y);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->OverrideSharpness.value_or_default())
#else
    if (config.fsr31.overrideSharpness)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _sharpness = Config::Instance()->Sharpness.value_or_default();
#else
        _sharpness = config.fsr31.sharpness;
#endif
    else
        _sharpness = GetSharpness(InParameters);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->RcasEnabled.value_or_default())
#else
    if (config.fsr31.rcasEnabled)
#endif
    {
        params.enableSharpening = false;
        params.sharpness = 0.0f;
    }
    else
    {
        if (_sharpness > 1.0f)
            _sharpness = 1.0f;

        params.enableSharpening = _sharpness > 0.0f;
        params.sharpness = _sharpness;
    }

    // Force enable RCAS when in FSR4 debug view mode
    // it crashes when sharpening is disabled
    // Debug view expects RCAS output (now sure why)
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version() >= feature_version { 4, 0, 2 } && Config::Instance()->FsrDebugView.value_or_default() &&
#else
    if (Version() >= feature_version { 4, 0, 2 } && config.fsr31.debugView &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->Fsr4EnableDebugView.value_or_default() && !params.enableSharpening)
#else
        config.fsr31.enableDebugViewForFsr4 && !params.enableSharpening)
#endif
    {
        params.enableSharpening = true;
        params.sharpness = 0.01f;
    }

    LOG_DEBUG("Jitter Offset: {0}x{1}", params.jitterOffset.x, params.jitterOffset.y);

    unsigned int reset;
    InParameters->Get(NVSDK_NGX_Parameter_Reset, &reset);
    params.reset = (reset == 1);

    GetRenderResolution(InParameters, &params.renderSize.width, &params.renderSize.height);

    bool useSS =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default() && (LowResMV() || RenderWidth() == DisplayWidth());
#else
        config.fsr31.outputScalingEnabled && (LowResMV() || RenderWidth() == DisplayWidth());
#endif

    LOG_DEBUG("Input Resolution: {0}x{1}", params.renderSize.width, params.renderSize.height);

    params.commandList = InCommandList;

    ID3D12Resource* paramColor;
    if (InParameters->Get(NVSDK_NGX_Parameter_Color, &paramColor) != NVSDK_NGX_Result_Success)
        InParameters->Get(NVSDK_NGX_Parameter_Color, (void**) &paramColor);

    if (paramColor)
    {
        LOG_DEBUG("Color exist..");

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->ColorResourceBarrier.has_value())
#else
        if (config.fsr31.colorResourceBarrier.has_value())
#endif
        {
            ResourceBarrier(InCommandList, paramColor,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->ColorResourceBarrier.value(),
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.colorResourceBarrier.value(),
#endif
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        }
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
                 State::Instance().GameEngine == GameEngineType::Unreal ||
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->ColorResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_RENDER_TARGET);
#else
            Config::Instance()->ColorResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_RENDER_TARGET);
            config.fsr31.colorResourceBarrier = D3D12_RESOURCE_STATE_RENDER_TARGET;
#endif
            ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_RENDER_TARGET,
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        }

        params.color = ffxApiGetResourceDX12(paramColor, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    }
    else
    {
        LOG_ERROR("Color not exist!!");
        return false;
    }

    ID3D12Resource* paramVelocity;
    if (InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, &paramVelocity) != NVSDK_NGX_Result_Success)
        InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, (void**) &paramVelocity);

    if (paramVelocity)
    {
        LOG_DEBUG("MotionVectors exist..");

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->MVResourceBarrier.has_value())
#else
        if (config.fsr31.mvResourceBarrier.has_value())
#endif
        {
            ResourceBarrier(InCommandList, paramVelocity,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->MVResourceBarrier.value(),
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.mvResourceBarrier.value(),
#endif
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        }
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
                 State::Instance().GameEngine == GameEngineType::Unreal ||
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->MVResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
            Config::Instance()->MVResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            config.fsr31.mvResourceBarrier = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
#endif
            ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
        }

        params.motionVectors = ffxApiGetResourceDX12(paramVelocity, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    }
    else
    {
        LOG_ERROR("MotionVectors not exist!!");
        return false;
    }

    ID3D12Resource* paramOutput;
    if (InParameters->Get(NVSDK_NGX_Parameter_Output, &paramOutput) != NVSDK_NGX_Result_Success)
        InParameters->Get(NVSDK_NGX_Parameter_Output, (void**) &paramOutput);

    if (paramOutput)
    {
        LOG_DEBUG("Output exist..");

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->OutputResourceBarrier.has_value())
#else
        if (config.fsr31.outputResourceBarrier.has_value())
#endif
            ResourceBarrier(InCommandList, paramOutput,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->OutputResourceBarrier.value(),
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.outputResourceBarrier.value(),
#endif
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

        if (useSS)
        {
            if (OutputScaler->CreateBufferResource(Device, paramOutput, TargetWidth(), TargetHeight(),
                                                   D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
            {
                OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
                params.output = ffxApiGetResourceDX12(OutputScaler->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
            }
            else
                params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
        }
        else
            params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->RcasEnabled.value_or_default() &&
#else
        if (config.fsr31.rcasEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            (_sharpness > 0.0f || (Config::Instance()->MotionSharpnessEnabled.value_or_default() &&
#else
            (_sharpness > 0.0f || (config.fsr31.motionSharpnessEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                   Config::Instance()->MotionSharpness.value_or_default() > 0.0f)) &&
#else
                                   config.fsr31.motionSharpness > 0.0f)) &&
#endif
            RCAS->IsInit() &&
            RCAS->CreateBufferResource(Device, (ID3D12Resource*) params.output.resource,
                                       D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
        {
            RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
            params.output = ffxApiGetResourceDX12(RCAS->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
        }
    }
    else
    {
        LOG_ERROR("Output not exist!!");
        return false;
    }

    ID3D12Resource* paramDepth;
    if (InParameters->Get(NVSDK_NGX_Parameter_Depth, &paramDepth) != NVSDK_NGX_Result_Success)
        InParameters->Get(NVSDK_NGX_Parameter_Depth, (void**) &paramDepth);

    if (paramDepth)
    {
        LOG_DEBUG("Depth exist..");

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->DepthResourceBarrier.has_value())
#else
        if (config.fsr31.depthResourceBarrier.has_value())
#endif
            ResourceBarrier(InCommandList, paramDepth,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->DepthResourceBarrier.value(),
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.depthResourceBarrier.value(),
#endif
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

        params.depth = ffxApiGetResourceDX12(paramDepth, FFX_API_RESOURCE_STATE_COMPUTE_READ);
    }
    else
    {
        LOG_ERROR("Depth not exist!!");

        if (LowResMV())
            return false;
    }

    ID3D12Resource* paramExp = nullptr;
    if (AutoExposure())
    {
        LOG_DEBUG("AutoExposure enabled!");
    }
    else
    {
        if (InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, &paramExp) != NVSDK_NGX_Result_Success)
            InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, (void**) &paramExp);

        if (paramExp)
        {
            LOG_DEBUG("ExposureTexture exist..");

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Config::Instance()->ExposureResourceBarrier.has_value())
#else
            if (config.fsr31.exposureResourceBarrier.has_value())
#endif
                ResourceBarrier(InCommandList, paramExp,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (D3D12_RESOURCE_STATES) Config::Instance()->ExposureResourceBarrier.value(),
#else
                                (D3D12_RESOURCE_STATES) config.fsr31.exposureResourceBarrier.value(),
#endif
                                D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

            params.exposure = ffxApiGetResourceDX12(paramExp, FFX_API_RESOURCE_STATE_COMPUTE_READ);
        }
        else
        {
            LOG_DEBUG("AutoExposure disabled but ExposureTexture is not exist, it may cause problems!!");
            State::Instance().AutoExposure = true;
            State::Instance().changeBackend[Handle()->Id] = true;
            return true;
        }
    }

    ID3D12Resource* paramTransparency = nullptr;
    if (InParameters->Get("FSR.transparencyAndComposition", &paramTransparency) == NVSDK_NGX_Result_Success)
        InParameters->Get("FSR.transparencyAndComposition", (void**) &paramTransparency);

    ID3D12Resource* paramReactiveMask = nullptr;
    if (InParameters->Get("FSR.reactive", &paramReactiveMask) == NVSDK_NGX_Result_Success)
        InParameters->Get("FSR.reactive", (void**) &paramReactiveMask);

    ID3D12Resource* paramReactiveMask2 = nullptr;
    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, &paramReactiveMask2) !=
        NVSDK_NGX_Result_Success)
        InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, (void**) &paramReactiveMask2);

    if (!Config::Instance()->DisableReactiveMask.value_or(paramReactiveMask == nullptr &&
                                                          paramReactiveMask2 == nullptr))
    {
        if (paramTransparency != nullptr)
        {
            LOG_DEBUG("Using FSR transparency mask..");
            params.transparencyAndComposition =
                ffxApiGetResourceDX12(paramTransparency, FFX_API_RESOURCE_STATE_COMPUTE_READ);
        }

        if (paramReactiveMask != nullptr)
        {
            LOG_DEBUG("Using FSR reactive mask..");
            params.reactive = ffxApiGetResourceDX12(paramReactiveMask, FFX_API_RESOURCE_STATE_COMPUTE_READ);
        }
        else
        {
            if (paramReactiveMask2 != nullptr)
            {
                LOG_DEBUG("Input Bias mask exist..");
                Config::Instance()->DisableReactiveMask.set_volatile_value(false);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (Config::Instance()->MaskResourceBarrier.has_value())
#else
                if (config.fsr31.maskResourceBarrier.has_value())
#endif
                    ResourceBarrier(InCommandList, paramReactiveMask2,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                    (D3D12_RESOURCE_STATES) Config::Instance()->MaskResourceBarrier.value(),
#else
                                    (D3D12_RESOURCE_STATES) config.fsr31.maskResourceBarrier.value(),
#endif
                                    D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

                if (paramTransparency == nullptr && Config::Instance()->FsrUseMaskForTransparency.value_or_default())
                    params.transparencyAndComposition =
                        ffxApiGetResourceDX12(paramReactiveMask2, FFX_API_RESOURCE_STATE_COMPUTE_READ);

                if (Config::Instance()->DlssReactiveMaskBias.value_or_default() > 0.0f && Bias->IsInit() &&
                    Bias->CreateBufferResource(Device, paramReactiveMask2, D3D12_RESOURCE_STATE_UNORDERED_ACCESS) &&
                    Bias->CanRender())
                {
                    Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

                    if (Bias->Dispatch(Device, InCommandList, paramReactiveMask2,
                                       Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->Buffer()))
                    {
                        Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
                        params.reactive = ffxApiGetResourceDX12(Bias->Buffer(), FFX_API_RESOURCE_STATE_COMPUTE_READ);
                    }
                }
                else
                {
                    LOG_DEBUG("Skipping reactive mask, Bias: {0}, Bias Init: {1}, Bias CanRender: {2}",
                              Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->IsInit(),
                              Bias->CanRender());
                }
            }
        }
    }

    _hasColor = params.color.resource != nullptr;
    _hasDepth = params.depth.resource != nullptr;
    _hasMV = params.motionVectors.resource != nullptr;
    _hasExposure = params.exposure.resource != nullptr;
    _hasTM = params.transparencyAndComposition.resource != nullptr;
    _accessToReactiveMask = paramReactiveMask != nullptr;
    _hasOutput = params.output.resource != nullptr;

    // For FSR 4 as it seems to be missing some conversions from typeless
    // transparencyAndComposition and exposure might be unnecessary here
    if (Version().major >= 4)
    {
        params.color.description.format = ffxResolveTypelessFormat(params.color.description.format);
        params.depth.description.format = ffxResolveTypelessFormat(params.depth.description.format);
        params.motionVectors.description.format = ffxResolveTypelessFormat(params.motionVectors.description.format);
        params.exposure.description.format = ffxResolveTypelessFormat(params.exposure.description.format);
        params.transparencyAndComposition.description.format =
            ffxResolveTypelessFormat(params.transparencyAndComposition.description.format);
        params.output.description.format = ffxResolveTypelessFormat(params.output.description.format);
    }

    params.motionVectorScale.x = 1.0f;
    params.motionVectorScale.y = 1.0f;

    if (InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &params.motionVectorScale.x) != NVSDK_NGX_Result_Success ||
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &params.motionVectorScale.y) != NVSDK_NGX_Result_Success)
    {
        LOG_WARN("Can't get motion vector scales!");
    }

    LOG_DEBUG("Sharpness: {0}", params.sharpness);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
        InParameters->Get("FSR.cameraNear", &params.cameraNear) != NVSDK_NGX_Result_Success)
    {
        if (DepthInverted())
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFar = Config::Instance()->FsrCameraNear.value_or_default();
#else
            params.cameraFar = config.fsr31.cameraNear;
#endif
        else
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraNear = Config::Instance()->FsrCameraNear.value_or_default();
#else
            params.cameraNear = config.fsr31.cameraNear;
#endif
    }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
        InParameters->Get("FSR.cameraFar", &params.cameraFar) != NVSDK_NGX_Result_Success)
    {
        if (DepthInverted())
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraNear = Config::Instance()->FsrCameraFar.value_or_default();
#else
            params.cameraNear = config.fsr31.cameraFar;
#endif
        else
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFar = Config::Instance()->FsrCameraFar.value_or_default();
#else
            params.cameraFar = config.fsr31.cameraFar;
#endif
    }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
        InParameters->Get("FSR.cameraFovAngleVertical", &params.cameraFovAngleVertical) != NVSDK_NGX_Result_Success)
    {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FsrVerticalFov.has_value())
#else
        if (config.fsr31.verticalFov.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFovAngleVertical = Config::Instance()->FsrVerticalFov.value() * 0.0174532925199433f;
#else
            params.cameraFovAngleVertical = config.fsr31.verticalFov.value() * 0.0174532925199433f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else if (Config::Instance()->FsrHorizontalFov.value_or_default() > 0.0f)
#else
        else if (config.fsr31.horizontalFov > 0.0f)
#endif
            params.cameraFovAngleVertical =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                2.0f * atan((tan(Config::Instance()->FsrHorizontalFov.value() * 0.0174532925199433f) * 0.5f) /
#else
                2.0f * atan((tan(config.fsr31.horizontalFov * 0.0174532925199433f) * 0.5f) /
#endif
                            (float) TargetHeight() * (float) TargetWidth());
        else
            params.cameraFovAngleVertical = 1.0471975511966f;
    }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
        InParameters->Get("FSR.frameTimeDelta", &params.frameTimeDelta) != NVSDK_NGX_Result_Success)
    {
        if (InParameters->Get(NVSDK_NGX_Parameter_FrameTimeDeltaInMsec, &params.frameTimeDelta) !=
                NVSDK_NGX_Result_Success ||
            params.frameTimeDelta < 1.0f)
            params.frameTimeDelta = (float) GetDeltaTime();
    }

    LOG_DEBUG("FrameTimeDeltaInMsec: {0}", params.frameTimeDelta);

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
        InParameters->Get("FSR.viewSpaceToMetersFactor", &params.viewSpaceToMetersFactor) != NVSDK_NGX_Result_Success)
        params.viewSpaceToMetersFactor = 0.0f;

    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Pre_Exposure, &params.preExposure) != NVSDK_NGX_Result_Success)
        params.preExposure = 1.0f;

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version() >= feature_version { 3, 1, 1 } && _velocity != Config::Instance()->FsrVelocity.value_or_default())
#else
    if (Version() >= feature_version { 3, 1, 1 } && _velocity != config.fsr31.velocity)
#endif
    {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _velocity = Config::Instance()->FsrVelocity.value_or_default();
#else
        _velocity = config.fsr31.velocity;
#endif
        ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
        m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
        m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FVELOCITYFACTOR;
        m_upscalerKeyValueConfig.ptr = &_velocity;
        auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);

        if (result != FFX_API_RETURN_OK)
            LOG_WARN("Velocity configure result: {}", (UINT) result);
    }

    if (Version() >= feature_version { 3, 1, 4 })
    {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_reactiveScale != Config::Instance()->FsrReactiveScale.value_or_default())
#else
        if (_reactiveScale != config.fsr31.reactiveScale)
#endif
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _reactiveScale = Config::Instance()->FsrReactiveScale.value_or_default();
#else
            _reactiveScale = config.fsr31.reactiveScale;
#endif
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FREACTIVENESSSCALE;
            m_upscalerKeyValueConfig.ptr = &_reactiveScale;
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);

            if (result != FFX_API_RETURN_OK)
                LOG_WARN("Reactive Scale configure result: {}", (UINT) result);
        }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_shadingScale != Config::Instance()->FsrShadingScale.value_or_default())
#else
        if (_shadingScale != config.fsr31.shadingScale)
#endif
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _shadingScale = Config::Instance()->FsrShadingScale.value_or_default();
#else
            _shadingScale = config.fsr31.shadingScale;
#endif
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FSHADINGCHANGESCALE;
            m_upscalerKeyValueConfig.ptr = &_shadingScale;
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);

            if (result != FFX_API_RETURN_OK)
                LOG_WARN("Shading Scale configure result: {}", (UINT) result);
        }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_accAddPerFrame != Config::Instance()->FsrAccAddPerFrame.value_or_default())
#else
        if (_accAddPerFrame != config.fsr31.accAddPerFrame)
#endif
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _accAddPerFrame = Config::Instance()->FsrAccAddPerFrame.value_or_default();
#else
            _accAddPerFrame = config.fsr31.accAddPerFrame;
#endif
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FACCUMULATIONADDEDPERFRAME;
            m_upscalerKeyValueConfig.ptr = &_accAddPerFrame;
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);

            if (result != FFX_API_RETURN_OK)
                LOG_WARN("Acc. Add Per Frame configure result: {}", (UINT) result);
        }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_minDisOccAcc != Config::Instance()->FsrMinDisOccAcc.value_or_default())
#else
        if (_minDisOccAcc != config.fsr31.minDisOccAcc)
#endif
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _minDisOccAcc = Config::Instance()->FsrMinDisOccAcc.value_or_default();
#else
            _minDisOccAcc = config.fsr31.minDisOccAcc;
#endif
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FMINDISOCCLUSIONACCUMULATION;
            m_upscalerKeyValueConfig.ptr = &_minDisOccAcc;
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);

            if (result != FFX_API_RETURN_OK)
                LOG_WARN("Minimum Disocclusion Acc. configure result: {}", (UINT) result);
        }
    }

    if (InParameters->Get("FSR.upscaleSize.width", &params.upscaleSize.width) == NVSDK_NGX_Result_Success &&
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default())
#else
        config.fsr31.outputScalingEnabled)
#endif
    {
        auto originalWidth = static_cast<float>(params.upscaleSize.width);
        params.upscaleSize.width =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            static_cast<uint32_t>(originalWidth * Config::Instance()->OutputScalingMultiplier.value_or_default());
#else
            static_cast<uint32_t>(originalWidth * config.fsr31.outputScalingMultiplier);
#endif
    }
    else if (params.upscaleSize.width == 0)
    {
        params.upscaleSize.width = TargetWidth();
    }

    if (InParameters->Get("FSR.upscaleSize.height", &params.upscaleSize.height) == NVSDK_NGX_Result_Success &&
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default())
#else
        config.fsr31.outputScalingEnabled)
#endif
    {
        auto originalHeight = static_cast<float>(params.upscaleSize.height);
        params.upscaleSize.height =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            static_cast<uint32_t>(originalHeight * Config::Instance()->OutputScalingMultiplier.value_or_default());
#else
            static_cast<uint32_t>(originalHeight * config.fsr31.outputScalingMultiplier);
#endif
    }
    else if (params.upscaleSize.height == 0)
    {
        params.upscaleSize.height = TargetHeight();
    }

    LOG_DEBUG("Dispatch!!");
    auto result = FfxApiProxy::D3D12_Dispatch(&_context, &params.header);

    if (result != FFX_API_RETURN_OK)
    {
        LOG_ERROR("_dispatch error: {0}", FfxApiProxy::ReturnCodeToString(result));

        if (result == FFX_API_RETURN_ERROR_RUNTIME_ERROR)
        {
            LOG_WARN("Trying to recover by recreating the feature");
            State::Instance().changeBackend[Handle()->Id] = true;
        }

        return false;
    }

    // apply rcas
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->RcasEnabled.value_or_default() &&
#else
    if (config.fsr31.rcasEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        (_sharpness > 0.0f || (Config::Instance()->MotionSharpnessEnabled.value_or_default() &&
#else
        (_sharpness > 0.0f || (config.fsr31.motionSharpnessEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                               Config::Instance()->MotionSharpness.value_or_default() > 0.0f)) &&
#else
                               config.fsr31.motionSharpness > 0.0f)) &&
#endif
        RCAS->CanRender())
    {
        if (params.output.resource != RCAS->Buffer())
            ResourceBarrier(InCommandList, (ID3D12Resource*) params.output.resource,
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

        RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

        RcasConstants rcasConstants {};

        rcasConstants.Sharpness = _sharpness;
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &rcasConstants.MvScaleX);
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &rcasConstants.MvScaleY);

        if (DepthInverted())
        {
            rcasConstants.CameraNear = params.cameraFar;
            rcasConstants.CameraFar = params.cameraNear;
        }
        else
        {
            rcasConstants.CameraNear = params.cameraNear;
            rcasConstants.CameraFar = params.cameraFar;
        }

        if (useSS)
        {
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, OutputScaler->Buffer(),
                                (ID3D12Resource*) params.depth.resource))
            {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
                Config::Instance()->RcasEnabled.set_volatile_value(false);
                config.fsr31.rcasEnabled = false;
#endif
                return true;
            }
        }
        else
        {
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, paramOutput,
                                (ID3D12Resource*) params.depth.resource))
            {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
                Config::Instance()->RcasEnabled.set_volatile_value(false);
                config.fsr31.rcasEnabled = false;
#endif
                return true;
            }
        }
    }

    if (useSS)
    {
        LOG_DEBUG("scaling output...");
        OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

        if (!OutputScaler->Dispatch(Device, InCommandList, OutputScaler->Buffer(), paramOutput))
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
#else
            Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
            config.fsr31.outputScalingEnabled = false;
#endif
            State::Instance().changeBackend[Handle()->Id] = true;
            return true;
        }
    }

    // imgui
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->OverlayMenu.value_or_default() && _frameCount > 30)
#else
    if (!config.fsr31.overlayMenu && _frameCount > 30)
#endif
    {
        if (Imgui != nullptr && Imgui.get() != nullptr)
        {
            if (Imgui->IsHandleDifferent())
            {
                Imgui.reset();
            }
            else
                Imgui->Render(InCommandList, paramOutput);
        }
        else
        {
            if (Imgui == nullptr || Imgui.get() == nullptr)
                Imgui = std::make_unique<Menu_Dx12>(GetForegroundWindow(), Device);
        }
    }

    // restore resource states
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramColor && Config::Instance()->ColorResourceBarrier.has_value())
#else
    if (paramColor && config.fsr31.colorResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->ColorResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.colorResourceBarrier.value());
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramVelocity && Config::Instance()->MVResourceBarrier.has_value())
#else
    if (paramVelocity && config.fsr31.mvResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->MVResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.mvResourceBarrier.value());
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramOutput && Config::Instance()->OutputResourceBarrier.has_value())
#else
    if (paramOutput && config.fsr31.outputResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->OutputResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.outputResourceBarrier.value());
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramDepth && Config::Instance()->DepthResourceBarrier.has_value())
#else
    if (paramDepth && config.fsr31.depthResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramDepth, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->DepthResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.depthResourceBarrier.value());
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramExp && Config::Instance()->ExposureResourceBarrier.has_value())
#else
    if (paramExp && config.fsr31.exposureResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramExp, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->ExposureResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.exposureResourceBarrier.value());
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramReactiveMask && Config::Instance()->MaskResourceBarrier.has_value())
#else
    if (paramReactiveMask && config.fsr31.maskResourceBarrier.has_value())
#endif
        ResourceBarrier(InCommandList, paramReactiveMask, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->MaskResourceBarrier.value());
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.maskResourceBarrier.value());
#endif

    _frameCount++;

    return true;
}

bool FSR31FeatureDx12::InitFSR3(const NVSDK_NGX_Parameter* InParameters)
{
    LOG_FUNC();
#ifndef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    RuntimeConfiguration::Instance().RefreshFromConfig();
    auto config = RuntimeConfiguration::Instance().GetSnapshot();
#endif

    if (!ModuleLoaded())
        return false;

    if (IsInited())
        return true;

    if (Device == nullptr)
    {
        LOG_ERROR("D3D12Device is null!");
        return false;
    }

    {
        ScopedSkipSpoofing skipSpoofing {};

        ffxQueryDescGetVersions versionQuery {};
        versionQuery.header.type = FFX_API_QUERY_DESC_TYPE_GET_VERSIONS;
        versionQuery.createDescType = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
        versionQuery.device = Device; // only for DirectX 12 applications
        uint64_t versionCount = 0;
        versionQuery.outputCount = &versionCount;
        // get number of versions for allocation
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);

        State::Instance().ffxUpscalerVersionIds.resize(versionCount);
        State::Instance().ffxUpscalerVersionNames.resize(versionCount);
        versionQuery.versionIds = State::Instance().ffxUpscalerVersionIds.data();
        versionQuery.versionNames = State::Instance().ffxUpscalerVersionNames.data();
        // fill version ids and names arrays.
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);

        _contextDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;

        _contextDesc.flags = 0;

#ifdef _DEBUG
        LOG_INFO("Debug checking enabled!");
        _contextDesc.fpMessage = FfxLogCallback;
        _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEBUG_CHECKING;
#endif

        if (DepthInverted())
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEPTH_INVERTED;

        if (AutoExposure())
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_AUTO_EXPOSURE;

        if (IsHdr())
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_HIGH_DYNAMIC_RANGE;

        if (JitteredMV())
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION;

        if (!LowResMV())
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS;

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FsrNonLinearColorSpace.value_or_default())
#else
        if (config.fsr31.nonLinearColorSpace)
#endif
        {
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_NON_LINEAR_COLORSPACE;
            LOG_INFO("contextDesc.initFlags (NonLinearColorSpace) {0:b}", _contextDesc.flags);
        }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->Fsr4EnableDebugView.value_or_default())
#else
        if (config.fsr31.enableDebugViewForFsr4)
#endif
        {
            LOG_INFO("Debug view enabled!");
            _contextDesc.flags |= 512; // FFX_UPSCALE_ENABLE_DEBUG_VISUALIZATION
        }

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->OutputScalingEnabled.value_or_default() &&
#else
        if (config.fsr31.outputScalingEnabled &&
#endif
            (LowResMV() || RenderWidth() == DisplayWidth()))
        {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            float ssMulti = Config::Instance()->OutputScalingMultiplier.value_or_default();
#else
            float ssMulti = config.fsr31.outputScalingMultiplier;
#endif

            if (ssMulti < 0.5f)
            {
                ssMulti = 0.5f;
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
#else
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
                config.fsr31.outputScalingMultiplier = ssMulti;
#endif
            }
            else if (ssMulti > 3.0f)
            {
                ssMulti = 3.0f;
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
#else
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
                config.fsr31.outputScalingMultiplier = ssMulti;
#endif
            }

            _targetWidth = static_cast<unsigned int>(DisplayWidth() * ssMulti);
            _targetHeight = static_cast<unsigned int>(DisplayHeight() * ssMulti);
        }
        else
        {
            _targetWidth = DisplayWidth();
            _targetHeight = DisplayHeight();
        }

        // extended limits changes how resolution
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->ExtendedLimits.value_or_default() && RenderWidth() > DisplayWidth())
#else
        if (config.fsr31.extendedLimits && RenderWidth() > DisplayWidth())
#endif
        {
            _contextDesc.maxRenderSize.width = RenderWidth();
            _contextDesc.maxRenderSize.height = RenderHeight();

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->OutputScalingMultiplier.set_volatile_value(1.0f);
#else
            Config::Instance()->OutputScalingMultiplier.set_volatile_value(1.0f);
            config.fsr31.outputScalingMultiplier = 1.0f;
#endif

            // if output scaling active let it to handle downsampling
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Config::Instance()->OutputScalingEnabled.value_or_default() &&
#else
            if (config.fsr31.outputScalingEnabled &&
#endif
                (LowResMV() || RenderWidth() == DisplayWidth()))
            {
                _contextDesc.maxUpscaleSize.width = _contextDesc.maxRenderSize.width;
                _contextDesc.maxUpscaleSize.height = _contextDesc.maxRenderSize.height;

                // update target res
                _targetWidth = _contextDesc.maxRenderSize.width;
                _targetHeight = _contextDesc.maxRenderSize.height;
            }
            else
            {
                _contextDesc.maxUpscaleSize.width = DisplayWidth();
                _contextDesc.maxUpscaleSize.height = DisplayHeight();
            }
        }
        else
        {
            _contextDesc.maxRenderSize.width = TargetWidth() > DisplayWidth() ? TargetWidth() : DisplayWidth();
            _contextDesc.maxRenderSize.height = TargetHeight() > DisplayHeight() ? TargetHeight() : DisplayHeight();
            _contextDesc.maxUpscaleSize.width = TargetWidth();
            _contextDesc.maxUpscaleSize.height = TargetHeight();
        }

        // Set stability values as default if not set by user
        {
            auto config = Config::Instance();
            auto const scaleRatioX = (float) TargetWidth() / (float) RenderWidth();
            auto const scaleRatioY = (float) TargetHeight() / (float) RenderHeight();
            auto const scaleRatio = std::max(scaleRatioX, scaleRatioY);

            if (scaleRatio > 0.0f && !std::isinf(scaleRatio))
            {
                if (config->FsrVelocity.value_for_config() == std::nullopt)
                    config->FsrVelocity.set_volatile_value(0.5f);

                if (config->FsrReactiveScale.value_for_config() == std::nullopt)
                    config->FsrReactiveScale.set_volatile_value(0.25f);

                if (config->FsrShadingScale.value_for_config() == std::nullopt)
                    config->FsrShadingScale.set_volatile_value(0.5f / scaleRatio);

                if (config->FsrAccAddPerFrame.value_for_config() == std::nullopt)
                    config->FsrAccAddPerFrame.set_volatile_value(scaleRatio / 10.0f);

                if (config->FsrMinDisOccAcc.value_for_config() == std::nullopt)
                    config->FsrMinDisOccAcc.set_volatile_value(scaleRatio / 20.0f);
            }
        }

        ffxCreateBackendDX12Desc backendDesc = { 0 };
        backendDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_BACKEND_DX12;
        backendDesc.device = Device;

        _contextDesc.header.pNext = &backendDesc.header;

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FfxUpscalerIndex.value_or_default() < 0 ||
#else
        if (config.fsr31.ffxUpscalerIndex < 0 ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->FfxUpscalerIndex.value_or_default() >= State::Instance().ffxUpscalerVersionIds.size())
#else
            config.fsr31.ffxUpscalerIndex >= State::Instance().ffxUpscalerVersionIds.size())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->FfxUpscalerIndex.set_volatile_value(0);
#else
            Config::Instance()->FfxUpscalerIndex.set_volatile_value(0);
        config.fsr31.ffxUpscalerIndex = 0;
#endif

        ffxOverrideVersion override = { 0 };
        override.header.type = FFX_API_DESC_TYPE_OVERRIDE_VERSION;
        override.versionId =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().ffxUpscalerVersionIds[Config::Instance()->FfxUpscalerIndex.value_or_default()];
#else
            State::Instance().ffxUpscalerVersionIds[config.fsr31.ffxUpscalerIndex];
#endif
        backendDesc.header.pNext = &override.header;

        LOG_DEBUG("_createContext!");

        {
            ScopedSkipHeapCapture skipHeapCapture {};

            auto ret = FfxApiProxy::D3D12_CreateContext(&_context, &_contextDesc.header, NULL);

            if (ret != FFX_API_RETURN_OK)
            {
                LOG_ERROR("_createContext error: {0}", FfxApiProxy::ReturnCodeToString(ret));
                return false;
            }
        }

        auto version =
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().ffxUpscalerVersionNames[Config::Instance()->FfxUpscalerIndex.value_or_default()];
#else
            State::Instance().ffxUpscalerVersionNames[config.fsr31.ffxUpscalerIndex];
#endif
        _name = "FSR";
        parse_version(version);
    }

    SetInit(true);

    return true;
}
