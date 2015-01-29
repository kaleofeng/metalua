#ifndef _METALUA_CLASSDEFINE_HPP_
#define _METALUA_CLASSDEFINE_HPP_

#include <cassert>
#include <new>

#include "MetaLua/BaseDefine.hpp"

DECL_NAMESPACE_METALUA_BEGIN

/* macros */

#define CTYPENAME_LIST_0 typename R, typename T
#define CTYPENAME_LIST_1 CTYPENAME_LIST_0, typename P1
#define CTYPENAME_LIST_2 CTYPENAME_LIST_1, typename P2
#define CTYPENAME_LIST_3 CTYPENAME_LIST_2, typename P3
#define CTYPENAME_LIST_4 CTYPENAME_LIST_3, typename P4
#define CTYPENAME_LIST_5 CTYPENAME_LIST_4, typename P5
#define CTYPENAME_LIST_6 CTYPENAME_LIST_5, typename P6
#define CTYPENAME_LIST_7 CTYPENAME_LIST_6, typename P7
#define CTYPENAME_LIST_8 CTYPENAME_LIST_7, typename P8
#define CTYPENAME_LIST_9 CTYPENAME_LIST_8, typename P9

#define CTYPENAME_VOID_LIST_0 typename T
#define CTYPENAME_VOID_LIST_1 CTYPENAME_VOID_LIST_0, typename P1
#define CTYPENAME_VOID_LIST_2 CTYPENAME_VOID_LIST_1, typename P2
#define CTYPENAME_VOID_LIST_3 CTYPENAME_VOID_LIST_2, typename P3
#define CTYPENAME_VOID_LIST_4 CTYPENAME_VOID_LIST_3, typename P4
#define CTYPENAME_VOID_LIST_5 CTYPENAME_VOID_LIST_4, typename P5
#define CTYPENAME_VOID_LIST_6 CTYPENAME_VOID_LIST_5, typename P6
#define CTYPENAME_VOID_LIST_7 CTYPENAME_VOID_LIST_6, typename P7
#define CTYPENAME_VOID_LIST_8 CTYPENAME_VOID_LIST_7, typename P8
#define CTYPENAME_VOID_LIST_9 CTYPENAME_VOID_LIST_8, typename P9

#define CTYPE_LIST_0 R, T
#define CTYPE_LIST_1 CTYPE_LIST_0, P1
#define CTYPE_LIST_2 CTYPE_LIST_1, P2
#define CTYPE_LIST_3 CTYPE_LIST_2, P3
#define CTYPE_LIST_4 CTYPE_LIST_3, P4
#define CTYPE_LIST_5 CTYPE_LIST_4, P5
#define CTYPE_LIST_6 CTYPE_LIST_5, P6
#define CTYPE_LIST_7 CTYPE_LIST_6, P7
#define CTYPE_LIST_8 CTYPE_LIST_7, P8
#define CTYPE_LIST_9 CTYPE_LIST_8, P9

#define CTYPE_VOID_LIST_0 void, T
#define CTYPE_VOID_LIST_1 CTYPE_VOID_LIST_0, P1
#define CTYPE_VOID_LIST_2 CTYPE_VOID_LIST_1, P2
#define CTYPE_VOID_LIST_3 CTYPE_VOID_LIST_2, P3
#define CTYPE_VOID_LIST_4 CTYPE_VOID_LIST_3, P4
#define CTYPE_VOID_LIST_5 CTYPE_VOID_LIST_4, P5
#define CTYPE_VOID_LIST_6 CTYPE_VOID_LIST_5, P6
#define CTYPE_VOID_LIST_7 CTYPE_VOID_LIST_6, P7
#define CTYPE_VOID_LIST_8 CTYPE_VOID_LIST_7, P8
#define CTYPE_VOID_LIST_9 CTYPE_VOID_LIST_8, P9

#define CPARAM_TYPE_LIST_0
#define CPARAM_TYPE_LIST_1 CPARAM_TYPE_LIST_0  P1
#define CPARAM_TYPE_LIST_2 CPARAM_TYPE_LIST_1, P2
#define CPARAM_TYPE_LIST_3 CPARAM_TYPE_LIST_2, P3
#define CPARAM_TYPE_LIST_4 CPARAM_TYPE_LIST_3, P4
#define CPARAM_TYPE_LIST_5 CPARAM_TYPE_LIST_4, P5
#define CPARAM_TYPE_LIST_6 CPARAM_TYPE_LIST_5, P6
#define CPARAM_TYPE_LIST_7 CPARAM_TYPE_LIST_6, P7
#define CPARAM_TYPE_LIST_8 CPARAM_TYPE_LIST_7, P8
#define CPARAM_TYPE_LIST_9 CPARAM_TYPE_LIST_8, P9

