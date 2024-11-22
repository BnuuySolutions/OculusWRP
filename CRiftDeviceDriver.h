#pragma once

#include <openvr_driver.h>
#include <ovrsdk/ovrsdk.hpp>

using namespace OVR;
using namespace OVR::HeadsetPlugin;
using namespace OVRInterface;

class CRiftDeviceDriver : public vr::ITrackedDeviceServerDriver, public vr::IVRDisplayComponent {
public:
  CRiftDeviceDriver(std::string serialNumber);

public:
  // Inherited via ITrackedDeviceServerDriver
  vr::EVRInitError Activate(uint32_t unObjectId) override;
  void EnterStandby() override;
  void* GetComponent(const char* pchComponentNameAndVersion) override;
  void DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) override;
  vr::DriverPose_t GetPose() override;
  void Deactivate() override;

public:
  // Inherited via IVRDisplayComponent
  bool IsDisplayOnDesktop() override;
  bool IsDisplayRealDisplay() override;
  void GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) override;
  void GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;
  void GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) override;
  vr::DistortionCoordinates_t ComputeDistortion(vr::EVREye eEye, float fU, float fV) override;
  void GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) override;
  bool ComputeInverseDistortion(vr::HmdVector2_t* pResult, vr::EVREye eEye, uint32_t unChannel, float fU, float fV) override;

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
};
