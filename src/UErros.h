//---------------------------------------------------------------------------

#ifndef UErrosH
#define UErrosH
//---------------------------------------------------------------------------
//#include <StdCtrls.hpp>
#include <string>
#include <vector>
class CErrosMsg
{
private:
  static CErrosMsg *instance;
  std::vector<std::string> erros;
  CErrosMsg();
public:
  static CErrosMsg *getInstance();
  //void defineMemo(TMemo* memo);
  void novoErro(std::string novo);
  static void destroi();
  static bool isNull(){return instance == 0;}
  ~CErrosMsg(){};
};

#endif
