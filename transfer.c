//
//  transfer.c
//  wsy
//
//  Created by Devin on 6/28/19.
//  Copyright © 2019 Devin. All rights reserved.
//

#include "wsy.h"
//port1: 监听端口， port2: 目标端口
void transfer_(char *ip, int port1, int port2){
    FILE *tmp = tmpfile();
    recv_(port1, tmp);
    send_(tmp, ip, port2);
    fclose(tmp);
}
