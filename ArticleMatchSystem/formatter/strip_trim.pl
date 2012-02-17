#!/usr/bin/perl

while(<>){
	chomp;
	$_ =~ s/^\s//g;
	$_ =~ s/\s$//g;
	$_ =~ s/\.+$//g;
	print "$_\n";
}

exit(0);
