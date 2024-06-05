package blackbox

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
 
class IME extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val valid   =  Input(Bool())
    
    val pc      =  Input(UInt(ADDR_WIDTH.W))
    val inst    = Output(UInt(DATA_WIDTH.W))
  })
  
  addResource("/IME.v")
}
