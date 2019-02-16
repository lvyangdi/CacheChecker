#!/usr/bin/perl
# @protocol = ("MSI", "MESI", "MOSI", "MOESI");
@protocol = ("MESI");
# @methods = ("MCjammer");

foreach $m (@methods){
	$p = "MESI";
	$filename = "$p"."_"."$m"."2.txt";
	print $filename;
	open(my $fh, ">$filename")
		or die "Could not open file";
	foreach $core (9..16){
		$sum = 0;
		foreach $times (1..5){
			$output = `./bin/CacheChecker $p $core dump $m`;
			$output =~ /Total edges: (\d+)/;
			$output = `sed -n '/ $1/p' ex_data.dat`;
			$output =~ /(\d+)/;
			$sum += $1;
		}
		$result = $sum / 5;
		print $fh "$result\n";
	}
	close($fh);
}
