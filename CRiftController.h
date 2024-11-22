#pragma once

#include <openvr_driver.h>
#include <ovrsdk/ovrsdk.hpp>

using namespace OVR;
using namespace OVR::HeadsetPlugin;
using namespace OVRInterface;

class CRiftController : public vr::ITrackedDeviceServerDriver {
public:
  CRiftController(std::string serialNumber, bool isRight = false);

public:
  // Inherited via ITrackedDeviceServerDriver
  vr::EVRInitError Activate(uint32_t unObjectId) override;
  void EnterStandby() override;
  void* GetComponent(const char* pchComponentNameAndVersion) override;
  void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
  vr::DriverPose_t GetPose() override;
  void Deactivate() override;

public:
  vr::PropertyContainerHandle_t GetPropertyContainer() { return m_ulPropertyContainer; }
  std::string GetSerialNumber() { return m_serialNumber; }
  bool IsActivated() { return m_isActivated; }

  void UpdatePose(ovrPoseStated* pose);

private:
  vr::PropertyContainerHandle_t m_ulPropertyContainer;
  vr::TrackedDeviceIndex_t m_unObjectId;
  bool m_isActivated;

  std::string m_serialNumber;

  ovrPoseStated m_pose;
  bool m_isRight;
};
