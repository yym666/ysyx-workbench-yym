package bus

import chisel3._
import chisel3.util._

class XBR extends Module{
    val io = IO(new Bundle {
        val arb   = new AXI
        val uart  = Flipped(new AXI)
        val sram  = Flipped(new AXI)
        val clint = Flipped(new AXI)
    })
    //ARB
    val arb_arready = RegInit(io.arb.arready)
    val arb_rdata   = RegInit(io.arb.rdata)
    val arb_rresp   = RegInit(io.arb.rresp)
    val arb_rvalid  = RegInit(io.arb.rvalid)
    val arb_awready = RegInit(io.arb.awready)
    val arb_wready  = RegInit(io.arb.wready)
    val arb_bresp   = RegInit(io.arb.bresp)
    val arb_bvalid  = RegInit(io.arb.bvalid)
    io.arb.arready  := arb_arready
    io.arb.rdata    := arb_rdata
    io.arb.rresp    := arb_rresp
    io.arb.rvalid   := arb_rvalid
    io.arb.awready  := arb_awready
    io.arb.wready   := arb_wready
    io.arb.bresp    := arb_bresp
    io.arb.bvalid   := arb_bvalid
    //UART
    val uart_araddr = RegInit(io.uart.araddr)
    val uart_arvalid= RegInit(io.uart.arvalid)
    val uart_rready = RegInit(io.uart.rready)
    val uart_awaddr = RegInit(io.uart.awaddr)
    val uart_awvalid= RegInit(io.uart.awvalid)
    val uart_wdata  = RegInit(io.uart.wdata)
    val uart_wstrb  = RegInit(io.uart.wstrb)
    val uart_wvalid = RegInit(io.uart.wvalid)
    val uart_bready = RegInit(io.uart.bready)
    io.uart.araddr  := uart_araddr
    io.uart.arvalid := uart_arvalid
    io.uart.rready  := uart_rready
    io.uart.awaddr  := uart_awaddr
    io.uart.awvalid := uart_awvalid
    io.uart.wdata   := uart_wdata
    io.uart.wstrb   := uart_wstrb
    io.uart.wvalid  := uart_wvalid
    io.uart.bready  := uart_bready
    //CLINT
    val clint_araddr = RegInit(io.clint.araddr)
    val clint_arvalid= RegInit(io.clint.arvalid)
    val clint_rready = RegInit(io.clint.rready)
    val clint_awaddr = RegInit(io.clint.awaddr)
    val clint_awvalid= RegInit(io.clint.awvalid)
    val clint_wdata  = RegInit(io.clint.wdata)
    val clint_wstrb  = RegInit(io.clint.wstrb)
    val clint_wvalid = RegInit(io.clint.wvalid)
    val clint_bready = RegInit(io.clint.bready)
    io.clint.araddr  := clint_araddr
    io.clint.arvalid := clint_arvalid
    io.clint.rready  := clint_rready
    io.clint.awaddr  := clint_awaddr
    io.clint.awvalid := clint_awvalid
    io.clint.wdata   := clint_wdata
    io.clint.wstrb   := clint_wstrb
    io.clint.wvalid  := clint_wvalid
    io.clint.bready  := clint_bready
    //SRAM
    val sram_araddr  = RegInit(io.sram.araddr)
    val sram_arvalid = RegInit(io.sram.arvalid)
    val sram_rready  = RegInit(io.sram.rready)
    val sram_awaddr  = RegInit(io.sram.awaddr)
    val sram_awvalid = RegInit(io.sram.awvalid)
    val sram_wdata   = RegInit(io.sram.wdata)
    val sram_wstrb   = RegInit(io.sram.wstrb)
    val sram_wvalid  = RegInit(io.sram.wvalid)
    val sram_bready  = RegInit(io.sram.bready)
    io.sram.araddr   := sram_araddr
    io.sram.arvalid  := sram_arvalid
    io.sram.rready   := sram_rready
    io.sram.awaddr   := sram_awaddr
    io.sram.awvalid  := sram_awvalid
    io.sram.wdata    := sram_wdata
    io.sram.wstrb    := sram_wstrb
    io.sram.wvalid   := sram_wvalid
    io.sram.bready   := sram_bready
   
