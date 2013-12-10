/* ArtClient.c */
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
#define VERSION "Client version 0.3.4"
#include "ostype.h"

//my include
#ifdef OSX
#include "/Users/pub/src1/include/alloc.c"
#else
#include "/home/pub/src1/include/alloc.c"
#endif
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
DATA_BUF SendBuf; //LONG_BUF_LEN

//funcdef
void print_help(void){
	printf("USAGE : \n");
	printf("  ArtClient method=<method> host=<host> port=<port> [path=<path>]\n");
	printf("  <method> : {SIN|FIN|TEST}\n");
	printf("  <host> : server name.\n");
	printf("  <port> : server port.\n");
	printf("  <path> : for FIN method.\n");
}

void init_options(struct argop *option){
	option->method = c_calloc_vec(STR_LEN);
	option->host = c_calloc_vec(STR_LEN);
	option->path = c_calloc_vec(LONG_STR_LEN);
	sscanf("TEST","%s",option->method);
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
	char *carrier_buf;
	char *greeting_buf;
	char *hostname;
	char *path;
	int port = 0;
	int carrier_read_size = 0;
	int opt_stat = 0;
	int server_ready = -1;
	int greeting_retry = 0;
	int greeting_retry_max = 5;
	FILE *IN;
	int get_i;
	char get_c;
	int buf_len = 0;

	//alloc or init
	InitBuf(&RecvBuf,BUF_LEN);
	InitBuf(&SendBuf,LONG_BUF_LEN);
	carrier_buf = c_calloc_vec(LONG_BUF_LEN);
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
			fprintf(stderr,"failed : gethostbyname().\n");
			exit(1);
		}
		bzero((char *)&server,sizeof(server));
		server.sin_family = AF_INET;
		bcopy(serverhost->h_addr,(char *)&server.sin_addr,serverhost->h_length);
		server.sin_port = htons(port);
		if((s = socket(AF_INET,SOCK_STREAM,0)) < 0){
			fprintf(stderr,"failed : socket().\n");
			exit(1);
		}
		if(connect(s,(struct sockaddr *)&server,sizeof(server)) == -1){
			fprintf(stderr,"failed : connect.\n");
			exit(1);
		}

		//get greeting - check "Ready." try 5 max
		//SetBlock(s,0); // non-block
		SetBlock(s,1); // block
		while(greeting_retry_max - greeting_retry > 0){
			RecvBuf.len = read(s,RecvBuf.buf,RecvBuf.size);
			if(RecvBuf.buf[RecvBuf.len - 1] == 0x04){
				fprintf(stderr,"%s\n",RecvBuf.buf);
				TrimBuf(&RecvBuf);
				server_ready = strcmp(RecvBuf.buf + RecvBuf.len - strlen(READY_FLAG),READY_FLAG);
				if(server_ready != 0){
					greeting_retry++;
					fprintf(stderr,"greeting error - retry.\n");
				}else{
					fprintf(stderr,"greeting - success!\n");
					break;
				}
			}else{
				fprintf(stderr,"Server error - retry.\n");
				greeting_retry++;
			}
		}
		if(server_ready != 0){
			fprintf(stderr,"Server error occurred - exit.\n");
			exit(0);
		}

		//send command
		SetBlock(s,1); // block
		fprintf(stderr,"mode: %s\n",options.method);
		InitBuf(&SendBuf,LONG_BUF_LEN);
		if(strcmp(options.method,"FIN") == 0){
			fprintf(stderr,"method FIN : not support.\n");
			exit(0);
		}else if(strcmp(options.method,"SIN") == 0){
			IN = stdin;
			while((get_i = getc(IN)) != EOF){
				get_c = (char)get_i;
				if(buf_len + 2 > SendBuf.size){
					SendBuf.buf[buf_len] = '\0';
					SendBuf.len = strlen(SendBuf.buf);
					write(s,SendBuf.buf,SendBuf.len);
				}
				SendBuf.buf[buf_len] = get_c;
				buf_len++;
			}
			SendBuf.buf[buf_len] = '\0';
			SendBuf.len = strlen(SendBuf.buf);
			AddOver(&SendBuf);
			write(s,SendBuf.buf,SendBuf.len);
		}else{
			sprintf(SendBuf.buf,"Test?");
			SendBuf.len = strlen(SendBuf.buf);
			AddOver(&SendBuf);
			SendBuf.len = strlen(SendBuf.buf);
			write(s,SendBuf.buf,SendBuf.len);
		}

		//mode set
		//SetBlock(s,0); // non-block
		SetBlock(s,1); // block
		while(1){
			for(i=0;i<BUF_LEN;i++){
				carrier_buf[i] = '\0';
			}
			carrier_read_size = read(s,carrier_buf,BUF_LEN);
			if(carrier_read_size > 0){
				for(i=0;i<BUF_LEN;i++){
					if(carrier_buf[i] == 0x04){
						goto loop_end;
					}else{
						write(1,carrier_buf+i,1);
					}
				}
			}else{
				goto loop_end;
			}
		}
		loop_end:
		fprintf(stderr,"break loop.\n");
		close(s);
		exit(0);
	}

	//exit
	return(0);
}
