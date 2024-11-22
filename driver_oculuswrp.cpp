#include <windows.h>
#include <openvr_driver.h>
#include "CServerDriver.h"

#if defined(_WIN32)
#define HMD_DLL_EXPORT extern "C" __declspec(dllexport)
#define HMD_DLL_IMPORT extern "C" __declspec(dllimport)
#else
#error "Unsupported Platform."
#endif

HMD_DLL_EXPORT void* HmdDriverFactory(const char* pInterfaceName, int* pReturnCode) {
  if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName)) {
    return &g_serverDriver;
  }
  if (pReturnCode) {
    *pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
  }
  return nullptr;
}
