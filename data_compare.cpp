#include "fourier.cpp"
#include "read_data.cpp"

int main(int argc, char* argv[]) {
  const char* filename = "data/diag_line.txt";
  if(argc > 1) filename = argv[1];
  
  //fill data
  vector<array<double, 2>> data;
  data = get_data_from_file(filename);
  
  //fill fourier
  vector<FourierElement> fouriers;
  for(int i = -1 * data.size(); i <= (signed int)data.size(); i++){
    if(i <= 0) continue; //skips 0 so we don't have a circle that sits still
    fouriers.push_back(fourier(data, i));
  }

  for(unsigned int i = 0; i < data.size(); i++){
    double x = 0;
    double y = 0;
    for(unsigned int j = 0; j < fouriers.size(); j++){
      FourierElement fe = fouriers[j];
      x += fe.amp * cos(TAU * fe.freq * (i / (double)data.size()) + fe.phase);
      y += fe.amp * sin(TAU * fe.freq * (i / (double)data.size()) + fe.phase);
    }
    cout << to_string(i) << ": expected: " << to_string(data[i][0]) << ", " << to_string(data[i][1]); 
    cout << ", calculated: " << to_string(x) << ", " << to_string(y) << endl;
    //calculate the in-between
    if(i == data.size() - 1) break;
    cout << to_string(i) << ".5: expected: " << to_string((data[i][0] + data[i + 1][0]) / 2.0) << ", " << to_string((data[i][1] + data[i + 1][1]) / 2.0);
    
    x = 0;
    y = 0;
    for(unsigned int j = 0; j < fouriers.size(); j++){
      FourierElement fe = fouriers[j];
      x += fe.amp * cos(TAU * fe.freq * ((i + 0.5) / (double)data.size()) + fe.phase);
      y += fe.amp * sin(TAU * fe.freq * ((i + 0.5) / (double)data.size()) + fe.phase);
    }

    cout << ", calculated: " << to_string(x) << ", " << to_string(y) << endl;
  }
}
