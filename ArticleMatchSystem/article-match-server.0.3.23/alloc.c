/* alloc.c */

/*************************************************/
/* Copyright (c) 2003-2005 AMANO Kou             */
/* University of Tsukuba                         */
/* University of Liblary And Information Science */
/* National Institute of Agrobilogical Sciences  */
/* kamano@affrc.go.jp                            */
/*************************************************/

char **c_alloc_mat(int num, int dim){
	char **c;
	int i;
	if((c = malloc((size_t)sizeof(char *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(char *)*num);
		exit(1);
	}
	if((c[0] = malloc((size_t)sizeof(char)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(char)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		c[i] = c[i-1]+dim;
	}
	return(c);
}

char *c_alloc_vec(int num){
	char *c;
	if((c = malloc((size_t)sizeof(char)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(char)*num);
		exit(1);
	}
	return(c);
}

float **f_alloc_mat(int num, int dim){
	float **m;
	int i;
	if((m = malloc((size_t)sizeof(float *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(float *)*num);
		exit(1);
	}
	if((m[0] = malloc((size_t)sizeof(float)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(float)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

float *f_alloc_vec(int num){
	float *v;
	if((v = malloc((size_t)sizeof(float)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(float)*num);
		exit(1);
	}
	return(v);
}

int **i_alloc_mat(int num, int dim){
	int **m;
	int i;
	if((m = malloc((size_t)sizeof(int *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int *)*num);
		exit(1);
	}
	if((m[0] = malloc((size_t)sizeof(int)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

int **i_ralloc_mat(int num, int dim){
	int **m;
	int i;
	if((m = malloc((size_t)sizeof(int *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int *)*num);
		exit(1);
	}
	if((m[0] = malloc((size_t)sizeof(int)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

long long int **lli_alloc_mat(int num, int dim){
	long long int **m;
	int i;
	if((m = malloc((size_t)sizeof(long long int *)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int *)*num);
		exit(1);
	}
	if((m[0] = malloc((size_t)sizeof(long long int)*num*dim)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

int *i_alloc_vec(int num){
	int *v;
	if((v = malloc((size_t)sizeof(int)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

unsigned int *ui_alloc_vec(int num){
	unsigned int *v;
	if((v = malloc((size_t)sizeof(int)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

long long int *lli_alloc_vec(int num){
	long long int *v;
	if((v = malloc((size_t)sizeof(long long int)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

unsigned long long int *ulli_alloc_vec(int num){
	unsigned long long int *v;
	if((v = malloc((size_t)sizeof(unsigned long long int)*num)) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

unsigned long long int *ulli_calloc_vec(int num){
	unsigned long long int *v;
	if((v = calloc(num,(size_t)sizeof(unsigned long long int))) == NULL){
		fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

char **c_calloc_mat(int num, int dim){
	char **c;
	int i;
	if((c = calloc(num,(size_t)sizeof(char *))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(char *)*num);
		exit(1);
	}
	if((c[0] = calloc(num*dim,(size_t)sizeof(char))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(char)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		c[i] = c[i-1]+dim;
	}
	return(c);
}

char *c_calloc_vec(int num){
	char *c;
	if((c = calloc(num,(size_t)sizeof(char))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(char)*num);
		exit(1);
	}
	return(c);
}

float **f_calloc_mat(int num, int dim){
	float **m;
	int i;
	if((m = calloc(num,(size_t)sizeof(float *))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(float *)*num);
		exit(1);
	}
	if((m[0] = calloc(num*dim,(size_t)sizeof(float))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(float)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

float *f_calloc_vec(int num){
	float *v;
	if((v = calloc(num,(size_t)sizeof(float))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(float)*num);
		exit(1);
	}
	return(v);
}

double *d_calloc_vec(int num){
	double *v;
	if((v = calloc(num,(size_t)sizeof(double))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(double)*num);
		exit(1);
	}
	return(v);
}

int **i_calloc_mat(int num, int dim){
	int **m;
	int i;
	if((m = calloc(num,(size_t)sizeof(int *))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(int *)*num);
		exit(1);
	}
	if((m[0] = calloc(num*dim,(size_t)sizeof(int))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(int)*num*dim);
		exit(1);
	}
	for(i=1;i<num;i++){
		m[i] = m[i-1]+dim;
	}
	return(m);
}

int *i_calloc_vec(int num){
	int *v;
	if((v = calloc(num,(size_t)sizeof(int))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(int)*num);
		exit(1);
	}
	return(v);
}

float **f_calloc_triangle(int dim){
	float **ff;
	int i;
	//dim = dim + 1;
	if((ff = calloc(dim,(size_t)sizeof(float *))) == NULL){
		fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(float *)*dim);
		exit(1);
	}
	for(i=0;i<dim;i++){
		if((ff[i] = calloc(i,(size_t)sizeof(float))) == NULL){
			fprintf(stderr,"[E]failed : calloc() at %ld byte.\n",(long int)sizeof(float)*i);
			exit(1);
		}
	}
	return(ff);
}

float *f_balloc_vec(int num, int base){
        int i;
        float *v;
        if((v = malloc((size_t)sizeof(float)*num)) == NULL){
                fprintf(stderr,"[E]failed : malloc() at %ld byte.\n",(long int)sizeof(float)*num);
                exit(1);
        }
        for(i=0;i<num;i++){
                v[i] = 1;
        }
        return(v);
}

