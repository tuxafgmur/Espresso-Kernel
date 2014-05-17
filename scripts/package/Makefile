# Makefile for the different targets used to generate full packages of a kernel
# It uses the generic clean infrastructure of kbuild

# tarball targets
# ---------------------------------------------------------------------------
tar%pkg: FORCE
	$(MAKE) KBUILD_SRC=
	$(CONFIG_SHELL) $(srctree)/scripts/package/buildtar $@

clean-dirs += $(objtree)/tar-install/

# perf-pkg - generate a source tarball with perf source
# ---------------------------------------------------------------------------

perf-tar=perf-$(KERNELVERSION)

quiet_cmd_perf_tar = TAR
      cmd_perf_tar = \
git archive --prefix=$(perf-tar)/ HEAD^{tree}                       \
	$$(cat $(srctree)/tools/perf/MANIFEST) -o $(perf-tar).tar;  \
mkdir -p $(perf-tar);                                               \
git rev-parse HEAD > $(perf-tar)/HEAD;                              \
tar rf $(perf-tar).tar $(perf-tar)/HEAD;                            \
rm -r $(perf-tar);                                                  \
$(if $(findstring tar-src,$@),,                                     \
$(if $(findstring bz2,$@),bzip2,                                    \
$(if $(findstring gz,$@),gzip,                                      \
$(if $(findstring xz,$@),xz,                                        \
$(error unknown target $@))))                                       \
	-f -9 $(perf-tar).tar)

perf-%pkg: FORCE
	$(call cmd,perf_tar)

# Help text displayed when executing 'make help'
# ---------------------------------------------------------------------------
help: FORCE
	@echo '  tar-pkg             - Build the kernel as an uncompressed tarball'
	@echo '  targz-pkg           - Build the kernel as a gzip compressed tarball'
	@echo '  tarbz2-pkg          - Build the kernel as a bzip2 compressed tarball'
	@echo '  tarxz-pkg           - Build the kernel as a xz compressed tarball'
	@echo '  perf-tar-src-pkg    - Build $(perf-tar).tar source tarball'
	@echo '  perf-targz-src-pkg  - Build $(perf-tar).tar.gz source tarball'
	@echo '  perf-tarbz2-src-pkg - Build $(perf-tar).tar.bz2 source tarball'
	@echo '  perf-tarxz-src-pkg  - Build $(perf-tar).tar.xz source tarball'
