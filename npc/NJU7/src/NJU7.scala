import chisel3._
import chisel3.util._

class NJU7 extends Module {
    val io = IO(new Bundle {
        val output = Output(UInt(8.W))
    })

    val reg = RegInit(1.U(8.W))

    io.output := reg
}