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
        val addr        = Output(UInt(ADDR_WIDTH.W))
        val addr_vl     = Output(Bool())
        val inst        =  Input(UInt(DATA_WIDTH.W))
        val inst_rd     =  Input(Bool())

        val inst_req    = Output(Bool())
        val br_taken    =  Input(Bool())
        val br_target   =  Input(UInt(ADDR_WIDTH.W))
    })

    val pc_reg = RegInit(UInt(ADDR_WIDTH.W), START_ADDR.U)
    val pc_nxt = Mux((io.br_taken === true.B), io.br_target, pc_reg + 4.U)
    val addr_vl_tmp  = RegInit(io.addr_vl)
    val addr_tmp     = RegInit(io.addr)
    io.addr     := addr_tmp
    io.addr_vl  := addr_vl_tmp

    val flush = RegInit(false.B)
    //State
    val idle :: in_icache :: wait_ready :: Nil = Enum(3)
    val IFUstate = RegInit(idle)
    val NXTstate = WireDefault(idle)
    val inst_req = (IFUstate    === idle)
    io.out.valid :=(IFUstate    === wait_ready && flush =/= true.B)
    NXTstate := MuxLookup(IFUstate, idle, List(
        idle       -> Mux(inst_req, in_icache, idle),
        in_icache  -> Mux(io.inst_rd, wait_ready, in_icache),
        wait_ready -> Mux(io.out.ready, idle, wait_ready)
    ))
    IFUstate    :=  NXTstate
    addr_vl_tmp := (NXTstate === in_icache)
    val cache_pc_update = RegInit(false.B)
    when (NXTstate === in_icache && IFUstate === idle){
        cache_pc_update := true.B
    }.otherwise{
        cache_pc_update := false.B
    }
    when (cache_pc_update === true.B){
        addr_tmp := pc_reg
    }

    when (io.br_taken === true.B) {
        pc_reg := pc_nxt
        flush  := true.B 
    }.elsewhen(IFUstate === idle && flush === true.B){
        flush  := false.B
    }.elsewhen(IFUstate === idle){
        pc_reg := pc_nxt
    }
    io.out.bits.inst := io.inst
    io.out.bits.pc   := pc_reg
    io.inst_req      := inst_req
}