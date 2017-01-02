#ifndef _METALUA_WRAPPER_HPP_
#define _METALUA_WRAPPER_HPP_

#include "MetaLua/ClassDefine.hpp"
#include "MetaLua/GlobalDefine.hpp"
#include "MetaLua/LuaTable.hpp"

DECL_NAMESPACE_METALUA_BEGIN

class LuaWrapper {

public:
    struct UserData {
        LuaWrapper* m_luaWrapper{ nullptr };
        int m_errroFunc{ 0 };
    };

private:
    lua_State* m_luaState{ nullptr };
    UserData m_userData;

public:
    LuaWrapper();

    ~LuaWrapper();

    LuaWrapper(const LuaWrapper& other) = delete;

    LuaWrapper& operator =(const LuaWrapper& other) = delete;

public:
    bool Initialize();

    void Finalize();

    void AddPackagePath(const char* path);

    void DoScript(const char* file);

    void DoString(const char* text);

    lua_State* LuaState() {
        return m_luaState;
    }

    template<typename T, typename P>
    void InheritParent() {
        InheritParentToLua<T, P>(m_luaState);
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
    void RegisterFunction(const char* name, F function) {
        RegisterFunctionToLua(m_luaState, name, function);
    }

    template<typename V>
    void RegisterVariable(const char* name, V variable) {
        RegisterVariableToLua<V>(m_luaState, name, variable);
    }

    template<typename R, typename... Args>
    R InvokeFunction(const char* name, Args... args) {
        return InvokeFunctionInLua<R, Args...>(m_luaState, name, args...);
    }

    template<typename R, typename... Args>
    R InvokeMethod(const char* table, const char* name, Args... args) {
        return InvokeMethodInLua<R, Args...>(m_luaState, table, name, args...);
    }

    template<typename V>
    V GetVariable(const char* name) {
        return GetVariableInLua<V>(m_luaState, name);
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
};

DECL_NAMESPACE_METALUA_END

extern thread_local NS_METALUA::LuaWrapper* g_luaWrapper;
extern bool LuaStartup();
extern void LuaCleanup();

/* macros for register a class */

#define METALUA_CLASS_BEGIN(Class) \
static void RegisterToLua(const char* className) { \
typedef Class T; \
g_luaWrapper->RegisterClass<T>(className);

#define DEF_METHOD(Method) \
g_luaWrapper->RegisterMethod(#Method, &T::Method);

#define DEF_MEMBER(Member) \
g_luaWrapper->RegisterMember(#Member, &T::Member);

#define INHERIT_PARENT(Parent) \
typedef Parent P; \
g_luaWrapper->InheritParent<T, P>();

#define METALUA_CLASS_END \
}

#define METALUA_REGISTER_CLASS(Class) \
Class::RegisterToLua(#Class);

#define METALUA_REGISTER_OBJECT(name, object) \
g_luaWrapper->RegisterObject(name, object);

#define METALUA_REGISTER_FUNCTION(name, function) \
g_luaWrapper->RegisterFunction(name, function);

#define METALUA_REGISTER_VARIABLE(name, variable) \
g_luaWrapper->RegisterVariable(name, variable);

#endif // _METALUA_WRAPPER_HPP_
