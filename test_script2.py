import re

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12_ab.cpp', 'r') as f:
    lines = f.readlines()

new_lines = []
for i, line in enumerate(lines):
    if line.strip() == "LOG_FUNC();":
        # Check if we are inside Evaluate or InitFSR3
        if "Evaluate" in "".join(lines[max(0, i-5):i]):
            new_lines.append(line)
            new_lines.append('#ifndef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK\n')
            new_lines.append('    auto config = RuntimeConfiguration::Instance().GetSnapshot();\n')
            new_lines.append('#endif\n')
        elif "InitFSR3" in "".join(lines[max(0, i-5):i]):
            new_lines.append(line)
            new_lines.append('#ifndef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK\n')
            new_lines.append('    RuntimeConfiguration::Instance().RefreshFromConfig();\n')
            new_lines.append('    auto config = RuntimeConfiguration::Instance().GetSnapshot();\n')
            new_lines.append('#endif\n')
        else:
            new_lines.append(line)
    else:
        new_lines.append(line)

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'w') as f:
    f.writelines(new_lines)
