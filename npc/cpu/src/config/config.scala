package config

import chisel3._ 
import math._

object MyConfig {
    val DATA_WIDTH  = 32
    val ADDR_WIDTH  = 32
    val INST_WIDTH  = 32
    val START_ADDR  = 0x80000000L
    val REG_NUM     = 32
    val REG_WIDTH   = 5
}