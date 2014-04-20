/*
 * rtc-twl.c -- TWL Real Time Clock interface
 *
 * Copyright (C) 2007 MontaVista Software, Inc
 * Author: Alexandre Rusev <source@mvista.com>
 *
 * Based on original TI driver twl4030-rtc.c
 *   Copyright (C) 2006 Texas Instruments, Inc.
 *
 * Based on rtc-omap.c
 *   Copyright (C) 2003 MontaVista Software, Inc.
 *   Author: George G. Davis <gdavis@mvista.com> or <source@mvista.com>
 *   Copyright (C) 2006 David Brownell
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/rtc.h>
#include <linux/bcd.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>

#include <linux/i2c/twl.h>

/*
 * RTC block register offsets (use TWL_MODULE_RTC)
 */
enum {
	REG_SECONDS_REG = 0,
	REG_MINUTES_REG,
	REG_HOURS_REG,
	REG_DAYS_REG,
	REG_MONTHS_REG,
	REG_YEARS_REG,
	REG_WEEKS_REG,

	REG_ALARM_SECONDS_REG,
	REG_ALARM_MINUTES_REG,
	REG_ALARM_HOURS_REG,
	REG_ALARM_DAYS_REG,
	REG_ALARM_MONTHS_REG,
	REG_ALARM_YEARS_REG,

	REG_RTC_CTRL_REG,
	REG_RTC_STATUS_REG,
	REG_RTC_INTERRUPTS_REG,

	REG_RTC_COMP_LSB_REG,
	REG_RTC_COMP_MSB_REG,
};
static const u8 twl4030_rtc_reg_map[] = {
	[REG_SECONDS_REG] = 0x00,
	[REG_MINUTES_REG] = 0x01,
	[REG_HOURS_REG] = 0x02,
	[REG_DAYS_REG] = 0x03,
	[REG_MONTHS_REG] = 0x04,
	[REG_YEARS_REG] = 0x05,
	[REG_WEEKS_REG] = 0x06,

	[REG_ALARM_SECONDS_REG] = 0x07,
	[REG_ALARM_MINUTES_REG] = 0x08,
	[REG_ALARM_HOURS_REG] = 0x09,
	[REG_ALARM_DAYS_REG] = 0x0A,
	[REG_ALARM_MONTHS_REG] = 0x0B,
	[REG_ALARM_YEARS_REG] = 0x0C,

	[REG_RTC_CTRL_REG] = 0x0D,
	[REG_RTC_STATUS_REG] = 0x0E,
	[REG_RTC_INTERRUPTS_REG] = 0x0F,

	[REG_RTC_COMP_LSB_REG] = 0x10,
	[REG_RTC_COMP_MSB_REG] = 0x11,
};
static const u8 twl6030_rtc_reg_map[] = {
	[REG_SECONDS_REG] = 0x00,
	[REG_MINUTES_REG] = 0x01,
	[REG_HOURS_REG] = 0x02,
	[REG_DAYS_REG] = 0x03,
	[REG_MONTHS_REG] = 0x04,
	[REG_YEARS_REG] = 0x05,
	[REG_WEEKS_REG] = 0x06,

	[REG_ALARM_SECONDS_REG] = 0x08,
	[REG_ALARM_MINUTES_REG] = 0x09,
	[REG_ALARM_HOURS_REG] = 0x0A,
	[REG_ALARM_DAYS_REG] = 0x0B,
	[REG_ALARM_MONTHS_REG] = 0x0C,
	[REG_ALARM_YEARS_REG] = 0x0D,

	[REG_RTC_CTRL_REG] = 0x10,
	[REG_RTC_STATUS_REG] = 0x11,
	[REG_RTC_INTERRUPTS_REG] = 0x12,

	[REG_RTC_COMP_LSB_REG] = 0x13,
	[REG_RTC_COMP_MSB_REG] = 0x14,
};

