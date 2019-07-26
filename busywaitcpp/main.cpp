
namespace gpiof {
    static constexpr unsigned int direction     = 0x40025400U;
    static constexpr unsigned int clock_gate    = 0x400FE608U;
    static constexpr unsigned int digital_mode  = 0x4002551CU;
    static constexpr unsigned int data          = 0x400253FCU;
};

template<unsigned int address, unsigned int val>
class register_t {
public:
  static void set() {
    *reinterpret_cast<volatile unsigned int*>(address) = val;
  }
};


int main(){
  register_t<gpiof::clock_gate, 0x20U>::set(); // enable gpiof
  register_t<gpiof::direction, 0x0EU>::set();  // set all to output
  register_t<gpiof::digital_mode, 0x0EU>::set(); // set all to digital
  using blue_led = register_t<gpiof::data, 0x4U>;
  using red_led  = register_t<gpiof::data, 0x2U>;

  auto busyWait = [] {
    int counter = 0;
      while(counter < 1000000 ){
        counter ++;
      }
    };

    while(1){
      blue_led::set();
      busyWait();

      red_led::set();
      busyWait();

    }
  
    return 0;
}
