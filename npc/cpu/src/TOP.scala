import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

import _root_.stage.IFU
import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.LSU
import _root_.stage.WBU

class TOP extends Module {
    val io = IO(new Bundle {
        //inst sram interface
        val inst    =  Input(UInt(DATA_WIDTH.W))
        val inst_req= Output(Bool())
        val pc      = Output(UInt(ADDR_WIDTH.W))
        // val rs1     = Output(UInt(REG_WIDTH.W))
        // val rs2     = Output(UInt(REG_WIDTH.W))
        // val data1   = Output(UInt(DATA_WIDTH.W))
        // val data2   = Output(UInt(DATA_WIDTH.W))
        // val rd      = Output(UInt(REG_WIDTH.W))
        // val res     = Output(UInt(DATA_WIDTH.W))
        val halt    = Output(UInt(1.W))

        val Store   = Output(Bool())
        val Load    = Output(Bool())
        val SL_len  = Output(UInt(5.W))
        val inst_code   = Output(UInt(INS_LEN.W))

        // val br_taken    = Output(Bool())
        // val br_target   = Output(UInt(ADDR_WIDTH.W))
        
        //data sram interface
        val data_sram_en    = Output(Bool())
        val data_sram_addr  = Output(UInt(ADDR_WIDTH.W))
        val data_sram_wdata = Output(UInt(DATA_WIDTH.W))
        val data_sram_rdata =  Input(UInt(DATA_WIDTH.W))
        // val mepc     = Output(UInt(DATA_WIDTH.W))
        // val mtvec     = Output(UInt(DATA_WIDTH.W))
    })

    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val LSU = Module(new LSU())
    val WBU = Module(new WBU())
    val GPR = Module(new GPR())
    val CSR = Module(new CSR())

    //IFU get inst from TOP
    IFU.io.inst     <> io.inst
    IFU.io.inst_req <> io.inst_req
    
    IFU.io.out <> IDU.io.in
    IDU.io.out <> EXU.io.in
    EXU.io.out <> LSU.io.in
    LSU.io.out <> WBU.io.in


    IFU.io.idu_done  <> IDU.io.idu_done
    IFU.io.exu_done  <> EXU.io.exu_done
    IFU.io.lsu_done  <> LSU.io.lsu_done
    //EXU <> IFU
    IFU.io.br_taken  <> IDU.io.br_taken
    IFU.io.br_target <> IDU.io.br_target

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

    //data_sram
    io.data_sram_wdata  <> LSU.io.wdata
    io.data_sram_addr   <> LSU.io.addr
    io.data_sram_en     <> LSU.io.isST
    io.inst_code        <> LSU.io.inst_code
    LSU.io.rdata        <> io.data_sram_rdata
    io.Store    := LSU.io.isST
    io.Load     := LSU.io.isLD
    io.SL_len   <>  LSU.io.mem_len


    io.inst     <> IDU.io.in.bits.inst
    io.pc       <> IFU.io.out.bits.pc
    io.halt     <> IDU.io.halt
    //DEBUG
    // io.mepc     <> CSR.io.get_mepc
    // io.mtvec    <> CSR.io.get_mtvec
    // io.rd       <> IDU.io.out.bits.rd_addr
    // io.rs1      <> IDU.io.rs1_addr
    // io.rs2      <> IDU.io.rs2_addr
    // io.res      <> WBU.io.alu_res
    // io.data1    <> IDU.io.out.bits.data1
    // io.data2    <> IDU.io.out.bits.data2
}