#include <pch.h>
#include <Config.h>
#include <Util.h>

#include <proxies/FfxApi_Proxy.h>

#include "FSR31Feature_Dx12.h"
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
#include "../../runtime/RuntimeConfiguration.h"
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
NVSDK_NGX_Parameter* FSR31FeatureDx12::SetParameters(NVSDK_NGX_Parameter* InParameters)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
{
#else
NVSDK_NGX_Parameter* FSR31FeatureDx12::SetParameters(NVSDK_NGX_Parameter* InParameters)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    InParameters->Set("OptiScaler.SupportsUpscaleSize", true);
#else
{
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    return InParameters;
#else
    InParameters->Set("OptiScaler.SupportsUpscaleSize", true);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
}
#else
    return InParameters;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
}
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
FSR31FeatureDx12::FSR31FeatureDx12(unsigned int InHandleId, NVSDK_NGX_Parameter* InParameters)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    : FSR31Feature(InHandleId, InParameters), IFeature_Dx12(InHandleId, InParameters),
#else
FSR31FeatureDx12::FSR31FeatureDx12(unsigned int InHandleId, NVSDK_NGX_Parameter* InParameters)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
      IFeature(InHandleId, SetParameters(InParameters))
#else
    : FSR31Feature(InHandleId, InParameters), IFeature_Dx12(InHandleId, InParameters),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
{
#else
      IFeature(InHandleId, SetParameters(InParameters))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    FfxApiProxy::InitFfxDx12();
#else
{
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    FfxApiProxy::InitFfxDx12();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _moduleLoaded = FfxApiProxy::IsSRReady();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    _moduleLoaded = FfxApiProxy::IsSRReady();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (_moduleLoaded)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_INFO("amd_fidelityfx_dx12.dll methods loaded!");
#else
    if (_moduleLoaded)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else
        LOG_INFO("amd_fidelityfx_dx12.dll methods loaded!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("can't load amd_fidelityfx_dx12.dll methods!");
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
}
#else
        LOG_ERROR("can't load amd_fidelityfx_dx12.dll methods!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
}
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
bool FSR31FeatureDx12::Init(ID3D12Device* InDevice, ID3D12GraphicsCommandList* InCommandList,
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            NVSDK_NGX_Parameter* InParameters)
#else
bool FSR31FeatureDx12::Init(ID3D12Device* InDevice, ID3D12GraphicsCommandList* InCommandList,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
{
#else
                            NVSDK_NGX_Parameter* InParameters)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("FSR31FeatureDx12::Init");
#else
{
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    LOG_DEBUG("FSR31FeatureDx12::Init");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (IsInited())
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return true;
#else
    if (IsInited())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    Device = InDevice;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    Device = InDevice;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InitFSR3(InParameters))
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    if (InitFSR3(InParameters))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (!Config::Instance()->OverlayMenu.value_or_default() && (Imgui == nullptr || Imgui.get() == nullptr))
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Imgui = std::make_unique<Menu_Dx12>(Util::GetProcessWindow(), InDevice);
#else
        if (!config.fsr31.overlayMenu && (Imgui == nullptr || Imgui.get() == nullptr))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            Imgui = std::make_unique<Menu_Dx12>(Util::GetProcessWindow(), InDevice);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        OutputScaler = std::make_unique<OS_Dx12>("Output Scaling", InDevice, (TargetWidth() < DisplayWidth()));
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        RCAS = std::make_unique<RCAS_Dx12>("RCAS", InDevice);
#else
        OutputScaler = std::make_unique<OS_Dx12>("Output Scaling", InDevice, (TargetWidth() < DisplayWidth()));
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Bias = std::make_unique<Bias_Dx12>("Bias", InDevice);
#else
        RCAS = std::make_unique<RCAS_Dx12>("RCAS", InDevice);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        Bias = std::make_unique<Bias_Dx12>("Bias", InDevice);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return true;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    return false;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
}
#else
    return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
}
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
bool FSR31FeatureDx12::Evaluate(ID3D12GraphicsCommandList* InCommandList, NVSDK_NGX_Parameter* InParameters)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
{
#else
bool FSR31FeatureDx12::Evaluate(ID3D12GraphicsCommandList* InCommandList, NVSDK_NGX_Parameter* InParameters)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_FUNC();
#else
{
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    LOG_FUNC();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!IsInited())
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
    const auto config = RuntimeConfiguration::Instance().GetSnapshot();
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!RCAS->IsInit())
#else
    if (!IsInited())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
        return false;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!OutputScaler->IsInit())
#else
    if (!RCAS->IsInit())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
#else
        Config::Instance()->RcasEnabled.set_volatile_value(false); config.fsr31.rcasEnabled = false;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->DADepthIsLinear.value_for_config_ignore_default() == std::nullopt)
#else
    if (!OutputScaler->IsInit())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->DADepthIsLinear.set_volatile_value(false);
#else
        Config::Instance()->OutputScalingEnabled.set_volatile_value(false); config.fsr31.outputScalingEnabled = false;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    struct ffxDispatchDescUpscale params = { 0 };
#else
    if (Config::Instance()->DADepthIsLinear.value_for_config_ignore_default() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.header.type = FFX_API_DISPATCH_DESC_TYPE_UPSCALE;
#else
        Config::Instance()->DADepthIsLinear.set_volatile_value(false); config.fsr31.daDepthIsLinear = false;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.flags = 0;
#else
    struct ffxDispatchDescUpscale params = { 0 };
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    params.header.type = FFX_API_DISPATCH_DESC_TYPE_UPSCALE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->FsrDebugView.value_or_default() &&
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        (Version() < feature_version { 4, 0, 0 } || Config::Instance()->Fsr4EnableDebugView.value_or_default()))
#else
    params.flags = 0;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.flags |= FFX_UPSCALE_FLAG_DRAW_DEBUG_VIEW;
#else
    if (config.fsr31.debugView &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        (Version() < feature_version { 4, 0, 0 } || config.fsr31.enableDebugViewForFsr4))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->FsrNonLinearPQ.value_or_default())
#else
        params.flags |= FFX_UPSCALE_FLAG_DRAW_DEBUG_VIEW;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_PQ;
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else if (Config::Instance()->FsrNonLinearSRGB.value_or_default())
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_SRGB;
#else
    if (config.fsr31.nonLinearPQ)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_PQ;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_X, &params.jitterOffset.x);
#else
    else if (config.fsr31.nonLinearSRGB)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_Y, &params.jitterOffset.y);
#else
        params.flags |= FFX_UPSCALE_FLAG_NON_LINEAR_COLOR_SRGB;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->OverrideSharpness.value_or_default())
#else
    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_X, &params.jitterOffset.x);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _sharpness = Config::Instance()->Sharpness.value_or_default();
#else
    InParameters->Get(NVSDK_NGX_Parameter_Jitter_Offset_Y, &params.jitterOffset.y);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _sharpness = GetSharpness(InParameters);
