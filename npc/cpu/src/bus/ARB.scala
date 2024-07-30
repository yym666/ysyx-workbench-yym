package bus

import chisel3._
import chisel3.util._

class ARB extends Module{
    val io = IO(new Bundle {
        val imem = new AXI
        val dmem = new AXI
        val  mem = Flipped(new AXI)
    })
    //DontCare
    io.mem.arlen   := 0.U
    io.mem.arid    := 0.U
    io.mem.arsize  := 0.U
    io.mem.arburst := 0.U
    io.mem.awid    := 0.U
    io.mem.awlen   := 0.U
    io.mem.awsize  := 0.U
    io.mem.awburst := 0.U
    io.mem.wlast   := false.B

    io.imem.rid     := 0.U
    io.imem.bid     := 0.U
    io.imem.rlast   := true.B
    io.dmem.rid     := 0.U
    io.dmem.bid     := 0.U
    io.dmem.rlast   := true.B
    //IMEM
    val imem_arready= RegInit(io.imem.arready)
    val imem_rdata  = RegInit(io.imem.rdata)
    val imem_rresp  = RegInit(io.imem.rresp)
    val imem_rvalid = RegInit(io.imem.rvalid)
    val imem_awready= RegInit(io.imem.awready)
    val imem_wready = RegInit(io.imem.wready)
    val imem_bresp  = RegInit(io.imem.bresp)
    val imem_bvalid = RegInit(io.imem.bvalid)
    io.imem.arready := imem_arready
    // io.imem.rdata   := imem_rdata
    io.imem.rdata   := io.mem.rdata
    io.imem.rresp   := imem_rresp
    // io.imem.rvalid  := imem_rvalid
    io.imem.rvalid  := io.mem.rvalid
    io.imem.awready := imem_awready
    io.imem.wready  := imem_wready
    io.imem.bresp   := imem_bresp
    io.imem.bvalid  := imem_bvalid
    //DMEM
    val dmem_arready= RegInit(io.dmem.arready)
    val dmem_rdata  = RegInit(io.dmem.rdata)
    val dmem_rresp  = RegInit(io.dmem.rresp)
    val dmem_rvalid = RegInit(io.dmem.rvalid)
    val dmem_awready= RegInit(io.dmem.awready)
    val dmem_wready = RegInit(io.dmem.wready)
    val dmem_bresp  = RegInit(io.dmem.bresp)
    val dmem_bvalid = RegInit(io.dmem.bvalid)
    io.dmem.arready := dmem_arready
    // io.dmem.rdata   := dmem_rdata
    io.dmem.rdata   := io.mem.rdata
    io.dmem.rresp   := dmem_rresp
    // io.dmem.rvalid  := dmem_rvalid
    io.dmem.rvalid  := io.mem.rvalid
    io.dmem.awready := dmem_awready
    io.dmem.wready  := dmem_wready
    io.dmem.bresp   := dmem_bresp
    io.dmem.bvalid  := dmem_bvalid
    //MEM
    val mem_araddr  = RegInit(io.mem.araddr)
    val mem_arvalid = RegInit(io.mem.arvalid)
    val mem_rready  = RegInit(io.mem.rready)
    val mem_awaddr  = RegInit(io.mem.awaddr)
    val mem_awvalid = RegInit(io.mem.awvalid)
    val mem_wdata   = RegInit(io.mem.wdata)
    val mem_wstrb   = RegInit(io.mem.wstrb)
    val mem_wvalid  = RegInit(io.mem.wvalid)
    val mem_bready  = RegInit(io.mem.bready)
    io.mem.araddr   := mem_araddr
    io.mem.arvalid  := mem_arvalid
    io.mem.rready   := mem_rready
    io.mem.awaddr   := mem_awaddr
    io.mem.awvalid  := mem_awvalid
    io.mem.wdata    := mem_wdata
    io.mem.wstrb    := mem_wstrb
    io.mem.wvalid   := mem_wvalid
    io.mem.bready   := mem_bready


    val rd_send :: rd_recv  :: Nil = Enum(2)
    val RDstate     = RegInit(rd_send)
    val RNXTstate   = WireDefault(rd_send)
    RNXTstate := MuxLookup(RDstate, rd_send, List(
        rd_send -> Mux(mem_arvalid && io.mem.arready, rd_recv, rd_send),
        rd_recv -> Mux(mem_rready && io.mem.rvalid, rd_send, rd_recv)
    ))
    RDstate := RNXTstate
    switch(RNXTstate){
        is(rd_send){
            when(io.imem.arvalid === true.B){
                RDtoIMEM()
                mem_arvalid := true.B
                mem_araddr  := io.imem.araddr
                dmem_arready:= false.B
            }.elsewhen(io.dmem.arvalid === true.B){
                RDtoDMEM()
                mem_arvalid := true.B
                mem_araddr  := io.dmem.araddr
                imem_arready:= false.B
            }.otherwise{
                mem_arvalid := false.B
                imem_arready:= false.B
                dmem_arready:= false.B
                mem_araddr  := 0.U
            }
        }
        is(rd_recv){
            mem_arvalid := false.B
            imem_arready:= false.B
            dmem_arready:= false.B
            mem_rready  := (io.imem.rready || io.dmem.rready)
        }
    }

    val wr_init :: wr_send :: wr_recv :: Nil = Enum(3)
    val WRstate     = RegInit(wr_init)
    val WNXTstate   = WireDefault(wr_init)
    WNXTstate := MuxLookup(WRstate, wr_init, List(
        wr_init -> Mux((io.dmem.awvalid && io.dmem.awready) && (io.dmem.wvalid && io.dmem.wready), wr_send, wr_init),
        wr_send -> Mux((io.mem.awvalid  && io.mem.awready ) && (io.mem.wvalid  && io.mem.wready ), wr_recv, wr_send),
        wr_recv -> Mux(mem_bready && dmem_bvalid, wr_init, wr_recv)
    ))
    WRstate := WNXTstate
    switch(WNXTstate){
        is(wr_init){
            dmem_awready:= io.mem.awready
            dmem_wready := io.mem.wready
            dmem_bresp  := false.B
            dmem_bvalid := false.B
            mem_awvalid := false.B
            mem_wvalid  := false.B
        }
        is(wr_send){
            dmem_bresp  := false.B
            dmem_bvalid := false.B
            mem_awvalid := io.dmem.awvalid
            mem_awaddr  := io.dmem.awaddr
            mem_wvalid  := io.dmem.wvalid
            mem_wdata   := io.dmem.wdata
            mem_wstrb   := io.dmem.wstrb
        }
        is(wr_recv){
            mem_awvalid := false.B
            mem_wvalid  := false.B
            mem_bready  := io.dmem.bready
            dmem_awready:= false.B
            dmem_wready := false.B
            dmem_bresp  := io.mem.bresp
            dmem_bvalid := io.mem.bvalid
        }
    }
    def RDtoIMEM(): Unit = {
        imem_arready:= io.mem.arready
        imem_rresp  := io.mem.rresp
        imem_awready:= io.mem.awready
        imem_wready := io.mem.wready
        imem_bresp  := io.mem.bresp
        imem_bvalid := io.mem.bvalid
    }
    def RDtoDMEM(): Unit = {
        dmem_arready:= io.mem.arready
        dmem_rresp  := io.mem.rresp
    }
}