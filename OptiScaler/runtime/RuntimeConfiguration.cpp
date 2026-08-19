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
    auto& ffx = _snapshot.ffx;

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
