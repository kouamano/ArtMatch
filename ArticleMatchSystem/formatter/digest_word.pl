#!/usr/bin/perl

$one;

while(<>){
	chomp;
	$_ =~ s/\.$//;
	$_ =~ s/$/ /;
	$one = $one.$_;
}

$one =~ s/\(/ ( /g;
$one =~ s/\)/ ) /g;
#$one =~ s/\{/ { /g;
#$one =~ s/\}/ } /g;
$one =~ s/\[/ [ /g;
$one =~ s/\]/ ] /g;
$one =~ s/\// \/ /g;
$one =~ s/: / : /g;
$one =~ s/\; / \; /g;
$one =~ s/, / , /g;
$one =~ s/\s/ /g;
@arr = split(' ', $one);

foreach(@arr){
	print "$_\n";
}
