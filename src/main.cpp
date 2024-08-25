LRESULT CALLBACK Win32WindowProc(HWND window, uint32_t msg, WPARAM wparam,
                                 LPARAM lparam) {
  switch (msg) {
    case WM_CREATE: {
      return 0;
    }
    case WM_DESTROY: {
      PostQuitMessage(0);
      return 0;
    }
    default: {
      return DefWindowProcW(window, msg, wparam, lparam);
    }
  }
}

int main() {
  HINSTANCE instance = GetModuleHandleW(nullptr);
  const wchar_t* class_name = L"Widget Center";

  WNDCLASSEXW window_class = {
      .cbSize = sizeof(WNDCLASSEXW),
      .style = CS_VREDRAW | CS_HREDRAW,
      .lpfnWndProc = Win32WindowProc,
      .cbClsExtra = 0,
      .cbWndExtra = 0,
      .hInstance = instance,
      .hIcon = nullptr,
      .hCursor = nullptr,
      .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
      .lpszMenuName = nullptr,
      .lpszClassName = class_name,
      .hIconSm = nullptr};
  RegisterClassExW(&window_class);

  RECT workArea;
  SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);
  int screen_height = workArea.bottom - workArea.top;

  uint64_t window_ex_style = 0;
  uint64_t window_style = WS_POPUP;
  int y_offset = 25;
  int window_x = 0;
  int window_y = y_offset;
  int window_width = 800;
  int window_height = screen_height - 2 * y_offset;
  HWND main_window = CreateWindowExW(
      window_ex_style, class_name, class_name, window_style, window_x, window_y,
      window_width, window_height, nullptr, nullptr, instance, nullptr);
  ShowWindow(main_window, SW_SHOWNORMAL);

  // Message pump
  MSG msg = {};
  while (GetMessageA(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageA(&msg);
  }

  return 0;
}