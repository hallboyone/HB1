#include <HB1/CmdLnFlags.hpp>
#include <iostream>

HB1::CmdLnFlags::CmdLnFlags(int argc, char ** argv) :
  args_(std::vector<FlagPair>(argc, FlagPair())){
  //Save each argument in its raw form into args_
  for(int i = 0; i < argc; i++){
    args_[i].arg = argv[i];
  }

  //Split up sh flag groups ([...,-fvm,...] becomes [...,-f,-v,-m,...])
  for(auto it = args_.begin(); it != args_.end(); ++it){
    //If current string is sh group, split it
    if (numLeadingChar(it->arg, '-') ==  1 && !verifyDoublePara(it->arg) && it->arg.size() > 2){
      std::vector<FlagPair> split_sh_group;
     
      for (auto it_char = it->arg.begin() + 1; it_char != it->arg.end(); ++it_char){
	//Make sure all chars after the first are letters
	if(!std::isalpha(*it_char)) throw std::invalid_argument("Invalid SH group found");
	
	//Add new element to group vector. Start with a dash and then add the char
	split_sh_group.push_back(FlagPair("-", false));
	split_sh_group.back().arg += *it_char; 
      }

      //Erase old group, insert split group, reset 'it'
      it = args_.erase(it);
      args_.insert(it, split_sh_group.begin(), split_sh_group.end());
      it = args_.begin();
    }
  }

  args_[0].examined = true;
}

const std::string & HB1::CmdLnFlags::operator[](size_t n){
  if(n < args_.size()){
    args_[n].examined = true;
    return args_[n].arg;
  }
  else{
    throw std::out_of_range("Index of argument greater than number of arguments");
  }
}

const std::string & HB1::CmdLnFlags::back(){
  if(args_.size() > 0) args_.back().examined = true;
  return args_.back().arg;
}

const std::string & HB1::CmdLnFlags::front(){
    if(args_.size() > 0) args_.front().examined = true;
  return args_.front().arg;
}

size_t HB1::CmdLnFlags::size(){
  return args_.size();
}

bool HB1::CmdLnFlags::isSet(const char * key, bool allow_sh){
  std::vector<FlagPair>::iterator match = findFlag(key, allow_sh);
  if(match != args_.end()){
    match->examined = true;
    return true;
  }
  else{
    return false;
  }
}

bool HB1::CmdLnFlags::isSetWithPara(const char * key, int & para, bool allow_sh){
  std::vector<FlagPair>::iterator matching_arg = findFlag(key, allow_sh);
  
  if (matching_arg != args_.end()){//if a match was found, look for parameter in following arg
    matching_arg->examined = true;
    ++matching_arg;
    if (matching_arg == args_.end())
      throw std::invalid_argument("Flag needing integer parameter set without parameter");

    if (!verifyIntPara(matching_arg->arg))
      throw std::invalid_argument("Flag needing integer parameter set with invalid parameter");

    para = std::stoi(matching_arg->arg);

    matching_arg->examined = true;
    (matching_arg - 1)->examined = true;
    return true;
  }
  else return false;
}

bool HB1::CmdLnFlags::isSetWithPara(const char * key, double & para, bool allow_sh){
  std::vector<FlagPair>::iterator matching_arg = findFlag(key, allow_sh);
      
  if (matching_arg != args_.end()){//if a match was found, look for parameter in following arg
    matching_arg->examined = true;
    ++matching_arg;
    if (matching_arg == args_.end())
      throw std::invalid_argument("Flag needing integer parameter set without parameter");

    if (!verifyDoublePara(matching_arg->arg))
      throw std::invalid_argument("Flag needing integer parameter set with invalid parameter");

    para = std::stod(matching_arg->arg);
    matching_arg->examined = true;
    return true;
  }
  else return false;
}

bool HB1::CmdLnFlags::isSetWithPara(const char * key, char & para, bool allow_sh){
  std::vector<FlagPair>::iterator matching_arg = findFlag(key, allow_sh);

  if (matching_arg != args_.end()){//if a match was found, look for parameter in following arg
    matching_arg->examined = true;
    ++matching_arg;
    if (matching_arg == args_.end())
      throw std::invalid_argument("Flag needing integer parameter set without parameter");

    if (!verifyCharPara(matching_arg->arg))
      throw std::invalid_argument("Flag needing integer parameter set with invalid parameter");

    para = matching_arg->arg[0];
    matching_arg->examined = true;
    return true;
  }
  else return false;
}

bool HB1::CmdLnFlags::isSetWithPara(const char * key, std::string & para, bool allow_sh){
  std::vector<FlagPair>::iterator matching_arg = findFlag(key, allow_sh);

  if (matching_arg != args_.end()){//if a match was found, look for parameter in following arg
    matching_arg->examined = true;
    ++matching_arg;
    if (matching_arg == args_.end())
      throw std::invalid_argument("Flag needing integer parameter set without parameter");

    if (!verifyStringPara(matching_arg->arg))
      throw std::invalid_argument("Flag needing integer parameter set with invalid parameter");

    para = matching_arg->arg;
    matching_arg->examined = true;
    return true;
  }
  else return false;
}

