// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU6.h for the primary calling header

#include "verilated.h"

#include "VNJU6__Syms.h"
#include "VNJU6___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU6___024root___dump_triggers__stl(VNJU6___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void VNJU6___024root___eval_triggers__stl(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.at(0U) = (0U == vlSelf->__VstlIterCount);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        VNJU6___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
