// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU6.h for the primary calling header

#include "verilated.h"

#include "VNJU6___024root.h"

void VNJU6___024root___eval_act(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___eval_act\n"); );
}

extern const VlUnpacked<CData/*7:0*/, 512> VNJU6__ConstPool__TABLE_hf79700e3_0;

VL_INLINE_OPT void VNJU6___024root___nba_sequent__TOP__0(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___nba_sequent__TOP__0\n"); );
    // Init
    SData/*8:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    // Body
    __Vtableidx1 = (((IData)(vlSelf->NJU6__DOT__reg_0) 
                     << 1U) | (IData)(vlSelf->reset));
    vlSelf->NJU6__DOT__reg_0 = VNJU6__ConstPool__TABLE_hf79700e3_0
        [__Vtableidx1];
    vlSelf->io_output = vlSelf->NJU6__DOT__reg_0;
}

void VNJU6___024root___eval_nba(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        VNJU6___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void VNJU6___024root___eval_triggers__act(VNJU6___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU6___024root___dump_triggers__act(VNJU6___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU6___024root___dump_triggers__nba(VNJU6___024root* vlSelf);
#endif  // VL_DEBUG

void VNJU6___024root___eval(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___eval\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            VNJU6___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    VNJU6___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("NJU6.v", 53, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                VNJU6___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                VNJU6___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("NJU6.v", 53, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            VNJU6___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void VNJU6___024root___eval_debug_assertions(VNJU6___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU6___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
}
#endif  // VL_DEBUG
