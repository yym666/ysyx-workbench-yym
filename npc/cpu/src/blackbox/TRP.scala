package blackbox

import chisel3._
import chisel3.util._
 
class TRP extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
    val clock = Input(Clock())
    val reset = Input(Bool())
    val halt  = Input(Bool())
  })
  
  addResource("/TRP.v")
}
