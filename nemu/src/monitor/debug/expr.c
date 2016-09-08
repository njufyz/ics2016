#include "nemu.h"
#include<stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256,OP ,EQ, NUM, REG, ADDR

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},                    // equal
    {"\\-",'-'},
    {"[*]",'*'},
    {"[/]",'/'},
    {"[(]",'('},
    {"[)]",')'},
    {"((0x)|(0X))([0-9,A-F,a-f]){1,8}",ADDR},
    {"[0-9]+",NUM},
   // {"[\$eacdbspixlh]+",REG},
};


#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(i) {
                    case 0:break;
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 7:tokens[nr_token].type=rules[i].token_type;
                           break;
                    case 9:tokens[nr_token].type=rules[i].token_type;
                           if(substr_len>=32) assert(0);
                           strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
                           tokens[nr_token].str[substr_len]=0;
                           break;
                    case 10:break;  /*TODO*/
                    case 8:tokens[nr_token].type=rules[i].token_type;
                           strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
                           tokens[nr_token].str[substr_len]=0;
                           break;
				//	default: panic("aa please implement me");
				}
                nr_token++;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

uint32_t eval(Token *,Token *);
int check_parentheses(Token* ,Token*);

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */

   // panic("please implement me");
	return eval(tokens,tokens+nr_token-1);
}
uint32_t eval(Token *p,Token *q)
{
    if(p>q) assert(0);
    else if(p==q)
    {
     char *num;
     int k;
     k = strtol(p->str,&num,16);
     return k;
    }
    else if(check_parentheses(p,q)==true)
    {
        return eval(p+1,q-1);
    }
    else
    {
       return 0x100000;
    }

}

int check_parentheses(Token *p, Token *q)
{
    if(p->type!='('||q->type!=')')
        return 0;
    Token *head = p+1;
    Token *tail = q-1;
    int num=0;
    for(;head!=tail;head++)
    {
        if(head->type=='(')
            num++;
        else if(head->type==')')
            num--;
        else;
        if(num<0) return 0;
    }
    if(num==0)
    return 1;
    else return 0;
}
