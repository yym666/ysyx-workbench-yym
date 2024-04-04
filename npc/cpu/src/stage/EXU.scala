package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class EXU extends Module {
    val io = IO(new Bundle{
        val data1   =  Input(UInt(DATA_WIDTH.W))
        val data2   =  Input(UInt(DATA_WIDTH.W))
        val alu_res = Output(UInt(DATA_WIDTH.W))
        val excode  =  Input(UInt(LEN_EXC.W))
    })
    io.alu_res := io.data1 + io.data2
}