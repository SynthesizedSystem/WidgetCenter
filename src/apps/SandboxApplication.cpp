#include "SandboxApplication.h"

SandboxApplication::SandboxApplication(const std::wstring& title)
    : Application(title) {}

SandboxApplication::~SandboxApplication() {}

bool SandboxApplication::Initialize() {
  LOG_TRACE("Initializing Sandbox Application");
  if (!Application::Initialize()) {
    LOG_FATAL("Failed to initialize Application");
    return false;
  }

  if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory_)))) {
    LOG_ERROR("Failed to create DXGI Factory");
    return false;
  }

  constexpr D3D_FEATURE_LEVEL device_feature_level = D3D_FEATURE_LEVEL_11_0;
  if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
                               &device_feature_level, 1, D3D11_SDK_VERSION,
                               &device_, nullptr, &device_context_))) {
    LOG_ERROR("Failed to create D3D11Device");
    return false;
  }

  DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {
      .Width = width(),
      .Height = height(),
      .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
      .Stereo = FALSE,
      .SampleDesc = {.Count = 1, .Quality = 0},
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = 2,
      .Scaling = DXGI_SCALING_STRETCH,
      .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
      .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
      .Flags = 0};

  if (FAILED(dxgi_factory_->CreateSwapChainForHwnd(
          device_.Get(), window().win32_window(), &swapchain_desc, nullptr,
          nullptr, &swapchain_))) {
    LOG_ERROR("Failed to create DXGI Factory");
    return false;
  }

  if (!CreateSwapchainResources()) {
    LOG_ERROR("Failed to create Swapchain Resources");
    return false;
  }

  return true;
}

bool SandboxApplication::Load() {
  LOG_TRACE("Loading Sandbox Application");
  /*if (true) {
    LOG_FATAL("Failed to load Application");
  }*/
  return true;
}

void SandboxApplication::OnResize(const uint32_t width, const uint32_t height) {
  device_context_->Flush();
  DestroySwapchainResources();
  if (FAILED(swapchain_->ResizeBuffers(0, width, height,
                                       DXGI_FORMAT_B8G8R8A8_UNORM, 0))) {
    LOG_ERROR("Failed to resize swapchain");
    return;
  }
  CreateSwapchainResources();
}

void SandboxApplication::Render() {
  D3D11_VIEWPORT viewport = {.TopLeftX = 0,
                             .TopLeftY = 0,
                             .Width = static_cast<float>(width()),
                             .Height = static_cast<float>(height()),
                             .MinDepth = 0.0f,
                             .MaxDepth = 1.0f};

  constexpr float clear_color[4] = {0.1f, 0.2f, 0.4f, 1.0f};
  device_context_->ClearRenderTargetView(render_target_.Get(), clear_color);
  device_context_->RSSetViewports(1, &viewport);
  device_context_->OMSetRenderTargets(1, render_target_.GetAddressOf(),
                                      nullptr);
  swapchain_->Present(1, 0);
}

void SandboxApplication::Update() {}

bool SandboxApplication::CreateSwapchainResources() {
  ComPtr<ID3D11Texture2D> back_buffer = nullptr;
  if (FAILED(swapchain_->GetBuffer(0, IID_PPV_ARGS(&back_buffer)))) {
    LOG_ERROR("Failed to get Buffer from Swapchain");
    return false;
  }

  if (FAILED(device_->CreateRenderTargetView(back_buffer.Get(), nullptr,
                                             &render_target_))) {
    LOG_ERROR("Failed to create RTV from Back Buffer");
    return false;
  }
  return true;
}

void SandboxApplication::DestroySwapchainResources() { render_target_.Reset(); }
