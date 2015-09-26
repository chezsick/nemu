#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"
#define NUM 100
typedef struct watchpoint {
	int NO;
	int val;
	char exp[NUM];
	struct watchpoint *next;

	/* TODO: Add more members if necessary */


} WP;
void init_wp_list();
int  new_wp(char* str);
void free_wp(int no);
int display_wp();
bool if_changes_wp();
#endif
