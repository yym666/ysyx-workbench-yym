import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._
import blackbox._

import _root_.stage.IFU
import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.LSU
import _root_.stage.WBU

class TOP extends Module {
    val io = IO(new Bundle {
        val inst    = Output(UInt(DATA_WIDTH.W))
        val inst_req= Output(Bool())
        val pc      = Output(UInt(ADDR_WIDTH.W))
        val rs1     = Output(UInt(REG_WIDTH.W))
        val rs2     = Output(UInt(REG_WIDTH.W))
        val data1   = Output(UInt(DATA_WIDTH.W))
        val data2   = Output(UInt(DATA_WIDTH.W))
        val rd      = Output(UInt(REG_WIDTH.W))

        val br_taken    = Output(Bool())
        val br_target   = Output(UInt(ADDR_WIDTH.W))
        val id_inst     = Output(UInt(DATA_WIDTH.W))
        val ls_rdata    = Output(UInt(DATA_WIDTH.W))
        val wb_wdata    = Output(UInt(DATA_WIDTH.W))
        val mem_valid   = Output(Bool())

        val ldu_mem_opt = Output(UInt(INS_LEN.W))
        val idu_csr_rdt = Output(UInt(DATA_WIDTH.W))
        val mstatus      = Output(UInt(DATA_WIDTH.W))
        // val mtvec     = Output(UInt(DATA_WIDTH.W))
    })

    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val LSU = Module(new LSU())
    val WBU = Module(new WBU())
    val GPR = Module(new GPR())
    val CSR = Module(new CSR())

    val TRP = Module(new TRP())
    val MEM = Module(new MEM())
    val IME = Module(new IME())

    //IFU get inst from TOP
    // IFU.io.inst     <> IME.io.inst
    IME.io.inst        <> IDU.io.inst
    IFU.io.ime_valid   <> IME.io.valid
    IFU.io.out.bits.pc <> IME.io.pc

    io.inst := IME.io.inst
    io.pc   := IFU.io.out.bits.pc
     
    IFU.io.out <> IDU.io.in
    IDU.io.out <> EXU.io.in
    EXU.io.out <> LSU.io.in
    LSU.io.out <> WBU.io.in

    //TRP
    TRP.io.clock := clock
    TRP.io.reset := reset
    TRP.io.halt := IDU.io.halt

    //mem
    MEM.io.clock := clock
    MEM.io.reset := reset
    MEM.io.valid := LSU.io.isST || LSU.io.isLD
    MEM.io.wen   := LSU.io.isST && MEM.io.valid

    MEM.io.waddr  <> LSU.io.addr
    MEM.io.raddr  <> LSU.io.addr
    MEM.io.wdata  <> LSU.io.wdata
    MEM.io.wmask  <> LSU.io.wmask
    LSU.io.rdata  <> MEM.io.rdata

    // io.inst_code        <> LSU.io.inst_code

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

    // io.halt     <> IDU.io.halt

    //DEBUG
    // io.mepc     <> CSR.io.get_mepc
    // io.mtvec    <> CSR.io.get_mtvec
    // io.rd       <> IDU.io.out.bits.rd_addr
    // io.res      <> WBU.io.alu_res

    io.rs1      <> IDU.io.rs1_addr
    io.rs2      <> IDU.io.rs2_addr
    io.data1    <> IDU.io.out.bits.data1
    io.data2    <> IDU.io.out.bits.data2
    io.br_taken     <> WBU.io.br_taken 
    io.br_target    <> WBU.io.br_target
    io.id_inst      <> IDU.io.out.bits.inst
    io.ls_rdata     <> MEM.io.rdata
    io.wb_wdata     <> WBU.io.reg_wdata
    io.mem_valid    := LSU.io.isST || LSU.io.isLD
    io.inst_req     := IFU.io.inst_req
    io.ldu_mem_opt  := IDU.io.out.bits.inst_code
    io.mstatus      := CSR.io.get_mstatus
    io.idu_csr_rdt  := CSR.io.rdata
    io.rd := IDU.io.out.bits.rd_addr
}