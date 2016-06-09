#include"BedFile.h"
using namespace std;

BedFile::BedFile(std::string filename)
{
	try
	{
		ifstream test(filename.c_str());
	
		if (!test.is_open())
			throw runtime_error("Bed file [" + filename + "] cannot be opened!");
	
		test.close();
		infile = filename;
	}
	catch (const runtime_error &e)
	{
		cerr << "BEDFILE: Bedfile(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

void BedFile::parse(void)
{
	try
	{
		ifstream file(infile);
		
		string line;
		while(getline(file,line) && !file.bad())
		{
			stringstream ss(line);	
			string chr, extra_cols;
			int start, end;

			if (! (ss >> chr >> start >> end))
			{
				cout << "skipping line: [" << line << "]" << endl;
				continue;
			}		
			
			getline(ss,extra_cols);		
			
			other_cols[chr].push_back(extra_cols);

			Peak tmp;	
			tmp.start = start;
			tmp.end = end;
			tmp.value = 0.0;

			peaks[chr].push_back(tmp);
		}

		sort_peaks();

	}
	catch(const runtime_error &e)
	{
		cerr << "BEDFILE: parse(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

void BedFile::map(Signal ** wig)
{
	for (auto chrIter = peaks.begin(); chrIter != peaks.end(); chrIter++)
	{
		if (!(*wig)->checkChr(chrIter->first))
			continue;

		for (auto peakIter = peaks[chrIter->first].begin(); peakIter != peaks[chrIter->first].end(); chrIter++)
		{
			vector<Peak> * wigOverlap = new vector<Peak>();
			wigOverlap = (*wig)->getOverlapPeaks(chrIter->first, *peakIter);
			for (auto wigIter = wigOverlap->begin(); wigIter != wigOverlap->end(); wigIter++)
			{
				unsigned int start = wigIter->start;	
				unsigned int end = wigIter->end;

				if (start < peakIter->start)
					start = peakIter->start;
				
				if (end > peakIter->end)
					end = peakIter->end;

				peakIter->value += wigIter->value * (end - start);
			}
			delete wigOverlap;
		}	
	}
}

void BedFile::output(void)
{
	try
	{
		ofstream file(outfile);

		if (!file.is_open())
			throw runtime_error("Could not open outfile [" + outfile + "]");

		for (auto chrIter = sortedChroms.begin(); chrIter != sortedChroms.end(); chrIter++)
		{
			for (unsigned int i = 0; i < peaks[*chrIter].size(); i++)
			{
				file << *chrIter << "\t" << peaks[*chrIter][i].start << "\t" << peaks[*chrIter][i].end << "\t" << peaks[*chrIter][i].value << "\t" << other_cols[*chrIter][i] << endl;
			}
		}	

		file.close();
	}
	catch (const runtime_error &e)
	{
		cerr << "BEDFILE:output(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

bool peakCmp(Peak peak1, Peak peak2)
{
	return (peak1.start < peak2.start);
}

void BedFile::sort_peaks(void)
{
	for (auto iter = peaks.begin(); iter != peaks.end(); iter++)
	{
		sortedChroms.push_back(iter->first);
		sort((iter->second).begin(), (iter->second).end(), peakCmp);
	}
	
	sort(sortedChroms.begin(), sortedChroms.end());
}
