#ifndef _METALUA_WRAPPER_HPP_
#define _METALUA_WRAPPER_HPP_

#include "MetaLua/ClassDefine.hpp"

DECL_NAMESPACE_METALUA_BEGIN

class LuaWrapper {
public:
    LuaWrapper();

    ~LuaWrapper();

public:
    bool Init();

    void Clear();

    void DoScript(const char* file);

public:
    template<typename T>
    void RegisterClass(const char* className) {
        RegisterClassToLua<T>(m_luaState, className);
    }

    template<typename F>
    void RegisterMethod(const char* funcName, F func) {
        RegisterMethodToLua(m_luaState, funcName, func);
    }

    template<typename T, typename V>
    void RegisterMember(const char* varName, V var) {
        RegisterMemberToLua<T>(m_luaState, varName, var);
    }

    template<typename T>
    void RegisterObject(const char* name, T* object) {
        RegisterObjectToLua<T>(m_luaState, name, object);
    }

private:
    lua_State* m_luaState;
};

DECL_NAMESPACE_METALUA_END

extern NS_METALUA::LuaWrapper* g_luaWrapper;
extern bool LuaInit();
extern void LuaClear();

#endif // _METALUA_WRAPPER_HPP_
