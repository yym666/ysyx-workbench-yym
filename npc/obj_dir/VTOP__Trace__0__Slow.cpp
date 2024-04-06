// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VTOP__Syms.h"


VL_ATTR_COLD void VTOP___024root__trace_init_sub__TOP__0(VTOP___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root__trace_init_sub__TOP__0\n"); );
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+34,"clock", false,-1);
    tracep->declBit(c+35,"reset", false,-1);
    tracep->declBit(c+36,"io_wen", false,-1);
    tracep->declBus(c+37,"io_inst", false,-1, 31,0);
    tracep->declBus(c+38,"io_pc", false,-1, 31,0);
    tracep->declBus(c+39,"io_rs1", false,-1, 4,0);
    tracep->declBus(c+40,"io_rs2", false,-1, 4,0);
    tracep->declBus(c+41,"io_res", false,-1, 31,0);
    tracep->pushNamePrefix("TOP ");
    tracep->declBit(c+34,"clock", false,-1);
    tracep->declBit(c+35,"reset", false,-1);
    tracep->declBit(c+36,"io_wen", false,-1);
    tracep->declBus(c+37,"io_inst", false,-1, 31,0);
    tracep->declBus(c+38,"io_pc", false,-1, 31,0);
    tracep->declBus(c+39,"io_rs1", false,-1, 4,0);
    tracep->declBus(c+40,"io_rs2", false,-1, 4,0);
    tracep->declBus(c+41,"io_res", false,-1, 31,0);
    tracep->pushNamePrefix("EXU ");
    tracep->declBus(c+42,"io_data1", false,-1, 31,0);
    tracep->declBus(c+43,"io_data2", false,-1, 31,0);
    tracep->declBus(c+41,"io_alu_res", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("GPR ");
    tracep->declBit(c+34,"clock", false,-1);
    tracep->declBit(c+36,"io_wen", false,-1);
    tracep->declBus(c+39,"io_raddr1", false,-1, 4,0);
    tracep->declBus(c+40,"io_raddr2", false,-1, 4,0);
    tracep->declBus(c+44,"io_waddr", false,-1, 4,0);
    tracep->declBus(c+41,"io_wdata", false,-1, 31,0);
    tracep->declBus(c+42,"io_rdata1", false,-1, 31,0);
    tracep->pushNamePrefix("regs_ext ");
    tracep->declBus(c+39,"R0_addr", false,-1, 4,0);
    tracep->declBit(c+48,"R0_en", false,-1);
    tracep->declBit(c+34,"R0_clk", false,-1);
    tracep->declBus(c+40,"R1_addr", false,-1, 4,0);
    tracep->declBit(c+48,"R1_en", false,-1);
    tracep->declBit(c+34,"R1_clk", false,-1);
    tracep->declBus(c+44,"W0_addr", false,-1, 4,0);
    tracep->declBit(c+45,"W0_en", false,-1);
    tracep->declBit(c+34,"W0_clk", false,-1);
    tracep->declBus(c+41,"W0_data", false,-1, 31,0);
    tracep->declBus(c+46,"R0_data", false,-1, 31,0);
    tracep->declBus(c+47,"R1_data", false,-1, 31,0);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+1+i*1,"Memory", true,(i+0), 31,0);
    }
    tracep->popNamePrefix(2);
    tracep->pushNamePrefix("IDU ");
    tracep->declBus(c+37,"io_inst", false,-1, 31,0);
    tracep->declBus(c+42,"io_rs1_data", false,-1, 31,0);
    tracep->declBus(c+39,"io_rs1_addr", false,-1, 4,0);
    tracep->declBus(c+40,"io_rs2_addr", false,-1, 4,0);
    tracep->declBus(c+44,"io_rd_addr", false,-1, 4,0);
    tracep->declBus(c+42,"io_op1_data", false,-1, 31,0);
    tracep->declBus(c+43,"io_op2_data", false,-1, 31,0);
    tracep->popNamePrefix(1);
    tracep->pushNamePrefix("IFU ");
    tracep->declBit(c+34,"clock", false,-1);
    tracep->declBit(c+35,"reset", false,-1);
    tracep->declBus(c+38,"io_pc", false,-1, 31,0);
    tracep->declBus(c+33,"pc_reg", false,-1, 31,0);
    tracep->popNamePrefix(2);
}

VL_ATTR_COLD void VTOP___024root__trace_init_top(VTOP___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root__trace_init_top\n"); );
    // Body
    VTOP___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void VTOP___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VTOP___024root__trace_chg_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp);
void VTOP___024root__trace_cleanup(void* voidSelf, VerilatedVcd* /*unused*/);

