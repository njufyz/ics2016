#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include "FLOAT.h"

extern char _vfprintf_internal;
extern char _fpmaxtostr;
extern int __stdio_fwrite(char *buf, int len, FILE *stream);
extern char _ppfs_setargs;
char* itoa(int,char*,int);
__attribute__((used)) static int format_FLOAT(FILE *stream, FLOAT fn) {
	/* TODO: Format a FLOAT argument `f' and write the formating
	 * result to `stream'. Keep the precision of the formating
	 * result with 6 by truncating. For example:
	 *              f          result
	 *         0x00010000    "1.000000"
	 *         0x00013333    "1.199996"
	 */
    long long f = fn;
	char buf[80];
  //  memset((void*)buf,0,80);
    buf[0]=0;
    int len;
    int flag=0;
    if(f<0) 
    {
        flag = 1;
        f=-f;
    }
   
    int a,b;
    a = f >> 16;
    f -= (a<<16);
    b = (1000000*f)/(1<<16);
    char tempa[20],tempb[20],tempc[2];
    tempc[0] = (flag)?'-':0;
    tempc[1]=0;
//    sprintf(tempa,"%d",a);
//    sprintf(tempb,"%d",b);
   char temp[80];
   strcpy(tempa,itoa(a,temp,10));
   strcpy(tempb,itoa(b,temp,10));
    if(f==0) strcpy(tempb,"000000");
    strcat(buf,tempc);
    strcat(buf,tempa);
    strcat(buf,".");
    strcat(buf,tempb);
    len = strlen(buf);
    
    memset(buf,0,80);
    strcpy(buf,"1.000000");
    len=8;
    return __stdio_fwrite(buf, len, stream);
}

static void modify_vfprintf() {
	/* TODO: Implement this function to hijack the formating of "%f"
	 * argument during the execution of `_vfprintf_internal'. Below
	 * is the code section in _vfprintf_internal() relative to the
	 * hijack.
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = _fpmaxtostr(stream,
				(__fpmax_t)
				(PRINT_INFO_FLAG_VAL(&(ppfs->info),is_long_double)
				 ? *(long double *) *argptr
				 : (long double) (* (double *) *argptr)),
				&ppfs->info, FP_OUT );
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif

	/* You should modify the run-time binary to let the code above
	 * call `format_FLOAT' defined in this source file, instead of
	 * `_fpmaxtostr'. When this function returns, the action of the
	 * code above should do the following:
	 */

#if 0
	else if (ppfs->conv_num <= CONV_A) {  /* floating point */
		ssize_t nf;
		nf = format_FLOAT(stream, *(FLOAT *) *argptr);
		if (nf < 0) {
			return -1;
		}
		*count += nf;

		return 0;
	} else if (ppfs->conv_num <= CONV_S) {  /* wide char or string */
#endif
       uint32_t p =(uint32_t) &_vfprintf_internal + 0x80488cc - 0x80485c6;
       int o =(int) &_fpmaxtostr - (int)&format_FLOAT ;
       p = p + 1;
       int i = *(int *)p;
//     mprotect((void *)((p-0x100) & 0xfffff000), 4096*2, PROT_READ | PROT_WRITE | PROT_EXEC);
      *(int*)p = *(int*)p - o;        //change call
      uint32_t sub = (uint32_t) &_vfprintf_internal + 0x80488df - 0x80485e6;
      *(uint32_t*)(sub) = 0xdb08ec83; //correct esp
      uint32_t push = (uint32_t) &_vfprintf_internal + 0x80488e2 - 0x80485e6;
     *(uint32_t*)(push) = 0xff9032ff;  //push (%edx)
    
     uint32_t finstr = (int)&_vfprintf_internal + 0x8048e2d - 0x8048b49;
     *(uint32_t*)finstr = 0x02eb9090;

     finstr = (int)&_vfprintf_internal + 0x8048e31 - 0x8048b49;
     *(uint32_t*)finstr = 0x53539090;  //clear float instrs

    }


static void modify_ppfs_setargs() {
	/* TODO: Implement this function to modify the action of preparing
	 * "%f" arguments for _vfprintf_internal() in _ppfs_setargs().
	 * Below is the code section in _vfprintf_internal() relative to
	 * the modification.
	 */

#if 0
	enum {                          /* C type: */
		PA_INT,                       /* int */
		PA_CHAR,                      /* int, cast to char */
		PA_WCHAR,                     /* wide char */
		PA_STRING,                    /* const char *, a '\0'-terminated string */
		PA_WSTRING,                   /* const wchar_t *, wide character string */
		PA_POINTER,                   /* void * */
		PA_FLOAT,                     /* float */
		PA_DOUBLE,                    /* double */
		__PA_NOARG,                   /* non-glibc -- signals non-arg width or prec */
		PA_LAST
	};

	/* Flag bits that can be set in a type returned by `parse_printf_format'.  */
	/* WARNING -- These differ in value from what glibc uses. */
#define PA_FLAG_MASK		(0xff00)
#define __PA_FLAG_CHAR		(0x0100) /* non-gnu -- to deal with hh */
#define PA_FLAG_SHORT		(0x0200)
#define PA_FLAG_LONG		(0x0400)
#define PA_FLAG_LONG_LONG	(0x0800)
#define PA_FLAG_LONG_DOUBLE	PA_FLAG_LONG_LONG
#define PA_FLAG_PTR		(0x1000) /* TODO -- make dynamic??? */

	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			case (PA_INT|PA_FLAG_LONG):
				GET_VA_ARG(p,ul,unsigned long,ppfs->arg);
				break;
			case PA_CHAR:	/* TODO - be careful */
				/* ... users could use above and really want below!! */
			case (PA_INT|__PA_FLAG_CHAR):/* TODO -- translate this!!! */
			case (PA_INT|PA_FLAG_SHORT):
			case PA_INT:
				GET_VA_ARG(p,u,unsigned int,ppfs->arg);
				break;
			case PA_WCHAR:	/* TODO -- assume int? */
				/* we're assuming wchar_t is at least an int */
				GET_VA_ARG(p,wc,wchar_t,ppfs->arg);
				break;
				/* PA_FLOAT */
			case PA_DOUBLE:
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			case (PA_DOUBLE|PA_FLAG_LONG_DOUBLE):
				GET_VA_ARG(p,ld,long double,ppfs->arg);
				break;
			default:
				/* TODO -- really need to ensure this can't happen */
				assert(ppfs->argtype[i-1] & PA_FLAG_PTR);
			case PA_POINTER:
			case PA_STRING:
			case PA_WSTRING:
				GET_VA_ARG(p,p,void *,ppfs->arg);
				break;
			case __PA_NOARG:
				continue;
		}
		++p;
	}
