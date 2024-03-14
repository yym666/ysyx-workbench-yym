import chisel3._
import chisel3.util._

class NJU6 extends Module {
    val io = IO(new Bundle {
        val output = Output(UInt(8.W))
    })

    val reg = RegInit(1.U(8.W))

    reg := Cat(reg(0) ^ reg(1) ^ reg(2) ^ reg(3), reg(7, 1))

    io.output := reg
}