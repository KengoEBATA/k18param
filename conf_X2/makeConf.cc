#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

int main(int argc,const char*argv[]){
  if(argc!=3){
    cout<< "#Arguments is insufficient." << endl;
    return -1;
  }
  
  int runnum[2];
  for(int i=0; i<argc-1; ++i){
    runnum[i] = atoi(argv[i+1]);
  }

  cout << "conf copied : " << runnum[0]<< " -> " << runnum[1] << endl;

  char ifsname[100];
  char magfsname[500];
  char ofsname[100];

  snprintf(ifsname, 100, "%s%d%s", "analyzer_e70_", runnum[0],".conf");
  std::ifstream ifs(ifsname);

  snprintf(magfsname, 500, "%s%s%d",
              "/group/had/sks/E70/JPARC2025Jan/subdata",
              "/K18MagnetParam/K18MagnetParam_",
              runnum[1]
           );
  std::ifstream magfs(magfsname);


  if(ifs.fail()){
    cout << "No such conf file found : "<< ifsname << endl;
    return -1;
  }
  if(magfs.fail()){
    cout << "No such k18magnet file found : "<< magfsname<< endl;
    return -1;
  }


  std::string str;
  std::string mag;
  std::vector<std::string> keyMag = {"PK18", "S2SD1_FLD"};
  double val[2];
  double _val, e_val;
  while(getline(magfs, str)){
    std::istringstream input_line(str);
    input_line >> mag >> _val >> e_val;
    auto it = std::find(keyMag.begin(),keyMag.end(), mag);
    if(it != keyMag.end()){
      int idx = std::distance(keyMag.begin(), it);
      val[idx] = _val;
    }
  }



  snprintf(ofsname, 100, "%s%d%s", "analyzer_e70_", runnum[1],".conf");
  std::ofstream ofs(ofsname);
  //std::string str;
  std::string attr, _f_param; 
  std::string dir_param; 
  std::stringstream f_param;

  std::vector<std::string> key = {"DCDRFT:", "DCTDC:","HDPRM:","HDPHC:"};
  std::vector<std::string> keyFLD = {"PK18:", "FLDNMR:"};
  std::string keyK18TM = "K18TM:";

  std::vector<std::string> prefix = {"DCDriftParam", "DCTdcCalib","HodoParam","HodoPHCParam"};
  std::regex prefixK18TM(R"(K18MatrixParam_(\d+))");


  while(getline(ifs, str)){
    std::istringstream input_line(str);
    input_line >> attr >> _f_param ;
    f_param.str("");
    dir_param="";
    auto it = std::find(key.begin(),key.end(), attr);
    auto itFLD = std::find(keyFLD.begin(),keyFLD.end(), attr);
    if(it != key.end()){
      int idx = std::distance(key.begin(), it);
      for(char ch: attr){
        if(ch != ':'){
          dir_param += ch;
        }
      }
      f_param << "../" << dir_param << "/" <<prefix[idx] << "_e70_KK_" << runnum[1] ;
    }

    else if(itFLD != keyFLD.end()){
      int idx = std::distance(keyFLD.begin(), itFLD);
      f_param << std::setprecision(12);
      f_param << val[idx]; 
    }

    else if(attr == keyK18TM){
      std::smatch match;
      if(std::regex_search(_f_param, match, prefixK18TM)){
        std::string old = match.str(1);
        std::string replaced = 
          std::regex_replace(_f_param, 
                             std::regex("K18MatrixParam_"+old),
                             "K18MatrixParam_"+std::to_string(runnum[1]));
      f_param << replaced;
      cout << "K18TM : replaced = " << replaced << endl;
      }
    }
    else{
      f_param << _f_param;
    }
    ofs << attr << "\t\t" << f_param.str() << "\n" ;
    cout << attr << "\t\t" << f_param.str() << endl ;
  }

  ifs.close(); 
  ofs.close(); 
  /*  
  if(std::remove(ifsname)!=0){
    std::cerr << "file to remove org file \n"; 
    return -1;
  }
  if(std::rename(ofsname,ifsname)!=0){
    std::cerr << "file to rename tmp file \n"; 
    return -1;
  }
*/

  return 0;
}
