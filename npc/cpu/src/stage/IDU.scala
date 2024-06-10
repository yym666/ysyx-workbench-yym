package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._

class IDU extends Module {
    val io = IO(new Bundle {
        val in  = Flipped(Decoupled(new MessageIF2ID))
        val out =         Decoupled(new MessageID2EX)

        // val inst        =  Input(UInt(ADDR_WIDTH.W))
        //from gpr
        val rs1_data    =  Input(UInt(DATA_WIDTH.W))
        val rs2_data    =  Input(UInt(DATA_WIDTH.W))
        val rs1_addr    = Output(UInt(REG_WIDTH.W))
        val rs2_addr    = Output(UInt(REG_WIDTH.W))

        //for csr
        val csr_rdata   =  Input(UInt(DATA_WIDTH.W))
        val csr_raddr   = Output(UInt(CSR_WIDTH.W))
        val set_mcause  = Output(Bool())
        val set_mepc    = Output(Bool())
        val set_mcause_val  = Output(UInt(DATA_WIDTH.W))
        val set_mepc_val    = Output(UInt(DATA_WIDTH.W))
        
        val get_mepc    =  Input(UInt(DATA_WIDTH.W))
        val get_mtvec   =  Input(UInt(DATA_WIDTH.W))
        // val br_taken    = Output(Bool())
        // val br_target   = Output(UInt(ADDR_WIDTH.W))
        val halt        = Output(Bool())

        val idu_done    = Output(Bool())
    })

    val wait_if2id :: wait_id2ex :: Nil = Enum(2)
    val IDUstate = RegInit(wait_if2id)
    IDUstate := MuxLookup(IDUstate, wait_if2id) (Seq(
        wait_if2id  -> Mux(io.in.valid , wait_id2ex, wait_if2id),
        wait_id2ex  -> Mux(io.out.ready, wait_if2id, wait_id2ex)
    ))
    io.out.valid := Mux(IDUstate === wait_if2id, 0.U, 1.U)
    io.in.ready  := Mux(IDUstate === wait_id2ex, 0.U, 1.U)
    
    io.idu_done := (IDUstate === wait_if2id)
    io.out.bits.pc      := io.in.bits.pc
    io.out.bits.inst    := io.in.bits.inst

    io.rs1_addr := io.in.bits.inst(19, 15)
    io.rs2_addr := io.in.bits.inst(24, 20)
    io.out.bits.rd_addr := io.in.bits.inst(11,  7)

    val imm_i       = io.in.bits.inst(31, 20)
    val imm_i_sext  = Cat(Fill(20, imm_i(11)), imm_i)
    val imm_u       = io.in.bits.inst(31, 12)
    val imm_u_shif  = Cat(imm_u, Fill(12, 0.U))
    val imm_s       = Cat(io.in.bits.inst(31, 25), io.in.bits.inst(11, 7))
    val imm_s_sext  = Cat(Fill(20, imm_s(11)), imm_s)
    val imm_b       = Cat(io.in.bits.inst(31), io.in.bits.inst(7), io.in.bits.inst(30, 25), io.in.bits.inst(11, 8))
    val imm_b_sext  = Cat(Fill(19, imm_b(11)), imm_b, 0.U(1.W))
    val imm_j       = Cat(io.in.bits.inst(31), io.in.bits.inst(19, 12), io.in.bits.inst(20), io.in.bits.inst(30, 21))
    val imm_j_sext  = Cat(Fill(11, imm_j(19)), imm_j, 0.U(1.W))

