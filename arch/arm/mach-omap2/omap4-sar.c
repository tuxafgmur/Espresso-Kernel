/*
 * OMAP4 Save Restore source file
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Written by Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/delay.h>

#include <plat/usb.h>

#include <mach/omap4-common.h>
#include <mach/ctrl_module_wkup_44xx.h>

#include "clockdomain.h"
#include "omap4-sar-layout.h"
#include "cm-regbits-44xx.h"
#include "prcm44xx.h"
#include "cminst44xx.h"

/*
 * These SECURE control registers are used to work-around
 * DDR corruption on the second chip select on OMAP443x.
 */
#define OMAP4_CTRL_SECURE_EMIF1_SDRAM_CONFIG2_REG	0x0114
#define OMAP4_CTRL_SECURE_EMIF2_SDRAM_CONFIG2_REG	0x011c

/* OMAP4 modulemode control */
#define OMAP4430_MODULEMODE_SWCTRL			1
#define OMAP4430_USBHOST_CTX_NUM			9

static void __iomem *sar_ram_base;
static void __iomem *omap4_sar_modules[MAX_SAR_MODULES];
static struct powerdomain *l3init_pwrdm;
	static struct clockdomain *l3init_clkdm;
static struct clk *usb_host_ck, *usb_tll_ck;

/*
 * OMAP4430
 * SAR_RAM1 register layout consist of EMIF1, EMIF2, CM1, CM2,
 * CONTROL_CORE efuse, DMM and USB TLL registers.
 * The layout is arranged is a two dimentional array like
 * below,
 * const u32 sar_ramX_layout[nb_regs_sets][4] = {
 *	{module_index, reg_offset, size, sar_ram_offset},
 * }
 */
