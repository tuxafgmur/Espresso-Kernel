#
# Makefile for the kernel block device drivers.
#
# 12 June 2000, Christoph Hellwig <hch@infradead.org>
# Rewritten to use lists instead of if-statements.
#

obj-$(CONFIG_MAC_FLOPPY)	+= swim3.o
obj-$(CONFIG_BLK_DEV_SWIM)	+= swim_mod.o
obj-$(CONFIG_BLK_DEV_FD)	+= floppy.o
obj-$(CONFIG_AMIGA_FLOPPY)	+= amiflop.o
obj-$(CONFIG_PS3_DISK)		+= ps3disk.o
obj-$(CONFIG_PS3_VRAM)		+= ps3vram.o
obj-$(CONFIG_ATARI_FLOPPY)	+= ataflop.o
obj-$(CONFIG_AMIGA_Z2RAM)	+= z2ram.o
obj-$(CONFIG_BLK_DEV_RAM)	+= brd.o
obj-$(CONFIG_BLK_DEV_LOOP)	+= loop.o
obj-$(CONFIG_XILINX_SYSACE)	+= xsysace.o
obj-$(CONFIG_CDROM_PKTCDVD)	+= pktcdvd.o
obj-$(CONFIG_MG_DISK)		+= mg_disk.o
obj-$(CONFIG_BLK_DEV_OSD)	+= osdblk.o
obj-$(CONFIG_BLK_DEV_NBD)	+= nbd.o
obj-$(CONFIG_BLK_DEV_CRYPTOLOOP) += cryptoloop.o
obj-$(CONFIG_VIRTIO_BLK)	+= virtio_blk.o
obj-$(CONFIG_BLK_DEV_UB)	+= ub.o

swim_mod-y			:= swim.o swim_asm.o
