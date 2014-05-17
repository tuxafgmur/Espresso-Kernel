/*
 * OMAP4 OPP table definitions.
 *
 * Copyright (C) 2010-2011 Texas Instruments Incorporated - http://www.ti.com/
 *	Nishanth Menon
 *	Kevin Hilman
 *	Thara Gopinath
 * Copyright (C) 2010-2011 Nokia Corporation.
 *      Eduardo Valentin
 *      Paul Walmsley
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/opp.h>
#include <linux/clk.h>

#include <plat/cpu.h>
#include <plat/common.h>

#include "control.h"
#include "omap_opp_data.h"
#include "pm.h"

/**
 * struct omap4_ldo_abb_trim_data - describe ABB trim bits for specific voltage
 * @volt_data:		voltage table
 * @volt_nominal:	voltage for which ABB type should be modified
 *			according to trim bits.
 * @rbb_trim_mask:	If this bit is set in trim register, ABB type
 *			should be modified to RBB.
 * @fbb_trim_mask:	If this bit is set in trim register, ABB type
 *			should be modified to FBB.
 */
struct omap4_ldo_abb_trim_data {
	struct omap_volt_data *volt_data;
	u32 volt_nominal;
	u32 rbb_trim_mask;
	u32 fbb_trim_mask;
};

/*
 * Structures containing OMAP4430 voltage supported
 *
 * Tuxafgmur: this seems, the minimun values we can use
 */
# ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10

#define OMAP4430_VDD_MPU_OPP50_UV            925000		/*  300 */
#define OMAP4430_VDD_MPU_OPP100_UV          1100000		/*  600 */
#define OMAP4430_VDD_MPU_OPPTURBO_UV        1215000		/*  800 */
#define OMAP4430_VDD_MPU_OPPNITRO_UV        1275000		/* 1080 */

#ifdef CONFIG_OMAP4430_TOP_CPU
#define OMAP4430_VDD_MPU_OPPNITRO2_UV       1305000		/* 1216 */
#define OMAP4430_VDD_MPU_OPPNITROSB_UV      1345000		/* 1360 */
#define OMAP4430_VDD_MPU_OPPNITROSB2_UV     1395000		/* 1420 */
#define OMAP4430_VDD_MPU_OPPSUPERSB_UV      1425000		/* 1460 */
#else
#define OMAP4430_VDD_MPU_OPPNITROSB_UV      1305000
#endif

#else   /* CONFIG_MACH_SAMSUNG_ESPRESSO_10 */

#define OMAP4430_VDD_MPU_OPP50_UV            925000		/*  300 */
#define OMAP4430_VDD_MPU_OPP100_UV          1100000		/*  600 */
#define OMAP4430_VDD_MPU_OPPTURBO_UV        1213000		/*  800 */
#define OMAP4430_VDD_MPU_OPPNITRO_UV        1274000		/* 1080 */

#ifdef CONFIG_OMAP4430_TOP_CPU
#define OMAP4430_VDD_MPU_OPPNITRO2_UV       1325000		/* 1200 */
#define OMAP4430_VDD_MPU_OPPNITROSB_UV      1360000		/* 1350 */
#define OMAP4430_VDD_MPU_OPPNITROSB2_UV     1375000		/* 1420 */
#define OMAP4430_VDD_MPU_OPPSUPERSB_UV      1410000		/* 1480 */
#else
#define OMAP4430_VDD_MPU_OPPNITROSB_UV      1325000
#endif

#endif /* CONFIG_MACH_SAMSUNG_ESPRESSO_10 */

struct omap_volt_data omap443x_vdd_mpu_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPP50_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPP50,
			0xf4, 0x0c, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPP100_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPP100,
			0xf9, 0x16, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPTURBO_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPTURBO,
			0xfa, 0x23, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITRO_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPNITRO,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
#ifdef CONFIG_OMAP4430_TOP_CPU
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITRO2_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPNITRO,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITROSB_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPNITROSB,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITROSB2_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPNITROSB,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPSUPERSB_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPSUPERSB,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
#else
	VOLT_DATA_DEFINE(OMAP4430_VDD_MPU_OPPNITROSB_UV, 0,
			OMAP44XX_CONTROL_FUSE_MPU_OPPNITROSB,
			0xfa, 0x27, OMAP_ABB_FAST_OPP),
#endif
	VOLT_DATA_DEFINE(0, 0, 0, 0, 0, 0),
};

/*
 * tuxafgmur: this seems, the minimun values we can use
 */
# ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10

#define OMAP4430_VDD_IVA_OPP50_UV		990000		/* 1013000 */
#define OMAP4430_VDD_IVA_OPP100_UV		1165000		/* 1188000 */
#define OMAP4430_VDD_IVA_OPPTURBO_UV		1275000		/* 1300000 */

