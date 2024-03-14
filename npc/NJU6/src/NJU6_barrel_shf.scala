
import chisel3._
import chisel3.util._

class NJU6_barrel_shf extends Module {
    val io = IO(new Bundle {
        val input = Input(UInt(8.W))
        val amt = Input(UInt(3.W))
        val output = Output(UInt(8.W))
    })

    val tmp = RegInit(0.U(8.W))

    switch(io.amt) {
        is(1.U) { tmp := Cat(io.input(0), io.input(7, 1)) }
        is(2.U) { tmp := Cat(io.input(1, 0), io.input(7, 2)) }
        is(3.U) { tmp := Cat(io.input(2, 0), io.input(7, 3)) }
        is(4.U) { tmp := Cat(io.input(3, 0), io.input(7, 4)) }
        is(5.U) { tmp := Cat(io.input(4, 0), io.input(7, 5)) }
        is(6.U) { tmp := Cat(io.input(5, 0), io.input(7, 6)) }
        is(7.U) { tmp := Cat(io.input(6, 0), io.input(7)) }
    }
    io.output := tmp
}