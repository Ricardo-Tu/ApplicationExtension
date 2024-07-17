#include "pch.h"
#include "tools.hpp"

namespace game
{
    /*
     Routine Description:

         This routine print debug message to output debug string in visual studio 2022 output window.
     */
    void DebugPrint(const wchar_t* FuncName, const wchar_t* Msg)
    {
        std::wstringstream ssErrorStr;
        ssErrorStr << FuncName << L": " << Msg << L"\n";
        OutputDebugString(ssErrorStr.str().c_str());
    }

    std::string byteVectorToString(std::vector<byte>& vec)
    {
        std::string ret;
        for (auto& v : vec)
        {
            ret += v;
        }
        return ret;
    }
}