//refine comm
void StripBuf(DATA_BUF *comm){
	int i;
	int skip = 0;
        for(i=0;i<comm->len;i++){
		if((comm->buf[i] >= 33)&&(comm->buf[i] <= 126)){
			break;
                }else{
                         skip++;
                }
	}
        for(i=skip; i<comm->len; i++){
                comm->buf[i - skip] = comm->buf[i];
        }
        comm->buf[i - skip] = '\0';
	comm->len = strlen(comm->buf);
}

void TrimBuf(DATA_BUF *comm){
	int i;
	int skip = 0;
	for(i=comm->len;i>0;i--){
		if((comm->buf[i] >= 33)&&(comm->buf[i] <= 126)){
			break;
		}else{
			skip++;
		}
	}
	comm->buf[comm->len - skip + 1] = '\0';
	comm->len = strlen(comm->buf);
}

//exec comm
int ExecComm(const DATA_BUF comm){
        DB(fprintf(stderr,"ExecComm()\n"));
        DB(fprintf(stderr,"::%s::",comm.buf));
        return(0);
}

int InterpretComm(int *endflag, int *commecho){
        if(CommBuf.len > 0){
                DB(fprintf(stderr,"COMMBUFF:%s:\n",CommBuf.buf));
                if(strcmp(CommBuf.buf,"End") == 0){
                        *commecho = 0;
                        *endflag = 1;
                        return(0);
                }else if(strcmp(CommBuf.buf,"Alive?") == 0){
                	DB(fprintf(stderr,"Alive polling.\n"));
                        RsltBuf.size = 6;
                        RsltBuf.buf = malloc(sizeof(char) * RsltBuf.size);
                        sprintf(RsltBuf.buf,"ALIVE");
                        RsltBuf.len = strlen(RsltBuf.buf);
                        *commecho = 0;
                        *endflag = 1;
                        return(1);
                }else if(strncmp(CommBuf.buf,"Search?",7) == 0){
                        ExecComm(CommBuf);
                        RsltBuf.size = 7;
                        RsltBuf.buf = malloc(sizeof(char) * RsltBuf.size);
                        sprintf(RsltBuf.buf,"No hit.\r\n");
                        RsltBuf.len = strlen(RsltBuf.buf);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(CommBuf.buf,"Test",4) == 0){
                        ExecComm(CommBuf);
                        RsltBuf.size = 50;
                        RsltBuf.buf = malloc(sizeof(char) * RsltBuf.size);
                        sprintf(RsltBuf.buf,"test (%d).\r\n",TEST_DATA[20]);
                        RsltBuf.len = strlen(RsltBuf.buf);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else{
                        //test
                        RsltBuf.size = 100;
                        if((RsltBuf.buf = malloc(sizeof(char) * RsltBuf.size)) == NULL){perror("malloc"); exit(1);}
                        sprintf(RsltBuf.buf,"undefined command.\r\n");
                        RsltBuf.len = strlen(RsltBuf.buf);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }
        }
        return(0);
}
