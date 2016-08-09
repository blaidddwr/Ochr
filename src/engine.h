#ifndef GWERS_OCHR_ENGINE_H
#define GWERS_OCHR_ENGINE_H
#include <atomic>
#include <Effro/effro.h>
#include "atom.h"
namespace Gwers {
namespace Ochr {



class Engine
{
public:
   using Lfp = void (*)(void);
   using Eefp = void (*)(Effro::Exception&);
   using Sefp = void (*)(std::exception&);
   DECLARE(Offr::Engine)
   EXCEPTION(InvalidUse)
   EXCEPTION(ThreadsNotSet)
   EXCEPTION(PeriodNotSet)
   Engine() = delete;
   static void initialize();
   static int threads();
   static void threads(int);
   static int periodMS();
   static void periodMS(int);
   static void loop(Lfp);
   static void exceptionHandlers(Eefp,Sefp);
   static void finalize();
   static void run();
   static int id();
   static int wIn();
   static int wOut();
   static void exit();
private:
   enum class State {null,init,final,exec,done};
   static void add(Atom*);
   static void erase(int);
   static void thread(int);
   static State _state;
   static int _threads;
   static int _periodNS;
   static Lfp _loopFunc;
   static Eefp _gweExcFunc;
   static Sefp _stdExcFunc;
   thread_local static int _id;
   static std::atomic<int> _alive;
   static std::atomic<int> _cnt[2];
   static std::atomic<int> _lock;
   static int _fst;
   static std::atomic<unsigned int> _tick;
   static std::vector<std::vector<Atom*>> _atoms;
};



}
}
#endif
