import re

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'r') as f:
    content = f.read()

# Remove the old BenchmarkTimer block
timer_pattern = r'    struct BenchmarkTimer \{.*?\} __timer;\n'
content = re.sub(timer_pattern, '', content, flags=re.DOTALL)

# Now, we need to inject the new BenchmarkTimer definition somewhere, and instantiate it at the top of Evaluate
timer_code = """
#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER
#include <vector>
#include <algorithm>
#endif
"""

# Let's insert the definition above FSR31FeatureDx12::Evaluate
def_code = """
#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER
struct BenchmarkTimer {
    std::chrono::high_resolution_clock::time_point start;
    BenchmarkTimer() { start = std::chrono::high_resolution_clock::now(); }
    ~BenchmarkTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        static std::vector<long long> samples;
        samples.push_back(duration);
        if (samples.size() >= 1000) {
            std::sort(samples.begin(), samples.end());
            long long sum = 0;
            for (auto s : samples) sum += s;
            double avg = sum / 1000.0;
            long long p50 = samples[500];
            long long p95 = samples[950];
            long long p99 = samples[990];
            long long max_val = samples[999];
            LOG_INFO("[FrameBridge Benchmark] Evaluate samples: 1000 | Avg: {:.3f} ms | P50: {:.3f} ms | P95: {:.3f} ms | P99: {:.3f} ms | Max: {:.3f} ms | Path: {}",
                     avg / 1000.0, p50 / 1000.0, p95 / 1000.0, p99 / 1000.0, max_val / 1000.0,
#ifdef FRAMEBRIDGE_RUNTIME_CONFIG_BENCHMARK
                     "Legacy"
#else
                     "Snapshot"
#endif
            );
            samples.clear();
        }
    }
};
#endif
"""

eval_signature = r'(bool FSR31FeatureDx12::Evaluate\(ID3D12GraphicsCommandList\* InCommandList, NVSDK_NGX_Parameter\* InParameters\)\n\{)'

# Also add the #include <vector> and <algorithm> at the top.
# Let's put it after #include <chrono>
content = content.replace('#include <chrono>', '#include <chrono>\n#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER\n#include <vector>\n#include <algorithm>\n#endif')

content = re.sub(eval_signature, def_code + r'\1\n#ifdef FRAMEBRIDGE_ENABLE_EVALUATE_TIMER\n    BenchmarkTimer __timer;\n#endif\n', content)

with open('d:\\FrameBridge\\OptiScaler\\upscalers\\fsr31\\FSR31Feature_Dx12.cpp', 'w') as f:
    f.write(content)
