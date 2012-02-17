typedef struct {
	unsigned long long int num_term;
	int *nums_pos;
	unsigned long long int *nums_next;
}DB_PROFILE;

typedef struct {
	unsigned long long int termINT;
	unsigned int num_pos;
	unsigned int *pos_ID;
	unsigned int *pos_POS;
	unsigned int num_next;
	unsigned int *next;
}DB_TERM_ENTRY;

