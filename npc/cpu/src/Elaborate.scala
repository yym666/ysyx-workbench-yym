import circt.stage._

object Elaborate extends App {
  def npc = new ysyx_23060250()
  val generator = Seq(chisel3.stage.ChiselGeneratorAnnotation(() => npc))
  (new ChiselStage).execute(args, generator :+ CIRCTTargetAnnotation(CIRCTTarget.Verilog))
}