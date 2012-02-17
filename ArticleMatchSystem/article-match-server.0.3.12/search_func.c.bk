NGRAM_UNSIG_32 *ArtBinSearch(unsigned int query, NGRAM_UNSIG_32 *ngram, unsigned int max, unsigned int min){
	unsigned int target;
	unsigned int cur_max = max;
	unsigned int cur_min = min;
	while(min <= max){
		target = (unsigned int)(cur_min + cur_max)/2;
		if(query == (*(ngram+target)).term){
			return(ngram+target);
		}else if(query > (ngram+target)->term){
			cur_min = target + 1;
		}else if(query < ngram[target].term){
			cur_max = target - 1;
		}
	}
	return(NULL);
}
