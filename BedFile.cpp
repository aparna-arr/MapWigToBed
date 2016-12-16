#include"BedFile.h"
using namespace std;

BedFile::BedFile(std::string filename, std::string wigfilename)
{
	try
	{
		debug("BedFile::BedFile(): begin",1);
		ifstream test(filename.c_str());
	
		if (!test.is_open())
			throw runtime_error("Bed file [" + filename + "] cannot be opened!");
	
		test.close();
		infile = filename;

		int wigname_start = wigfilename.rfind("/") + 1;

		if (wigname_start < 0 || (unsigned) wigname_start == string::npos) 
			wigname_start = 0;

		int wigname_end = wigfilename.find(".wig");

		debug("wigfilename is [" + wigfilename + "] start is " + conv(wigname_start) + " end is " + conv(wigname_end),2);

		string wigname = wigfilename.substr(wigname_start, wigname_end - wigname_start);

		int bedname_start = filename.rfind("/") + 1;

		if (bedname_start < 0 || (unsigned) bedname_start == string::npos) 
			bedname_start = 0;

		int bedname_end = filename.find(".bed");
		debug("bedfilename is [" + filename + "] start is " + conv(bedname_start) + " end is " + conv(bedname_end),2);
		string bedname = filename.substr(bedname_start, bedname_end - bedname_start);
		
		
		outfile = wigname + "_" + bedname + ".mapwig";
		
		debug("outfile is [" + outfile + "]", 2);
	
		debug("BedFile::BedFile(): end",1);
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
		debug("BedFile::parse(): begin",1);
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
		debug("BedFile::parse(): end",1);

	}
	catch(const runtime_error &e)
	{
		cerr << "BEDFILE: parse(): RUNTIME_ERROR: " << e.what() << endl;
	}
}

void BedFile::map(Signal ** wig)
{
	debug("BedFile::map(): begin",1);
	for (auto chrIter = peaks.begin(); chrIter != peaks.end(); chrIter++)
	{
		debug("BedFile::map()::On chr [" + chrIter->first + "]",2);
		if (!(*wig)->checkChr(chrIter->first))
			continue;

		for (auto peakIter = peaks[chrIter->first].begin(); peakIter != peaks[chrIter->first].end(); peakIter++)
		{
			debug("BedFile::map()::begin of Bed peak loop",2);
			vector<Peak> * wigOverlap = new vector<Peak>;
			wigOverlap = (*wig)->getOverlapPeaks(chrIter->first, *peakIter);

		//	debug("BedFile::map(): wigOverlap size is [" + conv((int) wigOverlap->size())+ "]", 2);
			debug("BedFile::map(): before for loop",2);
			for (auto wigIter = wigOverlap->begin(); wigIter != wigOverlap->end(); wigIter++)
			{
//				debug("BedFile::map(): On wig peak [" + conv((int) (wigIter - wigOverlap->begin())) + "] out of [" + conv( (int) (wigOverlap->end() - wigOverlap->begin())) + "]", 2);
//				unsigned int start = wigIter->start;	
//				unsigned int end = wigIter->end;

				unsigned int start = 0, end = 0;

				if (wigIter->start < peakIter->end && wigIter->end > peakIter->start)
				{
					if (wigIter->start < peakIter->start)
						start = peakIter->start;
					else
						start = wigIter->start;
					
					if (wigIter->end > peakIter->end)
						end = peakIter->end;
					else
						end = wigIter->end;
	
					peakIter->value += wigIter->value * (end - start);
				}
				else
				{
					cerr << "Peak does not overlap!" << endl;
				}
				debug("BedFile::map(): End of wig peak loop",2);
			}
			debug("BedFile::map(): after for loop",2);
			delete wigOverlap;
			debug("BedFile::map(): after delete",3);
		}	
		debug("BedFile::map(): end of chr [" + chrIter->first + "]",2);
	}
	debug("BedFile::map(): end",1);
}

void BedFile::output(void)
{
	try
	{
		debug("BedFile::out(): begin",1);
		ofstream file(outfile);

		if (!file.is_open())
			throw runtime_error("Could not open outfile [" + outfile + "]");

		for (auto chrIter = sortedChroms.begin(); chrIter != sortedChroms.end(); chrIter++)
		{
			for (unsigned int i = 0; i < peaks[*chrIter].size(); i++)
			{
				char valstr[50];
				double val = (peaks[*chrIter][i].value / (peaks[*chrIter][i].end - peaks[*chrIter][i].start) );

				sprintf(valstr, "%.4f", val);

				file << *chrIter << "\t" << peaks[*chrIter][i].start << "\t" << peaks[*chrIter][i].end << "\t" << valstr << endl;
			}
		}	

		file.close();
		debug("BedFile::out(): end",1);
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
	debug("BedFile::sort_peaks(): begin",1);

	for (auto iter = peaks.begin(); iter != peaks.end(); iter++)
	{
		sortedChroms.push_back(iter->first);
		sort((iter->second).begin(), (iter->second).end(), peakCmp);
	}
	
	sort(sortedChroms.begin(), sortedChroms.end());
	debug("BedFile::sort_peaks(): end",1);
}
