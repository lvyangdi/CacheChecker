#!/usr/bin/perl
# Input: test trace
# Output: operations (int)
# Deal with quotient protocol here
# Only MESI so far
# Yangdi
#

if ($#ARGV + 1 != 2) {
 	print "Usage: $0 <inputfilename> <PROTOCOL>\n";
 	exit;
}

my $inputfile = $ARGV[0];

# orbit.txt contains information about orbit
# each line represents one orbit
# inside each line, nodes are splited using ','
my $orbitfile = "orbit.txt";

my $outputfile = "input.txt";
my $protocol = $ARGV[1];

open(INPUT, $inputfile)
	|| die "Error: failed to open $inputfile";
open(my $orbin, $orbitfile)
	|| die "Error: failed to open orbit file";
open(OUTPUT, ">$outputfile")
	|| die "Error: failed to create $outputfile";

# read orbits, stored in two dimensional array
my @orbits;
while (my $line = <$orbin>) {
	chomp $line;
	push (@orbits, [split ',', $line]);
}

my $numoforbits = scalar @orbits;
# effectnode is the the node with data inside each orbit
# initialized to be -1, means no node contains data
my @effectnode = ((-1)x$numoforbits);

# deal with original operation
# just rand
while (my $line = <INPUT>) {
	$line =~ />(\w{4})(\d+\n)/;
	print OUTPUT orbitrandom($2) . "\n";
	# After all invalidate, put "-1" to invalidate L2
	if ($1 eq "IIII") {
		print OUTPUT "-1\n";
	}
}

# if protocol contains E, deal with IE->IS transitions
if ($protocol =~ /E/) {
	print "E exist in protocol, generating extra transitions\n";
	
	for (my $i = 0; $i < $numoforbits; $i++) {
		# store->evict to get all "I" states
		print OUTPUT "1\n";
		print OUTPUT "2\n";
		print OUTPUT "-1\n";
		# if obit i contains only one node, skip
		if (scalar @{$orbits[$i]} == 1) {
			next;
		}
		# find two random nodes in orbit i
		$core1 = $orbits[$i][int(rand(scalar @{$orbits[$i]}))];
		$core2 = $orbits[$i][int(rand(scalar @{$orbits[$i]} - 1))];
		if ($core2 >= $core1) {
			$core2++;
		}
		# core1 load-> core2 load, IE->IS
		print OUTPUT ($core1 * 4);
		print OUTPUT "\n";
		print OUTPUT ($core2 * 4);
		print OUTPUT "\n";
	}
}

# if protocol contains O, deal with IO->IS transitions
if ($protocol =~ /O/) {
	print "O exist in protocol, generating extra transitions\n";
	
	for (my $i = 0; $i < $numoforbits; $i++) {
		# if obit i contains only one node, skip
		if (scalar @{$orbits[$i]} == 1) {
			next;
		}
		# find two random nodes in orbit i
		$core1 = $orbits[$i][int(rand(scalar @{$orbits[$i]}))];
		$core2 = $orbits[$i][int(rand(scalar @{$orbits[$i]} - 1))];
		if ($core2 >= $core1) {
			$core2++;
		}
		# core1 store-> core2 load, IO->IS
		print OUTPUT ($core1 * 4 + 1);
		print OUTPUT "\n";
		print OUTPUT ($core2 * 4);
		print OUTPUT "\n";
	}
}


close INPUT
	|| warn "Error: failed to close $inputfile";
close $orbin
	|| warn "Error: failed to close orbit file";
close OUTPUT
	|| warn "Error: failed to close $outputfile";

sub orbitrandom {
	my $oldop = shift(@_);
	my $orb = int($oldop / 4);
	my $op = $oldop - $orb * 4;
	my $core;
	if ($effectnode[$orb] != -1) {
		$core = $effectnode[$orb];
		if (op == 3) {
			$effectnode[$orb] = -1;
		}
	} else {
		$core = $orbits[$orb][int(rand(scalar @{$orbits[$orb]}))];
		exit "Invalid evict\n" if (op == 3);
		$effectnode[$orb] = $core;
	}
	return ($core * 4 + $oldop - $orb * 4);
}
