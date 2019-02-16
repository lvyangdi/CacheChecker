#!/bin/sh
#./genInput.pl MOESI4coredump MOESI
./genInput.pl MESI4coredump MESI

cd ~/Documents/gem5/
./scripts/checker.sh

mv m5out/trace ~/Documents/CacheChecker/gem5/.

cd ~/Documents/CacheChecker/gem5
./genResult.pl 8 > myout
