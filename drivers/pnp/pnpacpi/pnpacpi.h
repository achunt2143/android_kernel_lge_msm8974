<<<<<<< HEAD
#ifndef ACPI_PNP_H
#define ACPI_PNP_H

#include <acpi/acpi_bus.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ACPI_PNP_H
#define ACPI_PNP_H

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/acpi.h>
#include <linux/pnp.h>

int pnpacpi_parse_allocated_resource(struct pnp_dev *);
int pnpacpi_parse_resource_option_data(struct pnp_dev *);
int pnpacpi_encode_resources(struct pnp_dev *, struct acpi_buffer *);
int pnpacpi_build_resource_template(struct pnp_dev *, struct acpi_buffer *);
#endif
