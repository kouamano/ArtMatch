#!/usr/bin/perl

$FILE = $ARGV[0];
$TITLE = 0;
$NO = 0;
@LIST = ();

if("-T" eq $ARGV[1]){
	$TITLE = 1;
}else{
	$TITLE = 0;
}

open(IN,$FILE);
while(<IN>){
	$_ =~ s/\s+/ /g;
	$_ =~ s/ $//;
	$_ =~ s/^ //;
	$_ =~ s/\.$//;
	$_ =~ s/$/\n/;
	$_ = uc $_;
	push(@LIST,$_);
}
close(IN);

if($TITLE == 0){
	foreach(@LIST){
		print "$NO\t$_";
		$NO++;
	}
}elsif($TITLE == 1){
	print "NO\t$LIST[0]";
	shift(@LIST);
	foreach(@LIST){
		print "$NO\t$_";
		$NO++;
	}
}
