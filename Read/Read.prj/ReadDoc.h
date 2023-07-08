// ReadDoc.h : interface of the ReadDoc class


#pragma once
#include "ABookDlg.h"
#include "CDoc.h"
#include "MainFrame.h"
#include "match.h"
#include "Persons.h"

class Book;


enum DataSource {NotePadSrc, StoreSrc, FontSrc};


class ReadDoc : public CDoc {

PathDlgDsc  pathDlgDsc;
String      dbPath;

DataSource  dataSource;

bool        dirty;

protected: // create from serialization only

  ReadDoc() noexcept;
  DECLARE_DYNCREATE(ReadDoc)

public:

  virtual BOOL    OnNewDocument();

            void initialLoad();

       DataSource dataSrc() {return dataSource;}
          void    display(DataSource ds);

  virtual void serialize(Archive& ar);

// Implementation
public:
  virtual ~ReadDoc();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:

  void    loadDBfile(TCchar* path);

  void    editEntry(Person* per, Book* book);
  void    updateEntries(PerTypPos perTypPos, ABookDlg& dlg, Book*& book);

  void    updateBookKeys();
  void    delPerson(Person* person);
  void    delBook(Book* book);
  void    display(TCchar* prefix);
  bool    findFilePrefix(String& s, RegExpr*& re);

  void    writeDB( Archive& ar);
  void    readDB(  Archive& ar);
  void    readDB_1(Archive& ar);
  void    readDB_2(Archive& ar);

  void testLine(int n);
  void wholePage();

// Generated message map functions

protected:

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void onOpenDatabase();
  afx_msg void onAddRecord();
  afx_msg void onSearch();
  afx_msg void onEdit();
  afx_msg void onDelete();

  afx_msg void onDisplayAll();
  afx_msg void onDisplay_AB();
  afx_msg void onDisplay_CD();
  afx_msg void onDisplay_EF();
  afx_msg void onDisplay_GH();
  afx_msg void onDisplay_IJ();
  afx_msg void onDisplay_KL();
  afx_msg void onDisplay_MN();
  afx_msg void onDisplay_OP();
  afx_msg void onDisplay_QR();
  afx_msg void onDisplay_ST();
  afx_msg void onDisplay_UV();
  afx_msg void onDisplay_WX();
  afx_msg void onDisplay_YZ();


  afx_msg void onFixIt();
  afx_msg void OnFileSave();
  afx_msg void onEditCopy();
  };

