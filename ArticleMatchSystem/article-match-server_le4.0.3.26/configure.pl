#!/usr/bin/perl

#Makefile
$MAKEFILE = "Makefile";

#config dir
$CONF_DIR = "/usr/local/var/arts/";

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
	$INST_DIR = "/usr/local/bin/"
}elsif($OS_TYPE =~ /darwin/i){
	$INST_DIR = "/Users/pub/bin/"
}

#create Makefile
open(IN,">$MAKEFILE");
select(IN);
print <<"EOF";
All: ArtServer

ArtServer: *.c *.h
	gcc -Wall ArtServer.c -o ArtServer

install:
	cp ArtServer $INST_DIR
	cp arts.N $CONF_DIR
	cp arts.conf $CONF_DIR
EOF
