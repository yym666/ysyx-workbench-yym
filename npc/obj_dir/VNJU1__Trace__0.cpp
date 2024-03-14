// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU1__Syms.h"


void VNJU1___024root__trace_chg_sub_0(VNJU1___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void VNJU1___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_chg_top_0\n"); );
    // Init
    VNJU1___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU1___024root*>(voidSelf);
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    VNJU1___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void VNJU1___024root__trace_chg_sub_0(VNJU1___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->clock));
    bufp->chgBit(oldp+1,(vlSelf->reset));
    bufp->chgCData(oldp+2,(vlSelf->io_x0),2);
    bufp->chgCData(oldp+3,(vlSelf->io_x1),2);
    bufp->chgCData(oldp+4,(vlSelf->io_x2),2);
    bufp->chgCData(oldp+5,(vlSelf->io_x3),2);
    bufp->chgCData(oldp+6,(vlSelf->io_s),2);
    bufp->chgCData(oldp+7,(vlSelf->io_y),2);
    bufp->chgCData(oldp+8,(((0U == (IData)(vlSelf->io_s))
                             ? (IData)(vlSelf->io_x0)
                             : ((1U == (IData)(vlSelf->io_s))
                                 ? (IData)(vlSelf->io_x1)
                                 : ((2U == (IData)(vlSelf->io_s))
                                     ? (IData)(vlSelf->io_x2)
                                     : (IData)(vlSelf->io_x3))))),2);
}

void VNJU1___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_cleanup\n"); );
    // Init
    VNJU1___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU1___024root*>(voidSelf);
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
