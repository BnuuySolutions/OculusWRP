#include "CServerDriver.h"
#include <ovrsdk/ovrsdk.hpp>
#include <format>

using namespace OVR;
using namespace OVR::HeadsetPlugin;
using namespace OVRInterface;

#pragma region "OVRGatekeeperInterfaceImplementation"
class IGatekeeper : public OVRInterface::IUnknown {
public:
  virtual uint8_t FetchGatekeeper(const char* gkName) = 0;
};

class OVRGatekeeperInterfaceImplementation : public IGatekeeper {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid == 0x32AA1AA5EF7D2559 || iid - 1 <= 1) {
      return (IGatekeeper*)this;
    }
    return nullptr;
  }

  uint8_t FetchGatekeeper(const char* gkName) {
    return 2;
  }
};

class OVRInterfaceFactoryOVRGatekeeperInterfaceImplementation : public IInterfaceFactory {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 2) {
      return (IInterfaceFactory*)this;
    }
    return nullptr;
  }

  void* GetInterface(void** outInterface, uint64_t classId) {
    if (classId == 0x5C048F7B40366B52) {
      *outInterface = new OVRGatekeeperInterfaceImplementation;
      return outInterface;
    }
    *outInterface = 0;
    return outInterface;
  }
};
#pragma endregion

#pragma region "OVRCrashDumpImplementation"
class IOVRCrashDump : public OVRInterface::IUnknown {
public:
  virtual void IOVRCrashDump_Unk05() {}
};

class OVRCrashDumpImplementation : public IOVRCrashDump {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid == 0xC223699272206867 || iid - 1 <= 1) {
      return (IOVRCrashDump*)this;
    }
    return nullptr;
  }
};

class OVRInterfaceFactoryOVRCrashDumpImplementation : public IInterfaceFactory {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 2) {
      return (IInterfaceFactory*)this;
    }
    return nullptr;
  }

  void* GetInterface(void** outInterface, uint64_t classId) {
    if (classId == 0xE79671E08A153048) {
      *outInterface = new OVRCrashDumpImplementation;
      return outInterface;
    }
    *outInterface = 0;
    return outInterface;
  }
};
#pragma endregion

#pragma region "OVRTelemetryInterfaceImplementation"
class IOVRTelemetry : public OVRInterface::IUnknown {
public:
  virtual void IOVRTelemetry_Unk05() {}
};

class OVRTelemetryInterfaceImplementation : public IOVRTelemetry {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid == 0xB00B00088073380 || iid - 1 <= 1) {
      return (IOVRTelemetry*)this;
    }
    return nullptr;
  }
};

class OVRInterfaceFactoryOVRTelemetryInterfaceImplementation : public IInterfaceFactory {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 2) {
      return (IInterfaceFactory*)this;
    }
    return nullptr;
  }

  void* GetInterface(void** outInterface, uint64_t classId) {
    if (classId == 0xB00B00088073381) {
      *outInterface = new OVRTelemetryInterfaceImplementation;
      return outInterface;
    }
    *outInterface = 0;
    return outInterface;
  }
};
#pragma endregion

#pragma region "OVRTimeCriticalTelemetryInterfaceImplementation"
class OVRInterfaceFactoryOVRTimeCriticalTelemetryInterfaceImplementation : public IInterfaceFactory {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 2) {
      return (IInterfaceFactory*)this;
    }
    return nullptr;
  }

  void* GetInterface(void** outInterface, uint64_t classId) {
    *outInterface = 0;
    return outInterface;
  }
};
#pragma endregion

