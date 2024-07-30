package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

import chisel3.dontTouch

class WBU extends Module {
    val io = IO(new Bundle{
        val in =    Flipped(Decoupled(new MessageLS2WB))

        val reg_we      = Output(Bool())
        val reg_waddr   = Output(UInt(REG_WIDTH.W))
        val reg_wdata   = Output(UInt(DATA_WIDTH.W))

        val csr_we      = Output(Bool())
        val csr_waddr   = Output(UInt(CSR_WIDTH.W))
        val csr_wdata   = Output(UInt(DATA_WIDTH.W))

        //debug
        val alu_res     = Output(UInt(DATA_WIDTH.W))
        val br_taken    = Output(Bool())
        val br_target   = Output(UInt(ADDR_WIDTH.W))

        val pc_nxt      = Output(UInt(ADDR_WIDTH.W))
        val rd_from_wb  = Output(UInt(DATA_WIDTH.W))
    })
    
    io.in.ready := 1.U
    val wait_ls2wb :: executing :: Nil = Enum(2)
    val WBUstate = RegInit(wait_ls2wb)
    WBUstate := MuxLookup(WBUstate, wait_ls2wb) (Seq(
        wait_ls2wb -> Mux(io.in.valid, executing, wait_ls2wb),
        executing  -> Mux(io.in.ready, wait_ls2wb, executing)
    ))
    io.rd_from_wb := Mux(WBUstate === executing, io.reg_waddr, 65.U)

    io.reg_wdata := MuxCase(
        io.in.bits.alu_res,
        Seq((io.in.bits.mem_opt === MEM_LD)   -> (io.in.bits.mem_rdata),
            (io.in.bits.inst_code === isJALR) -> (io.in.bits.pc + 4.U),
            (io.in.bits.inst_code === isJAL)  -> (io.in.bits.pc + 4.U))
    )
    io.reg_waddr := io.in.bits.rd_addr
    io.reg_we    := io.in.bits.reg_wen && (WBUstate === executing)  

    io.csr_wdata := io.in.bits.alu_res
    io.csr_waddr := io.in.bits.csr_waddr
    io.csr_we    := io.in.bits.csr_wen && (WBUstate === executing)

    //debug
    when (WBUstate === executing){
        io.alu_res  := io.in.bits.alu_res
        io.br_taken := io.in.bits.br_taken
        io.br_target:= io.in.bits.br_target
        io.pc_nxt := io.in.bits.pc_nxt
    }.otherwise{
        io.br_taken := false.B
        io.br_target:= 0.U
        io.alu_res  := 0.U
        io.pc_nxt := 0.U
    }

}