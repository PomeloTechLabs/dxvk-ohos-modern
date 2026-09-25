#pragma once

#include "../vulkan/vulkan_loader.h"
#include "../../include/native/ohos/dxvk_native_ohos.h"

// The registry lives in DXGI. D3D11 also links WSI code, so it must cross
// this C boundary rather than linking a second registry or a hidden C++ symbol.
extern "C" __attribute__((visibility("default"))) VkResult
DXVKOhosCreateSurface(DXVKOhosWindowHandle handle,
    PFN_vkGetInstanceProcAddr getProc, VkInstance instance,
    VkSurfaceKHR* surface);
