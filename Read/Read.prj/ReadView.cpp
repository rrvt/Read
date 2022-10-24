// ReadView.cpp : implementation of the ReadView class


#include "stdafx.h"
#include "ReadView.h"
#include "Read.h"
#include "ReadDoc.h"
#include "ClipLine.h"
#include "Options.h"
#include "Resource.h"
#include "Resources.h"


// ReadView

IMPLEMENT_DYNCREATE(ReadView, CScrView)

BEGIN_MESSAGE_MAP(ReadView, CScrView)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONDBLCLK()

#if 0
  ON_WM_CONTEXTMENU()
  ON_COMMAND(ID_Pup0, &onCopy)
  ON_COMMAND(ID_Pup1, &onPup1)
  ON_COMMAND(ID_Pup2, &onPup2)
#endif

END_MESSAGE_MAP()


ReadView::ReadView() noexcept :
                                    dspNote( dMgr.getNotePad()), prtNote( pMgr.getNotePad()) {
ResourceData res;
String       pn;
  if (res.getProductName(pn)) prtNote.setTitle(pn);

#if 0
  sub.LoadMenu(ID_PopupMenu);
  menu.CreatePopupMenu();
  menu.AppendMenu(MF_POPUP, (UINT_PTR) sub.GetSafeHmenu(), _T(""));        //

  sub.Detach();
#endif
  }


BOOL ReadView::PreCreateWindow(CREATESTRUCT& cs) {

  return CScrView::PreCreateWindow(cs);
  }


void ReadView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) {
uint   x;
double topMgn   = options.topMargin.stod(x);
double leftMgn  = options.leftMargin.stod(x);
double rightMgn = options.rightMargin.stod(x);
double botMgn   = options.botMargin.stod(x);

  setMgns(leftMgn,  topMgn,  rightMgn, botMgn, pDC);   CScrView::OnPrepareDC(pDC, pInfo);
  }


// Perpare output (i.e. report) then start the output with the call to SCrView

void ReadView::onPrepareOutput(bool printing) {
DataSource ds = doc()->dataSrc();

  if (printing)
    switch(ds) {
      case NotePadSrc : prtNote.print(*this);  break;
      }

  else
    switch(ds) {
      case NotePadSrc : dspNote.display(*this);  break;
      }


  CScrView::onPrepareOutput(printing);
  }


void ReadView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) {

  switch(doc()->dataSrc()) {
    case NotePadSrc : setOrientation(options.orient); break;    // Setup separate Orientation?
    case StoreSrc   : setOrientation(options.orient); break;
    }
  setPrntrOrient(theApp.getDevMode(), pDC);   CScrView::OnBeginPrinting(pDC, pInfo);
  }


// The footer is injected into the printed output, so the output goes directly to the device.
// The output streaming functions are very similar to NotePad's streaming functions so it should not
// be a great hardship to construct a footer.

void ReadView::printFooter(Device& dev, int pageNo) {
  switch(doc()->dataSrc()) {
    case NotePadSrc : prtNote.footer(dev, pageNo);  break;
    }
  }



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


void ReadView::OnLButtonDblClk(UINT nFlags, CPoint point)
  {clipLine.set(point);   RedrawWindow();   clipLine.load();   CScrView::OnLButtonDblClk(nFlags, point);}


#if 0
void ReadView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) {
CRect  rect;
CMenu* popup;
CWnd*  pWndPopupOwner = this;

  if (point.x == -1 && point.y == -1)
            {GetClientRect(rect);  ClientToScreen(rect);  point = rect.TopLeft();  point.Offset(5, 5);}

  popup = menu.GetSubMenu(0);   if (!popup) return;

  while (pWndPopupOwner->GetStyle() & WS_CHILD) pWndPopupOwner = pWndPopupOwner->GetParent();

  popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
  }
#endif


#if 0

void ReadView::onCopy() {clipLine.load();  invalidate();}


void ReadView::onPup1() {  }


void ReadView::onPup2() {  }
#endif

// ReadView diagnostics

#ifdef _DEBUG

void ReadView::AssertValid() const {CScrollView::AssertValid();}

void ReadView::Dump(CDumpContext& dc) const {CScrollView::Dump(dc);}
                                             // non-debug version is inline
ReadDoc* ReadView::GetDocument() const
  {ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ReadDoc))); return (ReadDoc*)m_pDocument;}

#endif //_DEBUG




#if 1
#else
  sub.CreatePopupMenu();

  sub.AppendMenu(MF_STRING, ID_Pup0, _T("Copy Selection\tCtrl+C"));
  sub.AppendMenu(MF_STRING, ID_Pup1, _T("Popup 1\tCtrl+A"));
  sub.AppendMenu(MF_STRING, ID_Pup2, _T("Popup 2\tCtrl+B"));
#endif
#ifdef Examples
      case StoreSrc   : prtStore.print(*this); break;
#endif
#ifdef Examples
      case StoreSrc   : dspStore.display(*this); break;
#endif
#ifdef Examples
    case StoreSrc   : prtStore.footer(dev, pageNo); break;
#endif
                                    #ifdef Examples
                                      dspStore(dMgr.getNotePad()), prtStore(pMgr.getNotePad()),
                                    #endif

