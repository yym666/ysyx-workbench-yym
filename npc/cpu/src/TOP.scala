import chisel3._
import chisel3.util._

import config.MyConfig._
import unit._

import _root_.stage.IDU
import _root_.stage.EXU
import _root_.stage.IFU

class TOP extends Module {
    val io = IO(new Bundle {
        //inst sram interface
        val wen     =  Input(Bool())
        val inst    =  Input(UInt(ADDR_WIDTH.W))
        val pc      = Output(UInt(DATA_WIDTH.W))
        val rs1     = Output(UInt(REG_WIDTH.W))
        val rs2     = Output(UInt(REG_WIDTH.W))
        val res     = Output(UInt(DATA_WIDTH.W))
        //val inst_sram_wdata = Output(UInt(DATA_WIDTH.W))
        //data sram interface
        //val data_sram_we    = Output(UInt(1.W))
        //val data_sram_addr  = Output(UInt(ADDR_WIDTH.W))
        //val data_sram_wdata = Output(UInt(DATA_WIDTH.W))
        //val data_sram_rdata =  Input(UInt(DATA_WIDTH.W))
        //halt
        //val halt            = Output(Bool())
    })
    val IFU = Module(new IFU())
    val IDU = Module(new IDU())
    val EXU = Module(new EXU())
    val GPR = Module(new GPR())
    
    GPR.io.wen <> io.wen

    //io.inst_sram_addr  <> IFU.io.inst_addr
    IDU.io.inst <> io.inst
    
    IDU.io.rs1_data <> GPR.io.rdata1
    IDU.io.rs2_data <> GPR.io.rdata2
    GPR.io.raddr1 <> IDU.io.rs1_addr
    GPR.io.raddr2 <> IDU.io.rs2_addr

    EXU.io.data1  <> IDU.io.op1_data
    EXU.io.data2  <> IDU.io.op2_data
    EXU.io.excode <> IDU.io.excode

    GPR.io.waddr <> IDU.io.rd_addr
    GPR.io.wdata <> EXU.io.alu_res

    io.pc  <> IFU.io.pc
    io.rs1 <> IDU.io.rs1_addr
    io.rs2 <> IDU.io.rs2_addr
    io.res <> EXU.io.alu_res
}