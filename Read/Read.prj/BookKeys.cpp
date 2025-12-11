// Book Keys


#include "pch.h"
#include "BookKeys.h"
#include "Books.h"
#include "ABookIO.h"
#include "qsort.h"


bool BookKey::operator== (BookKey& b) {return idP()->key ==  b()->key;}
auto BookKey::operator<=>(BookKey& b) {return idP()->key <=> b()->key;}


BookKeys& BookKeys::operator+= (BookKeys& bk) {
BkKysIter iter(bk);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++)
    if (!find(bky->idP)) {BookKey bookKey = *bky;   data += bookKey;}

  return *this;
  }


void BookKeys::addRef(Book* book) {         // Bk Edit
BkKysIter iter(*this);
BookKey*  bky;
BookKey   bookKey;

  for (bky = iter(); bky; bky = iter++) if ((*bky)() == book) return;

  bookKey.idP.id = book->id;   data += bookKey;
  }


BookKey* BookKeys::find(int id) {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) if (bky->idP == id) return bky;

  return 0;
  }


void BookKeys::sort() {if (nData() > 1) {qsort(&data[0], &data[nData()-1]);}}


void BookKeys::removeRefs(int personID) {           // PerDel
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) {(*bky)()->removeRef(personID);   iter.remove();}
  }


bool BookKeys::removeRef(int bookID) {              // BkDel
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) if (bky->idP.id == bookID) {iter.remove();   break;}

  return isEmpty();
  }


bool BookKeys::removeDuplicates() {
bool dirty = false;

BkKysIter iter(*this);
BookKey*  bky;
BkKysIter iterJ(*this);
BookKey*  bkyJ;

  for (bky = iter(); bky; bky = iter++)
    for (iterJ = iter, bkyJ = iterJ++; bkyJ; bkyJ = iterJ++)
      if ((*bky)() == (*bkyJ)()) {iterJ.remove();   dirty = true;}

  return dirty;
  }


bool BookKeys::verifyRefs(int personID) {
BkKysIter iter(*this);
BookKey*  bky;
Book*     book;
bool      dirty = false;

  for (bky = iter(); bky; bky = iter++) {

    book = (*bky)();

    if (book) {
      if (book->perKeys.find(personID)) continue;
      }

    iter.remove();   dirty = true;
    }

  return dirty;
  }


void BookKeys::display(int id) {
BkKysIter iter(*this);
BookKey*  bky;
Book*     book;
bool      lineBrk = false;

  for (bky = iter(); bky; bky = iter++) {

    book = (*bky)();    if (!book) continue;

    if (lineBrk) notePad << nCrlf;   lineBrk = true;

    book->display(-6, dbVer);
    }
  }


void BookKeys::dbgDsp() {
BkKysIter iter(*this);
BookKey*  bky;
int       tb = 40;

  for (int i = 0; i < 5; i++) {notePad << nSetRTab(tb);   tb += 4;}

  for (bky = iter(); bky; bky = iter++) {
    notePad << nTab << bky->idP;
    }
  }


void  BookKeys::load(Archive& ar) {
ABookIO aBkIO(ar);
int     i;
int     n;

  clear();   /*this->dbVer = dbVer;*/   aBkIO.read(n);

  for (i = 0; i < n; i++) {BookKey bookKey;   aBkIO.read(bookKey.idP.id);   data += bookKey;}
  }


void BookKeys::store(Archive& ar) {
ABookIO   aBkIO(ar);
BkKysIter iter(*this);
BookKey*  bky;

  aBkIO.write(nData());

  for (bky = iter(); bky; bky = iter++) aBkIO.write(bky->idP);
  }



////////-----------------

#if 0
void BookKeys::clear() {
int i;

  for (i = 0; i < noRcds(); i++) rcds[i].clear();

  rcds.clear();
  }
#endif

#if 0
BookKey* BookKeys::find(  int bookID, String& title) {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) {
    if (bky->bookID == bookID && bky->title == title) return bky;
    }

  return 0;
  }
#endif
#if 0
void BookKeys::updateRefs() {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) {

    Book* book = books.find(bky->bookID);

    if (book) bky->title = book->title;
    else      iter.remove();
    }
  }
#endif
#if 0
int  i;
int  j;

  for (i = 0; i < noRcds()-1; i++) {
    BookKey& bookKey = rcds[i];

    for (j = i+1; j < noRcds(); j++) {
      BookKey& b = rcds[j];

      if (bookKey.bookID == b.bookID)
        {rcds.del(j); j--; dirty = true;}
      }
    }

#endif
#if 1
#else
      if (book->authorIdP.id  == personID) continue;
      if (book->author2IdP.id == personID) continue;
      if (book->protagIdP.id  == personID) continue;
      if (book->protag2IdP.id == personID) continue;
#endif
#if 0
void BookKeys::resolveIDs() {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) (*bky)();
  }
#endif
#if 0
void BookKeys::resolveIDs(Archive& ar) {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) (*bky)();
  }
#endif
#if 0
void BookKeys::delRef(Book* book) {
BkKysIter iter(*this);
BookKey*  bky;

  for (bky = iter(); bky; bky = iter++) if ((*bky)() == book) {iter.remove(); break;}
  }
#endif

