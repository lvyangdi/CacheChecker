#!/usr/bin/perl

# my @protocols = ("MSI", "MESI", "MOSI", "MOESI");
my @protocols = ("MOESI");
# my @numcores = (2, 4, 8, 16);
my @numcores = (2);

foreach $p (@protocols){
    foreach $n (@numcores){
        my $output = `valgrind --tool=massif ./bin/CacheChecker $p $n BFS 2>&1 > /dev/null`;
    	system("mv massif* BFS-$p-$n.txt");
		sleep(60);
        # my $output = `valgrind --tool=massif ./bin/CacheChecker $p $n MCjammer 2>&1 > /dev/null`;
    	# system("mv massif* MCjammer-$p-$n.txt");
		# sleep(60);
		# to read the report
		# massif-visualizer $filename
	}
}

close($fh);