static const u32 omap443x_sar_ram1_layout[][4] = {
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_CONFIG, 1, 0x00000000},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_CONFIG_2, 1, 0x00000004},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_REF_CTRL, 1, 0x00000008},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_REF_CTRL_SHDW, 1, 0x0000000C},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_1, 1, 0x00000010},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_1_SHDW, 1, 0x00000014},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_2, 1, 0x00000018},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_2_SHDW, 1, 0x0000001C},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_3, 1, 0x00000020},
	{EMIF1_INDEX, OMAP44XX_EMIF_SDRAM_TIM_3_SHDW, 1, 0x00000024},
	{EMIF1_INDEX, OMAP44XX_EMIF_LPDDR2_NVM_TIM, 1, 0x00000028},
	{EMIF1_INDEX, OMAP44XX_EMIF_LPDDR2_NVM_TIM_SHDW, 1, 0x0000002C},
	{EMIF1_INDEX, OMAP44XX_EMIF_PWR_MGMT_CTRL, 1, 0x00000030},
	{EMIF1_INDEX, OMAP44XX_EMIF_PWR_MGMT_CTRL_SHDW, 1, 0x00000034},
	{EMIF1_INDEX, OMAP44XX_EMIF_OCP_CONFIG, 1, 0x00000038},
	{EMIF1_INDEX, OMAP44XX_EMIF_PERF_CNT_CFG, 1, 0x0000003C},
	{EMIF1_INDEX, OMAP44XX_EMIF_PERF_CNT_SEL, 1, 0x00000040},
	{EMIF1_INDEX, OMAP44XX_EMIF_READ_IDLE_CTRL, 1, 0x00000044},
	{EMIF1_INDEX, OMAP44XX_EMIF_READ_IDLE_CTRL_SHDW, 1, 0x00000048},
	{EMIF1_INDEX, OMAP44XX_EMIF_IRQENABLE_SET_SYS, 1, 0x0000004C},
	{EMIF1_INDEX, OMAP44XX_EMIF_IRQENABLE_SET_LL, 1, 0x00000050},
	{EMIF1_INDEX, OMAP44XX_EMIF_ZQ_CONFIG, 1, 0x00000054},
	{EMIF1_INDEX, OMAP44XX_EMIF_TEMP_ALERT_CONFIG, 1, 0x00000058},
	{EMIF1_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_1, 1, 0x0000005C},
	{EMIF1_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_1_SHDW, 1, 0x00000060},
	{EMIF1_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_2, 1, 0x00000064},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_CONFIG, 1, 0x00000068},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_CONFIG_2, 1, 0x0000006C},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_REF_CTRL, 1, 0x00000070},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_REF_CTRL_SHDW, 1, 0x00000074},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_1, 1, 0x00000078},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_1_SHDW, 1, 0x0000007C},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_2, 1, 0x00000080},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_2_SHDW, 1, 0x00000084},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_3, 1, 0x00000088},
	{EMIF2_INDEX, OMAP44XX_EMIF_SDRAM_TIM_3_SHDW, 1, 0x0000008C},
	{EMIF2_INDEX, OMAP44XX_EMIF_LPDDR2_NVM_TIM, 1, 0x00000090},
	{EMIF2_INDEX, OMAP44XX_EMIF_LPDDR2_NVM_TIM_SHDW, 1, 0x00000094},
	{EMIF2_INDEX, OMAP44XX_EMIF_PWR_MGMT_CTRL, 1, 0x00000098},
	{EMIF2_INDEX, OMAP44XX_EMIF_PWR_MGMT_CTRL_SHDW, 1, 0x0000009C},
	{EMIF2_INDEX, OMAP44XX_EMIF_OCP_CONFIG, 1, 0x000000A0},
	{EMIF2_INDEX, OMAP44XX_EMIF_PERF_CNT_CFG, 1, 0x000000A4},
	{EMIF2_INDEX, OMAP44XX_EMIF_PERF_CNT_SEL, 1, 0x000000A8},
	{EMIF2_INDEX, OMAP44XX_EMIF_READ_IDLE_CTRL, 1, 0x000000AC},
	{EMIF2_INDEX, OMAP44XX_EMIF_READ_IDLE_CTRL_SHDW, 1, 0x000000B0},
	{EMIF2_INDEX, OMAP44XX_EMIF_IRQENABLE_SET_SYS, 1, 0x000000B4},
	{EMIF2_INDEX, OMAP44XX_EMIF_IRQENABLE_SET_LL, 1, 0x000000B8},
	{EMIF2_INDEX, OMAP44XX_EMIF_ZQ_CONFIG, 1, 0x000000BC},
	{EMIF2_INDEX, OMAP44XX_EMIF_TEMP_ALERT_CONFIG, 1, 0x000000C0},
	{EMIF2_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_1, 1, 0x000000C4},
	{EMIF2_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_1_SHDW, 1, 0x000000C8},
	{EMIF2_INDEX, OMAP44XX_EMIF_DDR_PHY_CTRL_2, 1, 0x000000CC},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_MEMIF_CLKSTCTRL_RESTORE_OFFSET, 1, 0x000000D0},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_CLKSEL_CORE_RESTORE_OFFSET, 1, 0x000000D4},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M2_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000D8},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M3_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000DC},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M4_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000E0},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M5_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000E4},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M6_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000E8},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DIV_M7_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000EC},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_CLKSEL_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000F0},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
	OMAP4_CM_SSC_DELTAMSTEP_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000F4},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
	OMAP4_CM_SSC_MODFREQDIV_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000F8},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_CLKMODE_DPLL_CORE_RESTORE_OFFSET, 1, 0x000000FC},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_SHADOW_FREQ_CONFIG2_RESTORE_OFFSET, 1, 0x00000100},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_SHADOW_FREQ_CONFIG1_RESTORE_OFFSET, 1, 0x00000104},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST  +
		OMAP4_CM_AUTOIDLE_DPLL_CORE_RESTORE_OFFSET, 1, 0x00000108},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_MPU_CLKSTCTRL_RESTORE_OFFSET, 1, 0x0000010C},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST  +
		OMAP4_CM_CM1_PROFILING_CLKCTRL_RESTORE_OFFSET, 1, 0x00000110},
	{CM1_INDEX, OMAP4430_CM1_RESTORE_INST +
		OMAP4_CM_DYN_DEP_PRESCAL_RESTORE_OFFSET, 1, 0x00000114},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3_1_CLKSTCTRL_RESTORE_OFFSET,	1, 0x00000118},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3_2_CLKSTCTRL_RESTORE_OFFSET, 1, 0x0000011C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L4CFG_CLKSTCTRL_RESTORE_OFFSET, 1, 0x00000120},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_MEMIF_CLKSTCTRL_RESTORE_OFFSET, 1, 0x00000124},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L4PER_CLKSTCTRL_RESTORE_OFFSET, 1, 0x00000128},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3INIT_CLKSTCTRL_RESTORE_OFFSET, 1, 0x0000012C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3INSTR_L3_3_CLKCTRL_RESTORE_OFFSET, 1, 0x00000130},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
	OMAP4_CM_L3INSTR_L3_INSTR_CLKCTRL_RESTORE_OFFSET, 1, 0x00000134},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3INSTR_OCP_WP1_CLKCTRL_RESTORE_OFFSET, 1, 0x00000138},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_CM2_PROFILING_CLKCTRL_RESTORE_OFFSET, 1, 0x0000013C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_D2D_STATICDEP_RESTORE_OFFSET, 1, 0x00000140},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3_1_DYNAMICDEP_RESTORE_OFFSET, 1, 0x00000144},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L3_2_DYNAMICDEP_RESTORE_OFFSET, 1, 0x00000148},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_D2D_DYNAMICDEP_RESTORE_OFFSET, 1, 0x0000014C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L4CFG_DYNAMICDEP_RESTORE_OFFSET, 1, 0x00000150},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_L4PER_DYNAMICDEP_RESTORE_OFFSET, 1, 0x00000154},
	{C2C_INDEX,  0x0C, 1, 0x00000158},
	{C2C_INDEX,  0x10, 1, 0x0000015C},
	{C2C_INDEX,  0x28, 1, 0x00000160},
	{C2C_INDEX,  0x40, 1, 0x00000164},
	{C2C_INDEX,  0x44, 1, 0x00000168},
	{C2C_INDEX,  0x70, 1, 0x0000016C},
	{C2C_INDEX,  0x74, 1, 0x00000170},
	{C2C_INDEX,  0x84, 1, 0x00000174},
	{C2C_INDEX,  0x88, 1, 0x00000178},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_PADCONF_GLOBAL, 15, 0x0000017C},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_PBIASLITE, 14, 0x000001B8},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_LPDDR2IO1_0, 8, 0x000001F0},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_BUS_HOLD, 1, 0x00000210},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_C2C, 1, 0x00000214},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_EFUSE_1, 1, 0x00000218},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_EFUSE_2, 1, 0x0000021C},
	{CTRL_MODULE_PAD_CORE_INDEX,
	 OMAP4_CTRL_MODULE_PAD_CORE_CONTROL_EFUSE_4, 1, 0x00000220},
	{DMM_INDEX, OMAP44XX_DMM_LISA_MAP, 4, 0x000000224},
	{DMM_INDEX, OMAP44XX_DMM_LISA_LOCK, 1, 0x00000234},
	{DMM_INDEX, OMAP44XX_DMM_TILER_OR, 2, 0x00000238},
	{DMM_INDEX, OMAP44XX_DMM_PAT_VIEW, 2, 0x00000240},
	{DMM_INDEX, OMAP44XX_DMM_PAT_VIEW_MAP, 4, 0x00000248},
	{DMM_INDEX, OMAP44XX_DMM_PAT_VIEW_MAP_BASE, 1, 0x00000258},
	{DMM_INDEX, OMAP44XX_DMM_PAT_IRQENABLE_SET, 1, 0x0000025C},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DESCR, 1, 0x00000260},
	{DMM_INDEX, OMAP44XX_DMM_PAT_AREA, 1, 0x00000264},
	{DMM_INDEX, OMAP44XX_DMM_PAT_CTRL, 1, 0x00000268},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DATA, 1, 0x0000026C},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DESCR + 0x10, 1, 0x00000270},
	{DMM_INDEX, OMAP44XX_DMM_PAT_AREA + 0x10, 1, 0x00000274},
	{DMM_INDEX, OMAP44XX_DMM_PAT_CTRL + 0x10, 1, 0x00000278},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DATA + 0x10, 1, 0x0000027C},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DESCR + 0x20, 1, 0x00000280},
	{DMM_INDEX, OMAP44XX_DMM_PAT_AREA + 0x20, 1, 0x00000284},
	{DMM_INDEX, OMAP44XX_DMM_PAT_CTRL + 0x20, 1, 0x00000288},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DATA + 0x20, 1, 0x0000028C},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DESCR + 0x30, 1, 0x00000290},
	{DMM_INDEX, OMAP44XX_DMM_PAT_AREA + 0x30, 1, 0x00000294},
	{DMM_INDEX, OMAP44XX_DMM_PAT_CTRL + 0x30, 1, 0x00000298},
	{DMM_INDEX, OMAP44XX_DMM_PAT_DATA + 0x30, 1, 0x0000029C},
	{DMM_INDEX, OMAP44XX_DMM_PEG_PRIO, 2, 0x000002A0},
	{DMM_INDEX, OMAP44XX_DMM_PEG_PRIO_PAT, 1, 0x000002A8},
	{L3_CLK1_INDEX, 0x508, 1, 0x000002AC},
	{L3_CLK1_INDEX, 0x510, 1, 0x000002B0},
	{L3_CLK1_INDEX, 0x708, 1, 0x000002B4},
	{L3_CLK1_INDEX, 0x70C, 1, 0x000002B8},
	{L3_CLK1_INDEX, 0x808, 1, 0x000002BC},
	{L3_CLK2_INDEX, 0x1008, 1, 0x000002C0},
	{L3_CLK2_INDEX, 0x1010, 1, 0x000002C4},
	{L3_CLK2_INDEX, 0x1208, 1, 0x000002C8},
	{L3_CLK2_INDEX, 0x1308, 1, 0x000002CC},
	{L3_CLK2_INDEX, 0x130C, 1, 0x000002D0},
	{L3_CLK2_INDEX, 0x1408, 1, 0x000002D4},
	{L3_CLK2_INDEX, 0x140C, 1, 0x000002D8},
	{L3_CLK2_INDEX, 0x1508, 1, 0x000002DC},
	{L3_CLK2_INDEX, 0x150C, 1, 0x000002E0},
	{L3_CLK3_INDEX, 0x208, 1, 0x000002E4},
	{L3_CLK3_INDEX, 0x210, 1, 0x000002E8},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_SDMA_STATICDEP_RESTORE_OFFSET, 1, 0x00000924},
	/* Due to errata i719 (Multiple OFF Mode Transitions Introduce
	 * Corruption) the USB host context must only be saved if the USB host
	 * controller has been resumed since the previous OFF mode transition.
	 * Therefore move the USB SAR context descriptors to the end of the
	 * array, following the sDMA context, so we can skip them if we do not
	 * need to save the USB host context.
	 */
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
	OMAP4_CM_L3INIT_USB_HOST_CLKCTRL_RESTORE_OFFSET, 1, 0x000002EC},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
	OMAP4_CM_L3INIT_USB_TLL_CLKCTRL_RESTORE_OFFSET, 1, 0x000002F0},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
		OMAP4_CM_SDMA_STATICDEP_RESTORE_OFFSET, 1, 0x000002F4},
	{USBTLL_INDEX, 0x400, 7, 0x000002F8},
	{UHH_INDEX, 0x10, 1, 0x00000314},
	{UHH_INDEX, 0x40, 1, 0x00000318},
	{UHH_INDEX, 0x100, 384, 0x0000031C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
	OMAP4_CM_L3INIT_USB_HOST_CLKCTRL_RESTORE_OFFSET, 1, 0x0000091C},
	{CM2_INDEX, OMAP4430_CM2_RESTORE_INST +
	OMAP4_CM_L3INIT_USB_TLL_CLKCTRL_RESTORE_OFFSET, 1, 0x00000920},
};

