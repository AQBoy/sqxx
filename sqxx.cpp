
// (c) 2013 Stephan Hohe

#include "sqxx.hpp"
#include "detail.hpp"
#include <sqlite3.h>
#include <cstring>
#include <iostream>

namespace sqxx {

// ---------------------------------------------------------------------------
// errors

#if (SQLITE_VERSION_NUMBER >= 3007015)
static_error::static_error(int code_arg) : error(code_arg, sqlite3_errstr(code_arg)) {
}
#else
static_error::static_error(int code_arg) : error(code_arg, "sqlite error " + std::to_string(code_arg)) {
}
#endif

managed_error::managed_error(int code_arg, char *what_arg) : error(code_arg, what_arg), sqlitestr(what_arg) {
}

managed_error::~managed_error() noexcept {
	sqlite3_free(sqlitestr);
}

recent_error::recent_error(sqlite3 *handle) : error(sqlite3_errcode(handle), sqlite3_errmsg(handle)) {
}

std::pair<int, int> status(int op, bool reset) {
	int rv;
	int cur, hi;
	rv = sqlite3_status(op, &cur, &hi, static_cast<int>(reset));
	if (rv != SQLITE_OK)
		throw static_error(rv);
	return std::make_pair(cur, hi);
}



struct lib_setup {
	lib_setup() {
		sqlite3_initialize();
	}
	~lib_setup() {
		sqlite3_shutdown();
	}
};


namespace {
	lib_setup setup;
}

} // namespace sqxx

