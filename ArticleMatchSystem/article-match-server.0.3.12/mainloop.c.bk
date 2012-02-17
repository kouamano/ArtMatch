//main loop
int MainLoop(OPT option){
	int acc;
	struct sockaddr_in from;
	socklen_t len = 0;
	char *client_address;
	int client_port = 0;
	int pid;

	DB(fprintf(stderr,"MainLoop()\n"));

	/* (* connection loop */
	while(1){
		len=sizeof(from);
		//accept
		acc=accept(Soc,(struct sockaddr *)&from,&len);
		DB(fprintf(stderr,"acc:%d:\n",acc));
		if(acc < 0){
			if(errno != EINTR){
				perror("accept\n");
			}
			continue;
		}
		client_address = c_alloc_vec(strlen(inet_ntoa(from.sin_addr)));
		sprintf(client_address,"%s",inet_ntoa(from.sin_addr));
		client_port=ntohs(from.sin_port);
		DB(fprintf(stderr,"%s:%d\n",client_address,client_port));
		if((pid=fork()) == 0){
			close(Soc);
			Soc=0;
			//operation for client
			ChildLoop(acc,client_address);
			exit(0);
		}
		//close socket
		close(acc);
		acc=0;
		if(client_address != NULL){free(client_address);}
	}
	/* *) */
	return(0);
}
