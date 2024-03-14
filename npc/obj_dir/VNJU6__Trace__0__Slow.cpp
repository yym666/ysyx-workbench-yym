// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU6__Syms.h"


VL_ATTR_COLD void VNJU6___024root__trace_init_sub__TOP__0(VNJU6___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBus(c+3,"io_output", false,-1, 7,0);
    tracep->pushNamePrefix("NJU6 ");
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBus(c+3,"io_output", false,-1, 7,0);
    tracep->declBus(c+4,"reg_0", false,-1, 7,0);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void VNJU6___024root__trace_init_top(VNJU6___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root__trace_init_top\n"); );
    // Body
    VNJU6___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VNJU6___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU6___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU6___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VNJU6___024root__trace_register(VNJU6___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&VNJU6___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&VNJU6___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&VNJU6___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VNJU6___024root__trace_full_sub_0(VNJU6___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VNJU6___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root__trace_full_top_0\n"); );
    // Init
    VNJU6___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU6___024root*>(voidSelf);
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    VNJU6___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VNJU6___024root__trace_full_sub_0(VNJU6___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clock));
    bufp->fullBit(oldp+2,(vlSelf->reset));
    bufp->fullCData(oldp+3,(vlSelf->io_output),8);
    bufp->fullCData(oldp+4,(vlSelf->NJU6__DOT__reg_0),8);
}
