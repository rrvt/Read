// Book ID to Ptr

#pragma once

class Book;


class BkIdP {
Book* book{};

public:
int     id{};

  BkIdP() {}
  BkIdP(BkIdP& idP) {*this = idP;}

  void clear() {id = 0; book = 0;}

  operator int () {return id;}

  Book* operator() ();    // {return book ? book : book = books.find(id);}

  BkIdP& operator= (BkIdP& idP) {book = idP.book;   id = idP.id;   return *this;}
  };

