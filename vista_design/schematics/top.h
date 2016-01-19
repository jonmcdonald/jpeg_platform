#pragma once
#include "mgc_vista_schematics.h"
$includes_begin;
#include <systemc.h>
#include "../models/cpu_subsystem_model.h"
#include "../models/sram_model.h"
#include "../models/axi_bus_model.h"
#include "../models/axi2apb_bridge_model.h"
#include "../models/apb_bus_model.h"
#include "../models/RST_CLK_CTRL_model.h"
#include "../models/H264_DECODER_model.h"
#include "../models/pl110_model.h"
#include "../models/smc91x_model.h"
#include "JPEG_SUBSYSTEM.h"
#include "../models/IO_model.h"
#include "UART_SUBSYSTEM.h"
$includes_end;

$module_begin("top");
SC_MODULE(top) {
public:
  top(::sc_core::sc_module_name name):
    ::sc_core::sc_module(name)
    $initialization_begin
$init("AXI_BUS"),
AXI_BUS(0)
$end
$init("sram"),
sram(0)
$end
$init("CPU_Cortex_A9"),
CPU_Cortex_A9(0)
$end
$init("APB_BUS"),
APB_BUS(0)
$end
$init("h264_inst"),
h264_inst(0)
$end
$init("lcd"),
lcd(0)
$end
$init("JPEG_SUBSYSTEM0"),
JPEG_SUBSYSTEM0(0)
$end
$init("NetworkCard"),
NetworkCard(0)
$end
$init("bitmap_io"),
bitmap_io(0)
$end
$init("axi2apb_bridge"),
axi2apb_bridge(0)
$end
$init("UART_SYS"),
UART_SYS(0)
$end
    $initialization_end
{
    $elaboration_begin;
$create_component("AXI_BUS");
AXI_BUS = new axi_bus_pvt("AXI_BUS");
$end;
$create_component("sram");
sram = new sram_pvt("sram");
$end;
$create_component("CPU_Cortex_A9");
CPU_Cortex_A9 = new cpu_subsystem_pvt("CPU_Cortex_A9");
$end;
$create_component("APB_BUS");
APB_BUS = new apb_bus_pvt("APB_BUS");
$end;
$create_component("h264_inst");
h264_inst = new H264_DECODER_pvt("h264_inst");
$end;
$create_component("lcd");
lcd = new pl110_pvt("lcd");
$end;
$create_component("JPEG_SUBSYSTEM0");
JPEG_SUBSYSTEM0 = new JPEG_SUBSYSTEM("JPEG_SUBSYSTEM0");
$end;
$create_component("NetworkCard");
NetworkCard = new smc91x_pvt("NetworkCard");
$end;
$create_component("bitmap_io");
bitmap_io = new IO_pvt("bitmap_io");
$end;
$create_component("axi2apb_bridge");
axi2apb_bridge = new axi2apb_bridge_pvt("axi2apb_bridge");
$end;
$create_component("UART_SYS");
UART_SYS = new UART_SUBSYSTEM("UART_SYS");
$end;
$bind("AXI_BUS->jpeg_master","JPEG_SUBSYSTEM0->jpeg_slave");
vista_bind(AXI_BUS->jpeg_master, JPEG_SUBSYSTEM0->jpeg_slave);
$end;
$bind("lcd->master","AXI_BUS->lcd_slave");
vista_bind(lcd->master, AXI_BUS->lcd_slave);
$end;
$bind("h264_inst->master","AXI_BUS->h264_slave");
vista_bind(h264_inst->master, AXI_BUS->h264_slave);
$end;
$bind("JPEG_SUBSYSTEM0->jpeg_master","AXI_BUS->jpeg_slave");
vista_bind(JPEG_SUBSYSTEM0->jpeg_master, AXI_BUS->jpeg_slave);
$end;
$bind("CPU_Cortex_A9->master0","AXI_BUS->bus_slave0");
vista_bind(CPU_Cortex_A9->master0, AXI_BUS->bus_slave0);
$end;
$bind("AXI_BUS->lcd_master","lcd->ctrl_slave");
vista_bind(AXI_BUS->lcd_master, lcd->ctrl_slave);
$end;
$bind("AXI_BUS->h264_master","h264_inst->slave");
vista_bind(AXI_BUS->h264_master, h264_inst->slave);
$end;
$bind("CPU_Cortex_A9->master1","AXI_BUS->bus_slave1");
vista_bind(CPU_Cortex_A9->master1, AXI_BUS->bus_slave1);
$end;
$bind("JPEG_SUBSYSTEM0->irq","CPU_Cortex_A9->irq_2");
vista_bind(JPEG_SUBSYSTEM0->irq, CPU_Cortex_A9->irq_2);
$end;
$bind("NetworkCard->irq","CPU_Cortex_A9->irq_3");
vista_bind(NetworkCard->irq, CPU_Cortex_A9->irq_3);
$end;
$bind("AXI_BUS->eth_slave","NetworkCard->host");
vista_bind(AXI_BUS->eth_slave, NetworkCard->host);
$end;
$bind("bitmap_io->master","AXI_BUS->io_slave");
vista_bind(bitmap_io->master, AXI_BUS->io_slave);
$end;
$bind("AXI_BUS->io_master","bitmap_io->slave");
vista_bind(AXI_BUS->io_master, bitmap_io->slave);
$end;
$bind("axi2apb_bridge->apb_master","APB_BUS->bus_slave");
vista_bind(axi2apb_bridge->apb_master, APB_BUS->bus_slave);
$end;
$bind("APB_BUS->uart_rst_ctrl","UART_SYS->uart_ctrl");
vista_bind(APB_BUS->uart_rst_ctrl, UART_SYS->uart_ctrl);
$end;
$bind("APB_BUS->uart","UART_SYS->uart_regs");
vista_bind(APB_BUS->uart, UART_SYS->uart_regs);
$end;
$bind("UART_SYS->UARTINTR","CPU_Cortex_A9->irq_0");
vista_bind(UART_SYS->UARTINTR, CPU_Cortex_A9->irq_0);
$end;
$bind("AXI_BUS->apb_master","axi2apb_bridge->axi_slave");
vista_bind(AXI_BUS->apb_master, axi2apb_bridge->axi_slave);
$end;
$bind("AXI_BUS->sram_master","sram->slave");
vista_bind(AXI_BUS->sram_master, sram->slave);
$end;
    $elaboration_end;
  $vlnv_assign_begin;
m_library = "Schematics";
m_vendor = "";
m_version = "";
  $vlnv_assign_end;
  }
  ~top() {
    $destructor_begin;
$destruct_component("AXI_BUS");
delete AXI_BUS; AXI_BUS = 0;
$end;
$destruct_component("sram");
delete sram; sram = 0;
$end;
$destruct_component("CPU_Cortex_A9");
delete CPU_Cortex_A9; CPU_Cortex_A9 = 0;
$end;
$destruct_component("APB_BUS");
delete APB_BUS; APB_BUS = 0;
$end;
$destruct_component("h264_inst");
delete h264_inst; h264_inst = 0;
$end;
$destruct_component("lcd");
delete lcd; lcd = 0;
$end;
$destruct_component("JPEG_SUBSYSTEM0");
delete JPEG_SUBSYSTEM0; JPEG_SUBSYSTEM0 = 0;
$end;
$destruct_component("NetworkCard");
delete NetworkCard; NetworkCard = 0;
$end;
$destruct_component("bitmap_io");
delete bitmap_io; bitmap_io = 0;
$end;
$destruct_component("axi2apb_bridge");
delete axi2apb_bridge; axi2apb_bridge = 0;
$end;
$destruct_component("UART_SYS");
delete UART_SYS; UART_SYS = 0;
$end;
    $destructor_end;
  }
public:
  $fields_begin;
$component("AXI_BUS");
axi_bus_pvt *AXI_BUS;
$end;
$component("sram");
sram_pvt *sram;
$end;
$component("CPU_Cortex_A9");
cpu_subsystem_pvt *CPU_Cortex_A9;
$end;
$component("APB_BUS");
apb_bus_pvt *APB_BUS;
$end;
$component("h264_inst");
H264_DECODER_pvt *h264_inst;
$end;
$component("lcd");
pl110_pvt *lcd;
$end;
$component("JPEG_SUBSYSTEM0");
JPEG_SUBSYSTEM *JPEG_SUBSYSTEM0;
$end;
$component("NetworkCard");
smc91x_pvt *NetworkCard;
$end;
$component("bitmap_io");
IO_pvt *bitmap_io;
$end;
$component("axi2apb_bridge");
axi2apb_bridge_pvt *axi2apb_bridge;
$end;
$component("UART_SYS");
UART_SUBSYSTEM *UART_SYS;
$end;
  $fields_end;
  $vlnv_decl_begin;
public:
const char* m_library;
const char* m_vendor;
const char* m_version;
  $vlnv_decl_end;
};
$module_end;