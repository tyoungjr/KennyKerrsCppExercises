#include "Precompiled.h"

using namespace std;
using namespace KennyKerr;

struct connection_handle_traits
{
    using pointer = sqlite3 *;

    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }

    static auto close(pointer value) throw() -> void
    {
        VERIFY_(SQLITE_OK, sqlite3_close(value));
    }
};

using connection_handle = unique_handle<connection_handle_traits>;

struct statement_handle_traits
{
    using pointer = sqlite3_stmt *;

    static auto invalid() throw() -> pointer
    {
        return nullptr;
    }

    static auto close(pointer value) throw() -> void
    {
        VERIFY_(SQLITE_OK, sqlite3_finalize(value));
    }
};

using statement_handle = unique_handle<statement_handle_traits>;

struct sql_exception
{
    int code;
    string message;

    sql_exception(int result, char const * text) :
        code { result },
        message { text }
    {}
};

struct connection
{
    connection_handle handle;

    auto open(char const * filename) -> void
    {
        auto local = connection_handle {};

        auto const result = sqlite3_open(filename,
                                         local.get_address_of());

        if (SQLITE_OK != result)
        {
            throw sql_exception { result, sqlite3_errmsg(local.get()) };
        }

        handle = move(local);
    }
};

auto main() -> int
{
    try
    {
        connection c;

        c.open("X:\\Chickens.db");
    }
    catch (sql_exception const & e)
    {
        TRACE(L"%d %S\n", e.code, e.message.c_str());
    }
}
