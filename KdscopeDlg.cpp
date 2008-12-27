#include "StdAfx.h"

#include "Kaleidoscope.h"
#include "KdscopeDlg.h"
#include "KDFormats.h"
#include "AboutDialog.h"
#include "HiScore.h"

aKdscopeDlg::aKdscopeDlg(CWnd* pParent /*=NULL*/)
   : CDialog(aKdscopeDlg::IDD, pParent),
     scene(),
     gameState(false),
     backGround(CPoint(0,0),
                CSize(KDFormats::sceneLeft +
                      KDFormats::sceneWidth +
                      KDFormats::sceneXBrink,
                      KDFormats::sceneTop +
                      KDFormats::sceneHeight +
                      KDFormats::sceneYBrink
                     )
               ),
     act(CPoint(KDFormats::sceneLeft,
                KDFormats::sceneTop + 
                KDFormats::sceneHeight + 
                KDFormats::sceneAct),
                CSize(KDFormats::sceneActWidth,
                      KDFormats::sceneActHeight
                     ),
                RGB(0, 200, 200),
                RGB(0, 0,   100),
                "&Start"
        ),
     about(CPoint(KDFormats::sceneLeft + 
                  KDFormats::sceneWidth - 
                  KDFormats::sceneActWidth,
                  KDFormats::sceneTop + 
                  KDFormats::sceneHeight + 
                  KDFormats::sceneAct),
                  CSize(KDFormats::sceneActWidth,
                        KDFormats::sceneActHeight
                     ),
                RGB(0, 200, 200),
                RGB(0, 0,   100),
                "About"
        ),
     hiScore(CPoint(KDFormats::sceneLeft + 
                  KDFormats::sceneWidth - 
                  2 * KDFormats::sceneActWidth - 15,
                  KDFormats::sceneTop + 
                  KDFormats::sceneHeight + 
                  KDFormats::sceneAct),
                  CSize(KDFormats::sceneActWidth,
                        KDFormats::sceneActHeight
                     ),
                RGB(0, 200, 200),
                RGB(0, 0,   100),
                "Scores"
            ),
     loadSave(CPoint(KDFormats::sceneLeft + 
            KDFormats::sceneWidth - 
            3 * KDFormats::sceneActWidth - 30,
            KDFormats::sceneTop + 
            KDFormats::sceneHeight + 
            KDFormats::sceneAct),
            CSize(KDFormats::sceneActWidth,
            KDFormats::sceneActHeight
            ),
            RGB(0, 200, 200),
            RGB(0, 0,   100),
            "Load"
            ),
     selector(CPoint(KDFormats::sceneLeft + KDFormats::sceneSelColor,
                KDFormats::sceneTop + 
                KDFormats::sceneHeight + 
                KDFormats::sceneAct),
                CSize(KDFormats::sceneSelColorWidth,
                      KDFormats::sceneSelColorHeight
                     ),
                "color set :"
             ),
     status(false)
{
   //{{AFX_DATA_INIT(aKdscopeDlg)
      // NOTE: the ClassWizard will add member initialization here
   //}}AFX_DATA_INIT
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void aKdscopeDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   //{{AFX_DATA_MAP(aKdscopeDlg)
   DDX_Control(pDX, IDC_STARTSTOP, btnStartStop);
   //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(aKdscopeDlg, CDialog)
   //{{AFX_MSG_MAP(aKdscopeDlg)
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_RBUTTONDOWN()
   ON_WM_RBUTTONDBLCLK()
   ON_WM_NCHITTEST()
   ON_BN_CLICKED(IDC_STARTSTOP, OnStartStop)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL aKdscopeDlg::OnInitDialog()
{
   using namespace KDFormats;
   const int dlgWidth =
         sceneLeft + KDFormats::sceneWidth + KDFormats::sceneXBrink;
   const int dlgHeight =
         sceneTop + KDFormats::sceneHeight + KDFormats::sceneYBrink;

   CDialog::OnInitDialog();
   SetWindowText("Kaleidoscope");

   CDC* dc; dc = GetDC();
//   CWindowDC dc(this);

   btnStartStop.SetFocus();

   backGround.SelectDC(*dc);

   // buttons
   act.SelectDC(*dc);
   selector.SelectDC(*dc);
   selector.AddSelection("easy");
   selector.AddSelection("medium");
   selector.AddSelection("hard");
   about.SelectDC(*dc);
   hiScore.SelectDC(*dc);
   loadSave.SelectDC(*dc);

   // scene
   scene.SelectDC(*dc);
   scene.GenerateObjects();

   SetIcon(m_hIcon, TRUE);
   SetIcon(m_hIcon, FALSE);

   CRect dlgRect;
   GetWindowRect(&dlgRect);
   SetWindowPos( &wndTop, dlgRect.TopLeft().x, dlgRect.TopLeft().y,
                 dlgWidth, dlgHeight, 0);

   CRgn name;
   CRgn min;
   CRgn close;
   CRgn scene;
   CRgn wind;
   CRgn nameAndMin;
   CRgn closeAndScene;

   name.CreateRectRgn(nameLeft, nameTop, nameLeft + nameWidth + 2, nameTop + nameHeight + 2);
   min.CreateRectRgn(minLeft, minTop, minLeft + minWidth + 1, minTop + minHeight + 1);
   close.CreateRectRgn(closeLeft, closeTop, closeLeft + closeWidth + 1, closeTop + closeHeight + 1);
   scene.CreateRectRgn(0, sceneBegin, dlgWidth, dlgHeight);
   wind.CreateRectRgn(0, 0, dlgRect.right, dlgRect.bottom);
   nameAndMin.CreateRectRgn(0, 0, dlgRect.right, dlgRect.bottom);
   closeAndScene.CreateRectRgn(0, 0, dlgRect.right, dlgRect.bottom);
   nameAndMin.CombineRgn(&name, &min, RGN_OR);
   closeAndScene.CombineRgn(&close, &scene, RGN_OR);
   wind.CombineRgn(&nameAndMin, &closeAndScene, RGN_OR);
   SetWindowRgn(wind, true);

   return TRUE;
}

void aKdscopeDlg::OnPaint() 
{
   CPaintDC dc(this);
   if (IsIconic())
   {
      SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;
      
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDC tempDevice;
      CBitmap tempBitmap;
      CBitmap* oldBitmap;
      CRect rect;
      GetWindowRect(&rect);
      tempDevice.CreateCompatibleDC(&dc);
      tempBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
      oldBitmap = tempDevice.SelectObject(&tempBitmap);
      //
      backGround.Draw(tempDevice);
      act.Draw(tempDevice);
      about.Draw(tempDevice);
      hiScore.Draw(tempDevice);
      loadSave.Draw(tempDevice);
      selector.Draw(tempDevice);
      scene.Draw(tempDevice);
      dc.BitBlt(0, 0, rect.Width(), rect.Height(), &tempDevice, 0, 0, SRCCOPY);
      //
      tempDevice.SelectObject(oldBitmap);
      //CDialog::OnPaint();
  }
}

HCURSOR aKdscopeDlg::OnQueryDragIcon()
{
   return (HCURSOR) m_hIcon;
}

void aKdscopeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
   scene.OnMoveCursor(nFlags,
      CPoint(point - CPoint(KDFormats::sceneLeft, KDFormats::sceneTop)));
   CDialog::OnMouseMove(nFlags, point);
   Invalidate( FALSE );
//   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aKdscopeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
   act.SetSelected(false);
   about.SetSelected(false);
   hiScore.SetSelected(false);
   loadSave.SetSelected(false);
   if (act.IsIn(point))
   {
      act.SetSelected();
      act.SetPressedStatus();
   }
   if (about.IsIn(point))
   {
      about.SetSelected();
      about.SetPressedStatus();
   }
   if (hiScore.IsIn(point))
   {
      hiScore.SetSelected();
      hiScore.SetPressedStatus();
   }
   if (loadSave.IsIn(point))
   {
      loadSave.SetSelected();
      loadSave.SetPressedStatus();
   }
   if ((selector.IsIn(point)) && (!status))
      selector.Push(point);
   scene.OnLeftButtonDown(nFlags, 
      CPoint(point - CPoint(KDFormats::sceneLeft, KDFormats::sceneTop)));
   CDialog::OnLButtonDown(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aKdscopeDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
   using namespace KDFormats;
   int checkScore;
   act.SetPressedStatus(false);
   about.SetPressedStatus(false);
   hiScore.SetPressedStatus(false);
   loadSave.SetPressedStatus(false);
   selector.UnPush();
   if (act.IsIn(point) && act.GetSelected())
   {
      startStop(false);
      return;
   }
   if (about.IsIn(point) && about.GetSelected())
   {
      aAboutDialog aboutDlg;
      aboutDlg.DoModal();
   }
   if (loadSave.IsIn(point) && loadSave.GetSelected())
   {
      if (scene.loadSave(this))
         startStop(true);
   }
   if (hiScore.IsIn(point) && hiScore.GetSelected())
   {
      aHiScore hiScoreDlg;
      hiScoreDlg.DoModal();
   }
   if ((selector.IsIn(point)) && (!status))
   {
      selector.Act(point);
   }
   if ( (point.x > closeLeft) && (point.x < closeLeft + closeWidth) &&
        (point.y > closeTop) && (point.y < closeTop + closeHeight)
      )
   {
      OnOK();
      return;
   }
   if ( (point.x > minLeft) && (point.x < minLeft + minWidth) &&
        (point.y > minTop) && (point.y < minTop + minHeight)
      )
   {
      ::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
   }
   scene.OnLeftButtonUp(nFlags, 
      CPoint(point - CPoint(KDFormats::sceneLeft, KDFormats::sceneTop)));
   CDialog::OnLButtonUp(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
   if (scene.WinStatus(checkScore) && (status))
   {
      status = false;
      aHiScore hiScore(getenv("USERNAME"),checkScore);
      hiScore.DoModal();
   }
}

void aKdscopeDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
   scene.OnRightButtonDown(nFlags, 
      CPoint(point - CPoint(KDFormats::sceneLeft, KDFormats::sceneTop)));
   CDialog::OnRButtonDown(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aKdscopeDlg::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
   scene.OnRightButtonDown(nFlags, 
      CPoint(point - CPoint(KDFormats::sceneLeft, KDFormats::sceneTop)));
   CDialog::OnRButtonDblClk(nFlags, point);
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

void aKdscopeDlg::OnStartStop()
{
   startStop(false);
}

void aKdscopeDlg::startStop(bool loaded)
{
   gameState = ! gameState;
   if (gameState)
   {
      act.SetText("&Stop");
      loadSave.SetText("Save");
      if (!loaded)
      {
         scene.StartGame(selector.GetSelected());
      }
      else
      {
         selector.SetSelected(scene.GetColorSet());
      }
      status = true;
      selector.Enable(false);
   } else
   {
      act.SetText("&Start");
      loadSave.SetText("Load");
      scene.StopGame();
      status = false;
      selector.Enable(true);
   }
   RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW);
}

UINT aKdscopeDlg::OnNcHitTest(CPoint point)
{
   CRect rect;
   GetWindowRect(&rect);
   using namespace KDFormats;
   if ( (point.x > rect.left + nameLeft) && (point.x < rect.left + nameLeft + nameWidth) &&
        (point.y > rect.top + nameTop) && (point.y < rect.top + nameTop + nameHeight)
      )
      return HTCAPTION;
   return CDialog::OnNcHitTest(point);
}


BOOL aKdscopeDlg::OnEraseBkgnd(CDC* /*pDC*/) 
{
   return false;
}
