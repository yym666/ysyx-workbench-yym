package config

import chisel3._ 
import chisel3.util._ 

object InstPat {
    val EBREAK  = BitPat("b0000_0000_0001_00000_000_00000_1110011")
    val LUI     = BitPat("b????_????_????_?????_???_?????_0110111")
    val AUIPC   = BitPat("b????_????_????_?????_???_?????_0010111")

    val ADD     = BitPat("b0000000_?????_?????_000_?????_0110011")
    val SUB     = BitPat("b0100000_?????_?????_000_?????_0110011")
    val AND     = BitPat("b0000000_?????_?????_111_?????_0110011")
    val OR      = BitPat("b0000000_?????_?????_110_?????_0110011")
    val REM     = BitPat("b0000001_?????_?????_110_?????_0110011")
    val REMU    = BitPat("b0000001_?????_?????_111_?????_0110011")
    val SLT     = BitPat("b0000000_?????_?????_010_?????_0110011")
    val SLTU    = BitPat("b0000000_?????_?????_011_?????_0110011")
    val XOR     = BitPat("b0000000_?????_?????_100_?????_0110011")
    val DIV     = BitPat("b0000001_?????_?????_100_?????_0110011")
    val DIVU    = BitPat("b0000001_?????_?????_101_?????_0110011")
    val MUL     = BitPat("b0000001_?????_?????_000_?????_0110011")
    val MULH    = BitPat("b0000001_?????_?????_001_?????_0110011")
    val SRA     = BitPat("b0100000_?????_?????_101_?????_0110011")
    val SRL     = BitPat("b0000000_?????_?????_101_?????_0110011")
    val SLL     = BitPat("b0000000_?????_?????_001_?????_0110011")

    val ADDI    = BitPat("b??????_??????_?????_000_?????_0010011")
    val ANDI    = BitPat("b??????_??????_?????_111_?????_0010011")
    val ORI     = BitPat("b??????_??????_?????_110_?????_0010011")
    val XORI    = BitPat("b??????_??????_?????_100_?????_0010011")
    val SLTUI   = BitPat("b??????_??????_?????_011_?????_0010011")
    val SLLI    = BitPat("b000000_??????_?????_001_?????_0010011")
    val SRLI    = BitPat("b000000_??????_?????_101_?????_0010011")
    val SRAI    = BitPat("b010000_??????_?????_101_?????_0010011")

    val LB      = BitPat("b??????_??????_?????_000_?????_0000011")  
    val LH      = BitPat("b??????_??????_?????_001_?????_0000011")  
    val LW      = BitPat("b??????_??????_?????_010_?????_0000011")
    val LBU     = BitPat("b??????_??????_?????_100_?????_0000011")
    val LHU     = BitPat("b??????_??????_?????_101_?????_0000011")
    val LWU     = BitPat("b??????_??????_?????_010_?????_0000011")

    val SB      = BitPat("b??????_??????_?????_000_?????_0100011")
    val SH      = BitPat("b??????_??????_?????_001_?????_0100011")
    val SW      = BitPat("b??????_??????_?????_010_?????_0100011")

    val BEQ     = BitPat("b??????_??????_?????_000_?????_1100011")
    val BNE     = BitPat("b??????_??????_?????_001_?????_1100011")
    val BLT     = BitPat("b??????_??????_?????_100_?????_1100011")
    val BLTU    = BitPat("b??????_??????_?????_110_?????_1100011")
    val BGE     = BitPat("b??????_??????_?????_101_?????_1100011")
    val BGEU    = BitPat("b??????_??????_?????_111_?????_1100011")

    val CSRRW   = BitPat("b??????_??????_?????_001_?????_1110011")
    val CSRRS   = BitPat("b??????_??????_?????_010_?????_1110011")
    val ECALL   = BitPat("b000000_000000_00000_000_00000_1110011")
    val MRET    = BitPat("b001100_000010_00000_000_00000_1110011")

    val JAL     = BitPat("b??????_??????_?????_???_?????_1101111")
    val JALR    = BitPat("b??????_??????_?????_000_?????_1100111")
                           
   
    val LEN_EXC     = 6
    val ALU_ERR     = 0.U(LEN_EXC.W)
    val ALU_ADD     = 1.U(LEN_EXC.W)
    val ALU_SUB     = 2.U(LEN_EXC.W)
    val ALU_AND     = 3.U(LEN_EXC.W)
    val ALU_OR      = 4.U(LEN_EXC.W)
    val ALU_XOR     = 5.U(LEN_EXC.W)
    val ALU_MUL     = 6.U(LEN_EXC.W)
    val ALU_MULH    = 7.U(LEN_EXC.W)
    val ALU_SLL     = 8.U(LEN_EXC.W)
    val ALU_SRL     = 9.U(LEN_EXC.W)
    val ALU_SRA     = 10.U(LEN_EXC.W)
    val ALU_SLT     = 11.U(LEN_EXC.W)
    val ALU_SLTU    = 12.U(LEN_EXC.W)
    val ALU_REM     = 13.U(LEN_EXC.W)
    val ALU_REMU    = 14.U(LEN_EXC.W)
    val ALU_DIV     = 15.U(LEN_EXC.W)
    val ALU_DIVU    = 16.U(LEN_EXC.W)
    val ALU_JALR    = 17.U(LEN_EXC.W)
    val ALU_JAL     = 18.U(LEN_EXC.W)
    val ALU_CSRW    = 19.U(LEN_EXC.W)
    val ALU_CSRS    = 20.U(LEN_EXC.W)
    
