/*     ArtServer     */
/*     RIKEN BRC     */
/*    by K. AMANO    */

//include
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

//define
#define DB_LEVEL 3
#define DB(X) X
#define SERVICE_NAME "ArtServer"
#define SERVICE_PORT 19990
#define VERSION "Server version 0.3.23"
#define TIMEOUT_S 10
#define TIMEOUT_US 0
#define EXPAND_BUF_SIZE 64
#define SHORT_BUF_SIZE 256
#define BUF_SIZE 1024
#define LONG_BUF_SIZE 4096
#define LONG_LONG_BUF_SIZE 4194304 //1024 * 1024 * 4
#define HUGE_BUF_SIZE 1072693248 //1024 * 1024 * 1023
#define COMM_SEQ_END 0x04
#define NGRAM_LIST_DELIM ' '
//#define COMM_SEQ_END ';'
#define DEFAULT_ROOT "/usr/local/var/arts"
#define DEFAULT_CONF_FILE "arts.conf"
#define DEFAULT_DB_FILE "arts.idx"
#define DEFAULT_N_FILE "arts.N"

//typedef
#include "arg_op.h"
#include "data_op.h"
#include "db_func.h"

//global vars
static DATA_BUF RecvBuf={NULL,0,0}; //static
static DATA_BUF CommBuf={NULL,0,0}; //static
static DATA_BUF RsltBuf={NULL,0,0}; //alloc-free
static DATA_BUF SendBuf={NULL,0,0}; //alloc-free
static int Soc = 0;
static DB_N db_n;
static DB_PROFILE db_profile;
static DB_TERM_ENTRY *db_term_entry;

//my include
#include "/home/pub/include/alloc.c"
#include "arg_op.c"
#include "data_op.c"
#include "db_func.c"
#include "comm_func.c"
#include "server_func.c"
#include "childloop.c"
#include "mainloop.c"

//top main
int main(int argc, char **argv){
	DB(fprintf(stderr,"IN main()\n"));
	//argment analysis
	OPT serv_op;
	InitOpt(&serv_op);
	GetOpt(argc-1,argv+1,&serv_op);
	DB(fprintf(stderr,"n_file :%s:\n",serv_op.n_file));
	DB(fprintf(stderr,"data file :%s:\n",serv_op.db_file));
	DB(fprintf(stderr,"default service :%s:\n",SERVICE_NAME));
	DB(fprintf(stderr,"sub port :%d:\n",serv_op.port));

	//init global vars
	InitBuf(&RecvBuf,SHORT_BUF_SIZE);
	InitBuf(&CommBuf,LONG_BUF_SIZE);

	//init socket
	if(InitSocket(SERVICE_NAME,serv_op.port) == -1){
		return(-1);
	}

	//init signal - bind()
	InitSignal();

	//listen
	listen(Soc,SOMAXCONN);

	//data read
	read_db_n(serv_op);
        read_db_term(serv_op);

	//mainloop - data read, accept(), fork(), ChildLoop()
	MainLoop(serv_op);

	//close socket
	CloseSocket();

	DB(fprintf(stderr,"OUT main()\n"));
	return(0);
}
