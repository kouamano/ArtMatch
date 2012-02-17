#!/usr/bin/perl

$od = 0;
%hs = ();
@od = ();
@term = ();
@pos = ();
@next = ();
@inline_next = ();
@hs_next = ();
while(<>){
	chomp;
	if(length($_) > 2){
		($term,$pos,$next) = split('\t',$_);
		push(@od,$od);
		push(@term,$term);
		push(@pos,$pos);
		push(@next,$next);
		$od++;
	}
}

for($i=0;$i<$od;$i++){
	$hs{$term[$i]} = $od[$i];
}

for($i=0;$i<$od;$i++){
	$next[$i] =~ s/^\s+//;
	$next[$i] =~ s/\s+$//;
	@inline_next = split('\s',$next[$i]);
	$num_inline_next = @inline_next;
	for($j=0;$j<$num_inline_next;$j++){
		push(@hs_next,$hs{$inline_next[$j]});
	}
	$hs_next = join(' ',@hs_next);
	#print "$od[$i]\t$term[$i]\t$pos[$i]\t$next[$i]\n";
	print "$od[$i]\t$term[$i]\t$pos[$i]\t";
	#print "$num_inline_next,@inline_next,@hs_next\n";
	print " $hs_next\n";
	@hs_next = ();
	$hs_next = '';
}
