package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import javax.xml.transform.OutputKeys
import scala.collection.Stepper.UnboxingIntStepper

class EXU extends Module {
    val io = IO(new Bundle{
        val data1   =  Input(UInt(DATA_WIDTH.W))
        val data2   =  Input(UInt(DATA_WIDTH.W))
        val alu_res = Output(UInt(DATA_WIDTH.W))
        val br_taken= Output(Bool())
        val excode  =  Input(UInt(LEN_EXC.W))
        val mem_op  =  Input(UInt(LEN_MEM.W))
        val waddr   = Output(UInt(ADDR_WIDTH.W))

        val csr_data=  Input(UInt(DATA_WIDTH.W))
    })
    io.alu_res := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (io.excode === ALU_ADD)  -> (io.data1 + io.data2),
            (io.excode === ALU_SUB)  -> (io.data1 - io.data2),
            (io.excode === ALU_AND)  -> (io.data1 & io.data2),
            (io.excode === ALU_OR )  -> (io.data1 | io.data2),
            (io.excode === ALU_XOR)  -> (io.data1 ^ io.data2),
            (io.excode === ALU_MUL)  -> (io.data1 * io.data2),
            (io.excode === ALU_MULH) -> ((io.data1.asTypeOf(UInt(64.W)) * io.data2.asTypeOf(UInt(64.W))) >> 32.U),
            (io.excode === ALU_SRA)  -> (io.data1.asSInt >> io.data2(4, 0)).asUInt,
            (io.excode === ALU_SRL)  -> (io.data1 >> io.data2(4, 0)),
            (io.excode === ALU_SLL)  -> (io.data1 << io.data2(4, 0))(31, 0),
            (io.excode === ALU_SLT)  -> (io.data1.asSInt < io.data2.asSInt).asUInt,
            (io.excode === ALU_SLTU) -> (io.data1 < io.data2).asUInt,
            (io.excode === ALU_DIV)  -> (io.data1.asSInt / io.data2.asSInt).asUInt,
            //res should be SInt or UInt? or doesnt matter
            (io.excode === ALU_DIVU) -> (io.data1 / io.data2).asUInt,
            (io.excode === ALU_REM)  -> (io.data1.asSInt % io.data2.asSInt).asUInt,
            (io.excode === ALU_REMU) -> (io.data1 % io.data2).asUInt,
            (io.excode === ALU_JALR) -> ((io.data1 + io.data2) & ~1.U(DATA_WIDTH.W)),
            (io.excode === ALU_JAL)  -> (io.data1 + io.data2),

            (io.excode === ALU_CSRW) -> (io.data1),
            (io.excode === ALU_CSRS) -> (io.csr_data | io.data1)
        )
    )
    io.br_taken := MuxCase(
        false.B,
        Seq(
            (io.excode === BRC_BEQ)  -> (io.data1 === io.data2),
            (io.excode === BRC_BNE)  -> (io.data1 =/= io.data2),
            (io.excode === BRC_BLT)  -> (io.data1.asSInt <  io.data2.asSInt),
            (io.excode === BRC_BLTU) -> (io.data1 <  io.data2),
            (io.excode === BRC_BGE)  -> (io.data1.asSInt >= io.data2.asSInt),
            (io.excode === BRC_BGEU) -> (io.data1 >= io.data2),
            (io.excode === ALU_JALR) -> (true.B),
            (io.excode === ALU_JAL)  -> (true.B),
            (io.excode === FORCE_JUMP) -> (true.B)
        )
    )
    io.waddr := MuxCase(
        0.U(ADDR_WIDTH.W),
        Seq(
            (io.mem_op === MEM_ST) -> (io.alu_res),
            (io.mem_op === MEM_LD) -> (io.alu_res)
        )
    )
}