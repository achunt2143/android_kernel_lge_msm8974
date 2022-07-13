%option prefix="perf_pmu_"
<<<<<<< HEAD
=======
%option reentrant
%option bison-bridge
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

%{
#include <stdlib.h>
#include <linux/bitops.h>
#include "pmu.h"
#include "pmu-bison.h"

<<<<<<< HEAD
static int value(int base)
{
	long num;

	errno = 0;
	num = strtoul(perf_pmu_text, NULL, base);
	if (errno)
		return PP_ERROR;

	perf_pmu_lval.num = num;
=======
char *perf_pmu_get_text(yyscan_t yyscanner);
YYSTYPE *perf_pmu_get_lval(yyscan_t yyscanner);

static int value(yyscan_t scanner, int base)
{
	YYSTYPE *yylval = perf_pmu_get_lval(scanner);
	char *text = perf_pmu_get_text(scanner);
	long num;

	errno = 0;
	num = strtoul(text, NULL, base);
	if (errno)
		return PP_ERROR;

	yylval->num = num;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return PP_VALUE;
}

%}

num_dec         [0-9]+

%%

<<<<<<< HEAD
{num_dec}	{ return value(10); }
config		{ return PP_CONFIG; }
config1		{ return PP_CONFIG1; }
config2		{ return PP_CONFIG2; }
=======
{num_dec}	{ return value(yyscanner, 10); }
config		{ return PP_CONFIG; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
-		{ return '-'; }
:		{ return ':'; }
,		{ return ','; }
.		{ ; }
\n		{ ; }

%%

<<<<<<< HEAD
int perf_pmu_wrap(void)
=======
int perf_pmu_wrap(void *scanner __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 1;
}
