obj-y				+= tty_io.o n_tty.o tty_ioctl.o tty_ldisc.o \
				   tty_buffer.o tty_port.o tty_mutex.o
obj-$(CONFIG_LEGACY_PTYS)	+= pty.o
obj-$(CONFIG_UNIX98_PTYS)	+= pty.o
obj-$(CONFIG_AUDIT)		+= tty_audit.o
obj-$(CONFIG_MAGIC_SYSRQ)	+= sysrq.o
obj-$(CONFIG_N_HDLC)		+= n_hdlc.o
obj-$(CONFIG_N_GSM)		+= n_gsm.o
obj-$(CONFIG_TRACE_ROUTER)	+= n_tracerouter.o
obj-$(CONFIG_TRACE_SINK)	+= n_tracesink.o
obj-$(CONFIG_R3964)		+= n_r3964.o

obj-y				+= vt/
obj-$(CONFIG_HVC_DRIVER)	+= hvc/
obj-y				+= serial/

# tty drivers
obj-$(CONFIG_AMIGA_BUILTIN_SERIAL) += amiserial.o
obj-$(CONFIG_BFIN_JTAG_COMM)	+= bfin_jtag_comm.o

obj-y 				+= ipwireless/
