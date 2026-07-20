#pragma once

#include <optional>

struct Fsr31Configuration
{
    // Feature toggles
    bool debugView = false;
    bool enableDebugViewForFsr4 = false;
    bool rcasEnabled = false;
    bool outputScalingEnabled = false;
    bool extendedLimits = false;
    bool overlayMenu = false;

    // Quality parameters
    bool overrideSharpness = false;
    float sharpness = 0.0f;
    bool motionSharpnessEnabled = false;
    float motionSharpness = 0.0f;
    float outputScalingMultiplier = 1.0f;
    int ffxUpscalerIndex = 0;

    // Color processing
    bool nonLinearPQ = false;
    bool nonLinearSRGB = false;
    bool nonLinearColorSpace = false;

    // Camera parameters
    bool daDepthIsLinear = false;
    bool useFsrInputValues = false;
    std::optional<float> verticalFov = std::nullopt;
    float horizontalFov = 0.0f;
    float cameraNear = 0.0f;
    float cameraFar = 0.0f;

    // Additional rendering parameters
    float velocity = 1.0f;
    float reactiveScale = 1.0f;
    float shadingScale = 1.0f;
    float accAddPerFrame = 0.0f;
    float minDisOccAcc = 0.0f;

    // Resource Barriers
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
};

class RuntimeConfiguration
{
public:
    static RuntimeConfiguration& Instance();

    RuntimeConfigurationSnapshot GetSnapshot() const;

    void RefreshFromConfig();

private:
    RuntimeConfiguration() = default;
    ~RuntimeConfiguration() = default;

    RuntimeConfiguration(const RuntimeConfiguration&) = delete;
    RuntimeConfiguration& operator=(const RuntimeConfiguration&) = delete;

    RuntimeConfigurationSnapshot _snapshot;
};

