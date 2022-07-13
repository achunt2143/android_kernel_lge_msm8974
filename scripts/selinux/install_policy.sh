#!/bin/sh
<<<<<<< HEAD
=======
# SPDX-License-Identifier: GPL-2.0
set -e
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
if [ `id -u` -ne 0 ]; then
	echo "$0: must be root to install the selinux policy"
	exit 1
fi
<<<<<<< HEAD
SF=`which setfiles`
if [ $? -eq 1 ]; then
	if [ -f /sbin/setfiles ]; then
		SF="/usr/setfiles"
	else
		echo "no selinux tools installed: setfiles"
		exit 1
	fi
fi

cd mdp

CP=`which checkpolicy`
VERS=`$CP -V | awk '{print $1}'`

./mdp policy.conf file_contexts
$CP -o policy.$VERS policy.conf
=======

SF=`which setfiles`
if [ $? -eq 1 ]; then
	echo "Could not find setfiles"
	echo "Do you have policycoreutils installed?"
	exit 1
fi

CP=`which checkpolicy`
if [ $? -eq 1 ]; then
	echo "Could not find checkpolicy"
	echo "Do you have checkpolicy installed?"
	exit 1
fi
VERS=`$CP -V | awk '{print $1}'`

ENABLED=`which selinuxenabled`
if [ $? -eq 1 ]; then
	echo "Could not find selinuxenabled"
	echo "Do you have libselinux-utils installed?"
	exit 1
fi

if selinuxenabled; then
    echo "SELinux is already enabled"
    echo "This prevents safely relabeling all files."
    echo "Boot with selinux=0 on the kernel command-line."
    exit 1
fi

cd mdp
./mdp -m policy.conf file_contexts
$CP -U allow -M -o policy.$VERS policy.conf
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

mkdir -p /etc/selinux/dummy/policy
mkdir -p /etc/selinux/dummy/contexts/files

<<<<<<< HEAD
=======
echo "__default__:user_u:s0" > /etc/selinux/dummy/seusers
echo "base_r:base_t:s0" > /etc/selinux/dummy/contexts/failsafe_context
echo "base_r:base_t:s0 base_r:base_t:s0" > /etc/selinux/dummy/default_contexts
cat > /etc/selinux/dummy/contexts/x_contexts <<EOF
client * user_u:base_r:base_t:s0
property * user_u:object_r:base_t:s0
extension * user_u:object_r:base_t:s0
selection * user_u:object_r:base_t:s0
event * user_u:object_r:base_t:s0
EOF
touch /etc/selinux/dummy/contexts/virtual_domain_context
touch /etc/selinux/dummy/contexts/virtual_image_context

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
cp file_contexts /etc/selinux/dummy/contexts/files
cp dbus_contexts /etc/selinux/dummy/contexts
cp policy.$VERS /etc/selinux/dummy/policy
FC_FILE=/etc/selinux/dummy/contexts/files/file_contexts

if [ ! -d /etc/selinux ]; then
	mkdir -p /etc/selinux
fi
<<<<<<< HEAD
if [ ! -f /etc/selinux/config ]; then
	cat > /etc/selinux/config << EOF
SELINUX=enforcing
SELINUXTYPE=dummy
EOF
else
	TYPE=`cat /etc/selinux/config | grep "^SELINUXTYPE" | tail -1 | awk -F= '{ print $2 '}`
	if [ "eq$TYPE" != "eqdummy" ]; then
		selinuxenabled
		if [ $? -eq 0 ]; then
			echo "SELinux already enabled with a non-dummy policy."
			echo "Exiting.  Please install policy by hand if that"
			echo "is what you REALLY want."
			exit 1
		fi
		mv /etc/selinux/config /etc/selinux/config.mdpbak
		grep -v "^SELINUXTYPE" /etc/selinux/config.mdpbak >> /etc/selinux/config
		echo "SELINUXTYPE=dummy" >> /etc/selinux/config
	fi
fi

cd /etc/selinux/dummy/contexts/files
$SF file_contexts /

mounts=`cat /proc/$$/mounts | egrep "ext2|ext3|xfs|jfs|ext4|ext4dev|gfs2" | awk '{ print $2 '}`
$SF file_contexts $mounts


dodev=`cat /proc/$$/mounts | grep "/dev "`
if [ "eq$dodev" != "eq" ]; then
	mount --move /dev /mnt
	$SF file_contexts /dev
	mount --move /mnt /dev
fi

=======
if [ -f /etc/selinux/config ]; then
    echo "/etc/selinux/config exists, moving to /etc/selinux/config.bak."
    mv /etc/selinux/config /etc/selinux/config.bak
fi
echo "Creating new /etc/selinux/config for dummy policy."
cat > /etc/selinux/config << EOF
SELINUX=permissive
SELINUXTYPE=dummy
EOF

cd /etc/selinux/dummy/contexts/files
$SF -F file_contexts /

mounts=`cat /proc/$$/mounts | \
	grep -E "ext[234]|jfs|xfs|reiserfs|jffs2|gfs2|btrfs|f2fs|ocfs2" | \
	awk '{ print $2 '}`
$SF -F file_contexts $mounts

echo "-F" > /.autorelabel
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
