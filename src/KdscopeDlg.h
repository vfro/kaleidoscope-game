#ifndef KDSCOPE_DLG
#define KDSCOPE_DLG

#include "KDScene.h"
#include "Controls.h"

class aKdscopeDlg : public CDialog
{
public:

   aKdscopeDlg(CWnd* pParent = NULL);
   //{{AFX_DATA(aKdscopeDlg)
   enum { IDD = IDD_KDSCOPE_DIALOG };
   CButton btnStartStop;
   //}}AFX_DATA

   //{{AFX_VIRTUAL(aKdscopeDlg)
   protected:
      virtual void DoDataExchange(CDataExchange* pDX);
   //}}AFX_VIRTUAL

// Implementation
protected:
   HICON m_hIcon;
   aKDScene scene;
   aMainBackGround backGround;
   aButton act;
   aButton about;
   aButton hiScore;
   aButton loadSave;
   aSelector selector;
   bool status;

   //{{AFX_MSG(aKdscopeDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
   afx_msg UINT OnNcHitTest(CPoint point);
   afx_msg void OnStartStop();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   void startStop(bool loaded );
private:
   bool gameState;
};

#endif
