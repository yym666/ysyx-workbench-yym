// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU3.h for the primary calling header

#include "verilated.h"

#include "VNJU3__Syms.h"
#include "VNJU3___024root.h"

void VNJU3___024root___ctor_var_reset(VNJU3___024root* vlSelf);

VNJU3___024root::VNJU3___024root(VNJU3__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    VNJU3___024root___ctor_var_reset(this);
}

void VNJU3___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

VNJU3___024root::~VNJU3___024root() {
}
