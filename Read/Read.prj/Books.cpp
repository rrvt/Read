// Books Description


#include "pch.h"
#include "Books.h"
#include "ABookIO.h"
#include "NotePad.h"
#include "Persons.h"
#include "qsort.h"
#include "RegExpr.h"


void Books::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();

  rcds.clear(); nextIndex = 1;
  }


Book& Books::add(TCchar* title) {
Book& r = rcds[noRcds()];

  r.title = title; r.add(nextIndex);  return r;
  }


Book* Books::Books::find(TCchar* title) {
int i;

  for (i = 0; i < noRcds(); i++) {Book& r = rcds[i];   if (r.title == title) return &r;}

  return 0;
  }


Book* Books::find(int bookID) {
int i;

  for (i = 0; i < noRcds(); i++) {Book& r = rcds[i];   if (r.id == bookID) return &r;}

  return 0;
  }


void  Books::display(CString tgt) {
String  t = _T(".*") +  tgt + _T(".*");
RegExpr re(t);
int     i;
String  s;
bool    lineBrk = false;


  for (i = 0; i < noRcds(); i++) {
    Book& book = rcds[i];

    if (re.match(book.title)) {

      if (lineBrk) notePad << nCrlf;   lineBrk = true;

      book.display(0, false);
      }
    }
  }



void Books::del(Book& book) {
int i;
int n = noRcds();
int bookID = book.id;

  book.removePersonRefs();

  for (i = 0; i < n; i++) if (rcds[i].id == bookID) {rcds.del(i); return;}

  }



void Books::removeRef(int bookID, int personID) {
int i;
int n = noRcds();

  for (i = 0; i < n; i++)
                  {Book& book = rcds[i];   if (book.id == bookID) {book.removeRef(personID); return;}}
  }



void Books::load(Archive& ar) {
int n;
int i;
int x;

  clear();   ar.readBin(n);    if (!n) return;

  for (i = 0; i < n; i++) {
    x = rcds[noRcds()].load(ar);
    if (x > nextIndex) nextIndex = x;
    }

  nextIndex++;
  }


void Books::store(Archive& ar) {
int     i;

  ar.wrtBin(noRcds());

  for (i = 0; i < noRcds(); i++) {
    rcds[i].store(ar);
    }
  }



int Book::load(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.read(id);
  aBkIO.read(title);
  aBkIO.read(authorID);
  aBkIO.read(author2ID);
  aBkIO.read(protagID);
  aBkIO.read(protag2ID);
  aBkIO.read(location);
  aBkIO.read(pubDate);
  aBkIO.read(comment);

  return id;
  }



void Book::store(Archive& ar) {
ABookIO aBkIO(ar);

  aBkIO.write(id);
  aBkIO.write(title);
  aBkIO.write(authorID);
  aBkIO.write(author2ID);
  aBkIO.write(protagID);
  aBkIO.write(protag2ID);
  aBkIO.write(location);
  aBkIO.write(pubDate);
  aBkIO.write(comment);
  }



void Book::display(int personID, bool subOrd) {

  if (!subOrd) notePad << nClrTabs << nSetRTab(3) << nSetTab(5) << nSetTab(12);

  notePad << nTab << id << nTab << _T("Title:     ") << nTab << title << nCrlf;

  if (!subOrd) notePad << nClrTabs << nSetTab(7);

  if (personID != authorID  && persons.display(authorID))  notePad << nCrlf;
  if (personID != author2ID && persons.display(author2ID)) notePad << nCrlf;
  if (personID != protagID  && persons.display(protagID))  notePad << nCrlf;
  if (personID != protag2ID && persons.display(protag2ID)) notePad << nCrlf;

  if (!location.isEmpty()) notePad << nTab << _T("Loc:      ") << nTab << location << nCrlf;
  if (!pubDate.isEmpty())  notePad << nTab << _T("PubDate:") << nTab << pubDate  << nCrlf;
  if (!comment.isEmpty())  notePad << nTab << _T("Comment:") << nTab << comment  << nCrlf;
  }


void Book::verifyRefs() {
  if (!verifyRef(AuthorTyp, authorID )) {authorID  = 0;}
  if (!verifyRef(AuthorTyp, author2ID)) {author2ID = 0;}
  if (!verifyRef(ProtagTyp, protagID )) {protagID  = 0;}
  if (!verifyRef(ProtagTyp, protag2ID)) {protag2ID = 0;}
  }


bool Book::verifyRef(PerTyp perTyp, int personID)
                                              {return !personID || persons.find(perTyp, personID) != 0;}



void Book::removeRef(int personID) {
  if (personID == authorID)  {authorID  = 0; return;}
  if (personID == author2ID) {author2ID = 0; return;}
  if (personID == protagID)  {protagID  = 0; return;}
  if (personID == protag2ID) {protag2ID = 0; return;}
  }


void Book::removePersonRefs() {
  persons.removeBookRef(authorID,  id);
  persons.removeBookRef(author2ID, id);
  persons.removeBookRef(protagID,  id);
  persons.removeBookRef(protag2ID, id);
  }



void Book::clear() {protagID = 0; protag2ID = 0; authorID = 0; author2ID = 0;
                                             location.clear();  title.clear();   comment.clear();}


void Book::copy(Book& b) {rcdCopy(b);
                          protagID  = b.protagID;
                          protag2ID = b.protag2ID;
                          authorID  = b.authorID;
                          author2ID = b.author2ID;
                          location  = b.location;
                          title     = b.title;
                          pubDate   = b.pubDate;
                          comment   = b.comment;
                          }

