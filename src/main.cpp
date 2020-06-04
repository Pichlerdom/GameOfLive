#include "display.h"

int main(int argc, char* argv[]){
  uint32_t rule = 162405;
  if(argc == 2){
    rule = std::stoi(argv[1]);
  }

  display_loop(rule);
  
}
