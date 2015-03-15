/****************************************************************/
/* Submission by Prasath, Prabhu, Aruna                         */
/*  Tablemaker.cc : use Makefile in this repo to compile        */
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

int FileCopy(const char *fName)
{	
    ifstream src_fp(fName);
    std::string line = "";
    std::ofstream pDeliv(DTABLE_OUT);
    std::ofstream pCust(CTABLE_OUT);

    std::string backupFileName = string(fName) + ".bak";
    std::ofstream pBackup(backupFileName);

    if (src_fp.is_open())
    {
        while (getline (src_fp, line))
        {
		line.pop_back();
		std::string suffix(",0");
		line = line + suffix;
		
	        pDeliv << line;
		pDeliv <<endl;

	        pCust << line;
	        pCust << endl;

		pBackup << line;
		pBackup << endl;
        }
    }

    src_fp.close();
    pDeliv.close();
    pCust.close();

    return 1;
}

int main(int argc, char *argv[]) {
	
	if (!FileCopy(argv[1])) {
		cout<<"Error creating Delivery and Customer data"<<endl;
		return 0;
	}
	else
		cout<<"Created Delivery and Customer Tables from Input File"<<endl;
}
