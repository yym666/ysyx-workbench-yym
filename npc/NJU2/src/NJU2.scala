import chisel3._

/**
  * Compute GCD using subtraction method.
  * Subtracts the smaller from the larger until register y is zero.
  * value in register x is then the GCD
  */
class NJU2 extends Module {
    val io = IO(new Bundle {
        val en  = Input(Bool())    
        val in  = Input(Vec(8,UInt(1.W)))    
        val out = Output(UInt(3.W))    
        val seg = Output(UInt(7.W))    
    })
    io.out <> DontCare
    io.seg <> DontCare
    when(io.en === true.B){
        when (io.in(0) === 1.U){
            io.out := 0.U
            io.seg := "b1000000".U
        }.elsewhen (io.in(1) === 1.U){
            io.out := 1.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(2) === 1.U){
            io.out := 2.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(3) === 1.U){
            io.out := 3.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(4) === 1.U){
            io.out := 4.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(5) === 1.U){
            io.out := 5.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(6) === 1.U){
            io.out := 6.U
            io.seg := "b1111001".U
        }.elsewhen (io.in(7) === 1.U){
            io.out := 7.U
            io.seg := "b1111001".U
        }
    } .otherwise{
        io.out := 0.U
        io.seg := "b00000000".U
    }
}
