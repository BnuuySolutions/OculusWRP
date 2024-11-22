#include "CRiftDeviceDriver.h"

CRiftDeviceDriver::CRiftDeviceDriver(std::string serialNumber) :
  m_unObjectId(vr::k_unTrackedDeviceIndexInvalid),
  m_ulPropertyContainer(vr::k_ulInvalidPropertyContainer),
  m_serialNumber(serialNumber),
  m_isActivated(false),
  m_pose()
{}

vr::EVRInitError CRiftDeviceDriver::Activate(uint32_t unObjectId) {
  m_unObjectId = unObjectId;
  m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);
  m_isActivated = true;

  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_TrackingSystemName_String, "oculus");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, "Oculus CV1");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ManufacturerName_String, "Oculus");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_RenderModelName_String, "generic_hmd");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_SerialNumber_String, m_serialNumber.c_str());

  vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_CurrentUniverseId_Uint64, 1);

  const float ipd = vr::VRSettings()->GetFloat(vr::k_pch_SteamVR_Section, vr::k_pch_SteamVR_IPD_Float);
  vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserIpdMeters_Float, ipd);

  vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_DisplayFrequency_Float, 0.f);
  vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_UserHeadToEyeDepthMeters_Float, 0.f);
  vr::VRProperties()->SetFloatProperty(m_ulPropertyContainer, vr::Prop_SecondsFromVsyncToPhotons_Float, 0.f);

  vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_EdidVendorID_Int32, 0xD23E);
  vr::VRProperties()->SetInt32Property(m_ulPropertyContainer, vr::Prop_EdidProductID_Int32, 0x0004);

  vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_IsOnDesktop_Bool, false);
  vr::VRProperties()->SetBoolProperty(m_ulPropertyContainer, vr::Prop_DisplayAllowNightMode_Bool, true);

  return vr::VRInitError_None;
}

void CRiftDeviceDriver::EnterStandby() {
  vr::VRDriverLog()->Log("HMD has been put into standby.");
}

void* CRiftDeviceDriver::GetComponent(const char* pchComponentNameAndVersion) {
  if (0 == strcmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) {
    return (vr::IVRDisplayComponent*)this;
  }
  return nullptr;
}

void CRiftDeviceDriver::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
  if (unResponseBufferSize >= 1)
    pchResponseBuffer[0] = 0;
}

vr::DriverPose_t CRiftDeviceDriver::GetPose() {
  vr::DriverPose_t pose = { 0 };
  pose.qWorldFromDriverRotation.w = 1.f;
  pose.qDriverFromHeadRotation.w = 1.f;

  pose.vecPosition[0] = m_pose.ThePose.Position.x;
  pose.vecPosition[1] = m_pose.ThePose.Position.y;
  pose.vecPosition[2] = m_pose.ThePose.Position.z;

  pose.vecVelocity[0] = m_pose.LinearVelocity.x;
  pose.vecVelocity[1] = m_pose.LinearVelocity.y;
  pose.vecVelocity[2] = m_pose.LinearVelocity.z;

  pose.vecAcceleration[0] = m_pose.LinearAcceleration.x;
  pose.vecAcceleration[1] = m_pose.LinearAcceleration.y;
  pose.vecAcceleration[2] = m_pose.LinearAcceleration.z;

  pose.qRotation.w = m_pose.ThePose.Orientation.w;
  pose.qRotation.x = m_pose.ThePose.Orientation.x;
  pose.qRotation.y = m_pose.ThePose.Orientation.y;
  pose.qRotation.z = m_pose.ThePose.Orientation.z;

  pose.vecAngularVelocity[0] = m_pose.AngularVelocity.x;
  pose.vecAngularVelocity[1] = m_pose.AngularVelocity.y;
  pose.vecAngularVelocity[2] = m_pose.AngularVelocity.z;

  pose.vecAngularAcceleration[0] = m_pose.AngularAcceleration.x;
  pose.vecAngularAcceleration[1] = m_pose.AngularAcceleration.y;
  pose.vecAngularAcceleration[2] = m_pose.AngularAcceleration.z;

  pose.poseIsValid = true;
  pose.deviceIsConnected = true;
  pose.shouldApplyHeadModel = true;
  pose.willDriftInYaw = true;
  pose.poseTimeOffset = 0;
  pose.result = vr::TrackingResult_Running_OK;
  return pose;
}

void CRiftDeviceDriver::Deactivate() {
  m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
  m_isActivated = false;
}

bool CRiftDeviceDriver::IsDisplayOnDesktop() {
  return false;
}

bool CRiftDeviceDriver::IsDisplayRealDisplay() {
  return true;
}

void CRiftDeviceDriver::GetRecommendedRenderTargetSize(uint32_t* pnWidth, uint32_t* pnHeight) {
  *pnWidth = 2160;
  *pnHeight = 1200;
}

void CRiftDeviceDriver::GetEyeOutputViewport(vr::EVREye eEye, uint32_t* pnX, uint32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) {
  *pnY = 0;
  *pnWidth = 2160 / 2;
  *pnHeight = 1200;
  if (eEye == vr::Eye_Left) {
    *pnX = 0;
  } else {
    *pnX = 2160 / 2;
  }
}

void CRiftDeviceDriver::GetProjectionRaw(vr::EVREye eEye, float* pfLeft, float* pfRight, float* pfTop, float* pfBottom) {
  *pfLeft = -1.0;
  *pfRight = 1.0;
  *pfTop = -1.0;
  *pfBottom = 1.0;
}

vr::DistortionCoordinates_t CRiftDeviceDriver::ComputeDistortion(vr::EVREye eEye, float fU, float fV) {
  vr::DistortionCoordinates_t coordinates{};
  coordinates.rfBlue[0] = fU;
  coordinates.rfBlue[1] = fV;
  coordinates.rfGreen[0] = fU;
  coordinates.rfGreen[1] = fV;
  coordinates.rfRed[0] = fU;
  coordinates.rfRed[1] = fV;
  return coordinates;
}

void CRiftDeviceDriver::GetWindowBounds(int32_t* pnX, int32_t* pnY, uint32_t* pnWidth, uint32_t* pnHeight) {
  *pnX = 0;
  *pnY = 0;
  *pnWidth = 2160;
  *pnHeight = 1200;
}

bool CRiftDeviceDriver::ComputeInverseDistortion(vr::HmdVector2_t* pResult, vr::EVREye eEye, uint32_t unChannel, float fU, float fV) {
  return false;
}

void CRiftDeviceDriver::UpdatePose(ovrPoseStated* pose) {
  if (m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
    m_pose = *pose;
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, GetPose(), sizeof(vr::DriverPose_t));
  }
}
