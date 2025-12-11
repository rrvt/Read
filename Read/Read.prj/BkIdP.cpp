// Book ID to Ptr


#include "pch.h"
#include "BkIdP.h"
#include "Books.h"


Book* BkIdP::operator() () {return book ? book : book = books.find(id);}

