package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

class LSU extends Module {
    val io = IO(new Bundle{
        val in =    Flipped(Decoupled(new MessageEX2LS))
        val out=            Decoupled(new MessageLS2WB)
        
        val mem_len     = Output(UInt(5.W))
        val inst_code   = Output(UInt(INS_LEN.W))
        val rdata   =  Input(UInt(DATA_WIDTH.W))
        val wdata   = Output(UInt(DATA_WIDTH.W))
        val  addr   = Output(UInt(ADDR_WIDTH.W))
        val isST    = Output(Bool())
        val isLD    = Output(Bool())
        val lsu_done    = Output(Bool())
    })
    
    val wait_ex2ls :: wait_ls2wb :: Nil = Enum(2)
    val LSUstate = RegInit(wait_ex2ls)
    LSUstate := MuxLookup(LSUstate, wait_ex2ls) (Seq(
        wait_ex2ls  -> Mux(io.in.valid , wait_ls2wb, wait_ex2ls),
        wait_ls2wb  -> Mux(io.out.ready, wait_ex2ls, wait_ls2wb)
    ))
    io.out.valid := Mux(LSUstate === wait_ex2ls, 0.U, 1.U)
    io.in.ready  := Mux(LSUstate === wait_ls2wb, 0.U, 1.U)
    io.lsu_done  := (LSUstate === wait_ex2ls)

    io.out.bits.pc      := io.in.bits.pc
    io.out.bits.inst    := io.in.bits.inst
    io.out.bits.inst_code   := io.in.bits.inst_code
    io.out.bits.alu_res     := io.in.bits.alu_res
    io.out.bits.rd_addr := io.in.bits.rd_addr
    io.out.bits.reg_wen := io.in.bits.reg_wen
    io.out.bits.csr_wen := io.in.bits.csr_wen
    io.out.bits.mem_opt := io.in.bits.mem_opt
    io.out.bits.csr_waddr   := io.in.bits.csr_waddr

    io.mem_len   := io.in.bits.mem_len
    io.inst_code := io.in.bits.inst_code
    io.out.bits.mem_rdata   := io.rdata
    io.wdata := io.in.bits.mem_wdata
    io.addr  := io.in.bits.mem_addr
    io.isST  := (LSUstate === wait_ls2wb) && (io.in.bits.mem_opt === MEM_ST) 
    io.isLD  := (LSUstate === wait_ls2wb) && (io.in.bits.mem_opt === MEM_LD) 
}