#pragma region "OVRLogWriteImplementation"
namespace OVR {
namespace Service {

static void DriverLogVarArgs( const char *pMsgFormat, va_list args )
{
	char buf[ 1024 * 8 ];
	vsnprintf_s( buf, sizeof( buf ), pMsgFormat, args );

	vr::VRDriverLog()->Log( buf );
}


void DriverLog( const char *pMsgFormat, ... )
{
	va_list args;
	va_start( args, pMsgFormat );

	DriverLogVarArgs( pMsgFormat, args );

	va_end( args );
}

typedef enum ovrLogLevelType_ {
  OVR_LOG_LEVEL_TYPE_DISABLED = 0,
  OVR_LOG_LEVEL_TYPE_TRACE = 1,
  OVR_LOG_LEVEL_TYPE_DEBUG = 2,
  OVR_LOG_LEVEL_TYPE_INFO = 3,
  OVR_LOG_LEVEL_TYPE_WARNING = 4,
  OVR_LOG_LEVEL_TYPE_ERROR = 5
} ovrLogLevelType;

const char* ovrLogLevelType_ToString(ovrLogLevelType logLevel) {
  switch (logLevel) {
  case 0: return " {DISABLED}";
  case 1: return " {TRACE}   ";
  case 2: return " {DEBUG}   ";
  case 3: return " {INFO}    ";
  case 4: return " {WARNING} ";
  case 5: return " {!ERROR!} ";
  default: return " {???}     ";
  }
}

class IOVRLogWrite : public OVRInterface::IUnknown {
public:
  virtual void Log(ovrLogLevelType logLevel, const char* category, const char* msg) = 0;
};

class OVRLogWriteImplementation : public IOVRLogWrite {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid == 0xB00B00044683AB0 || iid - 1 <= 1) {
      return (IOVRLogWrite*)this;
    }
    return nullptr;
  }

  void Log(ovrLogLevelType logLevel, const char* category, const char* msg) {
    DriverLog("%s[%s] %s", ovrLogLevelType_ToString(logLevel), category, msg);
  }
};

class OVRInterfaceFactoryOVRLogWriteImplementation : public IInterfaceFactory {
  OVR_INTERFACE_IREFCOUNTED_IMPL

public:
  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 2) {
      return (IInterfaceFactory*)this;
    }
    return nullptr;
  }

  void* GetInterface(void** outInterface, uint64_t classId) {
    if (classId == 0x3F5065E1993F5D7B) {
      *outInterface = new OVRLogWriteImplementation;
      return outInterface;
    }
    *outInterface = 0;
    return outInterface;
  }
};

}
}
#pragma endregion

void (*OVRInterfaceInitialize)(IAggregateInterfaceFactory*) = nullptr;
IAggregateInterfaceFactory* (*GetOVRInterfaceFactory)() = nullptr;

class OpenVRRuntimeInterface : public IHeadsetToRuntimeInterface2 {
  OVR_INTERFACE_IREFCOUNTED_IMPL

private:
  CServerDriver* m_serverDriver;
  char m_displaySerial[0x40]{};
  bool m_hasDisplaySerial = false;

public:
  OpenVRRuntimeInterface(CServerDriver* serverDriver) {
    m_serverDriver = serverDriver;
  }

  void* QueryInterface(uint64_t iid) {
    if (iid - 1 <= 1) {
      return (IHeadsetToRuntimeInterface2*)this;
    }
    return nullptr;
  }

  void IHeadsetToRuntimeInterface_Unk11(uint32_t a2, const char* serialNumber, uint32_t eventType, void* a5) {
    if (!m_hasDisplaySerial && eventType == 1) {
      strncpy_s(m_displaySerial, serialNumber, 0xFFFFFFFFFFFFFFFF);
      m_hasDisplaySerial = true;
    }
  }

  void DevicePoseEvent(ovrDeviceType deviceType, ovrPoseStated* devicePose, uint32_t a4) {
    if (deviceType == OVR_DEVICE_TYPE_HMD && m_serverDriver->riftDevice->IsActivated()) {
      m_serverDriver->riftDevice->UpdatePose(devicePose);
    }
    if (deviceType == OVR_DEVICE_TYPE_LEFT_TOUCH && m_serverDriver->leftController->IsActivated()) {
      m_serverDriver->leftController->UpdatePose(devicePose);
    }
    if (deviceType == OVR_DEVICE_TYPE_RIGHT_TOUCH && m_serverDriver->rightController->IsActivated()) {
      m_serverDriver->rightController->UpdatePose(devicePose);
    }
  }

