#define TAU 3.14159265358979 * 2

#include <vector>
#include <cmath>

class FourierElement{ //needs a better name
public:
  double x;
  double y;
  int freq;
  double amp;
  double phase;

  FourierElement(double a, double b, double c, double d, double e){
    x = a;
    y = b;
    freq = c;
    amp = d;
    phase = e;
  }
};

FourierElement fourier(double[][2] data, int waves){
  double x = 0; //real portion 
  double y = 0; //imaginary portion
  //do a Discrete Fourier Transformation (no FFT)
  for(int n = 0; n < data.length; n++){
    double dx = data[n][0];
    double dy = data[n][1];
    x += dx * cos(TAU * waves * n / data.length) + dy * sin(TAU * waves * n / data.length);
    y += dy * cos(TAU * waves * n / data.length) - dx * sin(TAU * waves * n / data.length);
  }
  //calculate circle stuff
  double amp = sqrt(x * x + y * y); //radius
  double phase = atan2(y, x); //starting position to rotate from (probably)
  FourierElement fe(x, y, waves, amp, phase);
  return fe;
}
  
    
