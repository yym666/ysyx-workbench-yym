// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VNJU6.h"
#include "VNJU6__Syms.h"
#include "verilated_vcd_c.h"

//============================================================
// Constructors

VNJU6::VNJU6(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VNJU6__Syms(contextp(), _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_output{vlSymsp->TOP.io_output}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

VNJU6::VNJU6(const char* _vcname__)
    : VNJU6(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VNJU6::~VNJU6() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VNJU6___024root___eval_debug_assertions(VNJU6___024root* vlSelf);
#endif  // VL_DEBUG
void VNJU6___024root___eval_static(VNJU6___024root* vlSelf);
void VNJU6___024root___eval_initial(VNJU6___024root* vlSelf);
void VNJU6___024root___eval_settle(VNJU6___024root* vlSelf);
void VNJU6___024root___eval(VNJU6___024root* vlSelf);

void VNJU6::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VNJU6::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VNJU6___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_activity = true;
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VNJU6___024root___eval_static(&(vlSymsp->TOP));
        VNJU6___024root___eval_initial(&(vlSymsp->TOP));
        VNJU6___024root___eval_settle(&(vlSymsp->TOP));
    }
    // MTask 0 start
    VL_DEBUG_IF(VL_DBG_MSGF("MTask0 starting\n"););
    Verilated::mtaskId(0);
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VNJU6___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfThreadMTask(vlSymsp->__Vm_evalMsgQp);
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VNJU6::eventsPending() { return false; }

uint64_t VNJU6::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VNJU6::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VNJU6___024root___eval_final(VNJU6___024root* vlSelf);

VL_ATTR_COLD void VNJU6::final() {
    VNJU6___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VNJU6::hierName() const { return vlSymsp->name(); }
const char* VNJU6::modelName() const { return "VNJU6"; }
unsigned VNJU6::threads() const { return 1; }
std::unique_ptr<VerilatedTraceConfig> VNJU6::traceConfig() const {
    return std::unique_ptr<VerilatedTraceConfig>{new VerilatedTraceConfig{false, false, false}};
};

//============================================================
// Trace configuration

void VNJU6___024root__trace_init_top(VNJU6___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD static void trace_init(void* voidSelf, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VNJU6___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VNJU6___024root*>(voidSelf);
    VNJU6__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (!vlSymsp->_vm_contextp__->calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
            "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->scopeEscape(' ');
    tracep->pushNamePrefix(std::string{vlSymsp->name()} + ' ');
    VNJU6___024root__trace_init_top(vlSelf, tracep);
    tracep->popNamePrefix();
    tracep->scopeEscape('.');
}

VL_ATTR_COLD void VNJU6___024root__trace_register(VNJU6___024root* vlSelf, VerilatedVcd* tracep);

VL_ATTR_COLD void VNJU6::trace(VerilatedVcdC* tfp, int levels, int options) {
    if (tfp->isOpen()) {
        vl_fatal(__FILE__, __LINE__, __FILE__,"'VNJU6::trace()' shall not be called after 'VerilatedVcdC::open()'.");
    }
    if (false && levels && options) {}  // Prevent unused
    tfp->spTrace()->addModel(this);
    tfp->spTrace()->addInitCb(&trace_init, &(vlSymsp->TOP));
    VNJU6___024root__trace_register(&(vlSymsp->TOP), tfp->spTrace());
}
