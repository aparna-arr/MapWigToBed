#include"Signal.h"
using namespace std;

Signal::Signal(std::string filename, std::string cache_root)
{
	try
	{
		fileCache = new Cache(cache_root, filename);
		wigPeaks = new unordered_map<string,PeakInfo>;

		if(!fileCache->get_data(wigPeaks))	
			throw runtime_error("Could not get data from filecache!");
	}
	catch (const runtime_error &e)
	{
		cerr << "SIGNAL:Signal(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

std::vector<Peak> * Signal::getOverlapPeaks(std::string chr, Peak bed_peak)
{
	vector<Peak>::iterator start = bsearch(chr, bed_peak.start);
	vector<Peak>::iterator end = bsearch(chr, bed_peak.end);

	if (start == (*wigPeaks)[chr]->peaks.end() || end == (*wigPeaks)[chr]->peaks.end())
		return new vector<Peak>(0);
	

	if (start->start > bed_peak.end)
		return new vector<Peak>(0);
	
	if (end->end < bed_peak.start)
		return new vector<Peak>(0);

	else
		return new vector<Peak>(start,end+1);
}
	
bool Signal::checkChr(std::string bedChr)
{
	return (wigPeaks->find(bedChr) != wigPeaks->end())
}
	
std::vector<Peak>::iterator Signal::bsearch(string chr, unsigned int pos)
{
	vector<Peak>::iterator min = (*wigPeaks)[chr].begin();
	vector<Peak>::iterator max = (*wigPeaks)[chr].end()-1;

	while(min <= max)
	{
		vector<Peak>::iterator mid = (min + max) / 2;
		
		if (mid->start < pos && mid->end < pos)
			min = mid + 1;
		if (mid->start > pos && mid->end > pos)
			max = mid - 1;
		else
			return mid;
	}
	
	if (mid->end < pos && mid != (*wigPeaks)[chr]->end() - 1)
		return mid+1;
}
