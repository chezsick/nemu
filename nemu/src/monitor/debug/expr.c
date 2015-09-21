#include "nemu.h"
#include "stdlib.h"
#include <stdio.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>


#define DDEBUG
#define RED "\33[0;32;31m"
#define NONE "\33[m"

extern CPU_state cpu;
bool valid=true;
enum {
	NOTYPE = 256, EQ,HEX,DEC,POINT,NEG,AND,OR,NEQ,GE,LE,G,L,REG,REG_32,REG_16,REG_8

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
	{"\\-", '-'},					//minus
	{"\\*", '*'},					//mulitipy
	{"\\/", '/'},					//divide
	{"\\%", '%'},					//mod
	{"\\!=",NEQ},					//not equal
	{"\\!",   '!'},					//not
	{"==", EQ},					// equal
	{">=",GE},					//greater equal
	{"<=",LE},					//less equal
	{">",G},					//great
	{"<",L},					//less
	{"\\&\\&",AND},					//and
	{"\\|\\|",OR},					//or
	{"0[xX][0-9a-fA-F]+",HEX},			//hex
	{"[0-9]+",DEC},					//decimal
	{"\\(",'('},					//left bracket
	{"\\)",')'},					//right bracket
	{"\\$e[a-d]x|\\$esi|\\$edi|\\$ebp|\\$esp|\\$eip",REG_32},	//register 32
	{"\\$[a-d]x|\\$sp|\\$bp|\\$si|\\$di",REG_16},		//register 16
	{"\\$[a-d][lh]",REG_8}				//register 8
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
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
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
	valid=true;
	regmatch_t pmatch;
	
	nr_token = 0;
	memset(tokens,0,sizeof(tokens));
	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE: break;
					case DEC:case HEX:{
						tokens[nr_token].type=rules[i].token_type;
						strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
						nr_token++;
						break;
					}
					case '(':case ')':
					case '+':case '-':
					case '*':case '/':
					case '%':case '!':
					case EQ :case AND:
					case OR :case NEQ:
					case GE :case LE :
					case G  :case L  :
					{
						tokens[nr_token].type=rules[i].token_type;
						nr_token++;
						break; 
					}
					case REG_32:
					case REG_16:
					case REG_8 :{
						tokens[nr_token].type=REG;
						strncpy(tokens[nr_token].str,e+position-substr_len,substr_len);
				    		nr_token++;
						break;
					}
					default: panic("please implement me");
				}

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
bool check_parentheses(int p,int q)
{
	bool result=true;
	if ((tokens[p].type!='(')||(tokens[q].type!=')')) 
		result=false;
	int count=0,i;
	for(i=p;i<=q;i++){
		if (tokens[i].type=='(')
			count++;
		else if(tokens[i].type==')')
			count--;
		if (count==0 && i<q)result=false;
		if (count<0) {
			printf(RED"Brackets not match.\n"NONE);
			valid=false;
			return false;
		}
	}
	if (count>0) {
		printf(RED"Brackets not match\n"NONE);
		valid=false;
		return false;
	}
	return result;
}

static struct Operator{
	int oper;
	int prec;
	int is_bin;
}Operators[]={
	{'+',0,1},
	{'-',0,1},
	{'*',1,1},
	{'/',1,1},
	{'%',1,1},
	{'!',2,0},
	{POINT,2,0},
	{NEG,2,0},
	{AND,-2,1},
	{OR,-3,1},
	{NEQ,-1,1},
	{EQ,-1,1}
};
#define NR_OP (sizeof(Operators) / sizeof(Operators[0]) )


int locate_domin(int p,int q,bool *is_binary)
{
	int loc=p;
	int now_prec=32767;
	int ct_par=0;
	int i;
	*is_binary=true;
	for (i=p;i<=q;i++){
		if (tokens[i].type=='(') ct_par++;
		else if (tokens[i].type==')')ct_par--;
		else {
			int ii;
			if (ct_par!=0) continue;
			for (ii=0;ii<NR_OP;ii++){
				if (Operators[ii].oper==tokens[i].type){
					if((Operators[ii].prec<now_prec)||((Operators[ii].prec==now_prec)&&(Operators[ii].is_bin==1))){
						now_prec=Operators[ii].prec;
						loc=i;
						*is_binary=Operators[ii].is_bin;
					}
				}
			}
		}
	}
	if (now_prec==32767) {printf(RED"Invaild Expression!\nCannot locate dominant operator.\n"NONE);valid=false;return 0;}
	return loc;
}