/*
 * SAR_RAM2 register layout consist of SYSCTRL_PADCONF_CORE regsiters
 */
static const u32 omap443x_sar_ram2_layout[][4] = {
	{CTRL_MODULE_PAD_CORE_INDEX, 0x40, 102, 0x00000000},
};

/*
 * OMAP4430
 * SAR_RAM3 and  SAR_RAM4 layout is not listed since moslty it's handle by
 * secure software.
 * We should handle store od this layout on GP devices
 */
static const u32 omap443x_sar_ram3_layout[][4] = {
	{L4CORE_INDEX, 0x2140, 1, 0x00000000},
	{L4CORE_INDEX, 0x2104, 1, 0x00000004},
	{L4CORE_INDEX, 0x2100, 1, 0x00000008},
	{L4CORE_INDEX, 0x2108, 1, 0x0000000C},
	{L4CORE_INDEX, 0x210C, 1, 0x00000010},
	{L4CORE_INDEX, 0x2110, 1, 0x00000014},
	{L4CORE_INDEX, 0x2114, 1, 0x00000018},
	{L4CORE_INDEX, 0x204088, 14, 0x0000001C},
	{L4CORE_INDEX, 0x206088, 2, 0x00000054},
	{L4CORE_INDEX, 0x20C088, 30, 0x0000005C},
	{L4CORE_INDEX, 0x210088, 30, 0x000000D4},
	{L4CORE_INDEX, 0x212088, 38, 0x0000014C},
	{L4CORE_INDEX, 0x214088, 2, 0x000001E4},
	{L4CORE_INDEX, 0x216088, 2, 0x000001EC},
	{L4CORE_INDEX, 0x218088, 2, 0x000001F4},
	{L4CORE_INDEX, 0x21C088, 2, 0x000001FC},
	{L4CORE_INDEX, 0x21E088, 2, 0x00000204},
	{L4CORE_INDEX, 0x220088, 2, 0x0000020C},
	{L4CORE_INDEX, 0x226088, 6, 0x00000214},
	{L4CORE_INDEX, 0x228088, 2, 0x0000022C},
	{L4CORE_INDEX, 0x22A088, 14, 0x00000234},
	{L4PER_INDEX, 0x218, 1, 0x0000026C},
	{L4PER_INDEX, 0x220, 1, 0x00000270},
	{L4PER_INDEX, 0x228, 1, 0x00000274},
	{L4PER_INDEX, 0x230, 1, 0x00000278},
	{L4PER_INDEX, 0x238, 1, 0x0000027C},
	{L4PER_INDEX, 0x298, 2, 0x00000280},
	{L4PER_INDEX, 0x2A0, 2, 0x00000288},
	{L4PER_INDEX, 0x2A8, 2, 0x00000290},
	{L4PER_INDEX, 0x2B0, 2, 0x00000298},
	{L4PER_INDEX, 0x2B8, 2, 0x000002A0},
	{L4PER_INDEX, 0x304, 1, 0x000002A8},
	{L4PER_INDEX, 0x31C, 1, 0x000002AC},
	{L4PER_INDEX, 0x32C, 1, 0x000002B0},
	{L4PER_INDEX, 0x33C, 1, 0x000002B4},
	{L4PER_INDEX, 0x34C, 1, 0x000002B8},
	{L4PER_INDEX, 0x35C, 1, 0x000002BC},
	{L4PER_INDEX, 0x36C, 1, 0x000002C0},
	{L4PER_INDEX, 0x37C, 1, 0x000002C4},
	{L4PER_INDEX, 0x38C, 1, 0x000002C8},
	{L4PER_INDEX, 0x39C, 1, 0x000002CC},
	{L4PER_INDEX, 0x3AC, 1, 0x000002D0},
	{L4PER_INDEX, 0x3BC, 1, 0x000002D4},
	{L4PER_INDEX, 0x3CC, 1, 0x000002D8},
	{L4PER_INDEX, 0x3D4, 1, 0x000002DC},
	{L4PER_INDEX, 0x3E4, 1, 0x000002E0},
	{L4PER_INDEX, 0x3F4, 1, 0x000002E4},
	{L4PER_INDEX, 0x404, 1, 0x000002E8},
	{L4PER_INDEX, 0x414, 1, 0x000002EC},
	{L4PER_INDEX, 0x42C, 1, 0x000002F0},
	{L4PER_INDEX, 0x43C, 1, 0x000002F4},
	{L4PER_INDEX, 0x44C, 1, 0x000002F8},
	{L4PER_INDEX, 0x45C, 1, 0x000002FC},
	{L4PER_INDEX, 0x46C, 1, 0x00000300},
	{L4PER_INDEX, 0x47C, 1, 0x00000304},
	{L4PER_INDEX, 0x48C, 1, 0x00000308},
	{L4PER_INDEX, 0x49C, 1, 0x0000030C},
	{L4PER_INDEX, 0x4AC, 1, 0x00000310},
	{L4PER_INDEX, 0x4BC, 1, 0x00000314},
	{L4PER_INDEX, 0x4CC, 1, 0x00000318},
	{L4PER_INDEX, 0x4DC, 1, 0x0000031C},
	{L4PER_INDEX, 0x4EC, 1, 0x00000320},
	{L4PER_INDEX, 0x4FC, 1, 0x00000324},
	{L4PER_INDEX, 0x50C, 1, 0x00000328},
	{L4PER_INDEX, 0x51C, 1, 0x0000032C},
	{L4PER_INDEX, 0x52C, 1, 0x00000330},
	{L4PER_INDEX, 0x53C, 1, 0x00000334},
	{L4PER_INDEX, 0x54C, 1, 0x00000338},
	{L4PER_INDEX, 0x55C, 1, 0x0000033C},
	{L4PER_INDEX, 0x56C, 1, 0x00000340},
	{L4PER_INDEX, 0x57C, 1, 0x00000344},
	{L4PER_INDEX, 0x5A4, 1, 0x00000348},
	{L4CORE_INDEX, 0x230, 1, 0x0000034C},
	{L4CORE_INDEX, 0x238, 1, 0x00000350},
	{L4CORE_INDEX, 0x2B0, 2, 0x00000354},
	{L4CORE_INDEX, 0x2B8, 2, 0x0000035C},
	{L4CORE_INDEX, 0x304, 1, 0x00000364},
	{L4CORE_INDEX, 0x31C, 1, 0x00000368},
	{L4CORE_INDEX, 0x32C, 1, 0x0000036C},
	{L4CORE_INDEX, 0x33C, 1, 0x00000370},
	{L4CORE_INDEX, 0x354, 1, 0x00000374},
	{L4CORE_INDEX, 0x35C, 1, 0x00000378},
	{L4CORE_INDEX, 0x36C, 1, 0x0000037C},
	{L4CORE_INDEX, 0x37C, 1, 0x00000380},
	{L4CORE_INDEX, 0x38C, 1, 0x00000384},
	{L4CORE_INDEX, 0x3AC, 1, 0x00000388},
	{L4CORE_INDEX, 0x3BC, 1, 0x0000038C},
	{L4CORE_INDEX, 0x3CC, 1, 0x00000390},
	{L4CORE_INDEX, 0x3DC, 1, 0x00000394},
	{L4CORE_INDEX, 0x3EC, 1, 0x00000398},
	{L4CORE_INDEX, 0x3FC, 1, 0x0000039C},
	{L4CORE_INDEX, 0x40C, 1, 0x000003A0},
	{L4CORE_INDEX, 0x41C, 1, 0x000003A4},
	{L4CORE_INDEX, 0x42C, 1, 0x000003A8},
	{L4CORE_INDEX, 0x43C, 1, 0x000003AC},
	{L4CORE_INDEX, 0x44C, 1, 0x000003B0},
	{L4CORE_INDEX, 0x45C, 1, 0x000003B4},
	{L4CORE_INDEX, 0x46C, 1, 0x000003B8},
	{L4CORE_INDEX, 0x47C, 1, 0x000003BC},
	{L4CORE_INDEX, 0x48C, 1, 0x000003C0},
	{L4CORE_INDEX, 0x49C, 1, 0x000003C4},
	{L4CORE_INDEX, 0x4AC, 1, 0x000003C8},
	{L4CORE_INDEX, 0x4BC, 1, 0x000003CC},
	{L4CORE_INDEX, 0x4CC, 1, 0x000003D0},
	{L4CORE_INDEX, 0x4DC, 1, 0x000003D4},
	{L4CORE_INDEX, 0x4EC, 1, 0x000003D8},
	{L4CORE_INDEX, 0x4FC, 1, 0x000003DC},
	{L4CORE_INDEX, 0x50C, 1, 0x000003E0},
	{L4CORE_INDEX, 0x51C, 1, 0x000003E4},
	{L4CORE_INDEX, 0x52C, 1, 0x000003E8},
	{L4CORE_INDEX, 0x53C, 1, 0x000003EC},
	{L4CORE_INDEX, 0x54C, 1, 0x000003F0},
	{L4CORE_INDEX, 0x55C, 1, 0x000003F4},
	{L4CORE_INDEX, 0x56C, 1, 0x000003F8},
	{L4CORE_INDEX, 0x574, 1, 0x000003FC},
	{L4CORE_INDEX, 0x584, 1, 0x00000400},
	{L4CORE_INDEX, 0x594, 1, 0x00000404},
	{L4CORE_INDEX, 0x5A4, 1, 0x00000408},
	{L4CORE_INDEX, 0x5B4, 1, 0x0000040C},
	{L4CORE_INDEX, 0x5C4, 1, 0x00000410},
	{L4CORE_INDEX, 0x5D4, 1, 0x00000414},
	{L4CORE_INDEX, 0x5DC, 1, 0x00000418},
};

