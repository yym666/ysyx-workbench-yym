// import Mill dependency
import mill._
import mill.scalalib._
import mill.scalalib.scalafmt.ScalafmtModule
import mill.scalalib.TestModule.Utest
// support BSP
import mill.bsp._

object NJU1 extends ScalaModule with ScalafmtModule { m =>
  val useChisel5 = true#include "verilated.h"
#include "verilated_vcd_c.h"
#include "obj_dir/VNJU1.h"

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static VNJU1* top;

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}
void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new VNJU1;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("dump.vcd");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

int main() {
  sim_init();

  top->io_s=0; top->io_a=0; top->io_b=0;  step_and_dump_wave();   // 将s，a和b均初始化为“0”
                      top->io_b=1;  step_and_dump_wave();   // 将b改为“1”，s和a的值不变，继续保持“0”，
            top->io_a=1; top->io_b=0;  step_and_dump_wave();   // 将a，b分别改为“1”和“0”，s的值不变，
                      top->io_b=1;  step_and_dump_wave();   // 将b改为“1”，s和a的值不变，维持10个时间单位
  top->io_s=1; top->io_a=0; top->io_b=0;  step_and_dump_wave();   // 将s，a，b分别变为“1,0,0”，维持10个时间单位
                      top->io_b=1;  step_and_dump_wave();
            top->io_a=1; top->io_b=0;  step_and_dump_wave();
                      top->io_b=1;  step_and_dump_wave();

  sim_exit();
}
  override def scalaVersion = "2.13.10"
  override def scalacOptions = Seq(
    "-language:reflectiveCalls",
    "-deprecation",
    "-feature",
    "-Xcheckinit"
  )
  override def ivyDeps = Agg(
    if (useChisel5) ivy"org.chipsalliance::chisel:5.0.0" else
    ivy"edu.berkeley.cs::chisel3:3.6.0",
  )
  override def scalacPluginIvyDeps = Agg(
    if (useChisel5) ivy"org.chipsalliance:::chisel-plugin:5.0.0" else
    ivy"edu.berkeley.cs:::chisel3-plugin:3.6.0",
  )
  object test extends ScalaTests with Utest {
    override def ivyDeps = m.ivyDeps() ++ Agg(
      ivy"com.lihaoyi::utest:0.8.1",
      if (useChisel5) ivy"edu.berkeley.cs::chiseltest:5.0.0" else
      ivy"edu.berkeley.cs::chiseltest:0.6.0",
    )
  }
  def repositoriesTask = T.task { Seq(
    coursier.MavenRepository("https://maven.aliyun.com/repository/central"),
    coursier.MavenRepository("https://repo.scala-sbt.org/scalasbt/maven-releases"),
  ) ++ super.repositoriesTask() }
}
