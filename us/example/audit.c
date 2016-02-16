/*
* Caffeine Linux Security Module
*
* Author: Thomas Pasquier <tfjmp2@cam.ac.uk>
*
* Copyright (C) 2015 University of Cambridge
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "simplog.h"
#include "provenancelib.h"

#define	LOG_FILE "/tmp/audit.log"
#define gettid() syscall(SYS_gettid)

void tagarr_to_str(const uint64_t* in, const size_t in_size, char* out, size_t out_size){
  int i;
	size_t rm = out_size,cx;
	out[0]='\0';
  for(i = 0; i < in_size-1; i++){
		cx = snprintf(out, rm, "%lu, ", in[i]);
		if(cx<0){
			out[0]='\0';
			return;
		}
		out+=cx;
		rm-=cx;
  }
	cx = snprintf(out, rm, "%lu", in[i]);
	if(cx<0){
		out[0]='\0';
		return;
	}
}

void _init_logs( void ){
  simplog.setLogFile(LOG_FILE);
  simplog.setLineWrap(false);
  simplog.setLogSilentMode(true);
  simplog.setLogDebugLevel(SIMPLOG_VERBOSE);
}

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void write_to_log(const char* fmt, ...){
  char tmp[5096];
	va_list args;
	va_start(args, fmt);
  vsprintf(tmp, fmt, args);
	va_end(args);
  pthread_mutex_lock(&mut);
  simplog.writeLog(SIMPLOG_INFO, tmp);
  pthread_mutex_unlock(&mut);
}

void init( void ){
  pid_t tid = gettid();
  write_to_log("audit writer thread, tid:%ld", tid);
}

void log_str(struct str_struct* data){
  write_to_log("%lu - %s", data->event_id, data->str);
}

void log_edge(struct edge_struct* edge){
  if(edge->allowed==FLOW_ALLOWED)
  {
    write_to_log("%d[%lu]{%lu->%lu} allowed", edge_str[edge->type], edge->event_id, edge->snd_id, edge->rcv_id);
  }else{
    write_to_log("%d[%lu]{%lu->%lu} disallowed", edge_str[edge->type], edge->event_id, edge->snd_id, edge->rcv_id);
  }
}

void log_node(struct node_struct* node){
  write_to_log("%s[%lu]{%lu|%lu|%lu}", node_str[node->type], node->event_id, node->node_id, node->uid, node->gid);
}

struct provenance_ops ops = {
  .init=init,
  .log_edge=log_edge,
  .log_node=log_node,
  .log_str=log_str
};

int main(void){
  int rc;
  printf("Size: %d.\n", sizeof(prov_msg_t));
	_init_logs();
  simplog.writeLog(SIMPLOG_INFO, "audit process pid: %d", getpid());
  rc = provenance_register(&ops);
  if(rc){
    simplog.writeLog(SIMPLOG_ERROR, "Failed registering audit operation.");
    exit(rc);
  }
  while(1) sleep(60);
  provenance_stop();
  return 0;
}