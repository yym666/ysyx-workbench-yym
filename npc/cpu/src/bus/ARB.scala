package bus

import chisel3._
import chisel3.util._
import java.awt.BufferCapabilities.FlipContents

class ARB extends Module{
    val io = IO(new Bundle {
        val imem = new AXI
        val dmem = new AXI
        val  mem = Flipped(new AXI)
    })
    //IMEM_DONTCARE
    // io.imem.rid     := 0.U
    // io.imem.rlast   := true.B
    // io.imem.bid     := 0.U
    //DMEM_DONTCARE
    // io.dmem.rid     := 0.U
    // io.dmem.rlast   := true.B
    // io.dmem.bid     := 0.U
    //MEM_DONTCARE
    // io.mem.arid     := 0.U
    // io.mem.arlen    := 0.U
    // io.mem.arburst  := 0.U
    // io.mem.awid     := 0.U
    // io.mem.awlen    := 0.U
    // io.mem.awburst  := 0.U
    // io.mem.wlast    := false.B

    //imem reg
    val imem_arready= RegInit(io.imem.arready)
    val imem_rdata  = RegInit(io.imem.rdata)
    val imem_rresp  = RegInit(io.imem.rresp)
    val imem_rvalid = RegInit(io.imem.rvalid)
    val imem_awready= RegInit(io.imem.awready)
    val imem_wready = RegInit(io.imem.wready)
    val imem_bresp  = RegInit(io.imem.bresp)
    val imem_bvalid = RegInit(io.imem.bvalid)
    io.imem.arready := imem_arready
    io.imem.rdata   := imem_rdata
    io.imem.rresp   := imem_rresp
    io.imem.rvalid  := imem_rvalid
    io.imem.awready := imem_awready
    io.imem.wready  := imem_wready
    io.imem.bresp   := imem_bresp
    io.imem.bvalid  := imem_bvalid
    //dmem reg
    val dmem_arready= RegInit(io.dmem.arready)
    val dmem_rdata  = RegInit(io.dmem.rdata)
    val dmem_rresp  = RegInit(io.dmem.rresp)
    val dmem_rvalid = RegInit(io.dmem.rvalid)
    val dmem_awready= RegInit(io.dmem.awready)
    val dmem_wready = RegInit(io.dmem.wready)
    val dmem_bresp  = RegInit(io.dmem.bresp)
    val dmem_bvalid = RegInit(io.dmem.bvalid)
    io.dmem.arready := dmem_arready
    io.dmem.rdata   := dmem_rdata
    io.dmem.rresp   := dmem_rresp
    io.dmem.rvalid  := dmem_rvalid
    io.dmem.awready := dmem_awready
    io.dmem.wready  := dmem_wready
    io.dmem.bresp   := dmem_bresp
    io.dmem.bvalid  := dmem_bvalid
    //mem reg
    val mem_araddr  = RegInit(io.mem.araddr)
    val mem_arvalid = RegInit(io.mem.arvalid)
    // val mem_arsize  = RegInit(io.mem.arsize)
    val mem_rready  = RegInit(io.mem.rready)
    val mem_awaddr  = RegInit(io.mem.awaddr)
    val mem_awvalid = RegInit(io.mem.awvalid)
    // val mem_awsize  = RegInit(io.mem.awsize)
    val mem_wdata   = RegInit(io.mem.wdata)
    val mem_wstrb   = RegInit(io.mem.wstrb)
    val mem_wvalid  = RegInit(io.mem.wvalid)
    val mem_bready  = RegInit(io.mem.bready)
    io.mem.araddr   := mem_araddr
    io.mem.arvalid  := mem_arvalid
    // io.mem.arsize   := mem_arsize
    io.mem.rready   := mem_rready
    io.mem.awaddr   := mem_awaddr
    io.mem.awvalid  := mem_awvalid
    // io.mem.awsize   := mem_awsize
    io.mem.wdata    := mem_wdata
    io.mem.wstrb    := mem_wstrb
    io.mem.wvalid   := mem_wvalid
    io.mem.bready   := mem_bready

