// ReadView.h : interface of the ReadView class


#pragma once
#include "CScrView.h"
#include "ReportNtPd.h"


class ReadDoc;


class ReadView : public CScrView {
protected:

  ReadView() noexcept;

  DECLARE_DYNCREATE(ReadView)

public:

  virtual ~ReadView() { }

  virtual void       initNoteOrietn() { }
  virtual void       saveNoteOrietn() { }
  virtual void       initRptOrietn()  { }
  virtual void       saveRptOrietn()  { }
  virtual PrtrOrient getOrientation() {return prtNote.prtrOrietn;}

  virtual BOOL       PreCreateWindow(CREATESTRUCT& cs);

  virtual void       onDisplayOutput();

  virtual void       onPreparePrinting(CPrintInfo* info) {prtNote.onPreparePrinting(info);}
  virtual void       onBeginPrinting();
  virtual void       printFooter(DevStream& dev, int pageNo);
  virtual void       OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

  ReadDoc*           GetDocument() const;

public:

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

public:

  DECLARE_MESSAGE_MAP()

  afx_msg void onOptions();
  afx_msg void onRptOrietn();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnLButtonDown(  UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  };

#ifndef _DEBUG  // debug version in ReadView.cpp
inline ReadDoc* ReadView::GetDocument() const {return reinterpret_cast<ReadDoc*>(m_pDocument);}
#endif

