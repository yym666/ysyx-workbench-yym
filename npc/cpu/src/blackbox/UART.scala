package blackbox

import chisel3._
import chisel3.util._

import config.MyConfig._
import bus._
 
class UART extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
        val clock = Input(Clock())
        val reset = Input(Bool())
        val axi   = new AXI
  })
  
  addResource("/UART.v")
}
