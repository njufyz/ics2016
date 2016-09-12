#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "monitor/monitor.h"
#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp();
void free_wp(WP* wp);


WP* new_wp()
{
    if(free_==NULL) assert(0);
    WP* p = free_;
    free_->next=free_;
    p->next=NULL;

    WP* q= head;
    if(q==NULL) head=p;
    else
    {
        for(;q->next!=NULL;q=q->next);
        q->next = p;
    }
    return p;

}

void free_wp(WP* wp)
{
    WP* head1=head;
    WP* pre=NULL;

    for(;head1->NO!=wp->NO;pre=head1,head1=head1->next);
    pre->next=head1->next;
    wp->next=NULL;


    if(free_ == NULL) {
        free_=wp;
        free_->next=NULL;
    }

     head1 = free_;
     pre = NULL;
    if(wp->NO < free_->NO)
    {
        wp->next = free_;
        free_ = wp;
    }

    for(;head1->next!=NULL && wp->NO > head1->NO;pre = head1, head1=head1->next);
    pre->next = wp;
    wp->next = head1;

    return;
}
int check_watchpoint()
{
    WP* p = head;
    if(head==NULL) return 0;
    for(;p!=NULL;p=p->next)
    {
        bool su = 1;
    //    Log("%u",expr(p->expr,&su));
        if(expr(p->expr,&su)!=p->val)
        {
            nemu_state = STOP;
            printf("watchpoint %d: %s\nOld value:%u\nNew value:%u\n",p->NO,p->expr,p->val,expr(p->expr,&su));
            return 1;
        }
    }
    return 0;
}
            

    















