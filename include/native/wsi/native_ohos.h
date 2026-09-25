#pragma once

#include <windows.h>
#include <cstdint>

namespace dxvk::wsi {
  inline HWND toHwnd(uint64_t handle) {
    return reinterpret_cast<HWND>(static_cast<uintptr_t>(handle));
  }

  inline uint64_t fromHwnd(HWND window) {
    return static_cast<uint64_t>(reinterpret_cast<uintptr_t>(window));
  }
}
