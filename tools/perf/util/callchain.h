<<<<<<< HEAD
#ifndef __PERF_CALLCHAIN_H
#define __PERF_CALLCHAIN_H

#include "../perf.h"
#include <linux/list.h>
#include <linux/rbtree.h>
#include "event.h"
#include "symbol.h"
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __PERF_CALLCHAIN_H
#define __PERF_CALLCHAIN_H

#include <linux/list.h>
#include <linux/rbtree.h>
#include "map_symbol.h"
#include "branch.h"

struct addr_location;
struct evsel;
struct ip_callchain;
struct map;
struct perf_sample;
struct thread;
struct hists;

#define HELP_PAD "\t\t\t\t"

#define CALLCHAIN_HELP "setup and enables call-graph (stack chain/backtrace):\n\n"

# define RECORD_MODE_HELP  HELP_PAD "record_mode:\tcall graph recording mode (fp|dwarf|lbr)\n"

#define RECORD_SIZE_HELP						\
	HELP_PAD "record_size:\tif record_mode is 'dwarf', max size of stack recording (<bytes>)\n" \
	HELP_PAD "\t\tdefault: 8192 (bytes)\n"

#define CALLCHAIN_RECORD_HELP  CALLCHAIN_HELP RECORD_MODE_HELP RECORD_SIZE_HELP

#define CALLCHAIN_REPORT_HELP						\
	HELP_PAD "print_type:\tcall graph printing style (graph|flat|fractal|folded|none)\n" \
	HELP_PAD "threshold:\tminimum call graph inclusion threshold (<percent>)\n" \
	HELP_PAD "print_limit:\tmaximum number of call graph entry (<number>)\n" \
	HELP_PAD "order:\t\tcall graph order (caller|callee)\n" \
	HELP_PAD "sort_key:\tcall graph sort key (function|address)\n"	\
	HELP_PAD "branch:\t\tinclude last branch info to call graph (branch)\n" \
	HELP_PAD "value:\t\tcall graph value (percent|period|count)\n"

