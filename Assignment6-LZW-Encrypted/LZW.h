#ifndef LZW_Included
#define LZW_Included

#include "bstream.h"
#include "map.h"

void compress(ifstream& infile, obstream& outfile);
void decompress(ibstream& infile, ostream& outfile);

#endif