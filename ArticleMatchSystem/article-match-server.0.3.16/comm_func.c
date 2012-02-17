//refine comm
void StripBuf(DATA_BUF *comm){
	DB(fprintf(stderr,"IN StripBuf()\n"));
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
	DB(fprintf(stderr,"OUT StripBuf()\n"));
}

void TrimBuf(DATA_BUF *comm){
	DB(fprintf(stderr,"IN TrimBuf()\n"));
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
	DB(fprintf(stderr,"OUT TrimBuf()\n"));
}

//exec comm
/*
int ExecComm(const DATA_BUF comm){
        DB(fprintf(stderr,"ExecComm()\n"));
        DB(fprintf(stderr,"::%s::",comm.buf));
        return(0);
}
*/
int ExecCommAlive(const DATA_BUF comm){
	DB(fprintf(stderr,"IN ExecCommAlive().\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
	sprintf(RsltBuf.buf+RsltBuf.len,"Alive.\r\n");
	RsltBuf.len = strlen(RsltBuf.buf);
	DB(fprintf(stderr,"OUT ExecCommAlive().\n"));
	return(0);
}
int ExecCommList(const DATA_BUF comm){
	DB(fprintf(stderr,"IN ExecCommList().\n"));
	unsigned long long int i;
	DATA_BUF TmpBuf = {NULL,0,0};
	InitBuf(&TmpBuf,SHORT_BUF_SIZE);
	if(RsltBuf.buf == NULL){
		//InitBuf(&RsltBuf,HUGE_BUF_SIZE);
		//InitBuf(&RsltBuf,LONG_BUF_SIZE);
		InitBuf(&RsltBuf,LONG_LONG_BUF_SIZE);
	}else{
		//ExpandBuf(&RsltBuf,HUGE_BUF_SIZE);
		//ExpandBuf(&RsltBuf,LONG_BUF_SIZE);
		ExpandBuf(&RsltBuf,LONG_LONG_BUF_SIZE);
	}
	//sprintf(RsltBuf.buf+RsltBuf.len,"List:\r\n");
	for(i=0;i<db_profile.num_term;i++){
		sprintf(TmpBuf.buf,"%llx;",db_term_entry[i].termINT);
		TmpBuf.len = strlen(TmpBuf.buf);
		if(TmpBuf.len < RsltBuf.size-RsltBuf.len-EXPAND_BUF_SIZE){
			sprintf(RsltBuf.buf+RsltBuf.len,"%llx;",db_term_entry[i].termINT);
			RsltBuf.len = strlen(RsltBuf.buf);
		}else{
			sprintf(RsltBuf.buf+RsltBuf.len,"- BUF SIZE ERROR in ExecCommList() -");
			RsltBuf.len = strlen(RsltBuf.buf);
			break;
		}
	}
	//fprintf(stderr,"Rslt.len:%lld:\n",RsltBuf.len);
	//fprintf(stderr,"Rslt.size:%ld:\n",RsltBuf.size);
	FreeBuf(&TmpBuf);
	DB(fprintf(stderr,"OUT ExecCommList().\n"));
	return(0);
}
int ExecCommSearch(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommSearch()\n"));
	unsigned long long int query = 0;
	int hit = -1;
	unsigned long long int hit_pos = 0;
	int i;
	sscanf(comm.buf,"Search?%llx",&query);
	DB(fprintf(stderr,"search for :%lld(D):\n",query));
	hit = ArtDBTermBinSearch(query,db_term_entry,db_profile.num_term,0,&hit_pos);
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
	if(hit > 0){
        	sprintf(RsltBuf.buf+RsltBuf.len,"Hit");
		RsltBuf.len = strlen(RsltBuf.buf);
        	sprintf(RsltBuf.buf+RsltBuf.len," term entry pos:%lld(D):",hit_pos);
		RsltBuf.len = strlen(RsltBuf.buf);
		sprintf(RsltBuf.buf+RsltBuf.len,":%llX: Art No:",db_term_entry[hit_pos].termINT);
		RsltBuf.len = strlen(RsltBuf.buf);
		//print Art No.s which contain the hit term.
		for(i=0;i<db_term_entry[hit_pos].num_pos;i++){
			sprintf(RsltBuf.buf+RsltBuf.len," %u",db_term_entry[hit_pos].pos_ID[i]);
			RsltBuf.len = strlen(RsltBuf.buf);
		}
		sprintf(RsltBuf.buf+RsltBuf.len,"\r\n");
		RsltBuf.len = strlen(RsltBuf.buf);
		//UNDER CONSTRUCTION
	}else{
        	sprintf(RsltBuf.buf+RsltBuf.len,"No hit.\r\n");
	}
        RsltBuf.len = strlen(RsltBuf.buf);
        DB(fprintf(stderr,"OUT ExecCommSearch()\n"));
        return(0);
}
int ExecCommMulti(const DATA_BUF comm){
	/*UNDER CONSTRUCTION*/
        DB(fprintf(stderr,"IN ExecComm()\n"));
        DB(fprintf(stderr,"::%s::",comm.buf));
        DB(fprintf(stderr,"OUT ExecComm()\n"));
        return(0);
}
int ExecCommTest(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommTest()\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
        //sprintf(RsltBuf.buf+RsltBuf.len,"test (%d).\r\n",TEST_DATA[20]);
        RsltBuf.len = strlen(RsltBuf.buf);
	ExpandBuf(&RsltBuf,EXPAND_BUF_SIZE);
        sprintf(RsltBuf.buf+RsltBuf.len,"expand test -- OK.\r\n");
        RsltBuf.len = strlen(RsltBuf.buf);
        DB(fprintf(stderr,"OUT ExecCommTest()\n"));
        return(0);
}
int ExecCommUnknown(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommUnknown()\n"));
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
        sprintf(RsltBuf.buf+RsltBuf.len,"undefined command.\r\n");
        RsltBuf.len = strlen(RsltBuf.buf);
        DB(fprintf(stderr,"OUT ExecCommUnknown()\n"));
	return(0);
}

int InterpretComm(const DATA_BUF comm, int *endflag, int *commecho){
        if(comm.len > 0){
                DB(fprintf(stderr,"COMMBUFF:%s:\n",comm.buf));
                if(strcmp(comm.buf,"End?") == 0){
                        *commecho = 0;
                        *endflag = 1;
                        return(0);
                }else if(strcmp(comm.buf,"Alive?") == 0){
			ExecCommAlive(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strcmp(comm.buf,"List?") == 0){
			ExecCommList(comm);
                        *commecho = 0;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"Search?",7) == 0){
                        ExecCommSearch(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"Multi?",6) == 0){
			/*UNDER CONSTRUCTION*/
			ExecCommMulti(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"Test?",5) == 0){
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
