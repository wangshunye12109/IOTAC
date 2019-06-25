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
int main(int argc, char **argv){
	if(argc<3){
		fprintf(stderr, "file not fond or ip not given\n");
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(!fd){
		perror("file:");
		return -1;
	}
#ifdef DEBUG
	printf("getting filesize...\n");
#endif
	//获取文件长度
	off_t len = lseek(fd, 0, SEEK_END); 
	//重置文件游标
	lseek(fd, 0, SEEK_SET); 
#ifdef DEBUG
	printf("getting filesize end...\n");
#endif

	unsigned char *heap = (unsigned char*)malloc(len);
	memset(heap, 0, len);
	unsigned char *tmp = heap;
	int n;
	//n=-1则文件大小为BUFSIZE的整数倍, 否则返回最后一次读取大小
	while((n = read(fd, tmp, BUFLEN)) == BUFLEN){
#ifdef DEBUG
		fprintf(stderr, ".");
#endif
		tmp+=BUFLEN;
	}
#ifdef DEBUG
	//int fd_w;
	//fd_w = open("test", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	//write(fd_w, heap, len);
	//close(fd_w);
	printf("%d\n", n);
	for(int i=0; i<len; i++){
		//fprintf(stderr, "%X", heap[i]);	
	}
#endif
#ifdef DEBUG
	printf("len: %llu\n",len);
#endif
	unsigned int index = len/BUFLEN + ((len%BUFLEN)==0? 0: 1);

	int *shuffle=randpermC(index);
#ifdef DEBUG
	for(int i=0; i< index; i++)
		printf("%d\n", shuffle[i]);
#ifdef EXIT
	return 0;
#endif
#endif
	struct tmp_buf *buffer = (struct tmp_buf*)malloc(sizeof(struct tmp_buf));
#ifdef DEBUG
	printf("begin create socket...\n");
#endif
	//创建socket
	int s_fd;
	struct sockaddr_in serv_addr;
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&serv_addr, sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	//传入的第二个参数为ip
	inet_pton(AF_INET, argv[2], &serv_addr.sin_addr.s_addr);
	//连接端口
	serv_addr.sin_port = htons(12345);
	connect(s_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	//sleep(5);
	//传输数据
	printf("index: %d\n", index);
	//向接收端发送文件总大小, 并需要服务端回复 OK
	char req[16];
	sprintf(req, "%lld" , len);
	printf("%s\n", req);
	write(s_fd, req, 16);  
	
	for(int i=0; i<index; i++){
		printf("第%d次发送。。。\n", i);
		memset(buffer, 0, sizeof(struct tmp_buf));
#ifdef DEBUG
		for(int i=0; i<sizeof(struct tmp_buf); i++){
			//fprintf(stderr, "%X", ((unsigned char *)buffer)[i]);
		}
		//printf("\n");
#endif
		//shuffle[i]为乱序
		if(shuffle[i] == index-1){
			fprintf(stderr, "if 序号为: %d\n", shuffle[i]);
			//最后一个大小
			if(len%BUFLEN==0){
				memcpy(buffer->buf, heap+shuffle[i]*BUFLEN, BUFLEN);
				buffer->len = BUFLEN;
			}
			else{
				memcpy(buffer->buf, heap+shuffle[i]*BUFLEN, BUFLEN);
				memset(&(buffer->buf[n]), 0, BUFLEN-n);
#ifdef DEBUG
			//fprintf(stderr, "if end...\n");
#endif
				buffer->len=n;
			}
			buffer->index = shuffle[i];
		}
		else{
			fprintf(stderr, "else 序号为: %d\n", shuffle[i]);
			memcpy(buffer->buf, heap+shuffle[i]*BUFLEN, BUFLEN);
#ifdef DEBUG
			//fprintf(stderr, "else end...\n");
#endif
			buffer->index = shuffle[i];
			buffer->len = BUFLEN;
		}
#ifdef DEBUG
		for(int i=0; i< sizeof(struct tmp_buf); i++){
			//fprintf(stderr, "%x", ((unsigned char *)buffer)[i]);
		}
#endif
#ifdef DEBUG
		//fprintf(stderr, "\nbefore write...\n");
#endif
		write(s_fd, (unsigned char *)buffer, sizeof(struct tmp_buf));
	}

    return 0;
}
