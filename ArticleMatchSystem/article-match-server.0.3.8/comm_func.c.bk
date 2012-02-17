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
int ExecCommAlive(const DATA_BUF comm){
	DB(fprintf(stderr,"ExecCommAlive().\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,6);
	}
	sprintf(RsltBuf.buf+RsltBuf.len,"ALIVE");
	RsltBuf.len = strlen(RsltBuf.buf);
	return(0);
}
int ExecCommSearch(const DATA_BUF comm){
        DB(fprintf(stderr,"ExecCommSearch()\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,10);
	}
        sprintf(RsltBuf.buf+RsltBuf.len,"No hit.\r\n");
        RsltBuf.len = strlen(RsltBuf.buf);
        return(0);
}
int ExecCommMulti(const DATA_BUF comm){
	/*UNDER CONSTRUCTION*/
        DB(fprintf(stderr,"ExecComm()\n"));
        DB(fprintf(stderr,"::%s::",comm.buf));
        return(0);
}
int ExecCommTest(const DATA_BUF comm){
        DB(fprintf(stderr,"ExecCommTest()\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,50);
	}
        sprintf(RsltBuf.buf+RsltBuf.len,"test (%d).\r\n",TEST_DATA[20]);
        RsltBuf.len = strlen(RsltBuf.buf);
	ExpandBuf(&RsltBuf,50);
        sprintf(RsltBuf.buf+RsltBuf.len,"expand test -- OK.\r\n");
        RsltBuf.len = strlen(RsltBuf.buf);
        return(0);
}
int ExecCommUnknown(const DATA_BUF comm){
        DB(fprintf(stderr,"ExecCommUnknown()\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,100);
	}
        sprintf(RsltBuf.buf+RsltBuf.len,"undefined command.\r\n");
        RsltBuf.len = strlen(RsltBuf.buf);
	return(0);
}

int InterpretComm(const DATA_BUF comm, int *endflag, int *commecho){
        if(comm.len > 0){
                DB(fprintf(stderr,"COMMBUFF:%s:\n",comm.buf));
                if(strcmp(comm.buf,"End") == 0){
                        *commecho = 0;
                        *endflag = 1;
                        return(0);
                }else if(strcmp(comm.buf,"Alive?") == 0){
			ExecCommAlive(comm);
                        *commecho = 0;
                        *endflag = 1;
                        return(1);
                }else if(strncmp(comm.buf,"Search?",7) == 0){
                        ExecCommSearch(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"Multi",5) == 0){
			/*UNDER CONSTRUCTION*/
			ExecCommMulti(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"Test",4) == 0){
                        ExecCommTest(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else{
			ExecCommUnknown(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }
        }
        return(0);
}
