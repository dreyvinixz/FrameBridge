#include "pch.h"
#include "RuntimeConfiguration.h"
#include "../Config.h"

RuntimeConfiguration& RuntimeConfiguration::Instance()
{
    static RuntimeConfiguration instance;
    return instance;
}

RuntimeConfigurationSnapshot RuntimeConfiguration::GetSnapshot() const { return _snapshot; }

void RuntimeConfiguration::RefreshFromConfig()
{
    auto config = Config::Instance();

    // Feature toggles
    _snapshot.fsr31.debugView = config->FsrDebugView.value_or_default();
    _snapshot.fsr31.enableDebugViewForFsr4 = config->Fsr4EnableDebugView.value_or_default();
    _snapshot.fsr31.rcasEnabled = config->RcasEnabled.value_or_default();
    _snapshot.fsr31.outputScalingEnabled = config->OutputScalingEnabled.value_or_default();
    _snapshot.fsr31.extendedLimits = config->ExtendedLimits.value_or_default();
    _snapshot.fsr31.overlayMenu = config->OverlayMenu.value_or_default();

    // Quality parameters
    _snapshot.fsr31.overrideSharpness = config->OverrideSharpness.value_or_default();
    _snapshot.fsr31.sharpness = config->Sharpness.value_or_default();
    _snapshot.fsr31.motionSharpnessEnabled = config->MotionSharpnessEnabled.value_or_default();
    _snapshot.fsr31.motionSharpness = config->MotionSharpness.value_or_default();
    _snapshot.fsr31.outputScalingMultiplier = config->OutputScalingMultiplier.value_or_default();
    _snapshot.fsr31.ffxUpscalerIndex = config->FfxUpscalerIndex.value_or_default();

    // Color processing
    _snapshot.fsr31.nonLinearPQ = config->FsrNonLinearPQ.value_or_default();
    _snapshot.fsr31.nonLinearSRGB = config->FsrNonLinearSRGB.value_or_default();
    _snapshot.fsr31.nonLinearColorSpace = config->FsrNonLinearColorSpace.value_or_default();

    // Camera parameters
    _snapshot.fsr31.daDepthIsLinear = config->DADepthIsLinear.value_for_config_ignore_default().value_or(false);
    _snapshot.fsr31.useFsrInputValues = config->FsrUseFsrInputValues.value_or_default();
    _snapshot.fsr31.verticalFov = config->FsrVerticalFov.value_for_config_ignore_default();
    _snapshot.fsr31.horizontalFov = config->FsrHorizontalFov.value_or_default();
    _snapshot.fsr31.cameraNear = config->FsrCameraNear.value_or_default();
    _snapshot.fsr31.cameraFar = config->FsrCameraFar.value_or_default();

    // Additional rendering parameters
    _snapshot.fsr31.velocity = config->FsrVelocity.value_or_default();
    _snapshot.fsr31.reactiveScale = config->FsrReactiveScale.value_or_default();
    _snapshot.fsr31.shadingScale = config->FsrShadingScale.value_or_default();
    _snapshot.fsr31.accAddPerFrame = config->FsrAccAddPerFrame.value_or_default();
    _snapshot.fsr31.minDisOccAcc = config->FsrMinDisOccAcc.value_or_default();

    // Resource Barriers
    _snapshot.fsr31.colorResourceBarrier = config->ColorResourceBarrier.value_for_config();
    _snapshot.fsr31.mvResourceBarrier = config->MVResourceBarrier.value_for_config();
    _snapshot.fsr31.outputResourceBarrier = config->OutputResourceBarrier.value_for_config();
    _snapshot.fsr31.depthResourceBarrier = config->DepthResourceBarrier.value_for_config();
    _snapshot.fsr31.exposureResourceBarrier = config->ExposureResourceBarrier.value_for_config();
    _snapshot.fsr31.maskResourceBarrier = config->MaskResourceBarrier.value_for_config();
}