bool HB1::CmdLnFlags::isSetWithOptPara(const char * key, int & para, bool allow_sh){
  //Case 1: no match       -> isSetWithPara returns false, para unchaged
  //Case 2: match, no para -> isSetWithPara throws an invalid_argument
  //Case 3: match, para    -> isSetWithPara returns true and sets para
  try{
    //Will throw exception if match is found but no argument
    return isSetWithPara(key, para, allow_sh);
  }
  catch (std::invalid_argument & e){
    return true;
  }
}

bool HB1::CmdLnFlags::isSetWithOptPara(const char * key, double & para, bool allow_sh){
  //Case 1: no match       -> isSetWithPara returns false, para unchaged
  //Case 2: match, no para -> isSetWithPara throws an invalid_argument
  //Case 3: match, para    -> isSetWithPara returns true and sets para
  try{
    //Will throw exception if match is found but no argument
    return isSetWithPara(key, para, allow_sh);
  }
  catch (std::invalid_argument & e){
    return true;
  }
}

bool HB1::CmdLnFlags::isSetWithOptPara(const char * key, char & para, bool allow_sh){
  //Case 1: no match       -> isSetWithPara returns false, para unchaged
  //Case 2: match, no para -> isSetWithPara throws an invalid_argument
  //Case 3: match, para    -> isSetWithPara returns true and sets para
  try{
    //Will throw exception if match is found but no argument
    return isSetWithPara(key, para, allow_sh);
  }
  catch (std::invalid_argument & e){
    return true;
  }
}

bool HB1::CmdLnFlags::isSetWithOptPara(const char * key, std::string & para, bool allow_sh){
  //Case 1: no match       -> isSetWithPara returns false, para unchaged
  //Case 2: match, no para -> isSetWithPara throws an invalid_argument
  //Case 3: match, para    -> isSetWithPara returns true and sets para
  try{
    //Will throw exception if match is found but no argument
    return isSetWithPara(key, para, allow_sh);
  }
  catch (std::invalid_argument & e){
    return true;
  }
}

void HB1::CmdLnFlags::printFlags(){
  for (FlagPair & f : args_){
    if (f.examined) std::cout<<"\033[32m"<<f.arg<<"\033[0m ";
    else          std::cout<<"\033[31m"<<f.arg<<"\033[0m ";
  }
  std::cout<<std::endl;
}

int HB1::CmdLnFlags::warnUnused(const char * msg){
  int num_unused = 0;
  std::string warning = "\033[33mWarning: ";
  if (msg == NULL) warning += "Some flags where not used";
  else             warning +=  msg;
  warning += '\n';
  
  for (FlagPair & f : args_){
    if (!f.examined){
      num_unused++;
      warning += f.arg + " ";
    }
  }
  if (num_unused > 0) std::cout<<warning<<"\033[0m\n";
  return num_unused;
}

std::vector<HB1::CmdLnFlags::FlagPair>::iterator HB1::CmdLnFlags::findFlag(const char * key, bool allow_sh){
  std::string potential_match;
  std::vector<FlagPair>::iterator cur_arg;
  
  for (cur_arg = args_.begin(); cur_arg != args_.end(); ++cur_arg){
    if (numLeadingChar(cur_arg->arg, '-') == 2){ //Full flag argument
      potential_match = cur_arg->arg;
      potential_match.erase(0,2);
      if (potential_match == key) return cur_arg;
    }
    else if (allow_sh && numLeadingChar(cur_arg->arg, '-') == 1){ //SH flag arg
      //Check argument only if it is not a negative numeric parameter
      if (!verifyDoublePara(cur_arg->arg)){
	//If sh char matches the sh flag, then we found the match
	if (key[0] == cur_arg->arg.back()) return cur_arg;
      }
    }
  }

  return cur_arg;
}

//Counts the number of leading '-' in the string 
int HB1::CmdLnFlags::numLeadingChar(const std::string & arg, const char c){
  size_t n = 0;
  while(n < arg.size()){
    if(arg[n] != c){
      break;
    }
    n++;
  }
  return n;
}

bool HB1::CmdLnFlags::verifyIntPara(const std::string & arg){
  //Make sure that, if '-' exists, it is at the start of string
  if(arg[0]=='-' && arg.find_last_of('-') != 0) return false;

  //Make sure all chars are the dash, or are numeric
  if(arg.find_first_not_of("0123456789-") != std::string::npos) return false;

  return true;
}
 
bool HB1::CmdLnFlags::verifyDoublePara(const std::string & arg){
  //Make sure that, if '-' exists, it is at the start of string
  if(arg[0]=='-' && arg.find_last_of('-') != 0) return false;

  //Make sure that, if '.' exists, only one exists
  if(arg.find_last_of(".") != arg.find_first_of(".")) return false;

  //Make sure all chars are decimal, dash, or numeric
  if(arg.find_first_not_of("0123456789-.") != std::string::npos) return false;

  return true;
}

bool HB1::CmdLnFlags::verifyCharPara(const std::string & arg){
  //Make sure string has size 1
  return (arg.size() == 1);
}

bool HB1::CmdLnFlags::verifyStringPara(const std::string & arg){
  //Make sure string is not a flag
  return (numLeadingChar(arg, '-') == 0 || numLeadingChar(arg, '-') > 2);
}
