//init DB_PROFILE
void init_db_profile(DB_PROFILE *db_profile){
	db_profile->num_term = 0;
	db_profile->nums_pos = NULL;
	db_profile->nums_next = NULL;
}

//alloc DB_TERM_ENTRY
DB_TERM_ENTRY *alloc_db_term_entry(unsigned long long int num){
	DB_TERM_ENTRY *db;
	if((db = malloc(sizeof(DB_TERM_ENTRY) * (size_t)num)) == NULL){
		perror("malloc()");
		exit(1);
	}
	return(db);
}

//init "each" DB_TERM_ENTRY
void init_db_term_entry(DB_TERM_ENTRY *db_term_entry){
	db_term_entry->termINT = 0;
	db_term_entry->num_pos = 0;
	db_term_entry->pos_ID = NULL;
	db_term_entry->pos_POS = NULL;
	db_term_entry->num_next = 0;
	db_term_entry->next = NULL;
}
//UNDER CONSTRUCTION

//check db file format
//UNDER CONSTRUCTION
void db_check_form(FILE *dbf, DB_PROFILE *db_profile, DB_TERM_ENTRY *db_term_entry){
}

//alloc DB_PROFILE
//UNDER CONSTRUCTION
int db_profile_alloc(const DB_PROFILE db_profile, DB_TERM_ENTRY *db_term_entry){
	return(0);
}

//alloc DB_TERM_ENTRY
//UNDER CONSTRUCTION
