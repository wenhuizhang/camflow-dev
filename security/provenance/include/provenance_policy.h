/*
 *
 * Author: Thomas Pasquier <tfjmp@seas.harvard.edu>
 *
 * Copyright (C) 2015-2017 University of Cambridge, Harvard University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 */
#ifndef _PROVENANCE_POLICY_H
#define _PROVENANCE_POLICY_H

struct capture_policy {
	bool prov_enabled;
	bool prov_all;
	bool should_compress_node;
	bool should_compress_edge;
	uint64_t prov_node_filter;
	uint64_t prov_propagate_node_filter;
	uint64_t prov_relation_filter;
	uint64_t prov_propagate_relation_filter;
};

extern struct capture_policy prov_policy;

#endif