/* RTC_CTRL_REG bitfields */
#define BIT_RTC_CTRL_REG_STOP_RTC_M              0x01
#define BIT_RTC_CTRL_REG_ROUND_30S_M             0x02
#define BIT_RTC_CTRL_REG_AUTO_COMP_M             0x04
#define BIT_RTC_CTRL_REG_MODE_12_24_M            0x08
#define BIT_RTC_CTRL_REG_TEST_MODE_M             0x10
#define BIT_RTC_CTRL_REG_SET_32_COUNTER_M        0x20
#define BIT_RTC_CTRL_REG_GET_TIME_M              0x40
#define BIT_RTC_CTRL_REG_RTC_V_OPT               0x80

/* RTC_STATUS_REG bitfields */
#define BIT_RTC_STATUS_REG_RUN_M                 0x02
#define BIT_RTC_STATUS_REG_1S_EVENT_M            0x04
#define BIT_RTC_STATUS_REG_1M_EVENT_M            0x08
#define BIT_RTC_STATUS_REG_1H_EVENT_M            0x10
#define BIT_RTC_STATUS_REG_1D_EVENT_M            0x20
#define BIT_RTC_STATUS_REG_ALARM_M               0x40
#define BIT_RTC_STATUS_REG_POWER_UP_M            0x80

/* RTC_INTERRUPTS_REG bitfields */
#define BIT_RTC_INTERRUPTS_REG_EVERY_M           0x03
#define BIT_RTC_INTERRUPTS_REG_IT_TIMER_M        0x04
#define BIT_RTC_INTERRUPTS_REG_IT_ALARM_M        0x08

/* REG_SECONDS_REG through REG_YEARS_REG is how many registers? */
#define ALL_TIME_REGS		6

/*----------------------------------------------------------------------*/
static u8  *rtc_reg_map;

/*
 * Supports 1 byte read from TWL RTC register.
 */
static int twl_rtc_read_u8(u8 *data, u8 reg)
{
	int ret;

	ret = twl_i2c_read_u8(TWL_MODULE_RTC, data, (rtc_reg_map[reg]));
	if (ret < 0)
		pr_err("twl_rtc: Could not read TWL"
		       "register %X - error %d\n", reg, ret);
	return ret;
}

/*
 * Supports 1 byte write to TWL RTC registers.
 */
static int twl_rtc_write_u8(u8 data, u8 reg)
{
	int ret;

	ret = twl_i2c_write_u8(TWL_MODULE_RTC, data, (rtc_reg_map[reg]));
	if (ret < 0)
		pr_err("twl_rtc: Could not write TWL"
		       "register %X - error %d\n", reg, ret);
	return ret;
}

/*
 * Cache the value for timer/alarm interrupts register; this is
 * only changed by callers holding rtc ops lock (or resume).
 */
static unsigned char rtc_irq_bits;

/*
 * Enable 1/second update and/or alarm interrupts.
 */
static int set_rtc_irq_bit(unsigned char bit)
{
	unsigned char val;
	int ret;

	val = rtc_irq_bits | bit;
	val &= ~BIT_RTC_INTERRUPTS_REG_EVERY_M;
	ret = twl_rtc_write_u8(val, REG_RTC_INTERRUPTS_REG);
	if (ret == 0)
		rtc_irq_bits = val;

	return ret;
}

/*
 * Disable update and/or alarm interrupts.
 */
static int mask_rtc_irq_bit(unsigned char bit)
{
	unsigned char val;
	int ret;

	val = rtc_irq_bits & ~bit;
	ret = twl_rtc_write_u8(val, REG_RTC_INTERRUPTS_REG);
	if (ret == 0)
		rtc_irq_bits = val;

	return ret;
}

static int twl_rtc_alarm_irq_enable(struct device *dev, unsigned enabled)
{
	int ret;

	if (enabled)
		ret = set_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_ALARM_M);
	else
		ret = mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_ALARM_M);

	return ret;
}

/*
 * Gets current TWL RTC time and date parameters.
 *
 * The RTC's time/alarm representation is not what gmtime(3) requires
 * Linux to use:
 *
 *  - Months are 1..12 vs Linux 0-11
 *  - Years are 0..99 vs Linux 1900..N (we assume 21st century)
 */