#else   /* CONFIG_MACH_SAMSUNG_ESPRESSO_10 */

#define OMAP4430_VDD_IVA_OPP50_UV		988000 		/* 1013000 */
#define OMAP4430_VDD_IVA_OPP100_UV		1163000 	/* 1188000 */
#define OMAP4430_VDD_IVA_OPPTURBO_UV		1275000 	/* 1300000 */

#endif

struct omap_volt_data omap443x_vdd_iva_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPP50_UV, 0,
			OMAP44XX_CONTROL_FUSE_IVA_OPP50,
			0xf4, 0x0c, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPP100_UV, 0,
			OMAP44XX_CONTROL_FUSE_IVA_OPP100,
			0xf9, 0x16, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(OMAP4430_VDD_IVA_OPPTURBO_UV, 0,
			OMAP44XX_CONTROL_FUSE_IVA_OPPTURBO,
			0xfa, 0x23, OMAP_ABB_NOMINAL_OPP),
	VOLT_DATA_DEFINE(0, 0, 0, 0, 0, 0),
};

/*
 * Tuxafgmur: this seems, the minimun values we can use
 */
# ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10
#define OMAP4430_VDD_CORE_OPP50_UV		1000000		/* 1025000 */
#define OMAP4430_VDD_CORE_OPP100_UV		1150000		/* 1200000 */
#if defined(CONFIG_OMAP4430_TOP_CPU) || defined(CONFIG_OMAP4430_TOP_GPU)
#define OMAP4430_VDD_CORE_OPP100_OV_UV		1225000		/* 1250000 */
#endif
#else   /*#CONFIG_MACH_SAMSUNG_ESPRESSO */
#define OMAP4430_VDD_CORE_OPP50_UV		1000000 	/* 1025000 */
#define OMAP4430_VDD_CORE_OPP100_UV		1175000 	/* 1200000 */
#if defined(CONFIG_OMAP4430_TOP_CPU) || defined(CONFIG_OMAP4430_TOP_GPU)
#define OMAP4430_VDD_CORE_OPP100_OV_UV		1225000 	/* 1250000 */
#endif
#endif

struct omap_volt_data omap443x_vdd_core_volt_data[] = {
	VOLT_DATA_DEFINE(OMAP4430_VDD_CORE_OPP50_UV, 0,
			OMAP44XX_CONTROL_FUSE_CORE_OPP50,
			0xf4, 0x0c, OMAP_ABB_NONE),
	VOLT_DATA_DEFINE(OMAP4430_VDD_CORE_OPP100_UV, 0,
			OMAP44XX_CONTROL_FUSE_CORE_OPP100,
			0xf9, 0x16, OMAP_ABB_NONE),
#if defined(CONFIG_OMAP4430_TOP_CPU) || defined(CONFIG_OMAP4430_TOP_GPU)
	VOLT_DATA_DEFINE(OMAP4430_VDD_CORE_OPP100_OV_UV, 0,
			OMAP44XX_CONTROL_FUSE_CORE_OPP100OV,
			0xf9, 0x16, OMAP_ABB_NONE),
#endif
	VOLT_DATA_DEFINE(0, 0, 0, 0, 0, 0),
};

/* Dependency of domains are as follows for OMAP4430 (OPP based):
 *
 *	 MPU	IVA	   CORE
 *	 50	 50	     50+
 *	 50	100+        100
 *	100+	 50	    100
 *	100+	100+	    100
 *
 * OMAP 4430 MPU Core VDD dependency table
 */
static struct omap_vdd_dep_volt omap443x_vdd_mpu_core_dep_data[] = {
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPP50_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP50_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPP100_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPTURBO_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPNITRO_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
#ifdef CONFIG_OMAP4430_TOP_CPU
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPNITRO2_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPNITROSB_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPNITROSB2_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPSUPERSB_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_OV_UV},
#else
	{.main_vdd_volt = OMAP4430_VDD_MPU_OPPNITROSB_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
#endif /* CONFIG_OMAP4430_TOP_CPU */
};

struct omap_vdd_dep_info omap443x_vddmpu_dep_info[] = {
	{
		.name	= "core",
		.dep_table = omap443x_vdd_mpu_core_dep_data,
		.nr_dep_entries = ARRAY_SIZE(omap443x_vdd_mpu_core_dep_data),
	},
	{.name = NULL, .dep_table = NULL, .nr_dep_entries = 0},
};

