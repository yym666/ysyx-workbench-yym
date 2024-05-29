package unit

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class CSR extends Module {
    val io = IO(new Bundle{
        val wen =  Input(Bool())
        val addr  =  Input(UInt(CSR_WIDTH.W))
        val wdata =  Input(UInt(DATA_WIDTH.W))
        val rdata = Output(UInt(DATA_WIDTH.W))
        val get_mepc    = Output(UInt(DATA_WIDTH.W))
        val get_mtvec   = Output(UInt(DATA_WIDTH.W))
        val set_mcause  = Input(Bool())
        val set_mcause_val  =  Input(UInt(DATA_WIDTH.W))
        val set_mepc    = Input(Bool())
        val set_mepc_val    =  Input(UInt(DATA_WIDTH.W))
    })
    val csrs = Mem(CSR_NUM, UInt(DATA_WIDTH.W))
    io.get_mepc := csrs.read(3.U)
    io.get_mtvec := csrs.read(2.U)
    io.rdata     := csrs.read(io.addr)
    when (io.wen && io.addr =/= 0.U && io.addr < 5.U) {
        csrs.write(io.addr, io.wdata)
    }
    when (io.set_mcause === true.B){
        csrs.write(4.U, io.set_mcause_val)
    }
    when (io.set_mepc   === true.B){
        csrs.write(3.U, io.set_mepc_val)
    }
}