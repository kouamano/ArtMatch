#!/usr/bin/perl

while(<>){
	chomp;
	($ID,@terms) = split('\t',$_);
	foreach(@terms){
		($head,$tail) = split('~',$_);
		($pos,$hterm) = split('\|',$head);
		print "$hterm\t$ID,$pos\n";
	}
}
