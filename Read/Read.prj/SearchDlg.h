#pragma once


// SearchDlg dialog

class SearchDlg : public CDialogEx {

  DECLARE_DYNAMIC(SearchDlg)

public:

enum State {Author, Protag, Title};

State   srchType;
Cstring target;

  SearchDlg(CWnd* pParent = nullptr);   // standard constructor
  virtual ~SearchDlg();

  virtual BOOL OnInitDialog();
  virtual void OnOK();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_Search };
#endif

protected:

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  DECLARE_MESSAGE_MAP()
  };
