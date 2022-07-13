<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * trace-event-scripting.  Scripting engine common and initialization code.
 *
 * Copyright (C) 2009-2010 Tom Zanussi <tzanussi@gmail.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
<<<<<<< HEAD

#include "../perf.h"
#include "util.h"
#include "trace-event.h"

struct scripting_context *scripting_context;

=======
#ifdef HAVE_LIBTRACEEVENT
#include <traceevent/event-parse.h>
#endif

#include "debug.h"
#include "trace-event.h"
#include "evsel.h"
#include <linux/zalloc.h>
#include "util/sample.h"

struct scripting_context *scripting_context;

void scripting_context__update(struct scripting_context *c,
			       union perf_event *event,
			       struct perf_sample *sample,
			       struct evsel *evsel,
			       struct addr_location *al,
			       struct addr_location *addr_al)
{
	c->event_data = sample->raw_data;
	c->pevent = NULL;
#ifdef HAVE_LIBTRACEEVENT
	if (evsel->tp_format)
		c->pevent = evsel->tp_format->tep;
#endif
	c->event = event;
	c->sample = sample;
	c->evsel = evsel;
	c->al = al;
	c->addr_al = addr_al;
}

static int flush_script_unsupported(void)
{
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int stop_script_unsupported(void)
{
	return 0;
}

<<<<<<< HEAD
static void process_event_unsupported(union perf_event *event __unused,
				      struct perf_sample *sample __unused,
				      struct perf_evsel *evsel __unused,
				      struct machine *machine __unused,
				      struct thread *thread __unused)
=======
static void process_event_unsupported(union perf_event *event __maybe_unused,
				      struct perf_sample *sample __maybe_unused,
				      struct evsel *evsel __maybe_unused,
				      struct addr_location *al __maybe_unused,
				      struct addr_location *addr_al __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

static void print_python_unsupported_msg(void)
{
	fprintf(stderr, "Python scripting not supported."
		"  Install libpython and rebuild perf to enable it.\n"
		"For example:\n  # apt-get install python-dev (ubuntu)"
		"\n  # yum install python-devel (Fedora)"
		"\n  etc.\n");
}

<<<<<<< HEAD
static int python_start_script_unsupported(const char *script __unused,
					   int argc __unused,
					   const char **argv __unused)
=======
static int python_start_script_unsupported(const char *script __maybe_unused,
					   int argc __maybe_unused,
					   const char **argv __maybe_unused,
					   struct perf_session *session __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	print_python_unsupported_msg();

	return -1;
}

<<<<<<< HEAD
static int python_generate_script_unsupported(const char *outfile __unused)
=======
static int python_generate_script_unsupported(struct tep_handle *pevent
					      __maybe_unused,
					      const char *outfile
					      __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	print_python_unsupported_msg();

	return -1;
}

struct scripting_ops python_scripting_unsupported_ops = {
	.name = "Python",
<<<<<<< HEAD
	.start_script = python_start_script_unsupported,
=======
	.dirname = "python",
	.start_script = python_start_script_unsupported,
	.flush_script = flush_script_unsupported,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.stop_script = stop_script_unsupported,
	.process_event = process_event_unsupported,
	.generate_script = python_generate_script_unsupported,
};

static void register_python_scripting(struct scripting_ops *scripting_ops)
{
<<<<<<< HEAD
	int err;
	err = script_spec_register("Python", scripting_ops);
	if (err)
		die("error registering Python script extension");

	err = script_spec_register("py", scripting_ops);
	if (err)
		die("error registering py script extension");

	scripting_context = malloc(sizeof(struct scripting_context));
}

#ifdef NO_LIBPYTHON
=======
	if (scripting_context == NULL)
		scripting_context = malloc(sizeof(*scripting_context));

       if (scripting_context == NULL ||
	   script_spec_register("Python", scripting_ops) ||
	   script_spec_register("py", scripting_ops)) {
		pr_err("Error registering Python script extension: disabling it\n");
		zfree(&scripting_context);
	}
}

#ifndef HAVE_LIBPYTHON_SUPPORT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void setup_python_scripting(void)
{
	register_python_scripting(&python_scripting_unsupported_ops);
}
#else
extern struct scripting_ops python_scripting_ops;

void setup_python_scripting(void)
{
	register_python_scripting(&python_scripting_ops);
}
#endif

<<<<<<< HEAD
=======
#ifdef HAVE_LIBTRACEEVENT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void print_perl_unsupported_msg(void)
{
	fprintf(stderr, "Perl scripting not supported."
		"  Install libperl and rebuild perf to enable it.\n"
		"For example:\n  # apt-get install libperl-dev (ubuntu)"
		"\n  # yum install 'perl(ExtUtils::Embed)' (Fedora)"
		"\n  etc.\n");
}

<<<<<<< HEAD
static int perl_start_script_unsupported(const char *script __unused,
					 int argc __unused,
					 const char **argv __unused)
=======
static int perl_start_script_unsupported(const char *script __maybe_unused,
					 int argc __maybe_unused,
					 const char **argv __maybe_unused,
					 struct perf_session *session __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	print_perl_unsupported_msg();

	return -1;
}

<<<<<<< HEAD
static int perl_generate_script_unsupported(const char *outfile __unused)
=======
static int perl_generate_script_unsupported(struct tep_handle *pevent
					    __maybe_unused,
					    const char *outfile __maybe_unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	print_perl_unsupported_msg();

	return -1;
}

struct scripting_ops perl_scripting_unsupported_ops = {
	.name = "Perl",
<<<<<<< HEAD
	.start_script = perl_start_script_unsupported,
=======
	.dirname = "perl",
	.start_script = perl_start_script_unsupported,
	.flush_script = flush_script_unsupported,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.stop_script = stop_script_unsupported,
	.process_event = process_event_unsupported,
	.generate_script = perl_generate_script_unsupported,
};

static void register_perl_scripting(struct scripting_ops *scripting_ops)
{
<<<<<<< HEAD
	int err;
	err = script_spec_register("Perl", scripting_ops);
	if (err)
		die("error registering Perl script extension");

	err = script_spec_register("pl", scripting_ops);
	if (err)
		die("error registering pl script extension");

	scripting_context = malloc(sizeof(struct scripting_context));
}

#ifdef NO_LIBPERL
=======
	if (scripting_context == NULL)
		scripting_context = malloc(sizeof(*scripting_context));

       if (scripting_context == NULL ||
	   script_spec_register("Perl", scripting_ops) ||
	   script_spec_register("pl", scripting_ops)) {
		pr_err("Error registering Perl script extension: disabling it\n");
		zfree(&scripting_context);
	}
}

#ifndef HAVE_LIBPERL_SUPPORT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void setup_perl_scripting(void)
{
	register_perl_scripting(&perl_scripting_unsupported_ops);
}
#else
extern struct scripting_ops perl_scripting_ops;

void setup_perl_scripting(void)
{
	register_perl_scripting(&perl_scripting_ops);
}
#endif
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
