// ReadView.h : interface of the ReadView class


#pragma once
#include "CScrView.h"
#include "NotePadRpt.h"
#ifdef Examples
#include "StoreRpt.h"
#endif


class ReadDoc;


class ReadView : public CScrView {

NotePadRpt dspNote;
NotePadRpt prtNote;
#ifdef Examples
StoreRpt   dspStore;
StoreRpt   prtStore;

CMenu      menu;
CMenu      sub;
#endif

protected: // create from serialization only

  ReadView() noexcept;

  DECLARE_DYNCREATE(ReadView)

public:

  virtual ~ReadView() { }

  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void onPrepareOutput(bool printing);

  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void printFooter(Device& dev, int pageNo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  ReadDoc* GetDocument() const;

public:

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:

  DECLARE_MESSAGE_MAP()

  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnLButtonDown(  UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

#if 0
  afx_msg void OnContextMenu(  CWnd* ,      CPoint point);
  afx_msg void onCopy();
  afx_msg void onPup1();
  afx_msg void onPup2();
#endif
  };

#ifndef _DEBUG  // debug version in ReadView.cpp
inline ReadDoc* ReadView::GetDocument() const
   { return reinterpret_cast<ReadDoc*>(m_pDocument); }
#endif

