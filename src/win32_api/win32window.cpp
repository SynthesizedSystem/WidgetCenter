#include "win32window.h"

bool Win32Window::Create(uint32_t width, uint32_t height, const wchar_t* title,
                         Win32Window* window_ref) {
  static HINSTANCE instance = GetModuleHandleW(nullptr);
  WNDCLASSEXW window_class = {
      .cbSize = sizeof(WNDCLASSEXW),
      .style = CS_VREDRAW | CS_HREDRAW,
      .lpfnWndProc = &Win32Window::Proc,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = instance,
      .hIcon = nullptr,
      .hCursor = LoadCursorA(nullptr, IDC_ARROW),
      .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
      .lpszMenuName = nullptr,
      .lpszClassName = title,
      .hIconSm = nullptr};
  if (!RegisterClassExW(&window_class)) {
    LOG_FATAL("Failed to register Window Class");
    return false;
  }

  uint64_t window_ex_style = WS_EX_NOREDIRECTIONBITMAP;
  uint64_t window_style = WS_OVERLAPPEDWINDOW;
  int window_x = CW_USEDEFAULT;
  int window_y = CW_USEDEFAULT;
  RECT window_rect = {.left = 0,
                      .top = 0,
                      .right = static_cast<LONG>(width),
                      .bottom = static_cast<LONG>(height)};
  AdjustWindowRectEx(&window_rect, window_style, false, window_ex_style);
  int calculated_width = window_rect.right - window_rect.left;
  int calculated_height = window_rect.bottom - window_rect.top;

  HWND win32_window_handle = CreateWindowExW(
      window_ex_style, title, title, window_style, window_x, window_y,
      calculated_width, calculated_height, nullptr, nullptr, instance, nullptr);

  SetWindowLongPtrW(win32_window_handle, GWLP_USERDATA,
                    reinterpret_cast<LONG_PTR>(window_ref));
  ShowWindow(win32_window_handle, SW_SHOWNORMAL);
  *window_ref = Win32Window(win32_window_handle);

  return true;
}

LRESULT Win32Window::Proc(HWND window, uint32_t msg, WPARAM wparam,
                          LPARAM lparam) {
  switch (msg) {
    case WM_SIZE: {
      auto win32_window = reinterpret_cast<Win32Window*>(
          GetWindowLongPtrW(window, GWLP_USERDATA));
      uint32_t new_width = LOWORD(lparam);
      uint32_t new_height = HIWORD(lparam);
      LOG_TRACE("WM_SIZE: %d %d", new_width, new_height);
      win32_window->resize_callback_(LOWORD(lparam), HIWORD(lparam));
      return 0;
    }
    case WM_DESTROY: {
      LOG_TRACE("WM_DESTORY");
      PostQuitMessage(0);
      return 0;
    }
    default: {
      return DefWindowProcW(window, msg, wparam, lparam);
    }
  }
}

Win32Window::Win32Window(HWND win32_window_handle)
    : win32_window_(win32_window_handle) {}

void Win32Window::RunMessagePump(UpdateLoopCallback loop_attachment) {
  MSG msg = {};
  while (GetMessageW(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
    loop_attachment();
  }
}
