typedef struct {
        unsigned int db_no;
        unsigned int count;
        unsigned int *art_no;
}ART;

typedef struct {
        unsigned int ngram;
        unsigned int total_count;
        ART *art;
}NGRAM_UNSIG_32;
