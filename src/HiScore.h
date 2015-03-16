#ifndef HI_SCORE_INCLUDED
#define HI_SCORE_INCLUDED

#include "Controls.h"

class aHiScore : public CDialog
{
public:
   aHiScore(CString name = "", int value = 0, CWnd* pParent = NULL);
   virtual ~aHiScore();

   //{{AFX_DATA(aHiScore)
   enum { IDD = IDD_HISCORE};
   //}}AFX_DATA

protected:
   typedef std::vector<aButton*> aButtonVector;
   typedef std::vector<CString> aStringVector;
   //{{AFX_VIRTUAL(aHiScore)
   virtual BOOL OnInitDialog();
   virtual void DoDataExchange(CDataExchange* pDX);
   //}}AFX_VIRTUAL

   aButton ok;
   aButton* editName;
   aButton* editValue;
   bool editable;
   CString szName;
   int iValue;
   int edit;
   void ReadRegistry();
   void WriteRegistry();

   //{{AFX_MSG(aHiScore)
   afx_msg void OnPaint();
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
   afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
   afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
   aButtonVector names;
   aStringVector szNames;
   aButtonVector values;
   aStringVector szValues;
   DECLARE_MESSAGE_MAP()
};

#endif
