#pragma once

#include <memory>
#include <openvr_driver.h>
#include "CRiftDeviceDriver.h"
#include "CRiftController.h"

class CServerDriver : public vr::IServerTrackedDeviceProvider {
public:
  std::shared_ptr<CRiftDeviceDriver> riftDevice;
  std::shared_ptr<CRiftController> leftController;
  std::shared_ptr<CRiftController> rightController;

  virtual vr::EVRInitError Init(vr::IVRDriverContext* pDriverContext);
  virtual void Cleanup();
  virtual const char* const* GetInterfaceVersions() { return vr::k_InterfaceVersions; }
  virtual void RunFrame();

  virtual bool ShouldBlockStandbyMode() { return false; }
  virtual void EnterStandby() {}
  virtual void LeaveStandby() {}
};

extern CServerDriver g_serverDriver;
