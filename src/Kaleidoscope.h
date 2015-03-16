#ifndef KDSCOPE_APP
#define KDSCOPE_APP

#include "resource.h"

class aKdscopeApp : public CWinApp
{
public:
   aKdscopeApp(){};
   //{{AFX_VIRTUAL(aKdscopeApp)
   public:
   virtual BOOL InitInstance();
   //}}AFX_VIRTUAL

   //{{AFX_MSG(aKdscopeApp)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

#endif