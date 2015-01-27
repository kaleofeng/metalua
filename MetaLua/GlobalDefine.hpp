#ifndef _METALUA_GLOBALDEFINE_HPP_
#define _METALUA_GLOBALDEFINE_HPP_

#include "MetaLua/BaseDefine.hpp"

template<typename T>
void RegisterVariableToLua(lua_State* L, const char* name, T variable) {
    PushToLua<T>(L, variable);
    lua_setglobal(L, name);
}

template<typename T>
T GetGlobalVariable(lua_State* L, const char* name) {
    lua_getglobal(L, name);
    return ReadToCpp<T>(L, -1);
}

template<typename R, typename... Args>
R InvokeGlobalFunction(lua_State* L, const char*name, Args... args) {
    lua_getglobal(L, name);
    VaradicPushToLua(L, args...);

    const auto size = sizeof...(args);
    lua_call(L, size, 1);

    return ReadToCpp<R>(L, -1);
}

#endif // _METALUA_GLOBALDEFINE_HPP_
