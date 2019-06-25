#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFLEN 10240
//#define DEBUG

struct tmp_buf{
	char buf[BUFLEN];
	int len;
	int index;
};

//随机序列
int main(int argc, char **argv){
	int s_fd, c_fd;
	socklen_t clie_addr_len;
	struct sockaddr_in serv_addr, clie_addr;
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(12345);
	bind(s_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(s_fd, 5);
	clie_addr_len = sizeof(clie_addr_len);
	c_fd = accept(s_fd, (struct sockaddr *)&clie_addr, &clie_addr_len);

	char resp[16];
	read(c_fd, resp, 16);
	int len = atoi(resp);
	printf("recv len: %d\n", len);
	unsigned char *buffer = (unsigned char *)malloc(len);
	struct tmp_buf t;
	int n;
	while((n=read(c_fd, &t, sizeof(struct tmp_buf)))>0){
		memcpy(buffer+BUFLEN*t.index, t.buf, t.len);
		if(t.len<BUFLEN){
				
		}

		printf("recv %d seg, len:%d\n", t.index, t.len);
	}
	close(c_fd);
	close(s_fd);
	int w_fd=open("new.file", O_WRONLY|O_CREAT|O_TRUNC, 0644);	
	unsigned int index = len/BUFLEN + ((len%BUFLEN)==0? 0: 1);
#ifdef DEBUG
	int fd_w=open("r_test", O_WRONLY|O_CREAT|O_TRUNC, 0644); 
	write(fd_w, buffer, len);
	close(fd_w);
#endif
	for(int i=0; i<index; i++){
		if(len-i*BUFLEN<BUFLEN){
			write(w_fd, buffer+i*BUFLEN, len%BUFLEN);
		}
		else{
			write(w_fd, buffer+i*BUFLEN, BUFLEN);
		}
	}
	close(w_fd);

    return 0;
}
