#include <fstream>
#include <sstream>
#include "Utils.h"
#include "CompareNCopy.h"
#include <iostream>

CompareNCopy* CompareNCopy::s_compareNCopy = NULL;


CompareNCopy::CompareNCopy() {
    UpdateHeaderTypes();
    GetKeyPositions();
}


CompareNCopy::~CompareNCopy() {}


//-- Singleton Methods
void CompareNCopy::CreateCompareNCopy() {
    if (NULL != s_compareNCopy) {
        return;
    }
    s_compareNCopy = new CompareNCopy();
}


CompareNCopy* CompareNCopy::RefCompareNCopy() {
    if (NULL != s_compareNCopy) {
        return s_compareNCopy;
    }
    CreateCompareNCopy();
    return s_compareNCopy;
}


void CompareNCopy::DestroyCompareNCopy() {
    if (NULL != s_compareNCopy) {
        delete s_compareNCopy;
        s_compareNCopy = NULL;
    }
}


std::string trim(const std::string& str,
                 const std::string& whitespace = " ")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


void CompareNCopy::UpdateHeaderTypes() {
    std::string line;
    std::string filePath ((std::string) COMPARE_FILE_PATH + (std::string) "Table_Structure.txt");
    std::ifstream tableStructure (filePath);
    if (tableStructure.is_open())
    {
        std::string keyValue = "";
        while (getline (tableStructure, line))
        {
            line = trim (line);
            std::istringstream row (line);
            if (line == "Keys") {
                keyValue = "key";
            } else if (line == "Fields") {
                keyValue = "field";
            } else {
                std::string value = "";
                std::string type = "";
                CompareDataType compareDataType;

			    getline (row, value, '-');
                value = trim (value);
			    getline (row, type, '-');
                type = trim (type);
                if (type == CMP_UTILS_DATE) {
                    compareDataType = CMP_TYPE_DATE;
                } else if (type == CMP_UTILS_NUMBER) {
                    compareDataType = CMP_TYPE_NUMBER;
                } else if (type == CMP_UTILS_VARCHAR) {
                    compareDataType = CMP_TYPE_VARCHAR;
                }
                if (keyValue == "key") {
                    m_headerTypes.keyMap.insert (std::make_pair (value, compareDataType));
                } else if (keyValue == "field") {
                    m_headerTypes.fieldMap.insert (std::make_pair (value, compareDataType));
                }
            }
        }
        tableStructure.close();
    }
}


void GetChangedEntries (std::string filePath, std::vector<std::string> &added, std::vector<std::string> &modified) {
    std::string refLine;
    std::ifstream changedFile (filePath);

    //-- Get customer table updated / added entries
    while (getline (changedFile, refLine)) {
            std::cout << refLine [refLine.size() - 1] << std::endl;
            if (2 == refLine [refLine.size() - 1]) {
                added.push_back (refLine);
            } else if (1 == refLine [refLine.size() - 1]) {
                modified.push_back (refLine);
            }
    }
    changedFile.close();
}


void CompareNCopy::GetKeyPositions() {
    std::string keyLine;
    std::string filePath ((std::string) COMPARE_FILE_PATH + (std::string) COMPARE_REF_TABLE);
    std::ifstream refFile (filePath);

    if (refFile.is_open()) {
        while (getline (refFile, keyLine))
        {
			if ((keyLine == "") || (keyLine[0] == '#')) {
				continue;
			}
			break;
        }
        std::istringstream keyStream (keyLine);
	    while (getline (keyStream, keyLine, ',')) {
            keyLine = trim (keyLine);
		    m_firstRow.push_back (keyLine);
	    }
        refFile.close();
    }

    std::map<std::string, CompareDataType>::iterator it = m_headerTypes.keyMap.begin();
    for (size_t i = 0; i < m_firstRow.size(); i++) {
        if ((it != m_headerTypes.keyMap.end()) && (m_firstRow[i] == it->first)) {
            m_keyPositions.push_back (i);
            ++it;
        } else {
            m_fieldPositions.push_back (i);
        }
    }
}


