<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile

<<<<<<< HEAD
PHONY += oldconfig xconfig gconfig menuconfig config silentoldconfig update-po-config \
	localmodconfig localyesconfig

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ifdef KBUILD_KCONFIG
Kconfig := $(KBUILD_KCONFIG)
else
Kconfig := Kconfig
endif

<<<<<<< HEAD
xconfig: $(obj)/qconf
	$< $(Kconfig)

gconfig: $(obj)/gconf
	$< $(Kconfig)

menuconfig: $(obj)/mconf
	$< $(Kconfig)

config: $(obj)/conf
	$< --oldaskconfig $(Kconfig)

nconfig: $(obj)/nconf
	$< $(Kconfig)

oldconfig: $(obj)/conf
	$< --$@ $(Kconfig)

silentoldconfig: $(obj)/conf
	$(Q)mkdir -p include/generated
	$< --$@ $(Kconfig)

localyesconfig localmodconfig: $(obj)/streamline_config.pl $(obj)/conf
	$(Q)mkdir -p include/generated
	$(Q)perl $< --$@ $(srctree) $(Kconfig) > .tmp.config
	$(Q)if [ -f .config ]; then 					\
			cmp -s .tmp.config .config ||			\
			(mv -f .config .config.old.1;			\
			 mv -f .tmp.config .config;			\
			 $(obj)/conf --silentoldconfig $(Kconfig);	\
			 mv -f .config.old.1 .config.old)		\
	else								\
			mv -f .tmp.config .config;			\
			$(obj)/conf --silentoldconfig $(Kconfig);	\
	fi
	$(Q)rm -f .tmp.config

# Create new linux.pot file
# Adjust charset to UTF-8 in .po file to accept UTF-8 in Kconfig files
update-po-config: $(obj)/kxgettext $(obj)/gconf.glade.h
	$(Q)echo "  GEN     config.pot"
	$(Q)xgettext --default-domain=linux                         \
	    --add-comments --keyword=_ --keyword=N_                 \
	    --from-code=UTF-8                                       \
	    --files-from=$(srctree)/scripts/kconfig/POTFILES.in     \
	    --directory=$(srctree) --directory=$(objtree)           \
	    --output $(obj)/config.pot
	$(Q)sed -i s/CHARSET/UTF-8/ $(obj)/config.pot
	$(Q)(for i in `ls $(srctree)/arch/*/Kconfig      \
	    $(srctree)/arch/*/um/Kconfig`;               \
	    do                                           \
		echo "  GEN     $$i";                    \
		$(obj)/kxgettext $$i                     \
		     >> $(obj)/config.pot;               \
	    done )
	$(Q)echo "  GEN     linux.pot"
	$(Q)msguniq --sort-by-file --to-code=UTF-8 $(obj)/config.pot \
	    --output $(obj)/linux.pot
	$(Q)rm -f $(obj)/config.pot

PHONY += allnoconfig allyesconfig allmodconfig alldefconfig randconfig

allnoconfig allyesconfig allmodconfig alldefconfig randconfig: $(obj)/conf
	$< --$@ $(Kconfig)

PHONY += listnewconfig oldnoconfig savedefconfig defconfig

listnewconfig oldnoconfig: $(obj)/conf
	$< --$@ $(Kconfig)

savedefconfig: $(obj)/conf
	$< --$@=defconfig $(Kconfig)

defconfig: $(obj)/conf
ifeq ($(KBUILD_DEFCONFIG),)
	$< --defconfig $(Kconfig)
else
	@echo "*** Default configuration is based on '$(KBUILD_DEFCONFIG)'"
	$(Q)$< --defconfig=arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG) $(Kconfig)
endif

%_defconfig: $(obj)/conf
	$(Q)$< --defconfig=arch/$(SRCARCH)/configs/$@ $(Kconfig)

# Help text used by make help
help:
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  nconfig         - Update current config utilising a ncurses menu based program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  xconfig	  - Update current config utilising a QT based front-end'
	@echo  '  gconfig	  - Update current config utilising a GTK based front-end'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	@echo  '  localmodconfig  - Update current config disabling modules not loaded'
	@echo  '  localyesconfig  - Update current config converting local mods to core'
	@echo  '  silentoldconfig - Same as oldconfig, but quietly, additionally update deps'
