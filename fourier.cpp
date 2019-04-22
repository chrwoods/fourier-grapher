#define TAU 3.14159265358979 * 2

#include <iostream>
using namespace std;
#include <vector>
#include <array>
#include <cmath>
#include <string>

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

  string to_str(){
    string str = "";
    str += "x: " + to_string(x);
    str += ", y: " + to_string(y);
    str += ", frequency: " + to_string(freq);
    str += ", amplitude: " + to_string(amp);
    str += ", phase: " + to_string(phase);
    return str;
  }
};

FourierElement fourier(vector<array<double, 2>> data, int waves){
  double x = 0; //real portion 
  double y = 0; //imaginary portion
  int N = data.size();
  //do a Discrete Fourier Transformation (no FFT)
  for(int n = 0; n < N; n++){
    double dx = data[n][0];
    double dy = data[n][1];
    x += dx * cos(TAU * waves * n / N) + dy * sin(TAU * waves * n / N);
    y += dy * cos(TAU * waves * n / N) - dx * sin(TAU * waves * n / N);
  }
  //calculate circle stuff
  double amp = sqrt(x * x + y * y); //radius
  double phase = atan2(y, x); //starting position to rotate from (probably)
  FourierElement fe(x, y, waves, amp, phase);
  return fe;
}

int main(){
  vector<array<double, 2>> data;
  for(int i = 0; i < 19; i++){
    array<double, 2> temp;
    temp[0] = i;
    temp[1] = i * 2;
    data.push_back(temp);
  }
  for(int i = 1; i < 20; i++){
    FourierElement fe = fourier(data, i);
    cout << fe.to_str() << endl;
  }
}
  
    
