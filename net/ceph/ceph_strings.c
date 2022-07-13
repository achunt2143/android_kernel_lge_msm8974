<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Ceph string constants
 */
#include <linux/module.h>
#include <linux/ceph/types.h>

const char *ceph_entity_type_name(int type)
{
	switch (type) {
	case CEPH_ENTITY_TYPE_MDS: return "mds";
	case CEPH_ENTITY_TYPE_OSD: return "osd";
	case CEPH_ENTITY_TYPE_MON: return "mon";
	case CEPH_ENTITY_TYPE_CLIENT: return "client";
	case CEPH_ENTITY_TYPE_AUTH: return "auth";
	default: return "unknown";
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(ceph_entity_type_name);

const char *ceph_auth_proto_name(int proto)
{
	switch (proto) {
	case CEPH_AUTH_UNKNOWN:
		return "unknown";
	case CEPH_AUTH_NONE:
		return "none";
	case CEPH_AUTH_CEPHX:
		return "cephx";
	default:
		return "???";
	}
}

const char *ceph_con_mode_name(int mode)
{
	switch (mode) {
	case CEPH_CON_MODE_UNKNOWN:
		return "unknown";
	case CEPH_CON_MODE_CRC:
		return "crc";
	case CEPH_CON_MODE_SECURE:
		return "secure";
	default:
		return "???";
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const char *ceph_osd_op_name(int op)
{
	switch (op) {
<<<<<<< HEAD
	case CEPH_OSD_OP_READ: return "read";
	case CEPH_OSD_OP_STAT: return "stat";

	case CEPH_OSD_OP_MASKTRUNC: return "masktrunc";

	case CEPH_OSD_OP_WRITE: return "write";
	case CEPH_OSD_OP_DELETE: return "delete";
	case CEPH_OSD_OP_TRUNCATE: return "truncate";
	case CEPH_OSD_OP_ZERO: return "zero";
	case CEPH_OSD_OP_WRITEFULL: return "writefull";
	case CEPH_OSD_OP_ROLLBACK: return "rollback";

	case CEPH_OSD_OP_APPEND: return "append";
	case CEPH_OSD_OP_STARTSYNC: return "startsync";
	case CEPH_OSD_OP_SETTRUNC: return "settrunc";
	case CEPH_OSD_OP_TRIMTRUNC: return "trimtrunc";

	case CEPH_OSD_OP_TMAPUP: return "tmapup";
	case CEPH_OSD_OP_TMAPGET: return "tmapget";
	case CEPH_OSD_OP_TMAPPUT: return "tmapput";

	case CEPH_OSD_OP_GETXATTR: return "getxattr";
	case CEPH_OSD_OP_GETXATTRS: return "getxattrs";
	case CEPH_OSD_OP_SETXATTR: return "setxattr";
	case CEPH_OSD_OP_SETXATTRS: return "setxattrs";
	case CEPH_OSD_OP_RESETXATTRS: return "resetxattrs";
	case CEPH_OSD_OP_RMXATTR: return "rmxattr";
	case CEPH_OSD_OP_CMPXATTR: return "cmpxattr";

	case CEPH_OSD_OP_PULL: return "pull";
	case CEPH_OSD_OP_PUSH: return "push";
	case CEPH_OSD_OP_BALANCEREADS: return "balance-reads";
	case CEPH_OSD_OP_UNBALANCEREADS: return "unbalance-reads";
	case CEPH_OSD_OP_SCRUB: return "scrub";

	case CEPH_OSD_OP_WRLOCK: return "wrlock";
	case CEPH_OSD_OP_WRUNLOCK: return "wrunlock";
	case CEPH_OSD_OP_RDLOCK: return "rdlock";
	case CEPH_OSD_OP_RDUNLOCK: return "rdunlock";
	case CEPH_OSD_OP_UPLOCK: return "uplock";
	case CEPH_OSD_OP_DNLOCK: return "dnlock";

	case CEPH_OSD_OP_CALL: return "call";

	case CEPH_OSD_OP_PGLS: return "pgls";
	}
	return "???";
}


const char *ceph_pool_op_name(int op)
{
	switch (op) {
	case POOL_OP_CREATE: return "create";
	case POOL_OP_DELETE: return "delete";
	case POOL_OP_AUID_CHANGE: return "auid change";
	case POOL_OP_CREATE_SNAP: return "create snap";
	case POOL_OP_DELETE_SNAP: return "delete snap";
	case POOL_OP_CREATE_UNMANAGED_SNAP: return "create unmanaged snap";
	case POOL_OP_DELETE_UNMANAGED_SNAP: return "delete unmanaged snap";
	}
	return "???";
=======
#define GENERATE_CASE(op, opcode, str)	case CEPH_OSD_OP_##op: return (str);
__CEPH_FORALL_OSD_OPS(GENERATE_CASE)
#undef GENERATE_CASE
	default:
		return "???";
	}
}

const char *ceph_osd_watch_op_name(int o)
{
	switch (o) {
	case CEPH_OSD_WATCH_OP_UNWATCH:
		return "unwatch";
	case CEPH_OSD_WATCH_OP_WATCH:
		return "watch";
	case CEPH_OSD_WATCH_OP_RECONNECT:
		return "reconnect";
	case CEPH_OSD_WATCH_OP_PING:
		return "ping";
	default:
		return "???";
	}
}

const char *ceph_osd_state_name(int s)
{
	switch (s) {
	case CEPH_OSD_EXISTS:
		return "exists";
	case CEPH_OSD_UP:
		return "up";
	case CEPH_OSD_AUTOOUT:
		return "autoout";
	case CEPH_OSD_NEW:
		return "new";
	default:
		return "???";
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