static int twl_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char rtc_data[ALL_TIME_REGS + 1];
	int ret;
	u8 save_control;
	u8 rtc_control;

	ret = twl_rtc_read_u8(&save_control, REG_RTC_CTRL_REG);
	if (ret < 0) {
		dev_err(dev, "%s: reading CTRL_REG, error %d\n", __func__, ret);
		return ret;
	}
	/* for twl6030/32 make sure BIT_RTC_CTRL_REG_GET_TIME_M is clear */
	if (twl_class_is_6030()) {
		if (save_control & BIT_RTC_CTRL_REG_GET_TIME_M) {
			save_control &= ~BIT_RTC_CTRL_REG_GET_TIME_M;
			ret = twl_rtc_write_u8(save_control, REG_RTC_CTRL_REG);
			if (ret < 0) {
				dev_err(dev, "%s, writing CTRL_REG, error %d\n",
						__func__, ret);
				return ret;
			}
		}
	}

	/* Copy RTC counting registers to static registers or latches */
	rtc_control = save_control | BIT_RTC_CTRL_REG_GET_TIME_M;

	/* for twl6030/32 enable read access to static shadowed registers */
	if (twl_class_is_6030())
		rtc_control |= BIT_RTC_CTRL_REG_RTC_V_OPT;

	ret = twl_rtc_write_u8(rtc_control, REG_RTC_CTRL_REG);
	if (ret < 0) {
		dev_err(dev, "%s: writing CTRL_REG, error %d\n", __func__, ret);
		return ret;
	}

	ret = twl_i2c_read(TWL_MODULE_RTC, rtc_data,
			(rtc_reg_map[REG_SECONDS_REG]), ALL_TIME_REGS);

	if (ret < 0) {
		dev_err(dev, "%s: reading data, error %d\n", __func__, ret);
		return ret;
	}

	/* for twl6030 restore original state of rtc control register */
	if (twl_class_is_6030()) {
		ret = twl_rtc_write_u8(save_control, REG_RTC_CTRL_REG);
		if (ret < 0) {
			dev_err(dev, "%s: writing CTRL_REG, error %d\n",
					__func__, ret);
			return ret;
		}
	}

	tm->tm_sec = bcd2bin(rtc_data[0]);
	tm->tm_min = bcd2bin(rtc_data[1]);
	tm->tm_hour = bcd2bin(rtc_data[2]);
	tm->tm_mday = bcd2bin(rtc_data[3]);
	tm->tm_mon = bcd2bin(rtc_data[4]) - 1;
	tm->tm_year = bcd2bin(rtc_data[5]) + 100;

	return ret;
}

static int twl_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	unsigned char save_control;
	unsigned char rtc_data[ALL_TIME_REGS + 1];
	int ret;

	rtc_data[1] = bin2bcd(tm->tm_sec);
	rtc_data[2] = bin2bcd(tm->tm_min);
	rtc_data[3] = bin2bcd(tm->tm_hour);
	rtc_data[4] = bin2bcd(tm->tm_mday);
	rtc_data[5] = bin2bcd(tm->tm_mon + 1);
	rtc_data[6] = bin2bcd(tm->tm_year - 100);

	printk(KERN_INFO"%s, %d/%d/%d %d:%d:%d\n", __func__,
			tm->tm_year-100,
			tm->tm_mon+1,
			tm->tm_mday,
			tm->tm_hour,
			tm->tm_min,
			tm->tm_sec);

	/* Stop RTC while updating the TC registers */
	ret = twl_rtc_read_u8(&save_control, REG_RTC_CTRL_REG);
	if (ret < 0)
		goto out;

	save_control &= ~BIT_RTC_CTRL_REG_STOP_RTC_M;
	ret = twl_rtc_write_u8(save_control, REG_RTC_CTRL_REG);
	if (ret < 0)
		goto out;

	/* update all the time registers in one shot */
	ret = twl_i2c_write(TWL_MODULE_RTC, rtc_data,
		(rtc_reg_map[REG_SECONDS_REG]), ALL_TIME_REGS);
	if (ret < 0) {
		dev_err(dev, "rtc_set_time error %d\n", ret);
		goto out;
	}

	/* Start back RTC */
	save_control |= BIT_RTC_CTRL_REG_STOP_RTC_M;
	ret = twl_rtc_write_u8(save_control, REG_RTC_CTRL_REG);

