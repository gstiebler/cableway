//---------------------------------------------------------------------------

#ifndef UErrosH
#define UErrosH
//---------------------------------------------------------------------------

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
  void novoErro(std::string novo);
  static void destroi();
	void CErrosMsg::transferErrors( std::vector<std::string> &errors );
};

#endif
