#include <thread>
#include <atomic>
#include <vector>
#include "engine.h"
namespace Ochr {



namespace Engine
{
   enum class State {null,init,final,exec,done};
   void thread(int);
   State g_state {State::null};
   int g_threads {-1};
   int g_periodNS {-1};
   thread_local int g_id {-1};
   std::atomic<int> g_alive;
   std::atomic<int> g_cnt;
   std::atomic<unsigned int> g_tick {0};
   std::vector<std::vector<Unit*>> g_units;
}



void Engine::initialize()
{
   TRACE("Ochr::Engine::initialize()");
   ASSERT(g_state==State::null,InvalidUse,__LINE__);
   if (std::thread::hardware_concurrency()>0)
   {
      g_threads = std::thread::hardware_concurrency();
   }
   g_state = State::init;
}



int Engine::threads()
{
   TRACE("Ochr::Engine::threads()");
   return g_threads;
}



void Engine::threads(int Sz)
{
   TRACE("Ochr::Engine::threads(int)",Sz);
   ASSERT(g_state<=State::init,InvalidUse,__LINE__);
   if (g_state<=State::init)
   {
      g_threads = Sz;
   }
}



int Engine::period()
{
   TRACE("Ochr::Engine::period()");
   return g_periodNS/1000000;
}



void Engine::period(int time)
{
   TRACE("Ochr::Engine::period(int)",time);
   ASSERT(g_state<=State::init,InvalidUse,__LINE__);
   if (g_state<=State::init)
   {
      g_periodNS = time*1000000;
   }
}



void Engine::finalize()
{
   TRACE("Ochr::Engine::finalize()");
   ASSERT(g_state==State::init,InvalidUse,__LINE__);
   if (g_state==State::init)
   {
      g_units.resize(g_threads);
      g_state = State::final;
   }
}



void Engine::run()
{
   TRACE("Ochr::Engine::run()");
   ASSERT(g_state==State::final,InvalidUse,__LINE__);
   ASSERT(g_threads>0,ThreadsNotSet,__LINE__);
   ASSERT(g_periodNS>0,PeriodNotSet,__LINE__);
   if (g_state==State::final)
   {
      g_state = State::exec;
      g_alive = true;
      g_cnt = 0;
      for (int i = 1;i<g_threads;++i)
      {
         std::thread t {thread,i};
      }
      thread(0);
      g_state = State::done;
   }
}



void Engine::add(Unit* nu)
{
   TRACE("Ochr::Engine::add(Unit*)",nu);
   ASSERT(g_state==State::final||g_state==State::exec,InvalidUse,__LINE__);
   if (g_state==State::final||g_state==State::exec)
   {
      int i = g_id;
      if (g_state==State::final)
      {
         i = 0;
      }
      g_units[i].push_back(nu);
   }
}



void Engine::erase(int ui)
{
   TRACE("Ochr::Engine::erase(int)",ui);
   ASSERT(g_state==State::final||g_state==State::exec,InvalidUse,__LINE__);
   if (g_state==State::final||g_state==State::exec)
   {
      int i = g_id;
      if (g_state==State::final)
      {
         i = 0;
      }
      //CHANGE UNIT INCREMENT RIGHT HERE!!!
      g_units[i][ui] = g_units[i].back();
      g_units[i].pop_back();
   }
}



int Engine::id()
{
   TRACE("Ochr::Engine::id()");
   ASSERT(g_state==State::exec,InvalidUse,__LINE__);
   return g_id;
}



int Engine::which()
{
   TRACE("Ochr::Engine::which()");
   ASSERT(g_state==State::exec,InvalidUse,__LINE__);
   return g_tick&0x1;
}



void Engine::exit()
{
   TRACE("Ochr::Engine::exit()");
   ASSERT(g_state==State::exec,InvalidUse,__LINE__);
   if (g_state==State::exec)
   {
      g_alive = false;
   }
}



void Engine::thread(int id)
{
   D(Effro::Trace::flush());
   TRACE("Ochr::Engine::thread(int)",id);
   g_id = id;
   try
   {
      bool alive {true};
      while (alive)
      {
         ;
      }
   }
   catch(Effro::Exception e)
   {
      ;//WHAT TO DO HERE?
      g_alive = false;
   }
   catch(...)
   {
      ;//OR HERE??
      g_alive = false;
   }
}



}