out:
	return ret;
}

/*
 * Gets current TWL RTC alarm time.
 */
static int twl_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	unsigned char rtc_data[ALL_TIME_REGS + 1];
	int ret;

	ret = twl_i2c_read(TWL_MODULE_RTC, rtc_data,
			(rtc_reg_map[REG_ALARM_SECONDS_REG]), ALL_TIME_REGS);
	if (ret < 0) {
		dev_err(dev, "rtc_read_alarm error %d\n", ret);
		return ret;
	}

	/* some of these fields may be wildcard/"match all" */
	alm->time.tm_sec = bcd2bin(rtc_data[0]);
	alm->time.tm_min = bcd2bin(rtc_data[1]);
	alm->time.tm_hour = bcd2bin(rtc_data[2]);
	alm->time.tm_mday = bcd2bin(rtc_data[3]);
	alm->time.tm_mon = bcd2bin(rtc_data[4]) - 1;
	alm->time.tm_year = bcd2bin(rtc_data[5]) + 100;

	/* report cached alarm enable state */
	if (rtc_irq_bits & BIT_RTC_INTERRUPTS_REG_IT_ALARM_M)
		alm->enabled = 1;

	return ret;
}

static int twl_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alm)
{
	unsigned char alarm_data[ALL_TIME_REGS + 1];
	int ret;

	ret = twl_rtc_alarm_irq_enable(dev, 0);
	if (ret)
		goto out;

	alarm_data[1] = bin2bcd(alm->time.tm_sec);
	alarm_data[2] = bin2bcd(alm->time.tm_min);
	alarm_data[3] = bin2bcd(alm->time.tm_hour);
	alarm_data[4] = bin2bcd(alm->time.tm_mday);
	alarm_data[5] = bin2bcd(alm->time.tm_mon + 1);
	alarm_data[6] = bin2bcd(alm->time.tm_year - 100);

	printk(KERN_INFO"%s, %d/%d/%d %d:%d:%d\n", __func__,
			alm->time.tm_year-100,
			alm->time.tm_mon+1,
			alm->time.tm_mday,
			alm->time.tm_hour,
			alm->time.tm_min,
			alm->time.tm_sec);

	/* update all the alarm registers in one shot */
	ret = twl_i2c_write(TWL_MODULE_RTC, alarm_data,
		(rtc_reg_map[REG_ALARM_SECONDS_REG]), ALL_TIME_REGS);
	if (ret) {
		dev_err(dev, "rtc_set_alarm error %d\n", ret);
		goto out;
	}

	if (alm->enabled)
		ret = twl_rtc_alarm_irq_enable(dev, 1);
out:
	return ret;
}

#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
struct rtc_time alm_reg_time;
static void check_alarm_boot()
{
	int i, ret;
	unsigned long time_sec, alarm_sec;
	u8 t_rtc_time[6];
	struct rtc_time current_rtc_time;

	for (i = 0x00 ; i < 0x06 ; i++)
		ret = twl_rtc_read_u8(&t_rtc_time[i], i);

	current_rtc_time.tm_sec = bcd2bin(t_rtc_time[0]);
	current_rtc_time.tm_min = bcd2bin(t_rtc_time[1]);
	current_rtc_time.tm_hour = bcd2bin(t_rtc_time[2]);
	current_rtc_time.tm_mday = bcd2bin(t_rtc_time[3]);
	current_rtc_time.tm_mon = bcd2bin(t_rtc_time[4]) - 1;
	current_rtc_time.tm_year = bcd2bin(t_rtc_time[5]) + 100;

	rtc_tm_to_time(&current_rtc_time, &time_sec);
	rtc_tm_to_time(&alm_reg_time, &alarm_sec);

	if ((time_sec > alarm_sec - 20) && (time_sec < alarm_sec + 20))
		kernel_restart(NULL);
}
#endif

