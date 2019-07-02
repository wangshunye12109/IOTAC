//
//  send.c
//  wsy
//
//  Created by Devin on 6/28/19.
//  Copyright © 2019 Devin. All rights reserved.
//

#include "wsy.h"
//随机序列
int* randpermC(int N){
    int *arr = (int*)malloc(N*sizeof(int));
    int *ret_arr = (int*)malloc(N*sizeof(int));
    int count = 0;
    memset(arr,0,N*sizeof(int));
    memset(ret_arr,0,N*sizeof(int));
    srand(time(NULL));
    while(count<N){
        int val = rand()%N;
        if (!arr[val]){
            arr[val]=1;
            ret_arr[count] = val;
            ++count;
        }
    }
    free(arr);
    arr = NULL;
    return ret_arr;
}

void send_(FILE *file, char *ip, int port){
    union resq{
        int a;
        char b[4];
    } res;
    
//    int fd_r = open(filepath, O_RDONLY);
    int fd_r = fileno(file);
    if(fd_r == -1){
        perror("open");
    }
    off_t len = lseek(fd_r, 0, SEEK_END);
    printf("file lengh: %lld\n", len);
    lseek(fd_r, 0, SEEK_SET);
    unsigned int index = len/BUFLEN + ((len%BUFLEN)==0? 0: 1);
    int *shuffle=randpermC(index);
    int s_fd;
    struct sockaddr_in serv_addr;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //传入的第二个参数为ip
    inet_pton(AF_INET, ip, &serv_addr.sin_addr.s_addr);
    //连接端口
    serv_addr.sin_port = htons(port);
    int err =
    connect(s_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(err == -1){
        perror("connect");
        exit(-1);
    }
    printf("err: %d, index: %d\n", err, index);
    
    tmp_buf t;
    
    res.a = len;
    write(s_fd, res.b, 4);
    for(int i=0; i<index; i++){
        int len_r;
        lseek(fd_r, shuffle[i]*BUFLEN, SEEK_SET);
        len_r = read(fd_r, t.buf, BUFLEN);
        t.index = shuffle[i];
        t.len = len_r;
        write(s_fd, &t, sizeof(t));
        printf("index: %d, len: %d\n", t.index, t.len);
    }
//    close(fd_r);
    close(s_fd);
}
