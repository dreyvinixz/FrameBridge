#pragma once
#include <cstdint>

enum class UpscalerType
{
    Unknown,
    None,
    FSR2,
    FSR3,
    DLSS,
    XeSS,
    Custom
};

enum class FrameGenerationType
{
    Unknown,
    None,
    FSR3_FG,
    DLSS_FG,
    XeSS_FG,
    Opti_FG
};

enum class Fsr4PrecisionMode
{
    Unknown,
    FP8,
    INT8,
    Fallback
};

struct Fsr4RuntimeInfo
{
    Fsr4PrecisionMode precision = Fsr4PrecisionMode::Unknown;
    bool forced_int8 = false;
    bool hardware_supported = false;
};

struct RuntimeCapabilitiesSnapshot
{
    Fsr4RuntimeInfo fsr4;
    UpscalerType upscaler = UpscalerType::Unknown;
    FrameGenerationType frame_generation = FrameGenerationType::Unknown;
    // We can add graphics API or GPU capabilities here later
};

struct FrameSnapshot
{
    uint64_t frame_number = 0;
    double timestamp = 0.0;
    float frame_time_ms = 0.0f;
    uint32_t render_width = 0;
    uint32_t render_height = 0;
};

struct RuntimeSnapshot
{
    RuntimeCapabilitiesSnapshot capabilities;
    FrameSnapshot frame;
};
