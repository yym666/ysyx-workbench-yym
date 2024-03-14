// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VNJU1.h"
#include "VNJU1__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

VNJU1::VNJU1(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VNJU1__Syms(contextp(), _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_x0{vlSymsp->TOP.io_x0}
    , io_x1{vlSymsp->TOP.io_x1}
    , io_x2{vlSymsp->TOP.io_x2}
    , io_x3{vlSymsp->TOP.io_x3}
    , io_s{vlSymsp->TOP.io_s}
    , io_y{vlSymsp->TOP.io_y}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

VNJU1::VNJU1(const char* _vcname__)
    : VNJU1(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VNJU1::~VNJU1() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VNJU1___024root___eval_debug_assertions(VNJU1___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU1___024root___eval_static(VNJU1___024root* vlSelf);
void VNJU1___024root___eval_initial(VNJU1___024root* vlSelf);
void VNJU1___024root___eval_settle(VNJU1___024root* vlSelf);
void VNJU1___024root___eval(VNJU1___024root* vlSelf);

void VNJU1::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VNJU1::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VNJU1___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VNJU1___024root___eval_static(&(vlSymsp->TOP));
        VNJU1___024root___eval_initial(&(vlSymsp->TOP));
        VNJU1___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VNJU1___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VNJU1::eventsPending() { return false; }

uint64_t VNJU1::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VNJU1::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VNJU1___024root___eval_final(VNJU1___024root* vlSelf);

VL_ATTR_COLD void VNJU1::final() {
    VNJU1___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VNJU1::hierName() const { return vlSymsp->name(); }
const char* VNJU1::modelName() const { return "VNJU1"; }
unsigned VNJU1::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> VNJU1::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void VNJU1___024root__trace_init_top(VNJU1___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VNJU1___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU1___024root*>(voidSelf);
    VNJU1__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    VNJU1___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void VNJU1___024root__trace_register(VNJU1___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void VNJU1::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'VNJU1::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    VNJU1___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