  ovrResult GetDisplayList(int a2, ovrDisplayList* displays) {
    if (displays->Count < 1) {
      displays->Count = 1; // Force array to resize.
      return ovrError_InsufficientArraySize;
    }
    ovrDisplay* display = displays->Display;
    strncpy_s(display->UniqueId, "0123456789", 0xFFFFFFFFFFFFFFFF);
    display->PnpId = 0xD23E;
    strncpy_s(display->DisplaySerial, m_displaySerial, 0xFFFFFFFFFFFFFFFF); // oh god this is janky
    display->Unk0142 = 0x7A39;
    display->Resolution1.w = 2160;
    display->Resolution1.h = 1200;
    display->Resolution2.w = 2160;
    display->Resolution2.h = 1200;
    display->RefreshRate.Numerator = 89998;
    display->RefreshRate.Denominator = 1000;
    display->Unk015C = 1;
    display->Luid = 0x10AE100000000;
    return 0;
  }
};

#pragma region "Unknown"
typedef struct ovrRegisterCommandType_ {
  uint64_t unk0;
  uint64_t unk1;
  void* func0;
  void* func1;
} ovrRegisterCommandType;

void regcmd_func0(const char* cmd, const char* params, const char* desc0, const char* desc1, void* func) {
  printf("Attempted to register command: %s\n", cmd);
}
void regcmd_func1() {
  printf("test1\n");
}
#pragma endregion

