/* 
 * a simplified snprintf() for HiveBio Ministat
 * 
 * for license see isnprintf.c
 */

#ifndef ISNPRINTF_H
#define ISNPRINTF_H
#include <inttypes.h>

int isnprintf(char *s, int buf_len, const char *pszFmt,...);

#endif
/* [] END OF FILE */
