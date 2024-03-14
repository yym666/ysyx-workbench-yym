// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU2.h for the primary calling header

#include "verilated.h"

#include "VNJU2__Syms.h"
#include "VNJU2___024root.h"

void VNJU2___024root___ctor_var_reset(VNJU2___024root* vlSelf);

VNJU2___024root::VNJU2___024root(VNJU2__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    VNJU2___024root___ctor_var_reset(this);
}

void VNJU2___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

VNJU2___024root::~VNJU2___024root() {
}
