#include "stdafx.h"
#include "Kaleidoscope.h"
#include "AboutDialog.h"
#include "KDFormats.h"


aAboutDialog::aAboutDialog(CWnd* pParent /*=NULL*/)
   : CDialog(aAboutDialog::IDD, pParent),
     ok(CPoint(KDFormats::aboutOkLeft, KDFormats::aboutOkTop),
        CSize(KDFormats::sceneActWidth, KDFormats::sceneActHeight),
        RGB(0, 200, 200),
        RGB(0, 0,   100),
        "&Ok"
       )
{
}


void aAboutDialog::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(aAboutDialog)
   //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(aAboutDialog, CDialog)
   //{{AFX_MSG_MAP(aAboutDialog)
   ON_WM_PAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL aAboutDialog::OnInitDialog()
{

//   CPaintDC dc(this);
   CWindowDC dc(this);
   ok.SelectDC(dc);
   CRect rect;
   GetWindowRect(&rect);
   SetWindowPos(&wndTop ,
                KDFormats::sceneLeft + rect.left + KDFormats::aboutLeft,
                KDFormats::sceneTop + rect.top + KDFormats::aboutTop,
                KDFormats::aboutWidth, 
                KDFormats::aboutHeight, SWP_SHOWWINDOW
               );

   return TRUE;
}

void aAboutDialog::OnPaint() 
{
   using namespace KDFormats;
   CPaintDC dc(this);
   CDC tempDevice;
   CBitmap bitmap;
   CBitmap* oldBitmap;
   COLORREF color = RGB(0, 0, 200);
   COLORREF inColor = RGB(0, 0, 100);

   CPen pen(PS_SOLID, 2, color);
   CPen* oldPen;
   CBrush brush(inColor);
   CBrush* oldBrush;
   CFont font;
   CFont* oldFont;

   CString text;

   tempDevice.CreateCompatibleDC(&dc);
   bitmap.CreateCompatibleBitmap(&dc, aboutWidth, aboutHeight);
   oldBitmap = tempDevice.SelectObject(&bitmap);

   oldPen = tempDevice.SelectObject(&pen);
   oldBrush = tempDevice.SelectObject(&brush);

   tempDevice.Rectangle(CRect(2, 2, aboutWidth - 2, aboutHeight - 2));
   aKDDraw::DrawSceneHexagon(tempDevice, CPoint(aboutHexLeft, aboutHexTop),
                             aboutSize, RGB(0, 0, 0), RGB(0, 0, 100)
                            );
   aKDDraw::DrawHexagon(tempDevice, CPoint(aboutHexLeft, aboutHexTop),
                        aboutSize, RGB(0, 200, 200)
                       );

   font.CreateFont(16, 8, 0, 0, FW_BOLD, false, false,
                   0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
                   CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_DECORATIVE,"Times");

   oldFont = tempDevice.SelectObject(&font);
   tempDevice.SetBkMode(TRANSPARENT);
   tempDevice.SetTextColor(RGB(200, 200, 0));

   text.LoadString(IDS_ABOUT);
   tempDevice.DrawText(text, CRect(20, 20, 400, 400), DT_LEFT);

   ok.Draw(tempDevice);

   dc.BitBlt(0, 0, aboutWidth, aboutHeight, &tempDevice, 0, 0, SRCCOPY);
   tempDevice.SelectObject(oldBitmap);
   tempDevice.SelectObject(oldPen);
   tempDevice.SelectObject(oldBrush);
   tempDevice.SelectObject(oldFont);
}

void aAboutDialog::OnLButtonDown(UINT nFlags, CPoint point) 
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

void aAboutDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
   ok.SetPressedStatus(false);
   if (ok.IsIn(point) && ok.GetSelected())
   {
      CDialog::OnOK();
   }
   CDialog::OnLButtonUp(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

BOOL aAboutDialog::OnEraseBkgnd(CDC* /*pDC*/)
{
   return false;
}
