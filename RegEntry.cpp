#include "StdAfx.h"
#include "RegEntry.h"
#include <iostream>

//
// aRegEntry class
//

aRegEntry::aRegEntry()
{
   m_hKey = NULL;
   m_szKeyName = "\\";
}

aRegEntry::~aRegEntry()
{
   if (m_hKey != NULL)
      RegCloseKey(m_hKey);
}

void aRegEntry::CloseKey()
{
   if (m_hKey != NULL)
      RegCloseKey(m_hKey);
   m_hKey = NULL;
}

const std::string& aRegEntry::GetKeyName() const
{
   return m_szKeyName;
}

void aRegEntry::OpenKey(HKEY hKey, const std::string& szSubKey)
{
   if (m_hKey != NULL)
      RegCloseKey(m_hKey);
   if (RegOpenKeyEx(hKey, szSubKey.c_str(), NULL, KEY_ALL_ACCESS, &m_hKey) 
       != ERROR_SUCCESS)
   {
      m_hKey = NULL;
      m_szKeyName = "\\";
      throw(aRegExcept("Cann't open key : " + szSubKey));
   }
   m_szKeyName = "\\" + szSubKey;
}

void aRegEntry::OpenSubKey(const std::string& szSubKey)
{
   HKEY hKey;
   if (RegOpenKeyEx(m_hKey, szSubKey.c_str(), NULL, KEY_ALL_ACCESS, &hKey) 
       != ERROR_SUCCESS)
   {
      throw(aRegExcept("Cann't open sub key : " + szSubKey));
   } else
   {
      RegCloseKey(m_hKey);
      m_hKey = hKey;
      m_szKeyName += "\\" + szSubKey;
   }
}

void aRegEntry::OpenSubKey(const aRegEntry& reg, const std::string& szSubKey)
{
   HKEY hKey;
   if (RegOpenKeyEx(reg.m_hKey, szSubKey.c_str(), NULL, KEY_ALL_ACCESS, &hKey) 
       != ERROR_SUCCESS)
   {
      throw(aRegExcept("Cann't open sub key : " + szSubKey));
   } else
   {
      RegCloseKey(m_hKey);
      m_hKey = hKey;
      m_szKeyName = reg.m_szKeyName + "\\" + szSubKey;
   }
}

void aRegEntry::CreateKey(const std::string& szKey, bool bPassage)
{
   DWORD dwDispos;
   HKEY hKey;
   if (RegCreateKeyEx(m_hKey, szKey.c_str(), NULL, NULL, NULL, NULL, NULL, &hKey, &dwDispos)
        != ERROR_SUCCESS)
      throw(aRegExcept("Cann't create key : " + m_szKeyName + "\\" + "szKey"));
      else
      {
         RegCloseKey(hKey);
         if (bPassage) 
         {
            RegOpenKeyEx(m_hKey, szKey.c_str(), NULL, KEY_ALL_ACCESS, &hKey);
            RegCloseKey(m_hKey);
            m_szKeyName += "\\" + szKey;
            m_hKey = hKey;
         }
      }
}

void aRegEntry::DeleteKey(const std::string& szKey)
{
   if (szKey == "")
   {
      DeleteThisKey();
      return;
   }
   if ((RegDeleteKey(m_hKey, szKey.c_str()) != ERROR_SUCCESS))
      throw (aRegExcept("Cann't delete key : " + m_szKeyName + "\\" + szKey));
}

void aRegEntry::RecursiveDeleteKey(const std::string& szKey)
{
   aRegEntry reg;
   aRegKeys rKeys;
   reg.OpenKey(m_hKey,szKey);
   reg.QueryKeys(rKeys);
   for (int i = 0; i < rKeys.Size(); i++)
      reg.RecursiveDeleteKey(rKeys[i]);
   reg.DeleteThisKey();
}

void aRegEntry::DeleteThisKey()
{
   if (RegDeleteKey(m_hKey,"") == ERROR_SUCCESS)
   {
      m_hKey = NULL;
      m_szKeyName = "\\";
      RegCloseKey(m_hKey);
   } else
      throw (aRegExcept("Cann't delete key : " + m_szKeyName));
}

void aRegEntry::CreateStringValue(const std::string& szName, const std::string& szValue) const
{
   if (RegSetValueEx(m_hKey, szName.c_str(), NULL, 
                     REG_EXPAND_SZ, (const unsigned char*)szValue.c_str(),
                     szValue.size()) != ERROR_SUCCESS)
      throw (aRegExcept("Cann't create value : " + szName + "=" + szValue));
}

