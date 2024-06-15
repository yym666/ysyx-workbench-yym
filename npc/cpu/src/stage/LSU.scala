package stage

import chisel3._
import chisel3.util._

import config.MyConfig._
import config.InstPat._
import unit._
import bus._

class LSU extends Module {
    val io = IO(new Bundle{
        val in =    Flipped(Decoupled(new MessageEX2LS))
        val out=            Decoupled(new MessageLS2WB)
        val dmem =  Flipped(new AXI)
        
        // val wdata   = Output(UInt(DATA_WIDTH.W))
        // val wmask   = Output(UInt(8.W))
        
        val isST    = Output(Bool())
        val isLD    = Output(Bool())
        val lsu_done    = Output(Bool())
    })
    io.out.bits.pc      := io.in.bits.pc
    io.out.bits.inst    := io.in.bits.inst
    io.out.bits.inst_code   := io.in.bits.inst_code
    io.out.bits.alu_res     := io.in.bits.alu_res
    io.out.bits.rd_addr := io.in.bits.rd_addr
    io.out.bits.reg_wen := io.in.bits.reg_wen
    io.out.bits.csr_wen := io.in.bits.csr_wen
    io.out.bits.mem_opt := io.in.bits.mem_opt
    io.out.bits.csr_waddr   := io.in.bits.csr_waddr
    io.out.bits.br_taken  := io.in.bits.br_taken
    io.out.bits.br_target := io.in.bits.br_target
    
    val in_ready  = RegInit(io.in.ready)
    val out_valid = RegInit(io.out.valid)
    io.in.ready  := in_ready
    io.out.valid := out_valid

    io.dmem.arid    := 0.U
    io.dmem.arlen   := 0.U
    io.dmem.arburst := 0.U
    io.dmem.arsize  := DontCare
    io.dmem.awid    := 0.U
    io.dmem.awlen   := 0.U
    io.dmem.awsize  := DontCare
    io.dmem.awburst := 0.U
    io.dmem.wlast   := false.B
    val dmem_arvalid= RegInit(io.dmem.arvalid)
    val dmem_rready = RegInit(io.dmem.rready)
    val dmem_awvalid= RegInit(io.dmem.awvalid)
    val dmem_wvalid = RegInit(io.dmem.wvalid)
    val dmem_bready = RegInit(io.dmem.bready)
    io.dmem.arvalid:= dmem_arvalid
    io.dmem.rready := dmem_rready
    io.dmem.awvalid:= dmem_awvalid
    io.dmem.wvalid := dmem_wvalid
    io.dmem.bready := dmem_bready

    val wait_ex2ls :: slstate1 :: slstate2 :: slstate3 :: wait_ls2wb :: Nil = Enum(5)
    val LSUstate = RegInit(wait_ex2ls)
    val NXTstate = WireDefault(wait_ex2ls)
    NXTstate := MuxLookup(LSUstate, wait_ex2ls, List(
        wait_ex2ls  -> Mux(io.in.valid , Mux((io.isST | io.isLD), slstate1, wait_ls2wb), wait_ex2ls),
        wait_ls2wb  -> Mux(io.out.ready, wait_ex2ls, wait_ls2wb),
        slstate1    -> Mux((io.dmem.arvalid & io.dmem.arready) |
                           (io.dmem.awvalid & io.dmem.awready & io.dmem.wvalid & io.dmem.wready),
                            slstate2, slstate1),
        slstate2    -> Mux((io.dmem.rvalid & dmem_rready) | (io.dmem.bvalid & dmem_bready),
                            slstate3, slstate2),
        slstate3    -> Mux(io.out.ready, wait_ex2ls, slstate3)
    ))
    LSUstate := NXTstate
    io.lsu_done  := (LSUstate === wait_ex2ls)

    val lfsr  = RegInit(3.U(4.W))
    lfsr := Cat(lfsr(2,0), lfsr(0) ^ lfsr(1) ^ lfsr(2))
    val delay = RegInit(lfsr)
    
