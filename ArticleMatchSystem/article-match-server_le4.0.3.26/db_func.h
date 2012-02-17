//typedef
typedef int DB_N;

typedef struct {
	unsigned int num_term;
	int *nums_pos;
	unsigned int *nums_next;
}DB_PROFILE;

typedef struct {
	unsigned int termINT;
	unsigned int num_pos;
	unsigned int *pos_ID;
	unsigned int *pos_POS;
	unsigned int num_next;
	unsigned int *next;
}DB_TERM_ENTRY;

/*
typedef struct {
        unsigned int term;
        unsigned int total_pos_count;
        unsigned int total_neg_count;
        unsigned int *art_no;
        //int db_no;
}NGRAM_UNSIG_32;
*/
