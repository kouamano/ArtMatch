#!/usr/bin/perl

$file;
$N = 4;
$head = 1;
$check = 0;
$help = 0;
$im_ext = 0;

foreach(@ARGV){
	if($_ =~ /^if=/){
		$file = $_;
		$file =~ s/^if=//;
	}elsif($_ =~ /^N=/){
		$N = $_;
		$N =~ s/^N=//;
	}elsif($_ =~ /^-H$/){
		$head = 1;
	}elsif($_ =~ /^+H$/){
		$head = 0;
	}elsif($_ =~ /^-c$/){
		$check = 1;
	}elsif($_ =~ /^+c$/){
		$check = 0;
	}elsif($_ =~ /^-h$/){
		$help = 1;
	}elsif($_ =~ /^+h$/){
		$help = 0;
	}else{
		print "Unknown option:$_:\n";
	}
}

if($check == 1){
	print "\n";
	print "file:$file:\n";
	print "N:$N:\n";
	print "head:$head:\n";
	print "check:$check:\n";
	$im_ext++;
}

if($help == 1){
	print "\n";
	print "PROGRAM: ngram2ngram_pair.pl\n";
	print "DESCRIPTION: generate ngram pair from tab-separated ngram.\n";
	print "USAGE: ngram2ngram_pair.pl file=<file> N=<N> [-H|+H] [-h] [-c]\n";
	print "OPTIONS: \n";
	print "    <file>: input file.\n";
	print "    <N>: N of N-gram.\n";
	print "    -H: line with heading / +H: line without heading.\n";
	print "    -h: help.\n";
	print "    -c: print options.\n";
	$im_ext++;
}

if($im_ext > 0){
	exit(0);
}

#main routine
open(IN,$file);
while(<IN>){
	chomp;
	$line = $_;
	@line = split('\t',$line);
	print "$line[0]";
	shift(@line);
	$len = @line;
	for($i=0;$i<$len;$i++){
		print "\t$line[$i]~$line[$i+4]";
	}
	print "\n";
}
close(IN);

exit(0);
