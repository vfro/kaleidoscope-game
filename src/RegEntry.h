#ifndef _REGENTRY_
#define _REGENTRY_

#include <atlbase.h>
#include <string>
#include <vector>

class aRegExcept;
class aRegKeys;
class aRegValues;

class aRegEntry
{
public:
   aRegEntry();
   ~aRegEntry();
   const std::string& GetKeyName() const;
   void CloseKey();
   void OpenKey(HKEY hKey, const std::string& szSubKey) throw (aRegExcept);
   void OpenSubKey(const std::string& szSubKey) throw (aRegExcept);
   void OpenSubKey(const aRegEntry& reg, const std::string& szSubKey) 
      throw (aRegExcept);
   void CreateKey(const std::string& szKey, bool bPassage = false) throw (aRegExcept);
   void DeleteKey(const std::string& szKey) throw (aRegExcept);
   void RecursiveDeleteKey(const std::string& szKey) throw (aRegExcept);
   void DeleteThisKey() throw (aRegExcept);
   void CreateStringValue(const std::string& szName, const std::string& szValue) const
      throw (aRegExcept);
   void CreateValue(const std::string& szName,
                    DWORD dwType,
                    DWORD dwSize,
                    const unsigned char* cValue);
   const std::string GetStringValue(const std::string& szName) const;
   void GetValue(const std::string& szName,
                            DWORD& dwType,
                            DWORD& dwSize,
                            unsigned char* cBuffer) const;
   void QueryKeys(aRegKeys& rKeys) const;
   void QueryValues(aRegValues& rValues) const;
private:
   HKEY m_hKey;
   std::string m_szKeyName;
};

class aRegExcept
{
public:
   aRegExcept(const std::string& szMessage);
   void SetMessage(const std::string& szMessage);
   const std::string& GetMessage() const;
private:
   std::string m_szMessage;
};

class aRegKeys
{
public:
   aRegKeys();
   void ClearKeys();
   int Size() const;
   const std::string& operator[](int num) const;
private:
   std::vector<std::string> m_vKeys;
   friend class aRegEntry;
};

class aRegValues
{
public:
   aRegValues();
   ~aRegValues();
   void ClearValues();
   int Size() const;
   const std::string& GetName(int num);
   DWORD GetType(int num) const;
   DWORD GetDataSize(int num) const;
   const unsigned char* GetValue(int num) const;
   std::string GetStringValue(int num) const;
private:
   std::vector<std::string> m_vNames;
   std::vector<unsigned char*> m_vValues;
   std::vector<DWORD> m_vDataSizes;
   std::vector<DWORD> m_vTypes;
   friend class aRegEntry;
};

#define REG_BUFF_SIZE 1024

#endif // _REGENTRY_