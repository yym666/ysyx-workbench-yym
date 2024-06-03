// package stage

// import chisel3._
// import chisel3.util._

// import config.MyConfig._
// import config.InstPat._
// import unit._

// class LSU extends Module {
//     val io = IO(new Bundle{
//         val in =    Flipped(Decoupled(new MessageEX2ME))
        
//         val inst_code   = Output(UInt(INS_LEN.W))
//         val waddr       = Output(UInt(ADDR_WIDTH.W))
//         val alu_res     = Output(UInt(DATA_WIDTH.W))
//         val br_taken    = Output(Bool())
//         val br_target   = Output(UInt(ADDR_WIDTH.W))
//         val rd_addr     = Output(UInt(REG_WIDTH.W))
//         val reg_wen     = Output(Bool())
//         val mem_opt     = Output(UInt(LEN_REG.W))
//         val mem_data    = Output(UInt(DATA_WIDTH.W))
//     })

// }