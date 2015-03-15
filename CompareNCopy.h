#ifndef _COMPARE_N_COPY_H_
#define _COMPARE_N_COPY_H_

#include <map>
#include <vector>
#include <string>

class CompareNCopy {
public:
    typedef enum CompareDataTypeT {
        CMP_TYPE_NUMBER,
        CMP_TYPE_VARCHAR,
        CMP_TYPE_DATE
    } CompareDataType;
    typedef struct HeaderTypesT {
        std::map<std::string, CompareDataType> keyMap;    
        std::map<std::string, CompareDataType> fieldMap;
    } HeaderTypes;
    typedef std::vector<std::string> RowData;
    typedef std::vector<RowData> DataTable;

    void Compare();
    void UpdateCustomerTable();
    //-- Singleton Methods
    static void CreateCompareNCopy();
    static CompareNCopy* RefCompareNCopy();
    static void DestroyCompareNCopy();  
private:
    CompareNCopy();
	~CompareNCopy();
    CompareNCopy (CompareNCopy const&);
    CompareNCopy& operator = (CompareNCopy const&);
    void UpdateHeaderTypes();
    void GetKeyPositions();
    void CompareDeliveryWithRef();
    void CompareCustomerWithRef();
    void ConflictCheck();
    void Display();
private:
    DataTable refTable, devTable, cusTable;
    std::vector<std::string> m_changedRefRows;
    std::vector<std::string> m_changedDelRows;
    std::vector<std::string> m_addedDelRows;
    std::vector<std::string> m_changedCustRows;
    std::vector<std::string> m_addedCustRows;
    std::vector<std::string> m_firstRow;
    std::vector<int>         m_keyPositions;
    std::vector<int>         m_fieldPositions;
    HeaderTypes          m_headerTypes;
    static CompareNCopy *s_compareNCopy;
};

#endif
