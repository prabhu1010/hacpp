#include <iostream>
#include "CompareNCopy.h"

int main (int argc, char *argv[]) {
    bool exitFlag = false;

    while (false == exitFlag) {
        int command = 0;

        std::cout << "=====================================================" << std::endl;
        std::cout << "             Compare And Copy Simulator              " << std::endl;
        std::cout << "=====================================================" << std::endl;
        std::cout << std::endl << std::endl;
        std::cout << "Commands for updating customer table: " << std::endl;
        std::cout << "\t1: Compare and Display results" << std::endl;
        std::cout << "\t2: Copy to Customer Table" << std::endl;
        std::cout << "\t3: Exit Compare and Copy Simulator" << std::endl;
        //-- Get the command from the user
        std::cin >> command;
        //-- Process the command received
        switch (command) {
        case 1:
            CompareNCopy::RefCompareNCopy()->Compare();
            break;
        case 2:
            CompareNCopy::RefCompareNCopy()->UpdateCustomerTable();
            break;
        case 3:
            std::cout << "You have choosen to exit !!! " << std::endl << std::endl;
            exitFlag = true;
            continue;
        default:
            std::cout << "Invalid Option !!! " << std::endl << std::endl;
        }
    }
    CompareNCopy::DestroyCompareNCopy();
    return 0;
}
