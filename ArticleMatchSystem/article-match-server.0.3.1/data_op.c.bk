//print func
int DataPrint(FILE *fp, char *buf, int len){
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
        return(0);
}

//buff operation
int InitBuf(DATA_BUF *Buf, size_t size){
        int i;
        Buf->len = 0;
        Buf->size = size;
        Buf->buf = c_alloc_vec(Buf->size);
        for(i=0;i<Buf->size;i++){
                Buf->buf[i] = '\0';
        }
        return(Buf->size);
}

int ClearBuf(DATA_BUF *Buf){
        int i;
        for(i=0;i<Buf->size;i++){
                Buf->buf[i] = '\0';
        }
        Buf->len = 0;
        return(i);
}

int FreeBuf(DATA_BUF *Buf){
        if(Buf->buf != NULL){
                free(Buf->buf);
                Buf->buf = NULL;
                Buf->size = 0;
                Buf->len = 0;
                return(1);
        }
        return(0);
}

//recieve func
int RecvData(int acc, DATA_BUF *Buf){
        DB(fprintf(stderr,"RecvData()\n"));
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
                return(1);
        }
        return(0);
}

//send func
int SendData(int acc, DATA_BUF *Buf){
        int len=-2;
        int buf_start=0;
        DB(fprintf(stderr,"SendData()\n"));
        while(Buf->len > 0){
                len=send(acc,Buf->buf+buf_start,Buf->len,0);
                buf_start = buf_start + len;
                Buf->len = Buf->len - len;
        }
        DB(fprintf(stderr,"send %d byte {",len));
        DB(DataPrint(stderr,Buf->buf,len));
        DB(fprintf(stderr,"}\n"));
        free(Buf->buf);
        Buf->buf=NULL;
        Buf->size=0;
        Buf->len=0;
        return(0);
}