=======
ifndef KBUILD_DEFCONFIG
KBUILD_DEFCONFIG := defconfig
endif

ifeq ($(quiet),silent_)
silent := -s
endif

export KCONFIG_DEFCONFIG_LIST :=
ifndef cross_compiling
kernel-release := $(shell uname -r)
KCONFIG_DEFCONFIG_LIST += \
	/lib/modules/$(kernel-release)/.config \
	/etc/kernel-config \
	/boot/config-$(kernel-release)
endif
KCONFIG_DEFCONFIG_LIST += arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG)

ifneq ($(findstring c, $(KBUILD_EXTRA_WARN)),)
export KCONFIG_WARN_UNKNOWN_SYMBOLS=1
endif

ifneq ($(findstring e, $(KBUILD_EXTRA_WARN)),)
export KCONFIG_WERROR=1
endif

# We need this, in case the user has it in its environment
unexport CONFIG_

config-prog	:= conf
menuconfig-prog	:= mconf
nconfig-prog	:= nconf
gconfig-prog	:= gconf
xconfig-prog	:= qconf

define config_rule
PHONY += $(1)
$(1): $(obj)/$($(1)-prog)
	$(Q)$$< $(silent) $(Kconfig)

PHONY += build_$(1)
build_$(1): $(obj)/$($(1)-prog)
endef

$(foreach c, config menuconfig nconfig gconfig xconfig, $(eval $(call config_rule,$(c))))

PHONY += localmodconfig localyesconfig
localyesconfig localmodconfig: $(obj)/conf
	$(Q)$(PERL) $(srctree)/$(src)/streamline_config.pl --$@ $(srctree) $(Kconfig) > .tmp.config
	$(Q)if [ -f .config ]; then 				\
		cmp -s .tmp.config .config ||			\
		(mv -f .config .config.old.1;			\
		 mv -f .tmp.config .config;			\
		 $< $(silent) --oldconfig $(Kconfig);		\
		 mv -f .config.old.1 .config.old)		\
	else							\
		mv -f .tmp.config .config;			\
		$< $(silent) --oldconfig $(Kconfig);		\
	fi
	$(Q)rm -f .tmp.config

# These targets map 1:1 to the commandline options of 'conf'
#
# Note:
#  syncconfig has become an internal implementation detail and is now
#  deprecated for external use
simple-targets := oldconfig allnoconfig allyesconfig allmodconfig \
	alldefconfig randconfig listnewconfig olddefconfig syncconfig \
	helpnewconfig yes2modconfig mod2yesconfig mod2noconfig

PHONY += $(simple-targets)

$(simple-targets): $(obj)/conf
	$(Q)$< $(silent) --$@ $(Kconfig)

PHONY += savedefconfig defconfig

savedefconfig: $(obj)/conf
	$(Q)$< $(silent) --$@=defconfig $(Kconfig)

defconfig: $(obj)/conf
ifneq ($(wildcard $(srctree)/arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG)),)
	@$(kecho) "*** Default configuration is based on '$(KBUILD_DEFCONFIG)'"
	$(Q)$< $(silent) --defconfig=arch/$(SRCARCH)/configs/$(KBUILD_DEFCONFIG) $(Kconfig)
else
	@$(kecho) "*** Default configuration is based on target '$(KBUILD_DEFCONFIG)'"
	$(Q)$(MAKE) -f $(srctree)/Makefile $(KBUILD_DEFCONFIG)
endif

%_defconfig: $(obj)/conf
	$(Q)$< $(silent) --defconfig=arch/$(SRCARCH)/configs/$@ $(Kconfig)

configfiles = $(wildcard $(srctree)/kernel/configs/$(1) $(srctree)/arch/$(SRCARCH)/configs/$(1))
all-config-fragments = $(call configfiles,*.config)
config-fragments = $(call configfiles,$@)

