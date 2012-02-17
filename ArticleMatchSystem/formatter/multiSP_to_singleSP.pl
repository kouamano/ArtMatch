#!/usr/bin/perl

while(<>){
	chomp;
	$_ =~ s/ +/ /g;
	$_ =~ s/^\s//;
	$_ =~ s/\s$//;
	print "$_\n";
}
