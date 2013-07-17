//init DB_PROFILE
void init_db_profile(DB_PROFILE *db_profile){
	DB(fprintf(stderr,"IN init_db_profile()\n"));
	db_profile->num_term = 0;
	db_profile->nums_pos = NULL;
	db_profile->nums_next = NULL;
	DB(fprintf(stderr,"OUT init_db_profile()\n"));
}

//alloc DB_TERM_ENTRY
DB_TERM_ENTRY *alloc_db_term_entry(unsigned int num){
	DB(fprintf(stderr,"IN alloc_db_term_entry()\n"));
	DB_TERM_ENTRY *db;
	if((db = malloc(sizeof(DB_TERM_ENTRY) * (size_t)num)) == NULL){
		perror("malloc()");
		exit(1);
	}
	DB(fprintf(stderr,"OUT alloc_db_term_entry()\n"));
	return(db);
}

//init "each" DB_TERM_ENTRY
void init_db_term_entry(DB_TERM_ENTRY *db_term_entry){
	DB(fprintf(stderr,"IN init_db_term_entry()\n"));
	db_term_entry->termINT = 0;
	db_term_entry->num_pos = 0;
	db_term_entry->pos_ID = NULL;
	db_term_entry->pos_POS = NULL;
	db_term_entry->num_next = 0;
	db_term_entry->next = NULL;
	DB(fprintf(stderr,"OUT init_db_term_entry()\n"));
}

//read db_n 
void read_db_n(OPT option){
	DB(fprintf(stderr,"IN read_db_n()\n"));
	FILE *n_file;
	if((n_file = fopen(option.n_file,"r")) == NULL){
		perror(option.n_file);
		exit(1);
	}
	fscanf(n_file,"%d",&db_n);
	fclose(n_file);
	DB(fprintf(stderr,"OUT read_db_n()\n"));
}

