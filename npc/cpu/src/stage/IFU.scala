package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class IFU extends Module {
    val io = IO(new Bundle {
        val pc   = Output(UInt(ADDR_WIDTH.W))
        val br_taken    =  Input(Bool())
        val br_target   =  Input(UInt(ADDR_WIDTH.W))
    })
    val pc_reg = RegInit(UInt(ADDR_WIDTH.W), START_ADDR.U)
    val pc_nxt = Mux((io.br_taken === true.B), io.br_target, pc_reg + 4.U)
    pc_reg := pc_nxt
    io.pc := pc_reg
}