#endif

	/* You should modify the run-time binary to let the `PA_DOUBLE'
	 * branch execute the code in the `(PA_INT|PA_FLAG_LONG_LONG)'
	 * branch. Comparing to the original `PA_DOUBLE' branch, the
	 * target branch will also prepare a 64-bit argument, without
	 * introducing floating point instructions. When this function
	 * returns, the action of the code above should do the following:
	 */

#if 0
	while (i < ppfs->num_data_args) {
		switch(ppfs->argtype[i++]) {
			case (PA_INT|PA_FLAG_LONG_LONG):
			here:
				GET_VA_ARG(p,ull,unsigned long long,ppfs->arg);
				break;
			// ......
				/* PA_FLOAT */
			case PA_DOUBLE:
				goto here;
				GET_VA_ARG(p,d,double,ppfs->arg);
				break;
			// ......
		}
		++p;
	}
#endif
  uint32_t  p = (int)&_ppfs_setargs + 0x801148 - 0x8010d7;
  *(uint32_t*)p = 0x000042eb;
  
}
void init_FLOAT_vfprintf() {
	modify_vfprintf();
	modify_ppfs_setargs();
}


char *itoa(int value,char *string,int radix)  
{  
    //    assert(string!=NULL);  
        char tmp[32]={'\0'};  
        int tmpval=value;  
        int i,j;  
        for(i=0;i<32;i++)  
        {                  
            tmp[i]=(tmpval%radix)+'0';  
            tmpval=tmpval/radix;  
            if(tmpval==0)  
                break;  
        }  
        if(value<0)tmp[++i]='-';  
        for(j=0;i>=0;i--)  
            string[j++]=tmp[i];  
        string[j]='\0';  
        return string;  
}  


