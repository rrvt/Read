// DeleteDlg dialog


#pragma once
#include "Persons.h"
#include "RegExpr.h"

class Book;


class DeleteDlg : public CDialogEx {

  DECLARE_DYNAMIC(DeleteDlg)

public:

enum      State {Author, Protag, Title};

State     targetTyp;
CString   tgtID;
CEdit     tgtIDCtrl;
CString   target;
CComboBox targetCtrl;

Book*   book;
Person* person;

           DeleteDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~DeleteDlg();

      void load(String idStg);
      void load(CComboBox& cbx, int id, TCchar* s);


#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Delete };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()


private:

  void perToTgt(PerTyp perTyp, RegExpr& re);
  void bookToTgt(RegExpr& re);

public:
  afx_msg void OnLeaveTgtID();
  afx_msg void OnLeaveTarget();
  afx_msg void OnSelchangeOther();
  afx_msg void OnBnClickedRadio1();
  afx_msg void OnBnClickedRadio2();
  afx_msg void OnBnClickedRadio3();
};
