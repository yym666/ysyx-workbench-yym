package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class IDU extends Module {
    val io = IO(new Bundle {
        val inst        =  Input(UInt(DATA_WIDTH.W))
        val rs1_addr    = Output(UInt(REG_WIDTH.W))
        val rs2_addr    = Output(UInt(REG_WIDTH.W))
        val rs1_data    =  Input(UInt(DATA_WIDTH.W))
        val rs2_data    =  Input(UInt(DATA_WIDTH.W))
        val rd_addr     = Output(UInt(REG_WIDTH.W))
        val op1_data    = Output(UInt(DATA_WIDTH.W))
        val op2_data    = Output(UInt(DATA_WIDTH.W))
        val excode      = Output(UInt(LEN_EXC.W))
    })

    io.rs1_addr := io.inst(19, 15)
    io.rs2_addr := io.inst(24, 20)
    io.rd_addr  := io.inst(11,  7)

    val imm_i       = io.inst(31, 20)
    val imm_i_sext  = Cat(Fill(20, imm_i(11)), imm_i)

    val decode = ListLookup(
        io.inst,
        List(ALU_ERR, OP1_ERR, OP2_ERR),
        Array(
            ADDI    -> List(ALU_ADD, OP1_RS1, OP2_IMI, isADDI),
            EBREAK  -> List(ALU_ERR, OP1_ERR, OP2_ERR, isEBREAK)
        )
    )
    val excode_tmp :: op1 :: op2 :: inst_code = decode
    io.op1_data := io.rs1_data
    io.op2_data := imm_i_sext
    io.excode   := excode_tmp
}