<<<<<<< HEAD
#include <stdarg.h>

/* confdata.c */
P(conf_parse,void,(const char *name));
P(conf_read,int,(const char *name));
P(conf_read_simple,int,(const char *name, int));
P(conf_write_defconfig,int,(const char *name));
P(conf_write,int,(const char *name));
P(conf_write_autoconf,int,(void));
P(conf_get_changed,bool,(void));
P(conf_set_changed_callback, void,(void (*fn)(void)));
P(conf_set_message_callback, void,(void (*fn)(const char *fmt, va_list ap)));

/* menu.c */
P(rootmenu,struct menu,);

P(menu_is_visible, bool, (struct menu *menu));
P(menu_has_prompt, bool, (struct menu *menu));
P(menu_get_prompt,const char *,(struct menu *menu));
P(menu_get_root_menu,struct menu *,(struct menu *menu));
P(menu_get_parent_menu,struct menu *,(struct menu *menu));
P(menu_has_help,bool,(struct menu *menu));
P(menu_get_help,const char *,(struct menu *menu));
P(get_symbol_str, void, (struct gstr *r, struct symbol *sym));
P(get_relations_str, struct gstr, (struct symbol **sym_arr));
P(menu_get_ext_help,void,(struct menu *menu, struct gstr *help));

/* symbol.c */
P(symbol_hash,struct symbol *,[SYMBOL_HASHSIZE]);

P(sym_lookup,struct symbol *,(const char *name, int flags));
P(sym_find,struct symbol *,(const char *name));
P(sym_expand_string_value,const char *,(const char *in));
P(sym_escape_string_value, const char *,(const char *in));
P(sym_re_search,struct symbol **,(const char *pattern));
P(sym_type_name,const char *,(enum symbol_type type));
P(sym_calc_value,void,(struct symbol *sym));
P(sym_get_type,enum symbol_type,(struct symbol *sym));
P(sym_tristate_within_range,bool,(struct symbol *sym,tristate tri));
P(sym_set_tristate_value,bool,(struct symbol *sym,tristate tri));
P(sym_toggle_tristate_value,tristate,(struct symbol *sym));
P(sym_string_valid,bool,(struct symbol *sym, const char *newval));
P(sym_string_within_range,bool,(struct symbol *sym, const char *str));
P(sym_set_string_value,bool,(struct symbol *sym, const char *newval));
P(sym_is_changable,bool,(struct symbol *sym));
P(sym_get_choice_prop,struct property *,(struct symbol *sym));
P(sym_get_default_prop,struct property *,(struct symbol *sym));
P(sym_get_string_value,const char *,(struct symbol *sym));

P(prop_get_type_name,const char *,(enum prop_type type));

/* expr.c */
P(expr_compare_type,int,(enum expr_type t1, enum expr_type t2));
P(expr_print,void,(struct expr *e, void (*fn)(void *, struct symbol *, const char *), void *data, int prevtoken));
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LKC_PROTO_H
#define LKC_PROTO_H

#include <stdarg.h>

/* confdata.c */
void conf_parse(const char *name);
int conf_read(const char *name);
int conf_read_simple(const char *name, int);
int conf_write_defconfig(const char *name);
int conf_write(const char *name);
int conf_write_autoconf(int overwrite);
void conf_set_changed(bool val);
bool conf_get_changed(void);
void conf_set_changed_callback(void (*fn)(void));
void conf_set_message_callback(void (*fn)(const char *s));
bool conf_errors(void);

/* symbol.c */
struct symbol * sym_lookup(const char *name, int flags);
struct symbol * sym_find(const char *name);
void print_symbol_for_listconfig(struct symbol *sym);
struct symbol ** sym_re_search(const char *pattern);
const char * sym_type_name(enum symbol_type type);
void sym_calc_value(struct symbol *sym);
bool sym_dep_errors(void);
enum symbol_type sym_get_type(struct symbol *sym);
bool sym_tristate_within_range(struct symbol *sym,tristate tri);
bool sym_set_tristate_value(struct symbol *sym,tristate tri);
tristate sym_toggle_tristate_value(struct symbol *sym);
bool sym_string_valid(struct symbol *sym, const char *newval);
bool sym_string_within_range(struct symbol *sym, const char *str);
bool sym_set_string_value(struct symbol *sym, const char *newval);
bool sym_is_changeable(struct symbol *sym);
struct property * sym_get_choice_prop(struct symbol *sym);
const char * sym_get_string_value(struct symbol *sym);

const char * prop_get_type_name(enum prop_type type);

/* expr.c */
void expr_print(struct expr *e, void (*fn)(void *, struct symbol *, const char *), void *data, int prevtoken);

#endif /* LKC_PROTO_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
