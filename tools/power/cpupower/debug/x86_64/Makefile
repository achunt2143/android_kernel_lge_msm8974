<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
OUTPUT=./
ifeq ("$(origin O)", "command line")
	OUTPUT := $(O)/
endif

DESTDIR =
bindir  = /usr/bin

INSTALL = /usr/bin/install


default: all

$(OUTPUT)centrino-decode: ../i386/centrino-decode.c
<<<<<<< HEAD
	$(CC) $(CFLAGS) -o $@ $<

$(OUTPUT)powernow-k8-decode: ../i386/powernow-k8-decode.c
	$(CC) $(CFLAGS) -o $@ $<
=======
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $<

$(OUTPUT)powernow-k8-decode: ../i386/powernow-k8-decode.c
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $<
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

all: $(OUTPUT)centrino-decode $(OUTPUT)powernow-k8-decode

clean:
	rm -rf $(OUTPUT)centrino-decode $(OUTPUT)powernow-k8-decode

install:
	$(INSTALL) -d $(DESTDIR)${bindir}
	$(INSTALL) $(OUTPUT)centrino-decode $(DESTDIR)${bindir}
	$(INSTALL) $(OUTPUT)powernow-k8-decode $(DESTDIR)${bindir}

.PHONY: all default clean install