#define CARG_LIST_0
#define CARG_LIST_1 CARG_LIST_0  ReadToCpp<P1>(L, 2)
#define CARG_LIST_2 CARG_LIST_1, ReadToCpp<P2>(L, 3)
#define CARG_LIST_3 CARG_LIST_2, ReadToCpp<P3>(L, 4)
#define CARG_LIST_4 CARG_LIST_3, ReadToCpp<P4>(L, 5)
#define CARG_LIST_5 CARG_LIST_4, ReadToCpp<P5>(L, 6)
#define CARG_LIST_6 CARG_LIST_5, ReadToCpp<P6>(L, 7)
#define CARG_LIST_7 CARG_LIST_6, ReadToCpp<P7>(L, 8)
#define CARG_LIST_8 CARG_LIST_7, ReadToCpp<P8>(L, 9)
#define CARG_LIST_9 CARG_LIST_8, ReadToCpp<P9>(L, 10)

/* register class method */

struct GenericMethod {
    virtual int Call(lua_State *L) = 0;
};

#define DEF_METHOD_DATA(N) \
template<CTYPENAME_LIST_##N> \
struct MethodData##N : public GenericMethod { \
    typedef R (T::*Method)(CPARAM_TYPE_LIST_##N); \
    Method m_method; \
    MethodData##N(Method method) : m_method(method) {} \
    int Call(lua_State* L) override final { \
        auto object = ReadToCpp<T*>(L, 1); \
        auto result = (object->*m_method)(CARG_LIST_##N); \
        PushToLua<R>(L, result); \
        return 1; \
    } \
};

#define DEF_VOID_METHOD_DATA(N) \
template<CTYPENAME_VOID_LIST_##N> \
struct MethodData##N<CTYPE_VOID_LIST_##N> : public GenericMethod { \
    typedef void (T::*Method)(CPARAM_TYPE_LIST_##N); \
    Method m_method; \
    MethodData##N(Method method) : m_method(method) {} \
    int Call(lua_State* L) override final { \
        auto object = ReadToCpp<T*>(L, 1); \
        (object->*m_method)(CARG_LIST_##N); \
        return 0; \
    } \
};

template<typename T>
inline int ProxyMethodCall(lua_State *L) {
    auto index = lua_upvalueindex(1);
    auto memory = lua_touserdata(L, index);
    auto method = static_cast<GenericMethod*>(memory);
    return method->Call(L);
}

#define DEF_REGISTER_METHOD(N) \
template<CTYPENAME_LIST_##N> \
inline void RegisterMethodToLua(lua_State* L, const char* name, \
    R (T::*method)(CPARAM_TYPE_LIST_##N)) { \
    luaL_getmetatable(L, ClassInfo<T>::Name()); \
    lua_pushstring(L, name); \
    typedef MethodData##N<CTYPE_LIST_##N> MethodData_t; \
    auto memory = lua_newuserdata(L, sizeof(MethodData_t)); \
    new(memory) MethodData_t(method); \
    lua_pushcclosure(L, &ProxyMethodCall<T>, 1); \
    lua_rawset(L, -3); \
    lua_pop(L, 1); \
}

DEF_METHOD_DATA(9)
DEF_METHOD_DATA(8)
DEF_METHOD_DATA(7)
DEF_METHOD_DATA(6)
DEF_METHOD_DATA(5)
DEF_METHOD_DATA(4)
DEF_METHOD_DATA(3)
DEF_METHOD_DATA(2)
DEF_METHOD_DATA(1)
DEF_METHOD_DATA(0)

DEF_VOID_METHOD_DATA(9)
DEF_VOID_METHOD_DATA(8)
DEF_VOID_METHOD_DATA(7)
DEF_VOID_METHOD_DATA(6)
DEF_VOID_METHOD_DATA(5)
DEF_VOID_METHOD_DATA(4)
DEF_VOID_METHOD_DATA(3)
DEF_VOID_METHOD_DATA(2)
DEF_VOID_METHOD_DATA(1)
DEF_VOID_METHOD_DATA(0)

DEF_REGISTER_METHOD(9)
DEF_REGISTER_METHOD(8)
DEF_REGISTER_METHOD(7)
DEF_REGISTER_METHOD(6)
DEF_REGISTER_METHOD(5)
DEF_REGISTER_METHOD(4)
DEF_REGISTER_METHOD(3)
DEF_REGISTER_METHOD(2)
DEF_REGISTER_METHOD(1)
DEF_REGISTER_METHOD(0)

/* register class member */

struct GenericMember {
    virtual int Get(lua_State* L) = 0;
    virtual int Set(lua_State* L) = 0;
};

template<typename T, typename V>
struct MemberData : public GenericMember {
    typedef V T::*Var;
    Var m_var;
    MemberData(Var var) : m_var(var) {}
    int Get(lua_State* L) override final {
        auto object = ReadToCpp<T*>(L, 1);
        auto member = object->*m_var;
        PushToLua<V>(L, member);
        return 1;
    }
    int Set(lua_State* L) override final {
        auto object = ReadToCpp<T*>(L, 1);
        auto& member = object->*m_var;
        member = ReadToCpp<V>(L, 3);
        return 0;
    }
};

template<typename T, typename P>
void RegisterMemberToLua(lua_State* L, const char* name, P T::*member) {
    luaL_getmetatable(L, ClassInfo<T>::Name());
    typedef MemberData<T, P> MemberData_t;
    lua_pushstring(L, name);
    auto memory = lua_newuserdata(L, sizeof(MemberData_t));
    new(memory) MemberData_t(member);
    lua_rawset(L, -3);
    lua_pop(L, 1);
}

/* register class object */

template<typename T>
void RegisterObjectToLua(lua_State* L, const char* name, T* object) {
    typedef UserData<T*> UserData_t;
    auto memory = lua_newuserdata(L, sizeof(UserData_t));
    new(memory) UserData_t(object);
    luaL_getmetatable(L, ClassInfo<T>::Name());
    lua_setmetatable(L, -2);
    lua_setglobal(L, name);
}

/* inherit parent class */

template<typename T, typename P>
void InheritParentToLua(lua_State* L) {
    luaL_getmetatable(L, ClassInfo<T>::Name());
    if (lua_istable(L, -1)) {
        lua_pushstring(L, "__parent");
        luaL_getmetatable(L, ClassInfo<P>::Name());
        lua_rawset(L, -3);
    }
    lua_pop(L, 1);
}

inline void InvokeParent(lua_State *L) {
    lua_pushstring(L, "__parent");
    lua_rawget(L, -2);
    if (lua_istable(L, -1)) {
        lua_pushvalue(L, 2);
        lua_rawget(L, -2);
        if (lua_isnil(L, -1)) {
            lua_remove(L, -1);
            InvokeParent(L);
        }
        lua_remove(L, -2);
    }
}

/* set metatable for class */

template<typename T>
inline int MetaGet(lua_State* L) {
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);
    if (lua_isnil(L, -1)) {
        lua_remove(L, -1);
        InvokeParent(L);
    }
    if (lua_isuserdata(L, -1)) {
        auto memory = lua_touserdata(L, -1);
        auto member = static_cast<GenericMember*>(memory);
        member->Get(L);
        lua_remove(L, -2);
    }
    else if (lua_isfunction(L, -1)) {
        // methods
    }
    else if (lua_isnil(L, -1)) {
        lua_error(L);
    }
    lua_remove(L, -2);
    return 1;
}

template<typename T>
inline int MetaSet(lua_State* L) {
    lua_getmetatable(L, 1);
    lua_pushvalue(L, 2);
    lua_rawget(L, -2);
    if (lua_isnil(L, -1)) {
        lua_remove(L, -1);
        InvokeParent(L);
    }
    if (lua_isuserdata(L, -1)) {
        auto memory = lua_touserdata(L, -1);
        auto member = static_cast<GenericMember*>(memory);
        member->Set(L);
    }
    else if (lua_isnil(L, -1)) {
        lua_remove(L, -1);
        InvokeParent(L);
        if (lua_isnil(L, -1)) {
            lua_pushvalue(L, 2);
            lua_pushvalue(L, 3);
            lua_rawset(L, -4);
        }
    }
    lua_settop(L, 3);
    return 0;
}

template<typename T>
inline int DoGc(lua_State* L) {
    return 0;
}

template<typename T>
void SetMetatable(lua_State* L, const char* className) {
    luaL_newmetatable(L, className);

    lua_pushliteral(L, "__index");
    lua_pushcfunction(L, MetaGet<T>);
    lua_settable(L, -3);

    lua_pushliteral(L, "__newindex");
    lua_pushcfunction(L, MetaSet<T>);
    lua_settable(L, -3);

    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, DoGc<T>);
    lua_settable(L, -3);

    lua_setmetatable(L, -2);
}

/* register class type */

template<typename T>
void RegisterClassToLua(lua_State* L, const char* className) {
    ClassInfo<T>::Name(className);
    lua_newtable(L);
    SetMetatable<T>(L, className);
    lua_setglobal(L, className);
}

DECL_NAMESPACE_METALUA_END

#endif // _METALUA_CLASSDEFINE_HPP_
