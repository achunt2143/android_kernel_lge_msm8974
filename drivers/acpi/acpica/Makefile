<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#
# Makefile for ACPICA Core interpreter
#

<<<<<<< HEAD
ccflags-y			:= -Os
=======
ccflags-y			:= -D_LINUX -DBUILDING_ACPICA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ccflags-$(CONFIG_ACPI_DEBUG)	+= -DACPI_DEBUG_OUTPUT

# use acpi.o to put all files here into acpi.o modparam namespace
obj-y	+= acpi.o

acpi-y :=		\
	dsargs.o	\
	dscontrol.o	\
<<<<<<< HEAD
=======
	dsdebug.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dsfield.o	\
	dsinit.o	\
	dsmethod.o	\
	dsmthdat.o	\
	dsobject.o	\
	dsopcode.o	\
<<<<<<< HEAD
=======
	dspkginit.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dsutils.o	\
	dswexec.o	\
	dswload.o	\
	dswload2.o	\
	dswscope.o	\
	dswstate.o

acpi-y +=		\
	evevent.o	\
	evgpe.o		\
	evgpeblk.o	\
	evgpeinit.o	\
	evgpeutil.o	\
	evglock.o	\
<<<<<<< HEAD
=======
	evhandler.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	evmisc.o	\
	evregion.o	\
	evrgnini.o	\
	evsci.o		\
	evxface.o	\
	evxfevnt.o	\
	evxfgpe.o	\
	evxfregn.o

acpi-y +=		\
<<<<<<< HEAD
=======
	exconcat.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exconfig.o	\
	exconvrt.o	\
	excreate.o	\
	exdebug.o	\
	exdump.o	\
	exfield.o	\
	exfldio.o	\
<<<<<<< HEAD
=======
	exmisc.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exmutex.o	\
	exnames.o	\
	exoparg1.o	\
	exoparg2.o	\
	exoparg3.o	\
	exoparg6.o	\
	exprep.o	\
<<<<<<< HEAD
	exmisc.o	\
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exregion.o	\
	exresnte.o	\
	exresolv.o	\
	exresop.o	\
<<<<<<< HEAD
=======
	exserial.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exstore.o	\
	exstoren.o	\
	exstorob.o	\
	exsystem.o	\
<<<<<<< HEAD
=======
	extrace.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	exutils.o

acpi-y +=		\
	hwacpi.o	\
	hwesleep.o	\
	hwgpe.o		\
<<<<<<< HEAD
	hwpci.o		\
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hwregs.o	\
	hwsleep.o	\
	hwvalid.o	\
	hwxface.o	\
	hwxfsleep.o

<<<<<<< HEAD
=======
acpi-$(CONFIG_PCI) += hwpci.o
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
acpi-$(ACPI_FUTURE_USAGE) += hwtimer.o

acpi-y +=		\
	nsaccess.o	\
	nsalloc.o	\
<<<<<<< HEAD
=======
	nsarguments.o	\
	nsconvert.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nsdump.o	\
	nseval.o	\
	nsinit.o	\
	nsload.o	\
	nsnames.o	\
	nsobject.o	\
	nsparse.o	\
	nspredef.o	\
<<<<<<< HEAD
=======
	nsprepkg.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	nsrepair.o	\
	nsrepair2.o	\
	nssearch.o	\
	nsutils.o	\
	nswalk.o	\
	nsxfeval.o	\
	nsxfname.o	\
	nsxfobj.o

acpi-$(ACPI_FUTURE_USAGE) += nsdumpdv.o

acpi-y +=		\
	psargs.o	\
	psloop.o	\
<<<<<<< HEAD
	psopcode.o	\
=======
	psobject.o	\
	psopcode.o	\
	psopinfo.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	psparse.o	\
	psscope.o	\
	pstree.o	\
	psutils.o	\
	pswalk.o	\
	psxface.o

acpi-y +=		\
	rsaddr.o	\
	rscalc.o	\
	rscreate.o	\
<<<<<<< HEAD
=======
	rsdumpinfo.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rsinfo.o	\
	rsio.o		\
	rsirq.o		\
	rslist.o	\
	rsmemory.o	\
	rsmisc.o	\
	rsserial.o	\
	rsutils.o	\
	rsxface.o

<<<<<<< HEAD
acpi-$(ACPI_FUTURE_USAGE) += rsdump.o

acpi-y +=		\
	tbfadt.o	\
	tbfind.o	\
	tbinstal.o	\
	tbutils.o	\
	tbxface.o	\
=======
acpi-y +=		\
	tbdata.o	\
	tbfadt.o	\
	tbfind.o	\
	tbinstal.o	\
	tbprint.o	\
	tbutils.o	\
	tbxface.o	\
	tbxfload.o	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tbxfroot.o

acpi-y +=		\
	utaddress.o	\
	utalloc.o	\
<<<<<<< HEAD
	utcopy.o	\
	utdebug.o	\
	utdecode.o	\
	utdelete.o	\
	uteval.o	\
	utglobal.o	\
=======
	utascii.o	\
	utbuffer.o	\
	utcksum.o	\
	utcopy.o	\
	utexcep.o	\
	utdebug.o	\
	utdecode.o	\
	utdelete.o	\
	uterror.o	\
	uteval.o	\
	utglobal.o	\
	uthex.o		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	utids.o		\
	utinit.o	\
	utlock.o	\
	utmath.o	\
	utmisc.o	\
	utmutex.o	\
<<<<<<< HEAD
	utobject.o	\
	utosi.o		\
	utresrc.o	\
	utstate.o	\
	utxface.o	\
	utxferror.o	\
	utxfmutex.o
=======
	utnonansi.o	\
	utobject.o	\
	utosi.o		\
	utownerid.o	\
	utpredef.o	\
	utresdecode.o	\
	utresrc.o	\
	utstate.o	\
	utstring.o	\
	utstrsuppt.o	\
	utstrtoul64.o	\
	utxface.o	\
	utxfinit.o	\
	utxferror.o	\
	utxfmutex.o

acpi-$(CONFIG_ACPI_DEBUGGER) +=	\
	dbcmds.o		\
	dbconvert.o		\
	dbdisply.o		\
	dbexec.o		\
	dbhistry.o		\
	dbinput.o		\
	dbmethod.o		\
	dbnames.o		\
	dbobject.o		\
	dbstats.o		\
	dbutils.o		\
	dbxface.o		\
	rsdump.o		\

acpi-$(ACPI_FUTURE_USAGE) +=	\
	dbfileio.o		\
	dbtest.o		\
	utcache.o		\
	utprint.o		\
	uttrack.o		\
	utuuid.o

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
