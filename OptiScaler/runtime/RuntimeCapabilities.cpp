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

void RuntimeCapabilities::SetActiveUpscaler(UpscalerType type, uint32_t renderWidth, uint32_t renderHeight)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.capabilities.upscaler = type;
    _snapshot.frame.render_width = renderWidth;
    _snapshot.frame.render_height = renderHeight;
}

void RuntimeCapabilities::SetActiveFrameGeneration(FrameGenerationType type)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.capabilities.frame_generation = type;
}

void RuntimeCapabilities::PublishFrame(const FrameSnapshot& frame)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _snapshot.frame = frame;
}

RuntimeSnapshot RuntimeCapabilities::GetSnapshot() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _snapshot;
}
