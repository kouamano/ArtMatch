#!/usr/bin/perl

#Makefile
$MAKEFILE = "Makefile";

#C code for a OS
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

#make for a OS
if($OS_TYPE =~ /linux/i){
	$INST_DIR = "/home/pub/bin/"
}elsif($OS_TYPE =~ /darwin/i){
	$INST_DIR = "/Users/pub/bin/"
}

#create Makefile
open(IN,">$MAKEFILE");
select(IN);
print <<"EOF";
all: ArtClient

ArtClient: *.c *.h
	gcc -Wall ArtClient.c -o ArtClient

install:
	cp ArtClient $INST_DIR
EOF
