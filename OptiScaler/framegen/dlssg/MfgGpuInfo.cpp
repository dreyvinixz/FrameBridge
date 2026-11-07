#include "pch.h"

#include <misc/IdentifyGpu.h>
#include <nvapi/NvApiTypes.h>

GpuInformation IdentifyGpu::getPrimaryGpu()
{
    GpuInformation gpu {};
    Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return gpu;

    Microsoft::WRL::ComPtr<IDXGIAdapter1> selectedAdapter;
    for (UINT index = 0; factory->EnumAdapters1(index, &selectedAdapter) != DXGI_ERROR_NOT_FOUND; ++index)
    {
        DXGI_ADAPTER_DESC1 desc {};
        if (SUCCEEDED(selectedAdapter->GetDesc1(&desc)) && (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
            (gpu.vendorId == VendorId::Invalid || desc.VendorId == VendorId::Nvidia))
        {
            gpu.luid = desc.AdapterLuid;
            gpu.vendorId = static_cast<VendorId::Value>(desc.VendorId);
            gpu.deviceId = desc.DeviceId;
            gpu.dedicatedVramInBytes = static_cast<size_t>(desc.DedicatedVideoMemory);
            gpu.name = wstring_to_string(desc.Description);
            if (gpu.vendorId == VendorId::Nvidia)
                break;
        }
        selectedAdapter.Reset();
    }

    if (gpu.vendorId != VendorId::Nvidia)
        return gpu;

    HMODULE nvapi = LoadLibraryExW(L"nvapi64.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!nvapi)
        return gpu;

    auto queryInterface = reinterpret_cast<PFN_NvApi_QueryInterface>(GetProcAddress(nvapi, "nvapi_QueryInterface"));
    if (queryInterface)
    {
        auto initialize = GET_INTERFACE(NvAPI_Initialize, queryInterface);
        auto getLogicalGpus = GET_INTERFACE(NvAPI_SYS_GetLogicalGPUs, queryInterface);
        auto getLogicalGpuInfo = GET_INTERFACE(NvAPI_GPU_GetLogicalGpuInfo, queryInterface);
        auto getArchInfo = GET_INTERFACE(NvAPI_GPU_GetArchInfo, queryInterface);
        if (initialize && initialize() == NVAPI_OK && getLogicalGpus && getLogicalGpuInfo && getArchInfo)
        {
            NV_LOGICAL_GPUS logicalGpus {};
            logicalGpus.version = NV_LOGICAL_GPUS_VER;
            if (getLogicalGpus(&logicalGpus) == NVAPI_OK)
            {
                for (NvU32 index = 0; index < logicalGpus.gpuHandleCount; ++index)
                {
                    LUID luid {};
                    NV_LOGICAL_GPU_DATA logicalGpuData {};
                    logicalGpuData.pOSAdapterId = &luid;
                    logicalGpuData.version = NV_LOGICAL_GPU_DATA_VER;
                    if (getLogicalGpuInfo(logicalGpus.gpuHandleData[index].hLogicalGpu, &logicalGpuData) == NVAPI_OK &&
                        luid.HighPart == gpu.luid.HighPart && luid.LowPart == gpu.luid.LowPart &&
                        logicalGpuData.physicalGpuCount > 0)
                    {
                        gpu.nvidiaArchInfo.version = NV_GPU_ARCH_INFO_VER;
                        getArchInfo(logicalGpuData.physicalGpuHandles[0], &gpu.nvidiaArchInfo);
                        gpu.dlssCapable = gpu.nvidiaArchInfo.architecture_id >= NV_GPU_ARCHITECTURE_TU100;
                        break;
                    }
                }
            }
            auto unload = GET_INTERFACE(NvAPI_Unload, queryInterface);
            if (unload)
                unload();
        }
    }

    FreeLibrary(nvapi);
    return gpu;
}
