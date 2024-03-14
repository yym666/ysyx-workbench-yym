import chisel3._
import chisel3.util._

class NJU6_shif extends Module {
  val io = IO(new Bundle {
    val input = Input(UInt(8.W))
    val control = Input(UInt(3.W))
    val output = Output(UInt(8.W))
  })

  val tmp = RegInit(0.U(8.W))
  tmp := io.input

  switch(io.control) {
    is("b000".U) { tmp := 0.U } // 清零
    is("b001".U) { tmp := io.input } // 置数
    is("b010".U) { tmp := tmp >> 1.U } // 逻辑右移
    is("b011".U) { tmp := tmp << 1.U } // 逻辑左移
    is("b100".U) { tmp := (tmp.asSInt >> 1).asUInt } // 算术右移
    is("b101".U) { tmp := Cat(1.U, tmp(7, 1)) } // 左端串行输入并行输出j
    is("b110".U) { tmp := (tmp << 1.U) | (tmp >> 7.U) } // 循环右移
    is("b111".U) { tmp := (tmp >> 1.U) | (tmp << 7.U) } // 循环左移
 }

  io.output := tmp
}
