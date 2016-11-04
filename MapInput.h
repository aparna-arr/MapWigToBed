#ifndef MAP_INPUT_H
#define MAP_INPUT_H

#include"../Cache/Utils.h"

void usageMap(void);
void parse_opts(int argc, char * argv[], std::string & cache_root, std::string & filename, std::vector<std::string> & bedfiles);

#endif
