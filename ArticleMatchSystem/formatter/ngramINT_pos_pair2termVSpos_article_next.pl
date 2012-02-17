#!/usr/bin/perl

while(<>){
	chomp;
	($ID,@terms) = split('\t',$_);
	foreach(@terms){
		($head,$tail) = split('~',$_);
		($tpos,$tterm) = split('\|',$tail);
		($hpos,$hterm) = split('\|',$head);
		print "$hterm\t$ID,$hpos\t$tterm\n";
	}
}
