package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

class EXU extends Module {
    val io = IO(new Bundle{
        val in =    Flipped(Decoupled(new MessageID2EX))
        
        val inst_code   = Output(UInt(INS_LEN.W))
        val waddr       = Output(UInt(ADDR_WIDTH.W))
        val alu_res     = Output(UInt(DATA_WIDTH.W))
        val br_taken    = Output(Bool())
        val br_target   = Output(UInt(ADDR_WIDTH.W))
        val rd_addr     = Output(UInt(REG_WIDTH.W))
        val reg_wen     = Output(Bool())
        val mem_opt     = Output(UInt(LEN_REG.W))
        val mem_data    = Output(UInt(DATA_WIDTH.W))
    })
    io.in.ready := 1.U 
    val wait_id2ex :: executing :: Nil = Enum(2)
    val EXUstate = RegInit(wait_id2ex)
    EXUstate := MuxLookup(EXUstate, wait_id2ex) (Seq(
        wait_id2ex  -> Mux(io.in.valid, executing, wait_id2ex),
        executing   -> Mux(io.in.ready, wait_id2ex, executing)
    ))

    // io.out.bits.pc      := io.in.bits.pc
    // io.out.bits.inst    := io.in.bits.inst
    io.rd_addr  := io.in.bits.rd_addr
    io.reg_wen  := (io.in.bits.reg_wen === true.B) && (EXUstate === executing)
    io.mem_opt  := io.in.bits.mem_opt
    io.mem_data := io.in.bits.mem_data
    io.inst_code    := io.in.bits.inst_code
    io.br_target    := io.in.bits.br_target

    io.alu_res  := MuxCase(
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
            (io.in.bits.excode === ALU_JALR) -> ((io.in.bits.data1 + io.in.bits.data2) & ~1.U(DATA_WIDTH.W)),
            (io.in.bits.excode === ALU_JAL)  -> (io.in.bits.data1 + io.in.bits.data2),

            (io.in.bits.excode === ALU_CSRW) -> (io.in.bits.data1),
            (io.in.bits.excode === ALU_CSRS) -> (io.in.bits.csr_data | io.in.bits.data1)
        )
    )
    io.br_taken := MuxCase(
        false.B,
        Seq(
            (io.in.bits.excode === BRC_BEQ)  -> (io.in.bits.data1 === io.in.bits.data2),
            (io.in.bits.excode === BRC_BNE)  -> (io.in.bits.data1 =/= io.in.bits.data2),
            (io.in.bits.excode === BRC_BLT)  -> (io.in.bits.data1.asSInt <  io.in.bits.data2.asSInt),
            (io.in.bits.excode === BRC_BLTU) -> (io.in.bits.data1 <  io.in.bits.data2),
            (io.in.bits.excode === BRC_BGE)  -> (io.in.bits.data1.asSInt >= io.in.bits.data2.asSInt),
            (io.in.bits.excode === BRC_BGEU) -> (io.in.bits.data1 >= io.in.bits.data2),
            (io.in.bits.excode === ALU_JALR) -> (true.B),
            (io.in.bits.excode === ALU_JAL)  -> (true.B),
            (io.in.bits.excode === FORCE_JUMP) -> (true.B)
        )
    )
    io.waddr := MuxCase(
        0.U(ADDR_WIDTH.W),
        Seq(
            (io.mem_opt === MEM_ST) -> (io.alu_res),
            (io.mem_opt === MEM_LD) -> (io.alu_res)
        )
    )
}