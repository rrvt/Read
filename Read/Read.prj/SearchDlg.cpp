// SearchDlg.cpp : implementation file
//

#include "pch.h"
#include "SearchDlg.h"
#include "IniFileEx.h"
#include "resource.h"
#include "afxdialogex.h"


static TCchar* Section = _T("Search");
static TCchar* Key     = _T("State");
static TCchar* TgtKey  = _T("Target");


// SearchDlg dialog

IMPLEMENT_DYNAMIC(SearchDlg, CDialogEx)

SearchDlg::SearchDlg(CWnd* pParent) :
                               CDialogEx(IDD_Search, pParent), srchType(Author), target(_T("")) { }

SearchDlg::~SearchDlg() { }


BOOL SearchDlg::OnInitDialog() {

  srchType = (State) iniFile.readInt(Section, Key, 0);

  iniFile.readString(Section, TgtKey, target);

  CDialogEx::OnInitDialog();   return TRUE;
  }


void SearchDlg::DoDataExchange(CDataExchange* pDX) {
  CDialogEx::DoDataExchange(pDX);

  DDX_Radio(pDX, IDC_RADIO1, (int&) srchType);
  DDX_Text( pDX, IDC_Target,        target);
  }


BEGIN_MESSAGE_MAP(SearchDlg, CDialogEx)
END_MESSAGE_MAP()


void SearchDlg::OnOK() {

  CDialogEx::OnOK();

  iniFile.writeInt(   Section, Key,    srchType);
  iniFile.writeString(Section, TgtKey, target);
  }
