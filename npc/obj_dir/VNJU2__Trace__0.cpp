// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU2__Syms.h"


void VNJU2___024root__trace_chg_sub_0(VNJU2___024root* vlSelf, VerilatedVcd::Buffer* bufp);

void VNJU2___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_chg_top_0\n"); );
    // Init
    VNJU2___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU2___024root*>(voidSelf);
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    VNJU2___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void VNJU2___024root__trace_chg_sub_0(VNJU2___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    bufp->chgBit(oldp+0,(vlSelf->clock));
    bufp->chgBit(oldp+1,(vlSelf->reset));
    bufp->chgBit(oldp+2,(vlSelf->io_en));
    bufp->chgBit(oldp+3,(vlSelf->io_in_0));
    bufp->chgBit(oldp+4,(vlSelf->io_in_1));
    bufp->chgBit(oldp+5,(vlSelf->io_in_2));
    bufp->chgBit(oldp+6,(vlSelf->io_in_3));
    bufp->chgBit(oldp+7,(vlSelf->io_in_4));
    bufp->chgBit(oldp+8,(vlSelf->io_in_5));
    bufp->chgBit(oldp+9,(vlSelf->io_in_6));
    bufp->chgBit(oldp+10,(vlSelf->io_in_7));
    bufp->chgCData(oldp+11,(vlSelf->io_out),3);
    bufp->chgCData(oldp+12,(vlSelf->io_seg),7);
}

void VNJU2___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_cleanup\n"); );
    // Init
    VNJU2___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU2___024root*>(voidSelf);
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VlUnpacked<CData/*0:0*/, 1> __Vm_traceActivity;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        __Vm_traceActivity[__Vi0] = 0;
    }
    // Body
    vlSymsp->__Vm_activity = false;
    __Vm_traceActivity[0U] = 0U;
}
