// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VNJU2.h"
#include "VNJU2__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

VNJU2::VNJU2(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VNJU2__Syms(contextp(), _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_en{vlSymsp->TOP.io_en}
    , io_in_0{vlSymsp->TOP.io_in_0}
    , io_in_1{vlSymsp->TOP.io_in_1}
    , io_in_2{vlSymsp->TOP.io_in_2}
    , io_in_3{vlSymsp->TOP.io_in_3}
    , io_in_4{vlSymsp->TOP.io_in_4}
    , io_in_5{vlSymsp->TOP.io_in_5}
    , io_in_6{vlSymsp->TOP.io_in_6}
    , io_in_7{vlSymsp->TOP.io_in_7}
    , io_out{vlSymsp->TOP.io_out}
    , io_seg{vlSymsp->TOP.io_seg}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

VNJU2::VNJU2(const char* _vcname__)
    : VNJU2(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VNJU2::~VNJU2() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VNJU2___024root___eval_debug_assertions(VNJU2___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU2___024root___eval_static(VNJU2___024root* vlSelf);
void VNJU2___024root___eval_initial(VNJU2___024root* vlSelf);
void VNJU2___024root___eval_settle(VNJU2___024root* vlSelf);
void VNJU2___024root___eval(VNJU2___024root* vlSelf);

void VNJU2::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VNJU2::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VNJU2___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VNJU2___024root___eval_static(&(vlSymsp->TOP));
        VNJU2___024root___eval_initial(&(vlSymsp->TOP));
        VNJU2___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VNJU2___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VNJU2::eventsPending() { return false; }

uint64_t VNJU2::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VNJU2::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VNJU2___024root___eval_final(VNJU2___024root* vlSelf);

VL_ATTR_COLD void VNJU2::final() {
    VNJU2___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VNJU2::hierName() const { return vlSymsp->name(); }
const char* VNJU2::modelName() const { return "VNJU2"; }
unsigned VNJU2::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> VNJU2::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void VNJU2___024root__trace_init_top(VNJU2___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VNJU2___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU2___024root*>(voidSelf);
    VNJU2__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    VNJU2___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void VNJU2___024root__trace_register(VNJU2___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void VNJU2::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'VNJU2::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    VNJU2___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
