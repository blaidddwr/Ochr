#include "engine.h"
#include "../src/engine.h"
#include <random>
#include <thread>
#include <iostream>



using namespace Gwers::Ochr;



std::random_device g_rd;
std::mt19937 g_gen(g_rd());
std::uniform_int_distribution<> g_dis(1,10000);
int g_count[32] {0};
bool g_test {false};



void loopA()
{
   ++g_count[Engine::id()];
   if (g_count[Engine::id()]==100)
   {
      Engine::exit();
   }
}



void loopB()
{
   int inc = Engine::id()+Engine::wIn();
   if (inc==Engine::threads())
   {
      inc = 0;
   }
   ++g_count[inc];
   if (g_count[inc]==100)
   {
      Engine::exit();
   }
}



void loopC()
{
   int inc = Engine::id()+Engine::wOut();
   if (inc==Engine::threads())
   {
      inc = 0;
   }
   ++g_count[inc];
   if (g_count[inc]==100)
   {
      Engine::exit();
   }
}



void loopD()
{
   std::this_thread::sleep_for(std::chrono::microseconds(g_dis(g_gen)));
   ++g_count[Engine::id()];
   if (g_count[Engine::id()]==100)
   {
      Engine::exit();
   }
}



void loopKillA()
{
   ++g_count[Engine::id()];
   if (g_count[Engine::id()]==100)
   {
      throw Effro::Exception("test","test",__LINE__);
   }
}



void loopKillB()
{
   ++g_count[Engine::id()];
   if (g_count[Engine::id()]==100)
   {
      throw std::exception();
   }
}



void catchA(Effro::Exception&)
{
   g_test = true;
}



void catchB(std::exception&)
{
   g_test = true;
}



void loop(Gwirio::UTest::Run& rn)
{
   {
      Engine::initialize();
      Engine::threads(32);
      Engine::period(8);
      Engine::loop(loopA);
      Engine::finalize();
      Engine::run();
      for (int i=0;i<32;i++)
      {
         if (g_count[i]!=100)
         {
            throw Gwirio::UTest::Run::Fail();
         }
      }
   }
   {
      rn.next();
      for (int i=0;i<32;i++)
      {
         g_count[i] = 0;
      }
      Engine::initialize();
      Engine::threads(32);
      Engine::period(8);
      Engine::loop(loopB);
      Engine::finalize();
      Engine::run();
      for (int i=0;i<32;i++)
      {
         if (g_count[i]!=100)
         {
            throw Gwirio::UTest::Run::Fail();
         }
      }
   }
   {
      rn.next();
      for (int i=0;i<32;i++)
      {
         g_count[i] = 0;
      }
      Engine::initialize();
      Engine::threads(32);
      Engine::period(8);
      Engine::loop(loopC);
      Engine::finalize();
      Engine::run();
      for (int i=0;i<32;i++)
      {
         if (g_count[i]!=100)
         {
            throw Gwirio::UTest::Run::Fail();
         }
      }
   }
   {
      rn.next();
      for (int i=0;i<32;i++)
      {
         g_count[i] = 0;
      }
      Engine::initialize();
      Engine::threads(32);
      Engine::period(8);
      Engine::loop(loopD);
      Engine::finalize();
      Engine::run();
      for (int i=0;i<32;i++)
      {
         if (g_count[i]!=100)
         {
            throw Gwirio::UTest::Run::Fail();
         }
      }
   }
}



void eexception(Gwirio::UTest::Run&)
{
   for (int i=0;i<32;i++)
   {
      g_count[i] = 0;
   }
   Engine::initialize();
   Engine::threads(32);
   Engine::period(8);
   Engine::loop(loopKillA);
   Engine::exceptionHandlers(catchA,nullptr);
   Engine::finalize();
   g_test = false;
   Engine::run();
   std::this_thread::sleep_for(std::chrono::seconds(1));
   if (!g_test)
   {
      throw Gwirio::UTest::Run::Fail();
   }
}



void sexception(Gwirio::UTest::Run&)
{
   for (int i=0;i<32;i++)
   {
      g_count[i] = 0;
   }
   Engine::initialize();
   Engine::threads(32);
   Engine::period(8);
   Engine::loop(loopKillB);
   Engine::exceptionHandlers(nullptr,catchB);
   Engine::finalize();
   g_test = false;
   Engine::run();
   std::this_thread::sleep_for(std::chrono::seconds(1));
   if (!g_test)
   {
      throw Gwirio::UTest::Run::Fail();
   }
}



void engine(Gwirio::UTest& tests)
{
   Gwirio::UTest::Run& rn {tests.add("Engine",nullptr,nullptr)};
   rn.add("loop",loop);
   rn.add("effroException",eexception);
   rn.add("stdException",sexception);
}
