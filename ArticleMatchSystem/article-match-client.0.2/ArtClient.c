/* htclient.c */
#include <stdio.h>
#include <string.h>
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
#include "/home/pub/include/alloc.c"
#define BUF_LEN 1024
#define LONG_BUF_LEN 134217728 //1024 * 1024 * 128
#define STR_LEN 1024
#define LONG_STR_LEN 4096
#define READY_FLAG "Ready."
#define OVER_FLAG "Over."

struct argop {
	char method[STR_LEN];
	char host[STR_LEN];
	int port;
	char path[LONG_STR_LEN];
};

void print_help(void){
	printf("USAGE : \n");
	printf("  htclient method=<method> host=<host> [path=<path>] port=<port>\n");
}

void init_options(struct argop *option){
	sscanf("SIN","%s",option->method);
	option->host[0] = '\0';
	option->port = 19990;
	option->path[0] = '\0';
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

int main(int argc, char *argv[]){
	int i;
	struct argop options;
	int s = -1;
	struct hostent *serverhost = NULL;
	struct sockaddr_in server;
	char send_buf[BUF_LEN];
	char *get_buf;
	char hostname[STR_LEN];
	char path[LONG_STR_LEN];
	int port = 0;
	int read_size = 0;
	int opt_stat = 0;
	int send_buf_end = 0;
	get_buf = c_calloc_vec(LONG_BUF_LEN);
	init_options(&options);
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
		for(i=0;i<BUF_LEN;i++){
			get_buf[i] = '\0';
		}
		read_size = read(s,get_buf,BUF_LEN);
		printf("%s",get_buf);
		fprintf(stderr,"size:%d:",read_size);
		//read_size = read(s,get_buf,BUF_LEN);

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
			read_size = read(s,get_buf,BUF_LEN);
			if(read_size > 0){
				write(1,get_buf,read_size);
				close(s);
				exit(0);
			}else{
				break;
			}
			fprintf(stderr,"in while...\n");
		}
		fprintf(stderr,"exit while...\n");
		sprintf(send_buf,"end");
		send_buf_end = strlen(send_buf);
		send_buf[send_buf_end] = 0x04;
		send_buf[send_buf_end+1] = '\0';
		fprintf(stderr,"sending...\n");
		write(s,send_buf,strlen(send_buf));
		fprintf(stderr,"send.\n");
		read(s,get_buf,BUF_LEN);
		close(s);
		exit(0);
	}
	return(0);
}
