// Person ID to Ptr


#pragma once

class Person;


class PerIdP {
Person* person{};

public:
int     id{};

  PerIdP() { }
  PerIdP(PerIdP& idP) {*this = idP;}

  void clear() {id = 0; person = 0;}

  void set(int id) {person = 0; this->id = id;}         // Bk Edit

  operator int ()     {return id;}

  Person* operator() ();   // {return person ? person : person = persons.find(id);}

  PerIdP& operator= (PerIdP& idP) {person = idP.person;   id = idP.id;   return *this;}
  };