%.config: $(obj)/conf
	$(if $(config-fragments),, $(error $@ fragment does not exists on this architecture))
	$(Q)$(CONFIG_SHELL) $(srctree)/scripts/kconfig/merge_config.sh -m $(KCONFIG_CONFIG) $(config-fragments)
	$(Q)$(MAKE) -f $(srctree)/Makefile olddefconfig

PHONY += tinyconfig
tinyconfig:
	$(Q)KCONFIG_ALLCONFIG=kernel/configs/tiny-base.config $(MAKE) -f $(srctree)/Makefile allnoconfig
	$(Q)$(MAKE) -f $(srctree)/Makefile tiny.config

# CHECK: -o cache_dir=<path> working?
PHONY += testconfig
testconfig: $(obj)/conf
	$(Q)$(PYTHON3) -B -m pytest $(srctree)/$(src)/tests \
	-o cache_dir=$(abspath $(obj)/tests/.cache) \
	$(if $(findstring 1,$(KBUILD_VERBOSE)),--capture=no)
clean-files += tests/.cache

# Help text used by make help
help:
	@echo  'Configuration targets:'
	@echo  '  config	  - Update current config utilising a line-oriented program'
	@echo  '  nconfig         - Update current config utilising a ncurses menu based program'
	@echo  '  menuconfig	  - Update current config utilising a menu based program'
	@echo  '  xconfig	  - Update current config utilising a Qt based front-end'
	@echo  '  gconfig	  - Update current config utilising a GTK+ based front-end'
	@echo  '  oldconfig	  - Update current config utilising a provided .config as base'
	@echo  '  localmodconfig  - Update current config disabling modules not loaded'
	@echo  '                    except those preserved by LMC_KEEP environment variable'
	@echo  '  localyesconfig  - Update current config converting local mods to core'
	@echo  '                    except those preserved by LMC_KEEP environment variable'
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	@echo  '  defconfig	  - New config with default from ARCH supplied defconfig'
	@echo  '  savedefconfig   - Save current config as ./defconfig (minimal config)'
	@echo  '  allnoconfig	  - New config where all options are answered with no'
	@echo  '  allyesconfig	  - New config where all options are accepted with yes'
	@echo  '  allmodconfig	  - New config selecting modules when possible'
	@echo  '  alldefconfig    - New config with all symbols set to default'
	@echo  '  randconfig	  - New config with random answer to all options'
<<<<<<< HEAD
	@echo  '  listnewconfig   - List new options'
	@echo  '  oldnoconfig     - Same as silentoldconfig but set new symbols to n (unset)'

# lxdialog stuff
check-lxdialog  := $(srctree)/$(src)/lxdialog/check-lxdialog.sh

# Use recursively expanded variables so we do not call gcc unless
# we really need to do so. (Do not call gcc as part of make mrproper)
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(check-lxdialog) -ccflags) \
                    -DLOCALE

# ===========================================================================
# Shared Makefile for the various kconfig executables:
# conf:	  Used for defconfig, oldconfig and related targets
# nconf:  Used for the nconfig target.
#         Utilizes ncurses
# mconf:  Used for the menuconfig target
#         Utilizes the lxdialog package
# qconf:  Used for the xconfig target
#         Based on QT which needs to be installed to compile it
# gconf:  Used for the gconfig target
#         Based on GTK which needs to be installed to compile it
# object files used by all kconfig flavours

lxdialog := lxdialog/checklist.o lxdialog/util.o lxdialog/inputbox.o
lxdialog += lxdialog/textbox.o lxdialog/yesno.o lxdialog/menubox.o

conf-objs	:= conf.o  zconf.tab.o
mconf-objs     := mconf.o zconf.tab.o $(lxdialog)
nconf-objs     := nconf.o zconf.tab.o nconf.gui.o
kxgettext-objs	:= kxgettext.o zconf.tab.o
qconf-cxxobjs	:= qconf.o
qconf-objs	:= zconf.tab.o
gconf-objs	:= gconf.o zconf.tab.o

hostprogs-y := conf

