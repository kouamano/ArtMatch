/* htclient.c */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#define BUF_LEN 1024
#define LONG_BUF_LEN 1048576 //1024 * 1024 * 1
#define HUGE_BUF_LEN 134217728 //1024 * 1024 * 128
#define STR_LEN 1024
#define LONG_STR_LEN 4096
#define READY_FLAG "Ready."
#define OVER_FLAG "Over."
#define DEFAULT_PORT 19990
#define DB(X) X

//my include
#include "/home/pub/include/alloc.c"
#include "data_op.h"
#include "data_op.c"

//typedef
struct argop {
	char *method;
	char *host;
	int port;
	char *path;
};

//global vars
DATA_BUF RecvBuf; //BUF_LEN
//DATA_BUF SendBuf; //LONG_BUF_LEN
//DATA_BUF RsltBuf; //HUGE_BUF_LEN

//funcdef
void print_help(void){
	printf("USAGE : \n");
	printf("  ArtClient method=<method> host=<host> [path=<path>] port=<port>\n");
}

void init_options(struct argop *option){
	option->method = c_calloc_vec(STR_LEN);
	option->host = c_calloc_vec(STR_LEN);
	option->path = c_calloc_vec(LONG_STR_LEN);
	sscanf("SIN","%s",option->method);
	option->port = DEFAULT_PORT;
}

int get_options(int arg_start, int arg_end, char **arg_s, struct argop *option){
	int i = 0;
	int s = 0;
	for(i=arg_start;i<arg_end;i++){
		if(strcmp(arg_s[i],"-h") == 0){
			print_help();
			exit(0);
		}else if(strncmp(arg_s[i],"host=",5) == 0){
			sscanf(arg_s[i],"host=%s",option->host);
		}else if(strncmp(arg_s[i],"path=",5) == 0){
			sscanf(arg_s[i],"path=%s",option->path);
		}else if(strncmp(arg_s[i],"port=",5) == 0){
			sscanf(arg_s[i],"port=%d",&option->port);
		}else if(strncmp(arg_s[i],"method=",7) == 0){
			sscanf(arg_s[i],"method=%s",option->method);
		}else{
			printf("Unknown optios.\n");
			exit(0);
		}
	}
	return(s);
}

int SetBlock(int fd, int flag){
        int flags;
        if((flags=fcntl(fd,F_GETFL,0)) == -1){
                perror("fcntl");
                return(1);
        }
        if(flag == 0){
                fcntl(fd,F_SETFL,flags|O_NDELAY);
        }else if(flag == 1){
                fcntl(fd,F_SETFL,flags&~O_NDELAY);
        }
        return(0);
}

//top main
int main(int argc, char **argv){
	int i;
	struct argop options;
	int s = -1;
	struct hostent *serverhost = NULL;
	struct sockaddr_in server;
	char *get_buf;
	char *send_buf;
	char *greeting_buf;
	char *hostname;
	char *path;
	int port = 0;
	int read_size = 0;
	int opt_stat = 0;
	int send_buf_end = 0;

	//alloc or init
	InitBuf(&RecvBuf,BUF_LEN);
	//InitBuf(&SendBuf,LONG_BUF_LEN);
	//InitBuf(&RsltBuf,HUGE_BUF_LEN);
	get_buf = c_calloc_vec(LONG_BUF_LEN);
	send_buf = c_calloc_vec(HUGE_BUF_LEN);
	greeting_buf = c_calloc_vec(LONG_BUF_LEN);
	hostname = c_calloc_vec(LONG_STR_LEN);
	path = c_calloc_vec(LONG_STR_LEN);
	init_options(&options);

	//argment analysis
	if(argc == 1){
		print_help();
		exit(0);
	}else if(argc >= 2){
		opt_stat = get_options(1,argc,argv,&options);
		strcpy(hostname,options.host);
		strcpy(path,options.path);
		port = options.port;
		if((serverhost = gethostbyname(hostname)) == NULL){
			printf("failed : gethostbyname().\n");
			exit(1);
		}
		bzero((char *)&server,sizeof(server));
		server.sin_family = AF_INET;
		bcopy(serverhost->h_addr,(char *)&server.sin_addr,serverhost->h_length);
		server.sin_port = htons(port);
		if((s = socket(AF_INET,SOCK_STREAM,0)) < 0){
			printf("failed : socket().\n");
			exit(1);
		}
		if(connect(s,(struct sockaddr *)&server,sizeof(server)) == -1){
			printf("failed : connect.\n");
			exit(1);
		}

		//get greeting - check "Ready."
		//recv and check
		RecvBuf.size = read(s,RecvBuf.buf,RecvBuf.size);
		if(RecvBuf.buf[RecvBuf.size - 1] == 0x04){
			printf("::%d::%c::\n",RecvBuf.buf[RecvBuf.size - 1],RecvBuf.buf[RecvBuf.size - 1]);
		}
		printf("%s",RecvBuf.buf);

		//send command
		sprintf(send_buf,"Test");
		send_buf_end = strlen(send_buf);
		send_buf[send_buf_end] = 0x04;
		send_buf[send_buf_end+1] = '\0';
		write(s,send_buf,strlen(send_buf));
		fprintf(stderr,"enter while...\n");

		//mode set
		//SetBlock(s,0); // non-block
		SetBlock(s,1); // block
		while(1){
			for(i=0;i<BUF_LEN;i++){
				get_buf[i] = '\0';
			}
			read_size = read(s,get_buf,BUF_LEN);
			if(read_size > 0){
				for(i=0;i<BUF_LEN;i++){
					if(get_buf[i] == 0x04){
						fprintf(stderr,"recv '0x04' exit.\n");
						close(s);
					}else{
						write(1,get_buf+i,1);
					}
				}
				//close(s);
				//exit(0);
			}else{
				break;
			}
		}
		close(s);
		exit(0);
	}

	//exit
	return(0);
}
