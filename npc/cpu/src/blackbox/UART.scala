package blackbox

import chisel3._
import chisel3.util._

import config.MyConfig._
import bus._
 
class UART extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
        val clk =   Input(Clock())
        val rst =   Input(Bool())
        //AR
        val axi = new AXI
  })
  
  addResource("/UART.v")
}
