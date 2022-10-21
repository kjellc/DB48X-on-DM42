#ifndef TESTS_H
#define TESTS_H
// ****************************************************************************
//  tests.h                                                       DB48X project
// ****************************************************************************
//
//   File Description:
//
//     Tests for the runtime
//
//     The tests are run by actually sending keystrokes and observing the
//     calculator's state
//
//
//
//
//
// ****************************************************************************
//   (C) 2022 Christophe de Dinechin <christophe@dinechin.org>
//   This software is licensed under the terms outlined in LICENSE.txt
// ****************************************************************************
//   This file is part of DB48X.
//
//   DB48X is free software: you can redistribute it and/or modify
//   it under the terms outlined in the LICENSE.txt file
//
//   DB48X is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// ****************************************************************************

#include "dmcp.h"
#include "object.h"
#include "runtime.h"

#include <vector>
#include <string>
#include <sstream>

struct tests
// ----------------------------------------------------------------------------
//   Run a series of tests
// ----------------------------------------------------------------------------
{
    tests()
        : tname(), sname(), tindex(), sindex(), cindex(), count(),
          ok(), longpress(), failures()
    { }

    // Run all tests
    void run();

    // Individual test categories
    void shift_logic();
    void keyboard_entry();
    void data_types();
    void arithmetic();

    enum key
    {
        RELEASE    = 0,

        SIGMA      = KEY_SIGMA,
        INV        = KEY_INV,
        SQRT       = KEY_SQRT,
        LOG        = KEY_LOG,
        LN         = KEY_LN,
        XEQ        = KEY_XEQ,
        STO        = KEY_STO,
        RCL        = KEY_RCL,
        RDN        = KEY_RDN,
        SIN        = KEY_SIN,
        COS        = KEY_COS,
        TAN        = KEY_TAN,
        ENTER      = KEY_ENTER,
        SWAP       = KEY_SWAP,
        CHS        = KEY_CHS,
        EEX        = KEY_E,
        BSP        = KEY_BSP,
        UP         = KEY_UP,
        KEY7       = KEY_7,
        KEY8       = KEY_8,
        KEY9       = KEY_9,
        DIV        = KEY_DIV,
        DOWN       = KEY_DOWN,
        KEY4       = KEY_4,
        KEY5       = KEY_5,
        KEY6       = KEY_6,
        MUL        = KEY_MUL,
        SHIFT      = KEY_SHIFT,
        KEY1       = KEY_1,
        KEY2       = KEY_2,
        KEY3       = KEY_3,
        SUB        = KEY_SUB,
        EXIT       = KEY_EXIT,
        KEY0       = KEY_0,
        DOT        = KEY_DOT,
        RUNSTOP    = KEY_RUN,
        ADD        = KEY_ADD,
        F1         = KEY_F1,
        F2         = KEY_F2,
        F3         = KEY_F3,
        F4         = KEY_F4,
        F5         = KEY_F5,
        F6         = KEY_F6,
        SCREENSHOT = KEY_SCREENSHOT,
        SH_UP      = KEY_SH_UP,
        SH_DOWN    = KEY_SH_DOWN,

        A          = KEY_SIGMA,
        B          = KEY_INV,
        C          = KEY_SQRT,
        D          = KEY_LOG,
        E          = KEY_LN,
        F          = KEY_XEQ,
        G          = KEY_STO,
        H          = KEY_RCL,
        I          = KEY_RDN,
        J          = KEY_SIN,
        K          = KEY_COS,
        L          = KEY_TAN,
        M          = KEY_SWAP,
        N          = KEY_CHS,
        O          = KEY_E,
        P          = KEY_7,
        Q          = KEY_8,
        R          = KEY_9,
        S          = KEY_DIV,
        T          = KEY_4,
        U          = KEY_5,
        V          = KEY_6,
        W          = KEY_MUL,
        X          = KEY_1,
        Y          = KEY_2,
        Z          = KEY_3,
        UNDER      = KEY_SUB,
        COLON      = KEY_0,
        COMMA      = KEY_DOT,
        SPACE      = KEY_RUN,
        QUESTION   = KEY_ADD,

        // Special stuff
        ALPHA      = 100,       // Set alpha
        LOWERCASE  = 101,       // Set lowercase
        LONGPRESS,              // Force long press
        CLEAR,                  // Clear the editor
        NOKEYS,                 // Wait until keys buffer is empty
        REFRESH,                // Wait until there is a screen refresh
    };

  protected:
    struct failure
    {
        failure(cstring     test,
                cstring     step,
                std::string explanation,
                uint        ti,
                uint        si,
                int         ci)
            : test(test),
              step(step),
              explanation(explanation),
              tindex(ti),
              sindex(si),
              cindex(ci)
        {
        }
        cstring     test;
        cstring     step;
        std::string explanation;
        uint        tindex;
        uint        sindex;
        uint        cindex;
    };

protected:
    struct WAIT
    {
        WAIT(uint ms): delay(ms) {}
        uint delay;
    };

    // Naming / identifying tests
    tests &begin(cstring name);
    tests &step(cstring name);
    tests &check(bool test);
    tests &fail();
    tests &summary();
    tests &show(failure &f, cstring &last);
    tests &show(failure &f);

    // Used to build the tests
    tests &test(key k, bool release = true);
    tests &test(uint value);
    tests &test(int value);
    tests &test(ularge value);
    tests &test(large value);
    tests &test(char c);
    tests &test(cstring alpha);
    tests &test(WAIT delay);

    template <typename First, typename... Args>
    tests &test(First first, Args... args)
    {
        return test(first).test(args...);
    }

    tests &clear();
    tests &nokeys();
    tests &refreshed();
    tests &ready();
    tests &shifts(bool shift, bool xshift, bool alpha, bool lowercase);
    tests &wait(uint ms);
    tests &expect(cstring output);
    tests &expect(int output);
    tests &expect(uint output);
    tests &expect(large output);
    tests &expect(ularge output);
    tests &match(cstring regexp);
    tests &type(object::id ty);
    tests &shift(bool s);
    tests &xshift(bool x);
    tests &alpha(bool a);
    tests &lower(bool l);
    tests &editing();
    tests &editing(size_t length);
    tests &editor(cstring text);
    tests &cursor(size_t csr);
    tests &error(cstring msg);
    tests &noerr()      { return error(nullptr); }
    tests &command(cstring msg);
    tests &source(cstring msg);

    template<typename ...Args>
    tests &explain(Args... args)
    {
        if (explanation.length())
            explanation += "\n---- ";
        return explain_more(args...);
    }

    template<typename T>
    tests &explain_more(T t)
    {
        std::ostringstream out;
        out << t;
        explanation += out.str();
        return *this;
    }

    template <typename T, typename ...Args>
    tests &explain_more(T t, Args... args)
    {
        explain_more(t);
        return explain_more(args...);
    }

    template<typename ...Args>
    tests &check(bool test, Args... args)
    {
        if (!test)
            explain(args...);
        return check(test);
    }


protected:
    cstring              tname;
    cstring              sname;
    uint                 tindex;
    uint                 sindex;
    uint                 cindex;
    uint                 count;
    int                  lcd_update;
    bool                 ok;
    bool                 longpress;
    std::vector<failure> failures;
    std::string          explanation;
};

#endif // TESTS_H