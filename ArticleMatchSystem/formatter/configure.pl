#!/usr/bin/perl

#Makefile
$MAKEFILE = "Makefile";

#C code for OS
$OS_TYPE = `uname`;
chomp($OS_TYPE);
print "OS:$OS_TYPE:\n";
if($OS_TYPE =~ /linux/i){
	open(OUT,"> ostype.h");
	print OUT "#define LINUX\n";
	close(OUT);
}elsif($OS_TYPE =~ /darwin/i){
	open(OUT,"> ostype.h");
	print OUT "#define OSX\n";
	close(OUT);
}

#make for OS
if($OS_TYPE =~ /linux/i){
	$INST_DIR = "/usr/local/bin/"
}elsif($OS_TYPE =~ /darwin/i){
	$INST_DIR = "/Users/pub/bin/"
}

#create makefile
open(IN,">$MAKEFILE");
select(IN);
print <<"EOF";
all: line2ngramINT_pos

line2ngramINT_pos: line2ngramINT_pos.c *.h
	gcc -O3 line2ngramINT_pos.c -o line2ngramINT_pos

install:
	cp ./line2ngramINT_pos $INST_DIR
	cp ./toupper_addNO.pl $INST_DIR
	cp ./ngram2ngram_pair.pl $INST_DIR
	cp ./ngramINT_pos_pair2termVSpos_article_next.pl $INST_DIR
	cp ./sorted_termVSpos_article_next2reduced_list.pl $INST_DIR
	cp ./reduced_list2ordering.pl $INST_DIR
EOF