//raed term data function
void read_db_term(OPT option){
	DB(fprintf(stderr,"IN read_db_term()\n"));
	int i = 0;
	FILE *db_file;
	int cur_line_size = 0;
	int db_c = 0;
        int max_line_size = 0;
	int db_line = 0;
	int db_tab = 0;
        int cur_num_pos = 0;
        int cur_num_next = 0;
	char strbuf_termINT[BUF_SIZE];
        char strbuf_pos[BUF_SIZE];
        char strbuf_next[BUF_SIZE];
        int strlen_strbuf_pos = 0;
        int strlen_strbuf_next = 0;
        int strsize_termINT = 0;
        int strsize_pos = 0;
        int strsize_next = 0;
	int pos_entry = 0;
        int next_entry = 0;
        int in_str = 0;
        int end_line = 0;

	/* (* init vars */
	init_db_profile(&db_profile);
	DB(fprintf(stderr,"db_profile.num_term:%d:\n",db_profile.num_term));
	/* *) */

	/* (* open file */
	if((db_file = fopen(option.db_file,"r")) == NULL){
		perror(option.db_file);
		exit(1);
	}
	/* *) */
	/* (* check file format */
	fprintf(stderr,"scan(1) start.\n");
	while((db_c = fgetc(db_file)) != EOF){
		//count lines
		cur_line_size++;
		if(db_c == '\n'){
			if(cur_line_size > max_line_size){
				//check max line size
				max_line_size = cur_line_size;
			}
			cur_line_size = 0;
			db_line++;
		}
	}
	fprintf(stderr,"scan(1) end :: lines:%d: ;",db_line);
	fprintf(stderr,"max line size:%d:\n",max_line_size);
	//alloc DB_PROFILE
	db_profile.num_term = db_line;
	db_profile.nums_pos = i_alloc_vec(db_line);
	db_profile.nums_next = ui_alloc_vec(db_line);
	//count each entries
	fprintf(stderr,"scan(2) start.\n");
	fprintf(stderr,"seek:%d:\n",fseek(db_file,0U,SEEK_SET));
	db_line = 0;
	while((db_c = fgetc(db_file)) != EOF){
		if(db_c == '\n'){
			db_profile.nums_pos[db_line] = cur_num_pos;
			db_profile.nums_next[db_line] = cur_num_next;
			db_tab = 0;
			cur_num_pos = 0;
			cur_num_next = 0;
			db_line++;
		}else if(db_c == '\t'){
			db_tab++;
		}
		if(db_tab == 2){
			if(db_c == ' '){cur_num_pos++;}
		}else if(db_tab == 3){
			if(db_c == ' '){cur_num_next++;}
		}
	}
	if(DB_LEVEL >= 4){
		for(i=0;i<db_profile.num_term;i++){
			fprintf(stderr,"%d : pos,next : %d,%d\n",i,db_profile.nums_pos[i],db_profile.nums_next[i]);
		}
	}
	fprintf(stderr,"scan(2) end :: size of pos,next in each line.\n");
	/* *) */
	/* (* alloc DB_TERM_ENTRY */
	db_term_entry = alloc_db_term_entry(db_profile.num_term);
	for(i=0;i<db_profile.num_term;i++){
		db_term_entry[i].num_pos = db_profile.nums_pos[i];
		db_term_entry[i].pos_ID = ui_alloc_vec(db_term_entry[i].num_pos);
		db_term_entry[i].pos_POS = ui_alloc_vec(db_term_entry[i].num_pos);
		db_term_entry[i].num_next = db_profile.nums_next[i];
		db_term_entry[i].next = ui_alloc_vec(db_term_entry[i].num_next);
		
	}
	/* *) */
	/* (* free */
	free(db_profile.nums_pos);
	db_profile.nums_pos = NULL;
	free(db_profile.nums_next);
	db_profile.nums_next = NULL;
	/* *) */
	/* (* read data to DB from stream */
	fprintf(stderr,"scan(3) start.\n");
	fprintf(stderr,"seek:%d:\n",fseek(db_file,0U,SEEK_SET));
	db_line = 0;
	db_tab = 0;
	in_str = 0;
	while((db_c = fgetc(db_file)) != EOF){
		if(db_c == '\t'){
			in_str = 0;
			db_tab++;
			end_line = 0;
		}else if(db_c == '\n'){
			in_str = 0;
			db_tab = 0;
			db_line++;
			end_line = 1;
		}else{
			in_str = 1;
			end_line = 0;
		}

		if((db_tab == 1)&&(in_str == 1)){
			strbuf_termINT[strsize_termINT] = db_c;
			strsize_termINT++;
		}else if((db_tab == 2)&&(in_str == 0)){//termINT
			strbuf_termINT[strsize_termINT] = '\0';
			//read termINT
			sscanf(strbuf_termINT,"%x",&db_term_entry[db_line].termINT);
			strsize_termINT = 0;
			strbuf_termINT[0] = '\0';
		}else if((db_tab == 2)&&(in_str == 1)){//pos
			strbuf_pos[strsize_pos] = db_c;
			strsize_pos++;
		}else if((db_tab == 3)&&(in_str == 0)){
			strbuf_pos[strsize_pos] = '\0';
			strlen_strbuf_pos = strlen(strbuf_pos);
			pos_entry = 0;
			//read positions
			if(db_term_entry[db_line].num_pos > 0){
				for(i=0;i<strlen_strbuf_pos;i++){
					if(strbuf_pos[i] == ' '){
						sscanf(strbuf_pos+i," %u,%u",&db_term_entry[db_line].pos_ID[pos_entry],&db_term_entry[db_line].pos_POS[pos_entry]);
						pos_entry++;
					}
				}
			}
			/*
			for(i=0;i<db_term_entry[db_line].num_pos;i++){
				fprintf(stderr,"%u,%u\n",db_term_entry[db_line].pos_ID[i],db_term_entry[db_line].pos_POS[i]);
			}
			*/
			strsize_pos = 0;
		}else if((db_tab == 3)&&(in_str == 1)){//next
			strbuf_next[strsize_next] = db_c;
			strsize_next++;
		}else if(end_line == 1){// Caution::NEW db_line:: !!
			strbuf_next[strsize_next] = '\0';
			strlen_strbuf_next = strlen(strbuf_next);
			next_entry = 0;
			if(db_term_entry[db_line-1].num_next > 0){
				for(i=0;i<strlen_strbuf_next;i++){
					if(strbuf_next[i] == ' '){
						sscanf(strbuf_next+i," %u",&db_term_entry[db_line-1].next[next_entry]);
						next_entry++;
					}
				}
			}
			strsize_next = 0;
			//fprintf(stderr,"num_next:%u:\n",db_term_entry[db_line-1].num_next);
			/*
			for(i=0;i<db_term_entry[db_line-1].num_next;i++){
				fprintf(stderr,":%d:next(unsigned int):%u:\n",db_line-1,db_term_entry[db_line-1].next[i]);
			}
			*/
		}
	}
	fprintf(stderr,"scan(3) end.\n");
	/* *) */
	/* (* close */
	fclose(db_file);
	/* *) */
	DB(fprintf(stderr,"OUT read_db_term()\n"));
}

