// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU1__Syms.h"


VL_ATTR_COLD void VNJU1___024root__trace_init_sub__TOP__0(VNJU1___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBus(c+3,"io_x0", false,-1, 1,0);
    tracep->declBus(c+4,"io_x1", false,-1, 1,0);
    tracep->declBus(c+5,"io_x2", false,-1, 1,0);
    tracep->declBus(c+6,"io_x3", false,-1, 1,0);
    tracep->declBus(c+7,"io_s", false,-1, 1,0);
    tracep->declBus(c+8,"io_y", false,-1, 1,0);
    tracep->pushNamePrefix("NJU1 ");
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBus(c+3,"io_x0", false,-1, 1,0);
    tracep->declBus(c+4,"io_x1", false,-1, 1,0);
    tracep->declBus(c+5,"io_x2", false,-1, 1,0);
    tracep->declBus(c+6,"io_x3", false,-1, 1,0);
    tracep->declBus(c+7,"io_s", false,-1, 1,0);
    tracep->declBus(c+8,"io_y", false,-1, 1,0);
    tracep->declBus(c+9,"casez_tmp", false,-1, 1,0);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void VNJU1___024root__trace_init_top(VNJU1___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_init_top\n"); );
    // Body
    VNJU1___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VNJU1___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU1___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU1___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VNJU1___024root__trace_register(VNJU1___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&VNJU1___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&VNJU1___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&VNJU1___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VNJU1___024root__trace_full_sub_0(VNJU1___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VNJU1___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_full_top_0\n"); );
    // Init
    VNJU1___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU1___024root*>(voidSelf);
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    VNJU1___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VNJU1___024root__trace_full_sub_0(VNJU1___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clock));
    bufp->fullBit(oldp+2,(vlSelf->reset));
    bufp->fullCData(oldp+3,(vlSelf->io_x0),2);
    bufp->fullCData(oldp+4,(vlSelf->io_x1),2);
    bufp->fullCData(oldp+5,(vlSelf->io_x2),2);
    bufp->fullCData(oldp+6,(vlSelf->io_x3),2);
    bufp->fullCData(oldp+7,(vlSelf->io_s),2);
    bufp->fullCData(oldp+8,(vlSelf->io_y),2);
    bufp->fullCData(oldp+9,(((0U == (IData)(vlSelf->io_s))
                              ? (IData)(vlSelf->io_x0)
                              : ((1U == (IData)(vlSelf->io_s))
                                  ? (IData)(vlSelf->io_x1)
                                  : ((2U == (IData)(vlSelf->io_s))
                                      ? (IData)(vlSelf->io_x2)
                                      : (IData)(vlSelf->io_x3))))),2);
}
