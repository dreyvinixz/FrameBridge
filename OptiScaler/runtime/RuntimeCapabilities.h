#pragma once

#include "RuntimeCapabilityTypes.h"
#include <mutex>

class RuntimeCapabilities
{
public:
    static RuntimeCapabilities& Instance();

    void UpdateFsr4Info(const Fsr4RuntimeInfo& info);
    
    void SetActiveUpscaler(UpscalerType type, uint32_t render_width, uint32_t render_height);
    
    void SetActiveFrameGeneration(FrameGenerationType type);
    
    void PublishFrame(const FrameSnapshot& frame);

    RuntimeSnapshot GetSnapshot() const;

private:
    RuntimeCapabilities() = default;

    mutable std::mutex _mutex;
    RuntimeSnapshot _snapshot;
};
