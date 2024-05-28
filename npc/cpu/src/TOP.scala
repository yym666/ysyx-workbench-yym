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
    })
    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val GPR = Module(new GPR())
    val CSR = Module(new CSR())
    
    IDU.io.rs1_data <> GPR.io.rdata1
    IDU.io.rs2_data <> GPR.io.rdata2
    GPR.io.raddr1 <> IDU.io.rs1_addr
    GPR.io.raddr2 <> IDU.io.rs2_addr

    EXU.io.data1  <> IDU.io.op1_data
    EXU.io.data2  <> IDU.io.op2_data
    EXU.io.excode <> IDU.io.excode
    EXU.io.mem_op <> IDU.io.mem_op
    EXU.io.csr_data <> CSR.io.rdata
    IFU.io.pc       <> IDU.io.pc
    IFU.io.br_taken  <> EXU.io.br_taken
    IFU.io.br_target := MuxCase(
        IDU.io.br_target,
        Seq(
            (io.inst_code === isJALR) -> (EXU.io.alu_res),
            (io.inst_code === isJAL ) -> (EXU.io.alu_res)
        )
    )

    CSR.io.addr <> IDU.io.csr_addr
    CSR.io.wen := MuxCase(
        false.B,
        Seq(
            (IDU.io.csr_op === CSR_WT) -> (true.B)
        )
    )
    CSR.io.wdata <> EXU.io.alu_res

    CSR.io.set_mcause <> IDU.io.set_mcause
    CSR.io.set_mcause_val <> IDU.io.set_mcause_val
    CSR.io.set_mepc <> IDU.io.set_mepc
    CSR.io.set_mepc_val <> IDU.io.set_mepc_val

    GPR.io.wen := MuxCase(
        false.B,
        Seq(
            (IDU.io.reg_op === REG_WT) -> (true.B),
            (IDU.io.reg_op === REG_RD) -> (false.B)
        )
    )
    GPR.io.waddr <> IDU.io.rd_addr
    GPR.io.wdata := MuxCase(
        EXU.io.alu_res,
        Seq(
            (io.Load === true.B) -> (io.data_sram_rdata),
            (io.inst_code === isJALR) -> (io.pc + 4.U),
            (io.inst_code === isJAL) -> (io.pc + 4.U)
        )
    )

    io.inst <> IDU.io.inst
    io.halt <> IDU.io.halt
    io.rd  <> IDU.io.rd_addr
    io.pc  <> IFU.io.pc
    io.rs1 <> IDU.io.rs1_addr
    io.rs2 <> IDU.io.rs2_addr
    io.res <> EXU.io.alu_res
    io.data1 <> IDU.io.op1_data
    io.data2 <> IDU.io.op2_data
    io.data_sram_wdata <> IDU.io.mem_data
    io.data_sram_addr  <> EXU.io.waddr
         

    io.Store  <> IDU.io.Store
    io.Load   <> IDU.io.Load
    io.SL_len <> IDU.io.SL_len
    io.inst_code <> IDU.io.inst_code
    io.br_taken  <> EXU.io.br_taken
    io.br_target := MuxCase(
        IDU.io.br_target,
        Seq(
            (io.inst_code === isJALR)   -> (EXU.io.alu_res),
            (io.inst_code === isJAL )   -> (EXU.io.alu_res),
            (io.inst_code === isECALL ) -> (CSR.io.get_mtvec)
        )
    )
}