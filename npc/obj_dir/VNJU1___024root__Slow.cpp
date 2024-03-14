// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU1.h for the primary calling header

#include "verilated.h"

#include "VNJU1__Syms.h"
#include "VNJU1___024root.h"

void VNJU1___024root___ctor_var_reset(VNJU1___024root* vlSelf);

VNJU1___024root::VNJU1___024root(VNJU1__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    VNJU1___024root___ctor_var_reset(this);
}

void VNJU1___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

VNJU1___024root::~VNJU1___024root() {
}
