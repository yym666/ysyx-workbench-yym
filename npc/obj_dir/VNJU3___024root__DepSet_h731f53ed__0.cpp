// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU3.h for the primary calling header

#include "verilated.h"

#include "VNJU3___024root.h"

VL_INLINE_OPT void VNJU3___024root___ico_sequent__TOP__0(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___ico_sequent__TOP__0\n"); );
    // Init
    CData/*3:0*/ NJU3__DOT___res_T_1;
    NJU3__DOT___res_T_1 = 0;
    CData/*3:0*/ NJU3__DOT___res_T_9;
    NJU3__DOT___res_T_9 = 0;
    CData/*3:0*/ NJU3__DOT___res_T_15;
    NJU3__DOT___res_T_15 = 0;
    CData/*3:0*/ NJU3__DOT___res_T_13;
    NJU3__DOT___res_T_13 = 0;
    CData/*3:0*/ NJU3__DOT___res_T_17;
    NJU3__DOT___res_T_17 = 0;
    // Body
    NJU3__DOT___res_T_1 = (0xfU & ((IData)(vlSelf->io_a) 
                                   + (IData)(vlSelf->io_b)));
    NJU3__DOT___res_T_9 = (0xfU & ((IData)(1U) + ((~ (IData)(vlSelf->io_b)) 
                                                  + (IData)(vlSelf->io_a))));
    NJU3__DOT___res_T_15 = ((IData)(vlSelf->io_a) & (IData)(vlSelf->io_b));
    NJU3__DOT___res_T_13 = ((IData)(vlSelf->io_a) | (IData)(vlSelf->io_b));
    NJU3__DOT___res_T_17 = ((IData)(vlSelf->io_a) ^ (IData)(vlSelf->io_b));
    vlSelf->NJU3__DOT__casez_tmp = ((4U & (IData)(vlSelf->io_opt))
                                     ? ((2U & (IData)(vlSelf->io_opt))
                                         ? ((6U == (IData)(vlSelf->io_opt))
                                             ? VL_LTS_III(4, (IData)(vlSelf->io_a), (IData)(vlSelf->io_b))
                                             : ((IData)(vlSelf->io_a) 
                                                == (IData)(vlSelf->io_b)))
                                         : ((1U & (IData)(vlSelf->io_opt))
                                             ? ((0x10U 
                                                 & ((IData)(NJU3__DOT___res_T_17) 
                                                    << 1U)) 
                                                | (IData)(NJU3__DOT___res_T_17))
                                             : ((0x10U 
                                                 & ((IData)(NJU3__DOT___res_T_13) 
                                                    << 1U)) 
                                                | (IData)(NJU3__DOT___res_T_13))))
                                     : ((2U & (IData)(vlSelf->io_opt))
                                         ? ((1U & (IData)(vlSelf->io_opt))
                                             ? ((0x10U 
                                                 & ((IData)(NJU3__DOT___res_T_15) 
                                                    << 1U)) 
                                                | (IData)(NJU3__DOT___res_T_15))
                                             : ((0x10U 
                                                 & ((~ 
                                                     ((IData)(vlSelf->io_a) 
                                                      >> 3U)) 
                                                    << 4U)) 
                                                | (0xfU 
                                                   & (~ (IData)(vlSelf->io_a)))))
                                         : ((1U & (IData)(vlSelf->io_opt))
                                             ? ((0x10U 
                                                 & ((IData)(NJU3__DOT___res_T_9) 
                                                    << 1U)) 
                                                | (IData)(NJU3__DOT___res_T_9))
                                             : ((0x10U 
                                                 & ((IData)(NJU3__DOT___res_T_1) 
                                                    << 1U)) 
                                                | (IData)(NJU3__DOT___res_T_1)))));
    vlSelf->io_cr = (1U & ((IData)(vlSelf->NJU3__DOT__casez_tmp) 
                           >> 4U));
    vlSelf->io_of = (((1U & ((IData)(vlSelf->io_a) 
                             >> 3U)) == (1U & ((IData)(vlSelf->io_b) 
                                               >> 3U))) 
                     & ((1U & ((IData)(vlSelf->NJU3__DOT__casez_tmp) 
                               >> 3U)) != (1U & ((IData)(vlSelf->io_a) 
                                                 >> 3U))));
    vlSelf->io_c = (0xfU & (IData)(vlSelf->NJU3__DOT__casez_tmp));
    vlSelf->io_zf = (0U == (IData)(vlSelf->io_c));
}

void VNJU3___024root___eval_ico(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___eval_ico\n"); );
    // Body
    if (vlSelf->__VicoTriggered.at(0U)) {
        VNJU3___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void VNJU3___024root___eval_act(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___eval_act\n"); );
}

void VNJU3___024root___eval_nba(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___eval_nba\n"); );
}

void VNJU3___024root___eval_triggers__ico(VNJU3___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU3___024root___dump_triggers__ico(VNJU3___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU3___024root___eval_triggers__act(VNJU3___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU3___024root___dump_triggers__act(VNJU3___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void VNJU3___024root___dump_triggers__nba(VNJU3___024root* vlSelf);
#endif  // VL_DEBUG

void VNJU3___024root___eval(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___eval\n"); );
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
        VNJU3___024root___eval_triggers__ico(vlSelf);
        if (vlSelf->__VicoTriggered.any()) {
            __VicoContinue = 1U;
            if (VL_UNLIKELY((0x64U < vlSelf->__VicoIterCount))) {
#ifdef VL_DEBUG
                VNJU3___024root___dump_triggers__ico(vlSelf);
#endif
                VL_FATAL_MT("NJU3.v", 2, "", "Input combinational region did not converge.");
            }
            vlSelf->__VicoIterCount = ((IData)(1U) 
                                       + vlSelf->__VicoIterCount);
            VNJU3___024root___eval_ico(vlSelf);
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
            VNJU3___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    VNJU3___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("NJU3.v", 2, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                VNJU3___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                VNJU3___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("NJU3.v", 2, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            VNJU3___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void VNJU3___024root___eval_debug_assertions(VNJU3___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VNJU3___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
    if (VL_UNLIKELY((vlSelf->io_en & 0xfeU))) {
        Verilated::overWidthError("io_en");}
    if (VL_UNLIKELY((vlSelf->io_opt & 0xf8U))) {
        Verilated::overWidthError("io_opt");}
    if (VL_UNLIKELY((vlSelf->io_a & 0xf0U))) {
        Verilated::overWidthError("io_a");}
    if (VL_UNLIKELY((vlSelf->io_b & 0xf0U))) {
        Verilated::overWidthError("io_b");}
}
#endif  // VL_DEBUG
