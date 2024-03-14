// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VNJU6.h for the primary calling header

#include "verilated.h"

#include "VNJU6__Syms.h"
#include "VNJU6___024root.h"

void VNJU6___024root___ctor_var_reset(VNJU6___024root* vlSelf);

VNJU6___024root::VNJU6___024root(VNJU6__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    VNJU6___024root___ctor_var_reset(this);
}

void VNJU6___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

VNJU6___024root::~VNJU6___024root() {
}