/* OMAP 4430 MPU IVA VDD dependency table */
static struct omap_vdd_dep_volt omap443x_vdd_iva_core_dep_data[] = {
	{.main_vdd_volt = OMAP4430_VDD_IVA_OPP50_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP50_UV},
	{.main_vdd_volt = OMAP4430_VDD_IVA_OPP100_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
	{.main_vdd_volt = OMAP4430_VDD_IVA_OPPTURBO_UV,
			.dep_vdd_volt = OMAP4430_VDD_CORE_OPP100_UV},
};

struct omap_vdd_dep_info omap443x_vddiva_dep_info[] = {
	{
		.name	= "core",
		.dep_table = omap443x_vdd_iva_core_dep_data,
		.nr_dep_entries = ARRAY_SIZE(omap443x_vdd_iva_core_dep_data),
	},
	{.name = NULL, .dep_table = NULL, .nr_dep_entries = 0},
};

static struct omap_opp_def __initdata omap443x_opp_def_list[] = {
	/* MPU OPP1 - OPP50 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", true,
			300000000, OMAP4430_VDD_MPU_OPP50_UV),
	/* MPU OPP2 - OPP100 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", true,
			600000000, OMAP4430_VDD_MPU_OPP100_UV),
	/* MPU OPP3 - OPP-Turbo */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", true,
			800000000, OMAP4430_VDD_MPU_OPPTURBO_UV),
	/* MPU OPP4 - OPP-SB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", true,
			1008000000, OMAP4430_VDD_MPU_OPPNITRO_UV),
#ifdef CONFIG_OMAP4430_TOP_CPU

#ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10
	/* MPU OPP4 - OPP-NITRO2 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1216000000, OMAP4430_VDD_MPU_OPPNITRO2_UV),
	/* MPU OPP5 - OPP-NITROSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1360000000, OMAP4430_VDD_MPU_OPPNITROSB_UV),
	/* MPU OPP6 - OPP-NITROSB2 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1420000000, OMAP4430_VDD_MPU_OPPNITROSB2_UV),
	/* MPU OPP7 - OPP-SUPERSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1460000000, OMAP4430_VDD_MPU_OPPSUPERSB_UV),
#else   /* CONFIG_MACH_SAMSUNG_ESPRESSO_10 */
	/* MPU OPP4 - OPP-NITRO2 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1200000000, OMAP4430_VDD_MPU_OPPNITRO2_UV),
	/* MPU OPP5 - OPP-NITROSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1350000000, OMAP4430_VDD_MPU_OPPNITROSB_UV),
	/* MPU OPP6 - OPP-NITROSB2 */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1420000000, OMAP4430_VDD_MPU_OPPNITROSB2_UV),
	/* MPU OPP7 - OPP-SUPERSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1480000000, OMAP4430_VDD_MPU_OPPSUPERSB_UV),
#endif

#else   /*CONFIG_OMAP4430_TOP_CPU */

#ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10
	/* MPU OPP4 - OPP-NITROSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1216000000, OMAP4430_VDD_MPU_OPPNITROSB_UV),
#else   /* CONFIG_MACH_SAMSUNG_ESPRESSO_10 */
	/* MPU OPP4 - OPP-NITROSB */
	OPP_INITIALIZER("mpu", "dpll_mpu_ck", "mpu", false,
			1200000000, OMAP4430_VDD_MPU_OPPNITROSB_UV),
#endif

#endif  /*CONFIG_OMAP4430_TOP_CPU */
	/* L3 OPP1 - OPP50 */
	OPP_INITIALIZER("l3_main_1", "virt_l3_ck", "core", true,
			100000000, OMAP4430_VDD_CORE_OPP50_UV),
	/* L3 OPP2 - OPP100, OPP-Turbo, OPP-SB */
	OPP_INITIALIZER("l3_main_1", "virt_l3_ck", "core", true,
			200000000, OMAP4430_VDD_CORE_OPP100_UV),
	/* IVA OPP1 - OPP50 */
	OPP_INITIALIZER("iva", "virt_iva_ck", "iva", true,
			133030000, OMAP4430_VDD_IVA_OPP50_UV),
	/* IVA OPP2 - OPP100 */
	OPP_INITIALIZER("iva", "virt_iva_ck", "iva", true,
			266100000, OMAP4430_VDD_IVA_OPP100_UV),
	/* IVA OPP3 - OPP-Turbo */
	OPP_INITIALIZER("iva", "virt_iva_ck", "iva", true,
			332000000, OMAP4430_VDD_IVA_OPPTURBO_UV),
	/* SGX OPP1 - OPP50 */
	OPP_INITIALIZER("gpu", "dpll_per_m7x2_ck", "core", true,
			153600000, OMAP4430_VDD_CORE_OPP50_UV),
	/* SGX OPP2 - OPP100 */
	OPP_INITIALIZER("gpu", "dpll_per_m7x2_ck", "core", true,
			307200000, OMAP4430_VDD_CORE_OPP100_UV),
#ifdef CONFIG_OMAP4430_TOP_GPU
	OPP_INITIALIZER("gpu", "dpll_per_m7x2_ck", "core", true,
			384000000, OMAP4430_VDD_CORE_OPP100_UV),
	OPP_INITIALIZER("gpu", "dpll_per_m7x2_ck", "core", false,
			512000000, OMAP4430_VDD_CORE_OPP100_OV_UV),