VL_ATTR_COLD void VTOP___024root__trace_register(VTOP___024root* vlSelf, VerilatedVcd* tracep) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root__trace_register\n"); );
    // Body
    tracep->addFullCb(&VTOP___024root__trace_full_top_0, vlSelf);
    tracep->addChgCb(&VTOP___024root__trace_chg_top_0, vlSelf);
    tracep->addCleanupCb(&VTOP___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void VTOP___024root__trace_full_sub_0(VTOP___024root* vlSelf, VerilatedVcd::Buffer* bufp);

VL_ATTR_COLD void VTOP___024root__trace_full_top_0(void* voidSelf, VerilatedVcd::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root__trace_full_top_0\n"); );
    // Init
    VTOP___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<VTOP___024root*>(voidSelf);
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    VTOP___024root__trace_full_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void VTOP___024root__trace_full_sub_0(VTOP___024root* vlSelf, VerilatedVcd::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root__trace_full_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+1,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[0]),32);
    bufp->fullIData(oldp+2,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[1]),32);
    bufp->fullIData(oldp+3,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[2]),32);
    bufp->fullIData(oldp+4,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[3]),32);
    bufp->fullIData(oldp+5,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[4]),32);
    bufp->fullIData(oldp+6,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[5]),32);
    bufp->fullIData(oldp+7,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[6]),32);
    bufp->fullIData(oldp+8,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[7]),32);
    bufp->fullIData(oldp+9,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[8]),32);
    bufp->fullIData(oldp+10,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[9]),32);
    bufp->fullIData(oldp+11,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[10]),32);
    bufp->fullIData(oldp+12,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[11]),32);
    bufp->fullIData(oldp+13,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[12]),32);
    bufp->fullIData(oldp+14,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[13]),32);
    bufp->fullIData(oldp+15,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[14]),32);
    bufp->fullIData(oldp+16,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[15]),32);
    bufp->fullIData(oldp+17,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[16]),32);
    bufp->fullIData(oldp+18,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[17]),32);
    bufp->fullIData(oldp+19,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[18]),32);
    bufp->fullIData(oldp+20,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[19]),32);
    bufp->fullIData(oldp+21,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[20]),32);
    bufp->fullIData(oldp+22,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[21]),32);
    bufp->fullIData(oldp+23,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[22]),32);
    bufp->fullIData(oldp+24,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[23]),32);
    bufp->fullIData(oldp+25,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[24]),32);
    bufp->fullIData(oldp+26,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[25]),32);
    bufp->fullIData(oldp+27,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[26]),32);
    bufp->fullIData(oldp+28,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[27]),32);
    bufp->fullIData(oldp+29,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[28]),32);
    bufp->fullIData(oldp+30,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[29]),32);
    bufp->fullIData(oldp+31,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[30]),32);
    bufp->fullIData(oldp+32,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory[31]),32);
    bufp->fullIData(oldp+33,(vlSelf->TOP__DOT__IFU__DOT__pc_reg),32);
    bufp->fullBit(oldp+34,(vlSelf->clock));
    bufp->fullBit(oldp+35,(vlSelf->reset));
    bufp->fullBit(oldp+36,(vlSelf->io_wen));
    bufp->fullIData(oldp+37,(vlSelf->io_inst),32);
    bufp->fullIData(oldp+38,(vlSelf->io_pc),32);
    bufp->fullCData(oldp+39,(vlSelf->io_rs1),5);
    bufp->fullCData(oldp+40,(vlSelf->io_rs2),5);
    bufp->fullIData(oldp+41,(vlSelf->io_res),32);
    bufp->fullIData(oldp+42,(((0U == (IData)(vlSelf->io_rs1))
                               ? 0U : vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory
                              [vlSelf->io_rs1])),32);
    bufp->fullIData(oldp+43,((((- (IData)((vlSelf->io_inst 
                                           >> 0x1fU))) 
                               << 0xcU) | (vlSelf->io_inst 
                                           >> 0x14U))),32);
    bufp->fullCData(oldp+44,((0x1fU & (vlSelf->io_inst 
                                       >> 7U))),5);
    bufp->fullBit(oldp+45,(((IData)(vlSelf->io_wen) 
                            & (0U != (0x1fU & (vlSelf->io_inst 
                                               >> 7U))))));
    bufp->fullIData(oldp+46,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory
                             [vlSelf->io_rs1]),32);
    bufp->fullIData(oldp+47,(vlSelf->TOP__DOT__GPR__DOT__regs_ext__DOT__Memory
                             [vlSelf->io_rs2]),32);
    bufp->fullBit(oldp+48,(1U));
}
