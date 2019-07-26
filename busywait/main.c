
/*
Super useless code that just cycles through the USER LED's on a TivaC board
*/


void busyWait(){
  int counter = 0;
  while(counter < 10000000 ){
    counter ++;
  }
}

int main(){
    *((unsigned int*)0x400FE608U) = 0x20U; // enable GPIO F
    *((unsigned int*)0x40025400U) = 0X0Eu; // set all pins direction to output
    *((unsigned int*)0x4002551CU) = 0X0Eu; // set all pins to digital mode

    while(1){
      // 0x3FC is all bits, ie 0xFF*4
      *((unsigned int*)0x400253FCU) = 0X04u; // blue
      busyWait();
      
      *((unsigned int*)0x400253FCU) = 0X2u; // red
      busyWait();
      
      *((unsigned int*)0x400253FCU) = 0X8u; // green
      busyWait();
    }

    return 0;
}