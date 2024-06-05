package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

class EXU extends Module {
    val io = IO(new Bundle{
        val in  =   Flipped(Decoupled(new MessageID2EX))
        val out =           Decoupled(new MessageEX2LS)

        val exu_done    = Output(Bool())
    })
    // io.in.ready := 1.U 
    val wait_id2ex :: wait_ex2ls :: Nil = Enum(2)
    val EXUstate = RegInit(wait_id2ex)
    EXUstate := MuxLookup(EXUstate, wait_id2ex) (Seq(
        wait_id2ex  -> Mux(io.in.valid , wait_ex2ls, wait_id2ex),
        wait_ex2ls  -> Mux(io.out.ready, wait_id2ex, wait_ex2ls)
    ))
    io.out.valid := Mux(EXUstate === wait_id2ex, 0.U, 1.U)
    io.in.ready  := Mux(EXUstate === wait_ex2ls, 0.U, 1.U)
    io.exu_done  := (EXUstate === wait_id2ex)

    io.out.bits.pc      := io.in.bits.pc
    io.out.bits.inst    := io.in.bits.inst
    io.out.bits.rd_addr := io.in.bits.rd_addr
    io.out.bits.reg_wen := io.in.bits.reg_wen
    io.out.bits.csr_wen := io.in.bits.csr_wen
    io.out.bits.mem_msk := io.in.bits.mem_msk
    io.out.bits.mem_opt := io.in.bits.mem_opt
    io.out.bits.mem_wdata   := io.in.bits.mem_data
    io.out.bits.inst_code   := io.in.bits.inst_code
    io.out.bits.csr_waddr   := io.in.bits.csr_waddr

    io.out.bits.br_taken  := io.in.bits.br_taken
    io.out.bits.br_target := io.in.bits.br_target

    io.out.bits.alu_res  := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (io.in.bits.excode === ALU_ADD)  -> (io.in.bits.data1 + io.in.bits.data2),
            (io.in.bits.excode === ALU_SUB)  -> (io.in.bits.data1 - io.in.bits.data2),
            (io.in.bits.excode === ALU_AND)  -> (io.in.bits.data1 & io.in.bits.data2),
            (io.in.bits.excode === ALU_OR )  -> (io.in.bits.data1 | io.in.bits.data2),
            (io.in.bits.excode === ALU_XOR)  -> (io.in.bits.data1 ^ io.in.bits.data2),
            (io.in.bits.excode === ALU_MUL)  -> (io.in.bits.data1 * io.in.bits.data2),
            (io.in.bits.excode === ALU_MULH) -> ((io.in.bits.data1.asTypeOf(UInt(64.W)) * io.in.bits.data2.asTypeOf(UInt(64.W))) >> 32.U),
            (io.in.bits.excode === ALU_SRA)  -> (io.in.bits.data1.asSInt >> io.in.bits.data2(4, 0)).asUInt,
            (io.in.bits.excode === ALU_SRL)  -> (io.in.bits.data1 >> io.in.bits.data2(4, 0)),
            (io.in.bits.excode === ALU_SLL)  -> (io.in.bits.data1 << io.in.bits.data2(4, 0))(31, 0),
            (io.in.bits.excode === ALU_SLT)  -> (io.in.bits.data1.asSInt < io.in.bits.data2.asSInt).asUInt,
            (io.in.bits.excode === ALU_SLTU) -> (io.in.bits.data1 < io.in.bits.data2).asUInt,
            (io.in.bits.excode === ALU_DIV)  -> (io.in.bits.data1.asSInt / io.in.bits.data2.asSInt).asUInt,
            //res should be SInt or UInt? or doesnt matter
            (io.in.bits.excode === ALU_DIVU) -> (io.in.bits.data1 / io.in.bits.data2).asUInt,
            (io.in.bits.excode === ALU_REM)  -> (io.in.bits.data1.asSInt % io.in.bits.data2.asSInt).asUInt,
            (io.in.bits.excode === ALU_REMU) -> (io.in.bits.data1 % io.in.bits.data2).asUInt,

            (io.in.bits.excode === ALU_CSRW) -> (io.in.bits.data1),
            (io.in.bits.excode === ALU_CSRS) -> (io.in.bits.csr_data | io.in.bits.data1)
        )
    )

    io.out.bits.mem_addr := MuxCase(
        0.U(ADDR_WIDTH.W),
        Seq(
            (io.in.bits.mem_opt === MEM_ST) -> (io.out.bits.alu_res),
            (io.in.bits.mem_opt === MEM_LD) -> (io.out.bits.alu_res)
        )
    )
}