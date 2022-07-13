<<<<<<< HEAD
/* Authors: Karl MacMillan <kmacmillan@tresys.com>
 *	    Frank Mayer <mayerf@tresys.com>
 *
 * Copyright (C) 2003 - 2004 Tresys Technology, LLC
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/* Authors: Karl MacMillan <kmacmillan@tresys.com>
 *	    Frank Mayer <mayerf@tresys.com>
 *          Copyright (C) 2003 - 2004 Tresys Technology, LLC
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/spinlock.h>
#include <linux/slab.h>

#include "security.h"
#include "conditional.h"
#include "services.h"

/*
 * cond_evaluate_expr evaluates a conditional expr
 * in reverse polish notation. It returns true (1), false (0),
 * or undefined (-1). Undefined occurs when the expression
 * exceeds the stack depth of COND_EXPR_MAXDEPTH.
 */
static int cond_evaluate_expr(struct policydb *p, struct cond_expr *expr)
{
<<<<<<< HEAD

	struct cond_expr *cur;
	int s[COND_EXPR_MAXDEPTH];
	int sp = -1;

	for (cur = expr; cur; cur = cur->next) {
		switch (cur->expr_type) {
=======
	u32 i;
	int s[COND_EXPR_MAXDEPTH];
	int sp = -1;

	if (expr->len == 0)
		return -1;

	for (i = 0; i < expr->len; i++) {
		struct cond_expr_node *node = &expr->nodes[i];

		switch (node->expr_type) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case COND_BOOL:
			if (sp == (COND_EXPR_MAXDEPTH - 1))
				return -1;
			sp++;
<<<<<<< HEAD
			s[sp] = p->bool_val_to_struct[cur->bool - 1]->state;
=======
			s[sp] = p->bool_val_to_struct[node->boolean - 1]->state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case COND_NOT:
			if (sp < 0)
				return -1;
			s[sp] = !s[sp];
			break;
		case COND_OR:
			if (sp < 1)
				return -1;
			sp--;
			s[sp] |= s[sp + 1];
			break;
		case COND_AND:
			if (sp < 1)
				return -1;
			sp--;
			s[sp] &= s[sp + 1];
			break;
		case COND_XOR:
			if (sp < 1)
				return -1;
			sp--;
			s[sp] ^= s[sp + 1];
			break;
		case COND_EQ:
			if (sp < 1)
				return -1;
			sp--;
			s[sp] = (s[sp] == s[sp + 1]);
			break;
		case COND_NEQ:
			if (sp < 1)
				return -1;
			sp--;
			s[sp] = (s[sp] != s[sp + 1]);
			break;
		default:
			return -1;
		}
	}
	return s[0];
}

/*
 * evaluate_cond_node evaluates the conditional stored in
 * a struct cond_node and if the result is different than the
 * current state of the node it sets the rules in the true/false
 * list appropriately. If the result of the expression is undefined
 * all of the rules are disabled for safety.
 */
<<<<<<< HEAD
int evaluate_cond_node(struct policydb *p, struct cond_node *node)
{
	int new_state;
	struct cond_av_list *cur;

	new_state = cond_evaluate_expr(p, node->expr);
	if (new_state != node->cur_state) {
		node->cur_state = new_state;
		if (new_state == -1)
			printk(KERN_ERR "SELinux: expression result was undefined - disabling all rules.\n");
		/* turn the rules on or off */
		for (cur = node->true_list; cur; cur = cur->next) {
			if (new_state <= 0)
				cur->node->key.specified &= ~AVTAB_ENABLED;
			else
				cur->node->key.specified |= AVTAB_ENABLED;
		}

		for (cur = node->false_list; cur; cur = cur->next) {
			/* -1 or 1 */
			if (new_state)
				cur->node->key.specified &= ~AVTAB_ENABLED;
			else
				cur->node->key.specified |= AVTAB_ENABLED;
		}
	}
	return 0;
}

int cond_policydb_init(struct policydb *p)
{
	int rc;

	p->bool_val_to_struct = NULL;
	p->cond_list = NULL;

	rc = avtab_init(&p->te_cond_avtab);
	if (rc)
		return rc;

	return 0;
}

static void cond_av_list_destroy(struct cond_av_list *list)
{
	struct cond_av_list *cur, *next;
	for (cur = list; cur; cur = next) {
		next = cur->next;
		/* the avtab_ptr_t node is destroy by the avtab */
		kfree(cur);
	}
=======
static void evaluate_cond_node(struct policydb *p, struct cond_node *node)
{
	struct avtab_node *avnode;
	int new_state;
	u32 i;

	new_state = cond_evaluate_expr(p, &node->expr);
	if (new_state != node->cur_state) {
		node->cur_state = new_state;
		if (new_state == -1)
			pr_err("SELinux: expression result was undefined - disabling all rules.\n");
		/* turn the rules on or off */
		for (i = 0; i < node->true_list.len; i++) {
			avnode = node->true_list.nodes[i];
			if (new_state <= 0)
				avnode->key.specified &= ~AVTAB_ENABLED;
			else
				avnode->key.specified |= AVTAB_ENABLED;
		}

		for (i = 0; i < node->false_list.len; i++) {
			avnode = node->false_list.nodes[i];
			/* -1 or 1 */
			if (new_state)
				avnode->key.specified &= ~AVTAB_ENABLED;
			else
				avnode->key.specified |= AVTAB_ENABLED;
		}
	}
}

void evaluate_cond_nodes(struct policydb *p)
{
	u32 i;

	for (i = 0; i < p->cond_list_len; i++)
		evaluate_cond_node(p, &p->cond_list[i]);
}

void cond_policydb_init(struct policydb *p)
{
	p->bool_val_to_struct = NULL;
	p->cond_list = NULL;
	p->cond_list_len = 0;

	avtab_init(&p->te_cond_avtab);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void cond_node_destroy(struct cond_node *node)
{
<<<<<<< HEAD
	struct cond_expr *cur_expr, *next_expr;

	for (cur_expr = node->expr; cur_expr; cur_expr = next_expr) {
		next_expr = cur_expr->next;
		kfree(cur_expr);
	}
	cond_av_list_destroy(node->true_list);
	cond_av_list_destroy(node->false_list);
	kfree(node);
}

static void cond_list_destroy(struct cond_node *list)
{
	struct cond_node *next, *cur;

	if (list == NULL)
		return;

	for (cur = list; cur; cur = next) {
		next = cur->next;
		cond_node_destroy(cur);
	}
=======
	kfree(node->expr.nodes);
	/* the avtab_ptr_t nodes are destroyed by the avtab */
	kfree(node->true_list.nodes);
	kfree(node->false_list.nodes);
}

static void cond_list_destroy(struct policydb *p)
{
	u32 i;

	for (i = 0; i < p->cond_list_len; i++)
		cond_node_destroy(&p->cond_list[i]);
	kfree(p->cond_list);
	p->cond_list = NULL;
	p->cond_list_len = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void cond_policydb_destroy(struct policydb *p)
{
	kfree(p->bool_val_to_struct);
	avtab_destroy(&p->te_cond_avtab);
<<<<<<< HEAD
	cond_list_destroy(p->cond_list);
=======
	cond_list_destroy(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cond_init_bool_indexes(struct policydb *p)
{
	kfree(p->bool_val_to_struct);
<<<<<<< HEAD
	p->bool_val_to_struct =
		kmalloc(p->p_bools.nprim * sizeof(struct cond_bool_datum *), GFP_KERNEL);
=======
	p->bool_val_to_struct = kmalloc_array(
		p->p_bools.nprim, sizeof(*p->bool_val_to_struct), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!p->bool_val_to_struct)
		return -ENOMEM;
	return 0;
}

int cond_destroy_bool(void *key, void *datum, void *p)
{
	kfree(key);
	kfree(datum);
	return 0;
}

int cond_index_bool(void *key, void *datum, void *datap)
{
	struct policydb *p;
	struct cond_bool_datum *booldatum;
<<<<<<< HEAD
	struct flex_array *fa;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	booldatum = datum;
	p = datap;

	if (!booldatum->value || booldatum->value > p->p_bools.nprim)
		return -EINVAL;

<<<<<<< HEAD
	fa = p->sym_val_to_name[SYM_BOOLS];
	if (flex_array_put_ptr(fa, booldatum->value - 1, key,
			       GFP_KERNEL | __GFP_ZERO))
		BUG();
=======
	p->sym_val_to_name[SYM_BOOLS][booldatum->value - 1] = key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p->bool_val_to_struct[booldatum->value - 1] = booldatum;

	return 0;
}

static int bool_isvalid(struct cond_bool_datum *b)
{
	if (!(b->state == 0 || b->state == 1))
		return 0;
	return 1;
}

<<<<<<< HEAD
int cond_read_bool(struct policydb *p, struct hashtab *h, void *fp)
=======
int cond_read_bool(struct policydb *p, struct symtab *s, void *fp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char *key = NULL;
	struct cond_bool_datum *booldatum;
	__le32 buf[3];
	u32 len;
	int rc;

<<<<<<< HEAD
	booldatum = kzalloc(sizeof(struct cond_bool_datum), GFP_KERNEL);
	if (!booldatum)
		return -ENOMEM;

	rc = next_entry(buf, fp, sizeof buf);
=======
	booldatum = kzalloc(sizeof(*booldatum), GFP_KERNEL);
	if (!booldatum)
		return -ENOMEM;

	rc = next_entry(buf, fp, sizeof(buf));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		goto err;

	booldatum->value = le32_to_cpu(buf[0]);
	booldatum->state = le32_to_cpu(buf[1]);

	rc = -EINVAL;
	if (!bool_isvalid(booldatum))
		goto err;

	len = le32_to_cpu(buf[2]);
<<<<<<< HEAD
=======
	if (((len == 0) || (len == (u32)-1)))
		goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = -ENOMEM;
	key = kmalloc(len + 1, GFP_KERNEL);
	if (!key)
		goto err;
	rc = next_entry(key, fp, len);
	if (rc)
		goto err;
	key[len] = '\0';
<<<<<<< HEAD
	rc = hashtab_insert(h, key, booldatum);
=======
	rc = symtab_insert(s, key, booldatum);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		goto err;

	return 0;
err:
	cond_destroy_bool(key, booldatum, NULL);
	return rc;
}

struct cond_insertf_data {
	struct policydb *p;
<<<<<<< HEAD
	struct cond_av_list *other;
	struct cond_av_list *head;
	struct cond_av_list *tail;
};

static int cond_insertf(struct avtab *a, struct avtab_key *k, struct avtab_datum *d, void *ptr)
{
	struct cond_insertf_data *data = ptr;
	struct policydb *p = data->p;
	struct cond_av_list *other = data->other, *list, *cur;
	struct avtab_node *node_ptr;
	u8 found;
	int rc = -EINVAL;
=======
	struct avtab_node **dst;
	struct cond_av_list *other;
};

static int cond_insertf(struct avtab *a, const struct avtab_key *k,
			const struct avtab_datum *d, void *ptr)
{
	struct cond_insertf_data *data = ptr;
	struct policydb *p = data->p;
	struct cond_av_list *other = data->other;
	struct avtab_node *node_ptr;
	u32 i;
	bool found;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For type rules we have to make certain there aren't any
	 * conflicting rules by searching the te_avtab and the
	 * cond_te_avtab.
	 */
	if (k->specified & AVTAB_TYPE) {
<<<<<<< HEAD
		if (avtab_search(&p->te_avtab, k)) {
			printk(KERN_ERR "SELinux: type rule already exists outside of a conditional.\n");
			goto err;
=======
		if (avtab_search_node(&p->te_avtab, k)) {
			pr_err("SELinux: type rule already exists outside of a conditional.\n");
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		/*
		 * If we are reading the false list other will be a pointer to
		 * the true list. We can have duplicate entries if there is only
		 * 1 other entry and it is in our true list.
		 *
		 * If we are reading the true list (other == NULL) there shouldn't
		 * be any other entries.
		 */
		if (other) {
			node_ptr = avtab_search_node(&p->te_cond_avtab, k);
			if (node_ptr) {
<<<<<<< HEAD
				if (avtab_search_node_next(node_ptr, k->specified)) {
					printk(KERN_ERR "SELinux: too many conflicting type rules.\n");
					goto err;
				}
				found = 0;
				for (cur = other; cur; cur = cur->next) {
					if (cur->node == node_ptr) {
						found = 1;
=======
				if (avtab_search_node_next(node_ptr,
							   k->specified)) {
					pr_err("SELinux: too many conflicting type rules.\n");
					return -EINVAL;
				}
				found = false;
				for (i = 0; i < other->len; i++) {
					if (other->nodes[i] == node_ptr) {
						found = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						break;
					}
				}
				if (!found) {
<<<<<<< HEAD
					printk(KERN_ERR "SELinux: conflicting type rules.\n");
					goto err;
				}
			}
		} else {
			if (avtab_search(&p->te_cond_avtab, k)) {
				printk(KERN_ERR "SELinux: conflicting type rules when adding type rule for true.\n");
				goto err;
=======
					pr_err("SELinux: conflicting type rules.\n");
					return -EINVAL;
				}
			}
		} else {
			if (avtab_search_node(&p->te_cond_avtab, k)) {
				pr_err("SELinux: conflicting type rules when adding type rule for true.\n");
				return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
	}

	node_ptr = avtab_insert_nonunique(&p->te_cond_avtab, k, d);
	if (!node_ptr) {
<<<<<<< HEAD
		printk(KERN_ERR "SELinux: could not insert rule.\n");
		rc = -ENOMEM;
		goto err;
	}

	list = kzalloc(sizeof(struct cond_av_list), GFP_KERNEL);
	if (!list) {
		rc = -ENOMEM;
		goto err;
	}

	list->node = node_ptr;
	if (!data->head)
		data->head = list;
	else
		data->tail->next = list;
	data->tail = list;
	return 0;

err:
	cond_av_list_destroy(data->head);
	data->head = NULL;
	return rc;
}

static int cond_read_av_list(struct policydb *p, void *fp, struct cond_av_list **ret_list, struct cond_av_list *other)
{
	int i, rc;
	__le32 buf[1];
	u32 len;
	struct cond_insertf_data data;

	*ret_list = NULL;

	len = 0;
=======
		pr_err("SELinux: could not insert rule.\n");
		return -ENOMEM;
	}

	*data->dst = node_ptr;
	return 0;
}

static int cond_read_av_list(struct policydb *p, void *fp,
			     struct cond_av_list *list,
			     struct cond_av_list *other)
{
	int rc;
	__le32 buf[1];
	u32 i, len;
	struct cond_insertf_data data;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = next_entry(buf, fp, sizeof(u32));
	if (rc)
		return rc;

	len = le32_to_cpu(buf[0]);
	if (len == 0)
		return 0;

<<<<<<< HEAD
	data.p = p;
	data.other = other;
	data.head = NULL;
	data.tail = NULL;
	for (i = 0; i < len; i++) {
		rc = avtab_read_item(&p->te_cond_avtab, fp, p, cond_insertf,
				     &data);
		if (rc)
			return rc;
	}

	*ret_list = data.head;
	return 0;
}

static int expr_isvalid(struct policydb *p, struct cond_expr *expr)
{
	if (expr->expr_type <= 0 || expr->expr_type > COND_LAST) {
		printk(KERN_ERR "SELinux: conditional expressions uses unknown operator.\n");
		return 0;
	}

	if (expr->bool > p->p_bools.nprim) {
		printk(KERN_ERR "SELinux: conditional expressions uses unknown bool.\n");
=======
	list->nodes = kcalloc(len, sizeof(*list->nodes), GFP_KERNEL);
	if (!list->nodes)
		return -ENOMEM;

	data.p = p;
	data.other = other;
	for (i = 0; i < len; i++) {
		data.dst = &list->nodes[i];
		rc = avtab_read_item(&p->te_cond_avtab, fp, p, cond_insertf,
				     &data);
		if (rc) {
			kfree(list->nodes);
			list->nodes = NULL;
			return rc;
		}
	}

	list->len = len;
	return 0;
}

static int expr_node_isvalid(struct policydb *p, struct cond_expr_node *expr)
{
	if (expr->expr_type <= 0 || expr->expr_type > COND_LAST) {
		pr_err("SELinux: conditional expressions uses unknown operator.\n");
		return 0;
	}

	if (expr->boolean > p->p_bools.nprim) {
		pr_err("SELinux: conditional expressions uses unknown bool.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	return 1;
}

static int cond_read_node(struct policydb *p, struct cond_node *node, void *fp)
{
	__le32 buf[2];
<<<<<<< HEAD
	u32 len, i;
	int rc;
	struct cond_expr *expr = NULL, *last = NULL;

	rc = next_entry(buf, fp, sizeof(u32));
=======
	u32 i, len;
	int rc;

	rc = next_entry(buf, fp, sizeof(u32) * 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	node->cur_state = le32_to_cpu(buf[0]);

<<<<<<< HEAD
	len = 0;
	rc = next_entry(buf, fp, sizeof(u32));
	if (rc)
		return rc;

	/* expr */
	len = le32_to_cpu(buf[0]);

	for (i = 0; i < len; i++) {
		rc = next_entry(buf, fp, sizeof(u32) * 2);
		if (rc)
			goto err;

		rc = -ENOMEM;
		expr = kzalloc(sizeof(struct cond_expr), GFP_KERNEL);
		if (!expr)
			goto err;

		expr->expr_type = le32_to_cpu(buf[0]);
		expr->bool = le32_to_cpu(buf[1]);

		if (!expr_isvalid(p, expr)) {
			rc = -EINVAL;
			kfree(expr);
			goto err;
		}

		if (i == 0)
			node->expr = expr;
		else
			last->next = expr;
		last = expr;
=======
	/* expr */
	len = le32_to_cpu(buf[1]);
	node->expr.nodes = kcalloc(len, sizeof(*node->expr.nodes), GFP_KERNEL);
	if (!node->expr.nodes)
		return -ENOMEM;

	node->expr.len = len;

	for (i = 0; i < len; i++) {
		struct cond_expr_node *expr = &node->expr.nodes[i];

		rc = next_entry(buf, fp, sizeof(u32) * 2);
		if (rc)
			return rc;

		expr->expr_type = le32_to_cpu(buf[0]);
		expr->boolean = le32_to_cpu(buf[1]);

		if (!expr_node_isvalid(p, expr))
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rc = cond_read_av_list(p, fp, &node->true_list, NULL);
	if (rc)
<<<<<<< HEAD
		goto err;
	rc = cond_read_av_list(p, fp, &node->false_list, node->true_list);
	if (rc)
		goto err;
	return 0;
err:
	cond_node_destroy(node);
	return rc;
=======
		return rc;
	return cond_read_av_list(p, fp, &node->false_list, &node->true_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int cond_read_list(struct policydb *p, void *fp)
{
<<<<<<< HEAD
	struct cond_node *node, *last = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__le32 buf[1];
	u32 i, len;
	int rc;

<<<<<<< HEAD
	rc = next_entry(buf, fp, sizeof buf);
=======
	rc = next_entry(buf, fp, sizeof(buf));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	len = le32_to_cpu(buf[0]);

<<<<<<< HEAD
=======
	p->cond_list = kcalloc(len, sizeof(*p->cond_list), GFP_KERNEL);
	if (!p->cond_list)
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = avtab_alloc(&(p->te_cond_avtab), p->te_avtab.nel);
	if (rc)
		goto err;

<<<<<<< HEAD
	for (i = 0; i < len; i++) {
		rc = -ENOMEM;
		node = kzalloc(sizeof(struct cond_node), GFP_KERNEL);
		if (!node)
			goto err;

		rc = cond_read_node(p, node, fp);
		if (rc)
			goto err;

		if (i == 0)
			p->cond_list = node;
		else
			last->next = node;
		last = node;
	}
	return 0;
err:
	cond_list_destroy(p->cond_list);
	p->cond_list = NULL;
=======
	p->cond_list_len = len;

	for (i = 0; i < len; i++) {
		rc = cond_read_node(p, &p->cond_list[i], fp);
		if (rc)
			goto err;
	}
	return 0;
err:
	cond_list_destroy(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

int cond_write_bool(void *vkey, void *datum, void *ptr)
{
	char *key = vkey;
	struct cond_bool_datum *booldatum = datum;
	struct policy_data *pd = ptr;
	void *fp = pd->fp;
	__le32 buf[3];
	u32 len;
	int rc;

	len = strlen(key);
	buf[0] = cpu_to_le32(booldatum->value);
	buf[1] = cpu_to_le32(booldatum->state);
	buf[2] = cpu_to_le32(len);
	rc = put_entry(buf, sizeof(u32), 3, fp);
	if (rc)
		return rc;
	rc = put_entry(key, 1, len, fp);
	if (rc)
		return rc;
	return 0;
}

/*
 * cond_write_cond_av_list doesn't write out the av_list nodes.
 * Instead it writes out the key/value pairs from the avtab. This
 * is necessary because there is no way to uniquely identifying rules
 * in the avtab so it is not possible to associate individual rules
 * in the avtab with a conditional without saving them as part of
 * the conditional. This means that the avtab with the conditional
 * rules will not be saved but will be rebuilt on policy load.
 */
<<<<<<< HEAD
static int cond_write_av_list(struct policydb *p,
			      struct cond_av_list *list, struct policy_file *fp)
{
	__le32 buf[1];
	struct cond_av_list *cur_list;
	u32 len;
	int rc;

	len = 0;
	for (cur_list = list; cur_list != NULL; cur_list = cur_list->next)
		len++;

	buf[0] = cpu_to_le32(len);
=======
static int cond_write_av_list(struct policydb *p, struct cond_av_list *list,
			      struct policy_file *fp)
{
	__le32 buf[1];
	u32 i;
	int rc;

	buf[0] = cpu_to_le32(list->len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = put_entry(buf, sizeof(u32), 1, fp);
	if (rc)
		return rc;

<<<<<<< HEAD
	if (len == 0)
		return 0;

	for (cur_list = list; cur_list != NULL; cur_list = cur_list->next) {
		rc = avtab_write_item(p, cur_list->node, fp);
=======
	for (i = 0; i < list->len; i++) {
		rc = avtab_write_item(p, list->nodes[i], fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			return rc;
	}

	return 0;
}

static int cond_write_node(struct policydb *p, struct cond_node *node,
<<<<<<< HEAD
		    struct policy_file *fp)
{
	struct cond_expr *cur_expr;
	__le32 buf[2];
	int rc;
	u32 len = 0;
=======
			   struct policy_file *fp)
{
	__le32 buf[2];
	int rc;
	u32 i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	buf[0] = cpu_to_le32(node->cur_state);
	rc = put_entry(buf, sizeof(u32), 1, fp);
	if (rc)
		return rc;

<<<<<<< HEAD
	for (cur_expr = node->expr; cur_expr != NULL; cur_expr = cur_expr->next)
		len++;

	buf[0] = cpu_to_le32(len);
=======
	buf[0] = cpu_to_le32(node->expr.len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = put_entry(buf, sizeof(u32), 1, fp);
	if (rc)
		return rc;

<<<<<<< HEAD
	for (cur_expr = node->expr; cur_expr != NULL; cur_expr = cur_expr->next) {
		buf[0] = cpu_to_le32(cur_expr->expr_type);
		buf[1] = cpu_to_le32(cur_expr->bool);
=======
	for (i = 0; i < node->expr.len; i++) {
		buf[0] = cpu_to_le32(node->expr.nodes[i].expr_type);
		buf[1] = cpu_to_le32(node->expr.nodes[i].boolean);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = put_entry(buf, sizeof(u32), 2, fp);
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	rc = cond_write_av_list(p, node->true_list, fp);
	if (rc)
		return rc;
	rc = cond_write_av_list(p, node->false_list, fp);
=======
	rc = cond_write_av_list(p, &node->true_list, fp);
	if (rc)
		return rc;
	rc = cond_write_av_list(p, &node->false_list, fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	return 0;
}

<<<<<<< HEAD
int cond_write_list(struct policydb *p, struct cond_node *list, void *fp)
{
	struct cond_node *cur;
	u32 len;
	__le32 buf[1];
	int rc;

	len = 0;
	for (cur = list; cur != NULL; cur = cur->next)
		len++;
	buf[0] = cpu_to_le32(len);
=======
int cond_write_list(struct policydb *p, void *fp)
{
	u32 i;
	__le32 buf[1];
	int rc;

	buf[0] = cpu_to_le32(p->cond_list_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = put_entry(buf, sizeof(u32), 1, fp);
	if (rc)
		return rc;

<<<<<<< HEAD
	for (cur = list; cur != NULL; cur = cur->next) {
		rc = cond_write_node(p, cur, fp);
=======
	for (i = 0; i < p->cond_list_len; i++) {
		rc = cond_write_node(p, &p->cond_list[i], fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			return rc;
	}

	return 0;
}

void cond_compute_xperms(struct avtab *ctab, struct avtab_key *key,
<<<<<<< HEAD
		struct extended_perms_decision *xpermd)
=======
			 struct extended_perms_decision *xpermd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct avtab_node *node;

	if (!ctab || !key || !xpermd)
		return;

	for (node = avtab_search_node(ctab, key); node;
<<<<<<< HEAD
			node = avtab_search_node_next(node, key->specified)) {
		if (node->key.specified & AVTAB_ENABLED)
			services_compute_xperms_decision(xpermd, node);
	}
	return;

=======
	     node = avtab_search_node_next(node, key->specified)) {
		if (node->key.specified & AVTAB_ENABLED)
			services_compute_xperms_decision(xpermd, node);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
/* Determine whether additional permissions are granted by the conditional
 * av table, and if so, add them to the result
 */
void cond_compute_av(struct avtab *ctab, struct avtab_key *key,
<<<<<<< HEAD
		struct av_decision *avd, struct extended_perms *xperms)
=======
		     struct av_decision *avd, struct extended_perms *xperms)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct avtab_node *node;

	if (!ctab || !key || !avd)
		return;

	for (node = avtab_search_node(ctab, key); node;
<<<<<<< HEAD
				node = avtab_search_node_next(node, key->specified)) {
		if ((u16)(AVTAB_ALLOWED|AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_ALLOWED|AVTAB_ENABLED)))
			avd->allowed |= node->datum.u.data;
		if ((u16)(AVTAB_AUDITDENY|AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_AUDITDENY|AVTAB_ENABLED)))
=======
	     node = avtab_search_node_next(node, key->specified)) {
		if ((u16)(AVTAB_ALLOWED | AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_ALLOWED | AVTAB_ENABLED)))
			avd->allowed |= node->datum.u.data;
		if ((u16)(AVTAB_AUDITDENY | AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_AUDITDENY | AVTAB_ENABLED)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* Since a '0' in an auditdeny mask represents a
			 * permission we do NOT want to audit (dontaudit), we use
			 * the '&' operand to ensure that all '0's in the mask
			 * are retained (much unlike the allow and auditallow cases).
			 */
			avd->auditdeny &= node->datum.u.data;
<<<<<<< HEAD
		if ((u16)(AVTAB_AUDITALLOW|AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_AUDITALLOW|AVTAB_ENABLED)))
			avd->auditallow |= node->datum.u.data;
		if (xperms && (node->key.specified & AVTAB_ENABLED) &&
				(node->key.specified & AVTAB_XPERMS))
			services_compute_xperms_drivers(xperms, node);
	}
	return;
=======
		if ((u16)(AVTAB_AUDITALLOW | AVTAB_ENABLED) ==
		    (node->key.specified & (AVTAB_AUDITALLOW | AVTAB_ENABLED)))
			avd->auditallow |= node->datum.u.data;
		if (xperms && (node->key.specified & AVTAB_ENABLED) &&
		    (node->key.specified & AVTAB_XPERMS))
			services_compute_xperms_drivers(xperms, node);
	}
}

static int cond_dup_av_list(struct cond_av_list *new, struct cond_av_list *orig,
			    struct avtab *avtab)
{
	u32 i;

	memset(new, 0, sizeof(*new));

	new->nodes = kcalloc(orig->len, sizeof(*new->nodes), GFP_KERNEL);
	if (!new->nodes)
		return -ENOMEM;

	for (i = 0; i < orig->len; i++) {
		new->nodes[i] = avtab_insert_nonunique(
			avtab, &orig->nodes[i]->key, &orig->nodes[i]->datum);
		if (!new->nodes[i])
			return -ENOMEM;
		new->len++;
	}

	return 0;
}

static int duplicate_policydb_cond_list(struct policydb *newp,
					struct policydb *origp)
{
	int rc;
	u32 i;

	rc = avtab_alloc_dup(&newp->te_cond_avtab, &origp->te_cond_avtab);
	if (rc)
		return rc;

	newp->cond_list_len = 0;
	newp->cond_list = kcalloc(origp->cond_list_len,
				  sizeof(*newp->cond_list), GFP_KERNEL);
	if (!newp->cond_list)
		goto error;

	for (i = 0; i < origp->cond_list_len; i++) {
		struct cond_node *newn = &newp->cond_list[i];
		struct cond_node *orign = &origp->cond_list[i];

		newp->cond_list_len++;

		newn->cur_state = orign->cur_state;
		newn->expr.nodes =
			kmemdup(orign->expr.nodes,
				orign->expr.len * sizeof(*orign->expr.nodes),
				GFP_KERNEL);
		if (!newn->expr.nodes)
			goto error;

		newn->expr.len = orign->expr.len;

		rc = cond_dup_av_list(&newn->true_list, &orign->true_list,
				      &newp->te_cond_avtab);
		if (rc)
			goto error;

		rc = cond_dup_av_list(&newn->false_list, &orign->false_list,
				      &newp->te_cond_avtab);
		if (rc)
			goto error;
	}

	return 0;

error:
	avtab_destroy(&newp->te_cond_avtab);
	cond_list_destroy(newp);
	return -ENOMEM;
}

static int cond_bools_destroy(void *key, void *datum, void *args)
{
	/* key was not copied so no need to free here */
	kfree(datum);
	return 0;
}

static int cond_bools_copy(struct hashtab_node *new, struct hashtab_node *orig,
			   void *args)
{
	struct cond_bool_datum *datum;

	datum = kmemdup(orig->datum, sizeof(struct cond_bool_datum),
			GFP_KERNEL);
	if (!datum)
		return -ENOMEM;

	new->key = orig->key; /* No need to copy, never modified */
	new->datum = datum;
	return 0;
}

static int cond_bools_index(void *key, void *datum, void *args)
{
	struct cond_bool_datum *booldatum, **cond_bool_array;

	booldatum = datum;
	cond_bool_array = args;
	cond_bool_array[booldatum->value - 1] = booldatum;

	return 0;
}

static int duplicate_policydb_bools(struct policydb *newdb,
				    struct policydb *orig)
{
	struct cond_bool_datum **cond_bool_array;
	int rc;

	cond_bool_array = kmalloc_array(orig->p_bools.nprim,
					sizeof(*orig->bool_val_to_struct),
					GFP_KERNEL);
	if (!cond_bool_array)
		return -ENOMEM;

	rc = hashtab_duplicate(&newdb->p_bools.table, &orig->p_bools.table,
			       cond_bools_copy, cond_bools_destroy, NULL);
	if (rc) {
		kfree(cond_bool_array);
		return -ENOMEM;
	}

	hashtab_map(&newdb->p_bools.table, cond_bools_index, cond_bool_array);
	newdb->bool_val_to_struct = cond_bool_array;

	newdb->p_bools.nprim = orig->p_bools.nprim;

	return 0;
}

void cond_policydb_destroy_dup(struct policydb *p)
{
	hashtab_map(&p->p_bools.table, cond_bools_destroy, NULL);
	hashtab_destroy(&p->p_bools.table);
	cond_policydb_destroy(p);
}

int cond_policydb_dup(struct policydb *new, struct policydb *orig)
{
	cond_policydb_init(new);

	if (duplicate_policydb_bools(new, orig))
		return -ENOMEM;

	if (duplicate_policydb_cond_list(new, orig)) {
		cond_policydb_destroy_dup(new);
		return -ENOMEM;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
