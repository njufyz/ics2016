#include "nemu.h"
#include<stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>


enum {
	NOTYPE = 256,OP ,EQ, NEQ, NUM, REG, ADDR, AND, NOT, OR, DEREF, NEG
,OBJ
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
	{"==", EQ},
    {"[$eacdbspixlh]+",REG},
    {"!=",NEQ},
    {"&&",AND},
    {"[||]{2}",OR},
    {"!",NOT},
    {"\\-",'-'},
    {"[*]",'*'},
    {"[/]",'/'},
    {"[(]",'('},
    {"[)]",')'},
    {"((0x)|(0X))([0-9,A-F,a-f]){1,8}",ADDR},
    {"[0-9]+",NUM},
    {"[A-Z,a-z,0-9,_]+",OBJ}
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

				switch(rules[i].token_type) {
                    case NOTYPE:break;
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '(':
                    case ')':
                    case EQ:
                    case NEQ:
                    case AND:
                    case OR:
                    case NOT:tokens[nr_token].type=rules[i].token_type;
                           break;
                    case OBJ:
                    case NUM:
                    case ADDR:
                    case REG:tokens[nr_token].type=rules[i].token_type;
                           if(substr_len>=32) assert(0);
                           strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
                           tokens[nr_token].str[substr_len]=0;
                           break;
					default: break;
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

uint32_t eval(int,int);
int check_parentheses(int ,int);
int position_dominant(int,int);

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
    int i = 0;
    for(i = 0; i < nr_token; i ++) {
         if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type=='(' || tokens[i - 1].type == '+'||tokens[i - 1].type == '-'||tokens[i - 1].type == '*'||tokens[i - 1].type == '/'||tokens[i - 1].type == EQ||tokens[i - 1].type == NOT||tokens[i - 1].type == NEQ || tokens[i - 1].type == AND ||tokens[i - 1].type == OR))) 
         {
            tokens[i].type = DEREF;
         }
 }

    for(i = 0; i < nr_token; i ++) {                                                                                                                                               
    if(tokens[i].type == '-' && (i == 0 || ( tokens[i-1].type=='(' || tokens[i - 1].type == '+'||tokens[i - 1].type == '-'||tokens[i - 1].type == '*'||tokens[i - 1].type == '/'||tokens[i - 1].type      == EQ||tokens[i - 1].type == NOT||tokens[i - 1].type == NEQ || tokens[i - 1].type == AND ||tokens[i - 1].type == OR)))                                                            
                                    {                                                                                                                                                                                                                                                                tokens[i].type = NEG;                            }                                                                                                                                                                        
                        }
    for(i = 0; i < nr_token; i ++) {                                                                                                                                               
    if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type==NEG|| tokens[i-1].type=='(' || tokens[i - 1].type == '+'||tokens[i - 1].type == '-'||tokens[i - 1].type == '*'||tokens[i - 1].type == '/'||tokens[i - 1].type      == EQ||tokens[i - 1].type == NOT||tokens[i - 1].type == NEQ || tokens[i - 1].type == AND ||tokens[i - 1].type == OR)))                                                            
                                    {                                                                                                                                                                                                                                                                tokens[i].type = NEG;                            }                                                                                                                                                                        
                        }


    for(i=0;i<nr_token-1;i++)
    {
        if(tokens[i].type=='+'||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/'||tokens[i].type==NEG||tokens[i].type==DEREF)
        {
            if(tokens[i+1].type=='+'||tokens[i+1].type=='-'||tokens[i+1].type=='*'||tokens[i+1].type=='/'||tokens[i+1].type==')'||tokens[i+1].type==EQ||tokens[i+1].type==NEQ || tokens[i+1].type==AND || tokens[i+1].type == OR) 
            {
                puts("Bad EXPR!");
                *success = 0;
                return 0;
            }
        }
        if((tokens[i].type==NUM||tokens[i].type==ADDR) &&(tokens[i+1].type=='('||tokens[i+1].type==NEG||tokens[i+1].type==NOT||tokens[i+1].type==DEREF))
        {
            puts("Bad EXPR!");
            *success=0;
            return 0;
        }
    }
   // panic("please implement me");
	return eval(0,nr_token-1);
}

