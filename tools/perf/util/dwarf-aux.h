<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _DWARF_AUX_H
#define _DWARF_AUX_H
/*
 * dwarf-aux.h : libdw auxiliary interfaces
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <dwarf.h>
#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>
#include <elfutils/version.h>

<<<<<<< HEAD
/* Find the realpath of the target file */
extern const char *cu_find_realpath(Dwarf_Die *cu_die, const char *fname);

/* Get DW_AT_comp_dir (should be NULL with older gcc) */
extern const char *cu_get_comp_dir(Dwarf_Die *cu_die);

/* Get a line number and file name for given address */
extern int cu_find_lineinfo(Dwarf_Die *cudie, unsigned long addr,
			    const char **fname, int *lineno);

/* Walk on funcitons at given address */
extern int cu_walk_functions_at(Dwarf_Die *cu_die, Dwarf_Addr addr,
			int (*callback)(Dwarf_Die *, void *), void *data);

/* Compare diename and tname */
extern bool die_compare_name(Dwarf_Die *dw_die, const char *tname);

/* Get callsite line number of inline-function instance */
extern int die_get_call_lineno(Dwarf_Die *in_die);

/* Get callsite file name of inlined function instance */
extern const char *die_get_call_file(Dwarf_Die *in_die);

/* Get type die */
extern Dwarf_Die *die_get_type(Dwarf_Die *vr_die, Dwarf_Die *die_mem);

/* Get a type die, but skip qualifiers and typedef */
extern Dwarf_Die *die_get_real_type(Dwarf_Die *vr_die, Dwarf_Die *die_mem);

/* Check whether the DIE is signed or not */
extern bool die_is_signed_type(Dwarf_Die *tp_die);

/* Get data_member_location offset */
extern int die_get_data_member_location(Dwarf_Die *mb_die, Dwarf_Word *offs);
=======
struct strbuf;

/* Find the realpath of the target file */
const char *cu_find_realpath(Dwarf_Die *cu_die, const char *fname);

/* Get DW_AT_comp_dir (should be NULL with older gcc) */
const char *cu_get_comp_dir(Dwarf_Die *cu_die);

/* Get a line number and file name for given address */
int cu_find_lineinfo(Dwarf_Die *cudie, Dwarf_Addr addr,
		     const char **fname, int *lineno);

/* Walk on functions at given address */
int cu_walk_functions_at(Dwarf_Die *cu_die, Dwarf_Addr addr,
			 int (*callback)(Dwarf_Die *, void *), void *data);

/* Get DW_AT_linkage_name (should be NULL for C binary) */
const char *die_get_linkage_name(Dwarf_Die *dw_die);

/* Get the lowest PC in DIE (including range list) */
int die_entrypc(Dwarf_Die *dw_die, Dwarf_Addr *addr);

/* Ensure that this DIE is a subprogram and definition (not declaration) */
bool die_is_func_def(Dwarf_Die *dw_die);

/* Ensure that this DIE is an instance of a subprogram */
bool die_is_func_instance(Dwarf_Die *dw_die);

/* Compare diename and tname */
bool die_compare_name(Dwarf_Die *dw_die, const char *tname);

/* Matching diename with glob pattern */
bool die_match_name(Dwarf_Die *dw_die, const char *glob);

/* Get callsite line number of inline-function instance */
int die_get_call_lineno(Dwarf_Die *in_die);

/* Get callsite file name of inlined function instance */
const char *die_get_call_file(Dwarf_Die *in_die);

/* Get declared file name of a DIE */
const char *die_get_decl_file(Dwarf_Die *dw_die);

/* Get type die */
Dwarf_Die *die_get_type(Dwarf_Die *vr_die, Dwarf_Die *die_mem);

/* Get a type die, but skip qualifiers and typedef */
Dwarf_Die *die_get_real_type(Dwarf_Die *vr_die, Dwarf_Die *die_mem);

/* Check whether the DIE is signed or not */
bool die_is_signed_type(Dwarf_Die *tp_die);

