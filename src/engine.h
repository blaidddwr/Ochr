#ifndef GWERS_OCHR_ENGINE_H
#define GWERS_OCHR_ENGINE_H
#include <exception.h>
namespace Ochr {


//FOR NOW
class Unit;


namespace Engine
{
   DECLARE(Offr::Engine)
   EXCEPTION(InvalidUse)
   EXCEPTION(ThreadsNotSet)
   EXCEPTION(PeriodNotSet)
   void initialize();
   int threads();
   void threads(int);
   int period();
   void period(int);
   void finalize();
   void run();
   void add(Unit*);
   void erase(int);
   int id();
   int which();
   void exit();
}



}
#endif