    val chose :: invld :: uart1 :: uart2 :: sram1 :: sram2 :: clint1 :: clint2 :: Nil = Enum(8)
    val aw = Wire(UInt(4.W))
    val ar = Wire(UInt(4.W))
    aw := MuxCase(invld, Seq(
        (io.arb.awaddr >=  "h8000_0000".U(32.W) && io.arb.awaddr  <= "h87ff_ffff".U(32.W)) -> sram1,
        (io.arb.awaddr === "ha000_0048".U(32.W) || io.arb.awaddr === "ha000_004c".U(32.W)) -> clint1,
        (io.arb.awaddr === "ha000_03f8".U(32.W)) -> uart1
    ))
    ar := MuxCase(invld, Seq(
        (io.arb.araddr >=  "h8000_0000".U(32.W) && io.arb.araddr <=  "h87ff_ffff".U(32.W)) -> sram1,
        (io.arb.araddr === "ha000_0048".U(32.W) || io.arb.araddr === "ha000_004c".U(32.W)) -> clint1,
        (io.arb.araddr === "ha000_03f8".U(32.W)) -> uart1
    ))

    val state = RegInit(chose)
    val nextState = WireDefault(chose)
    nextState := MuxLookup(state, chose)(Seq(
        chose -> Mux((io.arb.awvalid & io.arb.awready)|(io.arb.arvalid & io.arb.arready), Mux(
                      io.arb.awvalid & io.arb.awready, aw, ar), chose),
        sram1 -> Mux((io.sram.rready & io.sram.rvalid)|(io.sram.bready & io.sram.bvalid), sram2, sram1),
        sram2 -> Mux((io.arb.rready & io.arb.rvalid)|(io.arb.bready & io.arb.bvalid), chose, sram2),
        uart1 -> Mux((io.uart.rready & io.uart.rvalid)|(io.uart.bready & io.uart.bvalid), uart2, uart1),
        uart2 -> Mux((io.arb.rready & io.arb.rvalid)|(io.arb.bready & io.arb.bvalid), chose, uart2),
        clint1-> Mux((io.clint.rready & io.clint.rvalid)|(io.clint.bready & io.clint.bvalid), clint2, clint1),
        clint2-> Mux((io.arb.rready & io.arb.rvalid)|(io.arb.bready & io.arb.bvalid), chose, clint2)
    ))
    state := nextState