/* Get data_member_location offset */
int die_get_data_member_location(Dwarf_Die *mb_die, Dwarf_Word *offs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Return values for die_find_child() callbacks */
enum {
	DIE_FIND_CB_END = 0,		/* End of Search */
	DIE_FIND_CB_CHILD = 1,		/* Search only children */
	DIE_FIND_CB_SIBLING = 2,	/* Search only siblings */
	DIE_FIND_CB_CONTINUE = 3,	/* Search children and siblings */
};

/* Search child DIEs */
<<<<<<< HEAD
extern Dwarf_Die *die_find_child(Dwarf_Die *rt_die,
				 int (*callback)(Dwarf_Die *, void *),
				 void *data, Dwarf_Die *die_mem);

/* Search a non-inlined function including given address */
extern Dwarf_Die *die_find_realfunc(Dwarf_Die *cu_die, Dwarf_Addr addr,
				    Dwarf_Die *die_mem);

/* Search an inlined function including given address */
extern Dwarf_Die *die_find_inlinefunc(Dwarf_Die *sp_die, Dwarf_Addr addr,
				      Dwarf_Die *die_mem);

/* Walk on the instances of given DIE */
extern int die_walk_instances(Dwarf_Die *in_die,
			      int (*callback)(Dwarf_Die *, void *), void *data);
=======
Dwarf_Die *die_find_child(Dwarf_Die *rt_die,
			 int (*callback)(Dwarf_Die *, void *),
			 void *data, Dwarf_Die *die_mem);

/* Search a non-inlined function including given address */
Dwarf_Die *die_find_realfunc(Dwarf_Die *cu_die, Dwarf_Addr addr,
			     Dwarf_Die *die_mem);

/* Search a non-inlined function with tail call at given address */
Dwarf_Die *die_find_tailfunc(Dwarf_Die *cu_die, Dwarf_Addr addr,
				    Dwarf_Die *die_mem);

/* Search the top inlined function including given address */
Dwarf_Die *die_find_top_inlinefunc(Dwarf_Die *sp_die, Dwarf_Addr addr,
				   Dwarf_Die *die_mem);

/* Search the deepest inlined function including given address */
Dwarf_Die *die_find_inlinefunc(Dwarf_Die *sp_die, Dwarf_Addr addr,
			       Dwarf_Die *die_mem);

/* Walk on the instances of given DIE */
int die_walk_instances(Dwarf_Die *in_die,
		       int (*callback)(Dwarf_Die *, void *), void *data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Walker on lines (Note: line number will not be sorted) */
typedef int (* line_walk_callback_t) (const char *fname, int lineno,
				      Dwarf_Addr addr, void *data);

/*
 * Walk on lines inside given DIE. If the DIE is a subprogram, walk only on
 * the lines inside the subprogram, otherwise the DIE must be a CU DIE.
 */
<<<<<<< HEAD
extern int die_walk_lines(Dwarf_Die *rt_die, line_walk_callback_t callback,
			  void *data);

/* Find a variable called 'name' at given address */
extern Dwarf_Die *die_find_variable_at(Dwarf_Die *sp_die, const char *name,
				       Dwarf_Addr addr, Dwarf_Die *die_mem);

/* Find a member called 'name' */
extern Dwarf_Die *die_find_member(Dwarf_Die *st_die, const char *name,
				  Dwarf_Die *die_mem);

/* Get the name of given variable DIE */
extern int die_get_typename(Dwarf_Die *vr_die, char *buf, int len);

/* Get the name and type of given variable DIE, stored as "type\tname" */
extern int die_get_varname(Dwarf_Die *vr_die, char *buf, int len);
#endif
=======
int die_walk_lines(Dwarf_Die *rt_die, line_walk_callback_t callback, void *data);

/* Find a variable called 'name' at given address */
Dwarf_Die *die_find_variable_at(Dwarf_Die *sp_die, const char *name,
				Dwarf_Addr addr, Dwarf_Die *die_mem);

/* Find a member called 'name' */
Dwarf_Die *die_find_member(Dwarf_Die *st_die, const char *name,
			   Dwarf_Die *die_mem);

/* Get the name of given type DIE */
int die_get_typename_from_type(Dwarf_Die *type_die, struct strbuf *buf);

/* Get the name of given variable DIE */
int die_get_typename(Dwarf_Die *vr_die, struct strbuf *buf);

/* Get the name and type of given variable DIE, stored as "type\tname" */
int die_get_varname(Dwarf_Die *vr_die, struct strbuf *buf);

/* Check if target program is compiled with optimization */
bool die_is_optimized_target(Dwarf_Die *cu_die);

/* Use next address after prologue as probe location */
void die_skip_prologue(Dwarf_Die *sp_die, Dwarf_Die *cu_die,
		       Dwarf_Addr *entrypc);

/* Get the list of including scopes */
int die_get_scopes(Dwarf_Die *cu_die, Dwarf_Addr pc, Dwarf_Die **scopes);

#ifdef HAVE_DWARF_GETLOCATIONS_SUPPORT

/* Get byte offset range of given variable DIE */
int die_get_var_range(Dwarf_Die *sp_die, Dwarf_Die *vr_die, struct strbuf *buf);

/* Find a variable saved in the 'reg' at given address */
Dwarf_Die *die_find_variable_by_reg(Dwarf_Die *sc_die, Dwarf_Addr pc, int reg,
				    int *poffset, bool is_fbreg,
				    Dwarf_Die *die_mem);

/* Find a (global) variable located in the 'addr' */
Dwarf_Die *die_find_variable_by_addr(Dwarf_Die *sc_die, Dwarf_Addr pc,
				     Dwarf_Addr addr, Dwarf_Die *die_mem,
				     int *offset);

#else /*  HAVE_DWARF_GETLOCATIONS_SUPPORT */

static inline int die_get_var_range(Dwarf_Die *sp_die __maybe_unused,
				    Dwarf_Die *vr_die __maybe_unused,
				    struct strbuf *buf __maybe_unused)
{
	return -ENOTSUP;
}

static inline Dwarf_Die *die_find_variable_by_reg(Dwarf_Die *sc_die __maybe_unused,
						  Dwarf_Addr pc __maybe_unused,
						  int reg __maybe_unused,
						  int *poffset __maybe_unused,
						  bool is_fbreg __maybe_unused,
						  Dwarf_Die *die_mem __maybe_unused)
{
	return NULL;
}

static inline Dwarf_Die *die_find_variable_by_addr(Dwarf_Die *sc_die __maybe_unused,
						   Dwarf_Addr pc __maybe_unused,
						   Dwarf_Addr addr __maybe_unused,
						   Dwarf_Die *die_mem __maybe_unused,
						   int *offset __maybe_unused)
{
	return NULL;
}

#endif /* HAVE_DWARF_GETLOCATIONS_SUPPORT */

#ifdef HAVE_DWARF_CFI_SUPPORT

/* Get the frame base information from CFA */
int die_get_cfa(Dwarf *dwarf, u64 pc, int *preg, int *poffset);

#else /* HAVE_DWARF_CFI_SUPPORT */

static inline int die_get_cfa(Dwarf *dwarf __maybe_unused, u64 pc __maybe_unused,
			      int *preg __maybe_unused, int *poffset __maybe_unused)
{
	return -1;
}

#endif /* HAVE_DWARF_CFI_SUPPORT */

#endif /* _DWARF_AUX_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
