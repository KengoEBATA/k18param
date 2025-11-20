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
  
  int runnum;
  runnum = atoi(argv[1]);
  int mag;
  mag = atoi(argv[2]);


  for(int i=1; i<7; ++i){
  
    char iQmConfname[100];
    char iQpConfname[100];
  
    snprintf(iQmConfname, 100, "%s%d%s%d%s", "analyzer_e70_", runnum,"_Qm00",i, ".conf");
    snprintf(iQpConfname, 100, "%s%d%s%d%s", "analyzer_e70_", runnum,"_Qp00",i, ".conf");
  
    std::ifstream iQmConf(iQmConfname);
    std::ifstream iQpConf(iQpConfname);
  
  
    if(iQmConf.fail()){
      cout << "No such conf file found " << iQmConfname<< endl;
      return -1;
    }
    if(iQpConf.fail()){
      cout << "No such conf file found " << iQpConfname<< endl;
      return -1;
    }
  
    char oQmConfname[100];
    char oQpConfname[100];
    snprintf(oQmConfname, 100, "%s%s", iQmConfname, "_temp");
    snprintf(oQpConfname, 100, "%s%s", iQpConfname, "_temp");
    std::ofstream oQmConf(oQmConfname);
    std::ofstream oQpConf(oQpConfname);
  
    std::string str;
    std::string attr, _f_param; 
    std::string dir_param; 
    std::stringstream f_param;
  
    std::string keyK18TM = "K18TM:";
  
    std::regex prefixK18TM(R"(K18MatrixParam_(\d{5}_Q\d{1,2}[m,p]\d{3}))");
  
  
    while(getline(iQmConf, str)){
      std::istringstream input_line(str);
      input_line >> attr >> _f_param ;
      f_param.str("");
      dir_param="";
  
      if(attr == keyK18TM){
        std::smatch match;
        if(std::regex_search(_f_param, match, prefixK18TM)){
          std::string old = match.str(1);
          cout <<"old : "<< match.str(0) << match.str(1) << endl;
          std::string replaced = 
            std::regex_replace(_f_param, 
                               std::regex("K18MatrixParam_"+old),
                               "K18MatrixParam_"
  			    +std::to_string(runnum)
  			    +"_Q"+std::to_string(mag)
  			    +"m00"+std::to_string(i));
        f_param << replaced;
        cout << "K18TM : replaced = " << replaced << endl;
        }
      }
      else{
        f_param << _f_param;
      }
      oQmConf << attr << "\t\t" << f_param.str() << "\n" ;
      //cout << attr << "\t\t" << f_param.str() << endl ;
    }
  
    iQmConf.close(); 
    oQmConf.close(); 
  
      
    if(std::remove(iQmConfname)!=0){
      std::cerr << "file to remove org file \n"; 
      return -1;
    }
    if(std::rename(oQmConfname,iQmConfname)!=0){
      std::cerr << "file to rename tmp file \n"; 
      return -1;
    }
  
  
    while(getline(iQpConf, str)){
      std::istringstream input_line(str);
      input_line >> attr >> _f_param ;
      f_param.str("");
      dir_param="";
  
      if(attr == keyK18TM){
        std::smatch match;
        if(std::regex_search(_f_param, match, prefixK18TM)){
          std::string old = match.str(1);
          std::string replaced = 
            std::regex_replace(_f_param, 
                               std::regex("K18MatrixParam_"+old),
                               "K18MatrixParam_"
  			    +std::to_string(runnum)
  			    +"_Q"+std::to_string(mag)
  			    +"p00"+std::to_string(i));
        f_param << replaced;
        cout << "K18TM : replaced = " << replaced << endl;
        }
      }
      else{
        f_param << _f_param;
      }
      oQpConf << attr << "\t\t" << f_param.str() << "\n" ;
      //cout << attr << "\t\t" << f_param.str() << endl ;
    }
  
    iQpConf.close(); 
    oQpConf.close(); 
  
     
    if(std::remove(iQpConfname)!=0){
      std::cerr << "file to remove org file \n"; 
      return -1;
    }
    if(std::rename(oQpConfname,iQpConfname)!=0){
      std::cerr << "file to rename tmp file \n"; 
      return -1;
    }
    
  }
  return 0;
}
