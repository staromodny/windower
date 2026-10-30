#include <format>
#include <windower.hpp>

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps{};
            const auto hdc = BeginPaint(hWnd, &ps);

            const char* text = "Hello from Windower!";
            TextOutA(hdc, 20, 20, text, static_cast<int>(strlen(text)));
            EndPaint(hWnd, &ps);
            return 0;
        }

    case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            return 0;
        }
    default:
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInst, [[maybe_unused]] HINSTANCE hInstPrev, [[maybe_unused]] PSTR cmdline, [[maybe_unused]] int cmdshow)
{
    static constexpr auto WINDOW_EX_FLAGS = WS_EX_TOPMOST;
    static constexpr auto WINDOW_SIZE_X = 350;
    static constexpr auto WINDOW_SIZE_Y = 230;

    static constexpr auto WINDOW_CLASS_NAME = "WindowerExample";
    static constexpr auto WINDOW_NAME = "TestWindow";

    const auto window_creation_flags = windower::WindowCreationInfo{
        .instance = hInst,
        .class_name = WINDOW_CLASS_NAME,
        .window_name = WINDOW_NAME,
        .wnd_proc = &WndProc,
        .ex_style = WINDOW_EX_FLAGS,
        .style = WS_OVERLAPPEDWINDOW,
        .size = {WINDOW_SIZE_X, WINDOW_SIZE_Y},
        .pos = {300,400}
    };

    WNDCLASSEX wc{};
    const auto window_creation_res = create_window(wc, window_creation_flags);
    if (!window_creation_res) {
        return MessageBox(nullptr,window_creation_res.error().c_str(),"",MB_OK | MB_ICONERROR);
    }
    const auto& window = *window_creation_res;

    const auto set_dark_mode_res = window->set_dark_theme(TRUE);
    if (!SUCCEEDED(set_dark_mode_res)) {
        const auto dark_mode_error = std::format("Failed to set dark mode. [{}]",set_dark_mode_res);
        MessageBox(nullptr,dark_mode_error.c_str(),"",MB_OK | MB_ICONWARNING);
    }

    if (!window->show(SW_SHOW))
        return MessageBox(nullptr,"Failed to show window.","",MB_OK | MB_ICONERROR);

    bool done = false;
    while (!done)
    {
        auto msg = MSG();
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
    }

    UnregisterClassA(wc.lpszClassName, wc.hInstance);
    return 0;
}