#endif
	/* FDIF OPP1 - OPP25 */
	OPP_INITIALIZER("fdif", "fdif_fck", "core", true,
			32000000, OMAP4430_VDD_CORE_OPP50_UV),
	/* FDIF OPP2 - OPP50 */
	OPP_INITIALIZER("fdif", "fdif_fck", "core", true,
			64000000, OMAP4430_VDD_CORE_OPP50_UV),
	/* FDIF OPP3 - OPP100 */
	OPP_INITIALIZER("fdif", "fdif_fck", "core", true,
			128000000, OMAP4430_VDD_CORE_OPP100_UV),
	/* DSP OPP1 - OPP50 */
	OPP_INITIALIZER("dsp_c0", "virt_dsp_ck", "iva", true,
			232750000, OMAP4430_VDD_IVA_OPP50_UV),
	/* DSP OPP2 - OPP100 */
	OPP_INITIALIZER("dsp_c0", "virt_dsp_ck", "iva", true,
			465500000, OMAP4430_VDD_IVA_OPP100_UV),
	/* DSP OPP3 - OPPTB */
	OPP_INITIALIZER("dsp_c0", "virt_dsp_ck", "iva", true,
			496000000, OMAP4430_VDD_IVA_OPPTURBO_UV),
	/* HSI OPP1 - OPP50 */
	OPP_INITIALIZER("hsi", "hsi_fck", "core", true,
			96000000, OMAP4430_VDD_CORE_OPP50_UV),
	/* HSI OPP2 - OPP100 */
	OPP_INITIALIZER("hsi", "hsi_fck", "core", true,
			192000000, OMAP4430_VDD_CORE_OPP100_UV),
#ifdef CONFIG_SND_OMAP_SOC_ABE_DSP
	/* ABE OPP1 - OPP50 */
	OPP_INITIALIZER("aess", "abe_clk", "iva", true,
			98304000, OMAP4430_VDD_IVA_OPP50_UV),
	/* ABE OPP2 - OPP100 */
	OPP_INITIALIZER("aess", "abe_clk", "iva", true,
			196608000, OMAP4430_VDD_IVA_OPP100_UV),
#endif
	/* DSS OPP1 - OPP50 */
	OPP_INITIALIZER("dss_dispc", "virt_lcd_pclk", "core", true,
			93000000, OMAP4430_VDD_CORE_OPP50_UV),
	/* DSS OPP2 - OPP100 */
	OPP_INITIALIZER("dss_dispc", "virt_lcd_pclk", "core", true,
			170000000, OMAP4430_VDD_CORE_OPP100_UV),
};

/**
 * omap4_opp_enable() - helper to enable the OPP
 * @oh_name: name of the hwmod device
 * @freq:	frequency to enable
 */
static void __init omap4_opp_enable(const char *oh_name, unsigned long freq)
{
	struct device *dev;
	int r;

	dev = omap_hwmod_name_get_dev(oh_name);
	if (IS_ERR(dev)) {
		pr_err("%s: no %s device, did not enable f=%ld\n", __func__, oh_name, freq);
		return;
	}
	r = opp_enable(dev, freq);
	if (r < 0)
		dev_err(dev, "%s: opp_enable failed(%d) f=%ld\n", __func__, r, freq);
}

/**
 * omap4_opp_init() - initialize omap4 opp table
 */
int __init omap4_opp_init(void)
{
	int r = -ENODEV;
	int trimmed = 1;

	if (!cpu_is_omap44xx())
		return r;

	r = omap_init_opp_table(omap443x_opp_def_list, ARRAY_SIZE(omap443x_opp_def_list));

	if (r)
		goto out;
#ifdef CONFIG_OMAP4430_TOP_CPU
	else {
#ifdef CONFIG_MACH_SAMSUNG_ESPRESSO_10
		omap4_opp_enable("mpu", 1216000000);
		omap4_opp_enable("mpu", 1360000000);
		omap4_opp_enable("mpu", 1420000000);
		omap4_opp_enable("mpu", 1460000000);
#else
		omap4_opp_enable("mpu", 1200000000);
		omap4_opp_enable("mpu", 1350000000);
		omap4_opp_enable("mpu", 1420000000);
		omap4_opp_enable("mpu", 1480000000);
#endif
	}
#endif

	/* Enable Nitro and NitroSB IVA OPPs */
	if (omap4_has_iva_430mhz())
		omap4_opp_enable("iva", 430000000);
	if (omap4_has_iva_500mhz())
		omap4_opp_enable("iva", 500000000);

out:
	return r;
}

fs_initcall(omap4_opp_init);
