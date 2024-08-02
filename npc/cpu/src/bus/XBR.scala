package bus

import chisel3._
import chisel3.util._

class XBR extends Module{
    val io = IO(new Bundle {
        val arb   = new AXI
        val clint = Flipped(new AXI)
        val mastr = Flipped(new AXI)
    })

    val is_clt = Mux( (((io.arb.araddr >= "h0200_0000".U(32.W) && io.arb.araddr <= "h0200_0fff".U(32.W))) || 
                       ((io.arb.awaddr >= "h0200_0000".U(32.W) && io.arb.awaddr <= "h0200_0fff".U(32.W)))), true.B, false.B)
    // val idle :: to_clt :: to_mst :: Nil = Enum(2)
    // val XBRstate = RegInit(idle)
    // val NXTstate = WireDefault(idle)
    // NXTstate := MuxLookup(XBRstate, idle, List(
    //     idle   -> Mux(is_clt, to_clt, to_mst),
    //     to_clt -> Mux(is_clt, to_clt, to_mst),
    //     to_mst -> Mux(is_clt, to_clt, to_mst)
    // ))
    // XBRstate := NXTstate
    when (is_clt === true.B){
        io.clint <> io.arb
        Empty_mst()
    }.otherwise{
        io.mastr <> io.arb
        Empty_clt()
    }

    def Empty_mst(): Unit = {
        io.mastr.arvalid := false.B
        io.mastr.araddr := 0.U
        io.mastr.arid := 0.U
        io.mastr.arlen := 0.U
        io.mastr.arsize := 0.U
        io.mastr.arburst := 0.U
        io.mastr.rready := false.B
        io.mastr.awvalid := false.B
        io.mastr.awaddr := 0.U
        io.mastr.awid := 0.U
        io.mastr.awlen := 0.U
        io.mastr.awsize := 0.U
        io.mastr.awburst := 0.U
        io.mastr.wvalid := false.B
        io.mastr.wdata := 0.U
        io.mastr.wstrb := 0.U
        io.mastr.wlast := false.B
        io.mastr.bready := false.B
    }
    def Empty_clt(): Unit = {
        io.clint.arvalid := false.B
        io.clint.araddr := 0.U
        io.clint.arid := 0.U
        io.clint.arlen := 0.U
        io.clint.arsize := 0.U
        io.clint.arburst := 0.U
        io.clint.rready := false.B
        io.clint.awvalid := false.B
        io.clint.awaddr := 0.U
        io.clint.awid := 0.U
        io.clint.awlen := 0.U
        io.clint.awsize := 0.U
        io.clint.awburst := 0.U
        io.clint.wvalid := false.B
        io.clint.wdata := 0.U
        io.clint.wstrb := 0.U
        io.clint.wlast := false.B
        io.clint.bready := false.B
    }
}