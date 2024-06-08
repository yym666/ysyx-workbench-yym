package bus

import chisel3._
import chisel3.util._

class AXI extends Bundle{
    //AR
    // val arid    = Input(UInt(4.W))
    val araddr  = Input(UInt(32.W))
    // val arlen   = Input(UInt(4.W))
    // val arsize  = Input(UInt(3.W))
    // val arburst = Input(UInt(2.W))
    val arvalid = Input(Bool())
    val arready = Output(Bool())
    //R
    // val rid     = Output(UInt(4.W))
    val rdata   = Output(UInt(32.W))
    val rresp   = Output(UInt(2.W))
    // val rlast   = Output(Bool())
    val rvalid  = Output(Bool())
    val rready  = Input(Bool())
    //AW
    // val awid    = Input(UInt(4.W))
    val awaddr  = Input(UInt(32.W))
    // val awlen   = Input(UInt(4.W))
    // val awsize  = Input(UInt(3.W))
    // val awburst = Input(UInt(2.W))
    val awvalid = Input(Bool())
    val awready = Output(Bool())
    //W
    val wdata   = Input(UInt(32.W))
    val wstrb   = Input(UInt(4.W))
    // val wlast   = Input(Bool())
    val wvalid  = Input(Bool())
    val wready  = Output(Bool())
    //B
    // val bid     = Output(UInt(4.W))
    val bresp   = Output(UInt(2.W))
    val bvalid  = Output(Bool())
    val bready  = Input(Bool())
}