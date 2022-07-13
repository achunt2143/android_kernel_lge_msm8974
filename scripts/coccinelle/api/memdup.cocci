<<<<<<< HEAD
/// Use kmemdup rather than duplicating its implementation
///
// Confidence: High
// Copyright: (C) 2010-2012 Nicolas Palix.  GPLv2.
// Copyright: (C) 2010-2012 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2010-2012 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: -no_includes -include_headers
=======
// SPDX-License-Identifier: GPL-2.0-only
/// Use kmemdup rather than duplicating its implementation
///
// Confidence: High
// Copyright: (C) 2010-2012 Nicolas Palix.
// Copyright: (C) 2010-2012 Julia Lawall, INRIA/LIP6.
// Copyright: (C) 2010-2012 Gilles Muller, INRIA/LiP6.
// URL: https://coccinelle.gitlabpages.inria.fr/website
// Comments:
// Options: --no-includes --include-headers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

virtual patch
virtual context
virtual org
virtual report

@r1@
expression from,to;
expression flag;
position p;
@@

   to = \(kmalloc@p\|kzalloc@p\)(strlen(from) + 1,flag);

@r2@
expression x,from,to;
expression flag,E1;
position p;
@@

    x = strlen(from) + 1;
    ... when != \( x = E1 \| from = E1 \)
    to = \(kmalloc@p\|kzalloc@p\)(x,flag);

@depends on patch@
expression from,to,size,flag;
position p != {r1.p,r2.p};
statement S;
@@

-  to = \(kmalloc@p\|kzalloc@p\)(size,flag);
+  to = kmemdup(from,size,flag);
   if (to==NULL || ...) S
-  memcpy(to, from, size);

@r depends on !patch@
expression from,to,size,flag;
position p != {r1.p,r2.p};
statement S;
@@

*  to = \(kmalloc@p\|kzalloc@p\)(size,flag);
<<<<<<< HEAD
   to = kmemdup(from,size,flag);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
   if (to==NULL || ...) S
*  memcpy(to, from, size);

@script:python depends on org@
p << r.p;
@@

<<<<<<< HEAD
coccilib.org.print_todo(p[0], "WARNING opportunity for kmemdep")
=======
coccilib.org.print_todo(p[0], "WARNING opportunity for kmemdup")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

@script:python depends on report@
p << r.p;
@@

<<<<<<< HEAD
coccilib.report.print_report(p[0], "WARNING opportunity for kmemdep")
=======
coccilib.report.print_report(p[0], "WARNING opportunity for kmemdup")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
