#include "pch.h"
#include "RuntimeCapabilities.h"

RuntimeCapabilities& RuntimeCapabilities::Instance()
{
    static RuntimeCapabilities instance;
    return instance;
}

void RuntimeCapabilities::UpdateFsr4Info(const Fsr4RuntimeInfo& info)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.capabilities.fsr4 = info;
}

void RuntimeCapabilities::SetActiveUpscaler(UpscalerType type, uint32_t render_width, uint32_t render_height)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.capabilities.upscaler = type;
    _snapshot.frame.render_width = render_width;
    _snapshot.frame.render_height = render_height;
}

void RuntimeCapabilities::SetActiveFrameGeneration(FrameGenerationType type)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.capabilities.frame_generation = type;
}

void RuntimeCapabilities::PublishFrame(const FrameSnapshot& frame)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.frame.frame_number = frame.frame_number;
    _snapshot.frame.timestamp = frame.timestamp;
    _snapshot.frame.frame_time_ms = frame.frame_time_ms;
    // Don't overwrite render_width/render_height if they weren't provided in this frame update,
    // though the caller could pass them. We can just copy the whole struct.
    _snapshot.frame = frame;
}

RuntimeSnapshot RuntimeCapabilities::GetSnapshot() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _snapshot;
}
