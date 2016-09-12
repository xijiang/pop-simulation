/**
 * # ID start from 1, hence ID will be added ne-1 in extended.ped
 *
 *                                                            by Xijiang Yu
 *                                                            Apr. 13, 2015
 */

#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>

using namespace std;

class Record{
public:
  string id;
  string pa;
  string ma;
  string info;
  Record(string a, string b, string c, string d):id{a}, pa{b}, ma{c}, info{d}{;}
};

int main(int argc, char *argv[])
{
  const set<string> missing{".", "-", "unknown", "Unknown", "UNKNOWN", "999999",
      "missing", "Missing", "MISSING"}; //and whatever else can be added here.
  //All of above will be renamed to "0".
  const string unknown{"0"};
  vector<Record> ped;
  
  for(string line, id, pa, ma, info; getline(cin, line);){
    stringstream ss(line);
    if(ss>>id>>pa>>ma){ //ignore those that have less than 3 fields.
      getline(ss,info);
      if(missing.find(pa) != missing.end()) pa=unknown;
      if(missing.find(ma) != missing.end()) ma=unknown;
      ped.push_back(Record(id,pa,ma,info));
    }else clog<<"Ignored: "<<line<<'\n';
  }

  map<string, int> uid;
  for(auto&p:ped){//Suppose unknown ID all appear in pa & ma columns
    uid[p.pa] = -2;
    uid[p.ma] = -2;
  }

  // ID in the id column
  for(auto&p:ped) uid[p.id] = -1;
  uid[unknown] = 0;

  // ID only appear in parent columns
  int id{0}, td(uid.size()-1);
  for(auto&p:uid) if(p.second == -2){
      p.second=++id;
      cout<<id<<" 0 0 Unknown\n";
      --td;
    }
  
  // Then all the ID in the ID column.
  while(td)
    for(auto&p:ped)
      if(uid[p.id] ==-1 &&
	 uid[p.ma] >= 0 &&
	 uid[p.ma] >= 0){
	++id;
	cout<<id<<' '<<uid[p.pa]<<' '<<uid[p.ma]<<' '<<p.id<<' '<<p.info<<'\n';
	uid[p.id] = id;
	--td;
      }

  return 0;
}
