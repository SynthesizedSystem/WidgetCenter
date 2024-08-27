#pragma once

class Win32Window {
  using UpdateLoopCallback = std::function<void()>;
  using ResizeCallback = std::function<void(const uint32_t, const uint32_t)>;

 public:
  static bool Create(uint32_t width, uint32_t height, const wchar_t* title,
                     Win32Window* window_ref);
  Win32Window() = default;
  ~Win32Window() = default;
  void RunMessagePump(UpdateLoopCallback loop_attachment = []() {});

  inline HWND win32_window() const { return win32_window_; }
  inline void SetResizeCallback(ResizeCallback resize_callback) {
    resize_callback_ = resize_callback;
  }

 private:
  static LRESULT CALLBACK Proc(HWND window, uint32_t msg, WPARAM wparam,
                               LPARAM lparam);
  Win32Window(HWND win32_window_handle);

  HWND win32_window_ = nullptr;
  ResizeCallback resize_callback_ = [](const uint32_t width,
                                       const uint32_t height) {};
};