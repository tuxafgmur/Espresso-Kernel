# Makefile for the Linux video drivers.
# 5 Aug 1999, James Simmons, <mailto:jsimmons@users.sf.net>
# Rewritten to use lists instead of if-statements.

obj-$(CONFIG_VGASTATE)            += vgastate.o
obj-y                             += fb_notify.o
obj-$(CONFIG_FB)                  += fb.o
fb-y                              := fbmem.o fbmon.o fbcmap.o fbsysfs.o \
                                     modedb.o fbcvt.o
fb-objs                           := $(fb-y)

obj-$(CONFIG_VT)		  += console/
obj-$(CONFIG_LOGO)		  += logo/
obj-y				  += backlight/ display/

obj-$(CONFIG_FB_CFB_FILLRECT)  += cfbfillrect.o
obj-$(CONFIG_FB_CFB_COPYAREA)  += cfbcopyarea.o
obj-$(CONFIG_FB_CFB_IMAGEBLIT) += cfbimgblt.o
obj-$(CONFIG_FB_SYS_FILLRECT)  += sysfillrect.o
obj-$(CONFIG_FB_SYS_COPYAREA)  += syscopyarea.o
obj-$(CONFIG_FB_SYS_IMAGEBLIT) += sysimgblt.o
obj-$(CONFIG_FB_SYS_FOPS)      += fb_sys_fops.o
obj-$(CONFIG_FB_SVGALIB)       += svgalib.o
obj-$(CONFIG_FB_MACMODES)      += macmodes.o
obj-$(CONFIG_FB_DDC)           += fb_ddc.o
obj-$(CONFIG_FB_DEFERRED_IO)   += fb_defio.o
obj-$(CONFIG_FB_WMT_GE_ROPS)   += wmt_ge_rops.o

