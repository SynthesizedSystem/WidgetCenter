#pragma once
#include "win32_api/win32window.h"

class Application {
 public:
  Application(const std::wstring& title);
  virtual ~Application();
  void Run();

  inline Win32Window window() const { return window_; }
  inline uint32_t width() const { return width_; }
  inline uint32_t height() const { return height_; }

 protected:
  virtual void Cleanup();
  virtual bool Initialize();
  virtual bool Load() = 0;
  virtual void OnResize(const uint32_t width, const uint32_t height) = 0;
  virtual void Render() = 0;
  virtual void Update() = 0;

 private:
  Win32Window window_;
  uint32_t width_ = 800;
  uint32_t height_ = 600;
  std::wstring title_;
};