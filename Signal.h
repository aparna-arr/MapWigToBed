#ifndef SIGNAL_H
#define SIGNAL_H

#include"../Cache/Utils.h"
#include"../Cache/Cache.h"
#include<iterator>

class Signal 
{
	public:
	Signal(std::string filename, std::string cache_root);

	std::vector<Peak> * getOverlapPeaks(std::string chr, Peak bed_peak);	
	bool checkChr(std::string bedChr);
	
	private:
	unsigned int bsearch(std::string chr, unsigned int pos);
	
	Cache * fileCache;
	std::unordered_map<std::string, PeakInfo> * wigPeaks;
};

#endif