# Hardware specific drivers go first
obj-$(CONFIG_FB_AMIGA)            += amifb.o c2p_planar.o
obj-$(CONFIG_FB_ARC)              += arcfb.o
obj-$(CONFIG_FB_CLPS711X)         += clps711xfb.o
obj-$(CONFIG_FB_CYBER2000)        += cyber2000fb.o
obj-$(CONFIG_FB_PM2)              += pm2fb.o
obj-$(CONFIG_FB_PM3)		  += pm3fb.o
obj-$(CONFIG_FB_KYRO)             += kyro/
obj-$(CONFIG_FB_MBX)		  += mbx/
obj-$(CONFIG_FB_NEOMAGIC)         += neofb.o
obj-$(CONFIG_FB_FM2)              += fm2fb.o
obj-$(CONFIG_FB_VT8623)           += vt8623fb.o
obj-$(CONFIG_FB_LE80578)          += vermilion/
obj-$(CONFIG_FB_S3)               += s3fb.o
obj-$(CONFIG_FB_ARK)              += arkfb.o
obj-$(CONFIG_FB_ACORN)            += acornfb.o
obj-$(CONFIG_FB_HECUBA)           += hecubafb.o
obj-$(CONFIG_FB_APOLLO)           += dnfb.o
obj-$(CONFIG_FB_Q40)              += q40fb.o
obj-$(CONFIG_FB_EP93XX)		  += ep93xx-fb.o
obj-$(CONFIG_FB_SA1100)           += sa1100fb.o
obj-$(CONFIG_FB_HIT)              += hitfb.o
obj-$(CONFIG_FB_ATMEL)		  += atmel_lcdfb.o
obj-$(CONFIG_FB_PVR2)             += pvr2fb.o
obj-$(CONFIG_FB_VOODOO1)          += sstfb.o
obj-$(CONFIG_FB_ARMCLCD)	  += amba-clcd.o
obj-$(CONFIG_FB_68328)            += 68328fb.o
obj-$(CONFIG_FB_PXA)		  += pxafb.o
obj-$(CONFIG_FB_PXA168)		  += pxa168fb.o
obj-$(CONFIG_PXA3XX_GCU)	  += pxa3xx-gcu.o
obj-$(CONFIG_FB_W100)		  += w100fb.o
obj-$(CONFIG_FB_TMIO)		  += tmiofb.o
obj-$(CONFIG_FB_VT8500)		  += vt8500lcdfb.o
obj-$(CONFIG_FB_WM8505)		  += wm8505fb.o
obj-$(CONFIG_FB_PMAG_AA)	  += pmag-aa-fb.o
obj-$(CONFIG_FB_PMAG_BA)	  += pmag-ba-fb.o
obj-$(CONFIG_FB_PMAGB_B)	  += pmagb-b-fb.o
obj-$(CONFIG_FB_MAXINE)		  += maxinefb.o
obj-$(CONFIG_FB_METRONOME)        += metronomefb.o
obj-$(CONFIG_FB_BROADSHEET)       += broadsheetfb.o
obj-$(CONFIG_FB_S1D13XXX)	  += s1d13xxxfb.o
obj-$(CONFIG_FB_SH7760)		  += sh7760fb.o
obj-$(CONFIG_FB_IMX)              += imxfb.o
obj-$(CONFIG_FB_S3C)		  += s3c-fb.o
obj-$(CONFIG_FB_S3C2410)	  += s3c2410fb.o
obj-$(CONFIG_FB_FSL_DIU)	  += fsl-diu-fb.o
obj-$(CONFIG_FB_PNX4008_DUM)	  += pnx4008/
obj-$(CONFIG_FB_PNX4008_DUM_RGB)  += pnx4008/
obj-$(CONFIG_FB_PS3)		  += ps3fb.o
obj-$(CONFIG_FB_SM501)            += sm501fb.o
obj-$(CONFIG_FB_UDL)		  += udlfb.o
obj-$(CONFIG_FB_XILINX)           += xilinxfb.o
obj-$(CONFIG_SH_MIPI_DSI)	  += sh_mipi_dsi.o
obj-$(CONFIG_FB_SH_MOBILE_HDMI)	  += sh_mobile_hdmi.o
obj-$(CONFIG_FB_SH_MOBILE_MERAM)  += sh_mobile_meram.o
obj-$(CONFIG_FB_SH_MOBILE_LCDC)	  += sh_mobile_lcdcfb.o
obj-$(CONFIG_FB_OMAP)             += omap/
obj-y                             += omap2/
obj-$(CONFIG_HDMI_TI_4XXX_IP)     += hdmi_ti_4xxx_ip.o
obj-$(CONFIG_FB_CARMINE)          += carminefb.o
obj-$(CONFIG_FB_MB862XX)	  += mb862xx/
obj-$(CONFIG_FB_MSM)              += msm/
obj-$(CONFIG_FB_NUC900)           += nuc900fb.o
obj-$(CONFIG_FB_JZ4740)		  += jz4740_fb.o
obj-$(CONFIG_FB_PUV3_UNIGFX)      += fb-puv3.o
obj-$(CONFIG_FB_UVESA)            += uvesafb.o
obj-$(CONFIG_FB_BF537_LQ035)      += bf537-lq035.o
obj-$(CONFIG_FB_BF54X_LQ043)	  += bf54x-lq043fb.o
obj-$(CONFIG_FB_BFIN_LQ035Q1)     += bfin-lq035q1-fb.o
obj-$(CONFIG_FB_BFIN_T350MCQB)	  += bfin-t350mcqb-fb.o
obj-$(CONFIG_FB_BFIN_7393)        += bfin_adv7393fb.o
obj-$(CONFIG_FB_MX3)		  += mx3fb.o
obj-$(CONFIG_FB_DA8XX)		  += da8xx-fb.o
obj-$(CONFIG_FB_MXS)		  += mxsfb.o
obj-$(CONFIG_SII9234)  		  += sii9234.o

# the test framebuffer is last
obj-$(CONFIG_FB_VIRTUAL)          += vfb.o

#video output switch sysfs driver
obj-$(CONFIG_VIDEO_OUTPUT_CONTROL) += output.o
