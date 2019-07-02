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
	FILE * file = fopen("/tmp/recv", "w");
    recv_(12346, file);
	fclose(file);
    return 0;
}
