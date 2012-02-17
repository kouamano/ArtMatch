#!/usr/bin/perl

while(<>){
	chomp;
	($ID,@terms) = split('\t',$_);
	foreach(@terms){
		($head,$tail) = split('~',$_);
		$head =~ s/[0-9]*\|//;
		print "$head\t$ID\n";
	}
}