static irqreturn_t twl_rtc_interrupt(int irq, void *rtc)
{
	unsigned long events = 0;
	int ret = IRQ_NONE;
	int res;
	u8 rd_reg;
#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
	u8 check_alm_boot;
#endif

	res = twl_rtc_read_u8(&rd_reg, REG_RTC_STATUS_REG);
	if (res)
		goto out;

#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
	res = twl_rtc_read_u8(&check_alm_boot, 0x17);
	if (res)
		goto out;

	if ((sec_bootmode == 5) && check_alm_boot) {
		if (rd_reg & 0x40)
			check_alarm_boot();
	}
#endif

	/*
	 * Figure out source of interrupt: ALARM or TIMER in RTC_STATUS_REG.
	 * only one (ALARM or RTC) interrupt source may be enabled
	 * at time, we also could check our results
	 * by reading RTS_INTERRUPTS_REGISTER[IT_TIMER,IT_ALARM]
	 */
	if (rd_reg & BIT_RTC_STATUS_REG_ALARM_M)
		events |= RTC_IRQF | RTC_AF;
	else
		events |= RTC_IRQF | RTC_UF;

	res = twl_rtc_write_u8(rd_reg | BIT_RTC_STATUS_REG_ALARM_M,
				   REG_RTC_STATUS_REG);
	if (res)
		goto out;

	if (twl_class_is_4030()) {
		/* Clear on Read enabled. RTC_IT bit of TWL4030_INT_PWR_ISR1
		 * needs 2 reads to clear the interrupt. One read is done in
		 * do_twl_pwrirq(). Doing the second read, to clear
		 * the bit.
		 *
		 * FIXME the reason PWR_ISR1 needs an extra read is that
		 * RTC_IF retriggered until we cleared REG_ALARM_M above.
		 * But re-reading like this is a bad hack; by doing so we
		 * risk wrongly clearing status for some other IRQ (losing
		 * the interrupt).  Be smarter about handling RTC_UF ...
		 */
		res = twl_i2c_read_u8(TWL4030_MODULE_INT,
			&rd_reg, TWL4030_INT_PWR_ISR1);
		if (res)
			goto out;
	}

	/* Notify RTC core on event */
	rtc_update_irq(rtc, 1, events);

	ret = IRQ_HANDLED;
out:
	return ret;
}

static struct rtc_class_ops twl_rtc_ops = {
	.read_time	= twl_rtc_read_time,
	.set_time	= twl_rtc_set_time,
	.read_alarm	= twl_rtc_read_alarm,
	.set_alarm	= twl_rtc_set_alarm,
	.alarm_irq_enable = twl_rtc_alarm_irq_enable,
};

/*----------------------------------------------------------------------*/

