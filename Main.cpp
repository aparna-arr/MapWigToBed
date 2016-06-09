#include"../Cache/Utils.h"
#include"../Cache/Cache.h"
#include"MapInput.h"
#include"BedFile.h"
#include"Signal.h"

using namespace std;

int main(int argc, char * argv[])
{
	try
	{
		vector<string> beds;
		string cache_root, filename;
		parse_opts(argc, argv, cache_root, filename, beds);

		
	}
	catch (const invalid_argument &e)
	{
		return 1;
	}
}
