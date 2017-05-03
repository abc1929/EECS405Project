A loosely VGram-Equipped PartEnum.

Currently using a direct frequency prune for hashmaps of grams (trie did not mesh well with the original code)
NAG was not able to bound to the desired depth due to data structure reason, but it works in principle.
Currently using raw string edit distance on predicate. NAGs can be switched on in the VPartEnum::vsearch()


Usage:
in /partenum

$ cmake .
$ make

run executable in /partenum/build

Swap CMakeLists and CMakeLists4 for compiling VGram-PartEnum and original PartEnum 

Modify /partenum/src/test.cc and /partenum/src/example.cc to adjust parameters and dataset 