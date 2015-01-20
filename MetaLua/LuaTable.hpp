#ifndef _METALUA_LUATABLE_HPP_
#define _METALUA_LUATABLE_HPP_

#include "MetaLua/BaseDefine.hpp"

DECL_NAMESPACE_METALUA_BEGIN

struct LuaTable {
    LuaTable(lua_State* luaState)
        : m_luaState(luaState) {
        lua_newtable(m_luaState);
        m_index = lua_gettop(m_luaState);
    }

    LuaTable(lua_State* luaState, int index)
        : m_luaState(luaState) {
        if (index < 0) {
            index = index + lua_gettop(m_luaState) + 1;
        }
        m_index = index;
    }

    LuaTable(const LuaTable& other)
        : m_luaState(other.m_luaState)
        , m_index(other.m_index) {}

    ~LuaTable() {}

    template<typename T>
    T Get(const char* key) const {
        lua_getfield(m_luaState, m_index, key);
        return ReadToCpp<T>(m_luaState, -1);
    }

    template<typename T>
    void Set(const char* key, T data) {
        PushToLua<T>(m_luaState, data);
        lua_setfield(m_luaState, m_index, key);
    }

    lua_State* m_luaState;
    int m_index;
};

DECL_NAMESPACE_METALUA_END

#endif // _METALUA_LUATABLE_HPP_
