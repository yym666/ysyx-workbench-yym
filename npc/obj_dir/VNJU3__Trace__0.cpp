// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU3__Syms.h"


void VNJU3___024root__trace_chg_sub_0(VNJU3___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void VNJU3___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_chg_top_0\n"); );
    // Init
    VNJU3___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU3___024root*>(voidSelf);
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    VNJU3___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void VNJU3___024root__trace_chg_sub_0(VNJU3___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->clock));
    bufp->chgBit(oldp+1,(vlSelf->reset));
    bufp->chgBit(oldp+2,(vlSelf->io_en));
    bufp->chgCData(oldp+3,(vlSelf->io_opt),3);
    bufp->chgCData(oldp+4,(vlSelf->io_a),4);
    bufp->chgCData(oldp+5,(vlSelf->io_b),4);
    bufp->chgCData(oldp+6,(vlSelf->io_c),4);
    bufp->chgBit(oldp+7,(vlSelf->io_zf));
    bufp->chgBit(oldp+8,(vlSelf->io_cr));
    bufp->chgBit(oldp+9,(vlSelf->io_of));
    bufp->chgCData(oldp+10,(vlSelf->NJU3__DOT__casez_tmp),5);
}

void VNJU3___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_cleanup\n"); );
    // Init
    VNJU3___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU3___024root*>(voidSelf);
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