    val BRC_BEQ     = 21.U(LEN_EXC.W)
    val BRC_BNE     = 22.U(LEN_EXC.W)
    val BRC_BLT     = 23.U(LEN_EXC.W)
    val BRC_BLTU    = 24.U(LEN_EXC.W)
    val BRC_BGE     = 25.U(LEN_EXC.W)
    val BRC_BGEU    = 26.U(LEN_EXC.W)

    val FORCE_JUMP  = 30.U(LEN_EXC.W)

    val LEN_OPT = 3
    val OP1_ERR = 0.U(LEN_OPT.W)
    val OP1_RS1 = 1.U(LEN_OPT.W)
    val OP1_PC  = 2.U(LEN_OPT.W)

    val OP2_ERR = 0.U(LEN_OPT.W)
    val OP2_RS2 = 1.U(LEN_OPT.W)
    val OP2_IMI = 2.U(LEN_OPT.W)
    val OP2_IMU = 3.U(LEN_OPT.W)
    val OP2_IMS = 4.U(LEN_OPT.W)
    val OP2_IMJ = 5.U(LEN_OPT.W)

    val LEN_MEM = 3
    val MEM_ERR = 0.U(LEN_MEM.W)
    val MEM_ST  = 1.U(LEN_MEM.W)
    val MEM_LD  = 2.U(LEN_MEM.W)

    val LEN_LSL = 8
    val LSL_0   = 0.U(LEN_LSL.W)
    val LSL_1   = 1.U(LEN_LSL.W)
    val LSL_2   = 3.U(LEN_LSL.W)
    val LSL_4   = 15.U(LEN_LSL.W)
    val LSL_1U  = 2.U(LEN_LSL.W)
    val LSL_2U  = 4.U(LEN_LSL.W)
    val LSL_4U  = 16.U(LEN_LSL.W)

    val LEN_REG = 3
    val REG_ERR = 0.U(LEN_REG.W)
    val REG_WT  = 1.U(LEN_REG.W)

    val LEN_CSR = 3
    val CSR_ERR = 0.U(LEN_CSR.W)
    val CSR_WT  = 1.U(LEN_CSR.W)
    val CSR_ECA = 2.U(LEN_CSR.W)

    val INS_LEN     = 8
    val isEBREAK    = 0.U(INS_LEN.W)
    val isAUIPC     = 50.U(INS_LEN.W)
    val isLUI       = 51.U(INS_LEN.W)

    val isADD       = 1.U(INS_LEN.W)
    val isSUB       = 2.U(INS_LEN.W)
    val isAND       = 3.U(INS_LEN.W)
    val isOR        = 4.U(INS_LEN.W)
    val isXOR       = 5.U(INS_LEN.W)
    val isMUL       = 6.U(INS_LEN.W)
    val isMULH      = 7.U(INS_LEN.W)
    val isDIV       = 8.U(INS_LEN.W)
    val isDIVU      = 9.U(INS_LEN.W)
    val isREM       = 10.U(INS_LEN.W)
    val isREMU      = 11.U(INS_LEN.W)
    val isSLT       = 12.U(INS_LEN.W)
    val isSLTU      = 13.U(INS_LEN.W)
    val isSRA       = 14.U(INS_LEN.W)
    val isSRL       = 15.U(INS_LEN.W)
    val isSLL       = 16.U(INS_LEN.W)

    val isADDI      = 17.U(INS_LEN.W)
    val isANDI      = 18.U(INS_LEN.W)
    val isXORI      = 19.U(INS_LEN.W)
    val isSLTUI     = 20.U(INS_LEN.W)
    val isSLLI      = 21.U(INS_LEN.W)
    val isSRLI      = 22.U(INS_LEN.W)
    val isSRAI      = 23.U(INS_LEN.W)
    val isORI       = 45.U(INS_LEN.W)

    val isLB        = 24.U(INS_LEN.W)
    val isLH        = 25.U(INS_LEN.W)
    val isLW        = 26.U(INS_LEN.W)
    val isLBU       = 27.U(INS_LEN.W)
    val isLHU       = 28.U(INS_LEN.W)
    val isLWU       = 29.U(INS_LEN.W)

    val isSB        = 30.U(INS_LEN.W)
    val isSH        = 31.U(INS_LEN.W)
    val isSW        = 32.U(INS_LEN.W)
    
    val isBEQ       = 33.U(INS_LEN.W)
    val isBNE       = 34.U(INS_LEN.W)
    val isBLT       = 35.U(INS_LEN.W)
    val isBLTU      = 36.U(INS_LEN.W)
    val isBGE       = 37.U(INS_LEN.W)
    val isBGEU      = 38.U(INS_LEN.W)

    val isCSRRW     = 39.U(INS_LEN.W)
    val isCSRRS     = 40.U(INS_LEN.W)
    val isECALL     = 41.U(INS_LEN.W)
    val isMRET      = 42.U(INS_LEN.W)

    val isJAL       = 43.U(INS_LEN.W)
    val isJALR      = 44.U(INS_LEN.W)
//  watch 50=ecall & 45=ori
}