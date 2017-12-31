#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[]) {
  string output = "max_" + string(argv[1]);
  ifstream ifs(argv[1]);
  ofstream ofs(output);
  string str;

  while (getline(ifs,str)) {
    int size = str.length();
    string pre = str.substr(0, 5);
    if (pre=="max s") {
      ofs << str.substr(12, size-12) << endl;
    }
  }
  
  ifs.close();
  ofs.close();
}
