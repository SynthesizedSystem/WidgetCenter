#include "apps/SandboxApplication.h"

int main() {
  SandboxApplication app{L"Sandbox"};
  LOG_TRACE("Starting application");
  app.Run();

  return 0;
}