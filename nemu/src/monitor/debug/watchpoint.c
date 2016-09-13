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
void free_wp(int );
void display();

WP* new_wp()
{
    if(free_==NULL) assert(0);
    WP* p = free_;
    free_=free_->next;
     p->next=NULL;
    
    WP* q= head;
    if(head==NULL) head=p;
    else
    {
        for(;q->next!=NULL;q=q->next);
        q->next = p;
        p->next=NULL;
    }
   display(); 
    return p;

}

void free_wp(int NO)
{
    WP* head1=head;
    WP* pre=NULL;
    if(head1->NO!=NO)
    {
        for(;head1->NO!=NO;pre=head1,head1=head1->next);
        pre->next=head1->next;
    }
    else {
    head=head1->next;
    }
    WP* wp=head1;

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
        return;
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
        if(p==NULL) break;
        bool su = 1;
        int k =expr(p->expr,&su);
        if(k!=p->val)
        {
            nemu_state = STOP;
            printf("watchpoint %d: %s\nOld value:%u\nNew value:%u\n",p->NO,p->expr,p->val,k);
            return 1;
        }
    }
    return 0;
}
            
void display()
{
    WP*p =head;
    for(;p!=NULL;p=p->next)
    {
        if(p==NULL) break;
        printf("Watchpoint:%d  EXPR:%s   Value:%u\n",p->NO,p->expr,p->val);
    }
}
    















