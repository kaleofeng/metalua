#ifndef _METALUA_GLOBALDEFINE_HPP_
#define _METALUA_GLOBALDEFINE_HPP_

#include "MetaLua/BaseDefine.hpp"

DECL_NAMESPACE_METALUA_BEGIN

/* macros */

#define GTYPENAME_LIST_0 typename R
#define GTYPENAME_LIST_1 GTYPENAME_LIST_0, typename P1
#define GTYPENAME_LIST_2 GTYPENAME_LIST_1, typename P2
#define GTYPENAME_LIST_3 GTYPENAME_LIST_2, typename P3
#define GTYPENAME_LIST_4 GTYPENAME_LIST_3, typename P4
#define GTYPENAME_LIST_5 GTYPENAME_LIST_4, typename P5
#define GTYPENAME_LIST_6 GTYPENAME_LIST_5, typename P6
#define GTYPENAME_LIST_7 GTYPENAME_LIST_6, typename P7
#define GTYPENAME_LIST_8 GTYPENAME_LIST_7, typename P8
#define GTYPENAME_LIST_9 GTYPENAME_LIST_8, typename P9

#define GTYPENAME_VOID_LIST_0
#define GTYPENAME_VOID_LIST_1 GTYPENAME_VOID_LIST_0 typename P1
#define GTYPENAME_VOID_LIST_2 GTYPENAME_VOID_LIST_1, typename P2
#define GTYPENAME_VOID_LIST_3 GTYPENAME_VOID_LIST_2, typename P3
#define GTYPENAME_VOID_LIST_4 GTYPENAME_VOID_LIST_3, typename P4
#define GTYPENAME_VOID_LIST_5 GTYPENAME_VOID_LIST_4, typename P5
#define GTYPENAME_VOID_LIST_6 GTYPENAME_VOID_LIST_5, typename P6
#define GTYPENAME_VOID_LIST_7 GTYPENAME_VOID_LIST_6, typename P7
#define GTYPENAME_VOID_LIST_8 GTYPENAME_VOID_LIST_7, typename P8
#define GTYPENAME_VOID_LIST_9 GTYPENAME_VOID_LIST_8, typename P9

#define GTYPE_LIST_0 R
#define GTYPE_LIST_1 GTYPE_LIST_0, P1
#define GTYPE_LIST_2 GTYPE_LIST_1, P2
#define GTYPE_LIST_3 GTYPE_LIST_2, P3
#define GTYPE_LIST_4 GTYPE_LIST_3, P4
#define GTYPE_LIST_5 GTYPE_LIST_4, P5
#define GTYPE_LIST_6 GTYPE_LIST_5, P6
#define GTYPE_LIST_7 GTYPE_LIST_6, P7
#define GTYPE_LIST_8 GTYPE_LIST_7, P8
#define GTYPE_LIST_9 GTYPE_LIST_8, P9

#define GTYPE_VOID_LIST_0 void
#define GTYPE_VOID_LIST_1 GTYPE_VOID_LIST_0, P1
#define GTYPE_VOID_LIST_2 GTYPE_VOID_LIST_1, P2
#define GTYPE_VOID_LIST_3 GTYPE_VOID_LIST_2, P3
#define GTYPE_VOID_LIST_4 GTYPE_VOID_LIST_3, P4
#define GTYPE_VOID_LIST_5 GTYPE_VOID_LIST_4, P5
#define GTYPE_VOID_LIST_6 GTYPE_VOID_LIST_5, P6
#define GTYPE_VOID_LIST_7 GTYPE_VOID_LIST_6, P7
#define GTYPE_VOID_LIST_8 GTYPE_VOID_LIST_7, P8
#define GTYPE_VOID_LIST_9 GTYPE_VOID_LIST_8, P9

#define GPARAM_TYPE_LIST_0
#define GPARAM_TYPE_LIST_1 GPARAM_TYPE_LIST_0  P1
#define GPARAM_TYPE_LIST_2 GPARAM_TYPE_LIST_1, P2
#define GPARAM_TYPE_LIST_3 GPARAM_TYPE_LIST_2, P3
#define GPARAM_TYPE_LIST_4 GPARAM_TYPE_LIST_3, P4
#define GPARAM_TYPE_LIST_5 GPARAM_TYPE_LIST_4, P5
#define GPARAM_TYPE_LIST_6 GPARAM_TYPE_LIST_5, P6
#define GPARAM_TYPE_LIST_7 GPARAM_TYPE_LIST_6, P7
#define GPARAM_TYPE_LIST_8 GPARAM_TYPE_LIST_7, P8
#define GPARAM_TYPE_LIST_9 GPARAM_TYPE_LIST_8, P9

#define GARG_LIST_0
#define GARG_LIST_1 GARG_LIST_0  ReadToCpp<P1>(L, 1)
#define GARG_LIST_2 GARG_LIST_1, ReadToCpp<P2>(L, 2)
#define GARG_LIST_3 GARG_LIST_2, ReadToCpp<P3>(L, 3)
#define GARG_LIST_4 GARG_LIST_3, ReadToCpp<P4>(L, 4)
#define GARG_LIST_5 GARG_LIST_4, ReadToCpp<P5>(L, 5)
#define GARG_LIST_6 GARG_LIST_5, ReadToCpp<P6>(L, 6)
#define GARG_LIST_7 GARG_LIST_6, ReadToCpp<P7>(L, 7)
#define GARG_LIST_8 GARG_LIST_7, ReadToCpp<P8>(L, 8)
#define GARG_LIST_9 GARG_LIST_8, ReadToCpp<P9>(L, 9)

