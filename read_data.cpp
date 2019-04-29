#include <fstream>
using namespace std;

vector<array<double, 2>> get_data_from_file(const char* filename){
  ifstream datafile;
  datafile.open(filename);
  if(!datafile){
    cerr << "Could not read data from file " << filename << "." << endl;
    exit(-1);
  }

  vector<array<double, 2>> data;
  double x;
  while(datafile >> x){
    double y;
    datafile >> y;
    array<double, 2> arr;
    arr[0] = x;
    arr[1] = y;
    data.push_back(arr);
  }
  return data;
}