/*
 * omap_sar_save :
 * common routine to save the registers to  SAR RAM with the
 * given parameters
 * @nb_regs - Number of registers to saved
 * @sar_bank_offset - where to backup
 * @sar_layout - constant table containing the backup info
 */
static void sar_save(u32 nb_regs, u32 sar_bank, const u32 sar_layout_table[][4])
{
	u32 reg_val, size, i, j;
	void __iomem *reg_read_addr, *sar_wr_addr;

	for (i = 0; i < nb_regs; i++) {
		if (omap4_sar_modules[(sar_layout_table[i][MODULE_ADDR_IDX])]) {
			size = sar_layout_table[i][MODULE_NB_REGS_IDX];
			reg_read_addr =
			    omap4_sar_modules[sar_layout_table[i]
					      [MODULE_ADDR_IDX]]
			    + sar_layout_table[i][MODULE_OFFSET_IDX];
			sar_wr_addr = sar_ram_base + sar_bank +
			    sar_layout_table[i][SAR_RAM_OFFSET_IDX];
			for (j = 0; j < size; j++) {
				reg_val = __raw_readl(reg_read_addr + j * 4);
				__raw_writel(reg_val, sar_wr_addr + j * 4);
			}
		}
	}
}

static void save_sar_bank3(void)
{
	struct clockdomain *l4_secure_clkdm;

	/*
	 * Not supported on ES1.0 silicon
	 */
	if (omap_rev() == OMAP4430_REV_ES1_0) {
		WARN_ONCE(1, "omap4: SAR backup not supported on ES1.0 ..\n");
		return;
	}

	l4_secure_clkdm = clkdm_lookup("l4_secure_clkdm");
	clkdm_wakeup(l4_secure_clkdm);

	sar_save(ARRAY_SIZE(omap443x_sar_ram3_layout), SAR_BANK3_OFFSET,
			omap443x_sar_ram3_layout);

	clkdm_allow_idle(l4_secure_clkdm);
}

