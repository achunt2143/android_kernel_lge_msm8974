<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Create default crypto algorithm instances.
 *
 * Copyright (c) 2006 Herbert Xu <herbert@gondor.apana.org.au>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 */

#include <crypto/internal/aead.h>
=======
 */

#include <crypto/internal/aead.h>
#include <linux/completion.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ctype.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/rtnetlink.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/sched/signal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/string.h>

#include "internal.h"

struct cryptomgr_param {
	struct rtattr *tb[CRYPTO_MAX_ATTRS + 2];

	struct {
		struct rtattr attr;
		struct crypto_attr_type data;
	} type;

<<<<<<< HEAD
	union {
		struct rtattr attr;
		struct {
			struct rtattr attr;
			struct crypto_attr_alg data;
		} alg;
		struct {
			struct rtattr attr;
			struct crypto_attr_u32 data;
		} nu32;
	} attrs[CRYPTO_MAX_ATTRS];

	char larval[CRYPTO_MAX_ALG_NAME];
	char template[CRYPTO_MAX_ALG_NAME];

=======
	struct {
		struct rtattr attr;
		struct crypto_attr_alg data;
	} attrs[CRYPTO_MAX_ATTRS];

	char template[CRYPTO_MAX_ALG_NAME];

	struct crypto_larval *larval;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 otype;
	u32 omask;
};

struct crypto_test_param {
	char driver[CRYPTO_MAX_ALG_NAME];
	char alg[CRYPTO_MAX_ALG_NAME];
	u32 type;
};

