import re

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'r') as f:
    orig_lines = f.readlines()

replacements = {
    r'Config::Instance\(\)->FsrDebugView\.value_or_default\(\)': 'config.fsr31.debugView',
    r'Config::Instance\(\)->Fsr4EnableDebugView\.value_or_default\(\)': 'config.fsr31.enableDebugViewForFsr4',
    r'Config::Instance\(\)->FsrNonLinearPQ\.value_or_default\(\)': 'config.fsr31.nonLinearPQ',
    r'Config::Instance\(\)->FsrNonLinearSRGB\.value_or_default\(\)': 'config.fsr31.nonLinearSRGB',
    r'Config::Instance\(\)->OverrideSharpness\.value_or_default\(\)': 'config.fsr31.overrideSharpness',
    r'Config::Instance\(\)->Sharpness\.value_or_default\(\)': 'config.fsr31.sharpness',
    r'Config::Instance\(\)->RcasEnabled\.value_or_default\(\)': 'config.fsr31.rcasEnabled',
    r'Config::Instance\(\)->OutputScalingEnabled\.value_or_default\(\)': 'config.fsr31.outputScalingEnabled',
    r'Config::Instance\(\)->FsrUseFsrInputValues\.value_or_default\(\)': 'config.fsr31.useFsrInputValues',
    r'Config::Instance\(\)->FsrCameraNear\.value_or_default\(\)': 'config.fsr31.cameraNear',
    r'Config::Instance\(\)->FsrCameraFar\.value_or_default\(\)': 'config.fsr31.cameraFar',
    r'Config::Instance\(\)->FsrVerticalFov\.has_value\(\)': 'config.fsr31.verticalFov.has_value()',
    r'Config::Instance\(\)->FsrVerticalFov\.value\(\)': 'config.fsr31.verticalFov.value()',
    r'Config::Instance\(\)->FsrHorizontalFov\.value_or_default\(\)': 'config.fsr31.horizontalFov',
    r'Config::Instance\(\)->FsrHorizontalFov\.value\(\)': 'config.fsr31.horizontalFov',
    r'Config::Instance\(\)->FsrVelocity\.value_or_default\(\)': 'config.fsr31.velocity',
    r'Config::Instance\(\)->FsrReactiveScale\.value_or_default\(\)': 'config.fsr31.reactiveScale',
    r'Config::Instance\(\)->FsrShadingScale\.value_or_default\(\)': 'config.fsr31.shadingScale',
    r'Config::Instance\(\)->FsrAccAddPerFrame\.value_or_default\(\)': 'config.fsr31.accAddPerFrame',
    r'Config::Instance\(\)->FsrMinDisOccAcc\.value_or_default\(\)': 'config.fsr31.minDisOccAcc',
    r'Config::Instance\(\)->OutputScalingMultiplier\.value_or_default\(\)': 'config.fsr31.outputScalingMultiplier',
    r'Config::Instance\(\)->MotionSharpnessEnabled\.value_or_default\(\)': 'config.fsr31.motionSharpnessEnabled',
    r'Config::Instance\(\)->MotionSharpness\.value_or_default\(\)': 'config.fsr31.motionSharpness',
    r'Config::Instance\(\)->OverlayMenu\.value_or_default\(\)': 'config.fsr31.overlayMenu',
    r'Config::Instance\(\)->ColorResourceBarrier\.has_value\(\)': 'config.fsr31.colorResourceBarrier.has_value()',
    r'Config::Instance\(\)->ColorResourceBarrier\.value\(\)': 'config.fsr31.colorResourceBarrier.value()',
    r'Config::Instance\(\)->MVResourceBarrier\.has_value\(\)': 'config.fsr31.mvResourceBarrier.has_value()',
    r'Config::Instance\(\)->MVResourceBarrier\.value\(\)': 'config.fsr31.mvResourceBarrier.value()',
    r'Config::Instance\(\)->OutputResourceBarrier\.has_value\(\)': 'config.fsr31.outputResourceBarrier.has_value()',
    r'Config::Instance\(\)->OutputResourceBarrier\.value\(\)': 'config.fsr31.outputResourceBarrier.value()',
    r'Config::Instance\(\)->DepthResourceBarrier\.has_value\(\)': 'config.fsr31.depthResourceBarrier.has_value()',
    r'Config::Instance\(\)->DepthResourceBarrier\.value\(\)': 'config.fsr31.depthResourceBarrier.value()',
    r'Config::Instance\(\)->ExposureResourceBarrier\.has_value\(\)': 'config.fsr31.exposureResourceBarrier.has_value()',
    r'Config::Instance\(\)->ExposureResourceBarrier\.value\(\)': 'config.fsr31.exposureResourceBarrier.value()',
    r'Config::Instance\(\)->MaskResourceBarrier\.has_value\(\)': 'config.fsr31.maskResourceBarrier.has_value()',
    r'Config::Instance\(\)->MaskResourceBarrier\.value\(\)': 'config.fsr31.maskResourceBarrier.value()',
    r'Config::Instance\(\)->FsrNonLinearColorSpace\.value_or_default\(\)': 'config.fsr31.nonLinearColorSpace',
    r'Config::Instance\(\)->ExtendedLimits\.value_or_default\(\)': 'config.fsr31.extendedLimits',
    r'Config::Instance\(\)->FfxUpscalerIndex\.value_or_default\(\)': 'config.fsr31.ffxUpscalerIndex',
    r'Config::Instance\(\)->RcasEnabled\.set_volatile_value\(false\);': 'Config::Instance()->RcasEnabled.set_volatile_value(false); config.fsr31.rcasEnabled = false;',
    r'Config::Instance\(\)->OutputScalingEnabled\.set_volatile_value\(false\);': 'Config::Instance()->OutputScalingEnabled.set_volatile_value(false); config.fsr31.outputScalingEnabled = false;',
    r'Config::Instance\(\)->DADepthIsLinear\.set_volatile_value\(false\);': 'Config::Instance()->DADepthIsLinear.set_volatile_value(false); config.fsr31.daDepthIsLinear = false;',
    r'Config::Instance\(\)->ColorResourceBarrier\.set_volatile_value\(([^)]+)\);': r'Config::Instance()->ColorResourceBarrier.set_volatile_value(\1); config.fsr31.colorResourceBarrier = \1;',
    r'Config::Instance\(\)->MVResourceBarrier\.set_volatile_value\(([^)]+)\);': r'Config::Instance()->MVResourceBarrier.set_volatile_value(\1); config.fsr31.mvResourceBarrier = \1;',
    r'Config::Instance\(\)->OutputScalingMultiplier\.set_volatile_value\(([^)]+)\);': r'Config::Instance()->OutputScalingMultiplier.set_volatile_value(\1); config.fsr31.outputScalingMultiplier = \1;',
    r'Config::Instance\(\)->FfxUpscalerIndex\.set_volatile_value\(([^)]+)\);': r'Config::Instance()->FfxUpscalerIndex.set_volatile_value(\1); config.fsr31.ffxUpscalerIndex = \1;'
}

new_lines = []
for line in orig_lines:
    if line.strip() == '#include "FSR31Feature_Dx12.h"':
        new_lines.append(line)
        new_lines.append('#include "../../runtime/RuntimeConfiguration.h"\n')
        continue
        
    modified_line = line
    for pattern, replacement in replacements.items():
        modified_line = re.sub(pattern, replacement, modified_line)
        
    if modified_line != line:
        new_lines.append('#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK\n')
        new_lines.append(line)
        new_lines.append('#else\n')
        new_lines.append(modified_line)
        new_lines.append('#endif\n')
    else:
        new_lines.append(line)
        
with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12_ab.cpp', 'w') as f:
    f.writelines(new_lines)