#else
    if (config.fsr31.overrideSharpness)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        _sharpness = config.fsr31.sharpness;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->RcasEnabled.value_or_default())
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        _sharpness = GetSharpness(InParameters);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.enableSharpening = false;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.sharpness = 0.0f;
#else
    if (config.fsr31.rcasEnabled)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else
        params.enableSharpening = false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        params.sharpness = 0.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_sharpness > 1.0f)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _sharpness = 1.0f;
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.enableSharpening = _sharpness > 0.0f;
#else
        if (_sharpness > 1.0f)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.sharpness = _sharpness;
#else
            _sharpness = 1.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        params.enableSharpening = _sharpness > 0.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // Force enable RCAS when in FSR4 debug view mode
#else
        params.sharpness = _sharpness;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // it crashes when sharpening is disabled
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // Debug view expects RCAS output (now sure why)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version() >= feature_version { 4, 0, 2 } && Config::Instance()->FsrDebugView.value_or_default() &&
#else
    // Force enable RCAS when in FSR4 debug view mode
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->Fsr4EnableDebugView.value_or_default() && !params.enableSharpening)
#else
    // it crashes when sharpening is disabled
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    // Debug view expects RCAS output (now sure why)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.enableSharpening = true;
#else
    if (Version() >= feature_version { 4, 0, 2 } && config.fsr31.debugView &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.sharpness = 0.01f;
#else
        config.fsr31.enableDebugViewForFsr4 && !params.enableSharpening)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        params.enableSharpening = true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("Jitter Offset: {0}x{1}", params.jitterOffset.x, params.jitterOffset.y);
#else
        params.sharpness = 0.01f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    unsigned int reset;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    InParameters->Get(NVSDK_NGX_Parameter_Reset, &reset);
#else
    LOG_DEBUG("Jitter Offset: {0}x{1}", params.jitterOffset.x, params.jitterOffset.y);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.reset = (reset == 1);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    unsigned int reset;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    GetRenderResolution(InParameters, &params.renderSize.width, &params.renderSize.height);
#else
    InParameters->Get(NVSDK_NGX_Parameter_Reset, &reset);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    params.reset = (reset == 1);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    bool useSS =
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default() && (LowResMV() || RenderWidth() == DisplayWidth());
#else
    GetRenderResolution(InParameters, &params.renderSize.width, &params.renderSize.height);
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("Input Resolution: {0}x{1}", params.renderSize.width, params.renderSize.height);
#else
    bool useSS =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        config.fsr31.outputScalingEnabled && (LowResMV() || RenderWidth() == DisplayWidth());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.commandList = InCommandList;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    LOG_DEBUG("Input Resolution: {0}x{1}", params.renderSize.width, params.renderSize.height);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramColor;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_Color, &paramColor) != NVSDK_NGX_Result_Success)
