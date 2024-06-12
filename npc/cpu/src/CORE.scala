import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._
import bus._
import blackbox._

import _root_.stage.IFU
import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.LSU
import _root_.stage.WBU

class CORE extends Module {
    val io = IO(new Bundle {
        val imem    = Flipped(new AXI)
        val dmem    = Flipped(new AXI)
        val inst_req= Output(Bool())
        val inst    = Output(UInt(DATA_WIDTH.W))
        val pc      = Output(UInt(ADDR_WIDTH.W))
    })

    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val LSU = Module(new LSU())
    val WBU = Module(new WBU())
    val GPR = Module(new GPR())
    val CSR = Module(new CSR())
    val TRP = Module(new TRP())
    // val ARB = Module(new ARB())
    // val IDM = Module(new IDM())
    // val XBR = Module(new XBR())
    // val UART  = Module(new UART())
    // val CLINT = Module(new CLINT())

    // IDM.io.clock    := clock
    // IDM.io.reset    := reset
    // UART.io.clock   := clock
    // UART.io.reset   := reset
    // CLINT.io.clock  := clock
    // CLINT.io.reset  := reset

    // XBR.io.arb  <> ARB.io.mem
    // ARB.io.imem <> IFU.io.imem
    // ARB.io.dmem <> LSU.io.dmem
    // XBR.io.sram <> IDM.io.axi
    // XBR.io.uart <> UART.io.axi
    // XBR.io.clint<> CLINT.io.axi

    IFU.io.imem :<>= io.imem
    LSU.io.dmem :<>= io.dmem
    IFU.io.out <> IDU.io.in
    IDU.io.out <> EXU.io.in
    EXU.io.out <> LSU.io.in
    LSU.io.out <> WBU.io.in

    //TRP
    TRP.io.clock := clock
    TRP.io.reset := reset
    TRP.io.halt := IDU.io.halt

    IFU.io.idu_done  <> IDU.io.idu_done
    IFU.io.exu_done  <> EXU.io.exu_done
    IFU.io.lsu_done  <> LSU.io.lsu_done
    //EXU <> IFU
    IFU.io.br_taken  <> WBU.io.br_taken
    IFU.io.br_target <> WBU.io.br_target

    //WBU <> CSR & GPR
    CSR.io.wen      <> WBU.io.csr_we
    CSR.io.waddr    <> WBU.io.csr_waddr
    CSR.io.wdata    <> WBU.io.csr_wdata

    GPR.io.wen      <> WBU.io.reg_we
    GPR.io.waddr    <> WBU.io.reg_waddr
    GPR.io.wdata    <> WBU.io.reg_wdata

    //IDU <> CSR & GRP
    IDU.io.rs1_data <> GPR.io.rdata1
    IDU.io.rs2_data <> GPR.io.rdata2
    IDU.io.rs1_addr <> GPR.io.raddr1
    IDU.io.rs2_addr <> GPR.io.raddr2

    IDU.io.csr_raddr <> CSR.io.raddr
    IDU.io.csr_rdata <> CSR.io.rdata
    CSR.io.set_mcause     <> IDU.io.set_mcause
    CSR.io.set_mcause_val <> IDU.io.set_mcause_val
    CSR.io.set_mepc       <> IDU.io.set_mepc
    CSR.io.set_mepc_val   <> IDU.io.set_mepc_val
    CSR.io.get_mepc       <> IDU.io.get_mepc
    CSR.io.get_mtvec      <> IDU.io.get_mtvec

    //DEBUG
    io.inst_req     := IFU.io.inst_req
    io.inst     := IFU.io.out.bits.inst
    io.pc       := IFU.io.out.bits.pc
}