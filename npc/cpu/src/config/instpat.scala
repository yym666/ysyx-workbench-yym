package config

import chisel3._ 
import chisel3.util._ 

object InstPat {
    val ADDI    = BitPat("b????_????_????_?????_000_?????_0010011")
    val EBREAK  = BitPat("b0000_0000_0001_00000_000_00000_1110011")

    val LEN_EXC = 5
    val ALU_ERR = 0.U(LEN_EXC.W)
    val ALU_ADD = 1.U(LEN_EXC.W)

    val LEN_OPT = 3
    val OP1_ERR = 0.U(LEN_OPT.W)
    val OP1_RS1 = 1.U(LEN_OPT.W)

    val OP2_ERR = 0.U(LEN_OPT.W)
    val OP2_RS2 = 1.U(LEN_OPT.W)
    val OP2_IMI = 2.U(LEN_OPT.W)

    val INS_LEN     = 3
    val isADDI      = 1.U(INS_LEN.W)
    val isEBREAK    = 2.U(INS_LEN.W)

}