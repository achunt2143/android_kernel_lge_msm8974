<<<<<<< HEAD
PROG=	aicasm

=======
# SPDX-License-Identifier: GPL-2.0
PROG=	aicasm

OUTDIR ?= ./

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
.SUFFIXES= .l .y .c .h

CSRCS=	aicasm.c aicasm_symbol.c
YSRCS=	aicasm_gram.y aicasm_macro_gram.y
LSRCS=	aicasm_scan.l aicasm_macro_scan.l

<<<<<<< HEAD
GENHDRS=	aicdb.h $(YSRCS:.y=.h)
GENSRCS=	$(YSRCS:.y=.c) $(LSRCS:.l=.c)
=======
GENHDRS=	$(addprefix ${OUTDIR}/,aicdb.h $(YSRCS:.y=.h))
GENSRCS=	$(addprefix ${OUTDIR}/,$(YSRCS:.y=.c) $(LSRCS:.l=.c))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

SRCS=	${CSRCS} ${GENSRCS}
LIBS=	-ldb
clean-files:= ${GENSRCS} ${GENHDRS} $(YSRCS:.y=.output) $(PROG)
# Override default kernel CFLAGS.  This is a userland app.
<<<<<<< HEAD
AICASM_CFLAGS:= -I/usr/include -I.
=======
AICASM_CFLAGS:= -I/usr/include -I. -I$(OUTDIR)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
LEX= flex
YACC= bison
YFLAGS= -d

NOMAN=	noman

ifneq ($(HOSTCC),)
AICASM_CC= $(HOSTCC)
else
AICASM_CC= $(CC)
endif

ifdef DEBUG
CFLAGS+= -DDEBUG -g
YFLAGS+= -t -v
LFLAGS= -d
endif

<<<<<<< HEAD
$(PROG):  ${GENHDRS} $(SRCS)
	$(AICASM_CC) $(AICASM_CFLAGS) $(SRCS) -o $(PROG) $(LIBS)

aicdb.h:
	@if [ -e "/usr/include/db4/db_185.h" ]; then		\
		echo "#include <db4/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db3/db_185.h" ]; then		\
		echo "#include <db3/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db2/db_185.h" ]; then		\
		echo "#include <db2/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db1/db_185.h" ]; then		\
		echo "#include <db1/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db/db_185.h" ]; then		\
		echo "#include <db/db_185.h>" > aicdb.h;	\
	 elif [ -e "/usr/include/db_185.h" ]; then		\
		echo "#include <db_185.h>" > aicdb.h;		\
=======
$(PROG):  $(OUTDIR) ${GENHDRS} $(SRCS)
	$(AICASM_CC) $(AICASM_CFLAGS) $(SRCS) -o $(OUTDIR)/$(PROG) $(LIBS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/aicdb.h:
	@if [ -e "/usr/include/db4/db_185.h" ]; then		\
		echo "#include <db4/db_185.h>" > $@;	\
	 elif [ -e "/usr/include/db3/db_185.h" ]; then		\
		echo "#include <db3/db_185.h>" > $@;	\
	 elif [ -e "/usr/include/db2/db_185.h" ]; then		\
		echo "#include <db2/db_185.h>" > $@;	\
	 elif [ -e "/usr/include/db1/db_185.h" ]; then		\
		echo "#include <db1/db_185.h>" > $@;	\
	 elif [ -e "/usr/include/db/db_185.h" ]; then		\
		echo "#include <db/db_185.h>" > $@;	\
	 elif [ -e "/usr/include/db_185.h" ]; then		\
		echo "#include <db_185.h>" > $@;		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 else							\
		echo "*** Install db development libraries";	\
	 fi

clean:
	rm -f $(clean-files)

<<<<<<< HEAD
# Create a dependency chain in generated files
# to avoid concurrent invocations of the single
# rule that builds them all.
aicasm_gram.c: aicasm_gram.h
aicasm_gram.c aicasm_gram.h: aicasm_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) $<
	mv $(<:.y=).tab.c $(<:.y=.c)
	mv $(<:.y=).tab.h $(<:.y=.h)

# Create a dependency chain in generated files
# to avoid concurrent invocations of the single
# rule that builds them all.
aicasm_macro_gram.c: aicasm_macro_gram.h
aicasm_macro_gram.c aicasm_macro_gram.h: aicasm_macro_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) -p mm $<
	mv $(<:.y=).tab.c $(<:.y=.c)
	mv $(<:.y=).tab.h $(<:.y=.h)

aicasm_scan.c: aicasm_scan.l
	$(LEX) $(LFLAGS) -o$@ $<

aicasm_macro_scan.c: aicasm_macro_scan.l
	$(LEX) $(LFLAGS) -Pmm -o$@ $<
=======
$(OUTDIR)/aicasm_gram.c $(OUTDIR)/aicasm_gram.h: aicasm_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) $< -o $(OUTDIR)/$(<:.y=.c)

$(OUTDIR)/aicasm_macro_gram.c $(OUTDIR)/aicasm_macro_gram.h: aicasm_macro_gram.y
	$(YACC) $(YFLAGS) -b $(<:.y=) -p mm $< -o $(OUTDIR)/$(<:.y=.c)

$(OUTDIR)/aicasm_scan.c: aicasm_scan.l
	$(LEX) $(LFLAGS) -o $@ $<

$(OUTDIR)/aicasm_macro_scan.c: aicasm_macro_scan.l
	$(LEX) $(LFLAGS) -Pmm -o $@ $<
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
