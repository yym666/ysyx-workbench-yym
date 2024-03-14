import chisel3._

/**
  * Compute GCD using subtraction method.
  * Subtracts the smaller from the larger until register y is zero.
  * value in register x is then the GCD
  */
class NJU3 extends Module {
    val io = IO(new Bundle {
        val en  = Input(Bool())    
        val opt = Input(UInt(3.W))    
        val a   = Input(SInt(4.W))
        val b   = Input(SInt(4.W))
        val c   = Output(SInt(4.W))    
        val zf  = Output(Bool())
        val cr  = Output(Bool())
        val of  = Output(Bool())
    })
    val res = Wire(SInt(5.W))
    when (io.opt === "b000".U){
        io.c := io.a + io.b
        res := io.a + io.b
    } .elsewhen (io.opt === "b001".U){
        //io.c := io.a + (~io.b) + 1.S
        res := io.a + (~io.b) + 1.S
    } .elsewhen (io.opt === "b010".U){
        //io.c := ~io.a
        res := ~io.a
    } .elsewhen (io.opt === "b100".U){
        //io.c := io.a | io.b
        res := io.a | io.b
    } .elsewhen (io.opt === "b011".U){
        //io.c := io.a & io.b
        res := io.a & io.b
    } .elsewhen (io.opt === "b101".U){
        //io.c := io.a ^ io.b
        res := io.a ^ io.b
    } .elsewhen (io.opt === "b110".U){
        res := Mux(io.a < io.b, 1.S, 0.S)
        //res := 0.S
    } .otherwise{
        res := Mux(io.a === io.b, 1.S, 0.S)
        //res := 0.S
    }
    io.c := res(3,0).asSInt
    io.zf := io.c === 0.S
    io.cr := res(4)
    io.of := (io.a(3) === io.b(3)) && (io.c(3) =/= io.a(3))
}
