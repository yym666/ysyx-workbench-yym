// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VNJU2.h for the primary calling header

#ifndef VERILATED_VNJU2___024ROOT_H_
#define VERILATED_VNJU2___024ROOT_H_  // guard

#include "verilated.h"

class VNJU2__Syms;

class VNJU2___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN8(io_en,0,0);
    VL_IN8(io_in_0,0,0);
    VL_IN8(io_in_1,0,0);
    VL_IN8(io_in_2,0,0);
    VL_IN8(io_in_3,0,0);
    VL_IN8(io_in_4,0,0);
    VL_IN8(io_in_5,0,0);
    VL_IN8(io_in_6,0,0);
    VL_IN8(io_in_7,0,0);
    VL_OUT8(io_out,2,0);
    VL_OUT8(io_seg,6,0);
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VicoIterCount;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VicoTriggered;
    VlTriggerVec<0> __VactTriggered;
    VlTriggerVec<0> __VnbaTriggered;

    // INTERNAL VARIABLES
    VNJU2__Syms* const vlSymsp;

    // CONSTRUCTORS
    VNJU2___024root(VNJU2__Syms* symsp, const char* v__name);
    ~VNJU2___024root();
    VL_UNCOPYABLE(VNJU2___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
