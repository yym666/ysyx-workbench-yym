// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU1.h for the primary calling header

#include "verilated.h"

#include "VNJU1___024root.h"

VL_INLINE_OPT void VNJU1___024root___ico_sequent__TOP__0(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->io_y = ((0U == (IData)(vlSelf->io_s)) ? (IData)(vlSelf->io_x0)
                     : ((1U == (IData)(vlSelf->io_s))
                         ? (IData)(vlSelf->io_x1) : 
                        ((2U == (IData)(vlSelf->io_s))
                          ? (IData)(vlSelf->io_x2) : (IData)(vlSelf->io_x3))));
}

void VNJU1___024root___eval_ico(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___eval_ico\n"); );
    // Body
    if (vlSelf->__VicoTriggered.at(0U)) {
        VNJU1___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void VNJU1___024root___eval_act(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___eval_act\n"); );
}

void VNJU1___024root___eval_nba(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___eval_nba\n"); );
}

void VNJU1___024root___eval_triggers__ico(VNJU1___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU1___024root___dump_triggers__ico(VNJU1___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU1___024root___eval_triggers__act(VNJU1___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU1___024root___dump_triggers__act(VNJU1___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU1___024root___dump_triggers__nba(VNJU1___024root* vlSelf);
#endif  // VL_DEBUG

void VNJU1___024root___eval(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___eval\n"); );
    // Init
    CData/*0:0*/ __VicoContinue;
    VlTriggerVec<0> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    vlSelf->__VicoIterCount = 0U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        __VicoContinue = 0U;
        VNJU1___024root___eval_triggers__ico(vlSelf);
        if (vlSelf->__VicoTriggered.any()) {
            __VicoContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VicoIterCount))) {
#ifdef VL_DEBUG
                VNJU1___024root___dump_triggers__ico(vlSelf);
#endif
                VL_FATAL_MT("NJU1.v", 2, "", "Input combinational region did not converge.");
            }
            vlSelf->__VicoIterCount = ((IData)(1U) 
                                       + vlSelf->__VicoIterCount);
            VNJU1___024root___eval_ico(vlSelf);
        }
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            VNJU1___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    VNJU1___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("NJU1.v", 2, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                VNJU1___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                VNJU1___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("NJU1.v", 2, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            VNJU1___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void VNJU1___024root___eval_debug_assertions(VNJU1___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU1___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_x0 & 0xfcU))) {
        Verilated::overWidthError("io_x0");}
    if (VL_UNLIKELY((vlSelf->io_x1 & 0xfcU))) {
        Verilated::overWidthError("io_x1");}
    if (VL_UNLIKELY((vlSelf->io_x2 & 0xfcU))) {
        Verilated::overWidthError("io_x2");}
    if (VL_UNLIKELY((vlSelf->io_x3 & 0xfcU))) {
        Verilated::overWidthError("io_x3");}
    if (VL_UNLIKELY((vlSelf->io_s & 0xfcU))) {
        Verilated::overWidthError("io_s");}
}
#endif  // VL_DEBUG
