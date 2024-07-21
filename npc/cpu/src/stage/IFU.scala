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
        val idu_done    =  Input(Bool())
        val exu_done    =  Input(Bool())
        val lsu_done    =  Input(Bool())
    })

    val pc_reg = RegInit(UInt(ADDR_WIDTH.W), START_ADDR.U)
    val pc_nxt = Mux((io.br_taken === true.B), io.br_target, pc_reg + 4.U)
    val addr_vl_tmp  = RegInit(io.addr_vl)
    io.addr     := pc_reg
    io.addr_vl  := addr_vl_tmp

    val lfsr  = RegInit(3.U(4.W))
    lfsr := Cat(lfsr(2,0), lfsr(0) ^ lfsr(1) ^ lfsr(2))
    val delay = RegInit(lfsr)

    //State
    val idle :: in_icache :: wait_ready :: Nil = Enum(3)
    val IFUstate = RegInit(idle)
    val NXTstate = WireDefault(idle)
    val inst_req = (io.idu_done === true.B && 
                    io.exu_done === true.B && 
                    io.lsu_done === true.B && 
                    IFUstate    === idle)
    io.out.valid := IFUstate    === wait_ready
    NXTstate := MuxLookup(IFUstate, idle, List(
        idle       -> Mux(inst_req, in_icache, idle),
        in_icache  -> Mux(io.inst_rd, wait_ready, in_icache),
        wait_ready -> Mux(io.out.ready, idle, wait_ready)
    ))
    IFUstate    :=  NXTstate
    addr_vl_tmp := (NXTstate === in_icache)

    when (inst_req === true.B) {
        pc_reg := pc_nxt
    }
    io.out.bits.inst := io.inst
    io.out.bits.pc   := pc_reg
    io.inst_req      := inst_req
}