/* register function */

struct GenericFunction {
    virtual int Call(lua_State *L) = 0;
};

#define DEF_FUNCTION_DATA(N) \
template<GTYPENAME_LIST_##N> \
struct FunctionData##N : public GenericFunction { \
    typedef R (*Function)(GPARAM_TYPE_LIST_##N); \
    Function m_function; \
    FunctionData##N(Function function) : m_function(function) {} \
    int Call(lua_State* L) override final { \
        auto result =(*m_function)(GARG_LIST_##N); \
        PushToLua<R>(L, result); \
        return 1; \
    } \
};

#define DEF_VOID_FUNCTION_DATA(N) \
template<GTYPENAME_VOID_LIST_##N> \
struct FunctionData##N<GTYPE_VOID_LIST_##N> : public GenericFunction { \
    typedef void (*Function)(GPARAM_TYPE_LIST_##N); \
    Function m_function; \
    FunctionData##N(Function function) : m_function(function) {} \
    int Call(lua_State* L) override final { \
        (*m_function)(GARG_LIST_##N); \
        return 0; \
    } \
};

inline int ProxyFunctionCall(lua_State *L) {
    auto index = lua_upvalueindex(1);
    auto memory = lua_touserdata(L, index);
    auto method = static_cast<GenericFunction*>(memory);
    return method->Call(L);
}

#define DEF_REGISTER_FUNCTION(N) \
template<GTYPENAME_LIST_##N> \
inline void RegisterFunctionToLua(lua_State* L, const char* name, \
    R (*function)(GPARAM_TYPE_LIST_##N)) { \
    typedef FunctionData##N<GTYPE_LIST_##N> FunctionData_t; \
    auto memory = lua_newuserdata(L, sizeof(FunctionData_t)); \
    new(memory) FunctionData_t(function); \
    lua_pushcclosure(L, &ProxyFunctionCall, 1); \
    lua_setglobal(L, name); \
}

DEF_FUNCTION_DATA(9)
DEF_FUNCTION_DATA(8)
DEF_FUNCTION_DATA(7)
DEF_FUNCTION_DATA(6)
DEF_FUNCTION_DATA(5)
DEF_FUNCTION_DATA(4)
DEF_FUNCTION_DATA(3)
DEF_FUNCTION_DATA(2)
DEF_FUNCTION_DATA(1)
DEF_FUNCTION_DATA(0)

DEF_VOID_FUNCTION_DATA(9)
DEF_VOID_FUNCTION_DATA(8)
DEF_VOID_FUNCTION_DATA(7)
DEF_VOID_FUNCTION_DATA(6)
DEF_VOID_FUNCTION_DATA(5)
DEF_VOID_FUNCTION_DATA(4)
DEF_VOID_FUNCTION_DATA(3)
DEF_VOID_FUNCTION_DATA(2)
DEF_VOID_FUNCTION_DATA(1)
DEF_VOID_FUNCTION_DATA(0)

DEF_REGISTER_FUNCTION(9)
DEF_REGISTER_FUNCTION(8)
DEF_REGISTER_FUNCTION(7)
DEF_REGISTER_FUNCTION(6)
DEF_REGISTER_FUNCTION(5)
DEF_REGISTER_FUNCTION(4)
DEF_REGISTER_FUNCTION(3)
DEF_REGISTER_FUNCTION(2)
DEF_REGISTER_FUNCTION(1)
DEF_REGISTER_FUNCTION(0)

/* register global variable */

template<typename V>
void RegisterVariableToLua(lua_State* L, const char* name, V variable) {
    PushToLua<V>(L, variable);
    lua_setglobal(L, name);
}

/* get global variable */

template<typename V>
V GetVariableInLua(lua_State* L, const char* name) {
    lua_getglobal(L, name);
    return ReadToCpp<V>(L, -1);
}

/* invoke global function */

template<typename R, typename... Args>
R InvokeFunctionInLua(lua_State* L, const char* name, Args... args) {
    StackAutoRecover sar(L);
    const auto errFunc = GetErrorFunction(L);

    lua_getglobal(L, name);
    if (!lua_isfunction(L, -1)) {
        lua_pushstring(L, "This arg is not a function.\n");
        lua_error(L);
    }

    VaradicPushToLua(L, args...);

    const auto size = sizeof...(args);
    lua_pcall(L, size, 1, errFunc);

    return ReadToCpp<R>(L, -1);
}

/* invoke table method */

template<typename R, typename... Args>
R InvokeMethodInLua(lua_State* L, const char* table, const char* name, Args... args) {
    StackAutoRecover sar(L);
    const auto errFunc = GetErrorFunction(L);

    lua_getglobal(L, table);
    if (!lua_istable(L, -1)) {
        lua_pushstring(L, "This arg is not a table.\n");
        lua_error(L);
    }

    lua_getfield(L, -1, name);
    if (!lua_isfunction(L, -1)) {
        lua_pushstring(L, "This arg is not a function.\n");
        lua_error(L);
    }

    lua_insert(L, -2);
    VaradicPushToLua(L, args...);

    const auto size = sizeof...(args) + 1;
    lua_pcall(L, size, 1, errFunc);

    return ReadToCpp<R>(L, -1);
}

DECL_NAMESPACE_METALUA_END

#endif // _METALUA_GLOBALDEFINE_HPP_
