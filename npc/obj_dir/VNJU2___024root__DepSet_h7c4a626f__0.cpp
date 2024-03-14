// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU2.h for the primary calling header

#include "verilated.h"

#include "VNJU2___024root.h"

VL_INLINE_OPT void VNJU2___024root___ico_sequent__TOP__0(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->io_seg = ((IData)(vlSelf->io_en) ? ((IData)(vlSelf->io_in_0)
                                                 ? 0x40U
                                                 : 0x79U)
                       : 0U);
    vlSelf->io_out = ((1U & ((~ (IData)(vlSelf->io_en)) 
                             | (IData)(vlSelf->io_in_0)))
                       ? 0U : ((IData)(vlSelf->io_in_1)
                                ? 1U : ((IData)(vlSelf->io_in_2)
                                         ? 2U : ((IData)(vlSelf->io_in_3)
                                                  ? 3U
                                                  : 
                                                 ((IData)(vlSelf->io_in_4)
                                                   ? 4U
                                                   : 
                                                  ((IData)(vlSelf->io_in_5)
                                                    ? 5U
                                                    : 
                                                   (6U 
                                                    | (1U 
                                                       & (~ (IData)(vlSelf->io_in_6))))))))));
}

void VNJU2___024root___eval_ico(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___eval_ico\n"); );
    // Body
    if (vlSelf->__VicoTriggered.at(0U)) {
        VNJU2___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void VNJU2___024root___eval_act(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___eval_act\n"); );
}

void VNJU2___024root___eval_nba(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___eval_nba\n"); );
}

void VNJU2___024root___eval_triggers__ico(VNJU2___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU2___024root___dump_triggers__ico(VNJU2___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU2___024root___eval_triggers__act(VNJU2___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU2___024root___dump_triggers__act(VNJU2___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU2___024root___dump_triggers__nba(VNJU2___024root* vlSelf);
#endif  // VL_DEBUG

void VNJU2___024root___eval(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___eval\n"); );
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
        VNJU2___024root___eval_triggers__ico(vlSelf);
        if (vlSelf->__VicoTriggered.any()) {
            __VicoContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VicoIterCount))) {
#ifdef VL_DEBUG
                VNJU2___024root___dump_triggers__ico(vlSelf);
#endif
                VL_FATAL_MT("NJU2.v", 2, "", "Input combinational region did not converge.");
            }
            vlSelf->__VicoIterCount = ((IData)(1U) 
                                       + vlSelf->__VicoIterCount);
            VNJU2___024root___eval_ico(vlSelf);
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
            VNJU2___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    VNJU2___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("NJU2.v", 2, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                VNJU2___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                VNJU2___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("NJU2.v", 2, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            VNJU2___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void VNJU2___024root___eval_debug_assertions(VNJU2___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU2___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_en & 0xfeU))) {
        Verilated::overWidthError("io_en");}
    if (VL_UNLIKELY((vlSelf->io_in_0 & 0xfeU))) {
        Verilated::overWidthError("io_in_0");}
    if (VL_UNLIKELY((vlSelf->io_in_1 & 0xfeU))) {
        Verilated::overWidthError("io_in_1");}
    if (VL_UNLIKELY((vlSelf->io_in_2 & 0xfeU))) {
        Verilated::overWidthError("io_in_2");}
    if (VL_UNLIKELY((vlSelf->io_in_3 & 0xfeU))) {
        Verilated::overWidthError("io_in_3");}
    if (VL_UNLIKELY((vlSelf->io_in_4 & 0xfeU))) {
        Verilated::overWidthError("io_in_4");}
    if (VL_UNLIKELY((vlSelf->io_in_5 & 0xfeU))) {
        Verilated::overWidthError("io_in_5");}
    if (VL_UNLIKELY((vlSelf->io_in_6 & 0xfeU))) {
        Verilated::overWidthError("io_in_6");}
    if (VL_UNLIKELY((vlSelf->io_in_7 & 0xfeU))) {
        Verilated::overWidthError("io_in_7");}
}
#endif  // VL_DEBUG
