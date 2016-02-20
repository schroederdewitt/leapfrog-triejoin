Copyright 2013, Christian Schroeder de Witt, University of Oxford


./benchmark contains a benchmarking class providing a wall clock timer in milliseconds being used internally by both sortmerge and leapfrog to time the joins
./cmdArgs contains classes dealing with command line arguments and encapsulating and handling query file, database files and relation files
./globalFunctions contain globally used functions such as type conversions
./leapfrog is the wrapper around the leapfrog triejoin
./leapFrogTrieJoin contains the classes implementing the leapfrog triejoin based on the trieIterator
./linearIterator contains the linearIterator class being the fundament of the trieIterator
./multiIterator contains the multiIterator class used for the sortmerge join based on simpleIterator
./simpleIterator contains the simpleIterator class
./sortmerge is the wrapper around sortMergeMultiway
./sortMergeMultiway contains the classes implementing the sortmerge multiway join based on the multiIterator
./trieIterator contains the trieIterator class based on linearIterator

Each folder contains a subfolder ./test, which contains a unit test main.cpp which may be compiled using the string from the accompanying cmd.txt


