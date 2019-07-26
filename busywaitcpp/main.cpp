// This is super useless code that cycles an LED on a TivaC board in modern c++

template<unsigned int T>
class register_t {
public:
    register_t() = default;
    register_t(unsigned int val){
      *((unsigned int*)T) = val;
    }
      
    void operator = (unsigned int val) {
      *((unsigned int*)T) = val;
    }
};

using Direction_Mode = register_t<0x40025400U>;
using Clock_Gate = register_t<0x400FE608U>;
using LED_Pin = register_t<0x400253FCU>;
using Digital_Mode = register_t<0x4002551CU>;


int main(){
    Clock_Gate( 0x20U ); // enable GPIO F
    Direction_Mode(0X0Eu); // set direction to output
    Digital_Mode( 0X0Eu ); // set to digital bode

    auto busyWait = [] {
    int counter = 0;
      while(counter < 1000000 ){
        counter ++;
      }
    };
    LED_Pin led;

    while(1){
      // 0x3FC is all bits, ie 0xFF*4
      led = 0X04u; // blue
      busyWait();

      led = 0X2u; // red
      busyWait();

    }

    return 0;
}
