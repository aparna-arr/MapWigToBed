#ifndef BEDFILE_H
#define BEDFILE_H

#include"../Cache/Utils.h"
#include"Signal.h"
#include<algorithm>

class BedFile
{
	public:
	BedFile(std::string filename);
	
	void parse(void);
	void map(Signal ** wig); // pass address of Signal * object
	void output(void);

	private:

	void sort_peaks(void);

	std::string infile, outfile;
	std::unordered_map<std::string, std::vector<std::string>> other_cols;
	std::unordered_map<std::string, std::vector<Peak>> peaks;
	
	std::vector<std::string> sortedChroms;
};

#endif
