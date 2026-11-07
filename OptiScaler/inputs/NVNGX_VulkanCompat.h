#pragma once

#include <nvsdk_ngx_vk.h>

// Implemented by OptiScaler's NGX interposer for callers using the legacy
// project-ID Vulkan initialization entry point.
NVSDK_NGX_API NVSDK_NGX_Result NVSDK_NGX_VULKAN_Init_ProjectID_Ext(
    const char* InProjectId, NVSDK_NGX_EngineType InEngineType, const char* InEngineVersion,
    const wchar_t* InApplicationDataPath, VkInstance InInstance, VkPhysicalDevice InPD, VkDevice InDevice,
    PFN_vkGetInstanceProcAddr InGIPA, PFN_vkGetDeviceProcAddr InGDPA, NVSDK_NGX_Version InSDKVersion,
    const NVSDK_NGX_FeatureCommonInfo* InFeatureInfo);
