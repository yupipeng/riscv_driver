
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

void lltest(lua_State *L)
{
    printf("lltest called\r\n");
}
void lua_test(void)
{
    // lua_State *L = lua_load();
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "lltest", lltest);

    luaL_dostring(L, "lltest()");
    luaL_dostring(L, "print('Hello World!')");
    luaL_dostring(L, "for i = 1, 9 do for j = 1, i do io.write(j, '*', i, '=', i*j, '\\t') end io.write('\\n') end");

    lua_close(L);
}
