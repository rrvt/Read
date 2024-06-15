// Read.h : main header file for the Read application

#pragma once
#include "CApp.h"
#include "MainFrame.h"


class ReadDoc;
class ReadView;


// Read:
// See Read.cpp for the implementation of this class

class Read : public CApp {

public:
               Read() noexcept : CApp(this) { }

  virtual BOOL InitInstance();
  virtual int  ExitInstance();

          ReadDoc*  doc()  {return (ReadDoc*)  CApp::getDoc();}
          ReadView* view() {return (ReadView*) CApp::getView();}
          MainFrame*       mainFrm() {return (MainFrame*) m_pMainWnd;}

  DECLARE_MESSAGE_MAP()

  afx_msg void OnAppAbout();
  afx_msg void OnHelp();
  };


extern Read theApp;

inline void       invalidate() {theApp.invalidate();}
inline ReadDoc*   doc()        {return theApp.doc();}
inline ReadView*  view()       {return theApp.view();}
inline MainFrame* mainFrm()    {return theApp.mainFrm();}
inline MyToolBar& getToolBar() {return mainFrm()->getToolBar();}

