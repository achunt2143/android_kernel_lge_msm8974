<<<<<<< HEAD
obj-y = audit.o common.o condition.o domain.o environ.o file.o gc.o group.o load_policy.o memory.o mount.o network.o realpath.o securityfs_if.o tomoyo.o util.o

$(obj)/policy/profile.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/profile.conf
	@touch $@

$(obj)/policy/exception_policy.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating a default policy/exception_policy.conf
	@echo initialize_domain /sbin/modprobe from any >> $@
	@echo initialize_domain /sbin/hotplug from any >> $@

$(obj)/policy/domain_policy.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/domain_policy.conf
	@touch $@

$(obj)/policy/manager.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/manager.conf
	@touch $@

$(obj)/policy/stat.conf:
	@mkdir -p $(obj)/policy/
	@echo Creating an empty policy/stat.conf
	@touch $@

$(obj)/builtin-policy.h: $(obj)/policy/profile.conf $(obj)/policy/exception_policy.conf $(obj)/policy/domain_policy.conf $(obj)/policy/manager.conf $(obj)/policy/stat.conf
	@echo Generating built-in policy for TOMOYO 2.5.x.
	@echo "static char tomoyo_builtin_profile[] __initdata =" > $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/profile.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_exception_policy[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/exception_policy.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_domain_policy[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/domain_policy.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_manager[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/manager.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@echo "static char tomoyo_builtin_stat[] __initdata =" >> $@.tmp
	@sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/"\1\\n"/' < $(obj)/policy/stat.conf >> $@.tmp
	@echo "\"\";" >> $@.tmp
	@mv $@.tmp $@

$(obj)/common.o: $(obj)/builtin-policy.h
=======
# SPDX-License-Identifier: GPL-2.0
obj-y = audit.o common.o condition.o domain.o environ.o file.o gc.o group.o load_policy.o memory.o mount.o network.o realpath.o securityfs_if.o tomoyo.o util.o

targets += builtin-policy.h

quiet_cmd_policy = POLICY  $@
      cmd_policy = { \
	$(foreach x, profile exception_policy domain_policy manager stat, \
	printf 'static char tomoyo_builtin_$x[] __initdata =\n'; \
	sed -e 's/\\/\\\\/g' -e 's/\"/\\"/g' -e 's/\(.*\)/\t"\1\\n"/' -- $(firstword $(filter %/$x.conf %/$x.conf.default, $^) /dev/null);  \
	printf '\t"";\n';) \
	} > $@

$(obj)/builtin-policy.h: $(wildcard $(obj)/policy/*.conf $(srctree)/$(src)/policy/*.conf.default) FORCE
	$(call if_changed,policy)

ifndef CONFIG_SECURITY_TOMOYO_INSECURE_BUILTIN_SETTING
$(obj)/common.o: $(obj)/builtin-policy.h
endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
