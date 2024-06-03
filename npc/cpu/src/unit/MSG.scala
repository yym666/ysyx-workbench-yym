package unit

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._

class MessageIF2ID extends Bundle {
    val pc          = Output(UInt(ADDR_WIDTH.W))
    val inst        = Output(UInt(DATA_WIDTH.W))
}

class MessageID2EX extends Bundle {
    // val pc      = Output(UInt(ADDR_WIDTH.W))
    // val inst    = Output(UInt(DATA_WIDTH.W))
    val data1   = Output(UInt(DATA_WIDTH.W))
    val data2   = Output(UInt(DATA_WIDTH.W))
    val excode  = Output(UInt(LEN_EXC.W))
    val rd_addr = Output(UInt(REG_WIDTH.W))
    val reg_wen = Output(Bool())
    val mem_opt = Output(UInt(LEN_MEM.W))
    val mem_data    = Output(UInt(DATA_WIDTH.W))
    val csr_data    = Output(UInt(DATA_WIDTH.W))
    val inst_code   = Output(UInt(INS_LEN.W))
    val br_target   = Output(UInt(ADDR_WIDTH.W))
}

class MessageEX2LS extends Bundle {
    val pc          = Output(UInt(ADDR_WIDTH.W))
    val inst        = Output(UInt(DATA_WIDTH.W))
    val inst_code   = Output(UInt(INS_LEN.W))
    val waddr       = Output(UInt(ADDR_WIDTH.W))
    val alu_res     = Output(UInt(DATA_WIDTH.W))
    val br_taken    = Output(Bool())
    val br_target   = Output(UInt(ADDR_WIDTH.W))
    val rd_addr     = Output(UInt(REG_WIDTH.W))
    val reg_wen     = Output(Bool())
    val mem_opt     = Output(UInt(LEN_REG.W))
    val mem_data    = Output(UInt(DATA_WIDTH.W))
}