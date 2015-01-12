#include "LuaWrapper.hpp"

DECL_NAMESPACE_METALUA_BEGIN

LuaWrapper::LuaWrapper() {}

LuaWrapper::~LuaWrapper() {}

bool LuaWrapper::Init() {
    m_luaState = luaL_newstate();
    if (m_luaState == nullptr) {
        return false;
    }

    luaL_openlibs(m_luaState);
    return true;
}

void LuaWrapper::Clear() {
    lua_close(m_luaState);
}

void LuaWrapper::DoScript(const char* file) {
    luaL_dofile(m_luaState, file);
}

DECL_NAMESPACE_METALUA_END

NS_METALUA::LuaWrapper* g_luaWrapper = nullptr;

bool LuaInit() {
    g_luaWrapper = new NS_METALUA::LuaWrapper();
    return g_luaWrapper->Init();
}

void LuaClear() {
    g_luaWrapper->Clear();
    delete g_luaWrapper;
    g_luaWrapper = nullptr;
}