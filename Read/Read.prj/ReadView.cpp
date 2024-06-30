// ReadView.cpp : implementation of the ReadView class


#include "pch.h"
#include "ReadView.h"
#include "Read.h"
#include "ReadDoc.h"
#include "ClipLine.h"
#include "OptionsDlg.h"
#include "Resource.h"
#include "Resources.h"
#include "RptOrientDlgOne.h"


// ReadView

IMPLEMENT_DYNCREATE(ReadView, CScrView)

BEGIN_MESSAGE_MAP(ReadView, CScrView)
  ON_COMMAND(ID_Options,     &onOptions)
  ON_COMMAND(ID_Orientation, &onRptOrietn)

  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


ReadView::ReadView() noexcept {
ResourceData res;
String       pn;
  if (res.getProductName(pn)) prtNote.setTitle(pn);
  }


BOOL ReadView::PreCreateWindow(CREATESTRUCT& cs) {return CScrView::PreCreateWindow(cs);}


void ReadView::onOptions() {
OptionsDlg dlg;

  if (printer.name.isEmpty()) printer.load(0);

  if (dlg.DoModal() == IDOK) pMgr.setFontScale(printer.scale);
  }


void ReadView::onRptOrietn() {
RptOrietnDlg dlg;

  dlg.lbl00 = _T("Media:");

  dlg.ntpd = printer.toStg(prtNote.prtrOrietn);

  if (dlg.DoModal() == IDOK) {prtNote.prtrOrietn = printer.toOrient(dlg.ntpd);   saveNoteOrietn();}
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void ReadView::onBeginPrinting() {prtNote.onBeginPrinting(*this);}


void ReadView::onDisplayOutput() {dspNote.display(*this);}


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should
// not be a great hardship to construct a footer.

void ReadView::printFooter(DevBase& dev, int pageNo) {prtNote.prtFooter(dev, pageNo);}



void ReadView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) {

  CScrView::OnEndPrinting(pDC, pInfo);

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void ReadView::OnSetFocus(CWnd* pOldWnd) {

  CScrView::OnSetFocus(pOldWnd);

  switch(doc()->dataSrc()) {
    case NotePadSrc : break;
    case StoreSrc   : break;
    }
  }


void ReadView::OnLButtonDown(UINT nFlags, CPoint point)
                   {clipLine.set(point);   invalidate();   CScrView::OnLButtonDown(nFlags, point);}


void ReadView::OnLButtonDblClk(UINT nFlags, CPoint point) {
  clipLine.set(point);   RedrawWindow();   clipLine.load();

  CScrView::OnLButtonDblClk(nFlags, point);
  }


// ReadView diagnostics

#ifdef _DEBUG
void ReadView::AssertValid() const          {CScrollView::AssertValid();}
void ReadView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}
                                                                     // non-debug version is inline
ReadDoc* ReadView::GetDocument() const
            {ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ReadDoc))); return (ReadDoc*)m_pDocument;}
#endif //_DEBUG

