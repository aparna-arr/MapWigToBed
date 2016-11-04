#include"Signal.h"
using namespace std;

Signal::Signal(std::string filename, std::string cache_root)
{
	try
	{
		debug("Signal::Signal(): begin",1);
		debug("Signal::Signal(): filename is [" + filename + "], cache_root is [" + cache_root + "]",2);

		fileCache = new Cache(cache_root, filename);
		wigPeaks = new unordered_map<string,vector<Peak>>;

		if(!fileCache->get_data(wigPeaks))	
			throw runtime_error("Could not get data from filecache!");
		debug("Signal::Signal(): end",1);
	}
	catch (const runtime_error &e)
	{
		cerr << "SIGNAL:Signal(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

std::vector<Peak> * Signal::getOverlapPeaks(std::string chr, Peak bed_peak)
{
	debug("Signal::getOverlapPeaks(): begin",1);
	unsigned int peaks_end = (*wigPeaks)[chr].size();
	unsigned int start = bsearch(chr, bed_peak.start);
	unsigned int end = bsearch(chr, bed_peak.end);
	debug("Signal::getOverlapPeaks(): after bsearch",2);

	if (start == peaks_end || end == peaks_end)
		return new vector<Peak>(0);
	
	if ((*wigPeaks)[chr][start].start > bed_peak.end)
		return new vector<Peak>(0);

	if ((*wigPeaks)[chr][end].end < bed_peak.start)
		return new vector<Peak>(0);

	vector<Peak> * result = new vector<Peak>;

	for (unsigned int i = start; i <= end; i++)
	{
		if ((*wigPeaks)[chr][i].start < bed_peak.end && (*wigPeaks)[chr][i].end > bed_peak.start)
			result->push_back((*wigPeaks)[chr][i]);
	}

	debug("Signal::getOverlapPeaks(): end successful",1);
	return result;
/*
	vector<Peak>::iterator start = bsearch(chr, bed_peak.start);
	vector<Peak>::iterator end = bsearch(chr, bed_peak.end);

	if (start == (*wigPeaks)[chr]-.end() || end == (*wigPeaks)[chr]-.end())
		return new vector<Peak>(0);
	

	if (start->start > bed_peak.end)
		return new vector<Peak>(0);
	
	if (end->end < bed_peak.start)
		return new vector<Peak>(0);

	return new vector<Peak>(start,end+1);
*/
}
	
bool Signal::checkChr(std::string bedChr)
{
	return (wigPeaks->find(bedChr) != wigPeaks->end());
}
	
unsigned int Signal::bsearch(string chr, unsigned int pos)
{
//	debug("bsearch start: chr " + chr + " pos " + conv(pos) + "\n",3);
	int min = 0;
	int max = (*wigPeaks)[chr].size() - 1;
	int mid = (min + max) / 2;

//	debug("min [" + conv(min) + "] max [" + conv(max) + "]\n", 3);

	while (min <= max)
	{
//		if (max < 0) {
//			debug("MAX < 0!\n",3);
//				break;
//		}
//		else {
//			debug("Max [" + conv(max) + "] >= 0", 3);
//		}
//		debug("[" + chr +"] [" + conv(pos) + " start of while min is [" + conv(min) + "] max is [" + conv(max) + "\n", 3);
		mid = (min + max) / 2;
//		debug("mid is " + conv(mid) + "]\n", 3);

//		debug("start: [" + conv((*wigPeaks)[chr][mid].start) + "]\n", 3);		

		if ((*wigPeaks)[chr][mid].start < pos && (*wigPeaks)[chr][mid].end < pos)
			min = mid + 1;
		else if ((*wigPeaks)[chr][mid].start > pos && (*wigPeaks)[chr][mid].end > pos)
			max = mid - 1;
		else
			return mid;
		
//		debug("end of while min is " + conv(min) + " max is " + conv(max), 3);
	}

//debug("after while", 3);

	if ((unsigned int) mid < (*wigPeaks)[chr].size() - 1 && (*wigPeaks)[chr][mid].end < pos)
		return mid + 1;

	return mid;

/*
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
*/
}
