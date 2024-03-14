// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VNJU1.h for the primary calling header

#ifndef VERILATED_VNJU1___024ROOT_H_
#define VERILATED_VNJU1___024ROOT_H_  // guard

#include "verilated.h"

class VNJU1__Syms;

class VNJU1___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_x0,1,0);
    VL_IN8(io_x1,1,0);
    VL_IN8(io_x2,1,0);
    VL_IN8(io_x3,1,0);
    VL_IN8(io_s,1,0);
    VL_OUT8(io_y,1,0);
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    VNJU1__Syms* const vlSymsp;

    // CONSTRUCTORS
    VNJU1___024root(VNJU1__Syms* symsp, const char* v__name);
    ~VNJU1___024root();
    VL_UNCOPYABLE(VNJU1___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
