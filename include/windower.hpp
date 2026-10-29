
#pragma once
#include <Windows.h>

#include <expected>
#include <memory>
#include <string>

namespace windower
{
    struct WindowSize
    {
        int w{};
        int h{};
    };

    struct WindowPos
    {
        int x{}, y{};
    };

    struct WindowCreationInfo
    {
        HINSTANCE instance{};
        std::string class_name{};
        std::string window_name{};
        WNDPROC wnd_proc{};
        DWORD ex_style{0};
        DWORD style{WS_OVERLAPPEDWINDOW}; // normal window style
        std::pair<int, int> size{800, 600};
        std::pair<int, int> pos{100, 100};
    };

    [[nodiscard]]
    static std::error_code last_win32_error() noexcept {
        return std::error_code{static_cast<int>(GetLastError()), std::system_category()};
    }

    class WindowManager
    {
    public:
        WindowManager() = default;

        explicit WindowManager(HWND hwnd) : _hwnd(hwnd) {}

        __forceinline operator HWND() const noexcept { return _hwnd; }

        [[nodiscard]]
        bool show(int cmd_show) const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> destroy() const noexcept;

        [[nodiscard]]
        std::expected<WindowSize, std::error_code> get_size() const noexcept;

        [[nodiscard]]
        std::expected<WindowPos, std::error_code> get_pos() const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> set_ex_style(LONG flags) const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> set_style(LONG flags) const noexcept;

        [[nodiscard]]
        std::expected<WNDPROC, std::error_code> set_window_proc(WNDPROC proc) const noexcept;

        [[nodiscard]]
        HRESULT set_dark_theme(BOOL dark) const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> move_window(WindowPos& pos, WindowSize size) const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> exclude_from_capture(bool exclude = false) const noexcept;

        [[nodiscard]]
        std::expected<void, std::error_code> use_alpha_channel() const noexcept;
    private:
        [[nodiscard]]
        std::expected<void, std::error_code> set_long(int index, LONG value) const noexcept;

        [[nodiscard]]
        std::expected<LONG_PTR, std::error_code> set_long_ptr(int index, LONG_PTR value) const noexcept;

        HWND _hwnd{};
    };

    [[nodiscard]]
    std::expected<std::unique_ptr<WindowManager>, std::string> create_window(WNDCLASSEX& wc, const WindowCreationInfo& params);

    [[nodiscard]]
    bool pump_messages();
}