uint32_t get_obj(char*);

uint32_t eval(int p,int q)
{
    if(p>q) assert(0);
   
    else if(p==q)
    {
     if(tokens[p].type==OBJ)
     {
         uint32_t k  = get_obj(tokens[p].str);
         if(k==0xffffffff) 
         {
             puts("NO Such Object!");
             assert(0);
         }
         else return k;
     }
     if(tokens[p].type!=REG)
     {
     char *num;
     int k;
     k = strtol(tokens[p].str,&num,0);
     return k;
     }
     else 
     {
        int index = 0;
        char* name = tokens[p].str+1;
        if(strcmp(name,"eip")==0) return cpu.eip;
        for(;index<8;index++)
            if(strcmp(name,regsl[index])==0) return cpu.gpr[index]._32;
        for(index=0;index<8;index++)
            if(strcmp(name,regsw[index])==0) return cpu.gpr[index]._16;
        for(index=0;index<4;index++)
            if(strcmp(name,regsb[index])==0) return cpu.gpr[index]._8[0];
        for(;index<8;index++)
            if(strcmp(name,regsb[index])==0) return cpu.gpr[index]._8[1];

        assert(0);
     }
    }
    else if(check_parentheses(p,q)==1)
    {
        return eval(p+1,q-1);
    }
    else if(check_parentheses(p,q)==-1) 
    {
        puts("Bad EXPR!");
        assert(0);
    }
    else
    {
        int op=-1; 
        op = position_dominant(p,q);
        int val1,val2;
        
        if(tokens[op].type != NOT && tokens[op].type!=DEREF && tokens[op].type != NEG)
           val1 =eval(p,op-1);
        else val1=0;

        val2 = eval(op+1,q);

       switch(tokens[op].type){
           case '+': return val1+val2;
           case '-': return val1-val2;
           case '*': return val1*val2;
           case '/': return val1/val2;
           case EQ: return (val1==val2)?1:0;
           case NEQ:return (val1!=val2)?1:0;
           case AND:return val1&&val2;
           case OR: return val1||val2;
           case NOT: return !val2;
           case DEREF: return swaddr_read(val2,4);
           case NEG: return -val2;
           default: assert(0);

            }

    }
}

int check_parentheses(int p, int q)
{
    if(tokens[p].type!='('||tokens[q].type!=')')
        return 0;
    int num=0;
    for(;p<=q;p++)
    {
        if(tokens[p].type=='(')
            num++;
        else if(tokens[p].type==')')
            num--;
        if(num<0) return -1;
    }
    if(num==0)
    return 1;
    else return -1;
}

int position_dominant(int p, int q)
{
    int pos1 =-1;    //pos of +/-
    int pos2 = -1;   //pos of */ /
    int pos3 = -1;   // == !=
    int pos4 = -1;  // &&
    int pos5 = -1;  // ||
     int pos6 = 63; //!
     int pos7 = -1;  // *
     int pos8 = 63;  //NEG


    int level = 0;
    for(;p<=q;p++)
    {
        if(tokens[p].type=='(') level++;
        else if(tokens[p].type==')') level--;
       
        if(level==0)
        {
            if(tokens[p].type=='+'||tokens[p].type=='-')
                pos1 = p;
            else if(tokens[p].type=='*'||tokens[p].type=='/')
                pos2 = p;
            else if(tokens[p].type==EQ||tokens[p].type==NEQ)
                pos3 = p;
            else if(tokens[p].type==AND)
                pos4 = p;
            else if(tokens[p].type==OR)
                pos5 = p;
            else if(tokens[p].type==NOT)
            {
                 if(p<pos6) pos6 = p; 
            }
            else if(tokens[p].type==DEREF)
                pos7 = p;
            else if(tokens[p].type==NEG)
            {
                if(p<pos8)  pos8 = p;
            }
        }
    }
    if(pos5!=-1) return pos5;
    else if(pos4!=-1) return pos4;
    else if(pos3!=-1) return pos3;
    else if(pos1!=-1) return pos1;
    else if(pos2!=-1) return pos2;
    else if(pos6!=63) return pos6;
    else if(pos7!=-1) return pos7;
    else if(pos8!=63) return pos8;
    else return -1;
}
