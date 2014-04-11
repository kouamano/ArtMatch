#/bin/bash
FILE=$1

toupper_addNO.pl $FILE -T | line2ngramINT_pos if=/dev/stdin -T | ngram2ngram_pair.pl if=/dev/stdin | ngramINT_pos_pair2termVSpos_article_next.pl |  sort | sorted_termVSpos_article_next2reduced_list.pl | reduced_list2ordering.pl
