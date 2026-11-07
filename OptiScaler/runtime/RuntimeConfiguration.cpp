#include "pch.h"
#include "RuntimeConfiguration.h"

#include "../Config.h"

RuntimeConfiguration& RuntimeConfiguration::Instance()
{
    static RuntimeConfiguration instance;
    return instance;
}

RuntimeConfigurationSnapshot RuntimeConfiguration::GetSnapshot() const { return _snapshot; }

void NotifyRuntimeConfigurationChanged() { RuntimeConfiguration::Instance().RefreshFromConfig(); }

void RuntimeConfiguration::RefreshFromConfig()
{
    auto config = Config::Instance();
    auto& fsr31 = _snapshot.fsr31;
    auto& ffx = _snapshot.ffx;

    fsr31.debugView = config->FsrDebugView.value_or_default();
    fsr31.enableDebugViewForFsr4 = config->Fsr4EnableDebugView.value_or_default();
    fsr31.rcasEnabled = config->RcasEnabled.value_or_default();
    fsr31.outputScalingEnabled = config->OutputScalingEnabled.value_or_default();
    fsr31.extendedLimits = config->ExtendedLimits.value_or_default();
    fsr31.overlayMenu = config->OverlayMenu.value_or_default();
    fsr31.overrideSharpness = config->OverrideSharpness.value_or_default();
    fsr31.sharpness = config->Sharpness.value_or_default();
    fsr31.motionSharpnessEnabled = config->MotionSharpnessEnabled.value_or_default();
    fsr31.motionSharpness = config->MotionSharpness.value_or_default();
    fsr31.outputScalingMultiplier = config->OutputScalingMultiplier.value_or_default();
    fsr31.ffxUpscalerIndex = config->FfxUpscalerIndex.value_or_default();
    fsr31.nonLinearPQ = config->FsrNonLinearPQ.value_or_default();
    fsr31.nonLinearSRGB = config->FsrNonLinearSRGB.value_or_default();
    fsr31.nonLinearColorSpace = config->FsrNonLinearColorSpace.value_or_default();
    fsr31.daDepthIsLinear = config->DADepthIsLinear.value_for_config_ignore_default().value_or(false);
    fsr31.useFsrInputValues = config->FsrUseFsrInputValues.value_or_default();
    fsr31.verticalFov = config->FsrVerticalFov.value_for_config_ignore_default();
    fsr31.horizontalFov = config->FsrHorizontalFov.value_or_default();
    fsr31.cameraNear = config->FsrCameraNear.value_or_default();
    fsr31.cameraFar = config->FsrCameraFar.value_or_default();
    fsr31.velocity = config->FsrVelocity.value_or_default();
    fsr31.reactiveScale = config->FsrReactiveScale.value_or_default();
    fsr31.shadingScale = config->FsrShadingScale.value_or_default();
    fsr31.accAddPerFrame = config->FsrAccAddPerFrame.value_or_default();
    fsr31.minDisOccAcc = config->FsrMinDisOccAcc.value_or_default();
    fsr31.colorResourceBarrier = config->ColorResourceBarrier.value_for_config();
    fsr31.mvResourceBarrier = config->MVResourceBarrier.value_for_config();
    fsr31.outputResourceBarrier = config->OutputResourceBarrier.value_for_config();
    fsr31.depthResourceBarrier = config->DepthResourceBarrier.value_for_config();
    fsr31.exposureResourceBarrier = config->ExposureResourceBarrier.value_for_config();
    fsr31.maskResourceBarrier = config->MaskResourceBarrier.value_for_config();

    ffx.debugView = config->FsrDebugView.value_or_default();
    ffx.nonLinearPQ = config->FsrNonLinearPQ.value_or_default();
    ffx.nonLinearSRGB = config->FsrNonLinearSRGB.value_or_default();
    ffx.nonLinearColorSpace = config->FsrNonLinearColorSpace.value_or_default();

    ffx.outputScalingEnabled = config->OutputScalingEnabled.value_or_default();
    ffx.outputScalingMultiplier = config->OutputScalingMultiplier.value_or_default();

    ffx.colorResourceBarrier = config->ColorResourceBarrier.value_for_config();
    ffx.motionVectorResourceBarrier = config->MVResourceBarrier.value_for_config();
    ffx.outputResourceBarrier = config->OutputResourceBarrier.value_for_config();
    ffx.depthResourceBarrier = config->DepthResourceBarrier.value_for_config();
    ffx.exposureResourceBarrier = config->ExposureResourceBarrier.value_for_config();
    ffx.maskResourceBarrier = config->MaskResourceBarrier.value_for_config();

    ffx.disableReactiveMask = config->DisableReactiveMask.value_for_config();
    ffx.useMaskForTransparency = config->FsrUseMaskForTransparency.value_or_default();
    ffx.dlssReactiveMaskBias = config->DlssReactiveMaskBias.value_or_default();

    ffx.useFsrInputValues = config->FsrUseFsrInputValues.value_or_default();
    ffx.verticalFov =
        config->FsrVerticalFov.has_value() ? std::optional { config->FsrVerticalFov.value() } : std::nullopt;
    ffx.horizontalFov = config->FsrHorizontalFov.value_or_default();
    ffx.cameraNear = config->FsrCameraNear.value_or_default();
    ffx.cameraFar = config->FsrCameraFar.value_or_default();

    ffx.velocity = config->FsrVelocity.value_or_default();
    ffx.reactiveScale = config->FsrReactiveScale.value_or_default();
    ffx.shadingScale = config->FsrShadingScale.value_or_default();
    ffx.accumulationAddedPerFrame = config->FsrAccAddPerFrame.value_or_default();
    ffx.minimumDisocclusionAccumulation = config->FsrMinDisOccAcc.value_or_default();
}

void RuntimeConfiguration::SetColorResourceBarrier(int32_t value) { _snapshot.ffx.colorResourceBarrier = value; }

void RuntimeConfiguration::SetMotionVectorResourceBarrier(int32_t value)
{
    _snapshot.ffx.motionVectorResourceBarrier = value;
}

void RuntimeConfiguration::SetDisableReactiveMask(bool value) { _snapshot.ffx.disableReactiveMask = value; }
