#include <cstring>
#include <format>

#include <windower.hpp>

#include <windows.h>
#include <dwmapi.h>

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

namespace windower
{
    bool WindowManager::show(int cmd_show) const noexcept
    {
        // If the window was previously visible, the return value is nonzero.
        // If the window was previously hidden, the return value is zero.
        ShowWindow(_hwnd, cmd_show);

        if (!UpdateWindow(_hwnd))
            return false;

        return true;
    }

    std::expected<void, std::error_code> WindowManager::destroy() const noexcept
    {
        if (!DestroyWindow(_hwnd))
            return std::unexpected(last_win32_error());

        return {};
    }

    std::expected<WindowSize, std::error_code> WindowManager::get_size() const noexcept
    {
        RECT area{};
        if (!GetClientRect(_hwnd, &area)) {
            return std::unexpected(last_win32_error());
        }

        return WindowSize{
            .w = static_cast<int>(area.right - area.left),
            .h = static_cast<int>(area.bottom - area.top)
        };
    }

    std::expected<WindowPos, std::error_code> WindowManager::get_pos() const noexcept
    {
        POINT origin{};
        if (!ClientToScreen(_hwnd, &origin)) {
            return std::unexpected(last_win32_error());
        }

        return WindowPos{
            .x = static_cast<int>(origin.x),
            .y = static_cast<int>(origin.y)
        };
    }

    std::expected<void, std::error_code> WindowManager::set_ex_style(LONG new_flags) const noexcept {
        return set_long(GWL_EXSTYLE, new_flags);
    }

    std::expected<void, std::error_code> WindowManager::set_style(LONG new_flags) const noexcept {
        return set_long(GWL_STYLE, new_flags);
    }

    std::expected<WNDPROC, std::error_code> WindowManager::set_window_proc(WNDPROC proc) const noexcept {
        const auto prev = set_long_ptr(GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(proc));
        if (!prev)
            return std::unexpected(prev.error());
        return reinterpret_cast<WNDPROC>(*prev);
    }

    std::expected<void, std::error_code> WindowManager::move_window(WindowPos& pos, WindowSize size) const noexcept
    {
        if (!MoveWindow(_hwnd, pos.x, pos.y, size.w, size.h, FALSE))
            return std::unexpected(last_win32_error());

        return {};
    }

    HRESULT WindowManager::set_dark_theme(BOOL dark) const noexcept
    {
        BOOL value = dark;
        return DwmSetWindowAttribute(_hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
    }

    std::expected<void, std::error_code> WindowManager::exclude_from_capture(bool exclude) const noexcept
    {
        if (!SetWindowDisplayAffinity(_hwnd, exclude ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE))
            return std::unexpected(last_win32_error());
        return {};
    }

    std::expected<void, std::error_code> WindowManager::use_alpha_channel() const noexcept
    {
        if (!SetLayeredWindowAttributes(_hwnd, RGB(0, 0, 0), 255, LWA_ALPHA))
            return std::unexpected(last_win32_error());

        return {};
    }

    std::expected<void, std::error_code> WindowManager::set_long(int index, LONG value) const noexcept
    {
        SetLastError(0);
        const auto prev = SetWindowLongW(_hwnd, index, value);
        if (prev == 0 && GetLastError() != 0)
            return std::unexpected(last_win32_error());
        return {};
    }
    std::expected<LONG_PTR, std::error_code> WindowManager::set_long_ptr(int index, LONG_PTR value) const noexcept {
        SetLastError(0);
        const auto prev = SetWindowLongPtrW(_hwnd, index, value);
        if (prev == 0 && GetLastError() != 0)
            return std::unexpected(last_win32_error());
        return prev;
    }

    std::expected<std::unique_ptr<WindowManager>, std::string> create_window(WNDCLASSEX& wc, const WindowCreationInfo& params)
    {
        //NOTE: I don't think it's possible to disable aero in w10/11, however, it's cool to have more checks that can break whole shit
        auto dwm_enabled = BOOL{FALSE};
        if (FAILED(DwmIsCompositionEnabled(&dwm_enabled)))
        {
            return std::unexpected("Please enable AERO composition.");
        }

        std::memset(&wc, 0, sizeof(wc));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_CLASSDC;
        wc.lpfnWndProc = params.wnd_proc;
        wc.hInstance = params.instance;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr;
        wc.lpszClassName = params.class_name.c_str();

        const auto res = RegisterClassExA(&wc);
        if (!res) {
            const auto win32_error = last_win32_error();
            return std::unexpected(std::format("Failed to register window class - {} - {}", win32_error.value(), win32_error.message()));
        }

        const auto hwnd = CreateWindowEx(params.ex_style, wc.lpszClassName, params.window_name.c_str(), params.style,
                                         params.pos.first, params.pos.second, params.size.first, params.size.second,
                                         nullptr, nullptr, wc.hInstance, nullptr);
        if (!hwnd) {
            const auto win32_error = last_win32_error();
            return std::unexpected(std::format("Failed to create window. - {} - {}", win32_error.value(), win32_error.message()));
        }
        return std::make_unique<WindowManager>(hwnd);
    }
}
