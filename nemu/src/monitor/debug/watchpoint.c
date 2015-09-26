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

int new_wp(char* str)
{	
	//printf("ok\n");
	bool success=true;
	uint32_t res=expr(str,&success);
	if (!success){
			printf("please try again:\n");
			return -1;
	}
	if (free_==NULL) {
		printf("no enough space\n");
		return -1;
	}
	WP* new_=free_;
	free_=free_->next;
	/*WP* pre=free_;
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
	}*/
	//printf("**%s\n%d\n",str,res);
	strcpy(new_->exp,str);
	new_->val=res;
	new_->next=head;
	head=new_;
	printf("**%d\t%s\t%d\n",head->NO,head->exp,head->val);
	return 0;
}
void free_wp(int no)
{
	if (head==NULL) {
		printf("no watchpoint\n");
		return;
	}

	WP* p=head;			//in the middle
	while (p->NO!=no){
		p=p->next;
		if (p==NULL) {
			printf("NO.%d watchpoint is not exist.\n",no);
			return;
		}
	}
	if (p==head){
		head=head->next;
		p->next=free_;
		free_=p;
	}else{
		WP* pre=head;
		while(pre->next!=p) pre=pre->next;
		pre->next=p->next;
		p->next=free_;
		free_=p;
	}
	return;
}
int display_wp()
{
	printf("NUM\tTYPE\tDISP\tADRESS\tWHAT\n");
	WP* p=head;
	while (p!=NULL)
	{
		printf("%d\twatchpoint\t%d\t\t%s\n",p->NO,p->val,p->exp);
		p=p->next;
	}
	return 0;
}
bool if_changes_wp()
{
	WP* p=head;
	bool flag=false,temp;
	while (p!=NULL){

		int nw_val=expr(p->exp,&temp);
		if (nw_val!=p->val){
			flag=true;
			printf("watchpoint %d: %s\n",p->NO,p->exp);
			printf("Old value= %d:\n",p->val);
			printf("New value= %d:\n",nw_val);
			p->val=nw_val;	
		}
		p=p->next;
	}
	return flag;
}