ifeq ($(MAKECMDGOALS),nconfig)
	hostprogs-y += nconf
endif

ifeq ($(MAKECMDGOALS),menuconfig)
	hostprogs-y += mconf
endif

ifeq ($(MAKECMDGOALS),update-po-config)
	hostprogs-y += kxgettext
endif

ifeq ($(MAKECMDGOALS),xconfig)
	qconf-target := 1
endif
ifeq ($(MAKECMDGOALS),gconfig)
	gconf-target := 1
endif


ifeq ($(qconf-target),1)
	hostprogs-y += qconf
endif

ifeq ($(gconf-target),1)
	hostprogs-y += gconf
endif

clean-files	:= qconf.moc .tmp_qtcheck .tmp_gtkcheck
clean-files	+= zconf.tab.c zconf.lex.c zconf.hash.c gconf.glade.h
clean-files     += mconf qconf gconf nconf
clean-files     += config.pot linux.pot

# Check that we have the required ncurses stuff installed for lxdialog (menuconfig)
PHONY += $(obj)/dochecklxdialog
$(addprefix $(obj)/,$(lxdialog)): $(obj)/dochecklxdialog
$(obj)/dochecklxdialog:
	$(Q)$(CONFIG_SHELL) $(check-lxdialog) -check $(HOSTCC) $(HOST_EXTRACFLAGS) $(HOSTLOADLIBES_mconf)

always := dochecklxdialog

# Add environment specific flags
HOST_EXTRACFLAGS += $(shell $(CONFIG_SHELL) $(srctree)/$(src)/check.sh $(HOSTCC) $(HOSTCFLAGS))

# generated files seem to need this to find local include files
HOSTCFLAGS_zconf.lex.o	:= -I$(src)
HOSTCFLAGS_zconf.tab.o	:= -I$(src)

LEX_PREFIX_zconf	:= zconf
YACC_PREFIX_zconf	:= zconf

HOSTLOADLIBES_qconf	= $(KC_QT_LIBS)
HOSTCXXFLAGS_qconf.o	= $(KC_QT_CFLAGS)

HOSTLOADLIBES_gconf	= `pkg-config --libs gtk+-2.0 gmodule-2.0 libglade-2.0`
HOSTCFLAGS_gconf.o	= `pkg-config --cflags gtk+-2.0 gmodule-2.0 libglade-2.0` \
                          -Wno-missing-prototypes

HOSTLOADLIBES_mconf   = $(shell $(CONFIG_SHELL) $(check-lxdialog) -ldflags $(HOSTCC))

HOSTLOADLIBES_nconf	= -lmenu -lpanel -lncurses
$(obj)/qconf.o: $(obj)/.tmp_qtcheck

ifeq ($(qconf-target),1)
$(obj)/.tmp_qtcheck: $(src)/Makefile
-include $(obj)/.tmp_qtcheck