uint32_t eval(int p, int q)
{
	if (!valid) return 0;
	printf("p=%d\tq=%d\n",p,q);
	if (p>q){
		/* Bad expression */
		//Assert(p<q,"Bad expression");
		printf(RED"Bad expression.\n"NONE);
		valid=false;
		return 0;


	}
	else if (p==q){
		/*Single token
		 *For now this token should be a number.
		 *Return the value of the number.
		 */
		int num=0;
		if (tokens[p].type==DEC){
			num=atoi(tokens[p].str);
			printf("%d:%d\t%s\n",p,num,tokens[p].str);
		}else if (tokens[p].type==HEX){
			sscanf(tokens[p].str,"%x",&num);
		}else{
			//tokens[nr_token].type=DEC; 
			char *s=tokens[p].str;
			//int reg;
			if (strcmp(s,"$eax")==0) num=cpu.eax;
			else if (strcmp(s,"$ebx")==0) num=cpu.ebx;
			else if (strcmp(s,"$ecx")==0) num=cpu.ecx;
			else if (strcmp(s,"$edx")==0) num=cpu.edx;
			else if (strcmp(s,"$esp")==0) num=cpu.esp;
			else if (strcmp(s,"$ebp")==0) num=cpu.ebp;
			else if (strcmp(s,"$esi")==0) num=cpu.esi;
			else if (strcmp(s,"$edi")==0) num=cpu.edi;
			else if (strcmp(s,"$eip")==0) num=cpu.eip;
			else if (strcmp(s,"$ax") ==0) num=cpu.gpr[0]._16;
			else if (strcmp(s,"$cx") ==0) num=cpu.gpr[1]._16;
			else if (strcmp(s,"$dx") ==0) num=cpu.gpr[2]._16;
			else if (strcmp(s,"$bx") ==0) num=cpu.gpr[3]._16;
			else if (strcmp(s,"$sp") ==0) num=cpu.gpr[4]._16;
			else if (strcmp(s,"$bp") ==0) num=cpu.gpr[5]._16;
			else if (strcmp(s,"$si") ==0) num=cpu.gpr[6]._16;
			else if (strcmp(s,"$di") ==0) num=cpu.gpr[7]._16;
			else if (strcmp(s,"$al") ==0) num=cpu.gpr[0]._8[0];
			else if (strcmp(s,"$cl") ==0) num=cpu.gpr[1]._8[0];
			else if (strcmp(s,"$dl") ==0) num=cpu.gpr[2]._8[0];
			else if (strcmp(s,"$bl") ==0) num=cpu.gpr[3]._8[0];
			else if (strcmp(s,"$ah") ==0) num=cpu.gpr[4]._8[1];
			else if (strcmp(s,"$ch") ==0) num=cpu.gpr[5]._8[1];
			else if (strcmp(s,"$dh") ==0) num=cpu.gpr[6]._8[1];
			else if (strcmp(s,"$bh") ==0) num=cpu.gpr[7]._8[1];
			else{	
			printf(RED"Invaild Expression!\nNot a number.\n"NONE);
			//assert(0);
			valid=false;
			return 0;
		}
		}
		return num;
	}
	else if (check_parentheses(p,q)==true){
		//printf("ok\n");
		if (!valid)return 0;
		return eval(p+1,q-1);
	}
	else{
		int op=0;bool is_binary=true;
		op=locate_domin(p,q,&is_binary);
		uint32_t val1=0;
		uint32_t val2=eval(op+1,q);
		if (is_binary){
			val1=eval(p,op-1);
		}
		printf("v1:%d\tv2:%d\n",val1,val2);
		switch (tokens[op].type){
			case '+':return val1+val2;
			case '-':return val1-val2;
			case '*':return val1*val2;
			case '/':if (val2==0) {printf(RED"Devided by zero!\n"NONE);valid=false;return 0;}
				 return val1/val2;
			case '%':return val1%val2;
			case '!':return !val2;
			case POINT:return swaddr_read(val2,4);
			case NEG:return -val2;
			case AND:return val1&&val2;
			case OR :return val1||val2;
			case EQ :return val1==val2;
			case NEQ:return val1!=val2;
			
		}
	
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int i;
#ifdef DDEBUG
	printf("%d\n",nr_token);
	for (i=0;i<nr_token;i++)
		printf("%d\t",tokens[i].type);
	
#endif
	/* TODO: Insert codes to evaluate the expression. */
	//int p=0,q=nr_token;
	for (i=0;i<nr_token;i++){
		if(tokens[i].type=='*'&&(i==0||(tokens[i-1].type!=DEC&&tokens[i-1].type!=HEX&&tokens[i-1].type!=REG&&tokens[i-1].type!=')')))
			tokens[i].type=POINT;
	}
	for (i=0;i<nr_token;i++){
		if(tokens[i].type=='-'&&(i==0||(tokens[i-1].type!=DEC&&tokens[i-1].type!=HEX&&tokens[i-1].type!=REG&&tokens[i-1].type!=')')))
			tokens[i].type=NEG;
	}
	uint32_t result=eval(0,nr_token-1);
	if (!valid) *success=false;
	return result;
	panic("please implement me");
	return 0;
}
