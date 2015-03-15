/****************************************************************/
/* Submission by Prasath, Prabhu, Aruna                         */
/*  TableUpdater.cc : use Makefile in this repo to compile        */
/****************************************************************/
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <cstdio>    
#include <ctime>
#include <string.h>
#include <string>
#define DTABLE_OUT "DeliveryTable.csv"
#define CTABLE_OUT "CustomerTable.csv"
#define MODIFYCOUNT 10

using namespace std;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) 
    	++it;
    return !s.empty() && it == s.end();
}

int AddOrModify(const char *fName) {

	int times = MODIFYCOUNT;
	int i;

    	std::string line = "";
	ifstream src_fp(fName);
    	std::string backupFileName = string(fName) + ".bak";
    	std::ofstream pTarget(backupFileName);

	if (src_fp.is_open())
    	{
		getline(src_fp, line); 
		line = line + ",Flag";
		pTarget << line;
		pTarget << endl;

			for (i=0; i<3; i++) 
			{
       			getline (src_fp, line);
			std::istringstream record(line);
			std::vector<std::string> values;
			int modified=0;

				while (getline (record, line, ',')) { //for this one line
					if (!modified) {
					if (is_number(line)) {
						line = line.append("0");
						modified=1;
					}
					values.push_back(line);
					}
				}

				int j;
				for (j=0; j < values.size(); j++)  {
					pTarget << values[j];
					pTarget << ","; 
				}
				if (modified)
					pTarget << "1"<<endl; 
				else
					pTarget << "0"<<endl;
			}
		while (getline (src_fp, line))
		{
			line.pop_back();
			std::string suffix(",0");
			line = line + suffix;
			
			pTarget << line;
			pTarget <<endl;
		}
	}
	pTarget.close();
}

int main(int argc, char *argv[]) {
	
	AddOrModify(argv[1]);
	return 1;
}
