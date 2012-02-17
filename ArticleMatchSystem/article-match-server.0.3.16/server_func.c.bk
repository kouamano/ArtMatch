int InitSocket(char *_servname, int _port){
	struct servent *se;
	struct sockaddr_in ss;
	int opt;
	//get port by service name
	if((se = getservbyname(_servname,"tcp")) == NULL){
		perror("getservbyname\n");
		if((se = getservbyport(_port,"tcp")) == NULL){
			perror("getservbyport\n");
			return(-1);
		}
	}
	DB(fprintf(stderr,"getservbyname():%d:\n",ntohs(se->s_port)));
	//create socket
	if((Soc=socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket\n");
		return(-1);
	}
	DB(fprintf(stderr,"Soc:%d:\n",Soc));
	//reset socket
	opt=1;
	if(setsockopt(Soc,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(int)) != 0){
		perror("setsockopt\n");
		return(-1);
	}
	DB(fprintf(stderr,"setsockopt()\n"));
	//bind
	memset((char *)&ss,0,sizeof(struct sockaddr_in));
	ss.sin_family=AF_INET;
	ss.sin_port=se->s_port;
	if(bind(Soc,(struct sockaddr *)&ss,sizeof(ss)) == -1){
		perror("bind\n");
		return(-1);
	}
	DB(fprintf(stderr,"bind()\n"));
	return(0);
}

int CloseSocket(){
	DB(fprintf(stderr,"CloseSocket()\n"));
	if(Soc>0){
		close(Soc);
	}
	Soc=0;
	return(0);
}

void EndSignal(int sig){
	DB(fprintf(stderr,"EndSignal()\n"));
	CloseSocket();
	exit(0);
}

void EndChild(int sig){
	int pid;
	pid=wait(0);
	DB(fprintf(stderr,"EndChild()\n"));
	DB(fprintf(stderr,"wait pid=%d\n",pid));
	//signal(SIGCHLD,EndChild);
}

int InitSignal(){
	DB(fprintf(stderr,"InitSignal()\n"));
	//end main loop
	signal(SIGINT,EndSignal);
	signal(SIGQUIT,EndSignal);
	signal(SIGTERM,EndSignal);
	//end child loop
	signal(SIGCHLD,EndChild);
	return(0);
}

//mode change func
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

