<<<<<<< HEAD
/// Find &&/|| operations that include the same argument more than once
//# A common source of false positives is when the argument performs a side
//# effect.
///
// Confidence: Moderate
// Copyright: (C) 2010 Nicolas Palix, DIKU.  GPLv2.
// Copyright: (C) 2010 Julia Lawall, DIKU.  GPLv2.
// Copyright: (C) 2010 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: -no_includes -include_headers
=======
// SPDX-License-Identifier: GPL-2.0-only
/// Find &&/|| operations that include the same argument more than once
//# A common source of false positives is when the expression, or
//# another expresssion in the same && or || operation, performs a
//# side effect.
///
// Confidence: Moderate
// Copyright: (C) 2010 Nicolas Palix, DIKU.
// Copyright: (C) 2010 Julia Lawall, DIKU.
// Copyright: (C) 2010 Gilles Muller, INRIA/LiP6.
// URL: https://coccinelle.gitlabpages.inria.fr/website
// Comments:
// Options: --no-includes --include-headers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

virtual context
virtual org
virtual report

@r expression@
expression E;
position p;
@@

(
<<<<<<< HEAD
* E@p
  || ... || E
|
* E@p
  && ... && E
)

@script:python depends on org@
=======
 E@p || ... || E
|
 E@p && ... && E
)

@bad@
expression r.E,e1,e2,fn;
position r.p;
assignment operator op;
@@

(
E@p
&
 <+... \(fn(...)\|e1 op e2\|e1++\|e1--\|++e1\|--e1\) ...+>
)

@depends on context && !bad@
expression r.E;
position r.p;
@@

*E@p

@script:python depends on org && !bad@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
p << r.p;
@@

cocci.print_main("duplicated argument to && or ||",p)

<<<<<<< HEAD
@script:python depends on report@
=======
@script:python depends on report && !bad@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
p << r.p;
@@

coccilib.report.print_report(p[0],"duplicated argument to && or ||")