static int omap4_sar_not_accessible(void)
{
	u32 usbhost_state, usbtll_state;

	/*
	 * Make sure that USB host and TLL modules are not
	 * enabled before attempting to save the context
	 * registers, otherwise this will trigger an exception.
	 */
	usbhost_state = omap4_cminst_read_inst_reg(OMAP4430_CM2_PARTITION,
						   OMAP4430_CM2_L3INIT_INST,
						   OMAP4_CM_L3INIT_USB_HOST_CLKCTRL_OFFSET)
	    & (OMAP4430_STBYST_MASK | OMAP4430_IDLEST_MASK);

	usbtll_state = omap4_cminst_read_inst_reg(OMAP4430_CM2_PARTITION,
						  OMAP4430_CM2_L3INIT_INST,
						  OMAP4_CM_L3INIT_USB_TLL_CLKCTRL_OFFSET)
	    & OMAP4430_IDLEST_MASK;

	if ((usbhost_state == (OMAP4430_STBYST_MASK | OMAP4430_IDLEST_MASK)) &&
	    (usbtll_state == (OMAP4430_IDLEST_MASK)))
		return 0;
	else
		return -EBUSY;
}

 /*
  * omap4_sar_save -
  * Save the context to SAR_RAM1 and SAR_RAM2 as per
  * omap4xxx_sar_ram1_layout and omap4xxx_sar_ram2_layout for the device OFF
  * mode
  */