static int __devinit twl_rtc_probe(struct platform_device *pdev)
{
	struct rtc_device *rtc;
	struct twl4030_rtc_data *rtc_platform_data
		= pdev->dev.platform_data;
	int ret = -EINVAL;
	int irq = platform_get_irq(pdev, 0);
	u8 rd_reg;
#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
	int i;
	u8 t_alarm_time[6];
#endif

	if (irq <= 0)
		goto out1;

	ret = twl_rtc_read_u8(&rd_reg, REG_RTC_STATUS_REG);
	if (ret < 0)
		goto out1;

	if (rd_reg & BIT_RTC_STATUS_REG_POWER_UP_M) {
		dev_warn(&pdev->dev, "Power up reset detected.\n");

		if (rtc_platform_data &&
				rtc_platform_data->
				rtc_default_time.tm_year > 100) {
			twl_rtc_set_time(&pdev->dev,
			&rtc_platform_data->rtc_default_time);
		}

	}

	if (rd_reg & BIT_RTC_STATUS_REG_ALARM_M)
		dev_warn(&pdev->dev, "Pending Alarm interrupt detected.\n");

	/* Clear RTC Power up reset and pending alarm interrupts */
	ret = twl_rtc_write_u8(rd_reg, REG_RTC_STATUS_REG);
	if (ret < 0)
		goto out1;

	if (twl_class_is_6030()) {
		twl6030_interrupt_unmask(TWL6030_RTC_INT_MASK,
			REG_INT_MSK_LINE_A);
		twl6030_interrupt_unmask(TWL6030_RTC_INT_MASK,
			REG_INT_MSK_STS_A);
	}

	/* Check RTC module status, Enable if it is off */
	ret = twl_rtc_read_u8(&rd_reg, REG_RTC_CTRL_REG);
	if (ret < 0)
		goto out1;

	if (!(rd_reg & BIT_RTC_CTRL_REG_STOP_RTC_M)) {
		dev_info(&pdev->dev, "Enabling TWL-RTC.\n");
		rd_reg = BIT_RTC_CTRL_REG_STOP_RTC_M;
		ret = twl_rtc_write_u8(rd_reg, REG_RTC_CTRL_REG);
		if (ret < 0)
			goto out1;
	}

	/* init cached IRQ enable bits */
	ret = twl_rtc_read_u8(&rtc_irq_bits, REG_RTC_INTERRUPTS_REG);
	if (ret < 0)
		goto out1;

	rtc = rtc_device_register(pdev->name,
				  &pdev->dev, &twl_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc)) {
		ret = PTR_ERR(rtc);
		dev_err(&pdev->dev, "can't register RTC device, err %ld\n",
			PTR_ERR(rtc));
		goto out1;

	}

	ret = request_threaded_irq(irq, NULL, twl_rtc_interrupt,
				   IRQF_TRIGGER_RISING,
				   dev_name(&rtc->dev), rtc);
	if (ret < 0) {
		dev_err(&pdev->dev, "IRQ is not free.\n");
		goto out2;
	}

	if (enable_irq_wake(irq) < 0)
		dev_warn(&pdev->dev, "Cannot enable wakeup for IRQ %d\n", irq);

	platform_set_drvdata(pdev, rtc);
	{
		struct rtc_time tm;
		twl_rtc_read_time(&pdev->dev, &tm);

		printk(KERN_INFO"%s, %d/%d/%d %d:%d:%d\n", __func__,
				tm.tm_year-100,
				tm.tm_mon+1,
				tm.tm_mday,
				tm.tm_hour,
				tm.tm_min,
				tm.tm_sec);
	}

	if (rtc_platform_data &&
		rtc_platform_data->auto_comp) {

		u8 lsb_value, msb_value, control;
		s16 comp_value = ~(rtc_platform_data->comp_value) + 1;
		dev_info(&pdev->dev, "Auto Comp =0x%x  2's-Complement=0x%x\n",
				rtc_platform_data->comp_value, comp_value);

		lsb_value = comp_value & 0xFF;
		msb_value = ((comp_value >> 15) << 7) |
					((comp_value & 0xFF00) >> 8);

		ret |= twl_rtc_write_u8(msb_value, REG_RTC_COMP_MSB_REG);
		ret |= twl_rtc_write_u8(lsb_value, REG_RTC_COMP_LSB_REG);

		ret |= twl_rtc_read_u8(&control, REG_RTC_CTRL_REG);
		control |= BIT_RTC_CTRL_REG_AUTO_COMP_M;
		ret |= twl_rtc_write_u8(control, REG_RTC_CTRL_REG);

		ret |= twl_rtc_read_u8(&msb_value, REG_RTC_COMP_MSB_REG);
		ret |= twl_rtc_read_u8(&lsb_value, REG_RTC_COMP_LSB_REG);
		dev_info(&pdev->dev, "COMP_MSB=0x%x  COMP_LSB=0x%x\n",
				msb_value, lsb_value);

		if (ret < 0)
			dev_err(&pdev->dev, "Fail to Compensate RTC Clock\n");
	}