    val chose :: imem1 :: dmem1 :: imem2 :: dmem2 :: Nil = Enum(5)
    val state = RegInit(chose)
    val nextState = WireDefault(chose)
    nextState := MuxLookup(state, chose, List(
        chose -> MuxCase(chose, Seq(
            ( io.imem.arvalid & imem_arready) -> imem1,
            ((io.dmem.arvalid & dmem_arready) || (io.dmem.awvalid & dmem_awready & io.dmem.wvalid & dmem_wready)).asBool -> dmem1)),
        imem1 -> Mux( io.mem.rready & io.mem.rvalid , imem2, imem1),
        imem2 -> Mux(io.imem.rready & io.imem.rvalid, chose, imem2),
        dmem1 -> Mux(( io.mem.rready & io.mem.rvalid ) | ( io.mem.bready & io.mem.bvalid ), dmem2, dmem1),
        dmem2 -> Mux((io.dmem.rready & io.dmem.rvalid) | (io.dmem.bready & io.dmem.bvalid), chose, dmem2)
    ))
    state := nextState

    switch(nextState){
        is(chose){
            DefaultMem()
            DefaultDmem()
            DefaultImem()
        }
        is(imem1){
            ConnectImem()
            DefaultDmem()
            dmem_arready := false.B
            dmem_wready  := false.B
            dmem_awready := false.B
        }
        is(imem2){
            ConnectImem()
            DefaultDmem()
            //if handshake hapen,it will go into this state,so we should close connection to memory
            mem_arvalid := false.B
            mem_rready  := false.B
            //block
            dmem_arready := false.B
            dmem_wready  := false.B
            dmem_awready := false.B
        }
        is(dmem1){
            ConnectDmem()
            DefaultImem()
            imem_arready := false.B
        }
        is(dmem2){
            ConnectDmem()
            DefaultImem()
            //close connection to memory
            mem_arvalid := false.B
            mem_rready  := false.B
            mem_awvalid := false.B
            mem_wvalid  := false.B
            mem_bready  := false.B
            //block
            imem_arready:= false.B
        }
    }
    
    def ConnectImem(): Unit = {
        imem_arready := io.mem.arready
        imem_rdata   := io.mem.rdata
        imem_rresp   := io.mem.rresp
        imem_rvalid  := io.mem.rvalid
        imem_awready := io.mem.awready
        imem_wready  := io.mem.wready
        imem_bresp   := io.mem.bresp
        imem_bvalid  := io.mem.bvalid

        mem_araddr  := io.imem.araddr
        mem_arvalid := io.imem.arvalid
        // mem_arsize  := io.imem.arsize
        mem_rready  := io.imem.rready
        mem_awaddr  := io.imem.awaddr
        mem_awvalid := io.imem.awvalid
        // mem_awsize  := io.imem.awsize
        mem_wdata   := io.imem.wdata
        mem_wstrb   := io.imem.wstrb
        mem_wvalid  := io.imem.wvalid
        mem_bready  := io.imem.bready
    
    }
    def ConnectDmem(): Unit = {
        dmem_arready:= io.mem.arready
        dmem_rdata  := io.mem.rdata
        dmem_rresp  := io.mem.rresp
        dmem_rvalid := io.mem.rvalid
        dmem_awready:= io.mem.awready
        dmem_wready := io.mem.wready
        dmem_bresp  := io.mem.bresp
        dmem_bvalid := io.mem.bvalid

        mem_araddr  := io.dmem.araddr
        mem_arvalid := io.dmem.arvalid
        // mem_arsize  := io.dmem.arsize
        mem_rready  := io.dmem.rready
        mem_awaddr  := io.dmem.awaddr
        mem_awvalid := io.dmem.awvalid
        // mem_awsize  := io.dmem.awsize
        mem_wdata   := io.dmem.wdata
        mem_wstrb   := io.dmem.wstrb
        mem_wvalid  := io.dmem.wvalid
        mem_bready  := io.dmem.bready

    }
    def DefaultMem(): Unit = {
        mem_araddr := DontCare
        mem_arvalid := false.B
        // mem_arsize := 2.U
        mem_rready := false.B
        mem_awaddr := false.B
        mem_awvalid := false.B
        // mem_awsize := 2.U
        mem_wdata := DontCare
        mem_wstrb := 0.U
        mem_wvalid := false.B
        mem_bready := false.B
    }
    def DefaultImem(): Unit = {
        imem_arready := true.B
        imem_rdata := DontCare
        imem_rresp := 0.U
        imem_rvalid := false.B
        imem_awready := false.B
        imem_wready := false.B
        imem_bresp := false.B
        imem_bvalid := false.B
    }
    def DefaultDmem(): Unit = {
        dmem_arready := true.B
        dmem_rdata := DontCare
        dmem_rresp := 0.U
        dmem_rvalid := false.B
        dmem_awready := true.B
        dmem_wready := true.B
        dmem_bresp := false.B
        dmem_bvalid := false.B
    }
}