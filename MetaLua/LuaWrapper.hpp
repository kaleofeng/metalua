#ifndef _METALUA_WRAPPER_HPP_
#define _METALUA_WRAPPER_HPP_

#include "MetaLua/ClassDefine.hpp"
#include "MetaLua/GlobalDefine.hpp"
#include "MetaLua/LuaTable.hpp"

DECL_NAMESPACE_METALUA_BEGIN

class LuaWrapper {
public:
    LuaWrapper();

    ~LuaWrapper();

public:
    bool Initialize();

    void Finalize();

    void DoScript(const char* file);

public:
    template<typename T, typename P>
    void InheritParent() {
        return InheritParentToLua<T, P>(m_luaState);
    }

    template<typename T>
    void RegisterClass(const char* name) {
        RegisterClassToLua<T>(m_luaState, name);
    }

    template<typename M>
    void RegisterMethod(const char* name, M method) {
        RegisterMethodToLua(m_luaState, name, method);
    }

    template<typename M>
    void RegisterMember(const char* name, M member) {
        RegisterMemberToLua(m_luaState, name, member);
    }

    template<typename T>
    void RegisterObject(const char* name, T* object) {
        RegisterObjectToLua<T>(m_luaState, name, object);
    }

    template<typename F>
    void RegisterFunction(const char* funcName, F func) {
        RegisterFunctionToLua(m_luaState, funcName, func);
    }

    template<typename V>
    void RegisterVariable(const char* name, V variable) {
        RegisterVariableToLua<V>(m_luaState, name, variable);
    }

    template<typename R, typename... Args>
    R InvokeFunction(const char* name, Args... args) {
        return InvokeGlobalFunction<R, Args...>(m_luaState, name, args...);
    }

    template<typename V>
    V GetVariable(const char* name) {
        return GetGlobalVariable<V>(m_luaState, name);
    }

    LuaTable GetTable(const char* name) {
        lua_getglobal(m_luaState, name);
        if (!lua_istable(m_luaState, -1)) {
            lua_newtable(m_luaState);
            lua_setglobal(m_luaState, name);
        }
        const auto index = lua_gettop(m_luaState);
        return LuaTable(m_luaState, index);
    }

    lua_State* GetLuaState() {
        return m_luaState;
    }

private:
    lua_State* m_luaState;
};

DECL_NAMESPACE_METALUA_END

extern NS_METALUA::LuaWrapper* g_luaWrapper;
extern bool LuaStartup();
extern void LuaCleanup();

/* macros for register a class */

#define DEF_LUA_CLASS_BEGIN(Class) \
static void RegisterToLua(const char* className) { \
    typedef Class T; \
    g_luaWrapper->RegisterClass<T>(className);

#define DEF_METHOD(Func) \
    g_luaWrapper->RegisterMethod(#Func, &T::Func);

#define DEF_MEMBER(Var) \
    g_luaWrapper->RegisterMember(#Var, &T::Var);

#define INHERIT_PARENT(Parent) \
    typedef Parent P; \
    g_luaWrapper->InheritParent<T, P>();

#define DEF_END \
}

#define REGISTER_LUA_CALSS(Class) \
Class::RegisterToLua(#Class);

#endif // _METALUA_WRAPPER_HPP_
