//
//  test.hpp
//  DB_TABLE
//
//  Created by Galambos Mihály on 2017. 11. 27..
//  Copyright © 2017. Galambos Mihály. All rights reserved.
//

#ifndef test_h
#define test_h

#include <string>
#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>

struct test_controller {
    bool enabled;
    std::string label;
    int points;
    bool ok;
    bool run;
    int &out;
    test_controller(bool enabled, std::string&& label, int points, int &out)
    : enabled(enabled)
    , label(std::move(label))
    , points(points)
    , ok(true)
    , run(true)
    , out(out)
    {
        if (enabled) {
            std::cout << ">>> Test: " << this->label << " (" << points << ")" << std::endl;
        }
    }
    ~test_controller() {
        if (enabled) {
            out += ok ? points : 0;
            std::cout << ">>> Result: " << (ok ? "PASS" : "FAIL") << std::endl;
        }
    }
};

#define TEST(label, points) \
for (test_controller cntr(++counter == test_no || test_no == 0, label, points, total); \
cntr.run; cntr.run = false)

#define CHECK(code) \
if (cntr.ok) { \
try { \
code; \
} catch (...) { \
cntr.ok = false; \
std::cout << ">>> unexpected exception" << std::endl; \
} \
if (!cntr.ok) { \
std::cout << ">>> " << #code << " failed at " << __LINE__ << std::endl; \
} \
}

#define CHECK_EQ(val, expr) \
if (cntr.ok) { \
try { \
cntr.ok = val == (expr); \
} catch (...) { \
cntr.ok = false; \
std::cout << ">>> unexpected exception" << std::endl; \
} \
if (!cntr.ok) { \
std::cout << ">>> " << #val << " == " << #expr << " failed at " << __LINE__ << std::endl; \
} \
}

#define CHECK_EXC(exc, code) \
if (cntr.ok) { \
try { \
code; \
cntr.ok = false; \
std::cout << ">>> expected exception" << std::endl; \
} catch (exc) { \
} catch (...) { \
std::cout << ">>> unexpected exception" << std::endl; \
cntr.ok = false; \
} \
if (!cntr.ok) { \
std::cout << #code << " failed at" << __LINE__ << std::endl; \
} \
}

#endif /* test_h */
