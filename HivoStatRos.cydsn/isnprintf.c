/* 
 * a simplified snprintf() for HiveBio Ministat
 * 
 * derived from http://www.cypress.com/?id=4&rID=81086, license terms unknown
 * modifications (c) C. Harrison BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 * isnprintf() only can print string with %s,%d,%c,%x. Numerical arguments are assumed to be int32.
 */
 
#include "isnprintf.h"

static void add_char(char **ps, int* pchars_generated, int buf_len, char ch)
{
   if((*pchars_generated)++ < buf_len)
   {
       *(*ps)++ = ch;
   }
   if (*pchars_generated == buf_len)
   {
       *(*ps-1) = 0;
   }
}

      
static char* change(uint8_t Index)
{
    return (char*)("0123456789abcdef"+Index);
}


int isnprintf(char *s, int buf_len, const char *pszFmt,...)
{
    int chars_generated=0;
    char *pszVal;
    uint32_t iVal, xVal, i = 0, index = 1;
    uint8_t buffer[12];
    char cVal;
    uint32_t *pArg;
    pArg =(uint32_t *)&pszFmt;

    while(*pszFmt)
    {
        if('%' != *pszFmt)
        {
            add_char(&s, &chars_generated, buf_len, *pszFmt);
            pszFmt++;
            continue;
        }
        pszFmt++;

        if(*pszFmt == 's')
        {
            pszVal = (char*)pArg[index++];
            for(; *pszVal != '\0'; pszVal++)
                add_char(&s, &chars_generated, buf_len, *pszVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'd')
        {
            iVal = pArg[index++];
            // handle negative sign
            if (iVal & 0x80000000) {
              iVal = ~iVal + 1;
              add_char(&s, &chars_generated, buf_len, '-');
            }
            i = 0;
            do{
                buffer[i++] = iVal % 10;
                iVal /= 10;
            }while(iVal);
            while(i > 0)
            {
                i--;
                add_char(&s, &chars_generated, buf_len, *change(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'c')
        {
            cVal = (char)pArg[index++];
            add_char(&s, &chars_generated, buf_len, cVal);
            pszFmt++;
            continue;
        }
        if(*pszFmt == 'x')
        {
            xVal = pArg[index++];
            i = 0;
            do{
                buffer[i++] = xVal % 16;
                xVal /= 16;
            }while(xVal);
            if(i%2!=0)
                buffer[i++]=0;
            if(i<2)
                buffer[i++]=0;

            while(i > 0)
            {
                i--;
                add_char(&s, &chars_generated, buf_len, *change(buffer[i]));
            }
            pszFmt++;
            continue;
        }
        if(pszFmt == '\0')
        {
            break;
        }
    }
    if (chars_generated < buf_len)
    {
        *s = '\0';
    }
    return chars_generated;
}
