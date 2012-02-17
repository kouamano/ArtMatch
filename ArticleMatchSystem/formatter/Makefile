all: line2ngramINT_pos

line2ngramINT_pos: line2ngramINT_pos.c *.h
	gcc -O3 line2ngramINT_pos.c -o line2ngramINT_pos

install:
	cp ./line2ngramINT_pos /usr/local/bin/
	cp ./toupper_addNO.pl /usr/local/bin/
	cp ./ngram2ngram_pair.pl /usr/local/bin/
	cp ./ngramINT_pos_pair2termVSpos_article_next.pl /usr/local/bin/
	cp ./sorted_termVSpos_article_next2reduced_list.pl /usr/local/bin/
	cp ./reduced_list2ordering.pl /usr/local/bin/
