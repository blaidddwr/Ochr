#include <gwirio.h>
#include "engine.h"



int main()
{
   Gwirio::UTest tests;
   engine(tests);
   if (!tests.execute())
   {
      return 1;
   }
   return 0;
}
