package unit

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class GPR extends Module {
    val io = IO(new Bundle{
        val wen     =  Input(Bool())
        val raddr1  =  Input(UInt(REG_WIDTH.W))
        val raddr2  =  Input(UInt(REG_WIDTH.W))
        val rdata1  = Output(UInt(DATA_WIDTH.W))
        val rdata2  = Output(UInt(DATA_WIDTH.W))
        val waddr   =  Input(UInt(REG_WIDTH.W))
        val wdata   =  Input(UInt(DATA_WIDTH.W))
    })
    val regs = Mem(REG_NUM, UInt(DATA_WIDTH.W))
    io.rdata1 := Mux((io.raddr1 === 0.U), 0.U, regs.read(io.raddr1))
    io.rdata2 := Mux((io.raddr2 === 0.U), 0.U, regs.read(io.raddr2))
    when (io.wen && io.waddr =/= 0.U) {
        regs.write(io.waddr, io.wdata)
    }    
}