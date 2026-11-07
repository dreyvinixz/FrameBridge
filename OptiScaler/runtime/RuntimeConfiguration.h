#pragma once

#include <cstdint>
#include <optional>

struct FfxRuntimeConfiguration
{
    bool debugView = false;
    bool nonLinearPQ = false;
    bool nonLinearSRGB = false;
    bool nonLinearColorSpace = false;

    bool outputScalingEnabled = false;
    float outputScalingMultiplier = 1.0f;

    std::optional<int32_t> colorResourceBarrier = std::nullopt;
    std::optional<int32_t> motionVectorResourceBarrier = std::nullopt;
    std::optional<int32_t> outputResourceBarrier = std::nullopt;
    std::optional<int32_t> depthResourceBarrier = std::nullopt;
    std::optional<int32_t> exposureResourceBarrier = std::nullopt;
    std::optional<int32_t> maskResourceBarrier = std::nullopt;

    std::optional<bool> disableReactiveMask = std::nullopt;
    bool useMaskForTransparency = true;
    float dlssReactiveMaskBias = 0.45f;

    bool useFsrInputValues = true;
    std::optional<float> verticalFov = std::nullopt;
    float horizontalFov = 0.0f;
    float cameraNear = 0.1f;
    float cameraFar = 100000.0f;

    float velocity = 1.0f;
    float reactiveScale = 1.0f;
    float shadingScale = 1.0f;
    float accumulationAddedPerFrame = 0.0f;
    float minimumDisocclusionAccumulation = 0.0f;
};

// Keep the established FSR 3.1 dispatch snapshot while FFX API settings use
// their dedicated runtime view above.
struct Fsr31Configuration
{
    bool debugView = false;
    bool enableDebugViewForFsr4 = false;
    bool rcasEnabled = false;
    bool outputScalingEnabled = false;
    bool extendedLimits = false;
    bool overlayMenu = false;
    bool overrideSharpness = false;
    float sharpness = 0.0f;
    bool motionSharpnessEnabled = false;
    float motionSharpness = 0.0f;
    float outputScalingMultiplier = 1.0f;
    int ffxUpscalerIndex = 0;
    bool nonLinearPQ = false;
    bool nonLinearSRGB = false;
    bool nonLinearColorSpace = false;
    bool daDepthIsLinear = false;
    bool useFsrInputValues = false;
    std::optional<float> verticalFov = std::nullopt;
    float horizontalFov = 0.0f;
    float cameraNear = 0.0f;
    float cameraFar = 0.0f;
    float velocity = 1.0f;
    float reactiveScale = 1.0f;
    float shadingScale = 1.0f;
    float accAddPerFrame = 0.0f;
    float minDisOccAcc = 0.0f;
    std::optional<int> colorResourceBarrier = std::nullopt;
    std::optional<int> mvResourceBarrier = std::nullopt;
    std::optional<int> outputResourceBarrier = std::nullopt;
    std::optional<int> depthResourceBarrier = std::nullopt;
    std::optional<int> exposureResourceBarrier = std::nullopt;
    std::optional<int> maskResourceBarrier = std::nullopt;
};

struct RuntimeConfigurationSnapshot
{
    Fsr31Configuration fsr31;
    FfxRuntimeConfiguration ffx;
};

class RuntimeConfiguration
{
  public:
    static RuntimeConfiguration& Instance();

    RuntimeConfigurationSnapshot GetSnapshot() const;
    void RefreshFromConfig();

    void SetColorResourceBarrier(int32_t value);
    void SetMotionVectorResourceBarrier(int32_t value);
    void SetDisableReactiveMask(bool value);

  private:
    RuntimeConfiguration() = default;
    ~RuntimeConfiguration() = default;

    RuntimeConfiguration(const RuntimeConfiguration&) = delete;
    RuntimeConfiguration& operator=(const RuntimeConfiguration&) = delete;

    RuntimeConfigurationSnapshot _snapshot;
};