enum perf_call_graph_mode {
	CALLCHAIN_NONE,
	CALLCHAIN_FP,
	CALLCHAIN_DWARF,
	CALLCHAIN_LBR,
	CALLCHAIN_MAX
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum chain_mode {
	CHAIN_NONE,
	CHAIN_FLAT,
	CHAIN_GRAPH_ABS,
<<<<<<< HEAD
	CHAIN_GRAPH_REL
=======
	CHAIN_GRAPH_REL,
	CHAIN_FOLDED,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum chain_order {
	ORDER_CALLER,
	ORDER_CALLEE
};

struct callchain_node {
	struct callchain_node	*parent;
<<<<<<< HEAD
	struct list_head	siblings;
	struct list_head	children;
	struct list_head	val;
	struct rb_node		rb_node; /* to sort nodes in an rbtree */
	struct rb_root		rb_root; /* sorted tree of children */
	unsigned int		val_nr;
=======
	struct list_head	val;
	struct list_head	parent_val;
	struct rb_node		rb_node_in; /* to insert nodes in an rbtree */
	struct rb_node		rb_node;    /* to sort nodes in an output tree */
	struct rb_root		rb_root_in; /* input tree of children */
	struct rb_root		rb_root;    /* sorted output tree of children */
	unsigned int		val_nr;
	unsigned int		count;
	unsigned int		children_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u64			hit;
	u64			children_hit;
};

struct callchain_root {
	u64			max_depth;
	struct callchain_node	node;
};

struct callchain_param;

typedef void (*sort_chain_func_t)(struct rb_root *, struct callchain_root *,
				 u64, struct callchain_param *);

<<<<<<< HEAD
struct callchain_param {
	enum chain_mode 	mode;
=======
enum chain_key {
	CCKEY_FUNCTION,
	CCKEY_ADDRESS,
	CCKEY_SRCLINE
};

enum chain_value {
	CCVAL_PERCENT,
	CCVAL_PERIOD,
	CCVAL_COUNT,
};

extern bool dwarf_callchain_users;

struct callchain_param {
	bool			enabled;
	enum perf_call_graph_mode record_mode;
	u32			dump_size;
	enum chain_mode 	mode;
	u16			max_stack;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32			print_limit;
	double			min_percent;
	sort_chain_func_t	sort;
	enum chain_order	order;
<<<<<<< HEAD
};

struct callchain_list {
	u64			ip;
	struct map_symbol	ms;
	struct list_head	list;
=======
	bool			order_set;
	enum chain_key		key;
	bool			branch_callstack;
	enum chain_value	value;
};

extern struct callchain_param callchain_param;
extern struct callchain_param callchain_param_default;

struct callchain_list {
	struct list_head	list;
	u64			ip;
	struct map_symbol	ms;
	const char		*srcline;
	u64			branch_count;
	u64			from_count;
	u64			cycles_count;
	u64			iter_count;
	u64			iter_cycles;
	struct branch_type_stat *brtype_stat;
	u64			predicted_count;
	u64			abort_count;
	struct /* for TUI */ {
		bool		unfolded;
		bool		has_children;
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * A callchain cursor is a single linked list that
 * let one feed a callchain progressively.
<<<<<<< HEAD
 * It keeps persitent allocated entries to minimize
=======
 * It keeps persistent allocated entries to minimize
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * allocations.
 */
struct callchain_cursor_node {
	u64				ip;
<<<<<<< HEAD
	struct map			*map;
	struct symbol			*sym;
	struct callchain_cursor_node	*next;
};

=======
	struct map_symbol		ms;
	const char			*srcline;
	/* Indicate valid cursor node for LBR stitch */
	bool				valid;

	bool				branch;
	struct branch_flags		branch_flags;
	u64				branch_from;
	int				nr_loop_iter;
	u64				iter_cycles;
	struct callchain_cursor_node	*next;
};

struct stitch_list {
	struct list_head		node;
	struct callchain_cursor_node	cursor;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct callchain_cursor {
	u64				nr;
	struct callchain_cursor_node	*first;
	struct callchain_cursor_node	**last;
	u64				pos;
	struct callchain_cursor_node	*curr;
};

static inline void callchain_init(struct callchain_root *root)
{
<<<<<<< HEAD
	INIT_LIST_HEAD(&root->node.siblings);
	INIT_LIST_HEAD(&root->node.children);
	INIT_LIST_HEAD(&root->node.val);
=======
	INIT_LIST_HEAD(&root->node.val);
	INIT_LIST_HEAD(&root->node.parent_val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	root->node.parent = NULL;
	root->node.hit = 0;
	root->node.children_hit = 0;
<<<<<<< HEAD
=======
	root->node.rb_root_in = RB_ROOT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	root->max_depth = 0;
}

static inline u64 callchain_cumul_hits(struct callchain_node *node)
{
	return node->hit + node->children_hit;
}

<<<<<<< HEAD
=======
static inline unsigned callchain_cumul_counts(struct callchain_node *node)
{
	return node->count + node->children_count;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int callchain_register_param(struct callchain_param *param);
int callchain_append(struct callchain_root *root,
		     struct callchain_cursor *cursor,
		     u64 period);

int callchain_merge(struct callchain_cursor *cursor,
		    struct callchain_root *dst, struct callchain_root *src);

<<<<<<< HEAD
struct ip_callchain;
union perf_event;

bool ip_callchain__valid(struct ip_callchain *chain,
			 const union perf_event *event);
/*
 * Initialize a cursor before adding entries inside, but keep
 * the previously allocated entries as a cache.
 */
static inline void callchain_cursor_reset(struct callchain_cursor *cursor)
{
	cursor->nr = 0;
	cursor->last = &cursor->first;
}

int callchain_cursor_append(struct callchain_cursor *cursor, u64 ip,
			    struct map *map, struct symbol *sym);
=======
void callchain_cursor_reset(struct callchain_cursor *cursor);

int callchain_cursor_append(struct callchain_cursor *cursor, u64 ip,
			    struct map_symbol *ms,
			    bool branch, struct branch_flags *flags,
			    int nr_loop_iter, u64 iter_cycles, u64 branch_from,
			    const char *srcline);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Close a cursor writing session. Initialize for the reader */
static inline void callchain_cursor_commit(struct callchain_cursor *cursor)
{
<<<<<<< HEAD
=======
	if (cursor == NULL)
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cursor->curr = cursor->first;
	cursor->pos = 0;
}

/* Cursor reading iteration helpers */
static inline struct callchain_cursor_node *
callchain_cursor_current(struct callchain_cursor *cursor)
{
<<<<<<< HEAD
	if (cursor->pos == cursor->nr)
=======
	if (cursor == NULL || cursor->pos == cursor->nr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;

	return cursor->curr;
}

static inline void callchain_cursor_advance(struct callchain_cursor *cursor)
{
	cursor->curr = cursor->curr->next;
	cursor->pos++;
}
<<<<<<< HEAD
=======

struct callchain_cursor *get_tls_callchain_cursor(void);

int callchain_cursor__copy(struct callchain_cursor *dst,
			   struct callchain_cursor *src);

struct option;
struct hist_entry;

int record_parse_callchain_opt(const struct option *opt, const char *arg, int unset);
int record_callchain_opt(const struct option *opt, const char *arg, int unset);

struct record_opts;

int record_opts__parse_callchain(struct record_opts *record,
				 struct callchain_param *callchain,
				 const char *arg, bool unset);

int sample__resolve_callchain(struct perf_sample *sample,
			      struct callchain_cursor *cursor, struct symbol **parent,
			      struct evsel *evsel, struct addr_location *al,
			      int max_stack);
int hist_entry__append_callchain(struct hist_entry *he, struct perf_sample *sample);
int fill_callchain_info(struct addr_location *al, struct callchain_cursor_node *node,
			bool hide_unresolved);

extern const char record_callchain_help[];
int parse_callchain_record(const char *arg, struct callchain_param *param);
int parse_callchain_record_opt(const char *arg, struct callchain_param *param);
int parse_callchain_report_opt(const char *arg);
int parse_callchain_top_opt(const char *arg);
int perf_callchain_config(const char *var, const char *value);

static inline void callchain_cursor_snapshot(struct callchain_cursor *dest,
					     struct callchain_cursor *src)
{
	*dest = *src;

	dest->first = src->curr;
	dest->nr -= src->pos;
}

#ifdef HAVE_SKIP_CALLCHAIN_IDX
int arch_skip_callchain_idx(struct thread *thread, struct ip_callchain *chain);
#else
static inline int arch_skip_callchain_idx(struct thread *thread __maybe_unused,
			struct ip_callchain *chain __maybe_unused)
{
	return -1;
}
#endif

void arch__add_leaf_frame_record_opts(struct record_opts *opts);

char *callchain_list__sym_name(struct callchain_list *cl,
			       char *bf, size_t bfsize, bool show_dso);
char *callchain_node__scnprintf_value(struct callchain_node *node,
				      char *bf, size_t bfsize, u64 total);
int callchain_node__fprintf_value(struct callchain_node *node,
				  FILE *fp, u64 total);

int callchain_list_counts__printf_value(struct callchain_list *clist,
					FILE *fp, char *bf, int bfsize);

void free_callchain(struct callchain_root *root);
void decay_callchain(struct callchain_root *root);
int callchain_node__make_parent_list(struct callchain_node *node);

int callchain_branch_counts(struct callchain_root *root,
			    u64 *branch_count, u64 *predicted_count,
			    u64 *abort_count, u64 *cycles_count);

void callchain_param_setup(u64 sample_type, const char *arch);

bool callchain_cnode_matched(struct callchain_node *base_cnode,
			     struct callchain_node *pair_cnode);

u64 callchain_total_hits(struct hists *hists);

s64 callchain_avg_cycles(struct callchain_node *cnode);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* __PERF_CALLCHAIN_H */
