#include "StdAfx.h"
#include "Kaleidoscope.h"
#include "KdscopeDlg.h"

BEGIN_MESSAGE_MAP(aKdscopeApp, CWinApp)
   //{{AFX_MSG_MAP(aKdscopeApp)
   //}}AFX_MSG
   ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

aKdscopeApp theApp;

BOOL aKdscopeApp::InitInstance()
{
   aKdscopeDlg dlg;
   m_pMainWnd = &dlg;
   int nResponse = dlg.DoModal();
   if (nResponse == IDOK)
   {
   }
   else if (nResponse == IDCANCEL)
   {
   }
   // Since the dialog has been closed, return FALSE so that we exit the
   //  application, rather than start the application's message pump.
   return FALSE;
}
