#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*

#include<stdio.h>

main()
{
	printf(":%s:\n", "Hello, world!");
	printf("Hello, world!");
	printf(":%15s:\n", "Hello, world!");
	printf(":%.10s:\n", "Hello, world!");
	printf(":%-10s:\n", "Hello, world!");
	printf(":%-15s:\n", "Hello, world!");
	printf(":%.15s:\n", "Hello, world!");
	printf(":%15.10s:\n", "Hello, world!");
	printf(":%-15.10s:\n", "Hello, world!");
	printf("The color: %s\n", "blue");
	printf("First number: %d\n", 12345);
	printf("Second number: %04d\n", 25);
	printf("Float number: %3.2f\n", 3.14159);
	int Fahrenheit;

	for (Fahrenheit = 0; Fahrenheit <= 300; Fahrenheit = Fahrenheit + 20)
		printf("%3d %06.3f\n", Fahrenheit, (5.0/9.0)*(Fahrenheit-32));

	int a,b;
		float c,d;

		a = 15;
		b = a / 2;
		printf("%d\n",b);
		printf("%3d\n",b);
		printf("%03d\n",b);

		c = 15.3;
		d = c / 3;
		printf("%3.2f\n",d);


*/

#define KBASE 1
#define KDOUBLE 2
#define KINT 3
#define KSTRING 4

#define MAXLEN 4

typedef struct tok 
{
	int kind;
	int min_width;
	int max_width;
	int dynamic;
	int precision;
	int others;
	char *unformated_message;
	char prefix;
	char postfix;
	struct tok* next;
} tok;

tok* head = NULL;
tok* tail = NULL;



int max_int;
#define CREATE(result, type, nsize)	\
do                                                          \
{                                                       \
	(result) = (type *) malloc(sizeof(type)* (nsize));  \
	if (!(result)) {	\                                        
		fprintf(stderr, "Out of RAM :: Failed to create "); \
		exit(1);                                            \
	}                                                       \
} while(0)                                                         

int eprint(char* message){
	fprintf(stderr, "%s", message);
	exit(EXIT_FAILURE);
}

#define ADDTOLIST(m_token, head, tail) \                  
do                                     \
{                                      \
	if(!(head)){                       \
		(head) = (m_token);            \
		(tail) = (m_token);            \
	}else{                             \
		(tail)->next = (m_token);      \
		(tail) = (tail)->next;         \
	}                                  \
} while(0)
	
	
void assert_len(int index, size_t nlen){
	if(index >= nlen)
		eprint("Index out of range");
}

char* recreate(char* buff, int arr_max, int count){
	char * snew;
	//printf("\nsee%ddee\n", arr_max);
	CREATE(snew, char, arr_max);
	int oldmax = arr_max/2;
	for(int i = 1; i < count; i++){
		snew[arr_max - i] = buff[oldmax - i];
	}
	printf("%d", count);
	//free(buff);
	return snew;
}

char* int_string(int* ncount, int ival, char* buffer)
{
	int hhold, rhold, thold;
	max_int = MAXLEN;
	hhold = ival / 10;
	rhold = ival % 10;
	buffer[max_int - *ncount] = rhold + '0';
	thold = hhold;
	*ncount = *ncount + 1;
	while(thold) {
		if (0 == (max_int - *ncount )){
			max_int = max_int*2;
			char* nbuffer = recreate(buffer,  max_int, *ncount);
			free(buffer);
			buffer = nbuffer; 
		}	
		hhold = thold / 10;
		rhold = thold % 10;
		buffer[max_int - *ncount] = rhold + '0';
		*ncount = *ncount + 1;
		thold = hhold;	
	}
	printf("foo%djooo", *ncount); 
	return buffer;
}

int get_type(int c){
	
	switch(c){
		case 'd':
				return KINT;
		case 'f':
				return KDOUBLE;
		default:
				return KSTRING;									
	}			
}

int get_int(char c, int* i, char val[], size_t nlen){
	int num = 0;
	num = num * 10 + c - '0';
	*i = *i + 1;
	printf("%d, out range %d\n", *i, (int)nlen);
	assert_len(*i, nlen);
	c = val[*i];
	while(isdigit(c)){
		num = num * 10 + c - '0';
		*i = *i + 1;
		assert_len(*i, nlen);
		c = val[*i];
	}
	printf("\n%d Result\n", num);
	return num;
}

void extractToken(tok* ntoken, char* val, int state, int* count, size_t len){
	int  lcount, acc, pflag , eflag ,dsflag ,sfalg ,iflag ;
	lcount = acc = pflag = eflag = dsflag = sfalg = iflag = 0;
	char c = val[*count];
	if(isdigit(c) && c != '0'){
		acc = acc * 10 + c - '0';
		lcount = 1;
		while(*count + lcount < len) {
			c = val[*count + lcount];
			switch(c){
				case 'd':
					dsflag = dsflag? eprint("Mixed format: Integer does not have precision width"): dsflag;
					ntoken->min_width = acc;
					ntoken->kind = KINT; 
					eflag = 1;
					acc = 0;
					break;
				case '.':
					iflag = iflag ? eprint("Mixed format: Integer does not have precision width"): iflag;
					ntoken->min_width = acc;
					acc = 0;
					dsflag = 1;
					pflag = 1;
					lcount++;
					break;
				case 's':
					if(dsflag){
						ntoken->max_width = acc;
						ntoken->min_width  = ntoken->precision;
					}
					ntoken->kind = KSTRING;
					eflag = 1;
					acc = 0; 
					break;
				case 'f':
					if(dsflag){
						ntoken->precision = acc;
						ntoken->kind = KDOUBLE; 
					}
					eflag = 1;
					acc = 0;
					break;
				default:
					if(!isdigit(c)){
						eprint("Expected a digit but found something else");
					}	
					acc = acc * 10 + c - '0';
					lcount++;
			}
			if (eflag) break;
		}
		if((*count + lcount > len) || acc || pflag ){
			eprint("Poorly structured format:");
		}
		*count = *count + lcount;
	}else {
		eprint("Poorly structured format:");
	}
	return;
}


			
void ftokenize(char* val){
	int i, nlen = strlen(val);
	char c;
	i = 0;
	while(i < nlen){
		c = val[i];
		tok* ntoken;
		CREATE(ntoken, tok, 1);
		//memset(ntoken, 0, sizeof(ntoken));
		ntoken->dynamic = -1;
		ntoken->precision = -1;
		ntoken->others = 1;
		ntoken->min_width = 0;
		ntoken->unformated_message = NULL;
		if(c == '%'){
			i++;
			assert_len(i, nlen);
			c = val[i];
			switch(c){
				case 'd':
					//process integer
					ntoken->kind = KINT;
					ntoken->others = 0;
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case 's':
					//process string
					ntoken->kind = KSTRING;
					ntoken->others = 0;
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case 'f':
					//process double
					ntoken->kind = KDOUBLE;
					ntoken->others = 0;
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case '*':
					//process dymanic width
					i++;
					assert_len(i, nlen);
					c = val[i];
					if(c == 'd'){
						ntoken->kind = KINT;	
					}else if (c == 's'){
						ntoken->kind = KSTRING;
					} else {
						eprint("Type conversion not allowed");
					}
					ntoken->dynamic = 1;
					ntoken->others = 0;
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case '-':
					i++;
					assert_len(i, nlen);
					extractToken(ntoken, val, 1, &i, nlen);
					ntoken->postfix = '-';
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case '0':
					i++;
					assert_len(i, nlen);
					extractToken(ntoken, val, 1, &i,nlen);
					ntoken->prefix = '0';
					if(ntoken->kind != KDOUBLE || ntoken->kind != KINT){
						eprint("Type specifier mismatch");
					}
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				case '.':
					i++;
					assert_len(i, nlen);
					extractToken(ntoken, val, -1, &i, nlen);
					ntoken->postfix = 'u';
					ntoken->prefix = 'u';
					ADDTOLIST(ntoken, head, tail);
					i++;
					break;
				default:
					if(isdigit(c)){
						ntoken->min_width = get_int(c, &i, val, nlen);
						c = val[i];
						if (c == '.'){
							i++;
							assert_len(i, nlen);
							c = val[i];
							ntoken->precision = get_int(c, &i, val, nlen);
							c = val[i];
							if(strchr("dfs", c)){
								ntoken->kind = get_type(c);
								ADDTOLIST(ntoken, head, tail);
								i++;
							}else {
								eprint("Failed to interpret type specifier");
							}
						}else if (strchr("dfs", c)){
							ntoken->kind = get_type(c);
							ADDTOLIST(ntoken, head, tail);
							i++;
						}else {
							eprint("Failed to interpret type specifier");
						}	
					}else{
						eprint("Failed to interpret string format");
					}
			}
		}else {
				
			int acount = 1;
			int hpoint = i;
			i++;
			//printf("precision integer99 %d", ntoken->precision);
			assert_len(i, nlen);
			c = val[i];
			while(c != '%'){
				acount++;
				i++;
				if(i == nlen - 1){
					i++;
					break;
				}
				c = val[i];
			}
			CREATE(ntoken->unformated_message, char, acount);
			strncpy(ntoken->unformated_message, val + hpoint, acount);
			ntoken->others  = -1;
			ntoken->min_width = acount;
			ADDTOLIST(ntoken, head, tail);
		}

	}
	return;
}

/*
int nfprint(char *des, size_t csize, char *format, ...) {
	ftokenize(format);
	char c;
	size_t remaining_size = csize;
	va_list args;
    va_start(args, format);
    int count, tcount;
    tcount = 0;
    count = 0;
	while((c = *format) != '\0') {
		format++;
		switch(c){
			case '%':
				{
				switch(*format){
					case 'd':
					{
						//read integer
						int ival = va_arg(args, int);
						tcount =  count > 0 ? tcount + count - 1 : 0;
						count = 1;
						char *chold, *tem;
						CREATE(chold, char, MAXLEN);
						tem = int_string(&count, ival, chold);
						if((count > 1) && (count + 1 <= (int)remaining_size)&& (count - 1 < max_int)){
							strncpy(des,tem + (max_int - (count - 1)), count - 1);
							des  = des + count - 1;
							remaining_size -= (size_t)count;
							tcount = tcount + count - 1;
						}
						format++;
						//printf("\n%s\n", des);
						free(tem);
						//free(chold);
						break;
					}
					case 's':
					{
						//read array of characters
						char *sval = va_arg(args, char *);	
						int arr_len = strlen(sval);
						if(arr_len + 1 <= (int)remaining_size){
							strncpy(des, sval, arr_len);
							des = des + arr_len ;
							remaining_size -= (size_t)arr_len;
							tcount =  count > 0 ? tcount + count - 1 : 0;
							count = arr_len;
						}
						format++;
						break;
					}
					case 'f':
						{
							double dval = va_arg(args, double);
							double inPart;
							int i_fraction = (int)round(fabs(modf(dval, &inPart)) * 1e5); 
							//http://stackoverflow.com/questions/17259315/getting-the-fractional-part-of-a-double-value-in-integer-without-losing-precisio
							int i_dval = (int) inPart;
							tcount =  count > 0 ? tcount + count - 1 : 0;
							count = 1;
							//int max_int = 16;
							char *chold;
							CREATE(chold, char, MAXLEN);
							char *fhold;
							CREATE(fhold, char, MAXLEN);
							char *tem; // = chold;
							tem = int_string(&count, i_dval,  chold);
							if((count > 1) && (count + 1 <= (int)remaining_size)&& (count - 1 < max_int)){
								strncpy(des,tem + (max_int - (count - 1) ), count - 1);
								des  = des + count - 1;
								remaining_size -= (size_t)count;
							}
							free(tem);
							*des = '\0';
							printf("Fool %s", des);
							*des = '.';
							des++;
							tcount = tcount + count - 1;
							count = 1;
							//tem = fhold;
							tem = int_string(&count, i_fraction, fhold);
							if((count > 1) && (count + 1 <= (int)remaining_size)&& (count - 1 < max_int)){
								strncpy(des,tem + (max_int - (count - 1) ), count - 1);
								des  = des + count - 1;
								remaining_size -= (size_t)count;
							}
							format++;
							free(tem);
							//free(fhold);
							//free(tem);
							break;
						}
					default:
						printf("You are bad!");
						
					}
				break;
				}
			case '\t':
			    {
				if(remaining_size > 1){
					*des = '\t';
					des++;
					count++;
				}
				break;
			   }
			case ' ':
			    {
				if(remaining_size > 1){
					*des = ' ';
					des++;
					count++;
				}
				break;
				}
			default:
			   if(isalpha(c)) {
				   *des = c;
				   des++;
				   count++;
			   } else{
				printf("Can't figure out your format");
			}
		}
	}
	*des = '\0';
	va_end(args);
	return count + tcount - 1;
}

*/

int main(void) {
	//size_t m_size = 1024;
	//char *result;
	//CREATE(result, char, m_size);
	//char *dss;
	//CREATE(dss, char, m_size);
	//printf("step 0");
	//nfprint(result, m_size, "%d %s %s %s %s %d", 227932342, "Home boy", "House girl", "New York", "Zip Code", 90012);
	//nfprint(dss, m_size, "%s %f", "Fraction", 8999.22);
	
	ftokenize("Hole in the head %d %s %7.4f");
	//printf("\n%s\n", result);
	//printf("\n%s\n", dss);

	//free(result);
	//free(dss);
	int i = 0;
	tok* current;
	while(head){
		current = head;
		head = head->next;
		i++;
		if(current->unformated_message){
			printf("\n%s\n", current->unformated_message);
			free(current->unformated_message);
		}
		free(current);
	}
	printf("\n%d\n", i);
	
	return 0;
} 
	
				
							
						
						
						
					
						
						 
	
