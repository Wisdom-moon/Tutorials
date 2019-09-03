#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

double geomean(std::vector<double> const&data) {
  const double too_large = 1.e64;
  const double too_small = 1.e-64;
  double sum_log = 0.0;
  double product = 1.0;
  for(auto x:data) {
      product *= x;
      if(product > too_large || product < too_small) {
          sum_log+= std::log(product);
          product = 1;      
      }
  }
  return std::exp((sum_log + std::log(product))/data.size());
}

std::vector<string> split_str(string str) {
  std::vector<string> sub_strs;
  unsigned int start_idx = 0;

  for (unsigned int id = 0; id < str.size(); id++) {
    if (str[id] == ' ' || str[id] == '\t' || str[id] == '\n') {
      if (id <= start_idx) {
	start_idx = id + 1;
      } 
      else {
        sub_strs.push_back(str.substr(start_idx, id - start_idx));
	start_idx = id + 1;
      }
    }
  }
  if (start_idx < str.size())
    sub_strs.push_back(str.substr(start_idx, str.size() - start_idx));

  return sub_strs;
}

class Data_Item {
private:
  unsigned long size;
  unsigned long partitions;
  unsigned long blocks;

  std::vector<double> h2d;
  std::vector<double> exe;
  std::vector<double> d2h;
  std::vector<double> total;

public:
  Data_Item() {size = 0;blocks = 0; partitions = 0;}
  ~Data_Item() {}
  bool add_Record(string str);
  void print_Geomean();
  void clean();
};

void Data_Item::print_Geomean() {
  if (size > 0)
    cout<<size<<"\t" <<partitions<<"\t"<<blocks;
  else if (blocks > 0)
    cout<<partitions<<"\t"<<blocks;

  if (h2d.size() > 0)
    cout<<"\t"<<geomean(h2d);
  if (exe.size() > 0)
    cout<<"\t"<<geomean(exe);
  if (d2h.size() > 0)
    cout<<"\t"<<geomean(d2h);
  if (total.size() > 0)
    cout<<"\t"<<geomean(total);

  cout<<endl;
}

void Data_Item::clean() {
  size = 0;
  partitions = 0;
  blocks = 0;
  h2d.clear();
  exe.clear();
  d2h.clear();
  total.clear();
}

bool Data_Item::add_Record(string str) {
    std::vector<string> sub_strs;
    sub_strs = split_str(str);
    bool ret = true;
    unsigned long s, p, b;
    switch (sub_strs.size()) {
      case 0:
	break;
      case 1:
	total.push_back(stod(sub_strs[0]));
	break;
      case 3:
	p = stoul (sub_strs[0]);
	b = stoul (sub_strs[1]);
	if (blocks == 0) {
	  partitions = p;
	  blocks = b;
	}

	if (partitions == p && blocks == b)
	  total.push_back(stod(sub_strs[2]));
	else
	  ret = false;
	break;
      case 4:
	s = stoul (sub_strs[0]);
	p = stoul (sub_strs[1]);
	b = stoul (sub_strs[2]);
	if (size == 0) {
	  size = s;
	  partitions = p;
	  blocks = b;
	}

	if (s == size && partitions == p && blocks == b)
	  total.push_back(stod(sub_strs[3]));
	else
	  ret = false;
	break;
      case 6:
	s = stoul (sub_strs[0]);
	p = stoul (sub_strs[1]);
	b = stoul (sub_strs[2]);
	if (size == 0) {
	  size = s;
	  partitions = p;
	  blocks = b;
	}

	if (s == size && partitions == p && blocks == b) {
	  h2d.push_back(stod(sub_strs[3]));
	  exe.push_back(stod(sub_strs[4]));
	  d2h.push_back(stod(sub_strs[5]));
	}
	else
	  ret = false;
	break;
      default:
	cerr << "Unknow format: " << str << endl;
	break;
    }

  return ret;
}


int main (int argc, char **argv)
{
  char * InputFile;
  ifstream fin;
  string line;

  if ( argc != 2 )
  {
    cout << "Usage: inputfile\n";
    return 1;
  }

  fin.open(argv[1]);

  Data_Item data;

  while(1) {
    if (fin.eof()) {
      data.print_Geomean();
      data.clean();
      break;
    }

    getline(fin, line);
    if (data.add_Record(line))
      continue;
    else {
      data.print_Geomean();
      data.clean();
      data.add_Record(line);
    }
  }

  fin.close();
  
  return 0;
}