int ArtDBTermBinSearch(unsigned int query, DB_TERM_ENTRY *db, unsigned int max, unsigned int min, unsigned int *hit_pos){
	DB(fprintf(stderr,"IN ArtDBTermBinSearch()\n"));
        unsigned int target = min;
        unsigned int cur_max = max;
        unsigned int cur_min = min;
        while(cur_min <= cur_max){
		if((cur_max > max)||(cur_min > max)||(cur_min < min)||(cur_max < min)){
			DB(fprintf(stderr,"NO HIT\n"));
			return(0);
		}
                target = (unsigned int)(cur_min + cur_max)/2;
                if(query == (*(db+target)).termINT){
			*hit_pos = target;
			DB(fprintf(stderr,"OUT ArtDBTermBinSearch()\n"));
			return(1);
                }else if(query > (db+target)->termINT){
			*hit_pos = target;
                        cur_min = target + 1;
                }else if(query < db[target].termINT){
			*hit_pos = target;
                        cur_max = target - 1;
                }
		DB(fprintf(stderr,"min:cur_min::max:cur_max (current pos) (termINT) - %d:%d::%d:%d (%u) (%X)\n",min,cur_min,max,cur_max,target,db[target].termINT));
        }
	DB(fprintf(stderr,"NO HIT\n"));
	DB(fprintf(stderr,"OUT ArtDBTermBinSearch()\n"));
        return(0);
}

int ArtDBTermSkipSearch(unsigned int query, DB_TERM_ENTRY *db, unsigned int max, unsigned int min, unsigned int *hit_pos){
	DB(fprintf(stderr,"IN ArtDBTermSkipSearch()\n"));
        unsigned int cur_max = max;
        unsigned int cur_min = min;
        unsigned int cur_pos = min;
        unsigned int target = min;
        double target_float = (double)target;
	double delta = 0;
        while(cur_min <= cur_max){
		if((cur_max > max)||(cur_min > max)||(cur_min < min)||(cur_max < min)){
			DB(fprintf(stderr,"NO HIT\n"));
			return(0);
		}

		//calc delta, target
		delta = ((double)db[cur_max].termINT - (double)db[cur_min].termINT) / ((double)cur_max - (double)cur_min);
		target_float = cur_pos + (((double)query - db[cur_pos].termINT) / delta);
		target = (unsigned int)target_float;
		if(target > cur_max){
			target = cur_max;
		}else if(target < cur_min){
			target = cur_min;
		}

		//UNDER CONSTRUCTION
                if(query == (*(db+target)).termINT){
			*hit_pos = target;
			DB(fprintf(stderr,"OUT ArtDBTermSkipSearch()\n"));
			return(1);
                }else if(query > (db+target)->termINT){
			*hit_pos = target;
                        cur_min = target;
                }else if(query < db[target].termINT){
			*hit_pos = target;
                        cur_max = target;
                }else if(cur_min == cur_max){
			DB(fprintf(stderr,"NO HIT\n"));
			return(0);
		}
		DB(fprintf(stderr,"min:cur_min::max:cur_max (current pos) (termINT) - %d:%d::%d:%d (%u) (%X)\n",min,cur_min,max,cur_max,target,db[target].termINT));
		cur_pos = target;
        }
	DB(fprintf(stderr,"NO HIT\n"));
	DB(fprintf(stderr,"OUT ArtDBTermSkipSearch()\n"));
        return(0);
}

