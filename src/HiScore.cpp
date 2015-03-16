#include "stdafx.h"
#include "Kaleidoscope.h"
#include "HiScore.h"
#include "KDFormats.h"
#include "RegEntry.h"


aHiScore::aHiScore(CString name, int value,CWnd* pParent /*=NULL*/)
   : CDialog(aHiScore::IDD, pParent),
     ok(CPoint(KDFormats::hiOkLeft, KDFormats::hiOkTop),
        CSize(KDFormats::sceneActScore, KDFormats::sceneActHeight),
        RGB(0, 200, 200),
        RGB(0, 0,   100),
        "&Ok"
       ),
     edit(0)
{
   editable = (value != 0);
   szName = name;
   iValue = value;
}

aHiScore::~aHiScore()
{
   int i;
   if (editable)
   {
      szNames[edit] = szName;
      WriteRegistry();
   }

   for (i = 0; i < 6; i++)
   {
      delete names[i];
      delete values[i];
   }
   names.clear();
   values.clear();
}

void aHiScore::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(aHiScore)
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(aHiScore, CDialog)
   //{{AFX_MSG_MAP(aHiScore)
   ON_WM_PAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL aHiScore::OnInitDialog()
{
   using namespace KDFormats;
   int i;
//   CPaintDC dc(this);
   CWindowDC dc(this);
   ok.SelectDC(dc);
   CRect rect;
   GetWindowRect(&rect);
   SetWindowPos(&wndTop ,
                sceneLeft + rect.left + hiLeft,
                sceneTop + rect.top + hiTop,
                hiWidth,
                hiHeight, SWP_SHOWWINDOW
               );
   COLORREF color;
   COLORREF bordColor = RGB(250, 0, 250);
   COLORREF inColor = RGB(100, 0, 100);

   names.clear();
   values.clear();
   szNames.clear();
   szValues.clear();
   ReadRegistry();
   edit = 0;
   if (editable)
   {
      CString szStr;
      while (( iValue < atoi(szValues[edit])) && (edit < 5)) edit++;
      szStr.Format("%i",iValue);
      szValues.insert(&szValues[edit],szStr);
      szNames.insert(&szNames[edit],szName);
   }

   for (i = 0; i < 6; i++)
   {
      if (i == edit)
      {
         color = (editable) ? RGB(30, 30, 30) : inColor;
      }
      else
         color = inColor;
      aButton* name = new aButton(CPoint(hiButtonLeft,
                                         hiButtonTop + i * (sceneActHeight + 9)
                                        ),
                                  CSize(sceneActScore + 20, sceneActHeight),
                                  bordColor, color,
                                  szNames[i], false
                                 );
      name->SelectDC(dc);
      names.push_back(name);
      aButton* value = new aButton(CPoint(hiButtonLeft + sceneActScore + 50,
                                          hiButtonTop + i * (sceneActHeight + 9)
                                         ),
                                   CSize(sceneActScore - 20, sceneActHeight),
                                   bordColor, inColor,
                                   szValues[i], false
                                  );
      value->SelectDC(dc);
      values.push_back(value);
      if ((i == edit) && (editable))
      {
         editName = name;
         editValue = value;
      }
   }
   if (editable)
   {
      editName->SetText(szName + "_");
   }

   return TRUE;
}

void aHiScore::OnPaint() 
{
   using namespace KDFormats;
   int i = 0;
   CPaintDC dc(this);
   CDC tempDevice;
   CBitmap bitmap;
   CBitmap* oldBitmap;
   COLORREF color = RGB(0, 0, 200);
   COLORREF inColor = RGB(0, 0, 100);

   CPen pen(PS_SOLID, 2, color);
   CPen linePen(PS_SOLID, 2, RGB(170, 0, 0));
   CPen* oldPen;
   CBrush brush(inColor);
   CBrush* oldBrush;
   CFont font;
   CFont* oldFont;

   CString text;

   tempDevice.CreateCompatibleDC(&dc);
   bitmap.CreateCompatibleBitmap(&dc, hiWidth, hiHeight);
   oldBitmap = tempDevice.SelectObject(&bitmap);

   oldPen = tempDevice.SelectObject(&pen);
   oldBrush = tempDevice.SelectObject(&brush);

   tempDevice.Rectangle(CRect(2, 2, hiWidth - 2, hiHeight - 2));
   tempDevice.SelectObject(&linePen);
   tempDevice.MoveTo(10, hiButtonTop + 5 * (sceneActHeight + 9) - 5);
   tempDevice.LineTo(hiWidth - 10, hiButtonTop + 5 * (sceneActHeight + 9) - 5);

   font.CreateFont(16, 8, 0, 0, FW_BOLD, false, false,
                   0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                   CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DECORATIVE,"Times");

   oldFont = tempDevice.SelectObject(&font);
   tempDevice.SetBkMode(TRANSPARENT);
   tempDevice.SetTextColor(RGB(200, 200, 0));

   text.LoadString(IDS_HISCORE);
   tempDevice.DrawText(text, CRect(90, 10, 400, 400), DT_LEFT);

   ok.Draw(tempDevice);

   for ( i = 0; i < 6; i++)
   {
      names[i]->Draw(tempDevice);
      values[i]->Draw(tempDevice);
   }

   dc.BitBlt(0, 0, hiWidth, hiHeight, &tempDevice, 0, 0, SRCCOPY);
   tempDevice.SelectObject(oldBitmap);
   tempDevice.SelectObject(oldPen);
   tempDevice.SelectObject(oldBrush);
   tempDevice.SelectObject(oldFont);
}

