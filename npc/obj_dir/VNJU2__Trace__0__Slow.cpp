// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VNJU2__Syms.h"


VL_ATTR_COLD void VNJU2___024root__trace_init_sub__TOP__0(VNJU2___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBit(c+3,"io_en", false,-1);
    tracep->declBit(c+4,"io_in_0", false,-1);
    tracep->declBit(c+5,"io_in_1", false,-1);
    tracep->declBit(c+6,"io_in_2", false,-1);
    tracep->declBit(c+7,"io_in_3", false,-1);
    tracep->declBit(c+8,"io_in_4", false,-1);
    tracep->declBit(c+9,"io_in_5", false,-1);
    tracep->declBit(c+10,"io_in_6", false,-1);
    tracep->declBit(c+11,"io_in_7", false,-1);
    tracep->declBus(c+12,"io_out", false,-1, 2,0);
    tracep->declBus(c+13,"io_seg", false,-1, 6,0);
    tracep->pushNamePrefix("NJU2 ");
    tracep->declBit(c+1,"clock", false,-1);
    tracep->declBit(c+2,"reset", false,-1);
    tracep->declBit(c+3,"io_en", false,-1);
    tracep->declBit(c+4,"io_in_0", false,-1);
    tracep->declBit(c+5,"io_in_1", false,-1);
    tracep->declBit(c+6,"io_in_2", false,-1);
    tracep->declBit(c+7,"io_in_3", false,-1);
    tracep->declBit(c+8,"io_in_4", false,-1);
    tracep->declBit(c+9,"io_in_5", false,-1);
    tracep->declBit(c+10,"io_in_6", false,-1);
    tracep->declBit(c+11,"io_in_7", false,-1);
    tracep->declBus(c+12,"io_out", false,-1, 2,0);
    tracep->declBus(c+13,"io_seg", false,-1, 6,0);
    tracep->popNamePrefix(1);
}

VL_ATTR_COLD void VNJU2___024root__trace_init_top(VNJU2___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_init_top\n"); );
    // Body
    VNJU2___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VNJU2___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU2___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VNJU2___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VNJU2___024root__trace_register(VNJU2___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&VNJU2___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&VNJU2___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&VNJU2___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VNJU2___024root__trace_full_sub_0(VNJU2___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VNJU2___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_full_top_0\n"); );
    // Init
    VNJU2___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU2___024root*>(voidSelf);
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    VNJU2___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VNJU2___024root__trace_full_sub_0(VNJU2___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelf->clock));
    bufp->fullBit(oldp+2,(vlSelf->reset));
    bufp->fullBit(oldp+3,(vlSelf->io_en));
    bufp->fullBit(oldp+4,(vlSelf->io_in_0));
    bufp->fullBit(oldp+5,(vlSelf->io_in_1));
    bufp->fullBit(oldp+6,(vlSelf->io_in_2));
    bufp->fullBit(oldp+7,(vlSelf->io_in_3));
    bufp->fullBit(oldp+8,(vlSelf->io_in_4));
    bufp->fullBit(oldp+9,(vlSelf->io_in_5));
    bufp->fullBit(oldp+10,(vlSelf->io_in_6));
    bufp->fullBit(oldp+11,(vlSelf->io_in_7));
    bufp->fullCData(oldp+12,(vlSelf->io_out),3);
    bufp->fullCData(oldp+13,(vlSelf->io_seg),7);
}
