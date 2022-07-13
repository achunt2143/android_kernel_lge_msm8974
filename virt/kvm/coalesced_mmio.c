<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * KVM coalesced MMIO
 *
 * Copyright (c) 2008 Bull S.A.S.
 * Copyright 2009 Red Hat, Inc. and/or its affiliates.
 *
 *  Author: Laurent Vivier <Laurent.Vivier@bull.net>
 *
 */

<<<<<<< HEAD
#include "iodev.h"
=======
#include <kvm/iodev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/kvm_host.h>
#include <linux/slab.h>
#include <linux/kvm.h>

#include "coalesced_mmio.h"

static inline struct kvm_coalesced_mmio_dev *to_mmio(struct kvm_io_device *dev)
{
	return container_of(dev, struct kvm_coalesced_mmio_dev, dev);
}

static int coalesced_mmio_in_range(struct kvm_coalesced_mmio_dev *dev,
				   gpa_t addr, int len)
{
	/* is it in a batchable area ?
	 * (addr,len) is fully included in
	 * (zone->addr, zone->size)
	 */
	if (len < 0)
		return 0;
	if (addr + len < addr)
		return 0;
	if (addr < dev->zone.addr)
		return 0;
	if (addr + len > dev->zone.addr + dev->zone.size)
		return 0;
	return 1;
}

<<<<<<< HEAD
static int coalesced_mmio_has_room(struct kvm_coalesced_mmio_dev *dev)
=======
static int coalesced_mmio_has_room(struct kvm_coalesced_mmio_dev *dev, u32 last)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct kvm_coalesced_mmio_ring *ring;
	unsigned avail;

	/* Are we able to batch it ? */

	/* last is the first free entry
	 * check if we don't meet the first used entry
	 * there is always one unused entry in the buffer
	 */
	ring = dev->kvm->coalesced_mmio_ring;
<<<<<<< HEAD
	avail = (ring->first - ring->last - 1) % KVM_COALESCED_MMIO_MAX;
=======
	avail = (ring->first - last - 1) % KVM_COALESCED_MMIO_MAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (avail == 0) {
		/* full */
		return 0;
	}

	return 1;
}

<<<<<<< HEAD
static int coalesced_mmio_write(struct kvm_io_device *this,
				gpa_t addr, int len, const void *val)
{
	struct kvm_coalesced_mmio_dev *dev = to_mmio(this);
	struct kvm_coalesced_mmio_ring *ring = dev->kvm->coalesced_mmio_ring;
=======
static int coalesced_mmio_write(struct kvm_vcpu *vcpu,
				struct kvm_io_device *this, gpa_t addr,
				int len, const void *val)
{
	struct kvm_coalesced_mmio_dev *dev = to_mmio(this);
	struct kvm_coalesced_mmio_ring *ring = dev->kvm->coalesced_mmio_ring;
	__u32 insert;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!coalesced_mmio_in_range(dev, addr, len))
		return -EOPNOTSUPP;

	spin_lock(&dev->kvm->ring_lock);

<<<<<<< HEAD
	if (!coalesced_mmio_has_room(dev)) {
=======
	insert = READ_ONCE(ring->last);
	if (!coalesced_mmio_has_room(dev, insert) ||
	    insert >= KVM_COALESCED_MMIO_MAX) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_unlock(&dev->kvm->ring_lock);
		return -EOPNOTSUPP;
	}

	/* copy data in first free entry of the ring */

<<<<<<< HEAD
	ring->coalesced_mmio[ring->last].phys_addr = addr;
	ring->coalesced_mmio[ring->last].len = len;
	memcpy(ring->coalesced_mmio[ring->last].data, val, len);
	smp_wmb();
	ring->last = (ring->last + 1) % KVM_COALESCED_MMIO_MAX;
=======
	ring->coalesced_mmio[insert].phys_addr = addr;
	ring->coalesced_mmio[insert].len = len;
	memcpy(ring->coalesced_mmio[insert].data, val, len);
	ring->coalesced_mmio[insert].pio = dev->zone.pio;
	smp_wmb();
	ring->last = (insert + 1) % KVM_COALESCED_MMIO_MAX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&dev->kvm->ring_lock);
	return 0;
}

static void coalesced_mmio_destructor(struct kvm_io_device *this)
{
	struct kvm_coalesced_mmio_dev *dev = to_mmio(this);

	list_del(&dev->list);

	kfree(dev);
}

