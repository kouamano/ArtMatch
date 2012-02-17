#!/usr/bin/perl

#Makefile
$MAKEFILE = "Makefile";

#config dir
$CONF_DIR = "/usr/local/var/arts/";

#C code for a OS
$OS_TYPE = `echo \$OSTYPE`;
chomp($OS_TYPE);
print "OS:$OS_TYPE:\n";
	`echo '#define LINUX' > ostype.h`;
if($OS_TYPE =~ /linux/i){
	`echo #define LINUX > ostype.h`;
}elsif($OS_TYPE =~ /darwin/i){
	`echo '#define OSX' > ostype.h`;
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
All: ArtServer

ArtServer: *.c *.h
	gcc -Wall ArtServer.c -o ArtServer

install:
	cp ArtServer $INST_DIR
	cp arts.N $CONF_DIR
	cp arts.conf $CONF_DIR
EOF