#else
    params.commandList = InCommandList;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_Color, (void**) &paramColor);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramColor;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramColor)
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_Color, &paramColor) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        InParameters->Get(NVSDK_NGX_Parameter_Color, (void**) &paramColor);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("Color exist..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramColor)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->ColorResourceBarrier.has_value())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        LOG_DEBUG("Color exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramColor,
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->ColorResourceBarrier.value(),
#else
        if (config.fsr31.colorResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            ResourceBarrier(InCommandList, paramColor,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.colorResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                 State::Instance().GameEngine == GameEngineType::Unreal ||
#else
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->ColorResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_RENDER_TARGET);
#else
                 State::Instance().GameEngine == GameEngineType::Unreal ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_RENDER_TARGET,
#else
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            Config::Instance()->ColorResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_RENDER_TARGET); config.fsr31.colorResourceBarrier = D3D12_RESOURCE_STATE_RENDER_TARGET;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_RENDER_TARGET,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.color = ffxApiGetResourceDX12(paramColor, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        params.color = ffxApiGetResourceDX12(paramColor, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("Color not exist!!");
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        LOG_ERROR("Color not exist!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramVelocity;
#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, &paramVelocity) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, (void**) &paramVelocity);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramVelocity;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramVelocity)
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, &paramVelocity) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        InParameters->Get(NVSDK_NGX_Parameter_MotionVectors, (void**) &paramVelocity);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("MotionVectors exist..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramVelocity)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->MVResourceBarrier.has_value())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        LOG_DEBUG("MotionVectors exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramVelocity,
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->MVResourceBarrier.value(),
#else
        if (config.fsr31.mvResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            ResourceBarrier(InCommandList, paramVelocity,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.mvResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                 State::Instance().GameEngine == GameEngineType::Unreal ||
#else
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        else if (State::Instance().NVNGX_Engine == NVSDK_NGX_ENGINE_TYPE_UNREAL ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->MVResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                 State::Instance().GameEngine == GameEngineType::Unreal ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
#else
                 State::Instance().gameQuirks & GameQuirk::ForceUnrealEngine)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            Config::Instance()->MVResourceBarrier.set_volatile_value(D3D12_RESOURCE_STATE_UNORDERED_ACCESS); config.fsr31.mvResourceBarrier = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.motionVectors = ffxApiGetResourceDX12(paramVelocity, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        params.motionVectors = ffxApiGetResourceDX12(paramVelocity, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("MotionVectors not exist!!");
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        LOG_ERROR("MotionVectors not exist!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramOutput;
#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_Output, &paramOutput) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_Output, (void**) &paramOutput);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramOutput;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramOutput)
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_Output, &paramOutput) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        InParameters->Get(NVSDK_NGX_Parameter_Output, (void**) &paramOutput);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("Output exist..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramOutput)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->OutputResourceBarrier.has_value())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramOutput,
#else
        LOG_DEBUG("Output exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->OutputResourceBarrier.value(),
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
        if (config.fsr31.outputResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            ResourceBarrier(InCommandList, paramOutput,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (useSS)
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.outputResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (OutputScaler->CreateBufferResource(Device, paramOutput, TargetWidth(), TargetHeight(),
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                                   D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
#else
        if (useSS)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
            if (OutputScaler->CreateBufferResource(Device, paramOutput, TargetWidth(), TargetHeight(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                params.output = ffxApiGetResourceDX12(OutputScaler->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                                                   D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            else
#else
                OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                params.output = ffxApiGetResourceDX12(OutputScaler->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->RcasEnabled.value_or_default() &&
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            (_sharpness > 0.0f || (Config::Instance()->MotionSharpnessEnabled.value_or_default() &&
#else
            params.output = ffxApiGetResourceDX12(paramOutput, FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                   Config::Instance()->MotionSharpness.value_or_default() > 0.0f)) &&
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            RCAS->IsInit() &&
#else
        if (config.fsr31.rcasEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            RCAS->CreateBufferResource(Device, (ID3D12Resource*) params.output.resource,
#else
            (_sharpness > 0.0f || (config.fsr31.motionSharpnessEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                       D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
#else
                                   config.fsr31.motionSharpness > 0.0f)) &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            RCAS->IsInit() &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
            RCAS->CreateBufferResource(Device, (ID3D12Resource*) params.output.resource,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.output = ffxApiGetResourceDX12(RCAS->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                                       D3D12_RESOURCE_STATE_UNORDERED_ACCESS))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else
            params.output = ffxApiGetResourceDX12(RCAS->Buffer(), FFX_API_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("Output not exist!!");
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        LOG_ERROR("Output not exist!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramDepth;
#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_Depth, &paramDepth) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_Depth, (void**) &paramDepth);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramDepth;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramDepth)
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_Depth, &paramDepth) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        InParameters->Get(NVSDK_NGX_Parameter_Depth, (void**) &paramDepth);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("Depth exist..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramDepth)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->DepthResourceBarrier.has_value())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, paramDepth,
#else
        LOG_DEBUG("Depth exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (D3D12_RESOURCE_STATES) Config::Instance()->DepthResourceBarrier.value(),
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
        if (config.fsr31.depthResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            ResourceBarrier(InCommandList, paramDepth,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.depth = ffxApiGetResourceDX12(paramDepth, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
                            (D3D12_RESOURCE_STATES) config.fsr31.depthResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
                            D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        params.depth = ffxApiGetResourceDX12(paramDepth, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("Depth not exist!!");
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (LowResMV())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            return false;
#else
        LOG_ERROR("Depth not exist!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (LowResMV())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramExp = nullptr;
#else
            return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (AutoExposure())
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("AutoExposure enabled!");
#else
    ID3D12Resource* paramExp = nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    if (AutoExposure())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        LOG_DEBUG("AutoExposure enabled!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, &paramExp) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, (void**) &paramExp);
#else
    else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (paramExp)
#else
        if (InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, &paramExp) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            InParameters->Get(NVSDK_NGX_Parameter_ExposureTexture, (void**) &paramExp);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_DEBUG("ExposureTexture exist..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (paramExp)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Config::Instance()->ExposureResourceBarrier.has_value())
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                ResourceBarrier(InCommandList, paramExp,
#else
            LOG_DEBUG("ExposureTexture exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (D3D12_RESOURCE_STATES) Config::Instance()->ExposureResourceBarrier.value(),
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
            if (config.fsr31.exposureResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                ResourceBarrier(InCommandList, paramExp,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.exposure = ffxApiGetResourceDX12(paramExp, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
                                (D3D12_RESOURCE_STATES) config.fsr31.exposureResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                                D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            params.exposure = ffxApiGetResourceDX12(paramExp, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_DEBUG("AutoExposure disabled but ExposureTexture is not exist, it may cause problems!!");
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().AutoExposure = true;
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().changeBackend[Handle()->Id] = true;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            return true;
#else
            LOG_DEBUG("AutoExposure disabled but ExposureTexture is not exist, it may cause problems!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            State::Instance().AutoExposure = true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            State::Instance().changeBackend[Handle()->Id] = true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramTransparency = nullptr;
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get("FSR.transparencyAndComposition", &paramTransparency) == NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.transparencyAndComposition", (void**) &paramTransparency);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramTransparency = nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramReactiveMask = nullptr;
#else
    if (InParameters->Get("FSR.transparencyAndComposition", &paramTransparency) == NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get("FSR.reactive", &paramReactiveMask) == NVSDK_NGX_Result_Success)
#else
        InParameters->Get("FSR.transparencyAndComposition", (void**) &paramTransparency);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.reactive", (void**) &paramReactiveMask);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    ID3D12Resource* paramReactiveMask = nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    ID3D12Resource* paramReactiveMask2 = nullptr;
#else
    if (InParameters->Get("FSR.reactive", &paramReactiveMask) == NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, &paramReactiveMask2) !=
#else
        InParameters->Get("FSR.reactive", (void**) &paramReactiveMask);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        NVSDK_NGX_Result_Success)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, (void**) &paramReactiveMask2);
#else
    ID3D12Resource* paramReactiveMask2 = nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, &paramReactiveMask2) !=
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->DisableReactiveMask.value_or(paramReactiveMask == nullptr &&
#else
        NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                                          paramReactiveMask2 == nullptr))
#else
        InParameters->Get(NVSDK_NGX_Parameter_DLSS_Input_Bias_Current_Color_Mask, (void**) &paramReactiveMask2);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (paramTransparency != nullptr)
#else
    if (!Config::Instance()->DisableReactiveMask.value_or(paramReactiveMask == nullptr &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
                                                          paramReactiveMask2 == nullptr))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_DEBUG("Using FSR transparency mask..");
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.transparencyAndComposition =
#else
        if (paramTransparency != nullptr)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                ffxApiGetResourceDX12(paramTransparency, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            LOG_DEBUG("Using FSR transparency mask..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            params.transparencyAndComposition =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (paramReactiveMask != nullptr)
#else
                ffxApiGetResourceDX12(paramTransparency, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_DEBUG("Using FSR reactive mask..");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.reactive = ffxApiGetResourceDX12(paramReactiveMask, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
        if (paramReactiveMask != nullptr)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            LOG_DEBUG("Using FSR reactive mask..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            params.reactive = ffxApiGetResourceDX12(paramReactiveMask, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (paramReactiveMask2 != nullptr)
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_DEBUG("Input Bias mask exist..");
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->DisableReactiveMask.set_volatile_value(false);
#else
            if (paramReactiveMask2 != nullptr)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (Config::Instance()->MaskResourceBarrier.has_value())
#else
                LOG_DEBUG("Input Bias mask exist..");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    ResourceBarrier(InCommandList, paramReactiveMask2,
#else
                Config::Instance()->DisableReactiveMask.set_volatile_value(false);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                    (D3D12_RESOURCE_STATES) Config::Instance()->MaskResourceBarrier.value(),
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                    D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
                if (config.fsr31.maskResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                    ResourceBarrier(InCommandList, paramReactiveMask2,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (paramTransparency == nullptr && Config::Instance()->FsrUseMaskForTransparency.value_or_default())
#else
                                    (D3D12_RESOURCE_STATES) config.fsr31.maskResourceBarrier.value(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    params.transparencyAndComposition =
#else
                                    D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        ffxApiGetResourceDX12(paramReactiveMask2, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                if (paramTransparency == nullptr && Config::Instance()->FsrUseMaskForTransparency.value_or_default())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (Config::Instance()->DlssReactiveMaskBias.value_or_default() > 0.0f && Bias->IsInit() &&
#else
                    params.transparencyAndComposition =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    Bias->CreateBufferResource(Device, paramReactiveMask2, D3D12_RESOURCE_STATE_UNORDERED_ACCESS) &&
#else
                        ffxApiGetResourceDX12(paramReactiveMask2, FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    Bias->CanRender())
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                {
#else
                if (Config::Instance()->DlssReactiveMaskBias.value_or_default() > 0.0f && Bias->IsInit() &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#else
                    Bias->CreateBufferResource(Device, paramReactiveMask2, D3D12_RESOURCE_STATE_UNORDERED_ACCESS) &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                    Bias->CanRender())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    if (Bias->Dispatch(Device, InCommandList, paramReactiveMask2,
#else
                {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                       Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->Buffer()))
#else
                    Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
                    if (Bias->Dispatch(Device, InCommandList, paramReactiveMask2,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        params.reactive = ffxApiGetResourceDX12(Bias->Buffer(), FFX_API_RESOURCE_STATE_COMPUTE_READ);
#else
                                       Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->Buffer()))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    }
#else
                    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                }
#else
                        Bias->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                else
#else
                        params.reactive = ffxApiGetResourceDX12(Bias->Buffer(), FFX_API_RESOURCE_STATE_COMPUTE_READ);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                {
#else
                    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    LOG_DEBUG("Skipping reactive mask, Bias: {0}, Bias Init: {1}, Bias CanRender: {2}",
#else
                }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                              Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->IsInit(),
#else
                else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                              Bias->CanRender());
#else
                {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                }
#else
                    LOG_DEBUG("Skipping reactive mask, Bias: {0}, Bias Init: {1}, Bias CanRender: {2}",
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
                              Config::Instance()->DlssReactiveMaskBias.value_or_default(), Bias->IsInit(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                              Bias->CanRender());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
                }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasColor = params.color.resource != nullptr;
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasDepth = params.depth.resource != nullptr;
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasMV = params.motionVectors.resource != nullptr;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasExposure = params.exposure.resource != nullptr;
#else
    _hasColor = params.color.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasTM = params.transparencyAndComposition.resource != nullptr;
#else
    _hasDepth = params.depth.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _accessToReactiveMask = paramReactiveMask != nullptr;
#else
    _hasMV = params.motionVectors.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _hasOutput = params.output.resource != nullptr;
#else
    _hasExposure = params.exposure.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    _hasTM = params.transparencyAndComposition.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // For FSR 4 as it seems to be missing some conversions from typeless
#else
    _accessToReactiveMask = paramReactiveMask != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // transparencyAndComposition and exposure might be unnecessary here
#else
    _hasOutput = params.output.resource != nullptr;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version().major >= 4)
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    // For FSR 4 as it seems to be missing some conversions from typeless
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.color.description.format = ffxResolveTypelessFormat(params.color.description.format);
#else
    // transparencyAndComposition and exposure might be unnecessary here
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.depth.description.format = ffxResolveTypelessFormat(params.depth.description.format);
#else
    if (Version().major >= 4)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.motionVectors.description.format = ffxResolveTypelessFormat(params.motionVectors.description.format);
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.exposure.description.format = ffxResolveTypelessFormat(params.exposure.description.format);
#else
        params.color.description.format = ffxResolveTypelessFormat(params.color.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.transparencyAndComposition.description.format =
#else
        params.depth.description.format = ffxResolveTypelessFormat(params.depth.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ffxResolveTypelessFormat(params.transparencyAndComposition.description.format);
#else
        params.motionVectors.description.format = ffxResolveTypelessFormat(params.motionVectors.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.output.description.format = ffxResolveTypelessFormat(params.output.description.format);
#else
        params.exposure.description.format = ffxResolveTypelessFormat(params.exposure.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        params.transparencyAndComposition.description.format =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            ffxResolveTypelessFormat(params.transparencyAndComposition.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.motionVectorScale.x = 1.0f;
#else
        params.output.description.format = ffxResolveTypelessFormat(params.output.description.format);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    params.motionVectorScale.y = 1.0f;
#else
    }
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &params.motionVectorScale.x) != NVSDK_NGX_Result_Success ||
#else
    params.motionVectorScale.x = 1.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &params.motionVectorScale.y) != NVSDK_NGX_Result_Success)
#else
    params.motionVectorScale.y = 1.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_WARN("Can't get motion vector scales!");
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &params.motionVectorScale.x) != NVSDK_NGX_Result_Success ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &params.motionVectorScale.y) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("Sharpness: {0}", params.sharpness);
#else
        LOG_WARN("Can't get motion vector scales!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.cameraNear", &params.cameraNear) != NVSDK_NGX_Result_Success)
#else
    LOG_DEBUG("Sharpness: {0}", params.sharpness);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (DepthInverted())
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFar = Config::Instance()->FsrCameraNear.value_or_default();
#else
        InParameters->Get("FSR.cameraNear", &params.cameraNear) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraNear = Config::Instance()->FsrCameraNear.value_or_default();
#else
        if (DepthInverted())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            params.cameraFar = config.fsr31.cameraNear;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
            params.cameraNear = config.fsr31.cameraNear;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.cameraFar", &params.cameraFar) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (DepthInverted())
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraNear = Config::Instance()->FsrCameraFar.value_or_default();
#else
        InParameters->Get("FSR.cameraFar", &params.cameraFar) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFar = Config::Instance()->FsrCameraFar.value_or_default();
#else
        if (DepthInverted())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            params.cameraNear = config.fsr31.cameraFar;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
            params.cameraFar = config.fsr31.cameraFar;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.cameraFovAngleVertical", &params.cameraFovAngleVertical) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FsrVerticalFov.has_value())
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFovAngleVertical = Config::Instance()->FsrVerticalFov.value() * 0.0174532925199433f;
#else
        InParameters->Get("FSR.cameraFovAngleVertical", &params.cameraFovAngleVertical) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else if (Config::Instance()->FsrHorizontalFov.value_or_default() > 0.0f)
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFovAngleVertical =
#else
        if (config.fsr31.verticalFov.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                2.0f * atan((tan(Config::Instance()->FsrHorizontalFov.value() * 0.0174532925199433f) * 0.5f) /
#else
            params.cameraFovAngleVertical = config.fsr31.verticalFov.value() * 0.0174532925199433f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            (float) TargetHeight() * (float) TargetWidth());
#else
        else if (config.fsr31.horizontalFov > 0.0f)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            params.cameraFovAngleVertical =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.cameraFovAngleVertical = 1.0471975511966f;
#else
                2.0f * atan((tan(config.fsr31.horizontalFov * 0.0174532925199433f) * 0.5f) /
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
                            (float) TargetHeight() * (float) TargetWidth());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else
            params.cameraFovAngleVertical = 1.0471975511966f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.frameTimeDelta", &params.frameTimeDelta) != NVSDK_NGX_Result_Success)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (InParameters->Get(NVSDK_NGX_Parameter_FrameTimeDeltaInMsec, &params.frameTimeDelta) !=
#else
    if (!config.fsr31.useFsrInputValues ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                NVSDK_NGX_Result_Success ||
#else
        InParameters->Get("FSR.frameTimeDelta", &params.frameTimeDelta) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.frameTimeDelta < 1.0f)
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            params.frameTimeDelta = (float) GetDeltaTime();
#else
        if (InParameters->Get(NVSDK_NGX_Parameter_FrameTimeDeltaInMsec, &params.frameTimeDelta) !=
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
                NVSDK_NGX_Result_Success ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            params.frameTimeDelta < 1.0f)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("FrameTimeDeltaInMsec: {0}", params.frameTimeDelta);
#else
            params.frameTimeDelta = (float) GetDeltaTime();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->FsrUseFsrInputValues.value_or_default() ||
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get("FSR.viewSpaceToMetersFactor", &params.viewSpaceToMetersFactor) != NVSDK_NGX_Result_Success)
#else
    LOG_DEBUG("FrameTimeDeltaInMsec: {0}", params.frameTimeDelta);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.viewSpaceToMetersFactor = 0.0f;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (!config.fsr31.useFsrInputValues ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Pre_Exposure, &params.preExposure) != NVSDK_NGX_Result_Success)
#else
        InParameters->Get("FSR.viewSpaceToMetersFactor", &params.viewSpaceToMetersFactor) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.preExposure = 1.0f;
#else
        params.viewSpaceToMetersFactor = 0.0f;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version() >= feature_version { 3, 1, 1 } && _velocity != Config::Instance()->FsrVelocity.value_or_default())
#else
    if (InParameters->Get(NVSDK_NGX_Parameter_DLSS_Pre_Exposure, &params.preExposure) != NVSDK_NGX_Result_Success)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        params.preExposure = 1.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _velocity = Config::Instance()->FsrVelocity.value_or_default();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#else
    if (Version() >= feature_version { 3, 1, 1 } && _velocity != config.fsr31.velocity)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FVELOCITYFACTOR;
#else
        _velocity = config.fsr31.velocity;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        m_upscalerKeyValueConfig.ptr = &_velocity;
#else
        ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#else
        m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FVELOCITYFACTOR;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (result != FFX_API_RETURN_OK)
#else
        m_upscalerKeyValueConfig.ptr = &_velocity;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_WARN("Velocity configure result: {}", (UINT) result);
#else
        auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Version() >= feature_version { 3, 1, 4 })
#else
            LOG_WARN("Velocity configure result: {}", (UINT) result);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_reactiveScale != Config::Instance()->FsrReactiveScale.value_or_default())
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
    if (Version() >= feature_version { 3, 1, 4 })
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _reactiveScale = Config::Instance()->FsrReactiveScale.value_or_default();
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#else
        if (_reactiveScale != config.fsr31.reactiveScale)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FREACTIVENESSSCALE;
#else
            _reactiveScale = config.fsr31.reactiveScale;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.ptr = &_reactiveScale;
#else
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#else
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FREACTIVENESSSCALE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (result != FFX_API_RETURN_OK)
#else
            m_upscalerKeyValueConfig.ptr = &_reactiveScale;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_WARN("Reactive Scale configure result: {}", (UINT) result);
#else
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_shadingScale != Config::Instance()->FsrShadingScale.value_or_default())
#else
                LOG_WARN("Reactive Scale configure result: {}", (UINT) result);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _shadingScale = Config::Instance()->FsrShadingScale.value_or_default();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#else
        if (_shadingScale != config.fsr31.shadingScale)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FSHADINGCHANGESCALE;
#else
            _shadingScale = config.fsr31.shadingScale;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.ptr = &_shadingScale;
#else
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#else
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FSHADINGCHANGESCALE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (result != FFX_API_RETURN_OK)
#else
            m_upscalerKeyValueConfig.ptr = &_shadingScale;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_WARN("Shading Scale configure result: {}", (UINT) result);
#else
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_accAddPerFrame != Config::Instance()->FsrAccAddPerFrame.value_or_default())
#else
                LOG_WARN("Shading Scale configure result: {}", (UINT) result);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _accAddPerFrame = Config::Instance()->FsrAccAddPerFrame.value_or_default();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#else
        if (_accAddPerFrame != config.fsr31.accAddPerFrame)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FACCUMULATIONADDEDPERFRAME;
#else
            _accAddPerFrame = config.fsr31.accAddPerFrame;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.ptr = &_accAddPerFrame;
#else
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#else
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FACCUMULATIONADDEDPERFRAME;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (result != FFX_API_RETURN_OK)
#else
            m_upscalerKeyValueConfig.ptr = &_accAddPerFrame;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_WARN("Acc. Add Per Frame configure result: {}", (UINT) result);
#else
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (_minDisOccAcc != Config::Instance()->FsrMinDisOccAcc.value_or_default())
#else
                LOG_WARN("Acc. Add Per Frame configure result: {}", (UINT) result);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _minDisOccAcc = Config::Instance()->FsrMinDisOccAcc.value_or_default();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#else
        if (_minDisOccAcc != config.fsr31.minDisOccAcc)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FMINDISOCCLUSIONACCUMULATION;
#else
            _minDisOccAcc = config.fsr31.minDisOccAcc;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            m_upscalerKeyValueConfig.ptr = &_minDisOccAcc;
#else
            ffxConfigureDescUpscaleKeyValue m_upscalerKeyValueConfig {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#else
            m_upscalerKeyValueConfig.header.type = FFX_API_CONFIGURE_DESC_TYPE_UPSCALE_KEYVALUE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            m_upscalerKeyValueConfig.key = FFX_API_CONFIGURE_UPSCALE_KEY_FMINDISOCCLUSIONACCUMULATION;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (result != FFX_API_RETURN_OK)
#else
            m_upscalerKeyValueConfig.ptr = &_minDisOccAcc;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_WARN("Minimum Disocclusion Acc. configure result: {}", (UINT) result);
#else
            auto result = FfxApiProxy::D3D12_Configure(&_context, &m_upscalerKeyValueConfig.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                LOG_WARN("Minimum Disocclusion Acc. configure result: {}", (UINT) result);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get("FSR.upscaleSize.width", &params.upscaleSize.width) == NVSDK_NGX_Result_Success &&
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default())
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        auto originalWidth = static_cast<float>(params.upscaleSize.width);
#else
    if (InParameters->Get("FSR.upscaleSize.width", &params.upscaleSize.width) == NVSDK_NGX_Result_Success &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.upscaleSize.width =
#else
        config.fsr31.outputScalingEnabled)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            static_cast<uint32_t>(originalWidth * Config::Instance()->OutputScalingMultiplier.value_or_default());
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        auto originalWidth = static_cast<float>(params.upscaleSize.width);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else if (params.upscaleSize.width == 0)
#else
        params.upscaleSize.width =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
            static_cast<uint32_t>(originalWidth * config.fsr31.outputScalingMultiplier);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.upscaleSize.width = TargetWidth();
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    else if (params.upscaleSize.width == 0)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (InParameters->Get("FSR.upscaleSize.height", &params.upscaleSize.height) == NVSDK_NGX_Result_Success &&
#else
        params.upscaleSize.width = TargetWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        Config::Instance()->OutputScalingEnabled.value_or_default())
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        auto originalHeight = static_cast<float>(params.upscaleSize.height);
#else
    if (InParameters->Get("FSR.upscaleSize.height", &params.upscaleSize.height) == NVSDK_NGX_Result_Success &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.upscaleSize.height =
#else
        config.fsr31.outputScalingEnabled)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            static_cast<uint32_t>(originalHeight * Config::Instance()->OutputScalingMultiplier.value_or_default());
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        auto originalHeight = static_cast<float>(params.upscaleSize.height);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    else if (params.upscaleSize.height == 0)
#else
        params.upscaleSize.height =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
            static_cast<uint32_t>(originalHeight * config.fsr31.outputScalingMultiplier);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        params.upscaleSize.height = TargetHeight();
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
    else if (params.upscaleSize.height == 0)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_DEBUG("Dispatch!!");
#else
        params.upscaleSize.height = TargetHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    auto result = FfxApiProxy::D3D12_Dispatch(&_context, &params.header);
#else
    }
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (result != FFX_API_RETURN_OK)
#else
    LOG_DEBUG("Dispatch!!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    auto result = FfxApiProxy::D3D12_Dispatch(&_context, &params.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("_dispatch error: {0}", FfxApiProxy::ReturnCodeToString(result));
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (result != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (result == FFX_API_RETURN_ERROR_RUNTIME_ERROR)
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        LOG_ERROR("_dispatch error: {0}", FfxApiProxy::ReturnCodeToString(result));
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_WARN("Trying to recover by recreating the feature");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().changeBackend[Handle()->Id] = true;
#else
        if (result == FFX_API_RETURN_ERROR_RUNTIME_ERROR)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            LOG_WARN("Trying to recover by recreating the feature");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
            State::Instance().changeBackend[Handle()->Id] = true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
        }
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // apply rcas
#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Config::Instance()->RcasEnabled.value_or_default() &&
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        (_sharpness > 0.0f || (Config::Instance()->MotionSharpnessEnabled.value_or_default() &&
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                               Config::Instance()->MotionSharpness.value_or_default() > 0.0f)) &&
#else
    // apply rcas
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        RCAS->CanRender())
#else
    if (config.fsr31.rcasEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
        (_sharpness > 0.0f || (config.fsr31.motionSharpnessEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (params.output.resource != RCAS->Buffer())
#else
                               config.fsr31.motionSharpness > 0.0f)) &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ResourceBarrier(InCommandList, (ID3D12Resource*) params.output.resource,
#else
        RCAS->CanRender())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (params.output.resource != RCAS->Buffer())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
            ResourceBarrier(InCommandList, (ID3D12Resource*) params.output.resource,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                            D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        RcasConstants rcasConstants {};
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        RCAS->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        rcasConstants.Sharpness = _sharpness;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &rcasConstants.MvScaleX);
#else
        RcasConstants rcasConstants {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &rcasConstants.MvScaleY);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        rcasConstants.Sharpness = _sharpness;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (DepthInverted())
#else
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_X, &rcasConstants.MvScaleX);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        InParameters->Get(NVSDK_NGX_Parameter_MV_Scale_Y, &rcasConstants.MvScaleY);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            rcasConstants.CameraNear = params.cameraFar;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            rcasConstants.CameraFar = params.cameraNear;
#else
        if (DepthInverted())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            rcasConstants.CameraNear = params.cameraFar;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            rcasConstants.CameraFar = params.cameraNear;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            rcasConstants.CameraNear = params.cameraNear;
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            rcasConstants.CameraFar = params.cameraFar;
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            rcasConstants.CameraNear = params.cameraNear;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (useSS)
#else
            rcasConstants.CameraFar = params.cameraFar;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, OutputScaler->Buffer(),
#else
        if (useSS)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (ID3D12Resource*) params.depth.resource))
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, OutputScaler->Buffer(),
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                return true;
#else
                                (ID3D12Resource*) params.depth.resource))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                Config::Instance()->RcasEnabled.set_volatile_value(false); config.fsr31.rcasEnabled = false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
                return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, paramOutput,
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                                (ID3D12Resource*) params.depth.resource))
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            if (!RCAS->Dispatch(Device, InCommandList, (ID3D12Resource*) params.output.resource,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->RcasEnabled.set_volatile_value(false);
#else
                                (ID3D12Resource*) params.motionVectors.resource, rcasConstants, paramOutput,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                return true;
#else
                                (ID3D12Resource*) params.depth.resource))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                Config::Instance()->RcasEnabled.set_volatile_value(false); config.fsr31.rcasEnabled = false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
                return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (useSS)
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("scaling output...");
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#else
    if (useSS)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (!OutputScaler->Dispatch(Device, InCommandList, OutputScaler->Buffer(), paramOutput))
#else
        LOG_DEBUG("scaling output...");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
        OutputScaler->SetBufferState(InCommandList, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->OutputScalingEnabled.set_volatile_value(false);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().changeBackend[Handle()->Id] = true;
#else
        if (!OutputScaler->Dispatch(Device, InCommandList, OutputScaler->Buffer(), paramOutput))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            return true;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            Config::Instance()->OutputScalingEnabled.set_volatile_value(false); config.fsr31.outputScalingEnabled = false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            State::Instance().changeBackend[Handle()->Id] = true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // imgui
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!Config::Instance()->OverlayMenu.value_or_default() && _frameCount > 30)
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Imgui != nullptr && Imgui.get() != nullptr)
#else
    // imgui
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
    if (!config.fsr31.overlayMenu && _frameCount > 30)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Imgui->IsHandleDifferent())
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
        if (Imgui != nullptr && Imgui.get() != nullptr)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Imgui.reset();
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
            if (Imgui->IsHandleDifferent())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            else
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Imgui->Render(InCommandList, paramOutput);
#else
                Imgui.reset();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
                Imgui->Render(InCommandList, paramOutput);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Imgui == nullptr || Imgui.get() == nullptr)
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Imgui = std::make_unique<Menu_Dx12>(GetForegroundWindow(), Device);
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else
            if (Imgui == nullptr || Imgui.get() == nullptr)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                Imgui = std::make_unique<Menu_Dx12>(GetForegroundWindow(), Device);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    // restore resource states
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramColor && Config::Instance()->ColorResourceBarrier.has_value())
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->ColorResourceBarrier.value());
#else
    // restore resource states
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramColor && config.fsr31.colorResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramVelocity && Config::Instance()->MVResourceBarrier.has_value())
#else
        ResourceBarrier(InCommandList, paramColor, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.colorResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->MVResourceBarrier.value());
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramVelocity && config.fsr31.mvResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramOutput && Config::Instance()->OutputResourceBarrier.has_value())
#else
        ResourceBarrier(InCommandList, paramVelocity, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.mvResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->OutputResourceBarrier.value());
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramOutput && config.fsr31.outputResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramDepth && Config::Instance()->DepthResourceBarrier.has_value())
#else
        ResourceBarrier(InCommandList, paramOutput, D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramDepth, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.outputResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->DepthResourceBarrier.value());
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramDepth && config.fsr31.depthResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramExp && Config::Instance()->ExposureResourceBarrier.has_value())
#else
        ResourceBarrier(InCommandList, paramDepth, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramExp, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.depthResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->ExposureResourceBarrier.value());
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramExp && config.fsr31.exposureResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (paramReactiveMask && Config::Instance()->MaskResourceBarrier.has_value())
#else
        ResourceBarrier(InCommandList, paramExp, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ResourceBarrier(InCommandList, paramReactiveMask, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#else
                        (D3D12_RESOURCE_STATES) config.fsr31.exposureResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                        (D3D12_RESOURCE_STATES) Config::Instance()->MaskResourceBarrier.value());
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (paramReactiveMask && config.fsr31.maskResourceBarrier.has_value())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    _frameCount++;
#else
        ResourceBarrier(InCommandList, paramReactiveMask, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE,
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                        (D3D12_RESOURCE_STATES) config.fsr31.maskResourceBarrier.value());
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    return true;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
}
#else
    _frameCount++;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
bool FSR31FeatureDx12::InitFSR3(const NVSDK_NGX_Parameter* InParameters)
#else
    return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
{
#else
}
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    LOG_FUNC();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
bool FSR31FeatureDx12::InitFSR3(const NVSDK_NGX_Parameter* InParameters)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (!ModuleLoaded())
#else
{
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
    LOG_FUNC();
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (IsInited())
#else
    const auto config = RuntimeConfiguration::Instance().GetSnapshot();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return true;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (!ModuleLoaded())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    if (Device == nullptr)
#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_ERROR("D3D12Device is null!");
#else
    if (IsInited())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        return false;
#else
        return true;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
    if (Device == nullptr)
#endif
    {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ScopedSkipSpoofing skipSpoofing {};
#else
        LOG_ERROR("D3D12Device is null!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ffxQueryDescGetVersions versionQuery {};
#else
    }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.header.type = FFX_API_QUERY_DESC_TYPE_GET_VERSIONS;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.createDescType = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
#else
    {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.device = Device; // only for DirectX 12 applications
#else
        ScopedSkipSpoofing skipSpoofing {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        uint64_t versionCount = 0;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.outputCount = &versionCount;
#else
        ffxQueryDescGetVersions versionQuery {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        // get number of versions for allocation
#else
        versionQuery.header.type = FFX_API_QUERY_DESC_TYPE_GET_VERSIONS;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);
#else
        versionQuery.createDescType = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        versionQuery.device = Device; // only for DirectX 12 applications
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        State::Instance().ffxUpscalerVersionIds.resize(versionCount);
#else
        uint64_t versionCount = 0;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        State::Instance().ffxUpscalerVersionNames.resize(versionCount);
#else
        versionQuery.outputCount = &versionCount;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.versionIds = State::Instance().ffxUpscalerVersionIds.data();
#else
        // get number of versions for allocation
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        versionQuery.versionNames = State::Instance().ffxUpscalerVersionNames.data();
#else
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        // fill version ids and names arrays.
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);
#else
        State::Instance().ffxUpscalerVersionIds.resize(versionCount);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        State::Instance().ffxUpscalerVersionNames.resize(versionCount);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _contextDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
#else
        versionQuery.versionIds = State::Instance().ffxUpscalerVersionIds.data();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        versionQuery.versionNames = State::Instance().ffxUpscalerVersionNames.data();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _contextDesc.flags = 0;
#else
        // fill version ids and names arrays.
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        FfxApiProxy::D3D12_Query(nullptr, &versionQuery.header);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
#ifdef _DEBUG
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_INFO("Debug checking enabled!");
#else
        _contextDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_UPSCALE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _contextDesc.fpMessage = FfxLogCallback;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEBUG_CHECKING;
#else
        _contextDesc.flags = 0;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
#endif
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
#ifdef _DEBUG
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (DepthInverted())
#else
        LOG_INFO("Debug checking enabled!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEPTH_INVERTED;
#else
        _contextDesc.fpMessage = FfxLogCallback;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEBUG_CHECKING;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (AutoExposure())
#else
#endif
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_AUTO_EXPOSURE;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (DepthInverted())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (IsHdr())
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DEPTH_INVERTED;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_HIGH_DYNAMIC_RANGE;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (AutoExposure())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (JitteredMV())
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_AUTO_EXPOSURE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (IsHdr())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (!LowResMV())
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_HIGH_DYNAMIC_RANGE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (JitteredMV())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FsrNonLinearColorSpace.value_or_default())
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_MOTION_VECTORS_JITTER_CANCELLATION;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_NON_LINEAR_COLORSPACE;
#else
        if (!LowResMV())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_INFO("contextDesc.initFlags (NonLinearColorSpace) {0:b}", _contextDesc.flags);
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_DISPLAY_RESOLUTION_MOTION_VECTORS;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (config.fsr31.nonLinearColorSpace)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->Fsr4EnableDebugView.value_or_default())
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            _contextDesc.flags |= FFX_UPSCALE_ENABLE_NON_LINEAR_COLORSPACE;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            LOG_INFO("Debug view enabled!");
#else
            LOG_INFO("contextDesc.initFlags (NonLinearColorSpace) {0:b}", _contextDesc.flags);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.flags |= 512; // FFX_UPSCALE_ENABLE_DEBUG_VISUALIZATION
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        if (config.fsr31.enableDebugViewForFsr4)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->OutputScalingEnabled.value_or_default() &&
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            (LowResMV() || RenderWidth() == DisplayWidth()))
#else
            LOG_INFO("Debug view enabled!");
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            _contextDesc.flags |= 512; // FFX_UPSCALE_ENABLE_DEBUG_VISUALIZATION
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            float ssMulti = Config::Instance()->OutputScalingMultiplier.value_or_default();
#else
        }
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (ssMulti < 0.5f)
#else
        if (config.fsr31.outputScalingEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            (LowResMV() || RenderWidth() == DisplayWidth()))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                ssMulti = 0.5f;
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
#else
            float ssMulti = config.fsr31.outputScalingMultiplier;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            else if (ssMulti > 3.0f)
#else
            if (ssMulti < 0.5f)
#endif
            {
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                ssMulti = 3.0f;
#else
                ssMulti = 0.5f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti);
#else
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti); config.fsr31.outputScalingMultiplier = ssMulti;
#endif
            }
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            else if (ssMulti > 3.0f)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _targetWidth = static_cast<unsigned int>(DisplayWidth() * ssMulti);
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _targetHeight = static_cast<unsigned int>(DisplayHeight() * ssMulti);
#else
                ssMulti = 3.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                Config::Instance()->OutputScalingMultiplier.set_volatile_value(ssMulti); config.fsr31.outputScalingMultiplier = ssMulti;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _targetWidth = DisplayWidth();
#else
            _targetWidth = static_cast<unsigned int>(DisplayWidth() * ssMulti);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _targetHeight = DisplayHeight();
#else
            _targetHeight = static_cast<unsigned int>(DisplayHeight() * ssMulti);
#endif
        }
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        // extended limits changes how resolution
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->ExtendedLimits.value_or_default() && RenderWidth() > DisplayWidth())
#else
            _targetWidth = DisplayWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            _targetHeight = DisplayHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxRenderSize.width = RenderWidth();
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxRenderSize.height = RenderHeight();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        // extended limits changes how resolution
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->OutputScalingMultiplier.set_volatile_value(1.0f);
#else
        if (config.fsr31.extendedLimits && RenderWidth() > DisplayWidth())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            // if output scaling active let it to handle downsampling
#else
            _contextDesc.maxRenderSize.width = RenderWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (Config::Instance()->OutputScalingEnabled.value_or_default() &&
#else
            _contextDesc.maxRenderSize.height = RenderHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                (LowResMV() || RenderWidth() == DisplayWidth()))
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            Config::Instance()->OutputScalingMultiplier.set_volatile_value(1.0f); config.fsr31.outputScalingMultiplier = 1.0f;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _contextDesc.maxUpscaleSize.width = _contextDesc.maxRenderSize.width;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _contextDesc.maxUpscaleSize.height = _contextDesc.maxRenderSize.height;
#else
            // if output scaling active let it to handle downsampling
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            if (config.fsr31.outputScalingEnabled &&
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                // update target res
#else
                (LowResMV() || RenderWidth() == DisplayWidth()))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _targetWidth = _contextDesc.maxRenderSize.width;
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _targetHeight = _contextDesc.maxRenderSize.height;
#else
                _contextDesc.maxUpscaleSize.width = _contextDesc.maxRenderSize.width;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
                _contextDesc.maxUpscaleSize.height = _contextDesc.maxRenderSize.height;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            else
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
                // update target res
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _contextDesc.maxUpscaleSize.width = DisplayWidth();
#else
                _targetWidth = _contextDesc.maxRenderSize.width;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                _contextDesc.maxUpscaleSize.height = DisplayHeight();
#else
                _targetHeight = _contextDesc.maxRenderSize.height;
#endif
            }
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        else
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
                _contextDesc.maxUpscaleSize.width = DisplayWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxRenderSize.width = TargetWidth() > DisplayWidth() ? TargetWidth() : DisplayWidth();
#else
                _contextDesc.maxUpscaleSize.height = DisplayHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxRenderSize.height = TargetHeight() > DisplayHeight() ? TargetHeight() : DisplayHeight();
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxUpscaleSize.width = TargetWidth();
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            _contextDesc.maxUpscaleSize.height = TargetHeight();
#else
        else
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            _contextDesc.maxRenderSize.width = TargetWidth() > DisplayWidth() ? TargetWidth() : DisplayWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        // Set stability values as default if not set by user
#else
            _contextDesc.maxRenderSize.height = TargetHeight() > DisplayHeight() ? TargetHeight() : DisplayHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            _contextDesc.maxUpscaleSize.width = TargetWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto config = Config::Instance();
#else
            _contextDesc.maxUpscaleSize.height = TargetHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto const scaleRatioX = (float) TargetWidth() / (float) RenderWidth();
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto const scaleRatioY = (float) TargetHeight() / (float) RenderHeight();
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto const scaleRatio = std::max(scaleRatioX, scaleRatioY);
#else
        // Set stability values as default if not set by user
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (scaleRatio > 0.0f && !std::isinf(scaleRatio))
#else
            auto config = Config::Instance();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            auto const scaleRatioX = (float) TargetWidth() / (float) RenderWidth();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (config->FsrVelocity.value_for_config() == std::nullopt)
#else
            auto const scaleRatioY = (float) TargetHeight() / (float) RenderHeight();
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    config->FsrVelocity.set_volatile_value(0.5f);
#else
            auto const scaleRatio = std::max(scaleRatioX, scaleRatioY);
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (config->FsrReactiveScale.value_for_config() == std::nullopt)
#else
            if (scaleRatio > 0.0f && !std::isinf(scaleRatio))
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    config->FsrReactiveScale.set_volatile_value(0.25f);
#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                if (config->FsrVelocity.value_for_config() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (config->FsrShadingScale.value_for_config() == std::nullopt)
#else
                    config->FsrVelocity.set_volatile_value(0.5f);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    config->FsrShadingScale.set_volatile_value(0.5f / scaleRatio);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                if (config->FsrReactiveScale.value_for_config() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (config->FsrAccAddPerFrame.value_for_config() == std::nullopt)
#else
                    config->FsrReactiveScale.set_volatile_value(0.25f);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    config->FsrAccAddPerFrame.set_volatile_value(scaleRatio / 10.0f);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
                if (config->FsrShadingScale.value_for_config() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                if (config->FsrMinDisOccAcc.value_for_config() == std::nullopt)
#else
                    config->FsrShadingScale.set_volatile_value(0.5f / scaleRatio);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                    config->FsrMinDisOccAcc.set_volatile_value(scaleRatio / 20.0f);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else
                if (config->FsrAccAddPerFrame.value_for_config() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
                    config->FsrAccAddPerFrame.set_volatile_value(scaleRatio / 10.0f);
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ffxCreateBackendDX12Desc backendDesc = { 0 };
#else
                if (config->FsrMinDisOccAcc.value_for_config() == std::nullopt)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        backendDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_BACKEND_DX12;
#else
                    config->FsrMinDisOccAcc.set_volatile_value(scaleRatio / 20.0f);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        backendDesc.device = Device;
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _contextDesc.header.pNext = &backendDesc.header;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        ffxCreateBackendDX12Desc backendDesc = { 0 };
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        if (Config::Instance()->FfxUpscalerIndex.value_or_default() < 0 ||
#else
        backendDesc.header.type = FFX_API_CREATE_CONTEXT_DESC_TYPE_BACKEND_DX12;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->FfxUpscalerIndex.value_or_default() >= State::Instance().ffxUpscalerVersionIds.size())
#else
        backendDesc.device = Device;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            Config::Instance()->FfxUpscalerIndex.set_volatile_value(0);
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        _contextDesc.header.pNext = &backendDesc.header;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        ffxOverrideVersion override = { 0 };
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        override.header.type = FFX_API_DESC_TYPE_OVERRIDE_VERSION;
#else
        if (config.fsr31.ffxUpscalerIndex < 0 ||
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        override.versionId =
#else
            config.fsr31.ffxUpscalerIndex >= State::Instance().ffxUpscalerVersionIds.size())
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().ffxUpscalerVersionIds[Config::Instance()->FfxUpscalerIndex.value_or_default()];
#else
            Config::Instance()->FfxUpscalerIndex.set_volatile_value(0); config.fsr31.ffxUpscalerIndex = 0;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        backendDesc.header.pNext = &override.header;
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        ffxOverrideVersion override = { 0 };
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        LOG_DEBUG("_createContext!");
#else
        override.header.type = FFX_API_DESC_TYPE_OVERRIDE_VERSION;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        override.versionId =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        {
#else
            State::Instance().ffxUpscalerVersionIds[config.fsr31.ffxUpscalerIndex];
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            ScopedSkipHeapCapture skipHeapCapture {};
#else
        backendDesc.header.pNext = &override.header;
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            auto ret = FfxApiProxy::D3D12_CreateContext(&_context, &_contextDesc.header, NULL);
#else
        LOG_DEBUG("_createContext!");
#endif

#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            if (ret != FFX_API_RETURN_OK)
#else
        {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            {
#else
            ScopedSkipHeapCapture skipHeapCapture {};
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                LOG_ERROR("_createContext error: {0}", FfxApiProxy::ReturnCodeToString(ret));
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                return false;
#else
            auto ret = FfxApiProxy::D3D12_CreateContext(&_context, &_contextDesc.header, NULL);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        }
#else
            if (ret != FFX_API_RETURN_OK)
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
            {
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        auto version =
#else
                LOG_ERROR("_createContext error: {0}", FfxApiProxy::ReturnCodeToString(ret));
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
            State::Instance().ffxUpscalerVersionNames[Config::Instance()->FfxUpscalerIndex.value_or_default()];
#else
                return false;
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        _name = "FSR";
#else
            }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
        parse_version(version);
#else
        }
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    }
#else

#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        auto version =
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    SetInit(true);
#else
            State::Instance().ffxUpscalerVersionNames[config.fsr31.ffxUpscalerIndex];
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK

#else
        _name = "FSR";
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
    return true;
#else
        parse_version(version);
#endif
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
}
#else
    }
#endif
