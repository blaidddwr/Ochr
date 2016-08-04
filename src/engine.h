#ifndef GWERS_OCHR_ENGINE_H
#define GWERS_OCHR_ENGINE_H
#include <atomic>
#include <effro/effro.h>
namespace Gwers {
namespace Ochr {


//FOR NOW
class Unit;


class Engine
{
public:
   using lfp = void (*)(void);
   using gefp = void (*)(Effro::Exception&);
   using sefp = void (*)(std::exception&);
   DECLARE(Offr::Engine)
   EXCEPTION(InvalidUse)
   EXCEPTION(ThreadsNotSet)
   EXCEPTION(PeriodNotSet)
   Engine() = delete;
   static void initialize();
   static int threads();
   static void threads(int);
   static int period();
   static void period(int);
   static void loop(lfp);
   static void exceptionHandlers(gefp,sefp);
   static void finalize();
   static void run();
   static int id();
   static int wIn();
   static int wOut();
   static void exit();
private:
   enum class State {null,init,final,exec,done};
   static void add(Unit*);
   static void erase(int);
   static void thread(int);
   static State _state;
   static int _threads;
   static int _periodNS;
   static lfp _loopFunc;
   static gefp _gweExcFunc;
   static sefp _stdExcFunc;
   thread_local static int _id;
   static std::atomic<int> _alive;
   static std::atomic<int> _cnt[2];
   static std::atomic<int> _lock;
   static int _fst;
   static std::atomic<unsigned int> _tick;
   static std::vector<std::vector<Unit*>> _units;
};



}
}
#endif
