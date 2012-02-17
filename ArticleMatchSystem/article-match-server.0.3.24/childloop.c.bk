//childloop
int ChildLoop(int acc, char *client_address){
	DB(fprintf(stderr,"IN ChildLoop()\n"));
	fd_set read_mask;
	fd_set write_mask;
	fd_set mask;
	int width;
	struct timeval timeout;
	int flag_loop_end;
	int i;
	int comm_ptr = 0;
	int comm_esc = 0;
	int flag_comm_end = 0;	// 0:initial value; 1:command string size - OK; 2: command string size - over.
	int is_exec = 0;
	int comm_echo = 0;

	/* (* set block mode */
	SetBlock(acc,0);
	/* *) */

	/* (* mask operation */
	FD_ZERO(&mask);
	FD_SET(acc,&mask);
	width = acc+1; //acc: (maximam during read_mask and write_mask) + 1
	DB(fprintf(stderr,"sizeof(fd_set):%ld:\n",sizeof(fd_set)));
	DB(fprintf(stderr,"size of fds_bits:%ld:\n",sizeof(mask.__fds_bits)));
	DB(fprintf(stderr,"fds_bits[0]:%ld:\n",mask.__fds_bits[0]));
	DB(fprintf(stderr,"acc before FD_SET:%d:\n",acc));
	DB(fprintf(stderr,"acc after FD_SET:%d:\n",acc));
	DB(fprintf(stderr,"width (acc+1):%d:\n",width));
	/* *) */

	/* (* init data for sending */
	InitBuf(&SendBuf,SHORT_BUF_SIZE);
	sprintf(SendBuf.buf,"%s\r\ndb_n:%d:\r\nReady.\r\n",VERSION,db_n);
	SendBuf.len=strlen(SendBuf.buf);
	/* *) */

	/* (* send-secv loop */
	flag_loop_end = 0;
	while(1){
		is_exec = 0;
		read_mask=mask;
		if(SendBuf.len>0){
			write_mask=mask;
		}else{
			FD_ZERO(&write_mask);
		}
		timeout.tv_sec=TIMEOUT_S;
		timeout.tv_usec=TIMEOUT_US;
		/* (* select switching */
		switch(select(width,(fd_set *)&read_mask,&write_mask,NULL,&timeout)){
			case -1:{
				if((errno != EINTR)&&(errno != EAGAIN)){
					perror("select");
				}
				break;
			}

			case 0:{
				break;
			}

			default:{
				/* (* recv */
				if(FD_ISSET(acc,&read_mask)){
					flag_loop_end = RecvData(acc,&RecvBuf);
					DB(fprintf(stderr,"flag_loop_end:%d:\n",flag_loop_end));
				}
				/* *) */
				/* (* command preprocess */
				DB(fprintf(stderr,"after RecvData() RecvBuf.buf:%s:\n",RecvBuf.buf));
				DB(fprintf(stderr,"after RecvData() RecvBuf.len:%lld:\n",RecvBuf.len));
				DB(fprintf(stderr,"CommBufSize(%ld) - CommBufLen(%lld) ?> RecBufSize(%ld).\n",CommBuf.size,CommBuf.len,RecvBuf.size));
				DB(fprintf(stderr,"comm_ptr(%d).\n",comm_ptr));
				if((CommBuf.size - CommBuf.len) > RecvBuf.size){
					for(i=0;i<RecvBuf.len;i++){
						if((RecvBuf.buf[i] == COMM_SEQ_END)&&(comm_esc == 0)){
							CommBuf.buf[comm_ptr + i] = '\0';
							CommBuf.len = comm_ptr + i;
							flag_comm_end = 1;
							comm_esc = 0;
						}else if((RecvBuf.buf[i] == '\\')&&(comm_esc == 0)){
							comm_ptr = comm_ptr - 1;
							comm_esc = 1;
						}else if((RecvBuf.buf[i] == '\\')&&(comm_esc > 0)){
							CommBuf.buf[comm_ptr + i] = '\\';
							CommBuf.len = comm_ptr + i;
							comm_esc = 0;
						}else{
							CommBuf.buf[comm_ptr + i] = RecvBuf.buf[i];
							CommBuf.len = comm_ptr + i;
							comm_esc = 0;
						}
					}
				}else{
					flag_comm_end = 2;
				}
				/* *) */
				/* (* command  analysis */
				if(flag_comm_end == 1){	//normal finish
					comm_esc = 0;
					//preprocess - drop head and tail space
					StripBuf(&CommBuf);
					TrimBuf(&CommBuf);
	
					DB(fprintf(stderr,"preprocessed COMMSIZE:%lld:\n",CommBuf.len));
					DB(fprintf(stderr,"preprocessed COMM:"));
					DB(fprintf(stderr,"%s",CommBuf.buf));
					DB(fprintf(stderr,":\n"));
					//interpret and exec command and  write to RsltBuf
					is_exec = InterpretComm(CommBuf,&flag_loop_end,&comm_echo);	//is_exec 0: no exec; 1: right exec; 2; exec error.
					//create Send Data
					if(is_exec == 1){
						InitBuf(&SendBuf, (CommBuf.len * comm_echo) + EXPAND_BUF_SIZE + RsltBuf.len);
					}else if(is_exec == 2){
						FreeBuf(&SendBuf);
						SendBuf.buf = NULL;
					}
					if(comm_echo > 0){
						sprintf(SendBuf.buf,"%s","\r\ncomm:");
						sprintf(SendBuf.buf+7,"%s",CommBuf.buf);
						sprintf(SendBuf.buf+7+CommBuf.len,"%s",": ");
						SendBuf.len = strlen(SendBuf.buf);
					}else{
						SendBuf.len = 0;
					}
					fprintf(stderr,"is_execc:%d:\n",is_exec);
					if(is_exec > 0){
						sprintf(SendBuf.buf+SendBuf.len,"%s",RsltBuf.buf);
						SendBuf.len = strlen(SendBuf.buf);
						if(RsltBuf.buf != NULL){
							free(RsltBuf.buf);
							RsltBuf.buf = NULL;
							RsltBuf.size = 0;
							RsltBuf.len = 0;
						}
					}				
					//cleaning
					if((CommBuf.len > 0) && (strncmp(CommBuf.buf,"end",3) == 0)){
						flag_loop_end = 1;
					}
					for(i=0;i<CommBuf.len;i++){
						CommBuf.buf[i] = '\0';
					}
					for(i=0;i<RecvBuf.len;i++){
						RecvBuf.buf[i] = '\0';
					}
					comm_ptr = 0;
					flag_comm_end = 0;
				}else if(flag_comm_end == 2){	//error finish
					if((SendBuf.buf = (char *)realloc(SendBuf.buf,sizeof(char) * (40))) == NULL){perror("realloc()"); exit(1);}
					sprintf(SendBuf.buf,"%s","error: commsize over.\r\n");
					SendBuf.len = strlen(SendBuf.buf);
					for(i=0;i<CommBuf.size;i++){
						CommBuf.buf[i] = '\0';
					}
					CommBuf.len = 0;
					for(i=0;i<RecvBuf.size;i++){
						RecvBuf.buf[i] = '\0';
					}
					RecvBuf.len = 0;
					comm_ptr = 0;
					flag_comm_end = 0;
				}else{			// in command line
					comm_ptr = comm_ptr + i;
				}
				/* *) */
				/* (* send result */
				if(FD_ISSET(acc,&write_mask)&&(SendBuf.len > 0)){
					fprintf(stderr,"over flag:%d\n",AddSendOver());
					SendData(acc,&SendBuf);
					DB(fprintf(stderr,"After SendData() SendBuf.len :%lld:\n",SendBuf.len));
				}
				/* *) */
				break;
			}
		}
		/* *) */

		/* (* ending */
		if(flag_loop_end > 0){
			DB(fprintf(stderr,"session break.\n"));
			break;
		}
		/* *) */
	}
	/* *) */

	/* (* return */
	return(0);
	/* *) */
	DB(fprintf(stderr,"OUT ChildLoop()\n"));
}