    val decode = ListLookup(
        io.in.bits.inst,
        List(ALU_ERR, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isADDI),
        Array(
            EBREAK  -> List(ALU_ERR, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isEBREAK),
            LUI     -> List(ALU_ADD, OP1_ERR, OP2_IMU, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isLUI),
            AUIPC   -> List(ALU_ADD, OP1_PC , OP2_IMU, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isAUIPC),

            ADD     -> List(ALU_ADD, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isADD),
            SUB     -> List(ALU_SUB, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isSUB),
            AND     -> List(ALU_AND, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isAND),
            OR      -> List(ALU_OR , OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isOR),
            XOR     -> List(ALU_XOR, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isXOR),
            MUL     -> List(ALU_MUL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isMUL),
            MULH    -> List(ALU_MULH, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isMULH),
            MULHU   -> List(ALU_MULHU,OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isMULHU),
            SRA     -> List(ALU_SRA, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isSRA),
            SRL     -> List(ALU_SRL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isSRL),
            SLL     -> List(ALU_SLL, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isSLL),
            SLT     -> List(ALU_SLT, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isSLT),
            SLTU    -> List(ALU_SLTU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isSLTU),
            DIV     -> List(ALU_DIV, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isDIV),
            DIVU    -> List(ALU_DIVU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isDIVU),
            REM     -> List(ALU_REM, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT , CSR_ERR, isREM),
            REMU    -> List(ALU_REMU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isREMU),

            ADDI    -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isADDI),
            ANDI    -> List(ALU_AND, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isANDI),
            ORI     -> List(ALU_OR , OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isORI),
            XORI    -> List(ALU_XOR, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isXORI),
            SLTUI   -> List(ALU_SLTU,OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isSLTUI),
            SRAI    -> List(ALU_SRA, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isSRAI),
            SRLI    -> List(ALU_SRL, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isSRLI),
            SLLI    -> List(ALU_SLL, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isSLLI),

            LB      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_1, REG_WT, CSR_ERR,  isLB),
            LH      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_2, REG_WT, CSR_ERR,  isLH),
            LW      -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_4, REG_WT, CSR_ERR,  isLW),
            LBU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_1U, REG_WT, CSR_ERR,  isLBU),
            LHU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_2U, REG_WT, CSR_ERR,  isLHU),
            LWU     -> List(ALU_ADD, OP1_RS1, OP2_IMI, MEM_LD , LSL_4U, REG_WT, CSR_ERR,  isLWU),

            SB      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_1, REG_ERR, CSR_ERR, isSB),
            SH      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_2, REG_ERR, CSR_ERR, isSH),
            SW      -> List(ALU_ADD, OP1_RS1, OP2_IMS, MEM_ST , LSL_4, REG_ERR, CSR_ERR, isSW),

            BEQ     -> List(BRC_BEQ, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBEQ),
            BNE     -> List(BRC_BNE, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBNE),
            BLT     -> List(BRC_BLT, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBLT),
            BLTU    -> List(BRC_BLTU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBLTU),
            BGE     -> List(BRC_BGE, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBGE),
            BGEU    -> List(BRC_BGEU, OP1_RS1, OP2_RS2, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isBGEU),

            CSRRW   -> List(ALU_CSRW, OP1_RS1, OP2_ERR, MEM_ERR, LSL_0, REG_WT, CSR_WT, isCSRRW),
            CSRRS   -> List(ALU_CSRS, OP1_RS1, OP2_ERR, MEM_ERR, LSL_0, REG_WT, CSR_WT, isCSRRS),
            ECALL   -> List(FORCE_JUMP, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, CSR_ECA, isECALL),
            MRET    -> List(FORCE_JUMP, OP1_ERR, OP2_ERR, MEM_ERR, LSL_0, REG_ERR, CSR_ERR, isMRET),

            JAL     -> List(ALU_JAL , OP1_PC , OP2_IMJ, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isJAL),
            JALR    -> List(ALU_JALR, OP1_RS1, OP2_IMI, MEM_ERR, LSL_0, REG_WT, CSR_ERR, isJALR)
        )
    )
    val excode_tmp :: op1 :: op2 :: mem_wen :: wb_msk :: reg_wen :: csr_opt :: inst_code_tmp :: Nil = decode
    io.out.bits.data1 := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (op1 === OP1_RS1) -> (io.rs1_data),
            (op1 === OP1_PC ) -> (io.in.bits.pc)
        )
    )
    io.out.bits.data2 := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (op2 === OP2_IMU) -> (imm_u_shif),
            (op2 === OP2_IMI) -> (imm_i_sext),
            (op2 === OP2_IMS) -> (imm_s_sext),
            (op2 === OP2_IMJ) -> (imm_j_sext),
            (op2 === OP2_RS2) -> (io.rs2_data)
        )
    )

    io.out.bits.br_taken := MuxCase(
        false.B,
        Seq(
            (excode_tmp === BRC_BEQ)  -> (io.out.bits.data1 === io.out.bits.data2),
            (excode_tmp === BRC_BNE)  -> (io.out.bits.data1 =/= io.out.bits.data2),
            (excode_tmp === BRC_BLT)  -> (io.out.bits.data1.asSInt <  io.out.bits.data2.asSInt),
            (excode_tmp === BRC_BLTU) -> (io.out.bits.data1 <  io.out.bits.data2),
            (excode_tmp === BRC_BGE)  -> (io.out.bits.data1.asSInt >= io.out.bits.data2.asSInt),
            (excode_tmp === BRC_BGEU) -> (io.out.bits.data1 >= io.out.bits.data2),
            (excode_tmp === ALU_JALR) -> (true.B),
            (excode_tmp === ALU_JAL)  -> (true.B),
            (excode_tmp === FORCE_JUMP) -> (true.B)
        )
    )
    io.out.bits.br_target:= MuxCase(
        io.in.bits.pc + imm_b_sext,
        Seq(
            (excode_tmp === ALU_JALR)   -> ((io.out.bits.data1 + io.out.bits.data2) & ~1.U(DATA_WIDTH.W)),
            (excode_tmp === ALU_JAL )   -> (io.out.bits.data1 + io.out.bits.data2),
            (inst_code_tmp === isMRET ) -> (io.get_mepc),
            (inst_code_tmp === isECALL) -> (io.get_mtvec)
        )
    )
