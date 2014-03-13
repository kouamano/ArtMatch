//print func
int DataPrint(FILE *fp, char *buf, int len){
	DB(fprintf(stderr,"IN DataPrint()\n"));
        int i;
        for(i=0;i<len;i++){
                if(buf[i] == '['){
                        fprintf(fp,"[%02x]",(unsigned int)(buf[i]&0xFF));
                }else if(buf[i] == ']'){
                        fprintf(fp,"[%02x]",(unsigned int)(buf[i]&0xFF));
                }else if(isascii(buf[i])&&isprint(buf[i])){
                        fputc(buf[i],fp);
                }else{
                        fprintf(fp,"[%02x]",(unsigned int)(buf[i]&0xFF));
                }
        }
	DB(fprintf(stderr,"OUT DataPrint()\n"));
        return(0);
}

//buff operation
int InitBuf(DATA_BUF *Buf, size_t size){
	DB(fprintf(stderr,"IN InitBuf()\n"));
	DB(fprintf(stderr,"buff size:%ld:\n",size));
        int i;
        Buf->len = 0;
        Buf->size = size;
	if(Buf->buf == NULL){
        	Buf->buf = c_alloc_vec(Buf->size);
	}
        for(i=0;i<Buf->size;i++){
                Buf->buf[i] = '\0';
        }
	DB(fprintf(stderr,"OUT InitBuf()\n"));
        return(Buf->size);
}

int ExpandBuf(DATA_BUF *Buf, size_t exsize){
	DB(fprintf(stderr,"IN ExpandBuf()\n"));
	int i;
	Buf->size = Buf->size + exsize;
	if((Buf->buf = realloc((char *)Buf->buf,Buf->size)) == NULL){return(-1);}
	for(i=Buf->len;i<Buf->size;i++){
		Buf->buf[i] = '\0';
	}
	DB(fprintf(stderr,"OUT ExpandBuf()\n"));
	return(0);
}

int ClearBuf(DATA_BUF *Buf){
	DB(fprintf(stderr,"IN ClearBuf()\n"));
        int i;
        for(i=0;i<Buf->size;i++){
                Buf->buf[i] = '\0';
        }
        Buf->len = 0;
	DB(fprintf(stderr,"OUT ClearBuf()\n"));
        return(i);
}

int FreeBuf(DATA_BUF *Buf){
	DB(fprintf(stderr,"IN FreeBuf()\n"));
        if(Buf->buf != NULL){
                free(Buf->buf);
                Buf->buf = NULL;
                Buf->size = 0;
                Buf->len = 0;
		DB(fprintf(stderr,"OUT FreeBuf()\n"));
                return(0);
        }
	DB(fprintf(stderr,"OUT FreeBuf()\n"));
        return(1);
}

int AddSendOver(){
	DB(fprintf(stderr,"IN AddSendOver()\n"));
	if(SendBuf.len + 2 > SendBuf.size){
		return(-1);
	}
	SendBuf.buf[SendBuf.len] = 0x04;
	SendBuf.buf[SendBuf.len + 1] = '\0';
	SendBuf.len = strlen(SendBuf.buf);
	DB(fprintf(stderr,"OUT AddSendOver()\n"));
	return(0);
}

int AddOver(DATA_BUF *Buf){
	DB(fprintf(stderr,"IN AddOver()\n"));
        if(Buf->len + 2 > Buf->size){
                return(-1);
        }
        Buf->buf[Buf->len] = 0x04;
        Buf->buf[Buf->len + 1] = '\0';
        Buf->len = strlen(Buf->buf);
	DB(fprintf(stderr,"OUT AddOver()\n"));
        return(0);
}

//recieve func
int RecvData(int acc, DATA_BUF *Buf){
        DB(fprintf(stderr,"IN RecvData()\n"));
        int len;
        int i;
        for(i=0;i<Buf->size;i++){
                Buf->buf[i] = '\0';
        }
        len=recv(acc,Buf->buf,(sizeof(char) * Buf->size),0);
        Buf->len = len;
        DB(fprintf(stderr,"recv %d byte {",len));
        DB(DataPrint(stderr,Buf->buf,len));
        DB(fprintf(stderr,"}\n"));
        if(len==0){
        	DB(fprintf(stderr,"OUT RecvData()\n"));
                return(1);
        }
        DB(fprintf(stderr,"OUT RecvData()\n"));
        return(0);
}

//send func
int SendData(int acc, DATA_BUF *Buf){
        DB(fprintf(stderr,"IN SendData()\n"));
        int len=-2;
        int buf_start=0;
        while(Buf->len > 0){
                len=send(acc,Buf->buf+buf_start,Buf->len,0);
                buf_start = buf_start + len;
                Buf->len = Buf->len - len;
        }
        DB(fprintf(stderr,"send %d byte {",len));
        DB(DataPrint(stderr,Buf->buf,len));
        DB(fprintf(stderr,"}\n"));
	FreeBuf(Buf);
	/*
        free(Buf->buf);
        Buf->buf=NULL;
        Buf->size=0;
        Buf->len=0;
	*/
        DB(fprintf(stderr,"OUT SendData()\n"));
        return(0);
}

