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
    val ICH = Module(new ICH())

    val diffpc = dontTouch(Wire(UInt(32.W)))
    diffpc := WBU.io.pc_nxt

    // IFU.io.imem :<>= io.imem
    IFU.io.addr     <> ICH.io.addr
    IFU.io.addr_vl  <> ICH.io.addr_vl
    IFU.io.inst     <> ICH.io.inst
    IFU.io.inst_rd  <> ICH.io.inst_rd

    IDU.io.in.bits := RegEnable(IFU.io.out.bits, IFU.io.out.valid & IDU.io.in.ready)
    EXU.io.in.bits := RegEnable(IDU.io.out.bits, IDU.io.out.valid & EXU.io.in.ready)
    LSU.io.in.bits := RegEnable(EXU.io.out.bits, EXU.io.out.valid & LSU.io.in.ready)
    WBU.io.in.bits := RegEnable(LSU.io.out.bits, LSU.io.out.valid & WBU.io.in.ready)
    
    IFU.io.out.ready := IDU.io.in.ready
    IDU.io.out.ready := EXU.io.in.ready
    EXU.io.out.ready := LSU.io.in.ready
    LSU.io.out.ready := WBU.io.in.ready

    EXU.io.isST <> LSU.io.isST
    EXU.io.isLD <> LSU.io.isLD

//IDU valid
    // when (IDU.io.idu_br_taken){
    //     IDU.io.in.valid := false.B
    // }.else
    when (IDU.io.in.ready && IFU.io.out.valid){
        IDU.io.in.valid := true.B
    }.elsewhen (IDU.io.out.fire){
        IDU.io.in.valid := false.B
    }.otherwise{
        IDU.io.in.valid := false.B
    }
//EXU valid
    // when (WBU.io.br_taken){
    //     EXU.io.in.valid := false.B
    // }.else
        when (EXU.io.in.ready && IDU.io.out.valid){
        EXU.io.in.valid := true.B
    }.elsewhen (EXU.io.out.fire){
        EXU.io.in.valid := false.B
    }.otherwise{
        EXU.io.in.valid := false.B
    }
//LSU valid
    // when (WBU.io.br_taken){
    //     LSU.io.in.valid := false.B
    // }.else
        when (LSU.io.in.ready && EXU.io.out.valid){
        LSU.io.in.valid := true.B
    }.elsewhen (LSU.io.out.fire){
        LSU.io.in.valid := false.B
    }.otherwise{
        LSU.io.in.valid := false.B
    }
//WBU valid
    when (WBU.io.in.ready && LSU.io.out.valid){
        WBU.io.in.valid := true.B
    }.otherwise{
        WBU.io.in.valid := false.B
    }

    EXU.io.rd_from_ex <> IDU.io.rd_from_ex
    LSU.io.rd_from_ls <> IDU.io.rd_from_ls
    WBU.io.rd_from_wb <> IDU.io.rd_from_wb

    ICH.io.imem :<>= io.imem
    LSU.io.dmem :<>= io.dmem

    //TRP
    TRP.io.clock := clock
    TRP.io.reset := reset
    TRP.io.halt := IDU.io.halt

    //EXU <> IFU
    IFU.io.br_taken <> IDU.io.idu_br_taken
    IFU.io.br_target<> IDU.io.idu_br_target

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