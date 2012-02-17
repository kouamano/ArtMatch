//childloop
int ChildLoop(int acc, char *client_address){
	fd_set read_mask;
	fd_set write_mask;
	fd_set mask;
	int width;
	struct timeval timeout;
	int flag_loop_end;
	int i;
	int comm_ptr = 0;
	int comm_esc = 0;
	int flag_comm_end = 0;
	int is_exec = 0;
	int comm_echo = 0;
	DB(fprintf(stderr,"ChildLoop()\n"));

	//block mode
	SetBlock(acc,0);

	//mask
	FD_ZERO(&mask);
	FD_SET(acc,&mask);
	width = acc+1; //acc: (maximam during read_mask and write_mask) + 1
	DB(fprintf(stderr,"sizeof(fd_set):%ld:\n",sizeof(fd_set)));
	DB(fprintf(stderr,"size of fds_bits:%ld:\n",sizeof(mask.__fds_bits)));
	DB(fprintf(stderr,"fds_bits[0]:%ld:\n",mask.__fds_bits[0]));
	DB(fprintf(stderr,"acc before FD_SET:%d:\n",acc));
	DB(fprintf(stderr,"acc after FD_SET:%d:\n",acc));
	DB(fprintf(stderr,"width (acc+1):%d:\n",width));

	//init data for sending
	InitBuf(&SendBuf,SHORT_BUF_SIZE);
	sprintf(SendBuf.buf,"%s\r\nReady.\r\n",VERSION);
	SendBuf.len=strlen(SendBuf.buf);
	DB(fprintf(stderr,"\n\n-----init SendBuf.size [0]:%ld:-----\n\n",SendBuf.size));

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
				//recv
				if(FD_ISSET(acc,&read_mask)){
					flag_loop_end = RecvData(acc,&RecvBuf);
					DB(fprintf(stderr,"flag_loop_end:%d:\n",flag_loop_end));
				}

				//operation after recv
				DB(fprintf(stderr,"after RecvData() RecvBuf.buf:%s:\n",RecvBuf.buf));
				DB(fprintf(stderr,"after RecvData() RecvBuf.len:%d:\n",RecvBuf.len));
				DB(fprintf(stderr,"CommBufSize(%ld) - CommBufLen(%d) ?> RecBufSize(%ld).\n",CommBuf.size,CommBuf.len,RecvBuf.size));
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

				//command operation 
				if(flag_comm_end == 1){
					comm_esc = 0;
					//preprocess - drop head and tail space
					StripBuf(&CommBuf);
					TrimBuf(&CommBuf);
	
					DB(fprintf(stderr,"preprocessed COMMSIZE:%d:\n",CommBuf.len));
					DB(fprintf(stderr,"preprocessed COMM:"));
					DB(fprintf(stderr,"%s",CommBuf.buf));
					DB(fprintf(stderr,":\n"));
					//exec comm and write to RsltBuf
					is_exec = InterpretComm(CommBuf,&flag_loop_end,&comm_echo);
					//create Send Data
					InitBuf(&SendBuf, (CommBuf.len * comm_echo) + EXPAND_BUF_SIZE + RsltBuf.len);
					//SendBuf.buf = c_alloc_vec(CommBuf.len + 40 + RsltBuf.len);
					if(comm_echo > 0){
						sprintf(SendBuf.buf,"%s","\r\ncomm:");
						sprintf(SendBuf.buf+7,"%s",CommBuf.buf);
						sprintf(SendBuf.buf+7+CommBuf.len,"%s",": ");
						SendBuf.len = strlen(SendBuf.buf);
					}else{
						SendBuf.len = 0;
					}
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
				}else if(flag_comm_end == 2){
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
				}
	
				//cleaning
				if(flag_comm_end == 1){
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
				}else{
					comm_ptr = comm_ptr + i;
				}
	
				//send
				if(FD_ISSET(acc,&write_mask)&&(SendBuf.len > 0)){
					DB(fprintf(stderr,"sending...\n"));
					DB(fprintf(stderr,"len:%d:\n",SendBuf.len));
					DB(fprintf(stderr,"size:%ld:\n",SendBuf.size));
					fprintf(stderr,"over flag:%d\n",AddSendOver());
					SendData(acc,&SendBuf);
				}
				break;
			}
		}

		//end
		if(flag_loop_end > 0){
			DB(fprintf(stderr,"session break.\n"));
			break;
		}
	}
	/* *) */
	return(0);
}

