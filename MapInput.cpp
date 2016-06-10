#include"MapInput.h"
using namespace std;

void usage(void)
{
	cout << "usage: mapwigtobed [-d <DEBUG_LEVEL 0..3>] -c <CACHE ROOT PATH> -i <WIGFILE> <BEDFILES>" << endl;
}

void parse_opts(int argc, char * argv[], std::string & cache_root, std::string & filename, vector<string> & bedfiles)
{
	try
	{
		bool has_i = false, has_c = false;
		int curr_opt;
		
		while( (curr_opt = getopt(argc, argv, "-i:c:d:")) != -1)
			switch(curr_opt)
			{
				case 1:
					debug("free arg: [" + string(optarg) + "]", 1);
					bedfiles.push_back(string(optarg));			
					break;	
				case 'i':
					filename = optarg;
					has_i = true;	
					break;
				case 'c':
					cache_root = optarg;
					has_c = true;
					break;
				case 'd':
				{
					int debugOpt;	

					if (!toInt(optarg, debugOpt))
						throw invalid_argument("-d requires an INT argument!");	
			
					DebugLevel(debugOpt);
				}
					break;
				case '?':
					if (optopt == 'i')
						throw invalid_argument ("-i requires a STRING argument!");
					else if (optopt == 'c')
						throw invalid_argument ("-c requires a STRING argument!");
					else if (optopt == 'd')
						throw invalid_argument ("-d requires an INT argument!");
					else
						throw invalid_argument ("Unrecognized argument!");	
		
					break;
				default:
					throw invalid_argument ("Something went wrong: default switch case on optopt [" + conv(optopt) + "]");	
			}

		if(!has_i && !has_c)
			throw invalid_argument("option -i and -c required!");	
		else if (!has_i)
			throw invalid_argument("option -i required!");
		else if (!has_c) 
			throw invalid_argument("option -c required!"); 
		else if (bedfiles.size() == 0)
			throw invalid_argument("bedfile(s) are required!");
	
	}
	catch (const invalid_argument &e)
	{
		cerr << "INVALID_ARGUMENT_ERROR: " << e.what() << endl << endl;
		usage();
		throw;
	}
}
