#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	int val;
	char* exp;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */


} WP;
void init_wp_list();
WP* new_wp(char* str,int res);
void free_wp(WP * wp);
#endif
