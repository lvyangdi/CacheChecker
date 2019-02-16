#!/usr/bin/perl
use 5.010;
# Input: trace file from gem5

$inputfile = "trace";

# Argument: number of cores
if ($#ARGV + 1 != 1) {
 	print "Usage: $0 <numberofcores>\n";
 	exit;
}

$numberofcores = $ARGV[0];

open(INPUT, $inputfile)
	|| die "Error: failed to open $inputfile\n";

# Init states for all node, not present
@states = ("NP")x$numberofcores;

# Deal with gem5 trace file
while ($line = <INPUT>) {
	# Only L1 is concerned
	if ($line =~ /\d+.*(\d).*L1Cache.*>(\w+).*\[(\w+),/) {
		# core 0 is for reading Input
		# only core 1..n is needed
		# The address is "0xd2c00"
		if (($1 > 0) && ($3 eq "0xd2c00")) {
			$states[$1 - 1] = $2;
			printstates();
		}
	}
}
close(INPUT);

# print states, mainly to eliminate transite states (more than one char)
sub printstates {
	$removedup = 0;
	state $prevstate = ("I")x$numberofcores;
	$newstate = "";
	foreach $s (@states) {
		if ($s eq "NP") {
			$newstate = $newstate . "I";
		} elsif (length $s == 1) {
			$newstate = $newstate . $s;
		} else {
			return;
		}
	}
	$prevstate = $newstate;
	if (($removedup) && ($prevstate eq $newstate)) {
		return;
	}
	print "$newstate\n";
}
