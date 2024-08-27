#include "application.h"

Application::Application(const std::wstring& title) : title_(title) {}

Application::~Application() { Cleanup(); }

void Application::Run() {
  if (!Initialize()) {
    return;
  }
  if (!Load()) {
    return;
  }

  window_.RunMessagePump([this]() {
    Update();
    Render();
  });
}

void Application::Cleanup() {}

bool Application::Initialize() {
  if (!Win32Window::Create(width_, height_, title_.data(), &window_)) {
    return false;
  }
  window_.SetResizeCallback(
      [this](const uint32_t width, const uint32_t height) {
        this->OnResize(width, height);
      });
  return true;
}
