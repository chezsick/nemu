#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
WP* tail(WP* h)
{
	WP* p=h;
	if (p==NULL) return h;
	while (p->next!=NULL) p=p->next;
	return p;
}

WP* new_wp(char* str,int res)
{	
	printf("ok\n");
	if (free_==NULL) assert(0);
	WP* pre=free_;
	WP* p=free_->next;
	WP* new_=NULL;
	if (p==NULL) {
		new_=pre;	
		free_=NULL;
	}else {
		while (p->next!=NULL){
			pre=p;
			p=p->next;	
		}
		new_=p;
		pre->next=NULL;
	}
	//printf("**%s\n%d\n",str,res);
	new_->exp=str;
	new_->val=res;
	printf("**%s\n%d\n",str,res);
	return new_;
}
void free_wp(WP *wp)
{
		if (wp==head){			//wp is the first point
		WP* f=tail(free_);
		f->next=head;
		head=head->next;
	}
	WP* p=head;			//in the middle
	while (p->next!=wp){
		p=p->next;
		if (p==NULL) assert(0);
	}
	WP* ftail=tail(free_);
	ftail->next=p->next;
	p->next=p->next->next;
	ftail->next->next=NULL;

}



