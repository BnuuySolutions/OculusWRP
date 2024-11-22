#include "CRiftController.h"

CRiftController::CRiftController(std::string serialNumber, bool isRight) :
  m_unObjectId(vr::k_unTrackedDeviceIndexInvalid),
  m_ulPropertyContainer(vr::k_ulInvalidPropertyContainer),
  m_serialNumber(serialNumber),
  m_isActivated(false),
  m_isRight(isRight),
  m_pose()
{}

vr::EVRInitError CRiftController::Activate(uint32_t unObjectId) {
  m_unObjectId = unObjectId;
  m_ulPropertyContainer = vr::VRProperties()->TrackedDeviceToPropertyContainer(m_unObjectId);
  m_isActivated = true;

  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_TrackingSystemName_String, "oculus");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ModelNumber_String, !m_isRight ? "Oculus CV1 (Left Controller)" : "Oculus CV1 (Right Controller)");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_ManufacturerName_String, "Oculus");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_RenderModelName_String, !m_isRight ? "oculus_cv1_controller_left" : "oculus_cv1_controller_right");
  vr::VRProperties()->SetStringProperty(m_ulPropertyContainer, vr::Prop_SerialNumber_String, m_serialNumber.c_str());

  vr::VRProperties()->SetUint64Property(m_ulPropertyContainer, vr::Prop_CurrentUniverseId_Uint64, 1);

  return vr::VRInitError_None;
}

void CRiftController::EnterStandby() {
  vr::VRDriverLog()->Log("HMD has been put into standby.");
}

void* CRiftController::GetComponent(const char* pchComponentNameAndVersion) {
  if (0 == strcmp(pchComponentNameAndVersion, vr::IVRDisplayComponent_Version)) {
    return (vr::IVRDisplayComponent*)this;
  }
  return nullptr;
}

void CRiftController::DebugRequest(const char* pchRequest, char* pchResponseBuffer, uint32_t unResponseBufferSize) {
  if (unResponseBufferSize >= 1)
    pchResponseBuffer[0] = 0;
}

vr::DriverPose_t CRiftController::GetPose() {
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
  pose.poseTimeOffset = 0;
  pose.result = vr::TrackingResult_Running_OK;
  return pose;
}

void CRiftController::Deactivate() {
  m_unObjectId = vr::k_unTrackedDeviceIndexInvalid;
  m_isActivated = false;
}

void CRiftController::UpdatePose(ovrPoseStated* pose) {
  if (m_unObjectId != vr::k_unTrackedDeviceIndexInvalid) {
    m_pose = *pose;
    vr::VRServerDriverHost()->TrackedDevicePoseUpdated(m_unObjectId, GetPose(), sizeof(vr::DriverPose_t));
  }
}
