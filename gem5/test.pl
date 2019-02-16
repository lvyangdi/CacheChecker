#!/usr/bin/perl
# test with two neighbor nodes be one orbit
# Yangdi

if ($#ARGV + 1 != 1) {
 	print "Usage: $0 <inputfile>\n";
 	exit;
}
my $inputfile = $ARGV[0];
my $outputfile = "test.txt";

open(INPUT, $inputfile)
	|| die "Error: failed to open $inputfile";
open(OUTPUT, ">$outputfile")
	|| die "Error: failed to create $outputfile";

while (my $line = <INPUT>) {
	@array = split(//, $line);
	$newline = "";
	for (my $i = 0; $i < scalar @array - 1; $i += 2) {
		if ($array[$i] eq "M" || $array[$i + 1] eq "M") {
			$newline .= "M";
		} elsif ($array[$i] eq "E" || $array[$i + 1] eq "E") {
			$newline .= "E";
		} elsif ($array[$i] eq "O" || $array[$i + 1] eq "O") {
			$newline .= "O";
		} elsif ($array[$i] eq "S" || $array[$i + 1] eq "S") {
			$newline .= "S";
		} else {
			$newline .= "I";
		}
	}
	print OUTPUT "$newline\n";
}

close INPUT
	|| warn "Error: failed to close $inputfile";
close OUTPUT
	|| warn "Error: failed to close $outputfile";
