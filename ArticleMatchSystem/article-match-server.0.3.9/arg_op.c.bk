void InitOpt(OPT *option){
	option->root_file = c_calloc_vec(BUF_SIZE);
	sprintf(option->root_file,"%s",DEFAULT_ROOT);
	strcat(option->root_file,"/");
	strcat(option->root_file,DEFAULT_FILE);
	option->port = SERVICE_PORT;
	option->check = 0;
}


void GetOpt(int optc, char **optv, OPT *option){
	int i;
	for(i=0;i<optc;i++){
		if(strncmp(optv[i],"file=",5) == 0){
			sscanf(optv[i],"file=%s",option->root_file);
		}else if(strncmp(optv[i],"port=",5) == 0){
			sscanf(optv[i],"port=%d",&option->port);
		}else if(strncmp(optv[i],"--check",7) == 0){
			option->check = 1;
		}else{
			fprintf(stderr,"undefined option :%s:\n",optv[i]);
		}
	}
}
