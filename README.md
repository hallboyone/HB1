# HB1
Library of function in the HB1 namespace. A DoxyFile is provided to generate documentation if desired. 

## CmdLnFlags
Abstracts away the use of command line arguments in c++ projects. Basic usage would be to create an instance of a
CmdLnFlags object from the argc and argv objects in the main function. Then, as needed member functions can be called
to check the status of flags. For example
```cpp
int main(int argc, char ** argv){
  HB1::CmdLnFlags flags(argc, argv);
  
  if (flags.isSet("verbose")){ //Check if verbose flag is set
    std::cout<<"Extra info!"<<std::endl;
  }
  
  int para = 0;
  flags.isSetWithPara("iterations", para, true); // Looks for flag followed by int parameter
  
  return 1;
}
```
