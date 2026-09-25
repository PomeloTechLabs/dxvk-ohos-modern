#if defined(DXVK_WSI_OHOS)

#include "../wsi_platform.h"
#include "../ohos_window_bridge.h"
#include "../../vulkan/vulkan_loader.h"
#include "../../../include/native/wsi/native_ohos.h"

#include <algorithm>
#include <cstring>
#include <new>

namespace dxvk::wsi {

  namespace {
    constexpr uint32_t nominalWidth = 1280;
    constexpr uint32_t nominalHeight = 720;

    HMONITOR primaryMonitor() {
      return reinterpret_cast<HMONITOR>(static_cast<uintptr_t>(1));
    }

    class OhosWsiDriver final : public WsiDriver {
    public:
      std::vector<const char*> getInstanceExtensions() override {
        return { VK_KHR_SURFACE_EXTENSION_NAME, VK_OHOS_SURFACE_EXTENSION_NAME };
      }

      HMONITOR getDefaultMonitor() override { return primaryMonitor(); }
      HMONITOR enumMonitors(uint32_t index) override {
        return index == 0 ? primaryMonitor() : nullptr;
      }
      HMONITOR enumMonitors(const LUID*[], uint32_t, uint32_t index) override {
        return enumMonitors(index);
      }

      bool getDisplayName(HMONITOR monitor, WCHAR (&name)[32]) override {
        if (monitor != primaryMonitor()) return false;
        const wchar_t value[] = L"\\\\.\\DISPLAY1";
        std::fill(std::begin(name), std::end(name), 0);
        std::copy(value, value + std::size(value), name);
        return true;
      }

      bool getDesktopCoordinates(HMONITOR monitor, RECT* rect) override {
        if (monitor != primaryMonitor() || !rect) return false;
        *rect = { 0, 0, static_cast<LONG>(nominalWidth),
          static_cast<LONG>(nominalHeight) };
        return true;
      }

      bool getDisplayMode(HMONITOR monitor, uint32_t index, WsiMode* mode) override {
        if (monitor != primaryMonitor() || index != 0 || !mode) return false;
        *mode = { nominalWidth, nominalHeight, { 60, 1 }, 32, false };
        return true;
      }
      bool getCurrentDisplayMode(HMONITOR monitor, WsiMode* mode) override {
        return getDisplayMode(monitor, 0, mode);
      }
      bool getDesktopDisplayMode(HMONITOR monitor, WsiMode* mode) override {
        return getDisplayMode(monitor, 0, mode);
      }
      WsiEdidData getMonitorEdid(HMONITOR) override { return {}; }

      void getWindowSize(HWND window, uint32_t* width, uint32_t* height) override {
        DXVKOhosWindowInfo info = {};
        const bool valid = DXVKOhosGetWindowInfo(fromHwnd(window), &info)
          == DXVK_OHOS_WINDOW_OK;
        if (width) *width = valid ? info.width : 0;
        if (height) *height = valid ? info.height : 0;
      }
      void resizeWindow(HWND, DxvkWindowState*, uint32_t, uint32_t) override {
        // XComponent owns the native window size. Its callback updates the registry.
      }
      bool setWindowMode(HMONITOR monitor, HWND window, DxvkWindowState*,
          const WsiMode& mode) override {
        DXVKOhosWindowInfo info = {};
        return monitor == primaryMonitor() &&
          DXVKOhosGetWindowInfo(fromHwnd(window), &info) == DXVK_OHOS_WINDOW_OK &&
          mode.width == info.width && mode.height == info.height;
      }
      bool enterFullscreenMode(HMONITOR monitor, HWND window, DxvkWindowState*,
          bool) override {
        return monitor == primaryMonitor() && isWindow(window);
      }
      bool leaveFullscreenMode(HWND window, DxvkWindowState*, bool) override {
        return isWindow(window);
      }
      bool restoreDisplayMode() override { return true; }
      HMONITOR getWindowMonitor(HWND window) override {
        return isWindow(window) ? primaryMonitor() : nullptr;
      }
      bool isWindow(HWND window) override {
        DXVKOhosWindowInfo info = {};
        return DXVKOhosGetWindowInfo(fromHwnd(window), &info) == DXVK_OHOS_WINDOW_OK;
      }
      bool isMinimized(HWND window) override {
        DXVKOhosWindowInfo info = {};
        return DXVKOhosGetWindowInfo(fromHwnd(window), &info) != DXVK_OHOS_WINDOW_OK
          || !info.width || !info.height;
      }
      bool isOccluded(HWND window) override { return isMinimized(window); }
      void updateFullscreenWindow(HMONITOR, HWND, bool) override { }

      VkResult createSurface(HWND window, PFN_vkGetInstanceProcAddr getProc,
          VkInstance instance, VkSurfaceKHR* surface) override {
        return DXVKOhosCreateSurface(fromHwnd(window), getProc, instance, surface);
      }
    };

    bool createOhosWsiDriver(WsiDriver** driver) {
      if (!driver) return false;
      *driver = new (std::nothrow) OhosWsiDriver();
      return *driver != nullptr;
    }
  }

  WsiBootstrap OhosWSI = { "OHOS", createOhosWsiDriver };
}

#endif
