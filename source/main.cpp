#include "display.h"
#include <iostream>



int32_t main(int32_t argc, char* argv[]) {

  const int limbs = 3;
  
  Display display;
  display.init(argc,argv, limbs);

  return 0;
}
