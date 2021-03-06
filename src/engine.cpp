#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include "engine.h"
#include <iostream>
namespace Gwers {
namespace Ochr {



constexpr int g_NanoSecsInMilliSec {1000000};



Engine::State Engine::_state {Engine::State::null};
int Engine::_threads {-1};
int Engine::_periodNS {-1};
Engine::Lfp Engine::_loopFunc {nullptr};
Engine::Eefp Engine::_gweExcFunc {nullptr};
Engine::Sefp Engine::_stdExcFunc {nullptr};
thread_local int Engine::_id {-1};
std::atomic<int> Engine::_alive;
std::atomic<int> Engine::_cnt[2];
std::atomic<int> Engine::_lock;
int Engine::_fst;
std::atomic<unsigned int> Engine::_tick {0};
std::vector<std::vector<Atom*>> Engine::_atoms;



void Engine::initialize()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::null,InvalidUse,__LINE__);
   if (std::thread::hardware_concurrency()>0)
   {
      _threads = std::thread::hardware_concurrency();
   }
   _state = State::init;
}



int Engine::threads()
{
   TRACE(__PRETTY_FUNCTION__);
   return _threads;
}



void Engine::threads(int Sz)
{
   TRACE(__PRETTY_FUNCTION__,Sz);
   ASSERT(_state<=State::init,InvalidUse,__LINE__);
   if (_state<=State::init)
   {
      _threads = Sz;
   }
}



int Engine::periodMS()
{
   TRACE(__PRETTY_FUNCTION__);
   return _periodNS/g_NanoSecsInMilliSec;
}



void Engine::periodMS(int time)
{
   TRACE(__PRETTY_FUNCTION__,time);
   ASSERT(_state<=State::init,InvalidUse,__LINE__);
   if (_state<=State::init)
   {
      _periodNS = time*g_NanoSecsInMilliSec;
   }
}



void Engine::loop(Lfp loopFunc)
{
   TRACE(__PRETTY_FUNCTION__,loopFunc);
   ASSERT(_state<=State::init,InvalidUse,__LINE__);
   if (_state<=State::init)
   {
      _loopFunc = loopFunc;
   }
}



void Engine::exceptionHandlers(Eefp gweExcFunc, Sefp stdExcFunc)
{
   TRACE(__PRETTY_FUNCTION__,gweExcFunc,stdExcFunc);
   ASSERT(_state<=State::init,InvalidUse,__LINE__);
   if (_state<=State::init)
   {
      _gweExcFunc = gweExcFunc;
      _stdExcFunc = stdExcFunc;
   }
}



void Engine::finalize()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::init,InvalidUse,__LINE__);
   if (_state==State::init)
   {
      _atoms.resize(_threads);
      _state = State::final;
   }
}



void Engine::run()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::final,InvalidUse,__LINE__);
   ASSERT(_threads>0,ThreadsNotSet,__LINE__);
   ASSERT(_periodNS>0,PeriodNotSet,__LINE__);
   if (_state==State::final)
   {
      _state = State::exec;
      _alive = true;
      _cnt[0] = _cnt[1] = 0;
      _lock = true;
      std::thread* ts[_threads];
      for (int i = 1;i<_threads;++i)
      {
         ts[i] = new std::thread(thread,i);
      }
      thread(0);
      for (int i = 1;i<_threads;++i)
      {
         ts[i]->join();
         delete ts[i];
      }
      _state = State::done;
   }
}



void Engine::add(Atom* nu)
{
   TRACE(__PRETTY_FUNCTION__,nu);
   ASSERT(_state==State::final||_state==State::exec,InvalidUse,__LINE__);
   if (_state==State::final||_state==State::exec)
   {
      int i = _id;
      if (_state==State::final)
      {
         i = 0;
      }
      _atoms[i].push_back(nu);
      nu->move(_atoms[i].size()-1);
      nu->newTId(i);
   }
}



void Engine::erase(int ui)
{
   TRACE(__PRETTY_FUNCTION__,ui);
   ASSERT(_state==State::final||_state==State::exec,InvalidUse,__LINE__);
   if (_state==State::final||_state==State::exec)
   {
      int i = _id;
      if (_state==State::final)
      {
         i = 0;
      }
      _atoms[i][ui] = nullptr;
   }
}



int Engine::id()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::exec,InvalidUse,__LINE__);
   return _id;
}



int Engine::wIn()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::exec,InvalidUse,__LINE__);
   return _tick&0x1;
}



int Engine::wOut()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::exec,InvalidUse,__LINE__);
   return (_tick&0x1)^0x1;
}



void Engine::exit()
{
   TRACE(__PRETTY_FUNCTION__);
   ASSERT(_state==State::exec,InvalidUse,__LINE__);
   if (_state==State::exec)
   {
      _alive = false;
   }
}



void Engine::thread(int id)
{
   using namespace std::chrono;
   D(Effro::Trace::flush();)
   TRACE(__PRETTY_FUNCTION__,id);
   _id = id;
   try
   {
      bool alive {true};
      while (alive)
      {
         auto t1 = high_resolution_clock::now();
         int ntick = _tick+1;
         ++_cnt[0];
         while (_cnt[0]<_threads)
         {
            std::this_thread::sleep_for(microseconds(1));
         }
         _loopFunc();
         for (int i = 0;i<_atoms[_id].size();)
         {
            Atom* p = _atoms[_id][i];
            if (p)
            {
               p->tock();
               ++i;
            }
            else
            {
               if (p!=_atoms[_id].back())
               {
                  p = _atoms[_id][i] = _atoms[_id].back();
                  if (p)
                  {
                     p->move(i);
                  }
               }
               _atoms[_id].pop_back();
            }
         }
         int det = ++_cnt[1];
         if (det==_threads)
         {
            while (_lock);
            if (_atoms[_id].size()>0)
            {
               Atom* p = _atoms[_id].back();
               _atoms[_id].pop_back();
               _atoms[_fst].push_back(p);
               p->move(_atoms[_fst].size()-1);
               p->newTId(_fst);
            }
            _lock = true;
            _cnt[0] = _cnt[1] = 0;
            _tick++;
         }
         else if (det==1)
         {
            _fst = _id;
            _lock = false;
         }
         auto t2 = high_resolution_clock::now();
         int dur = duration_cast<nanoseconds>(t2-t1).count();
         if (dur<_periodNS)
         {
            std::this_thread::sleep_for(nanoseconds(_periodNS-dur));
         }
         while (ntick!=_tick&&_alive)
         {
            std::this_thread::sleep_for(microseconds(1));
         }
         if (!_alive)
         {
            alive = false;
         }
      }
   }
   catch(Effro::Exception e)
   {
      if (_gweExcFunc)
      {
         _alive = false;
         _lock = false;
         _gweExcFunc(e);
      }
      else
      {
         throw e;
      }
   }
   catch(std::exception e)
   {
      if (_stdExcFunc)
      {
         _alive = false;
         _lock = false;
         _stdExcFunc(e);
      }
      else
      {
         throw e;
      }
   }
}



}
}