void CompareNCopy::Compare() {
    std::string filePath ((std::string) COMPARE_FILE_PATH + (std::string) COMPARE_REF_TABLE);
    std::ifstream refFile (filePath);
    std::string delFile  ("V:\\OracleHaCpp\\SampleData_Pblm1\\SampleData\\Challenge1\\NewDeliveryTable.csv");
    std::string custFile ((std::string) COMPARE_FILE_PATH + (std::string) COMPARE_CUSTOMER_TABLE);
    std::vector<std::string> addDelEnt;
    std::vector<std::string> modDelEnt;
    std::vector<std::string> addCustEnt;
    std::vector<std::string> modCustEnt;
    std::string customisedKey;
    int rowId = 0;

    //-- Get customer table updated / added entries
    GetChangedEntries (delFile, addDelEnt, modDelEnt);
    GetChangedEntries (custFile, addCustEnt, modCustEnt);

    //-- Get customer table updated / added entries
    std::string refLine;
    while (getline (refFile, refLine)) {
        std::vector<std::string> keys;
        std::vector<std::string> modDel;
        std::vector<std::string> modCust;
        bool checkForCust = false;
        bool first = true;
        bool changed = false;
        std::istringstream keyStream (refLine);

        rowId++;
	    while (getline (keyStream, refLine, ',')) {
		    keys.push_back (refLine);
        }
        std::string modLine;
        for (size_t i = 0; i < modDelEnt.size(); i++) {
            std::istringstream modStream (modDelEnt[i]);
	        while (getline (modStream, modLine, ',')) {
		        modDel.push_back (modLine);
            }
        }
        std::string custLine;
        for (size_t i = 0; i < modCustEnt.size(); i++) {
            std::istringstream modStream (modCustEnt[i]);
	        while (getline (modStream, custLine, ',')) {
		        modCust.push_back (modLine);
            }
        }
        for (size_t i = 0; i < m_keyPositions.size(); i++) {
            if (keys[m_keyPositions[i]] != modDel[m_keyPositions[i]]) {
                checkForCust = true;
                break;
            }
        }
        for (size_t i = 0; i < m_fieldPositions.size(); i++) {
            if (false == checkForCust) {
                if (keys[m_fieldPositions[i]] != modDel[m_fieldPositions[i]]) {
                    if (true == first) {
                        std::cout << "Row " << rowId << " - " << m_firstRow[m_fieldPositions[i]];
                        first = false;
                    } else {
                        std::cout << "," << m_firstRow[m_fieldPositions[i]];
                    }
                    changed = true;
                }
            }
            if (keys[m_keyPositions[i]] != modCust[m_keyPositions[i]]) {
                if (true == first) {
                    std::cout << "Row " << rowId << " - " << m_firstRow[m_fieldPositions[i]];
                    first = false;
                } else {
                    std::cout << "," << m_firstRow[m_fieldPositions[i]];
                }
            }
        }
    }
    refFile.close();
}

void CompareNCopy::UpdateCustomerTable() {
    //-- Add new entries
    std::ofstream custTable  ("V:\\OracleHaCpp\\SampleData_Pblm1\\SampleData\\Challenge1\\NewCustomerTable.csv");
    for (size_t i = 0; i < m_addedDelRows.size(); i++) {
        custTable << m_addedDelRows[i].c_str() << std::endl;
    }
    custTable.close();
    // Changed only in delivery table
    std::string currentLine;
    std::vector<std::vector<std::string>> modDel;
    for (size_t index = 0; index < m_changedDelRows.size(); index++) {
        std::istringstream modStream (m_changedDelRows[index]);
        std::string changedEntry;
        std::vector<std::string> modDelEntry;

        while (getline (modStream, changedEntry, ',')) {
		    modDelEntry.push_back (changedEntry);
        }
        modDel.push_back(modDelEntry);
    }
    std::vector<std::vector<std::string>> modCust;
    for (size_t index = 0; index < m_changedCustRows.size(); index++) {
        std::istringstream modStream (m_changedCustRows[index]);
        std::string changedEntry;
        std::vector<std::string> modDelEntry;

        while (getline (modStream, changedEntry, ',')) {
		    modDelEntry.push_back (changedEntry);
        }
        modCust.push_back(modDelEntry);
    }
    std::ifstream custTable1 ("V:\\OracleHaCpp\\SampleData_Pblm1\\SampleData\\Challenge1\\NewCustomerTable.csv");
    std::stringstream content;
    for (size_t index = 0; index < modDel.size(); index++) {
        for (size_t i = 0; i < modCust.size(); i++) {
            std::vector<std::string> modDelEntry = modDel[index];
            std::vector<std::string> modCustEntry = modCust[i];
            if (modCustEntry[m_keyPositions[i]] != modCustEntry[m_keyPositions[i]]) {
                continue;
            }
        }
        while (getline (custTable1, currentLine)) {
            
            std::istringstream modStream (currentLine);
            std::string custEntry;
            bool continueFlag = false;
        
            int wordPos = 0;
            while (getline (modStream, custEntry, ',')) {
                for (size_t i = 0; i < m_keyPositions.size(); i++) {
                    if (wordPos == m_keyPositions[i]) {
                        std::vector<std::string> modDelEntry = modDel[index];
                        if (custEntry != modDelEntry[m_keyPositions[i]]) {
                            continueFlag = true;
                            break;
                        }
                    }
                }
                if (continueFlag == true) {
                    content << currentLine << std::endl;
                    continue;
                }
                currentLine = m_changedDelRows[index];
                content << currentLine << std::endl;
            }
        }
    }
    custTable1.close();

    std::ofstream custTable3 ("V:\\OracleHaCpp\\SampleData_Pblm1\\SampleData\\Challenge1\\NewCustomerTable.csv");
    custTable3 << content;
    custTable3.close();

    //-- Update modified entries
    /*while (getline (custTable1, currentLine)) {
        bool continueFlag = false;
        std::istringstream modStream (currentLine);
        std::string custEntry;
        for (size_t index = 0; index < m_changedDelRows.size(); index++) {
            if (currentLine == m_changedDelRows[index]) {
                continueFlag = true;
                break;
            }
        }
        if (continueFlag == true) {
            continue;
        }
        while (getline (modStream, custEntry, ',')) {
		    for (size_t i = 0; i < m_keyPositions.size(); i++) {
                if (keys[m_keyPositions[i]] != modDel[m_keyPositions[i]]) {
                    checkForCust = true;
                    break;
                }
            }
        }
    }*/
    //custTable1.close();
}
