//
//  wsy.h
//  wsy
//
//  Created by Devin on 6/28/19.
//  Copyright © 2019 Devin. All rights reserved.
//

#ifndef wsy_h
#define wsy_h

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFLEN 10240

typedef struct tmp_buf{
    char buf[BUFLEN];
    int len;
    int index;
    
} tmp_buf;

void send_(FILE *file, char *ip, int port);
void recv_(int port, FILE *file);
void transfer_(char *ip, int port1, int port2);

#endif /* wsy_h */