    switch(nextState){
        is(chose){
        DefaultArb()
        DefaultSram()
        DefaultUart()
        DefaultClint()
        }
        is(invld){
        DefaultArb()
        DefaultSram()
        DefaultUart()
        DefaultClint()
        arb_bresp := Mux(aw === invld, 1.U, 0.U)
        arb_rresp := Mux(ar === invld, 1.U, 0.U)
        }
        is(sram1){
        Connectsram()
        DefaultUart()
        DefaultClint()
        }
        is(sram2){
        Connectsram()
        DefaultUart()
        DefaultClint()
        sram_arvalid := false.B
        sram_rready := false.B
        sram_awvalid := false.B
        sram_wvalid := false.B
        sram_bready := false.B
        }
        is(uart1){
        ConnectUart()
        DefaultSram()
        DefaultClint()
        }
        is(uart2){
        ConnectUart()
        DefaultSram()
        DefaultClint()
        uart_arvalid := false.B
        uart_rready := false.B
        uart_awvalid := false.B
        uart_wvalid := false.B
        uart_bready := false.B      
        }
        is(clint1){
        ConnectClint()
        DefaultSram()
        }
        is(clint2){
        ConnectClint()
        DefaultSram()
        clint_arvalid := false.B
        clint_rready := false.B
        clint_awvalid := false.B
        clint_wvalid := false.B
        clint_bready := false.B      
        }
    }
    //-----------------------------------------------------------------------------------------------------------------------------------
    def ConnectUart(): Unit = {
    arb_arready := io.uart.arready
    arb_rdata := io.uart.rdata
    arb_rresp := io.uart.rresp
    arb_rvalid := io.uart.rvalid
    arb_awready := io.uart.awready
    arb_wready := io.uart.wready
    arb_bresp := io.uart.bresp
    arb_bvalid := io.uart.bvalid

    uart_araddr := io.arb.araddr
    uart_arvalid := io.arb.arvalid
    uart_rready := io.arb.rready
    uart_awaddr := io.arb.awaddr
    uart_awvalid := io.arb.awvalid
    uart_wdata := io.arb.wdata
    uart_wstrb := io.arb.wstrb
    uart_wvalid := io.arb.wvalid
    uart_bready := io.arb.bready
    
    }
    def ConnectClint(): Unit = {
    arb_arready := io.clint.arready
    arb_rdata := io.clint.rdata
    arb_rresp := io.clint.rresp
    arb_rvalid := io.clint.rvalid
    arb_awready := io.clint.awready
    arb_wready := io.clint.wready
    arb_bresp := io.clint.bresp
    arb_bvalid := io.clint.bvalid

    clint_araddr := io.arb.araddr
    clint_arvalid := io.arb.arvalid
    clint_rready := io.arb.rready
    clint_awaddr := io.arb.awaddr
    clint_awvalid := io.arb.awvalid
    clint_wdata := io.arb.wdata
    clint_wstrb := io.arb.wstrb
    clint_wvalid := io.arb.wvalid
    clint_bready := io.arb.bready
    
    }
    def Connectsram(): Unit = {
    arb_arready := io.sram.arready
    arb_rdata := io.sram.rdata
    arb_rresp := io.sram.rresp
    arb_rvalid := io.sram.rvalid
    arb_awready := io.sram.awready
    arb_wready := io.sram.wready
    arb_bresp := io.sram.bresp
    arb_bvalid := io.sram.bvalid

    sram_araddr := io.arb.araddr
    sram_arvalid := io.arb.arvalid
    sram_rready := io.arb.rready
    sram_awaddr := io.arb.awaddr
    sram_awvalid := io.arb.awvalid
    sram_wdata := io.arb.wdata
    sram_wstrb := io.arb.wstrb
    sram_wvalid := io.arb.wvalid
    sram_bready := io.arb.bready

    }
    def DefaultUart(): Unit = {
    uart_araddr := DontCare
    uart_arvalid := false.B
    uart_rready := false.B
    uart_awaddr := false.B
    uart_awvalid := false.B
    uart_wdata := DontCare
    uart_wstrb := 0.U
    uart_wvalid := false.B
    uart_bready := false.B
    }
    def DefaultClint(): Unit = {
    clint_araddr := DontCare
    clint_arvalid := false.B
    clint_rready := false.B
    clint_awaddr := false.B
    clint_awvalid := false.B
    clint_wdata := DontCare
    clint_wstrb := 0.U
    clint_wvalid := false.B
    clint_bready := false.B
    }
    def DefaultSram(): Unit = {
    sram_araddr := DontCare
    sram_arvalid := false.B
    sram_rready := false.B
    sram_awaddr := false.B
    sram_awvalid := false.B
    sram_wdata := DontCare
    sram_wstrb := 0.U
    sram_wvalid := false.B
    sram_bready := false.B
    }
    def DefaultArb(): Unit = {
    arb_arready := true.B
    arb_rdata := DontCare
    arb_rresp := 0.U
    arb_rvalid := false.B
    arb_awready := true.B
    arb_wready := true.B
    arb_bresp := 0.U
    arb_bvalid := false.B
    }

}