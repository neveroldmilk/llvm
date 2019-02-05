//===-- RegisterContextPOSIXProcessMonitor_x86.h ----------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_RegisterContextPOSIXProcessMonitor_x86_H_
#define liblldb_RegisterContextPOSIXProcessMonitor_x86_H_

#include "Plugins/Process/Utility/RegisterContextPOSIX_x86.h"
#include "RegisterContextPOSIX.h"
#include <sys/uio.h>

class RegisterContextPOSIXProcessMonitor_x86_64
    : public RegisterContextPOSIX_x86,
      public POSIXBreakpointProtocol {
public:
  RegisterContextPOSIXProcessMonitor_x86_64(
      lldb_private::Thread &thread, uint32_t concrete_frame_idx,
      lldb_private::RegisterInfoInterface *register_info);

protected:
  bool ReadGPR();

  bool ReadFPR();

  bool WriteGPR();

  bool WriteFPR();

  // lldb_private::RegisterContext
  bool ReadRegister(const unsigned reg, lldb_private::RegisterValue &value);

  bool WriteRegister(const unsigned reg,
                     const lldb_private::RegisterValue &value);

  bool ReadRegister(const lldb_private::RegisterInfo *reg_info,
                    lldb_private::RegisterValue &value);

  bool WriteRegister(const lldb_private::RegisterInfo *reg_info,
                     const lldb_private::RegisterValue &value);

  bool ReadAllRegisterValues(lldb::DataBufferSP &data_sp);

  bool WriteAllRegisterValues(const lldb::DataBufferSP &data_sp);

  uint32_t SetHardwareWatchpoint(lldb::addr_t addr, size_t size, bool read,
                                 bool write);

  bool ClearHardwareWatchpoint(uint32_t hw_index);

  bool HardwareSingleStep(bool enable);

  // POSIXBreakpointProtocol
  bool UpdateAfterBreakpoint();

  unsigned GetRegisterIndexFromOffset(unsigned offset);

  bool IsWatchpointHit(uint32_t hw_index);

  bool ClearWatchpointHits();

  lldb::addr_t GetWatchpointAddress(uint32_t hw_index);

  bool IsWatchpointVacant(uint32_t hw_index);

  bool SetHardwareWatchpointWithIndex(lldb::addr_t addr, size_t size, bool read,
                                      bool write, uint32_t hw_index);

  uint32_t NumSupportedHardwareWatchpoints();

private:
  ProcessMonitor &GetMonitor();
  uint32_t
      m_fctrl_offset_in_userarea; // Offset of 'fctrl' in 'UserArea' Structure
  struct iovec m_iovec;
};

#endif
