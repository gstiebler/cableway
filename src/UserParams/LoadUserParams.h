#ifndef LOAD_USER_PARAMS_H__
#define LOAD_USER_PARAMS_H__

#include <string>

struct UserParams;

void loadUserParams( const std::string excelFileName, UserParams *userParams );

#endif
