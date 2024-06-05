package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

class IFU extends Module {
    val io = IO(new Bundle {
        val out =   Decoupled(new MessageIF2ID)
        val br_taken    =  Input(Bool())
        val br_target   =  Input(UInt(ADDR_WIDTH.W))
        val inst        =  Input(UInt(DATA_WIDTH.W))
        val inst_req    = Output(Bool())
        val idu_done    =  Input(Bool())
        val exu_done    =  Input(Bool())
        val lsu_done    =  Input(Bool())
    })

    val idle :: wait_ready :: Nil = Enum(2)
    val IFUstate = RegInit(idle)
    IFUstate := MuxLookup(IFUstate, idle, List(
        idle        -> Mux(io.out.valid, wait_ready, idle),
        wait_ready  -> Mux(io.out.ready, idle, wait_ready)
    ))

    val if_valid = RegInit(false.B)
    val inst_req = (io.idu_done === true.B && 
                    io.exu_done === true.B && 
                    io.lsu_done === true.B && 
                    if_valid === false.B)
    io.out.valid := if_valid

    val pc_reg = RegInit(UInt(ADDR_WIDTH.W), START_ADDR.U)
    val pc_nxt = Mux((io.br_taken === true.B), io.br_target, pc_reg + 4.U)
    
    when (inst_req === true.B) {
        pc_reg := pc_nxt
        if_valid := true.B
    }
    when (inst_req === false.B) {
        if_valid := false.B
    }
    io.out.bits.pc   := pc_reg
    io.out.bits.inst := io.inst
    io.inst_req      := inst_req
}