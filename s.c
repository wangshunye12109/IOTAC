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
	FILE  *s = fopen(argv[1], "r");
	send_(s, "127.0.0.1", 12345);
	fclose(s);
    return 0;
}
