import sys

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'r') as f:
    lines = f.readlines()

new_lines = []
for line in lines:
    if line.strip() == '#include "FSR31Feature_Dx12.h"':
        new_lines.append(line)
        new_lines.append('#include <chrono>\n')
        continue
    
    if line.startswith('bool FSR31FeatureDx12::Evaluate(ID3D12GraphicsCommandList* InCommandList, NVSDK_NGX_Parameter* InParameters)'):
        new_lines.append(line)
        new_lines.append('{\n')
        new_lines.append('    struct BenchmarkTimer {\n')
        new_lines.append('        std::chrono::high_resolution_clock::time_point start;\n')
        new_lines.append('        BenchmarkTimer() { start = std::chrono::high_resolution_clock::now(); }\n')
        new_lines.append('        ~BenchmarkTimer() {\n')
        new_lines.append('            auto end = std::chrono::high_resolution_clock::now();\n')
        new_lines.append('            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();\n')
        new_lines.append('            static int frames = 0;\n')
        new_lines.append('            static long long total = 0;\n')
        new_lines.append('            static long long max_time = 0;\n')
        new_lines.append('            total += duration;\n')
        new_lines.append('            if (duration > max_time) max_time = duration;\n')
        new_lines.append('            frames++;\n')
        new_lines.append('            if (frames >= 300) {\n')
        new_lines.append('                LOG_INFO("Evaluate() Benchmark: Avg {} us, Max {} us (over 300 frames)", total / 300.0, max_time);\n')
        new_lines.append('                frames = 0; total = 0; max_time = 0;\n')
        new_lines.append('            }\n')
        new_lines.append('        }\n')
        new_lines.append('    } __timer;\n')
        # Skip the original '{'
        lines.pop(lines.index(line) + 1)
        continue
        
    new_lines.append(line)

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'w') as f:
    f.writelines(new_lines)
