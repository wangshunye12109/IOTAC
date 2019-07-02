//
//  recv.c
//  wsy
//
//  Created by Devin on 6/28/19.
//  Copyright © 2019 Devin. All rights reserved.
//

#include "wsy.h"
void recv_(int port, FILE *file){
    int s_fd, c_fd;
    struct sockaddr_in serv_addr, client_addr;
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port);
    
    bind(s_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(s_fd, 5);
    
    socklen_t client_addr_len;
    client_addr_len = sizeof(client_addr);
    c_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    
    union resp{
        int a;
        char b[4];
    } res;
    
    read(c_fd, res.b, 4);
    printf("%d\n", res.a);
    FILE *tmp = tmpfile();
//    int fd_tmp = open("/tmp/buffer", O_CREAT|O_RDWR|O_TRUNC, 0644);
    int fd_tmp  = fileno(tmp);
    int n, sum=0;
    tmp_buf t;
    while((n = read(c_fd, &t, BUFLEN))>0){
        write(fd_tmp, (unsigned char*)&t, n);
        sum+=n;
    }
    printf("sum: %d\n", sum);
    lseek(fd_tmp, 0, SEEK_SET);
//    int fd_newfile=open(filepath, O_CREAT|O_WRONLY|O_TRUNC, 0644);
    int fd_newfile = fileno(file);
    unsigned int index = res.a/BUFLEN + ((res.a%BUFLEN)==0? 0: 1);
    printf("index: %d\n", index);
    while((n = read(fd_tmp, &t, sizeof(tmp_buf)))>0){
        lseek(fd_newfile, t.index*BUFLEN, SEEK_SET);
        printf("index: %d, len: %d\n", t.index, t.len);
        write(fd_newfile, t.buf, t.len);
    }
    fclose(tmp);
//    close(fd_newfile);
    close(s_fd);
}