int omap4_sar_save(void)
{
	unsigned uhh_save = 1;

	/*
	 * Not supported on ES1.0 silicon
	 */
	if (omap_rev() == OMAP4430_REV_ES1_0) {
		WARN_ONCE(1, "omap4: SAR backup not supported on ES1.0 ..\n");
		return -ENODEV;
	}

	if (omap4_sar_not_accessible()) {
		return -EBUSY;
	}

	/*
	 * SAR bits and clocks needs to be enabled
	 */
	clkdm_wakeup(l3init_clkdm);
	if (uhh_save)
		pwrdm_enable_hdwr_sar(l3init_pwrdm);
	clk_enable(usb_tll_ck);
	clk_enable(usb_host_ck);

	/* Save SAR BANK1 */
	sar_save((ARRAY_SIZE(omap443x_sar_ram1_layout) -
		(uhh_save ? 0 : OMAP4430_USBHOST_CTX_NUM)),
		SAR_BANK1_OFFSET, omap443x_sar_ram1_layout);

	clk_disable(usb_host_ck);
	clk_disable(usb_tll_ck);
	if (uhh_save)
		pwrdm_disable_hdwr_sar(l3init_pwrdm);
	clkdm_allow_idle(l3init_clkdm);

	/* Save SAR BANK2 */
	sar_save(ARRAY_SIZE(omap443x_sar_ram2_layout), SAR_BANK2_OFFSET,
			omap443x_sar_ram2_layout);

	return 0;
}

