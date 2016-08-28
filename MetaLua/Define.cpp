#include "MetaLua/ClassDefine.hpp"
#include "MetaLua/GlobalDefine.hpp"
#include "MetaLua/LuaTable.hpp"
#include "MetaLua/LuaWrapper.hpp"

#include <string>

DECL_NAMESPACE_METALUA_BEGIN

template<>
void ReadToCpp(lua_State* L, int index) {
    lua_remove(L, index);
}

template<>
bool ReadToCpp(lua_State* L, int index) {
    if (lua_isboolean(L, index)) {
        return lua_toboolean(L, index) ? true : false;
    }
    return false;
}

template<>
const char* ReadToCpp(lua_State* L, int index) {
    if (lua_isstring(L, index)) {
        return static_cast<const char*>(lua_tostring(L, index));
    }
    return "";
}

template<>
int8_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<int8_t>(lua_tointeger(L, index));
    }
    return 0;
};

template<>
uint8_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<uint8_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
int16_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<int16_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
uint16_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<uint16_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
int32_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<int32_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
uint32_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<uint32_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
int64_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<int64_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
uint64_t ReadToCpp(lua_State* L, int index) {
    if (lua_isinteger(L, index)) {
        return static_cast<uint64_t>(lua_tointeger(L, index));
    }
    return 0;
}

template<>
float ReadToCpp(lua_State* L, int index) {
    if (lua_isnumber(L, index)) {
        return static_cast<float>(lua_tonumber(L, index));
    }
    return 0;
}

template<>
double ReadToCpp(lua_State* L, int index) {
    if (lua_isnumber(L, index)) {
        return static_cast<double>(lua_tonumber(L, index));
    }
    return 0;
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

static int GetDebugInfo(lua_Debug& dbg, char* buf, int len) {
    int n = 0;
    char* pc = buf;

    {
        int l = snprintf(pc, len, "\t");
        pc += l;
        n += l;
    }

    if (dbg.name != nullptr) {
        int l = snprintf(pc, len, "%s(%s)", dbg.name, dbg.what ? dbg.what : "");
        pc += l;
        n += l;
    }

    if (dbg.source != nullptr) {
        int l = snprintf(pc, len, "%s%s", (n ? " at " : ""), dbg.source);
        pc += l;
        n += l;
    }

    if (dbg.currentline > 0) {
        int l = snprintf(pc, len, "%s%d", n ? " line:" : "", dbg.currentline);
        pc += l;
        n += l;
    }

    if (n != 0) {
        int l = snprintf(pc, len, "\n");
        pc += l;
        n += l;
    }

    return n;
}

int luaU_ErrorFunc(lua_State* L) {
    int n = 0;
    std::string str;
    char buf[1024]{ '\0' };

    str = lua_tostring(L, -1);
    str.append("\n");

    int level = 0;
    lua_Debug luaDebug;
    while (lua_getstack(L, level++, &luaDebug)) {
        if (!lua_getinfo(L, "nSl", &luaDebug)) break;
        n += GetDebugInfo(luaDebug, &buf[n], 1024 - n);
    }

    str.append(buf);

    printf("Lua Error: %s\n", str.c_str());
    return LUA_OK;
}

int GetErrorFunc(lua_State* L) {
    const auto userData = reinterpret_cast<LuaWrapper::UserData*>(G(L)->ud);
    lua_rawgeti(L, LUA_REGISTRYINDEX, userData->m_errroFunc);
    return lua_gettop(L);
}

DECL_NAMESPACE_METALUA_END