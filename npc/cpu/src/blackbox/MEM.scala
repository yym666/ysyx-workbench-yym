package blackbox

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
 
class MEM extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val clock =  Input(Clock())
    val reset =  Input(Bool())
    val wen   =  Input(Bool())
    val valid =  Input(Bool())
    val raddr =  Input(UInt(ADDR_WIDTH.W))
    val waddr =  Input(UInt(ADDR_WIDTH.W))
    val wdata =  Input(UInt(DATA_WIDTH.W))
    val wmask =  Input(UInt(8.W))

    val rdata = Output(UInt(DATA_WIDTH.W))
  })
  
  addResource("/MEM.v")
}