/**
 * omap4_sar_overwrite :
 * This API overwrite some of the SAR locations as a special cases
 * The register content to be saved can be the register value before
 * going into OFF-mode or a value that is required on wake up. This means
 * that the restored register value can be different from the last value
 * of the register before going into OFF-mode
 *	- CM1 and CM2 configuration
 *		Bits 0 of the CM_SHADOW_FREQ_CONFIG1 regiser and the
 *		CM_SHADOW_FREQ_CONFIG2 register are self-clearing and must
 *		 be set at restore time. Thus, these data must always be
 *		overwritten in the SAR RAM.
 *	- Because USBHOSTHS and USBTLL restore needs a particular
 *		sequencing, the software must overwrite data read from
 *		the following registers implied in phase2a and phase 2b
 */
void omap4_sar_overwrite(void)
{
	u32 val = 0;
	u32 offset = 0;

	/* Overwriting Phase1 data to be restored */
	/* CM2 MEMIF_CLKTRCTRL = SW_WKUP, before FREQ UPDATE */
	__raw_writel(0x2, sar_ram_base + SAR_BANK1_OFFSET + 0xd0);
	/* CM1 CM_SHADOW_FREQ_CONFIG2, Enable FREQ UPDATE */
	val = __raw_readl(OMAP4430_CM_SHADOW_FREQ_CONFIG2);
	/*
	 * FIXME: Implement FREQ UPDATE for L#/M5 before enabling this
	 * val |= 1 << OMAP4430_FREQ_UPDATE_SHIFT;
	 */
	__raw_writel(val, sar_ram_base + SAR_BANK1_OFFSET + 0x100);
	/* CM1 CM_SHADOW_FREQ_CONFIG1, Enable FREQ UPDATE */
	val = __raw_readl(OMAP4430_CM_SHADOW_FREQ_CONFIG1);
	val |= 1 << OMAP4430_FREQ_UPDATE_SHIFT;
	val &= ~OMAP4430_DLL_OVERRIDE_MASK;
	__raw_writel(val, sar_ram_base + SAR_BANK1_OFFSET + 0x104);
	/* CM2 MEMIF_CLKTRCTRL = HW_AUTO, after FREQ UPDATE */
	__raw_writel(0x3, sar_ram_base + SAR_BANK1_OFFSET + 0x124);

	/* Overwriting Phase2a data to be restored */
	/* CM_L3INIT_USB_HOST_CLKCTRL: SAR_MODE = 1, MODULEMODE = 2 */
	__raw_writel(0x00000012,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x2ec + offset);
	/* CM_L3INIT_USB_TLL_CLKCTRL: SAR_MODE = 1, MODULEMODE = 1 */
	__raw_writel(0x00000011,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x2f0 + offset);
	/* CM2 CM_SDMA_STATICDEP : Enable static depedency for SAR modules */
	__raw_writel(0x000090e8,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x2f4 + offset);

	/* Overwriting Phase2b data to be restored */
	/* CM_L3INIT_USB_HOST_CLKCTRL: SAR_MODE = 0, MODULEMODE = 0 */
	val = __raw_readl(OMAP4430_CM_L3INIT_USB_HOST_CLKCTRL);
	val &= (OMAP4430_CLKSEL_UTMI_P1_MASK | OMAP4430_CLKSEL_UTMI_P2_MASK);
	__raw_writel(val, sar_ram_base + SAR_BANK1_OFFSET + 0x91c + offset);
	/* CM_L3INIT_USB_TLL_CLKCTRL: SAR_MODE = 0, MODULEMODE = 0 */
	__raw_writel(0x0000000,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x920 + offset);
	/* CM2 CM_SDMA_STATICDEP : Clear the static depedency */
	__raw_writel(0x00000040,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x924 + offset);

	/* XXX: WA: The wrong value of
	 *   CM_L3INIT_CLKSTCTRL.CLKTRCTRL = 2 (SW_WKUP) has been stored
	 *   in SAR during omap4_sar_save() execution and causes power
	 *   over consumption on OFF mode due to C2C operation.
	 *   It happens because the L3INIT CD need to be enabled
	 *   during omap4_sar_save() execution for proper USBHOST
	 *   module context storing.
	 *   As WA overwrite
	 *   CM_L3INIT_CLKSTCTRL.CLKTRCTRL = 0x1 (SW_SLEEP). */
	__raw_writel(0x00000001,
		     sar_ram_base + SAR_BANK1_OFFSET + 0x0000012C + offset);

	/* readback to ensure data reaches to SAR RAM */
	barrier();
	val = __raw_readl(sar_ram_base + SAR_BANK1_OFFSET + 0x924 + offset);
}

void __iomem *omap4_get_sar_ram_base(void)
{
	return sar_ram_base;
}

/*
 * SAR RAM used to save and restore the HW
 * context in low power modes
 */
