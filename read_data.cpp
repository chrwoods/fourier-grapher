#include <fstream>
using namespace std;

/*#include <vector>
#include <array>
#include <iostream>
using namespace std;
*/

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

vector<array<double, 2>> get_normalized_data_from_file(const char* filename){
  ifstream datafile;
  datafile.open(filename);
  if(!datafile){
    cerr << "Could not read data from file " << filename << "." << endl;
    exit(-1);
  }

  double min_x = 10000000;
  double min_y = 10000000;
  double max_x = -10000000;
  double max_y = -10000000;
  vector<array<double, 2>> data;
  double x;
  while(datafile >> x){
    double y;
    datafile >> y;
    array<double, 2> arr;
    arr[0] = x;
    arr[1] = y;
    data.push_back(arr);
    if(x < min_x) min_x = x;
    if(y < min_y) min_y = y;
    if(x > max_x) max_x = x;
    if(y > max_y) max_y = y;
  }
  double cent_x = (max_x + min_x) / 2;
  double cent_y = (max_y + min_y) / 2;
  for(unsigned int i = 0; i < data.size(); i++){
    data[i][0] -= cent_x;
    data[i][1] -= cent_y;
  }
  return data;
}

/*int main(int, char**){
  vector<array<double, 2>> data = get_normalized_data_from_file("data/peace.txt");
  for(int i = 0; i < data.size(); i++){
    cout<<to_string(data[i][0])<<", "<<to_string(data[i][1])<<endl;
  }
}
*/
