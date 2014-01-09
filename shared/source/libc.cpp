#include "../include/libc.h"
#include "../include/time.h"
signed int atoi( const char *nptr	)
{
	int			c;
	signed int	total;
	int			sign;
	
	while (*nptr==' ')
		++nptr;

	c = (int)(unsigned char)*nptr++;
	sign = c;           // save sign indication 
	if (c == '-' || c == '+')
		c = (int)(unsigned char)*nptr++;    // skip sign 
	
	total = 0;
	
	while ((c>='0') && (c<='9'))
	{
		total = 10 * total + (c - '0');     // accumulate digit 
		c = (int)(unsigned char)*nptr++;    // get next char 
	}
	
	if (sign == '-')
		return (int)-total;// return result, negated if necessary 
	else
		return (int)total;   
}


void generateRandomFileName(char *pFileName, int n) {
     char random[]="abcdefghijklmnopqrstuvwxyz0123456789";
     int i;
     srand((unsigned)time(NULL));

     for(i=0;i<(n-4);i++) {
         pFileName[i]=random[rand() % (36)];
     }
     pFileName[n-4]='.';
     pFileName[n-3]='t';
     pFileName[n-2]='m';
     pFileName[n-1]='p';
     pFileName[n]=0;
}

bool inStr(char *str, char car) {
     int i=0;
     while (str[i])
           if (str[i++]==car)
              return true;
     return false;
}