static int __init omap4_sar_ram_init(void)
{
	/*
	 * To avoid code running on other OMAPs in
	 * multi-omap builds
	 */
	if (!cpu_is_omap44xx())
		return -ENODEV;

	/*
	 * Static mapping, never released Actual SAR area used is 8K it's
	 * spaced over 16K address with some part is reserved.
	 */
	sar_ram_base = ioremap(OMAP44XX_SAR_RAM_BASE, SZ_16K);
	BUG_ON(!sar_ram_base);

	/*
	 * All these are static mappings so ioremap() will
	 * just return with mapped VA
	 */
	omap4_sar_modules[EMIF1_INDEX] = ioremap(OMAP44XX_EMIF1, SZ_1M);
	BUG_ON(!omap4_sar_modules[EMIF1_INDEX]);
	omap4_sar_modules[EMIF2_INDEX] = ioremap(OMAP44XX_EMIF2, SZ_1M);
	BUG_ON(!omap4_sar_modules[EMIF2_INDEX]);
	omap4_sar_modules[DMM_INDEX] = ioremap(OMAP44XX_DMM_BASE, SZ_1M);
	BUG_ON(!omap4_sar_modules[DMM_INDEX]);
	omap4_sar_modules[CM1_INDEX] = ioremap(OMAP4430_CM1_BASE, SZ_8K);
	BUG_ON(!omap4_sar_modules[CM1_INDEX]);
	omap4_sar_modules[CM2_INDEX] = ioremap(OMAP4430_CM2_BASE, SZ_8K);
	BUG_ON(!omap4_sar_modules[CM2_INDEX]);
	omap4_sar_modules[C2C_INDEX] = ioremap(OMAP44XX_C2C_BASE, SZ_1M);
	BUG_ON(!omap4_sar_modules[C2C_INDEX]);
	omap4_sar_modules[CTRL_MODULE_PAD_CORE_INDEX] =
	    ioremap(OMAP443X_CTRL_BASE, SZ_4K);
	BUG_ON(!omap4_sar_modules[CTRL_MODULE_PAD_CORE_INDEX]);
	omap4_sar_modules[L3_CLK1_INDEX] = ioremap(L3_44XX_BASE_CLK1, SZ_1M);
	BUG_ON(!omap4_sar_modules[L3_CLK1_INDEX]);
	omap4_sar_modules[L3_CLK2_INDEX] = ioremap(L3_44XX_BASE_CLK2, SZ_1M);
	BUG_ON(!omap4_sar_modules[L3_CLK2_INDEX]);
	omap4_sar_modules[L3_CLK3_INDEX] = ioremap(L3_44XX_BASE_CLK3, SZ_1M);
	BUG_ON(!omap4_sar_modules[L3_CLK3_INDEX]);
	omap4_sar_modules[USBTLL_INDEX] = ioremap(OMAP44XX_USBTLL_BASE, SZ_1M);
	BUG_ON(!omap4_sar_modules[USBTLL_INDEX]);
	omap4_sar_modules[UHH_INDEX] = ioremap(OMAP44XX_UHH_CONFIG_BASE, SZ_1M);
	BUG_ON(!omap4_sar_modules[UHH_INDEX]);
	omap4_sar_modules[L4CORE_INDEX] = ioremap(L4_44XX_PHYS, SZ_4M);
	BUG_ON(!omap4_sar_modules[L4CORE_INDEX]);
	omap4_sar_modules[L4PER_INDEX] = ioremap(L4_PER_44XX_PHYS, SZ_4M);
	BUG_ON(!omap4_sar_modules[L4PER_INDEX]);

	/*
	 * SAR BANK3 contains all firewall settings and it's saved through
	 * secure API on HS device. On GP device these registers are
	 * meaningless but still needs to be saved. Otherwise Auto-restore
	 * phase DMA takes an abort. Hence save these conents only once
	 * in init to avoid the issue while waking up from device OFF
	 */
	if (omap_type() == OMAP2_DEVICE_TYPE_GP)
		save_sar_bank3();
	/*
	 * Work around for OMAP443x Errata i632: "LPDDR2 Corruption After OFF
	 * Mode Transition When CS1 Is Used On EMIF":
	 * Overwrite EMIF1/EMIF2
	 * SECURE_EMIF1_SDRAM_CONFIG2_REG
	 * SECURE_EMIF2_SDRAM_CONFIG2_REG
	 */
	if (cpu_is_omap443x()) {
		void __iomem *secure_ctrl_mod;

		secure_ctrl_mod = ioremap(OMAP4_CTRL_MODULE_WKUP, SZ_4K);
		BUG_ON(!secure_ctrl_mod);

		__raw_writel(0x10, secure_ctrl_mod +
				OMAP4_CTRL_SECURE_EMIF1_SDRAM_CONFIG2_REG);
		__raw_writel(0x10, secure_ctrl_mod +
				OMAP4_CTRL_SECURE_EMIF2_SDRAM_CONFIG2_REG);
		wmb();
		iounmap(secure_ctrl_mod);
	}

	/*
	 * L3INIT PD and clocks are needed for SAR save phase
	 */
	l3init_pwrdm = pwrdm_lookup("l3init_pwrdm");
	if (!l3init_pwrdm)
		pr_err("Failed to get l3init_pwrdm\n");

	l3init_clkdm = clkdm_lookup("l3_init_clkdm");
	if (!l3init_clkdm)
		pr_err("Failed to get l3_init_clkdm\n");

	usb_host_ck = clk_get(NULL, "usb_host_hs_fck");
	if (!usb_host_ck)
		pr_err("Could not get usb_host_ck\n");

	usb_tll_ck = clk_get(NULL, "usb_tll_hs_ick");
	if (!usb_tll_ck)
		pr_err("Could not get usb_tll_ck\n");

	return 0;
}
early_initcall(omap4_sar_ram_init);
