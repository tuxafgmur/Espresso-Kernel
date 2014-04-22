# Makefile for the Linux sound card driver

obj-$(CONFIG_SOUND) += soundcore.o
obj-$(CONFIG_SND) += core/ i2c/ drivers/ arm/ synth/ usb/ spi/ soc/

ifeq ($(CONFIG_SND),y)
  obj-y += last.o
endif

soundcore-objs  := sound_core.o
