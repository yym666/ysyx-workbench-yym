// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VNJU3.h"
#include "VNJU3__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

VNJU3::VNJU3(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VNJU3__Syms(contextp(), _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_en{vlSymsp->TOP.io_en}
    , io_opt{vlSymsp->TOP.io_opt}
    , io_a{vlSymsp->TOP.io_a}
    , io_b{vlSymsp->TOP.io_b}
    , io_c{vlSymsp->TOP.io_c}
    , io_zf{vlSymsp->TOP.io_zf}
    , io_cr{vlSymsp->TOP.io_cr}
    , io_of{vlSymsp->TOP.io_of}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

VNJU3::VNJU3(const char* _vcname__)
    : VNJU3(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VNJU3::~VNJU3() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VNJU3___024root___eval_debug_assertions(VNJU3___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU3___024root___eval_static(VNJU3___024root* vlSelf);
void VNJU3___024root___eval_initial(VNJU3___024root* vlSelf);
void VNJU3___024root___eval_settle(VNJU3___024root* vlSelf);
void VNJU3___024root___eval(VNJU3___024root* vlSelf);

void VNJU3::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VNJU3::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VNJU3___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VNJU3___024root___eval_static(&(vlSymsp->TOP));
        VNJU3___024root___eval_initial(&(vlSymsp->TOP));
        VNJU3___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VNJU3___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VNJU3::eventsPending() { return false; }

uint64_t VNJU3::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VNJU3::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VNJU3___024root___eval_final(VNJU3___024root* vlSelf);

VL_ATTR_COLD void VNJU3::final() {
    VNJU3___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VNJU3::hierName() const { return vlSymsp->name(); }
const char* VNJU3::modelName() const { return "VNJU3"; }
unsigned VNJU3::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> VNJU3::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void VNJU3___024root__trace_init_top(VNJU3___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VNJU3___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU3___024root*>(voidSelf);
    VNJU3__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    VNJU3___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void VNJU3___024root__trace_register(VNJU3___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void VNJU3::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'VNJU3::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    VNJU3___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
