// EditDlg dialog


#pragma once
#include "Persons.h"
#include "RegExpr.h"

class Book;



class EditDlg : public CDialogEx {

  DECLARE_DYNAMIC(EditDlg)

public:

enum       State {Author, Protag, Title};

State      targetTyp;
CButton    targetTypCtrl;
Cstring    targetID;
CEdit      tgtIDCtrl;
Cstring    target;
CEdit      targetCtrl;
Cstring    other;
CComboBox  otherCtrl;

enum       OtherState {NilOther, TargetOther, OtherOther};
OtherState otherState;
Person*    person;
Book*      book;

               EditDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual     ~EditDlg();

  // Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Edit };
#endif

private:

  virtual BOOL OnInitDialog();
          void load(String idStg, bool startEdit);
          void load(CComboBox& cbx, int id, TCchar* s);
          void perToTgt(PerTyp perTyp, RegExpr& re);
          void bookToTgt(RegExpr& re);
  virtual void OnOK();

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void onLeaveID();
  afx_msg void onLeaveTarget();
  afx_msg void OnClickedRadio1();
  afx_msg void OnKillfocusRadio1();
  afx_msg void OnRadio3();
  afx_msg void OnBnClickedRadio1();
  afx_msg void OnBnClickedRadio2();
  afx_msg void OnBnClickedRadio3();
  afx_msg void OnSelchangeOther();
  };

