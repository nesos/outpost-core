
#include "cobs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdint.h>

#include <cobc/utils/cobs.h>

using cobc::utils::Cobs;

static int
l_cobs_encode(lua_State* L)
{
	size_t length;
    const uint8_t* str = reinterpret_cast<const uint8_t*>(lua_tolstring(L, 1, &length));

    // The encoding adds at maximum 1 byte plus and additional byte
    // per 254 bytes of input.
    size_t maxiumLength = Cobs::getMaximumSizeOfEncodedData(length);

    luaL_Buffer b;
    uint8_t* dst = reinterpret_cast<uint8_t*>(luaL_buffinitsize(L, &b, maxiumLength));

    size_t encodedLength = Cobs::encode(str, length, dst, maxiumLength);
    luaL_pushresultsize(&b, encodedLength);

    return 1;
}

static int
l_cobs_decode(lua_State* L)
{
    size_t length;
    const uint8_t* str = reinterpret_cast<const uint8_t*>(lua_tolstring(L, 1, &length));

    luaL_Buffer b;
    uint8_t* dst = reinterpret_cast<uint8_t*>(luaL_buffinitsize(L, &b, length));

    size_t encodedLength = Cobs::decode(str, length, dst);
    luaL_pushresultsize(&b, encodedLength);

    return 1;
}

// ----------------------------------------------------------------------------
static const struct luaL_Reg lib_functions[] = {
	{ "encode", l_cobs_encode },
	{ "decode", l_cobs_decode },
	{ NULL, NULL }
};

// ----------------------------------------------------------------------------
extern "C"
int
luaopen_cobs(lua_State* L)
{
	luaL_newlib(L, lib_functions);
	return 1;
}
