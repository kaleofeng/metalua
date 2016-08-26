#include "MetaLua/ClassDefine.hpp"
#include "MetaLua/GlobalDefine.hpp"
#include "MetaLua/LuaTable.hpp"

DECL_NAMESPACE_METALUA_BEGIN

template<>
void ReadToCpp(lua_State* L, int index) {
    lua_remove(L, index);
}

template<>
bool ReadToCpp(lua_State* L, int index) {
    return lua_toboolean(L, index) ? true : false;
}

template<>
const char* ReadToCpp(lua_State* L, int index) {
    return static_cast<const char*>(lua_tostring(L, index));
}

template<>
int8_t ReadToCpp(lua_State* L, int index) {
    return static_cast<int8_t>(lua_tointeger(L, index));
};

template<>
uint8_t ReadToCpp(lua_State* L, int index) {
    return static_cast<uint8_t>(lua_tointeger(L, index));
}

template<>
int16_t ReadToCpp(lua_State* L, int index) {
    return static_cast<int16_t>(lua_tointeger(L, index));
}

template<>
uint16_t ReadToCpp(lua_State* L, int index) {
    return static_cast<uint16_t>(lua_tointeger(L, index));
}

template<>
int32_t ReadToCpp(lua_State* L, int index) {
    return static_cast<int32_t>(lua_tointeger(L, index));
}

template<>
uint32_t ReadToCpp(lua_State* L, int index) {
    return static_cast<uint32_t>(lua_tointeger(L, index));
}

template<>
int64_t ReadToCpp(lua_State* L, int index) {
    return static_cast<int64_t>(lua_tointeger(L, index));
}

template<>
uint64_t ReadToCpp(lua_State* L, int index) {
    return static_cast<uint64_t>(lua_tointeger(L, index));
}

template<>
float ReadToCpp(lua_State* L, int index) {
    return static_cast<float>(lua_tonumber(L, index));
}

template<>
double ReadToCpp(lua_State* L, int index) {
    return static_cast<double>(lua_tonumber(L, index));
}

template<>
LuaTable ReadToCpp(lua_State* L, int index) {
    return LuaTable(L, index);
}


template<>
void PushToLua(lua_State* L, bool data) {
    lua_pushboolean(L, data);
}

template<>
void PushToLua(lua_State* L, char* data) {
    lua_pushstring(L, data);
}

template<>
void PushToLua(lua_State* L, const char* data) {
    lua_pushstring(L, data);
}

template<>
void PushToLua(lua_State* L, int8_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, uint8_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, int16_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, uint16_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, int32_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, uint32_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, int64_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, uint64_t data) {
    lua_pushinteger(L, data);
}

template<>
void PushToLua(lua_State* L, float data) {
    lua_pushnumber(L, data);
}

template<>
void PushToLua(lua_State* L, double data) {
    lua_pushnumber(L, data);
}

template<>
void PushToLua(lua_State* L, LuaTable data) {
    lua_pushvalue(L, data.m_index);
}

DECL_NAMESPACE_METALUA_END