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
#include <crypto/hash.h>

#include "provenance.h"
#include "provenance_query.h"

static int out_edge(prov_entry_t *node, prov_entry_t *edge)
{
	uint64_t hash[3];

	get_prov_hash(node) = djb2_hash_n(get_prov_identifier(node).buffer,
																		sizeof(union prov_identifier));
	hash[0] = get_prov_hash(node);
	hash[1] = djb2_hash_n(get_prov_hash_in(node), PROV_N_BYTES);
	hash[2] = djb2_hash_n(get_prov_identifier(edge).buffer,
												sizeof(union prov_identifier));
	get_prov_hash(edge) = djb2_hash_n((uint8_t*)hash, 3*sizeof(uint64_t));
	return 0;
}

static int in_edge(prov_entry_t *edge, prov_entry_t *node)
{
	get_prov_hash(node) = djb2_hash_n(get_prov_identifier(node).buffer,
																		sizeof(union prov_identifier));
	if( edge_type(edge) == RL_VERSION
		|| edge_type(edge) == RL_VERSION_PROCESS)
		memset(get_prov_hash_in(node), 0, PROV_N_BYTES);
	prov_bloom_add(get_prov_hash_in(node), get_prov_hash(edge));
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
