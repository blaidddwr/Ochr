#include "engine.h"
#include "../src/engine.h"
#include <iostream>



using namespace Gwers::Ochr;



int g_count[32] {0};



void loopA()
{
   ++g_count[Engine::id()];
   if (g_count[Engine::id()]==100)
   {
      Engine::exit();
   }
}



void testall(Gwirio::UTest::Run& rn)
{
   Engine::initialize();
   Engine::threads(32);
   Engine::period(16);
   Engine::loop(loopA);
   Engine::finalize();
   Engine::run();
   std::cout << "\n";
   for (int i=0;i<32;i++)
   {
      std::cout << g_count[i] << "\n";
   }
   for (int i=0;i<32;i++)
   {
      if (g_count[i]!=100)
      {
         throw Gwirio::UTest::Run::Fail();
      }
   }
}



void engine(Gwirio::UTest& tests)
{
   Gwirio::UTest::Run& rn {tests.add("Engine",nullptr,nullptr)};
   rn.add("testall",testall);
}
