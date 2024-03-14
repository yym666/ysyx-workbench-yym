// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU3__Syms.h"


VL_ATTR_COLD void VNJU3___024root__trace_init_sub__TOP__0(VNJU3___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBit(c+3,"io_en", false,-1);
    tracep->declBus(c+4,"io_opt", false,-1, 2,0);
    tracep->declBus(c+5,"io_a", false,-1, 3,0);
    tracep->declBus(c+6,"io_b", false,-1, 3,0);
    tracep->declBus(c+7,"io_c", false,-1, 3,0);
    tracep->declBit(c+8,"io_zf", false,-1);
    tracep->declBit(c+9,"io_cr", false,-1);
    tracep->declBit(c+10,"io_of", false,-1);
    tracep->pushNamePrefix("NJU3 ");
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBit(c+3,"io_en", false,-1);
    tracep->declBus(c+4,"io_opt", false,-1, 2,0);
    tracep->declBus(c+5,"io_a", false,-1, 3,0);
    tracep->declBus(c+6,"io_b", false,-1, 3,0);
    tracep->declBus(c+7,"io_c", false,-1, 3,0);
    tracep->declBit(c+8,"io_zf", false,-1);
    tracep->declBit(c+9,"io_cr", false,-1);
    tracep->declBit(c+10,"io_of", false,-1);
    tracep->declBus(c+11,"casez_tmp", false,-1, 4,0);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void VNJU3___024root__trace_init_top(VNJU3___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_init_top\n"); );
    // Body
    VNJU3___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VNJU3___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU3___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU3___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VNJU3___024root__trace_register(VNJU3___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&VNJU3___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&VNJU3___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&VNJU3___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VNJU3___024root__trace_full_sub_0(VNJU3___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VNJU3___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_full_top_0\n"); );
    // Init
    VNJU3___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU3___024root*>(voidSelf);
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    VNJU3___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VNJU3___024root__trace_full_sub_0(VNJU3___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clock));
    bufp->fullBit(oldp+2,(vlSelf->reset));
    bufp->fullBit(oldp+3,(vlSelf->io_en));
    bufp->fullCData(oldp+4,(vlSelf->io_opt),3);
    bufp->fullCData(oldp+5,(vlSelf->io_a),4);
    bufp->fullCData(oldp+6,(vlSelf->io_b),4);
    bufp->fullCData(oldp+7,(vlSelf->io_c),4);
    bufp->fullBit(oldp+8,(vlSelf->io_zf));
    bufp->fullBit(oldp+9,(vlSelf->io_cr));
    bufp->fullBit(oldp+10,(vlSelf->io_of));
    bufp->fullCData(oldp+11,(vlSelf->NJU3__DOT__casez_tmp),5);
}
