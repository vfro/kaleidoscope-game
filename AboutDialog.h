#ifndef ABOUT_BOX_INCLUDED
#define ABOUT_BOX_INCLUDED

#include "Controls.h"

class aAboutDialog : public CDialog
{
public:
   aAboutDialog(CWnd* pParent = NULL);

   //{{AFX_DATA(aAboutDialog)
   enum { IDD = IDD_ABOUTBOX };
   //}}AFX_DATA

protected:
   //{{AFX_VIRTUAL(aAboutDialog)
   virtual BOOL OnInitDialog();
   virtual void DoDataExchange(CDataExchange* pDX);
   //}}AFX_VIRTUAL

   aButton ok;

   //{{AFX_MSG(aAboutDialog)
   afx_msg void OnPaint();
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#endif
