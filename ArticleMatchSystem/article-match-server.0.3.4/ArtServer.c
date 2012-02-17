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
#define DB(X) X
#define SERVICE_NAME "ArtServer"
#define VERSION "Server version 0.2"
#define TIMEOUT_S 10
#define TIMEOUT_US 0
#define SHORT_BUF_SIZE 256
#define EXPAND_BUF_SIZE 64
#define BUF_SIZE 1024
#define LONG_BUF_SIZE 4096
#define HUGE_BUF_SIZE 1072693248 //1024 * 1024 * 1023
#define COMM_SEQ_END 0x04
//#define COMM_SEQ_END ';'
#define DEFAULT_ROOT "/usr/local/var/arts"

//typedef
#include "data_op.h"
#include "search_func.h"

//global vars
static DATA_BUF RecvBuf={NULL,0,0}; //static
static DATA_BUF CommBuf={NULL,0,0}; //static
static DATA_BUF SendBuf={NULL,0,0}; //alloc-free
static DATA_BUF RsltBuf={NULL,0,0}; //alloc-free
static int *TEST_DATA;
static int Soc = 0;

//my include
#include "/home/pub/include/alloc.c"
#include "comm_func.c"
#include "server_func.c"
#include "data_op.c"
#include "search_func.c"
#include "childloop.c"
#include "mainloop.c"


//top main
int main(int argc, char **argv){
	//init global vars
	InitBuf(&RecvBuf,SHORT_BUF_SIZE);
	InitBuf(&CommBuf,LONG_BUF_SIZE);

	//init socket
	if(InitSocket(SERVICE_NAME) == -1){
		return(-1);
	}

	//init signal - bind()
	InitSignal();

	//listen
	listen(Soc,SOMAXCONN);

	//mainloop - accept(), fork(), ChildLoop()
	MainLoop();

	//close socket
	CloseSocket();

	return(0);
}
