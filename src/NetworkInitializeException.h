#pragma once

#include <exception>
using namespace std;

struct NetworkInitializeException : public exception {
   const char * what () const throw () {
      return "Network initialization failed!";
   }
};