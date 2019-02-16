if ($#ARGV + 1 < 1) {
	print "Usage: $0 filename\n";
	exit 0;
}

open(my $fh, "$ARGV[0]")
	or die "Could not open file '$ARGV[0]' $!"; 
open(my $fout, ">", "mem.dat")
	or die "Could not open file mem.dat $!"; 


while (my $row = <$fh>) {
	if ($row =~ /mem_heap_B=(\d+)/) {
		print $fout "$1\n";
	}
	if ($row =~ /time=(\d+)/) {
		print $fout "$1\t";
	}
}

close $fh;
close $fout;