# QT needs some extra effort...
$(obj)/.tmp_qtcheck:
	@set -e; echo "  CHECK   qt"; dir=""; pkg=""; \
	if ! pkg-config --exists QtCore 2> /dev/null; then \
	    echo "* Unable to find the QT4 tool qmake. Trying to use QT3"; \
	    pkg-config --exists qt 2> /dev/null && pkg=qt; \
	    pkg-config --exists qt-mt 2> /dev/null && pkg=qt-mt; \
	    if [ -n "$$pkg" ]; then \
	      cflags="\$$(shell pkg-config $$pkg --cflags)"; \
	      libs="\$$(shell pkg-config $$pkg --libs)"; \
	      moc="\$$(shell pkg-config $$pkg --variable=prefix)/bin/moc"; \
	      dir="$$(pkg-config $$pkg --variable=prefix)"; \
	    else \
	      for d in $$QTDIR /usr/share/qt* /usr/lib/qt*; do \
	        if [ -f $$d/include/qconfig.h ]; then dir=$$d; break; fi; \
	      done; \
	      if [ -z "$$dir" ]; then \
	        echo "*"; \
	        echo "* Unable to find any QT installation. Please make sure that"; \
	        echo "* the QT4 or QT3 development package is correctly installed and"; \
	        echo "* either qmake can be found or install pkg-config or set"; \
	        echo "* the QTDIR environment variable to the correct location."; \
	        echo "*"; \
	        false; \
	      fi; \
	      libpath=$$dir/lib; lib=qt; osdir=""; \
	      $(HOSTCXX) -print-multi-os-directory > /dev/null 2>&1 && \
	        osdir=x$$($(HOSTCXX) -print-multi-os-directory); \
	      test -d $$libpath/$$osdir && libpath=$$libpath/$$osdir; \
	      test -f $$libpath/libqt-mt.so && lib=qt-mt; \
	      cflags="-I$$dir/include"; \
	      libs="-L$$libpath -Wl,-rpath,$$libpath -l$$lib"; \
	      moc="$$dir/bin/moc"; \
	    fi; \
	    if [ ! -x $$dir/bin/moc -a -x /usr/bin/moc ]; then \
	      echo "*"; \
	      echo "* Unable to find $$dir/bin/moc, using /usr/bin/moc instead."; \
	      echo "*"; \
	      moc="/usr/bin/moc"; \
	    fi; \
	else \
	  cflags="\$$(shell pkg-config QtCore QtGui Qt3Support --cflags)"; \
	  libs="\$$(shell pkg-config QtCore QtGui Qt3Support --libs)"; \
	  binpath="\$$(shell pkg-config QtCore --variable=prefix)"; \
	  moc="$$binpath/bin/moc"; \
	fi; \
	echo "KC_QT_CFLAGS=$$cflags" > $@; \
	echo "KC_QT_LIBS=$$libs" >> $@; \
	echo "KC_QT_MOC=$$moc" >> $@
endif

$(obj)/gconf.o: $(obj)/.tmp_gtkcheck

ifeq ($(gconf-target),1)
-include $(obj)/.tmp_gtkcheck

# GTK needs some extra effort, too...
$(obj)/.tmp_gtkcheck:
	@if `pkg-config --exists gtk+-2.0 gmodule-2.0 libglade-2.0`; then		\
		if `pkg-config --atleast-version=2.0.0 gtk+-2.0`; then			\
			touch $@;								\
		else									\
			echo "*"; 							\
			echo "* GTK+ is present but version >= 2.0.0 is required.";	\
			echo "*";							\
			false;								\
		fi									\
	else										\
		echo "*"; 								\
		echo "* Unable to find the GTK+ installation. Please make sure that"; 	\
		echo "* the GTK+ 2.0 development package is correctly installed..."; 	\
		echo "* You need gtk+-2.0, glib-2.0 and libglade-2.0."; 		\
		echo "*"; 								\
		false;									\
	fi
endif

$(obj)/zconf.tab.o: $(obj)/zconf.lex.c $(obj)/zconf.hash.c

$(obj)/qconf.o: $(obj)/qconf.moc

$(obj)/%.moc: $(src)/%.h
	$(KC_QT_MOC) -i $< -o $@

# Extract gconf menu items for I18N support
$(obj)/gconf.glade.h: $(obj)/gconf.glade
	$(Q)intltool-extract --type=gettext/glade --srcdir=$(srctree) \
	$(obj)/gconf.glade

=======
	@echo  '  yes2modconfig	  - Change answers from yes to mod if possible'
	@echo  '  mod2yesconfig	  - Change answers from mod to yes if possible'
	@echo  '  mod2noconfig	  - Change answers from mod to no if possible'
	@echo  '  listnewconfig   - List new options'
	@echo  '  helpnewconfig   - List new options and help text'
	@echo  '  olddefconfig	  - Same as oldconfig but sets new symbols to their'
	@echo  '                    default value without prompting'
	@echo  '  tinyconfig	  - Configure the tiniest possible kernel'
	@echo  '  testconfig	  - Run Kconfig unit tests (requires python3 and pytest)'
	@echo  ''
	@echo  'Configuration topic targets:'
	@$(foreach f, $(all-config-fragments), \
		if help=$$(grep -m1 '^# Help: ' $(f)); then \
			printf '  %-25s - %s\n' '$(notdir $(f))' "$${help#*: }"; \
		fi;)

