// Choose Record to edit or delete


#pragma once
#include "Resource.h"

class Book;
class Person;


// ChooseDlg dialog

class ChooseDlg : public CDialogEx {
CComboBox bookCtl;
CComboBox authorCtl;
CComboBox protagCtl;

  DECLARE_DYNAMIC(ChooseDlg)

public:
Book*   book{};
Person* person{};

           ChooseDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~ChooseDlg();

  virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ChooseRcd };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()

private:

  void* getDataPtr(CComboBox& cbx)
                             {int x = cbx.GetCurSel();   return x >= 0 ? cbx.GetItemDataPtr(x) : 0;
  }

public:

  afx_msg void onEditBook();
  afx_msg void onEditAuthor();
  afx_msg void onEditProtag();
  afx_msg void onDelBook();
  afx_msg void onDelAuthor();
  afx_msg void onDelProtag();
  };
