// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VNJU6.h for the primary calling header

#ifndef VERILATED_VNJU6___024ROOT_H_
#define VERILATED_VNJU6___024ROOT_H_  // guard

#include "verilated.h"

class VNJU6__Syms;

class VNJU6___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_OUT8(io_output,7,0);
    CData/*7:0*/ NJU6__DOT__reg_0;
    CData/*0:0*/ __Vtrigrprev__TOP__clock;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ __VstlIterCount;
    IData/*31:0*/ __VactIterCount;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    VNJU6__Syms* const vlSymsp;

    // CONSTRUCTORS
    VNJU6___024root(VNJU6__Syms* symsp, const char* v__name);
    ~VNJU6___024root();
    VL_UNCOPYABLE(VNJU6___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);


#endif  // guard
