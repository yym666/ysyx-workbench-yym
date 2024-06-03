import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.IFU

class TOP extends Module {
    val io = IO(new Bundle {
        //inst sram interface
        val inst    =  Input(UInt(DATA_WIDTH.W))
        val inst_req= Output(Bool())
        val pc      = Output(UInt(ADDR_WIDTH.W))
        val rs1     = Output(UInt(REG_WIDTH.W))
        val rs2     = Output(UInt(REG_WIDTH.W))
        val data1   = Output(UInt(DATA_WIDTH.W))
        val data2   = Output(UInt(DATA_WIDTH.W))
        val rd      = Output(UInt(REG_WIDTH.W))
        val res     = Output(UInt(DATA_WIDTH.W))
        val halt    = Output(UInt(1.W))

        val Store   = Output(Bool())
        val Load    = Output(Bool())
        val SL_len  = Output(UInt(5.W))
        val inst_code   = Output(UInt(INS_LEN.W))

        val br_taken    = Output(Bool())
        val br_target   = Output(UInt(ADDR_WIDTH.W))
        
        //data sram interface
        val data_sram_addr  = Output(UInt(ADDR_WIDTH.W))
        val data_sram_wdata = Output(UInt(DATA_WIDTH.W))
        val data_sram_rdata =  Input(UInt(DATA_WIDTH.W))
        val mepc     = Output(UInt(DATA_WIDTH.W))
        val mtvec     = Output(UInt(DATA_WIDTH.W))
        val out1     = Output(UInt(DATA_WIDTH.W))
        val out2     = Output(UInt(DATA_WIDTH.W))
    })

    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val GPR = Module(new GPR())
    val CSR = Module(new CSR())

    //IFU get inst from TOP
    IFU.io.inst     <> io.inst
    IFU.io.inst_req <> io.inst_req
    
    IFU.io.out <> IDU.io.in
    IDU.io.out <> EXU.io.in


    IFU.io.idu_done  <> IDU.io.idu_done
    //EXU <> IFU
    IFU.io.br_taken  <> EXU.io.br_taken
    IFU.io.br_target := MuxCase(
        EXU.io.br_target,
        Seq(
            (EXU.io.inst_code === isJALR)   -> (EXU.io.alu_res),
            (EXU.io.inst_code === isJAL )   -> (EXU.io.alu_res),
            (EXU.io.inst_code === isMRET  ) -> (CSR.io.get_mepc),
            (EXU.io.inst_code === isECALL ) -> (CSR.io.get_mtvec)
        )
    )

    // EXU <> CSR
    CSR.io.wdata <> EXU.io.alu_res

    //IDU, EXU <> GRP
    IDU.io.rs1_data <> GPR.io.rdata1
    IDU.io.rs2_data <> GPR.io.rdata2
    IDU.io.rs1_addr <> GPR.io.raddr1
    IDU.io.rs2_addr <> GPR.io.raddr2

    GPR.io.wen      <> EXU.io.reg_wen
    GPR.io.waddr    <> EXU.io.rd_addr
    GPR.io.wdata := MuxCase( EXU.io.alu_res,
        Seq((EXU.io.mem_opt === MEM_LD)   -> (io.data_sram_rdata),
            (EXU.io.inst_code === isJALR) -> (io.pc + 4.U),
            (EXU.io.inst_code === isJAL)  -> (io.pc + 4.U)))
    
    //IDU <> CSR 
    IDU.io.csr_raddr <> CSR.io.raddr
    IDU.io.csr_rdata <> CSR.io.rdata
    IDU.io.csr_wen   <> CSR.io.wen
    CSR.io.set_mcause     <> IDU.io.set_mcause
    CSR.io.set_mcause_val <> IDU.io.set_mcause_val
    CSR.io.set_mepc       <> IDU.io.set_mepc
    CSR.io.set_mepc_val   <> IDU.io.set_mepc_val

    //DEBUG
    io.mepc     <> CSR.io.get_mepc
    io.mtvec    <> CSR.io.get_mtvec
    io.out1     <> EXU.io.alu_res
    io.out2     <> IDU.io.csr_raddr
    io.inst     <> IDU.io.in.bits.inst
    io.halt     <> IDU.io.halt
    io.rd       <> IDU.io.out.bits.rd_addr
    io.pc       <> IFU.io.out.bits.pc
    io.rs1      <> IDU.io.rs1_addr
    io.rs2      <> IDU.io.rs2_addr
    io.res      <> EXU.io.alu_res
    io.data1    <> IDU.io.out.bits.data1
    io.data2    <> IDU.io.out.bits.data2
    io.data_sram_wdata <> EXU.io.mem_data
    io.data_sram_addr  <> EXU.io.waddr
    io.Store  <> IDU.io.Store
    io.Load   <> IDU.io.Load
    io.SL_len <> IDU.io.SL_len
    io.inst_code <> EXU.io.inst_code
    io.br_taken  <> EXU.io.br_taken
    io.br_target := MuxCase(
        EXU.io.br_target,
        Seq(
            (EXU.io.inst_code === isJALR)   -> (EXU.io.alu_res),
            (EXU.io.inst_code === isJAL )   -> (EXU.io.alu_res),
            (EXU.io.inst_code === isMRET  ) -> (CSR.io.get_mepc),
            (EXU.io.inst_code === isECALL ) -> (CSR.io.get_mtvec)
        )
    )
}