vr::EVRInitError CServerDriver::Init(vr::IVRDriverContext* pDriverContext) {
  VR_INIT_SERVER_DRIVER_CONTEXT(pDriverContext);

  riftDevice = std::make_shared<CRiftDeviceDriver>("WMHD00001");
  leftController = std::make_shared<CRiftController>("WMCL00001");
  rightController = std::make_shared<CRiftController>("WMCR00001", true);

  IAggregateInterfaceFactory* interfaceFactory = new OVRAggregateInterfaceFactory;

  #pragma region "Interfaces"
  OVRInterfaceFactoryOVRGatekeeperInterfaceImplementation GatekeeperInterfaceImpl;
  ovrInterfaceDesc GatekeeperInterfaceDesc{
    .ClassId = 0x5C048F7B40366B52,
    .InterfaceId = 0x32AA1AA5EF7D2559
  };
  strncpy_s(GatekeeperInterfaceDesc.ClassName, "OVRGatekeeperInterfaceImplementation", 0x40);
  interfaceFactory->RegisterInterface(&GatekeeperInterfaceImpl, &GatekeeperInterfaceDesc);

  OVRInterfaceFactoryOVRCrashDumpImplementation CrashDumpImpl;
  ovrInterfaceDesc CrashDumpDesc{
    .ClassId = 0xE79671E08A153048,
    .InterfaceId = 0xC223699272206867
  };
  strncpy_s(CrashDumpDesc.ClassName, "OVRCrashDumpImplementation", 0x40);
  interfaceFactory->RegisterInterface(&CrashDumpImpl, &CrashDumpDesc);

  OVR::Service::OVRInterfaceFactoryOVRLogWriteImplementation LogWriteImpl;
  ovrInterfaceDesc LogWriteDesc{
    .ClassId = 0x3F5065E1993F5D7B,
    .InterfaceId = 0xB00B00044683AB0
  };
  strncpy_s(LogWriteDesc.ClassName, "OVRLogWriteImplementation", 0x40);
  interfaceFactory->RegisterInterface(&LogWriteImpl, &LogWriteDesc);

  OVRInterfaceFactoryOVRTelemetryInterfaceImplementation TelemetryInterfaceImpl;
  ovrInterfaceDesc TelemetryInterfaceDesc{
    .ClassId = 0xB00B00088073381,
    .InterfaceId = 0xB00B00088073380
  };
  strncpy_s(TelemetryInterfaceDesc.ClassName, "OVRTelemetryInterfaceImplementation", 0x40);
  interfaceFactory->RegisterInterface(&TelemetryInterfaceImpl, &TelemetryInterfaceDesc);

  OVRInterfaceFactoryOVRTimeCriticalTelemetryInterfaceImplementation TimeCriticalTelemetryInterfaceImpl;
  ovrInterfaceDesc TimeCriticalTelemetryInterfaceDesc{
    .ClassId = 0xB00B00088073381,
    .InterfaceId = 0xB00B00088073380
  };
  strncpy_s(TimeCriticalTelemetryInterfaceDesc.ClassName, "OVRTimeCriticalTelemetryInterfaceImplementation", 0x40);
  interfaceFactory->RegisterInterface(&TimeCriticalTelemetryInterfaceImpl, &TimeCriticalTelemetryInterfaceDesc);
  #pragma endregion

  HMODULE DllLib = LoadLibraryW(L"C:\\Users\\Kaitlyn\\Downloads\\server-plugins\\Rift.dll");
  if (DllLib) {
    OVRInterfaceInitialize = decltype(OVRInterfaceInitialize)(GetProcAddress(DllLib, "OVRInterfaceInitialize"));
    GetOVRInterfaceFactory = decltype(GetOVRInterfaceFactory)(GetProcAddress(DllLib, "GetOVRInterfaceFactory"));

    OVRInterfaceInitialize(interfaceFactory);

    IHeadsetToRuntimeInterface2* openVRRuntimeInterface = new OpenVRRuntimeInterface(this);

    void* outInterface = nullptr;
    GetOVRInterfaceFactory()->GetInterface(&outInterface, 0xEB5208F2B6F376B4); // hardcoded
    IOVRHeadsetPluginAddition* outHeadsetPluginAddition = (IOVRHeadsetPluginAddition*)outInterface;

    //SetEnvironmentVariableW(L"OVREmulationPlugin", L"1");

    IHeadsetInterface* outHeadsetInterface = nullptr;
    if (!outHeadsetPluginAddition->CreateHeadsetInterface(&openVRRuntimeInterface, &outHeadsetInterface)) {
      vr::VRDriverLog()->Log("Headset interface created.");
    }

    ovrRegisterCommandType reg_cmd;
    reg_cmd.unk0 = 0x1010100;
    reg_cmd.unk1 = 0xC700000000;
    reg_cmd.func0 = regcmd_func0;
    reg_cmd.func1 = regcmd_func1;

    outHeadsetInterface->IHeadsetInterface_Unk05((char*)&reg_cmd);
    outHeadsetInterface->IHeadsetInterface_Unk06(1);

    Sleep(5000);
    outHeadsetInterface->IHeadsetInterface_Unk06(2);

    Sleep(2000);
    outHeadsetInterface->IHeadsetInterface_Unk55(0x70);
  }

  vr::VRServerDriverHost()->TrackedDeviceAdded(
    riftDevice->GetSerialNumber().c_str(),
    vr::TrackedDeviceClass_HMD,
    riftDevice.get());

  vr::VRServerDriverHost()->TrackedDeviceAdded(
    leftController->GetSerialNumber().c_str(),
    vr::TrackedDeviceClass_Controller,
    leftController.get());
  vr::VRServerDriverHost()->TrackedDeviceAdded(
    rightController->GetSerialNumber().c_str(),
    vr::TrackedDeviceClass_Controller,
    rightController.get());

  return vr::VRInitError_None;
}

void CServerDriver::Cleanup() {
  // TODO: Shutdown
  riftDevice.reset();
  leftController.reset();
  rightController.reset();

  VR_CLEANUP_SERVER_DRIVER_CONTEXT();
}

void CServerDriver::RunFrame() {
  vr::VREvent_t vrEvent;
  while (vr::VRServerDriverHost()->PollNextEvent(&vrEvent, sizeof(vrEvent))) {
    // TODO
  }
}

CServerDriver g_serverDriver;
