// Person ID to Ptr


#include "pch.h"
#include "PerIdP.h"
#include "Persons.h"

Person* PerIdP::operator() () {return person ? person : person = persons.find(id);}