# ===========================================================================
# object files used by all kconfig flavours
common-objs	:= confdata.o expr.o lexer.lex.o menu.o parser.tab.o \
		   preprocess.o symbol.o util.o

$(obj)/lexer.lex.o: $(obj)/parser.tab.h
HOSTCFLAGS_lexer.lex.o	:= -I $(srctree)/$(src)
HOSTCFLAGS_parser.tab.o	:= -I $(srctree)/$(src)

# conf: Used for defconfig, oldconfig and related targets
hostprogs	+= conf
conf-objs	:= conf.o $(common-objs)

# nconf: Used for the nconfig target based on ncurses
hostprogs	+= nconf
nconf-objs	:= nconf.o nconf.gui.o mnconf-common.o $(common-objs)

HOSTLDLIBS_nconf       = $(call read-file, $(obj)/nconf-libs)
HOSTCFLAGS_nconf.o     = $(call read-file, $(obj)/nconf-cflags)
HOSTCFLAGS_nconf.gui.o = $(call read-file, $(obj)/nconf-cflags)

$(obj)/nconf: | $(obj)/nconf-libs
$(obj)/nconf.o $(obj)/nconf.gui.o: | $(obj)/nconf-cflags

# mconf: Used for the menuconfig target based on lxdialog
hostprogs	+= mconf
lxdialog	:= $(addprefix lxdialog/, \
		     checklist.o inputbox.o menubox.o textbox.o util.o yesno.o)
mconf-objs	:= mconf.o $(lxdialog) mnconf-common.o $(common-objs)

HOSTLDLIBS_mconf = $(call read-file, $(obj)/mconf-libs)
$(foreach f, mconf.o $(lxdialog), \
  $(eval HOSTCFLAGS_$f = $$(call read-file, $(obj)/mconf-cflags)))

$(obj)/mconf: | $(obj)/mconf-libs
$(addprefix $(obj)/, mconf.o $(lxdialog)): | $(obj)/mconf-cflags

# qconf: Used for the xconfig target based on Qt
hostprogs	+= qconf
qconf-cxxobjs	:= qconf.o qconf-moc.o
qconf-objs	:= images.o $(common-objs)

HOSTLDLIBS_qconf         = $(call read-file, $(obj)/qconf-libs)
HOSTCXXFLAGS_qconf.o     = -std=c++11 -fPIC $(call read-file, $(obj)/qconf-cflags)
HOSTCXXFLAGS_qconf-moc.o = -std=c++11 -fPIC $(call read-file, $(obj)/qconf-cflags)
$(obj)/qconf: | $(obj)/qconf-libs
$(obj)/qconf.o $(obj)/qconf-moc.o: | $(obj)/qconf-cflags

quiet_cmd_moc = MOC     $@
      cmd_moc = $(call read-file, $(obj)/qconf-bin)/moc $< -o $@

$(obj)/qconf-moc.cc: $(src)/qconf.h FORCE | $(obj)/qconf-bin
	$(call if_changed,moc)

targets += qconf-moc.cc

# gconf: Used for the gconfig target based on GTK+
hostprogs	+= gconf
gconf-objs	:= gconf.o images.o $(common-objs)

HOSTLDLIBS_gconf   = $(call read-file, $(obj)/gconf-libs)
HOSTCFLAGS_gconf.o = $(call read-file, $(obj)/gconf-cflags)

$(obj)/gconf: | $(obj)/gconf-libs
$(obj)/gconf.o: | $(obj)/gconf-cflags

# check if necessary packages are available, and configure build flags
cmd_conf_cfg = $< $(addprefix $(obj)/$*conf-, cflags libs bin); touch $(obj)/$*conf-bin

$(obj)/%conf-cflags $(obj)/%conf-libs $(obj)/%conf-bin: $(src)/%conf-cfg.sh
	$(call cmd,conf_cfg)

clean-files += *conf-cflags *conf-libs *conf-bin
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
