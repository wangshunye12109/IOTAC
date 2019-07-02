//
//  main.c
//  wsy
//
//  Created by Devin on 6/28/19.
//  Copyright © 2019 Devin. All rights reserved.
//

#include <stdio.h>
#include "wsy.h"

int main(int argc, const char * argv[]) {
    // insert code here...     
  char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  time_t t;
  struct tm *p;
  time(&t);
  p = localtime(&t);
  printf ("%d/%02d/%02d ", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
  printf("%s %02d:%02d:%02d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	FILE  *s = fopen(argv[1], "r");
	send_(s, "127.0.0.1", 12345);
	fclose(s);
    return 0;
}
