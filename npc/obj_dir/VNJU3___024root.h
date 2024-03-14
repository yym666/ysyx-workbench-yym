// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VNJU3.h for the primary calling header

#ifndef VERILATED_VNJU3___024ROOT_H_
#define VERILATED_VNJU3___024ROOT_H_  // guard

#include "verilated.h"

class VNJU3__Syms;

class VNJU3___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_en,0,0);
    VL_IN8(io_opt,2,0);
    VL_IN8(io_a,3,0);
    VL_IN8(io_b,3,0);
    VL_OUT8(io_c,3,0);
    VL_OUT8(io_zf,0,0);
    VL_OUT8(io_cr,0,0);
    VL_OUT8(io_of,0,0);
    CData/*4:0*/ NJU3__DOT__casez_tmp;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    VNJU3__Syms* const vlSymsp;

    // CONSTRUCTORS
    VNJU3___024root(VNJU3__Syms* symsp, const char* v__name);
    ~VNJU3___024root();
    VL_UNCOPYABLE(VNJU3___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
