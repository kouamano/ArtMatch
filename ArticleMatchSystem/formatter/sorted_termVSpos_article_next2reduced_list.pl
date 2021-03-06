#!/usr/bin/perl
use strict;
use warnings;

my $prev_term = '';
my $current_term = '';
my $current_pos = '';
my $current_next = '';
my @prev_pos = ();
my @prev_next = ();
my %hash = ();

while(<>){
	chomp;
	($current_term,$current_pos,$current_next) = split('\t',$_);
	#print "$current_term --- \t";
	#print "$current_pos --- \t";
	#print "$current_next\n";
	#printf "prev:{$prev_term}:  current:{$current_term}:\n";
	#if($prev_term == $current_term){ #cause over flow
	if($prev_term eq $current_term){
		push(@prev_pos,$current_pos);
		push(@prev_next,$current_next);
	}else{
		print "$prev_term";
		print "\t";

		#print "@prev_pos\t";
		foreach my $line (sort {
			my $a_item = (split ',', $a)[0];
			my $b_item = (split ',', $b)[0];
			return $a_item <=> $b_item;
		} @prev_pos){
			print " $line";
		}
		print "\t";

		#print "@prev_next\t";
		my @prev_next_sort = sort(@prev_next);
		foreach my $line_n (@prev_next_sort) {
			$hash{$line_n} = 1;
		}
		my @prev_next_sort_uniq = keys %hash;
		#foreach my $line_n (@prev_next_sort) { #not uniq
		foreach my $line_ns (@prev_next_sort_uniq) { #uniq
			print " $line_ns";
		}
		print "\n";
		@prev_pos = ();
		@prev_next = ();
		%hash = ();
		push(@prev_pos,$current_pos);
		push(@prev_next,$current_next);
	}
	$prev_term = $current_term;
}
#last line
print "$prev_term\t";

#print "@prev_pos\t";
foreach my $line (sort {
	my $a_item = (split ',', $a)[0];
	my $b_item = (split ',', $b)[0];
	return $a_item <=> $b_item;
} @prev_pos){
	print " $line";
}
print "\t";

#print "@prev_next\t";
my @prev_next_sort = sort(@prev_next);
foreach my $line_n (@prev_next_sort) {
	$hash{$line_n} = 1;
}
my @prev_next_sort_uniq = keys %hash;
#foreach my $line_n (@prev_next_sort) { #not uniq
foreach my $line_ns (@prev_next_sort_uniq) { #uniq
	print " $line_ns";
}
print "\n";

