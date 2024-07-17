#pragma once
#include <string>
#include <vector>
#include <sstream>
namespace game 
{
    void DebugPrint(const wchar_t* FuncName, const wchar_t* Msg);
    std::string byteVectorToString(std::vector<byte>& vec);

}