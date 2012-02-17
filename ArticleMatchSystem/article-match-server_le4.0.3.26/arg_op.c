void InitOpt(OPT *option){
	DB(fprintf(stderr,"IN InitOpt()\n"));
	//database file
	option->db_file = c_calloc_vec(BUF_SIZE);
	sprintf(option->db_file,"%s",DEFAULT_ROOT);
	strcat(option->db_file,"/");
	strcat(option->db_file,DEFAULT_DB_FILE);
	//n file
	option->n_file = c_calloc_vec(BUF_SIZE);
	sprintf(option->n_file,"%s",DEFAULT_ROOT);
	strcat(option->n_file,"/");
	strcat(option->n_file,DEFAULT_N_FILE);
	//port
	option->port = SERVICE_PORT;
	option->check = 0;
	DB(fprintf(stderr,"OUT InitOpt()\n"));
}


void GetOpt(int optc, char **optv, OPT *option){
	DB(fprintf(stderr,"IN GetOpt()\n"));
	int i;
	for(i=0;i<optc;i++){
		if(strncmp(optv[i],"db_file=",5) == 0){
			sscanf(optv[i],"db_file=%s",option->db_file);
		}else if(strncmp(optv[i],"port=",5) == 0){
			sscanf(optv[i],"port=%d",&option->port);
		}else if(strncmp(optv[i],"--check",7) == 0){
			option->check = 1;
		}else{
			fprintf(stderr,"undefined option :%s:\n",optv[i]);
		}
	}
	DB(fprintf(stderr,"OUT GetOpt()\n"));
}
