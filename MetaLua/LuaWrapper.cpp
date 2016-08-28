#include "LuaWrapper.hpp"

DECL_NAMESPACE_METALUA_BEGIN

LuaWrapper::LuaWrapper()
    : m_luaState(nullptr) {}

LuaWrapper::~LuaWrapper() {}

bool LuaWrapper::Initialize() {
    m_luaState = luaL_newstate();
    if (m_luaState == nullptr) {
        return false;
    }

    G(m_luaState)->ud = &m_userData;
    m_userData.m_luaWrapper = this;

    luaL_openlibs(m_luaState);

    {
        AutoStackRecover asr(m_luaState);
        lua_pushcclosure(m_luaState, luaU_ErrorFunc, 0);
        m_userData.m_errroFunc = luaL_ref(m_luaState, LUA_REGISTRYINDEX);

    }
    return true;
}

void LuaWrapper::Finalize() {
    lua_close(m_luaState);
}

void LuaWrapper::DoScript(const char* file) {
    luaL_dofile(m_luaState, file);
}

DECL_NAMESPACE_METALUA_END

NS_METALUA::LuaWrapper* g_luaWrapper = nullptr;

bool LuaStartup() {
    g_luaWrapper = new NS_METALUA::LuaWrapper();
    return g_luaWrapper->Initialize();
}

void LuaCleanup() {
    g_luaWrapper->Finalize();
    delete g_luaWrapper;
    g_luaWrapper = nullptr;
}