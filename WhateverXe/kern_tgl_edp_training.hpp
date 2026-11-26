// AI 修改: opt-in internal eDP training fix (-wex-edp-fix-training).
// Extracted from kern_xe.cpp so upstream updates merge with a minimal diff.
// The fix installs a TGL LightUpEDP hook that runs the donor PPS off/T12/on
// sink reset and the DDI/PG power-domain repair before the native internal
// light-up. It is the 0.8.2 implementation, moved without logic changes.
//
// Upstream contract: all XE state is reached through Host pointers filled at
// the single call site in XE::processNativePanel. If upstream renames or
// relocates those members, only that small call site fails to compile.
#ifndef KERN_TGL_EDP_TRAINING_HPP
#define KERN_TGL_EDP_TRAINING_HPP
#include <stdint.h>
#include <stddef.h>
#include <mach/mach_types.h>

class KernelPatcher;
namespace XELP { namespace VBT { struct Ports; } }

namespace TGLEDPTraining {
struct State {
 bool lightUpReady {};
 mach_vm_address_t orgLightUp {};
 mach_vm_address_t panelPowerFunction {};
 bool panelPowerFunctionReady {},coldCycleDone {};
 bool trainingFixReady {};
 bool powerDomainReady {};
 uint32_t repairBusy {};
 uint32_t cycleSeq {};
 mach_vm_address_t powerWellEnableAux {},powerWellDisableAux {};
 mach_vm_address_t powerWellEnableDDI {},powerWellDisableDDI {};
 mach_vm_address_t powerWellEnableEngine {};
};
// Live upstream state needed while the hooks run. Member addresses stay valid
// for the XE singleton lifetime; adlsLightUp is a snapshot taken after the
// ADLS panel block inside processNativePanel.
struct Host {
 bool *active {};
 void **observationController {};
 mach_vm_address_t *orgRegisterRead {};
 const XELP::VBT::Ports *ports {};
 mach_vm_address_t adlsLightUp {};
};
bool install(State &state, KernelPatcher &patcher, size_t index,
 mach_vm_address_t address, size_t size, const Host &host);
}
#endif
