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
    UserData(T object) : m_object(object) {}
    ~UserData() {}

    T m_object;
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
T ReadToCpp(lua_State* L, int index) {
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
class ClassInfo {
public:
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
void PushToLua(lua_State* L, T data) {
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

template<typename T, typename... Args>
void VaradicPushToLua(lua_State* L, T t, Args... args) {
    PushToLua<T>(L, t);
    VaradicPushToLua(L, args...);
}

template<typename T>
void VaradicPushToLua(lua_State* L, T t) {
    PushToLua<T>(L, t);
}

DECL_NAMESPACE_METALUA_END

#define TYPENAME_LIST_0 typename R, typename T
#define TYPENAME_LIST_1 TYPENAME_LIST_0, typename P1
#define TYPENAME_LIST_2 TYPENAME_LIST_1, typename P2
#define TYPENAME_LIST_3 TYPENAME_LIST_2, typename P3
#define TYPENAME_LIST_4 TYPENAME_LIST_3, typename P4
#define TYPENAME_LIST_5 TYPENAME_LIST_4, typename P5
#define TYPENAME_LIST_6 TYPENAME_LIST_5, typename P6
#define TYPENAME_LIST_7 TYPENAME_LIST_6, typename P7
#define TYPENAME_LIST_8 TYPENAME_LIST_7, typename P8
#define TYPENAME_LIST_9 TYPENAME_LIST_8, typename P9

#define TYPENAME_VOID_LIST_0 typename T
#define TYPENAME_VOID_LIST_1 TYPENAME_VOID_LIST_0, typename P1
#define TYPENAME_VOID_LIST_2 TYPENAME_VOID_LIST_1, typename P2
#define TYPENAME_VOID_LIST_3 TYPENAME_VOID_LIST_2, typename P3
#define TYPENAME_VOID_LIST_4 TYPENAME_VOID_LIST_3, typename P4
#define TYPENAME_VOID_LIST_5 TYPENAME_VOID_LIST_4, typename P5
#define TYPENAME_VOID_LIST_6 TYPENAME_VOID_LIST_5, typename P6
#define TYPENAME_VOID_LIST_7 TYPENAME_VOID_LIST_6, typename P7
#define TYPENAME_VOID_LIST_8 TYPENAME_VOID_LIST_7, typename P8
#define TYPENAME_VOID_LIST_9 TYPENAME_VOID_LIST_8, typename P9

#define TYPE_LIST_0 R, T
#define TYPE_LIST_1 TYPE_LIST_0, P1
#define TYPE_LIST_2 TYPE_LIST_1, P2
#define TYPE_LIST_3 TYPE_LIST_2, P3
#define TYPE_LIST_4 TYPE_LIST_3, P4
#define TYPE_LIST_5 TYPE_LIST_4, P5
#define TYPE_LIST_6 TYPE_LIST_5, P6
#define TYPE_LIST_7 TYPE_LIST_6, P7
#define TYPE_LIST_8 TYPE_LIST_7, P8
#define TYPE_LIST_9 TYPE_LIST_8, P9

#define TYPE_VOID_LIST_0 T
#define TYPE_VOID_LIST_1 TYPE_VOID_LIST_0, P1
#define TYPE_VOID_LIST_2 TYPE_VOID_LIST_1, P2
#define TYPE_VOID_LIST_3 TYPE_VOID_LIST_2, P3
#define TYPE_VOID_LIST_4 TYPE_VOID_LIST_3, P4
#define TYPE_VOID_LIST_5 TYPE_VOID_LIST_4, P5
#define TYPE_VOID_LIST_6 TYPE_VOID_LIST_5, P6
#define TYPE_VOID_LIST_7 TYPE_VOID_LIST_6, P7
#define TYPE_VOID_LIST_8 TYPE_VOID_LIST_7, P8
#define TYPE_VOID_LIST_9 TYPE_VOID_LIST_8, P9

#define PARAM_TYPE_LIST_0
#define PARAM_TYPE_LIST_1 PARAM_TYPE_LIST_0  P1
#define PARAM_TYPE_LIST_2 PARAM_TYPE_LIST_1, P2
#define PARAM_TYPE_LIST_3 PARAM_TYPE_LIST_2, P3
#define PARAM_TYPE_LIST_4 PARAM_TYPE_LIST_3, P4
#define PARAM_TYPE_LIST_5 PARAM_TYPE_LIST_4, P5
#define PARAM_TYPE_LIST_6 PARAM_TYPE_LIST_5, P6
#define PARAM_TYPE_LIST_7 PARAM_TYPE_LIST_6, P7
#define PARAM_TYPE_LIST_8 PARAM_TYPE_LIST_7, P8
#define PARAM_TYPE_LIST_9 PARAM_TYPE_LIST_8, P9

#define ARG_LIST_0
#define ARG_LIST_1 ARG_LIST_0  ReadToCpp<P1>(L, 2)
#define ARG_LIST_2 ARG_LIST_1, ReadToCpp<P2>(L, 3)
#define ARG_LIST_3 ARG_LIST_2, ReadToCpp<P3>(L, 4)
#define ARG_LIST_4 ARG_LIST_3, ReadToCpp<P4>(L, 5)
#define ARG_LIST_5 ARG_LIST_4, ReadToCpp<P5>(L, 6)
#define ARG_LIST_6 ARG_LIST_5, ReadToCpp<P6>(L, 7)
#define ARG_LIST_7 ARG_LIST_6, ReadToCpp<P7>(L, 8)
#define ARG_LIST_8 ARG_LIST_7, ReadToCpp<P8>(L, 9)
#define ARG_LIST_9 ARG_LIST_8, ReadToCpp<P9>(L, 10)

#endif // _METALUA_BASEDEFINE_HPP_