#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
	for (i = 0; i < 6; i++)
		ret = twl_rtc_read_u8(&t_alarm_time[i], i+7);
	alm_reg_time.tm_sec = bcd2bin(t_alarm_time[0]);
	alm_reg_time.tm_min = bcd2bin(t_alarm_time[1]);
	alm_reg_time.tm_hour = bcd2bin(t_alarm_time[2]);
	alm_reg_time.tm_mday = bcd2bin(t_alarm_time[3]);
	alm_reg_time.tm_mon = bcd2bin(t_alarm_time[4]) - 1;
	alm_reg_time.tm_year = bcd2bin(t_alarm_time[5]) + 100;

	ret = twl_i2c_write_u8(TWL_MODULE_RTC, 0x00, 0x17);
	if (ret < 0)
		pr_err("twl_rtc: Could not write TWL"
		       "register 0x17 - error %d\n", ret);
#endif

	return 0;

out2:
	rtc_device_unregister(rtc);
out1:
	return ret;
}

/*
 * Disable all TWL RTC module interrupts.
 * Sets status flag to free.
 */
static int __devexit twl_rtc_remove(struct platform_device *pdev)
{
	/* leave rtc running, but disable irqs */
	struct rtc_device *rtc = platform_get_drvdata(pdev);
	int irq = platform_get_irq(pdev, 0);

	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_ALARM_M);
	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_TIMER_M);
	if (twl_class_is_6030()) {
		twl6030_interrupt_mask(TWL6030_RTC_INT_MASK,
			REG_INT_MSK_LINE_A);
		twl6030_interrupt_mask(TWL6030_RTC_INT_MASK,
			REG_INT_MSK_STS_A);
	}

	free_irq(irq, rtc);

	rtc_device_unregister(rtc);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

static void twl_rtc_shutdown(struct platform_device *pdev)
{
	/* mask timer interrupts*/
	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_TIMER_M);

#ifdef CONFIG_ANDROID
	/* mask alarm interrupts as well so that we don't get powered on
	 when alarm is triggered on android */
	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_ALARM_M);
#endif

#if !defined(CONFIG_RTC_CHN_ALARM_BOOT)
	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_ALARM_M);
#endif

#if defined(CONFIG_RTC_CHN_ALARM_BOOT)
	twl_rtc_set_alarm(&pdev->dev, &autoboot_alm_exit);
	if (autoboot_alm_exit.enabled)
		twl_i2c_write_u8(TWL_MODULE_RTC, 0x01, 0x17);
	else
		twl_i2c_write_u8(TWL_MODULE_RTC, 0x00, 0x17);
#endif
}

#ifdef CONFIG_PM

static unsigned char irqstat;

static int twl_rtc_suspend(struct platform_device *pdev, pm_message_t state)
{
	irqstat = rtc_irq_bits;

	mask_rtc_irq_bit(BIT_RTC_INTERRUPTS_REG_IT_TIMER_M);
	return 0;
}

static int twl_rtc_resume(struct platform_device *pdev)
{
	set_rtc_irq_bit(irqstat);
	return 0;
}

#else
#define twl_rtc_suspend NULL
#define twl_rtc_resume  NULL
#endif

MODULE_ALIAS("platform:twl_rtc");

static struct platform_driver twl4030rtc_driver = {
	.probe		= twl_rtc_probe,
	.remove		= __devexit_p(twl_rtc_remove),
	.shutdown	= twl_rtc_shutdown,
	.suspend	= twl_rtc_suspend,
	.resume		= twl_rtc_resume,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "twl_rtc",
	},
};

static int __init twl_rtc_init(void)
{
	if (twl_class_is_4030())
		rtc_reg_map = (u8 *) twl4030_rtc_reg_map;
	else
		rtc_reg_map = (u8 *) twl6030_rtc_reg_map;

	return platform_driver_register(&twl4030rtc_driver);
}
module_init(twl_rtc_init);

static void __exit twl_rtc_exit(void)
{
	platform_driver_unregister(&twl4030rtc_driver);
}
module_exit(twl_rtc_exit);

MODULE_AUTHOR("Texas Instruments, MontaVista Software");
MODULE_LICENSE("GPL");
