<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/// Find uses of standard freeing functons on values allocated using devm_
/// functions.  Values allocated using the devm_functions are freed when
/// the device is detached, and thus the use of the standard freeing
/// function would cause a double free.
<<<<<<< HEAD
/// See Documentation/driver-model/devres.txt for more information.
=======
/// See Documentation/driver-api/driver-model/devres.rst for more information.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
///
/// A difficulty of detecting this problem is that the standard freeing
/// function might be called from a different function than the one
/// containing the allocation function.  It is thus necessary to make the
/// connection between the allocation function and the freeing function.
/// Here this is done using the specific argument text, which is prone to
/// false positives.  There is no rule for the request_region and
/// request_mem_region variants because this heuristic seems to be a bit
/// less reliable in these cases.
///
// Confidence: Moderate
<<<<<<< HEAD
// Copyright: (C) 2011 Julia Lawall, INRIA/LIP6.  GPLv2.
// Copyright: (C) 2011 Gilles Muller, INRIA/LiP6.  GPLv2.
// URL: http://coccinelle.lip6.fr/
// Comments:
// Options: -no_includes -include_headers
=======
// Copyright: (C) 2011 Julia Lawall, INRIA/LIP6.
// Copyright: (C) 2011 Gilles Muller, INRIA/LiP6.
// URL: https://coccinelle.gitlabpages.inria.fr/website
// Comments:
// Options: --no-includes --include-headers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

virtual org
virtual report
virtual context

@r depends on context || org || report@
expression x;
@@

(
<<<<<<< HEAD
 x = devm_kzalloc(...)
|
=======
 x = devm_kmalloc(...)
|
 x = devm_kvasprintf(...)
|
 x = devm_kasprintf(...)
|
 x = devm_kzalloc(...)
|
 x = devm_kmalloc_array(...)
|
 x = devm_kcalloc(...)
|
 x = devm_kstrdup(...)
|
 x = devm_kmemdup(...)
|
 x = devm_get_free_pages(...)
|
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 x = devm_request_irq(...)
|
 x = devm_ioremap(...)
|
<<<<<<< HEAD
 x = devm_ioremap_nocache(...)
|
 x = devm_ioport_map(...)
)

@pb@
expression r.x;
=======
 x = devm_ioport_map(...)
)

@safe depends on context || org || report exists@
expression x;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
position p;
@@

(
<<<<<<< HEAD
* kfree@p(x)
|
=======
 x = kmalloc(...)
|
 x = kvasprintf(...)
|
 x = kasprintf(...)
|
 x = kzalloc(...)
|
 x = kmalloc_array(...)
|
 x = kcalloc(...)
|
 x = kstrdup(...)
|
 x = kmemdup(...)
|
 x = get_free_pages(...)
|
 x = request_irq(...)
|
 x = ioremap(...)
|
 x = ioport_map(...)
)
...
(
 kfree@p(x)
|
 kfree_sensitive@p(x)
|
 krealloc@p(x, ...)
|
 free_pages@p(x, ...)
|
 free_page@p(x)
|
 free_irq@p(x)
|
 iounmap@p(x)
|
 ioport_unmap@p(x)
)

@pb@
expression r.x;
position p != safe.p;
@@

(
* kfree@p(x)
|
* kfree_sensitive@p(x)
|
* krealloc@p(x, ...)
|
* free_pages@p(x, ...)
|
* free_page@p(x)
|
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
* free_irq@p(x)
|
* iounmap@p(x)
|
* ioport_unmap@p(x)
)

@script:python depends on org@
p << pb.p;
@@

msg="WARNING: invalid free of devm_ allocated data"
coccilib.org.print_todo(p[0], msg)

@script:python depends on report@
p << pb.p;
@@

msg="WARNING: invalid free of devm_ allocated data"
coccilib.report.print_report(p[0], msg)

