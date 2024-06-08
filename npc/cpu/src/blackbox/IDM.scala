package blackbox

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
 
class IDM extends BlackBox with HasBlackBoxResource {
  val io = IO(new Bundle {
        val clk =   Input(Clock())
        val rst =   Input(Bool())
        //AR
        val araddr  = Input(UInt(32.W))
        val arvalid = Input(Bool())
        val arready = Output(Bool())
        //R
        val rdata  = Output(UInt(32.W))
        val rresp  = Output(UInt(2.W))
        val rvalid = Output(Bool())
        val rready = Input(Bool())
        //AW
        val awaddr  = Input(UInt(32.W))
        val awvalid = Input(Bool())
        val awready = Output(Bool())
        //W
        val wdata  = Input(UInt(32.W))
        val wstrb  = Input(UInt((4).W))
        val wvalid = Input(Bool())
        val wready = Output(Bool())
        //B
        val bresp = Output(UInt(2.W))
        val bvalid = Output(Bool())
        val bready = Input(Bool())
  })
  
  addResource("/IDM.v")
}
