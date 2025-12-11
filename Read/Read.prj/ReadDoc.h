// ReadDoc.h : interface of the ReadDoc class


#pragma once
#include "BookDlg.h"
#include "CDoc.h"
#include "MainFrame.h"
#include "PathDlgDsc.h"
#include "RegExpr.h"

class Book;
class Persons;


enum DataSource {NotePadSrc, StoreSrc, FontSrc};



class ReadDoc : public CDoc {

PathDlgDsc  pathDlgDsc{_T("Book Database"), _T("BookDB.bdb"), _T("bdb"), _T("*.bdb")};
String      dbPath{};
int         dbVer{};

DataSource  dataSource{NotePadSrc};

DspOptions  dspOption{NilDsp};              // Current Display option
String      dspPrefix;                      // Current Display Prefix for Person Display

bool        dirty{false};

protected:                                  // create from serialization only

  ReadDoc() noexcept;
  DECLARE_DYNCREATE(ReadDoc)

public:
      virtual ~ReadDoc();

          void initialLoad();

    DataSource dataSrc() {return dataSource;}
       void    display(DataSource ds = NotePadSrc);

       void    saveData();

  virtual void serialize(Archive& ar);

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

private:

  bool    getDBpath();
  void    loadDBfile(TCchar* path);
  bool    verifyRefs();                             // LoadDB

  bool    findFilePrefix(String& s, RegExpr*& re);

  void    writeDB( Archive& ar);
  void    readDB(  Archive& ar);

  void    dspDirty();

  void    testLine(int n);
  void    wholePage();

  void    displayData();
  void    displayCur();
  void    displayBk(DspOptions opt);
  void    display(TCchar* prefix);
  void    displayDB();

  bool    dspModify(TCchar* prefix, Book*   book);
  bool    dspModify(TCchar* prefix, Person* person);

  void    setVersion(int ver);

protected:

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void onCreateDB();
  afx_msg void onOpenDatabase();
  afx_msg void onAddRecord();
  afx_msg void onSearch();
  afx_msg void onModifyRcd();

  afx_msg void onSortTitle();
  afx_msg void onSortDate();
  afx_msg void onSortAuthor();
  afx_msg void onDebugDsp();

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



/////-------------------

//void    editEntry(Person* per, Book* book);
//  void    delPerson(Person* person);
//  void    delBook(Book* book);
//  afx_msg void onDelete();
//void    updateEntries(PerTypPos perTypPos, BookDlg& dlg, Book*& book);

