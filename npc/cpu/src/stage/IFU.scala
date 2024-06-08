package stage

import chisel3._
import chisel3.util._
import bus._

import config.MyConfig._
import config.InstPat._
import unit._

class IFU extends Module {
    val io = IO(new Bundle {
        val out =   Decoupled(new MessageIF2ID)
        val imem=   Flipped(new AXI)

        val inst_req    = Output(Bool())

        val br_taken    =  Input(Bool())
        val br_target   =  Input(UInt(ADDR_WIDTH.W))
        val idu_done    =  Input(Bool())
        val exu_done    =  Input(Bool())
        val lsu_done    =  Input(Bool())
    })

    //PC
    val pc_reg = RegInit(UInt(ADDR_WIDTH.W), START_ADDR.U)
    val pc_nxt = Mux((io.br_taken === true.B), io.br_target, pc_reg + 4.U)
    //AXI
    // io.imem.arid    := 0.U
    // io.imem.arlen   := 0.U
    // io.imem.arburst := 0.U
    io.imem.awaddr  := DontCare
    io.imem.awvalid := false.B
    // io.imem.awid    := 0.U
    // io.imem.awlen   := 0.U
    // io.imem.awsize  := 0.U
    // io.imem.awburst := 0.U
    io.imem.wdata   := DontCare
    io.imem.wstrb   := 0.U
    io.imem.wvalid  := 0.U
    // io.imem.wlast   := false.B
    io.imem.bready  := false.B
    val imem_arvalid = RegInit(io.imem.arvalid)
    val imem_rready  = RegInit(io.imem.rready)
    // val imem_arsize  = RegInit(io.imem.arsize)
    io.imem.arvalid := imem_arvalid
    // io.imem.arsize  := imem_arsize
    io.imem.rready  := imem_rready
    io.imem.araddr  := pc_reg 
    val if_valid = RegInit(false.B)
    io.out.valid := if_valid

    val idle :: wait_imem1 :: wait_imem2 :: wait_ready :: Nil = Enum(4)
    val IFUstate = RegInit(idle)
    val NXTstate = WireDefault(idle)
    val inst_req = (io.idu_done === true.B && 
                    io.exu_done === true.B && 
                    io.lsu_done === true.B && 
                    IFUstate    === idle)

    NXTstate := MuxLookup(IFUstate, idle, List(
        idle       -> Mux(inst_req, wait_imem1, idle),
        wait_imem1 -> Mux(io.imem.arvalid & io.imem.arready, wait_imem2, wait_imem1),
        wait_imem2 -> Mux(io.imem.rvalid  & imem_rready    , wait_ready, wait_imem2),
        wait_ready -> Mux(io.out.ready, idle, wait_ready)
    ))
    IFUstate := NXTstate

    val lfsr  = RegInit(3.U(4.W))
    lfsr := Cat(lfsr(2,0), lfsr(0) ^ lfsr(1) ^ lfsr(2))
    val delay = RegInit(lfsr)

    switch(NXTstate){
        is(idle){
            delay        := lfsr
            if_valid     := false.B
            imem_arvalid := false.B
            imem_rready  := false.B
            // imem_arsize  := 2.U
        }
        is(wait_imem1){
            if_valid := io.imem.rvalid// & (io.imem.rresp === 0.U)
            when(delay === 0.U){
                imem_arvalid := true.B
                imem_rready  := false.B
                // imem_arsize  := 2.U
            }.otherwise{
                delay        := delay - 1.U
                imem_arvalid := false.B
                imem_rready  := false.B
                // imem_arsize  := 2.U
            }
        }
        is(wait_imem2){
            if_valid := io.imem.rvalid
            imem_arvalid := false.B
            imem_rready  := true.B
        }
        is(wait_ready){
            if_valid     := io.imem.rvalid// & (io.imem.rresp === 0.U)
            imem_arvalid := false.B
            imem_rready  := false.B
        }
    }

    when (inst_req === true.B) {
        pc_reg := pc_nxt
    }
    io.out.bits.inst := io.imem.rdata
    io.out.bits.pc   := pc_reg
    io.inst_req      := inst_req
}