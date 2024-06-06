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
        
        val inst_code   = Output(UInt(INS_LEN.W))
        val rdata   =  Input(UInt(DATA_WIDTH.W))
        val wdata   = Output(UInt(DATA_WIDTH.W))
        val wmask   = Output(UInt(8.W))
        val addr    = Output(UInt(ADDR_WIDTH.W))
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

    io.out.bits.br_taken  := io.in.bits.br_taken
    io.out.bits.br_target := io.in.bits.br_target

    io.inst_code := io.in.bits.inst_code

    val mem_rdata_reg = RegInit(0.U(DATA_WIDTH.W))
    when(io.isLD){
        mem_rdata_reg := MuxCase(
            0.U,
            Seq(
                (io.in.bits.mem_msk === LSL_1) -> Cat(Fill(24, io.rdata( 7)), io.rdata( 7, 0)),
                (io.in.bits.mem_msk === LSL_2) -> Cat(Fill(16, io.rdata(15)), io.rdata(15, 0)),
                (io.in.bits.mem_msk === LSL_4) -> io.rdata,
                (io.in.bits.mem_msk === LSL_1U) -> Cat(Fill(24, 0.U), io.rdata( 7, 0)),
                (io.in.bits.mem_msk === LSL_2U) -> Cat(Fill(16, 0.U), io.rdata(15, 0)),
                (io.in.bits.mem_msk === LSL_4U) -> io.rdata
            )    
        )
    }
    io.out.bits.mem_rdata   := mem_rdata_reg
    io.wdata := io.in.bits.mem_wdata
    io.wmask := MuxCase(
        0.U,
        Seq(
            (io.in.bits.mem_msk === LSL_1) -> MSK_1,
            (io.in.bits.mem_msk === LSL_2) -> MSK_2,
            (io.in.bits.mem_msk === LSL_4) -> MSK_4,
            (io.in.bits.mem_msk === LSL_1U) -> MSK_1,
            (io.in.bits.mem_msk === LSL_2U) -> MSK_2,
            (io.in.bits.mem_msk === LSL_4U) -> MSK_4
        )
    )
    io.addr  := io.in.bits.mem_addr
    io.isST  := (LSUstate === wait_ls2wb) && (io.in.bits.mem_opt === MEM_ST) 
    io.isLD  := (LSUstate === wait_ls2wb) && (io.in.bits.mem_opt === MEM_LD) 
}