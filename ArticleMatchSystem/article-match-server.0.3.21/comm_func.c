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
int (*ExecComm)();
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
		InitBuf(&RsltBuf,LONG_LONG_BUF_SIZE);
	}else{
		ExpandBuf(&RsltBuf,LONG_LONG_BUF_SIZE);
	}
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
	FreeBuf(&TmpBuf);
	DB(fprintf(stderr,"OUT ExecCommList().\n"));
	return(0);
}
int ExecCommSearch(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommSearch()\n"));
	unsigned long long int query = 0;
	unsigned long long int hit_pos = 0;
	int hit = -1;
	int i;
	sscanf(comm.buf,"Search?%llx",&query);
	DB(fprintf(stderr,"search for :%lld(D):\n",query));
	hit = ArtDBTermBinSearch(query,db_term_entry,db_profile.num_term,0,&hit_pos);
	//hit = ArtDBTermSkipSearch(query,db_term_entry,db_profile.num_term,0,&hit_pos);
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
	if(hit > 0){
        	sprintf(RsltBuf.buf+RsltBuf.len,"\nHit:\n");
		RsltBuf.len = strlen(RsltBuf.buf);
        	sprintf(RsltBuf.buf+RsltBuf.len,"%llX,{",db_term_entry[hit_pos].termINT);
		RsltBuf.len = strlen(RsltBuf.buf);
			sprintf(RsltBuf.buf+RsltBuf.len,"%u",db_term_entry[hit_pos].pos_ID[0]);
			RsltBuf.len = strlen(RsltBuf.buf);
			sprintf(RsltBuf.buf+RsltBuf.len,",%u",db_term_entry[hit_pos].pos_POS[0]);
			RsltBuf.len = strlen(RsltBuf.buf);
		for(i=1;i<db_term_entry[hit_pos].num_pos;i++){
			sprintf(RsltBuf.buf+RsltBuf.len," %u",db_term_entry[hit_pos].pos_ID[i]);
			RsltBuf.len = strlen(RsltBuf.buf);
			sprintf(RsltBuf.buf+RsltBuf.len,",%u",db_term_entry[hit_pos].pos_POS[i]);
			RsltBuf.len = strlen(RsltBuf.buf);
		}
		sprintf(RsltBuf.buf+RsltBuf.len,"}\r\n");
		RsltBuf.len = strlen(RsltBuf.buf);
	}else{
        	sprintf(RsltBuf.buf+RsltBuf.len,"No hit.\r\n");
	}
        RsltBuf.len = strlen(RsltBuf.buf);
        DB(fprintf(stderr,"OUT ExecCommSearch()\n"));
        return(0);
}
int ExecCommSearchList(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommSearch()\n"));
        DB(fprintf(stderr,"command in ExecCommSearchList:::%s:::",comm.buf));
	char query_str[SHORT_BUF_SIZE];
	int query_str_len = 0;
	int num_query_entry = 0;
	unsigned long long int *queries;
	unsigned long long int *hit_poss;
	int num_queries = 0;
	int hit = 0;
	int total_hit = 0;
	int i;
	int j;
	int scan = 0;
	int num_delims = 0;
	//scan number of ngrams
	for(i=0;i<comm.len;i++){
		if(comm.buf[i] == '?'){
			scan++;
		}
		if(scan > 0){
			if(comm.buf[i] == NGRAM_LIST_DELIM){
				num_delims++;
			}
		}
	}
	num_queries = num_delims+1;
	//allocation
	queries = ulli_calloc_vec(num_queries);
	hit_poss = ulli_calloc_vec(num_queries);
	//scan ngrams
	scan = 0;
	query_str_len = 0;
	num_query_entry = 0;
	DB(fprintf(stderr,"\ncomm.buf:::%s:::\n",comm.buf));
	for(i=0;i<comm.len;i++){
		if(comm.buf[i] == '?'){
			scan++;
			i++;
		}
		if(scan > 0){
			if(comm.buf[i] == NGRAM_LIST_DELIM){
				query_str[query_str_len] = '\0';
				DB(fprintf(stderr,"query_str:::%s:::\n",query_str));
				sscanf(query_str,"%llx",queries+num_query_entry);
				num_query_entry++;
				query_str[0] = '\0';
				query_str_len = 0;
			}else{
				query_str[query_str_len] = comm.buf[i];
				query_str_len++;
			}
		}
	}
	if(query_str[0] != '\0'){
		query_str[query_str_len] = '\0';
		DB(fprintf(stderr,"query_str:::%s:::\n",query_str));
		sscanf(query_str,"%llx",&queries[num_query_entry]);
		num_query_entry++;
	}
	//init buf
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE*(num_query_entry + 1));
	}
	//search
	DB(fprintf(stderr,"num_queries:::%d:::\n",num_queries));
	hit = 0;
	for(i=0;i<num_query_entry;i++){
		DB(fprintf(stderr,"query:::%llx:::\n",queries[i]));
		hit = ArtDBTermBinSearch(queries[i],db_term_entry,db_profile.num_term,0,&hit_poss[i]);
		if(hit == 1){
			//hit terms
			total_hit += hit;
			sprintf(RsltBuf.buf+RsltBuf.len,"\n%llX,{",db_term_entry[hit_poss[i]].termINT);
			RsltBuf.len = strlen(RsltBuf.buf);
				//hit positions
				sprintf(RsltBuf.buf+RsltBuf.len,"%u,",db_term_entry[hit_poss[i]].pos_ID[0]);
				RsltBuf.len = strlen(RsltBuf.buf);
				sprintf(RsltBuf.buf+RsltBuf.len,"%u",db_term_entry[hit_poss[i]].pos_POS[0]);
				RsltBuf.len = strlen(RsltBuf.buf);
			for(j=1;j<db_term_entry[hit_poss[i]].num_pos;j++){
				sprintf(RsltBuf.buf+RsltBuf.len," %u,",db_term_entry[hit_poss[i]].pos_ID[j]);
				RsltBuf.len = strlen(RsltBuf.buf);
				sprintf(RsltBuf.buf+RsltBuf.len,"%u",db_term_entry[hit_poss[i]].pos_POS[j]);
				RsltBuf.len = strlen(RsltBuf.buf);
			}
			sprintf(RsltBuf.buf+RsltBuf.len,"}");
			RsltBuf.len = strlen(RsltBuf.buf);
		}else{
			//no hit terms
			sprintf(RsltBuf.buf+RsltBuf.len,"\n%llX,{}",queries[i]);
			RsltBuf.len = strlen(RsltBuf.buf);
		}
	}
	//result tailing
	sprintf(RsltBuf.buf+RsltBuf.len,"\n");
	RsltBuf.len = strlen(RsltBuf.buf);
	DB(fprintf(stderr,"hits:::%d:::\n",total_hit));
	//return
        DB(fprintf(stderr,"OUT ExecCommSearch()\n"));
	return(0);
}
int ExecCommShlinkSearchList(const DATA_BUF comm){
        DB(fprintf(stderr,"IN ExecCommSearch()\n"));
        DB(fprintf(stderr,"command in ExecCommSearchList:::%s:::",comm.buf));
	char query_str[SHORT_BUF_SIZE];
	int query_str_len = 0;
	int num_query_entry = 0;
	unsigned long long int *queries;
	unsigned long long int *hit_poss;
	int num_queries = 0;
	int hit = 0;
	int i;
	int j;
	int scan = 0;
	int num_delims = 0;
	//scan number of ngrams
	for(i=0;i<comm.len;i++){
		if(comm.buf[i] == '?'){
			scan++;
		}
		if(scan > 0){
			if(comm.buf[i] == NGRAM_LIST_DELIM){
				num_delims++;
			}
		}
	}
	num_queries = num_delims+1;
	//allocation
	queries = ulli_calloc_vec(num_queries);
	hit_poss = ulli_calloc_vec(num_queries);
	//scan ngrams
	scan = 0;
	query_str_len = 0;
	num_query_entry = 0;
	DB(fprintf(stderr,"\ncomm.buf:::%s:::\n",comm.buf));
	for(i=0;i<comm.len;i++){
		if(comm.buf[i] == '?'){
			scan++;
			i++;
		}
		if(scan > 0){
			if(comm.buf[i] == NGRAM_LIST_DELIM){
				query_str[query_str_len] = '\0';
				DB(fprintf(stderr,"query_str:::%s:::\n",query_str));
				sscanf(query_str,"%llx",queries+num_query_entry);
				num_query_entry++;
				query_str[0] = '\0';
				query_str_len = 0;
			}else{
				query_str[query_str_len] = comm.buf[i];
				query_str_len++;
			}
		}
	}
	if(query_str[0] != '\0'){
		query_str[query_str_len] = '\0';
		DB(fprintf(stderr,"query_str:::%s:::\n",query_str));
		sscanf(query_str,"%llx",&queries[num_query_entry]);
		num_query_entry++;
	}
	//search
	DB(fprintf(stderr,"num_queries:::%d:::\n",num_queries));
	hit = 0;
	for(i=0;i<num_query_entry;i++){
		DB(fprintf(stderr,"query:::%llx:::\n",queries[i]));
		hit += ArtDBTermBinSearch(queries[i],db_term_entry,db_profile.num_term,0,&hit_poss[i]);
	}
	DB(fprintf(stderr,"hits:::%d:::\n",hit));
        	//sprintf(RsltBuf.buf+RsltBuf.len,"hoge");
	DB(fprintf(stderr,"!!!HOGE!!!\n"));
	//init buf
	if(RsltBuf.buf == NULL){
		InitBuf(&RsltBuf,SHORT_BUF_SIZE);
	}
	if(hit > 0){
        	sprintf(RsltBuf.buf+RsltBuf.len,"\nHit:\n");
		RsltBuf.len = strlen(RsltBuf.buf);
		if(ExpandBuf(&RsltBuf,SHORT_BUF_SIZE*num_query_entry) == -1){
			fprintf(stderr,"failed: ExpandBuf().\n");
		}
		for(i=0;i<num_query_entry;i++){
			sprintf(RsltBuf.buf+RsltBuf.len,"%llX,{",db_term_entry[hit_poss[i]].termINT);
			RsltBuf.len = strlen(RsltBuf.buf);
				sprintf(RsltBuf.buf+RsltBuf.len,"%u,",db_term_entry[hit_poss[i]].pos_ID[0]);
				RsltBuf.len = strlen(RsltBuf.buf);
				sprintf(RsltBuf.buf+RsltBuf.len,"%u",db_term_entry[hit_poss[i]].pos_POS[0]);
				RsltBuf.len = strlen(RsltBuf.buf);
			for(j=1;j<db_term_entry[hit_poss[i]].num_pos;j++){
				sprintf(RsltBuf.buf+RsltBuf.len," %u,",db_term_entry[hit_poss[i]].pos_ID[j]);
				RsltBuf.len = strlen(RsltBuf.buf);
				sprintf(RsltBuf.buf+RsltBuf.len,"%u",db_term_entry[hit_poss[i]].pos_POS[j]);
				RsltBuf.len = strlen(RsltBuf.buf);
			}
			sprintf(RsltBuf.buf+RsltBuf.len,"}\n");
			RsltBuf.len = strlen(RsltBuf.buf);
		}
	}
	//return
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


//interpreter
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
                }else if(strncmp(comm.buf,"ShlinkSearchList?",17) == 0){
                        ExecCommShlinkSearchList(comm);
                        *commecho = 1;
                        *endflag = 0;
                        return(1);
                }else if(strncmp(comm.buf,"SearchList?",11) == 0){
                        ExecCommSearchList(comm);
                        *commecho = 1;
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
