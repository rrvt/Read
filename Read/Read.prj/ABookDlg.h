#pragma once
#include "Persons.h"

// ABookDlg dialog

class ABookDlg : public CDialogEx {
  DECLARE_DYNAMIC(ABookDlg)

public:

CString title;


CString authorID;
CEdit   authorIDctrl;
CString author;
CEdit   authFNctrl;
CString authorLast;
CEdit   authLNctrl;

CString author2ID;
CEdit   author2IDctrl;
CString author2;
CEdit   auth2FNctrl;
CString author2Last;
CEdit   auth2LNctrl;

CString protagID;
CEdit   protagIDctrl;
CString protag;
CEdit   prtgFNctrl;
CString protagLast;
CEdit   prtgLNctrl;

CString protag2ID;
CEdit   protag2IDctrl;
CString protag2;
CEdit   prtg2FNctrl;
CString protag2Last;
CEdit   prtg2LNctrl;

CString location;
CString pubDate;
CString comment;


           ABookDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~ABookDlg();

  void load(PerTyp typ, CEdit& idCtrl, CEdit& fnCtrl, CEdit& lnCtrl);


// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Abook };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLeaveAuthorID();
  afx_msg void OnLeaveAurthor2ID();
  afx_msg void OnLeaveProtagID();
  afx_msg void OnLeaveProtag2ID();
  };