void aHiScore::OnLButtonDown(UINT nFlags, CPoint point) 
{
   ok.SetPressedStatus(false);
   ok.SetSelected(false);
   if (ok.IsIn(point))
   {
      ok.SetPressedStatus(true);
      ok.SetSelected(true);
   }
   CDialog::OnLButtonDown(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aHiScore::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ok.SetPressedStatus(false);
   if (ok.IsIn(point) && ok.GetSelected())
   {
      CDialog::OnOK();
   }
   CDialog::OnLButtonUp(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aHiScore::ReadRegistry()
{
   aRegEntry reg;
   int i;
   try
   {
      reg.OpenKey(HKEY_LOCAL_MACHINE, "SoftWare\\Kaleidoscope");
   }
   catch(...)
   {
      try
      {
         reg.OpenKey(HKEY_LOCAL_MACHINE,"SoftWare");
         reg.CreateKey("Kaleidoscope");
      }
      catch(...)
      {
      }
      reg.CloseKey();
   }
   try
   {
      for( i = 0; i < 6; i++)
      {
         CString str;
         str.Format("Slot%i",i  + 1);
         str = "SoftWare\\Kaleidoscope\\" + str;
         aRegEntry subKey;
         subKey.OpenKey(HKEY_LOCAL_MACHINE,std::string(str));
         szNames.push_back(subKey.GetStringValue("name").c_str());
         szValues.push_back(subKey.GetStringValue("value").c_str());
         subKey.CloseKey();
      }
   }
   catch(aRegExcept a)
   {
      while( szNames.size() != 6)
      {
         szNames.push_back("---");
         szValues.push_back("0");
      }
   }
}

void aHiScore::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   const int maxLenght = 7;
   const int backSpace = 8;

   if ((editable) && (isalnum(nChar) || nChar == UINT(' ') || nChar == UINT('_')) && (szName.GetLength() < maxLenght))
   {
      szName += (GetKeyState(VK_SHIFT) < 0) ? (char)toupper((char)nChar) : (char)tolower((char)nChar);
      editName->SetText(szName + "_");
   }
   if ((editable) && (nChar == backSpace))
   {
      szName.Delete(szName.GetLength() - 1);
      editName->SetText(szName + "_");
   }

   CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aHiScore::WriteRegistry()
{
   aRegEntry reg;
   try
   {
      reg.OpenKey(HKEY_LOCAL_MACHINE,"SoftWare");
      try
      {
         reg.RecursiveDeleteKey("Kaleidoscope");
      }
      catch(...)
      {
      }
      reg.CreateKey("Kaleidoscope");
      reg.CloseKey();
      for (int i = 0; i < 6; i++)
      {
         std::string slodName = (const char*)szNames[i];
         std::string slodValue = (const char*)szValues[i];
         aRegEntry subKey;
         CString str;
         str.Format("Slot%i",i  + 1);
         subKey.OpenKey(HKEY_LOCAL_MACHINE,"SoftWare\\Kaleidoscope");
         subKey.CreateKey((const char*)str,true);
         subKey.CreateStringValue("name", slodName.c_str());
         subKey.CreateStringValue("value", slodValue.c_str());
      }
   }
   catch(...)
   {
   }
}

BOOL aHiScore::OnEraseBkgnd(CDC* /*pDC*/)
{
   return false;
}