static int cryptomgr_probe(void *data)
{
	struct cryptomgr_param *param = data;
	struct crypto_template *tmpl;
<<<<<<< HEAD
	struct crypto_instance *inst;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	tmpl = crypto_lookup_template(param->template);
	if (!tmpl)
<<<<<<< HEAD
		goto err;

	do {
		if (tmpl->create) {
			err = tmpl->create(tmpl, param->tb);
			continue;
		}

		inst = tmpl->alloc(param->tb);
		if (IS_ERR(inst))
			err = PTR_ERR(inst);
		else if ((err = crypto_register_instance(tmpl, inst)))
			tmpl->free(inst);
=======
		goto out;

	do {
		err = tmpl->create(tmpl, param->tb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (err == -EAGAIN && !signal_pending(current));

	crypto_tmpl_put(tmpl);

<<<<<<< HEAD
	if (err)
		goto err;

out:
	kfree(param);
	module_put_and_exit(0);

err:
	crypto_larval_error(param->larval, param->otype, param->omask);
	goto out;
=======
out:
	complete_all(&param->larval->completion);
	crypto_alg_put(&param->larval->alg);
	kfree(param);
	module_put_and_kthread_exit(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cryptomgr_schedule_probe(struct crypto_larval *larval)
{
	struct task_struct *thread;
	struct cryptomgr_param *param;
	const char *name = larval->alg.cra_name;
	const char *p;
	unsigned int len;
	int i;

	if (!try_module_get(THIS_MODULE))
		goto err;

	param = kzalloc(sizeof(*param), GFP_KERNEL);
	if (!param)
		goto err_put_module;

	for (p = name; isalnum(*p) || *p == '-' || *p == '_'; p++)
		;

	len = p - name;
	if (!len || *p != '(')
		goto err_free_param;

	memcpy(param->template, name, len);

	i = 0;
	for (;;) {
<<<<<<< HEAD
		int notnum = 0;

		name = ++p;
		len = 0;

		for (; isalnum(*p) || *p == '-' || *p == '_'; p++)
			notnum |= !isdigit(*p);
=======
		name = ++p;

		for (; isalnum(*p) || *p == '-' || *p == '_'; p++)
			;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (*p == '(') {
			int recursion = 0;

			for (;;) {
				if (!*++p)
					goto err_free_param;
				if (*p == '(')
					recursion++;
				else if (*p == ')' && !recursion--)
					break;
			}

<<<<<<< HEAD
			notnum = 1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			p++;
		}

		len = p - name;
		if (!len)
			goto err_free_param;

<<<<<<< HEAD
		if (notnum) {
			param->attrs[i].alg.attr.rta_len =
				sizeof(param->attrs[i].alg);
			param->attrs[i].alg.attr.rta_type = CRYPTOA_ALG;
			memcpy(param->attrs[i].alg.data.name, name, len);
		} else {
			param->attrs[i].nu32.attr.rta_len =
				sizeof(param->attrs[i].nu32);
			param->attrs[i].nu32.attr.rta_type = CRYPTOA_U32;
			param->attrs[i].nu32.data.num =
				simple_strtol(name, NULL, 0);
		}
=======
		param->attrs[i].attr.rta_len = sizeof(param->attrs[i]);
		param->attrs[i].attr.rta_type = CRYPTOA_ALG;
		memcpy(param->attrs[i].data.name, name, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		param->tb[i + 1] = &param->attrs[i].attr;
		i++;

		if (i >= CRYPTO_MAX_ATTRS)
			goto err_free_param;

		if (*p == ')')
			break;

		if (*p != ',')
			goto err_free_param;
	}

	if (!i)
		goto err_free_param;

	param->tb[i + 1] = NULL;

	param->type.attr.rta_len = sizeof(param->type);
	param->type.attr.rta_type = CRYPTOA_TYPE;
	param->type.data.type = larval->alg.cra_flags & ~CRYPTO_ALG_TESTED;
	param->type.data.mask = larval->mask & ~CRYPTO_ALG_TESTED;
	param->tb[0] = &param->type.attr;

	param->otype = larval->alg.cra_flags;
	param->omask = larval->mask;

<<<<<<< HEAD
	memcpy(param->larval, larval->alg.cra_name, CRYPTO_MAX_ALG_NAME);

	thread = kthread_run(cryptomgr_probe, param, "cryptomgr_probe");
	if (IS_ERR(thread))
		goto err_free_param;

	return NOTIFY_STOP;

=======
	crypto_alg_get(&larval->alg);
	param->larval = larval;

	thread = kthread_run(cryptomgr_probe, param, "cryptomgr_probe");
	if (IS_ERR(thread))
		goto err_put_larval;

	return NOTIFY_STOP;

err_put_larval:
	crypto_alg_put(&larval->alg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_free_param:
	kfree(param);
err_put_module:
	module_put(THIS_MODULE);
err:
	return NOTIFY_OK;
}

static int cryptomgr_test(void *data)
{
	struct crypto_test_param *param = data;
	u32 type = param->type;
<<<<<<< HEAD
	int err = 0;

#ifdef CONFIG_CRYPTO_MANAGER_DISABLE_TESTS
	goto skiptest;
#endif
#ifdef CONFIG_CRYPTO_FIPS
	if (!get_cc_mode_state())
		goto skiptest;
#endif
	if (type & CRYPTO_ALG_TESTED)
		goto skiptest;

	err = alg_test(param->driver, param->alg, type, CRYPTO_ALG_TESTED);

skiptest:
	crypto_alg_tested(param->driver, err);

	kfree(param);
	module_put_and_exit(0);
=======
	int err;

	err = alg_test(param->driver, param->alg, type, CRYPTO_ALG_TESTED);

	crypto_alg_tested(param->driver, err);

	kfree(param);
	module_put_and_kthread_exit(0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cryptomgr_schedule_test(struct crypto_alg *alg)
{
	struct task_struct *thread;
	struct crypto_test_param *param;
<<<<<<< HEAD
	u32 type;
=======

	if (IS_ENABLED(CONFIG_CRYPTO_MANAGER_DISABLE_TESTS))
		return NOTIFY_DONE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!try_module_get(THIS_MODULE))
		goto err;

	param = kzalloc(sizeof(*param), GFP_KERNEL);
	if (!param)
		goto err_put_module;

	memcpy(param->driver, alg->cra_driver_name, sizeof(param->driver));
	memcpy(param->alg, alg->cra_name, sizeof(param->alg));
<<<<<<< HEAD
	type = alg->cra_flags;

	/* This piece of crap needs to disappear into per-type test hooks. */
	if ((!((type ^ CRYPTO_ALG_TYPE_BLKCIPHER) &
	       CRYPTO_ALG_TYPE_BLKCIPHER_MASK) && !(type & CRYPTO_ALG_GENIV) &&
	     ((alg->cra_flags & CRYPTO_ALG_TYPE_MASK) ==
	      CRYPTO_ALG_TYPE_BLKCIPHER ? alg->cra_blkcipher.ivsize :
					  alg->cra_ablkcipher.ivsize)) ||
	    (!((type ^ CRYPTO_ALG_TYPE_AEAD) & CRYPTO_ALG_TYPE_MASK) &&
	     alg->cra_type == &crypto_nivaead_type && alg->cra_aead.ivsize))
		type |= CRYPTO_ALG_TESTED;

	param->type = type;
=======
	param->type = alg->cra_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	thread = kthread_run(cryptomgr_test, param, "cryptomgr_test");
	if (IS_ERR(thread))
		goto err_free_param;

	return NOTIFY_STOP;

err_free_param:
	kfree(param);
err_put_module:
	module_put(THIS_MODULE);
err:
	return NOTIFY_OK;
}

static int cryptomgr_notify(struct notifier_block *this, unsigned long msg,
			    void *data)
{
	switch (msg) {
	case CRYPTO_MSG_ALG_REQUEST:
		return cryptomgr_schedule_probe(data);
	case CRYPTO_MSG_ALG_REGISTER:
		return cryptomgr_schedule_test(data);
<<<<<<< HEAD
=======
	case CRYPTO_MSG_ALG_LOADED:
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return NOTIFY_DONE;
}

static struct notifier_block cryptomgr_notifier = {
	.notifier_call = cryptomgr_notify,
};

static int __init cryptomgr_init(void)
{
	return crypto_register_notifier(&cryptomgr_notifier);
}

static void __exit cryptomgr_exit(void)
{
	int err = crypto_unregister_notifier(&cryptomgr_notifier);
	BUG_ON(err);
}

<<<<<<< HEAD
subsys_initcall(cryptomgr_init);
=======
/*
 * This is arch_initcall() so that the crypto self-tests are run on algorithms
 * registered early by subsys_initcall().  subsys_initcall() is needed for
 * generic implementations so that they're available for comparison tests when
 * other implementations are registered later by module_init().
 */
arch_initcall(cryptomgr_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_exit(cryptomgr_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Crypto Algorithm Manager");
