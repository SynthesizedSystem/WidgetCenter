#pragma once
#include "apps/base/application.h"

class SandboxApplication final : public Application {
  template <typename T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;

 public:
  SandboxApplication(const std::wstring& title);
  ~SandboxApplication() override;

 protected:
  bool Initialize() override;
  bool Load() override;
  void OnResize(const uint32_t width, const uint32_t height) override;
  void Render() override;
  void Update() override;

 private:
  bool CreateSwapchainResources();
  void DestroySwapchainResources();

  ComPtr<IDXGIFactory2> dxgi_factory_ = nullptr;
  ComPtr<ID3D11Device> device_ = nullptr;
  ComPtr<ID3D11DeviceContext> device_context_ = nullptr;
  ComPtr<IDXGISwapChain1> swapchain_ = nullptr;
  ComPtr<ID3D11RenderTargetView> render_target_ = nullptr;
};