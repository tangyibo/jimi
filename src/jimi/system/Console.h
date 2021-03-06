
#ifndef _JIMI_SYSTEM_CONSOLE_H_
#define _JIMI_SYSTEM_CONSOLE_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimi/basic/stddef.h"
#include "jimi/internal/NonCopyable.h"

#include "jimic/system/getchar.h"
#include "jimic/system/console.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef _MSC_VER
#include <conio.h>
#endif // _MSC_VER

namespace jimi {

class Console : public internal::NonCopyable
{
public:
#if defined(JIMI_HAS_DEFAULTED_FUNCTIONS) && (JIMI_HAS_DEFAULTED_FUNCTIONS != 0)
    //! Allow default construction
    JIMI_CONSTEXPR Console() = default;
    ~Console() = default;
#else
    //! Allow default construction
    Console() {}
    ~Console() {}
#endif

    static void Write(const char *fmt, ...) {
        va_list arg_list;
        va_start(arg_list, fmt);
        vprintf(fmt, arg_list);
        va_end(arg_list);
    }

    static void WriteLine(const char *fmt = NULL, ...) {
        va_list arg_list;
        if (fmt != NULL) {
            va_start(arg_list, fmt);
            vprintf(fmt, arg_list);
            va_end(arg_list);
        }
        printf("\n");
    }

    static int ReadKey(bool enabledCpuWarmup = false, bool displayTips = true,
                       bool echoInput = false, bool newLine = false) {
        int keyCode;
        if (displayTips) {
            printf("Press any key to continue ...");

            keyCode = jimi_getch();
            printf("\n");
        }
        else {
            keyCode = jimi_getch();
            if (echoInput) {
                if (keyCode != EOF)
                    printf("%c", (char)keyCode);
                else
                    printf("EOF: (%d)", keyCode);
            }
        }

        if (newLine) {
            printf("\n");
        }

        // After call jimi_getch(), warm up the CPU again, at least 500 ms.
        if (enabledCpuWarmup) {
            jimi_cpu_warmup(500);
        }
        return keyCode;
    }

    static int ReadKeyLine(bool enabledCpuWarmup = false, bool displayTips = true,
                           bool echoInput = false) {
        return ReadKey(false, displayTips, echoInput, true);
    }

    static int Read() {
        int keyCode = 0;
        // TODO: Read the inputs.
        return keyCode;
    }
};

static class Console Console;

}  /* namespace jimi */

typedef class jimi::Console jimiConsole;

static class jimi::Console jmConsole;

#endif  /* !_JIMI_SYSTEM_CONSOLE_H_ */
