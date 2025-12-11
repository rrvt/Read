// Book Dialog -- Adding and Editing a book entry


#pragma once
#include "Persons.h"


// BookDlg dialog

class BookDlg : public CDialogEx {
CComboBox     author1Ctl{};
CComboBox     author2Ctl{};
CComboBox     protag1Ctl{};
CComboBox     protag2Ctl{};

  DECLARE_DYNAMIC(BookDlg)

public:
Cstring       title;
Person*       author1{};
Person*       author2{};
Person*       protag1{};
Person*       protag2{};
Cstring       location{};
CString       comment{};
COleDateTime  pubDate{};
COleDateTime  readDate{COleDateTime::GetCurrentTime()};

CDateTimeCtrl pubDateCtl{};                     // Controls for managing date/times
CDateTimeCtrl readDateCtl{};

           BookDlg(CWnd* pParent = nullptr);    // standard constructor
  virtual ~BookDlg();

  virtual BOOL OnInitDialog();
  virtual void OnOK();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Book };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void  loadCbx(CComboBox& cbx, PerTyp pTyp, Person* per = 0);
  int   addCbx( CComboBox& cbx, Person* per);
  void* getDataPtr(CComboBox& cbx);

  bool  checkDate(CDateTimeCtrl& ctl, COleDateTime& oleDate);

public:

  afx_msg void onNewAuth1();
  afx_msg void onNewAuth2();
  afx_msg void onNewProtag1();
  afx_msg void onNewProtag2();
  afx_msg void onChangeAuthor1();
  afx_msg void onChangeAuthor2();
  afx_msg void onChangeProtag1();
  afx_msg void onChangeProtag2();
  afx_msg void onLeavePubDate(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void onLeaveReadDate(NMHDR* pNMHDR, LRESULT* pResult);
  };
