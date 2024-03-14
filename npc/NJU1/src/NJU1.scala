import chisel3._

/**
  * Compute GCD using subtraction method.
  * Subtracts the smaller from the larger until register y is zero.
  * value in register x is then the GCD
  */
class NJU1 extends Module {
    val io = IO(new Bundle {
        val x0 = Input(UInt(2.W))    
        val x1 = Input(UInt(2.W))    
        val x2 = Input(UInt(2.W))    
        val x3 = Input(UInt(2.W))
        val s  = Input(UInt(2.W))
        val y  = Output(UInt(2.W))    
    })
    when(io.s === 0.U) {
        io.y := io.x0
    } .elsewhen(io.s === 1.U) {
        io.y := io.x1
    } .elsewhen(io.s === 2.U) {
        io.y := io.x2
    } .elsewhen(io.s === 3.U) {
        io.y := io.x3
    } .otherwise {
        io.y := 0.U
    }
}