void aRegEntry::CreateValue(const std::string& szName,
                            DWORD dwType,
                            DWORD dwSize,
                            const unsigned char* cValue)
{
   if (RegSetValueEx(m_hKey, szName.c_str(), NULL,
                     dwType, cValue,dwSize) != ERROR_SUCCESS)
      throw (aRegExcept("Cann't create value : " + szName));
}

const std::string aRegEntry::GetStringValue(const std::string& szName) const
{
   std::string szValue;
   DWORD dwType;
   DWORD dwSize = REG_BUFF_SIZE;
   unsigned char szBuffer[REG_BUFF_SIZE];
   if (RegQueryValueEx(m_hKey, szName.c_str(), NULL, &dwType, szBuffer, &dwSize)
       != ERROR_SUCCESS)
      throw (aRegExcept("Cann't read value : " + szName));
   if (dwSize == 0)
      szValue = "";
      else
         szValue = (const char*)szBuffer;
   return szValue;
}

void aRegEntry::GetValue(const std::string& szName, 
                         DWORD& dwType, 
                         DWORD& dwSize,
                         unsigned char* cBuffer) const
{
   if (RegQueryValueEx(m_hKey,szName.c_str(), NULL, &dwType, cBuffer, &dwSize) 
       != ERROR_SUCCESS)
      throw (aRegExcept("Cann't read value : " + szName));
}

void aRegEntry::QueryKeys(aRegKeys& rKeys) const
{
   char szBuffer[REG_BUFF_SIZE];
   DWORD dwSize = REG_BUFF_SIZE;
   int i = 0;
   rKeys.ClearKeys();
   while (RegEnumKeyEx(m_hKey, i, szBuffer, &dwSize, NULL, NULL, NULL, NULL) 
          == ERROR_SUCCESS)
   {
      rKeys.m_vKeys.push_back(szBuffer);
      i++;
      dwSize = REG_BUFF_SIZE;
   }
}

void aRegEntry::QueryValues(aRegValues& rValues) const
{
   unsigned char* cData = new unsigned char[REG_BUFF_SIZE];
   char szName[REG_BUFF_SIZE];
   DWORD dwNameSize = REG_BUFF_SIZE;
   DWORD dwDataSize = REG_BUFF_SIZE;
   DWORD dwType;
   rValues.ClearValues();
   int i = 0;
   while (RegEnumValue(m_hKey, i, szName, &dwNameSize, NULL,
                       &dwType, cData, &dwDataSize) == ERROR_SUCCESS)
   {
      rValues.m_vNames.push_back(szName);
      rValues.m_vTypes.push_back(dwType);
      rValues.m_vDataSizes.push_back(dwDataSize);
      cData[dwDataSize] = 0;
      rValues.m_vValues.push_back(cData);
      cData = new unsigned char[REG_BUFF_SIZE];
      dwNameSize = REG_BUFF_SIZE;
      dwDataSize = REG_BUFF_SIZE;
      i++;
   }
   delete[] cData;
}


//
// aRegExcept class
//

aRegExcept::aRegExcept(const std::string& szMessage)
{
   m_szMessage = szMessage;
}

void aRegExcept::SetMessage(const std::string& szMessage)
{
   m_szMessage = szMessage;
}

const std::string& aRegExcept::GetMessage() const
{
   return m_szMessage;
}

//
// aRegValues class
//

aRegKeys::aRegKeys()
{
   m_vKeys.clear();
}

void aRegKeys::ClearKeys()
{
   m_vKeys.clear();
}

const std::string& aRegKeys::operator[](int num) const
{
   if ( ((size_t)num <m_vKeys.size()) && (num>=0))
      return m_vKeys[num];
      else
         throw (aRegExcept("Number is out of range"));
}

int aRegKeys::Size() const
{
   return m_vKeys.size();
}

//
// aRegValues
//

aRegValues::aRegValues()
{
   ClearValues();
}

aRegValues::~aRegValues()
{
   ClearValues();
}

void aRegValues::ClearValues()
{
   m_vNames.clear();
   m_vTypes.clear();
   m_vDataSizes.clear();
   for (size_t i = 0; i < m_vValues.size(); i++)
      delete[] m_vValues[i];
   m_vValues.clear();
}

int aRegValues::Size() const
{
   return m_vNames.size();
}

const std::string& aRegValues::GetName(int num)
{
   return m_vNames[num];
}

DWORD aRegValues::GetType(int num) const
{
   return m_vTypes[num];
}

DWORD aRegValues::GetDataSize(int num) const
{
   return m_vDataSizes[num];
}

const unsigned char* aRegValues::GetValue(int num) const
{
   return m_vValues[num];
}

std::string aRegValues::GetStringValue(int num) const
{
   std::string szStr;
   szStr = (char*)m_vValues[num];
   return szStr;
}