//   case 0x300: return &(cpu.csr.mstatus);
//   case 0x305: return &(cpu.csr.mtvec);
//   case 0x341: return &(cpu.csr.mepc);
//   case 0x342: return &(cpu.csr.mcause);
    io.csr_raddr := MuxCase(
        0.U(CSR_WIDTH.W),
        Seq(
            (imm_i === 0x300.U) -> 1.U,
            (imm_i === 0x305.U) -> 2.U,
            (imm_i === 0x341.U) -> 3.U,
            (imm_i === 0x342.U) -> 4.U
        )
    )
    io.out.bits.csr_waddr := io.csr_raddr

    io.set_mcause       := (IDUstate === wait_id2ex) && (csr_opt === CSR_ECA)
    io.set_mcause_val   := MuxCase(1.U, Seq(((IDUstate === wait_id2ex) && (csr_opt === CSR_ECA)) -> 11.U))
    io.set_mepc         := (IDUstate === wait_id2ex) && (csr_opt === CSR_ECA)
    io.set_mepc_val     := MuxCase(1.U, Seq(((IDUstate === wait_id2ex) && (csr_opt === CSR_ECA)) -> (io.in.bits.pc + 4.U(DATA_WIDTH.W))))
    io.out.bits.csr_wen  := MuxCase(false.B, Seq((csr_opt === CSR_WT) -> (true.B)))
    io.out.bits.csr_data := io.csr_rdata
   
    io.out.bits.mem_msk  := wb_msk 
    io.out.bits.mem_opt  := mem_wen
    io.out.bits.reg_wen  := (reg_wen === REG_WT)
    io.out.bits.mem_data := MuxCase(
        0.U(DATA_WIDTH.W),
        Seq(
            (inst_code_tmp === isSB) -> (Cat(Fill(24, 0.U), io.rs2_data(7, 0))),
            (inst_code_tmp === isSH) -> (Cat(Fill(16, 0.U), io.rs2_data(15, 0))),
            (inst_code_tmp === isSW) -> (io.rs2_data)
        )
    )
    io.out.bits.excode   := excode_tmp
    io.out.bits.inst_code:= inst_code_tmp
    io.halt := Mux((inst_code_tmp === isEBREAK), true.B, false.B)
}