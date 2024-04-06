import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.IFU
import _root_.blackbox.TRP
import javax.xml.transform.OutputKeys

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
        
        //data sram interface
        val data_sram_addr  = Output(UInt(ADDR_WIDTH.W))
        val data_sram_wdata = Output(UInt(DATA_WIDTH.W))
        val data_sram_rdata =  Input(UInt(DATA_WIDTH.W))
    })
    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val GPR = Module(new GPR())

    
    GPR.io.wen := MuxCase(
        false.B,
        Seq(
            (IDU.io.reg_op === REG_WT) -> (true.B),
            (IDU.io.reg_op === REG_RD) -> (false.B)
        )
    )
    
    IDU.io.rs1_data <> GPR.io.rdata1
    IDU.io.rs2_data <> GPR.io.rdata2
    GPR.io.raddr1 <> IDU.io.rs1_addr
    GPR.io.raddr2 <> IDU.io.rs2_addr

    EXU.io.data1  <> IDU.io.op1_data
    EXU.io.data2  <> IDU.io.op2_data
    EXU.io.excode <> IDU.io.excode
    EXU.io.mem_op <> IDU.io.mem_op
    IFU.io.pc     <> IDU.io.pc

    GPR.io.waddr <> IDU.io.rd_addr
    GPR.io.wdata := MuxCase(
        EXU.io.alu_res,
        Seq(
            (io.Load === true.B) -> (io.data_sram_rdata)
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
    io.data_sram_addr <> EXU.io.waddr
         

    io.Store  <> IDU.io.Store
    io.Load   <> IDU.io.Load
    io.SL_len <> IDU.io.SL_len
}