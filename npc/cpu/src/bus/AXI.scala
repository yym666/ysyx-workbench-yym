package bus

import chisel3._
import chisel3.util._

class AXI extends Bundle{
    //AR
    val arready = Output(Bool())
    val arvalid =  Input(Bool())
    val araddr  =  Input(UInt(32.W))
    val arid    =  Input(UInt(4.W)) //
    val arlen   =  Input(UInt(8.W)) //
    val arsize  =  Input(UInt(3.W)) //
    val arburst =  Input(UInt(2.W)) //
    //R
    val rready  =  Input(Bool())
    val rvalid  = Output(Bool())
    val rresp   = Output(UInt(2.W))
    val rdata   = Output(UInt(64.W))
    val rlast   = Output(Bool())   //
    val rid     = Output(UInt(4.W))//
    //AW
    val awready = Output(Bool())
    val awvalid =  Input(Bool())
    val awaddr  =  Input(UInt(32.W))
    val awid    =  Input(UInt(4.W)) //
    val awlen   =  Input(UInt(8.W)) //
    val awsize  =  Input(UInt(3.W)) //
    val awburst =  Input(UInt(2.W)) //
    //W
    val wready  = Output(Bool())
    val wvalid  =  Input(Bool())
    val wdata   =  Input(UInt(64.W))
    val wstrb   =  Input(UInt(8.W))
    val wlast   =  Input(Bool())    //
    //B
    val bready  =  Input(Bool())
    val bvalid  = Output(Bool())
    val bresp   = Output(UInt(2.W))
    val bid     = Output(UInt(4.W)) //
}