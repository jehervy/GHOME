/*
 * utils.h
 *
 *  Created on: 29 janv. 2012
 *      Author: miroof
 */

#ifndef UTILS_H_
#define UTILS_H_

char xtod(char c) {
 if (c>='0' && c<='9') return c-'0';
 if (c>='A' && c<='F') return c-'A'+10;
 if (c>='a' && c<='f') return c-'a'+10;
 return c=0;        // not Hex digit
}

int HextoDec(const char *hex)
{
    if (*hex==0) return 0;
    return  HextoDec(hex-1)*16 +  xtod(*hex) ;
}

int xstrtoi(const char *hex)      // hex string to integer
{
    return HextoDec(hex+strlen(hex)-1);
}

#endif /* UTILS_H_ */
