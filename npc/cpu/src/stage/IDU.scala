package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import javax.xml.transform.OutputKeys

class IDU extends Module {
    val io = IO(new Bundle {
        val inst        =  Input(UInt(DATA_WIDTH.W))
        val pc          =  Input(UInt(DATA_WIDTH.W))
        val rs1_addr    = Output(UInt(REG_WIDTH.W))
        val rs2_addr    = Output(UInt(REG_WIDTH.W))
        val rs1_data    =  Input(UInt(DATA_WIDTH.W))
        val rs2_data    =  Input(UInt(DATA_WIDTH.W))
        val rd_addr     = Output(UInt(REG_WIDTH.W))
        val op1_data    = Output(UInt(DATA_WIDTH.W))
        val op2_data    = Output(UInt(DATA_WIDTH.W))
        val mem_data    = Output(UInt(DATA_WIDTH.W))
        val excode      = Output(UInt(LEN_EXC.W))
        val mem_op      = Output(UInt(LEN_MEM.W))
        val reg_op      = Output(UInt(LEN_REG.W))
        val inst_code   = Output(UInt(INS_LEN.W))
        
        val br_target   = Output(UInt(ADDR_WIDTH.W))

        val halt        = Output(Bool())

//      Load & Store
        val Store   = Output(Bool())
        val Load    = Output(Bool())
        val SL_len  = Output(UInt(5.W))
    })

    io.rs1_addr := io.inst(19, 15)
    io.rs2_addr := io.inst(24, 20)
    io.rd_addr  := io.inst(11,  7)

    val imm_i       = io.inst(31, 20)
    val imm_i_sext  = Cat(Fill(20, imm_i(11)), imm_i)
    val imm_u       = io.inst(31, 12)
    val imm_u_shif  = Cat(imm_u, Fill(12, 0.U))
    val imm_s       = Cat(io.inst(31, 25), io.inst(11, 7))
    val imm_s_sext  = Cat(Fill(20, imm_s(11)), imm_s)
    val imm_b       = Cat(io.inst(31), io.inst(7), io.inst(30, 25), io.inst(11, 8))
    val imm_b_sext  = Cat(Fill(19, imm_b(11)), imm_b, 0.U(1.W))
    val imm_j       = Cat(io.inst(31), io.inst(19, 12), io.inst(20), io.inst(30, 21))
    val imm_j_sext  = Cat(Fill(11, imm_j(19)), imm_j, 0.U(1.W))

    val decode = ListLookup(
        io.inst,
        List(ALU_ERR, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, isADDI),
        Array(
            EBREAK  -> List(ALU_ERR, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, isEBREAK),
            AUIPC   -> List(ALU_ADD, OP1_PC , OP2_IMU, MEM_ERR, LSL_0, REG_WT , isAUIPC),

            ADD     -> List(ALU_ADD, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isADD),
            SUB     -> List(ALU_SUB, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isSUB),
            AND     -> List(ALU_AND, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isAND),
            OR      -> List(ALU_OR , OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isOR),
            XOR     -> List(ALU_XOR, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isXOR),
            MUL     -> List(ALU_MUL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isMUL),
            MULH    -> List(ALU_MULH, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, isMULH),
            SRA     -> List(ALU_SRA, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isSRA),
            SRL     -> List(ALU_SRL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isSRL),
            SLL     -> List(ALU_SLL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isSLL),
            SLT     -> List(ALU_SLT, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isSLT),
            SLTU    -> List(ALU_SLTU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, isSLTU),
            DIV     -> List(ALU_DIV, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isDIV),
            DIVU    -> List(ALU_DIVU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, isDIVU),
            REM     -> List(ALU_REM, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , isREM),
            REMU    -> List(ALU_REMU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, isREMU),

            ADDI    -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isADDI),
            ANDI    -> List(ALU_AND, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isANDI),
            XORI    -> List(ALU_XOR, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isXORI),
            SLTUI   -> List(ALU_SLTU,OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, isSLTUI),
            SRAI    -> List(ALU_SRA, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isSRAI),
            SRLI    -> List(ALU_SRL, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isSRLI),
            SLLI    -> List(ALU_SLL, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT , isSLLI),

            LB      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_1, REG_WT,  isLB),
            LH      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_2, REG_WT,  isLH),
            LW      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_4, REG_WT,  isLW),
            LBU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_1, REG_WT,  isLBU),
            LHU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_2, REG_WT,  isLHU),
            LWU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_4, REG_WT,  isLWU),

            SB      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_1, REG_ERR, isSB),
            SH      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_2, REG_ERR, isSH),
            SW      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_4, REG_ERR, isSW),

            BEQ     -> List(BRC_BEQ, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBEQ),
            BNE     -> List(BRC_BNE, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBNE),
            BLT     -> List(BRC_BLT, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBLT),
            BLTU    -> List(BRC_BLTU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBLTU),
            BGE     -> List(BRC_BGE, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBGE),
            BGEU    -> List(BRC_BGEU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, isBGEU),

            JAL     -> List(ALU_JAL , OP1_PC , OP2_IMJ, MEM_ERR, LSL_0, REG_WT, isJAL),
            JALR    -> List(ALU_JALR, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, isJALR)
        )
    )
    val excode_tmp :: op1 :: op2 :: mem_wen :: wb_len :: reg_wen :: inst_code_tmp :: Nil = decode
    io.op1_data := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (op1 === OP1_RS1) -> (io.rs1_data),
            (op1 === OP1_PC ) -> (io.pc)
        )
    )
    io.op2_data := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (op2 === OP2_IMU) -> (imm_u_shif),
            (op2 === OP2_IMI) -> (imm_i_sext),
            (op2 === OP2_IMS) -> (imm_s_sext),
            (op2 === OP2_IMJ) -> (imm_j_sext),
            (op2 === OP2_RS2) -> (io.rs2_data)
        )
    )
    io.Store := Mux(mem_wen === MEM_ST, true.B, false.B)
    io.Load  := Mux(mem_wen === MEM_LD, true.B, false.B)
    io.SL_len := wb_len
    
    io.mem_op   := mem_wen
    io.reg_op   := reg_wen
    io.mem_data := Cat(Fill(24, 0.U), io.rs2_data(7, 0))
    io.excode   := excode_tmp
    io.inst_code:= inst_code_tmp
    io.br_target:= io.pc + imm_b_sext
    io.halt     := Mux((inst_code_tmp === isEBREAK), true.B, false.B)
}