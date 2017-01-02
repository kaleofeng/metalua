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
        StackAutoRecover asr(m_luaState);
        lua_pushcclosure(m_luaState, luaU_ErrorFunction, 0);
        m_userData.m_errroFunc = luaL_ref(m_luaState, LUA_REGISTRYINDEX);

    }
    return true;
}

void LuaWrapper::Finalize() {
    lua_close(m_luaState);
}

void LuaWrapper::AddPackagePath(const char* path) {
    StackAutoRecover sar(m_luaState);
    lua_getglobal(m_luaState, "package");
    lua_getfield(m_luaState, -1, "path");
    lua_pushfstring(m_luaState, "%s;%s/?.lua", lua_tostring(m_luaState, -1), path);
    lua_setfield(m_luaState, -3, "path");
}

void LuaWrapper::DoScript(const char* file) {
    StackAutoRecover sar(m_luaState);
    const auto errFunc = GetErrorFunction(m_luaState);

    const auto result = luaL_loadfile(m_luaState, file);
    if (result == LUA_OK) {
        lua_pcall(m_luaState, 0, LUA_MULTRET, errFunc);
    }
}

void LuaWrapper::DoString(const char* text) {
    StackAutoRecover sar(m_luaState);
    const auto errFunc = GetErrorFunction(m_luaState);

    const auto result = luaL_loadstring(m_luaState, text);
    if (result == LUA_OK) {
        lua_pcall(m_luaState, 0, LUA_MULTRET, errFunc);
    }
}

DECL_NAMESPACE_METALUA_END

thread_local NS_METALUA::LuaWrapper* g_luaWrapper = nullptr;

bool LuaStartup() {
    g_luaWrapper = new NS_METALUA::LuaWrapper();
    return g_luaWrapper->Initialize();
}

void LuaCleanup() {
    g_luaWrapper->Finalize();
    delete g_luaWrapper;
    g_luaWrapper = nullptr;
}