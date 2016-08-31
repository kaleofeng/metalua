#ifndef _METALUA_BASEDEFINE_HPP_
#define _METALUA_BASEDEFINE_HPP_

#include <cstdint>
#include <cstring>

#include "Lua/lua.hpp"

#define NS_METALUA                      metalua
#define DECL_NAMESPACE_METALUA_BEGIN    namespace NS_METALUA {
#define DECL_NAMESPACE_METALUA_END      }
#define USING_NAMESPACE_METALUA         using namespace NS_METALUA;

DECL_NAMESPACE_METALUA_BEGIN

struct LuaTable;

template<typename T>
struct UserData {

    T m_object;

    UserData(T object) : m_object(object) {}
    ~UserData() {}
};

/* read a value from lua to cpp */

template<typename T>
struct LuaToCpp {
    static T ConvertUserdata(lua_State* L, int index){
        typedef UserData<T> UserData_t;
        auto memory = lua_touserdata(L, index);
        auto userData = static_cast<UserData_t*>(memory);
        return userData->m_object;
    }
};

template<typename T>
inline T ReadToCpp(lua_State* L, int index) {
    if(!lua_isuserdata(L, index)) {
        lua_pushstring(L, "This arg is not a userdata.\n");
        lua_error(L);
    }
    return LuaToCpp<T>::ConvertUserdata(L, index);
}

template<> void ReadToCpp(lua_State* L, int index);
template<> bool ReadToCpp(lua_State* L, int index);
template<> const char* ReadToCpp(lua_State* L, int index);
template<> int8_t ReadToCpp(lua_State* L, int index);
template<> uint8_t ReadToCpp(lua_State* L, int index);
template<> int16_t ReadToCpp(lua_State* L, int index);
template<> uint16_t ReadToCpp(lua_State* L, int index);
template<> int32_t ReadToCpp(lua_State* L, int index);
template<> uint32_t ReadToCpp(lua_State* L, int index);
template<> int64_t ReadToCpp(lua_State* L, int index);
template<> uint64_t ReadToCpp(lua_State* L, int index);
template<> float ReadToCpp(lua_State* L, int index);
template<> double ReadToCpp(lua_State* L, int index);
template<> LuaTable ReadToCpp(lua_State* L, int index);

/* push a value from cpp to lua */

template<typename T>
struct ClassInfo {
    static const char* Name(const char* name = nullptr) {
        static char s_name[32] = "";
        if (name != nullptr) {
            ::strcpy(s_name, name);
        }
        return s_name;
    }
};

template<typename T>
struct CppToLua {
    static void ConvertUserdata(lua_State* L, T object){
        auto memory = lua_newuserdata(L, sizeof(T));
        new(memory) UserData<T>(object);
        luaL_getmetatable(L, ClassInfo<T>::Name());
        lua_setmetatable(L, -2);
    }
};

template<typename T>
struct CppToLua<const T> {
    static void ConvertUserdata(lua_State* L, const T object) {
        auto memory = lua_newuserdata(L, sizeof(T));
        new(memory) UserData<const T>(object);
        luaL_getmetatable(L, ClassInfo<T>::Name());
        lua_setmetatable(L, -2);
    }
};

template<typename T>
struct CppToLua<T*> {
    static void ConvertUserdata(lua_State* L, T* object) {
        auto memory = lua_newuserdata(L, sizeof(T));
        new(memory) UserData<T*>(object);
        luaL_getmetatable(L, ClassInfo<T>::Name());
        lua_setmetatable(L, -2);
    }
};

template<typename T>
struct CppToLua<const T*> {
    static void ConvertUserdata(lua_State* L, const T* object) {
        auto memory = lua_newuserdata(L, sizeof(T));
        new(memory) UserData<const T*>(object);
        luaL_getmetatable(L, ClassInfo<T>::Name());
        lua_setmetatable(L, -2);
    }
};

template<typename T>
inline void PushToLua(lua_State* L, T data) {
    CppToLua<T>::ConvertUserdata(L, data);
};

template<> void PushToLua(lua_State* L, bool data);
template<> void PushToLua(lua_State* L, char* data);
template<> void PushToLua(lua_State* L, const char* data);
template<> void PushToLua(lua_State* L, int8_t data);
template<> void PushToLua(lua_State* L, uint8_t data);
template<> void PushToLua(lua_State* L, int16_t data);
template<> void PushToLua(lua_State* L, uint16_t data);
template<> void PushToLua(lua_State* L, int32_t data);
template<> void PushToLua(lua_State* L, uint32_t data);
template<> void PushToLua(lua_State* L, int64_t data);
template<> void PushToLua(lua_State* L, uint64_t data);
template<> void PushToLua(lua_State* L, float data);
template<> void PushToLua(lua_State* L, double data);
template<> void PushToLua(lua_State* L, LuaTable data);

inline void VaradicPushToLua(lua_State* L) {

}

template<typename T, typename... Args>
inline void VaradicPushToLua(lua_State* L, T t, Args... args) {
    PushToLua<T>(L, t);
    VaradicPushToLua(L, args...);
}

/* user functions */

struct StackAutoRecover {

    lua_State* m_luaState{ nullptr };
    int m_top{ 0 };

    StackAutoRecover(lua_State* luaState)
        : m_luaState(luaState) {
        m_top = lua_gettop(m_luaState);
    }

    ~StackAutoRecover() {
        lua_settop(m_luaState, m_top);
    }
};

int luaU_ErrorFunction(lua_State* L);

int GetErrorFunction(lua_State* L);

DECL_NAMESPACE_METALUA_END

#endif // _METALUA_BASEDEFINE_HPP_
