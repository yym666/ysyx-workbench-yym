package unit

import chisel3._
import chisel3.util._
import bus._

import config.MyConfig._
import config.InstPat._

class ICH extends Module{
    val io = IO(new Bundle{
        val addr    =  Input(UInt(ADDR_WIDTH.W))
        val addr_vl =  Input(Bool())
        val inst    = Output(UInt(DATA_WIDTH.W))
        val inst_rd = Output(Bool())
        val imem    = Flipped(new AXI)
    })

    //AXI
    //DontCare
    io.imem.arid    := 0.U
    io.imem.arlen   := 0.U
    io.imem.arburst := 0.U
    io.imem.arsize  := DontCare
    io.imem.awaddr  := DontCare
    io.imem.awvalid := false.B
    io.imem.awid    := 0.U
    io.imem.awlen   := 0.U
    io.imem.awsize  := 0.U
    io.imem.awburst := 0.U
    io.imem.wdata   := DontCare
    io.imem.wstrb   := 0.U
    io.imem.wvalid  := 0.U
    io.imem.wlast   := false.B
    io.imem.bready  := false.B
    //Care
    val imem_arvalid = RegInit(io.imem.arvalid)
    val imem_rready  = RegInit(io.imem.rready)
    io.imem.arvalid := imem_arvalid
    io.imem.rready  := imem_rready
    io.imem.araddr  := io.addr

    val inst_tmp    = RegInit(0.U(32.W))
    val inst_rd_tmp = RegInit(false.B)

    val icache          = Reg(Vec(16, UInt(DATA_WIDTH.W)))
    val icache_tag      = Reg(Vec(16, UInt(30.W)))
    val icache_valid    = Reg(Vec(16, Bool()))

    val tag     =  io.addr / 4.U
    val index   = (io.addr / 4.U) & 0xF.U

    val cache_dat   = icache(index)
    val cache_tag   = icache_tag(index)
    val cache_vld   = icache_valid(index)

    val hit = ((cache_tag === tag) && (cache_vld === true.B)).asBool

    //State
    val idle :: cache_arv :: cache_hit :: cache_miss :: cache_wait :: cache_refl :: Nil = Enum(6)
    val ICHstate = RegInit(idle)
    val NXTstate = WireDefault(idle)
    NXTstate := MuxLookup(ICHstate, idle, List(
        idle       -> Mux(io.addr_vl, cache_arv, idle),
        cache_arv  -> Mux(hit, cache_hit, cache_miss),
        cache_hit  -> idle,
        cache_miss -> Mux(io.imem.arvalid & io.imem.arready, cache_wait, cache_miss),
        cache_wait -> Mux(io.imem.rvalid  & imem_rready    , cache_refl, cache_wait),
        cache_refl -> idle
    ))
    ICHstate := NXTstate

    switch(NXTstate){
        is(idle){
            imem_arvalid := false.B
            imem_rready  := false.B
            inst_rd_tmp  := false.B
        }
        is(cache_hit){
            inst_tmp    := cache_dat
            inst_rd_tmp := true.B
        }
        is(cache_miss){
            imem_arvalid := true.B
            imem_rready  := false.B
            inst_rd_tmp  := false.B
        }
        is(cache_wait){
            imem_arvalid := false.B
            imem_rready  := true.B
            inst_rd_tmp  := false.B
        }
        is(cache_refl){
            imem_arvalid := false.B
            imem_rready  := false.B
            
            inst_tmp     := Mux(io.addr(2) =/= 1.U, io.imem.rdata, io.imem.rdata >> 32)
            inst_rd_tmp  := true.B

            icache(index)       := Mux(io.addr(2) =/= 1.U, io.imem.rdata, io.imem.rdata >> 32)
            icache_tag(index)   := io.addr / 4.U
            icache_valid(index) := true.B
        }
    }
    io.inst     := inst_tmp
    io.inst_rd  := inst_rd_tmp
}