    switch(NXTstate){
        is(wait_ex2ls){
            in_ready    := true.B
            out_valid   := false.B
            
            delay       := lfsr
            dmem_arvalid:= false.B
            dmem_rready := false.B
            dmem_awvalid:= false.B
            dmem_wvalid := false.B
            dmem_bready := false.B
        }
        is(wait_ls2wb){
            in_ready    := false.B
            out_valid   := true.B
            
            dmem_arvalid:= false.B
            dmem_rready := false.B
            dmem_awvalid:= false.B
            dmem_wvalid := false.B
            dmem_bready := false.B
        }
        is(slstate1){
            in_ready  := false.B
            out_valid := Mux(io.isLD, io.dmem.rvalid, io.dmem.bvalid)
            
            when(delay === 0.U){
                dmem_arvalid:= Mux(io.isLD, true.B , false.B)
                dmem_rready := Mux(io.isLD, false.B, false.B)
                dmem_awvalid:= Mux(io.isST, true.B , false.B)
                dmem_wvalid := Mux(io.isST, true.B , false.B)
                dmem_bready := Mux(io.isST, false.B, false.B)
            }.otherwise{
                delay       := delay - 1.U
                dmem_arvalid:= false.B
                dmem_rready := false.B
                dmem_awvalid:= false.B
                dmem_wvalid := false.B
                dmem_bready := false.B
            }
        }
        is(slstate2){
            in_ready    := false.B
            out_valid   := Mux(io.isLD, io.dmem.rvalid, io.dmem.bvalid)
            dmem_arvalid:= Mux(io.isLD, false.B, false.B)
            dmem_rready := Mux(io.isLD, true.B , false.B)
            dmem_awvalid:= Mux(io.isST, false.B, false.B)
            dmem_wvalid := Mux(io.isST, false.B, false.B)
            dmem_bready := Mux(io.isST, true.B , false.B)
        }
        is(slstate3){
            in_ready    := false.B
            out_valid   := Mux(io.isLD, io.dmem.rvalid, io.dmem.bvalid)
            dmem_arvalid:= false.B
            dmem_rready := false.B
            dmem_awvalid:= false.B
            dmem_wvalid := false.B
            dmem_bready := false.B
        }
    }

    io.dmem.araddr   := io.in.bits.mem_addr
    io.dmem.awaddr   := io.in.bits.mem_addr
    val mask_tmp= Wire(UInt(8.W))
    val shift   = Wire(UInt(32.W))
    val mem_addr_tmp = Wire(UInt(32.W))
    mask_tmp := MuxCase(
        0.U,
        Seq(
            (io.in.bits.mem_msk === LSL_1) -> MSK_1,
            (io.in.bits.mem_msk === LSL_2) -> MSK_2,
            (io.in.bits.mem_msk === LSL_4) -> MSK_4,
            (io.in.bits.mem_msk === LSL_1U) -> MSK_1,
            (io.in.bits.mem_msk === LSL_2U) -> MSK_2,
            (io.in.bits.mem_msk === LSL_4U) -> MSK_4
        )
    )
    mem_addr_tmp := io.in.bits.mem_addr & (~7.U(32.W))
    shift        := io.in.bits.mem_addr - mem_addr_tmp
    io.dmem.wstrb := mask_tmp << shift(2, 0)
    io.dmem.wdata := io.in.bits.mem_wdata << (shift(2, 0) << 3)
    // io.dmem.wstrb := MuxCase(
    //     0.U,
    //     Seq(
    //         (io.in.bits.mem_msk === LSL_1) -> MSK_1,
    //         (io.in.bits.mem_msk === LSL_2) -> MSK_2,
    //         (io.in.bits.mem_msk === LSL_4) -> MSK_4,
    //         (io.in.bits.mem_msk === LSL_1U) -> MSK_1,
    //         (io.in.bits.mem_msk === LSL_2U) -> MSK_2,
    //         (io.in.bits.mem_msk === LSL_4U) -> MSK_4
    //     )
    // )
    val rdata_shif = io.dmem.rdata >> (shift(2, 0) << 3)
    val rdatareg = RegInit(UInt(DATA_WIDTH.W), 0.U)
    rdatareg := Mux(NXTstate === slstate3, Mux(io.isLD, MuxCase(0.U, Seq(
                        (io.in.bits.mem_msk === LSL_1) -> Cat(Fill(24, rdata_shif( 7)), rdata_shif( 7, 0)),
                        (io.in.bits.mem_msk === LSL_2) -> Cat(Fill(16, rdata_shif(15)), rdata_shif(15, 0)),
                        (io.in.bits.mem_msk === LSL_4) -> rdata_shif,
                        (io.in.bits.mem_msk === LSL_1U) -> Cat(Fill(24, 0.U), rdata_shif( 7, 0)),
                        (io.in.bits.mem_msk === LSL_2U) -> Cat(Fill(16, 0.U), rdata_shif(15, 0)),
                        (io.in.bits.mem_msk === LSL_4U) -> rdata_shif
                    )), 0.U), rdatareg)
    io.out.bits.mem_rdata   := rdatareg
    io.isST  := (io.in.bits.mem_opt === MEM_ST) 
    io.isLD  := (io.in.bits.mem_opt === MEM_LD) 
}