static const struct kvm_io_device_ops coalesced_mmio_ops = {
	.write      = coalesced_mmio_write,
	.destructor = coalesced_mmio_destructor,
};

int kvm_coalesced_mmio_init(struct kvm *kvm)
{
	struct page *page;
<<<<<<< HEAD
	int ret;

	ret = -ENOMEM;
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page)
		goto out_err;

	ret = 0;
=======

	page = alloc_page(GFP_KERNEL_ACCOUNT | __GFP_ZERO);
	if (!page)
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kvm->coalesced_mmio_ring = page_address(page);

	/*
	 * We're using this spinlock to sync access to the coalesced ring.
<<<<<<< HEAD
	 * The list doesn't need it's own lock since device registration and
=======
	 * The list doesn't need its own lock since device registration and
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * unregistration should only happen when kvm->slots_lock is held.
	 */
	spin_lock_init(&kvm->ring_lock);
	INIT_LIST_HEAD(&kvm->coalesced_zones);

<<<<<<< HEAD
out_err:
	return ret;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void kvm_coalesced_mmio_free(struct kvm *kvm)
{
	if (kvm->coalesced_mmio_ring)
		free_page((unsigned long)kvm->coalesced_mmio_ring);
}

int kvm_vm_ioctl_register_coalesced_mmio(struct kvm *kvm,
					 struct kvm_coalesced_mmio_zone *zone)
{
	int ret;
	struct kvm_coalesced_mmio_dev *dev;

<<<<<<< HEAD
	dev = kzalloc(sizeof(struct kvm_coalesced_mmio_dev), GFP_KERNEL);
=======
	if (zone->pio != 1 && zone->pio != 0)
		return -EINVAL;

	dev = kzalloc(sizeof(struct kvm_coalesced_mmio_dev),
		      GFP_KERNEL_ACCOUNT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dev)
		return -ENOMEM;

	kvm_iodevice_init(&dev->dev, &coalesced_mmio_ops);
	dev->kvm = kvm;
	dev->zone = *zone;

	mutex_lock(&kvm->slots_lock);
<<<<<<< HEAD
	ret = kvm_io_bus_register_dev(kvm, KVM_MMIO_BUS, zone->addr,
				      zone->size, &dev->dev);
=======
	ret = kvm_io_bus_register_dev(kvm,
				zone->pio ? KVM_PIO_BUS : KVM_MMIO_BUS,
				zone->addr, zone->size, &dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		goto out_free_dev;
	list_add_tail(&dev->list, &kvm->coalesced_zones);
	mutex_unlock(&kvm->slots_lock);

	return 0;

out_free_dev:
	mutex_unlock(&kvm->slots_lock);
	kfree(dev);

	return ret;
}

int kvm_vm_ioctl_unregister_coalesced_mmio(struct kvm *kvm,
					   struct kvm_coalesced_mmio_zone *zone)
{
	struct kvm_coalesced_mmio_dev *dev, *tmp;
<<<<<<< HEAD

	mutex_lock(&kvm->slots_lock);

	list_for_each_entry_safe(dev, tmp, &kvm->coalesced_zones, list)
		if (coalesced_mmio_in_range(dev, zone->addr, zone->size)) {
			kvm_io_bus_unregister_dev(kvm, KVM_MMIO_BUS, &dev->dev);
			kvm_iodevice_destructor(&dev->dev);
		}

	mutex_unlock(&kvm->slots_lock);

=======
	int r;

	if (zone->pio != 1 && zone->pio != 0)
		return -EINVAL;

	mutex_lock(&kvm->slots_lock);

	list_for_each_entry_safe(dev, tmp, &kvm->coalesced_zones, list) {
		if (zone->pio == dev->zone.pio &&
		    coalesced_mmio_in_range(dev, zone->addr, zone->size)) {
			r = kvm_io_bus_unregister_dev(kvm,
				zone->pio ? KVM_PIO_BUS : KVM_MMIO_BUS, &dev->dev);
			/*
			 * On failure, unregister destroys all devices on the
			 * bus, including the target device. There's no need
			 * to restart the walk as there aren't any zones left.
			 */
			if (r)
				break;
		}
	}

	mutex_unlock(&kvm->slots_lock);

	/*
	 * Ignore the result of kvm_io_bus_unregister_dev(), from userspace's
	 * perspective, the coalesced MMIO is most definitely unregistered.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
