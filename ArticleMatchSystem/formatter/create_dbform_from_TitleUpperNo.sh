#/bin/bash
FILE=$1
line2ngramINT_pos if=${FILE} -T | ngram2ngram_pair.pl if=/dev/stdin | ngramINT_pos_pair2termVSpos_article_next.pl |  sort | sorted_termVSpos_article_next2reduced_list.pl | reduced_list2ordering.pl
