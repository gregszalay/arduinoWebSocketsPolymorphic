#pragma once

#include <exception>
using namespace std;

struct NetworkInitializeException : public exception {
   char * what () const throw () {
      return "Network initialization failed!";
   }
};