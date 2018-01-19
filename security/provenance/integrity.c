/*
 *
 * Author: Thomas Pasquier <thomas.pasquier@cl.cam.ac.uk>
 *
 * Copyright (C) 2015-2018 University of Cambridge, Harvard University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 */
#include "provenance.h"
#include "provenance_query.h"

static int out_edge(prov_entry_t *node, prov_entry_t *edge)
{
	return 0;
}

static int in_edge(prov_entry_t *edge, prov_entry_t *node)
{
	return 0;
}

static struct provenance_query_hooks hooks = {
	QUERY_HOOK_INIT(out_edge, out_edge),
	QUERY_HOOK_INIT(in_edge,  in_edge),
};

static int __init init_prov_integrity(void)
{
	register_provenance_query_hooks(&hooks);
	pr_info("Provenance: integrity ready.\n");
	return 0;
}
security_initcall(init_prov_integrity);
