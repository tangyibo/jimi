
#ifndef _JIMIC_CORE_CONFIG_H_
#define _JIMIC_CORE_CONFIG_H_

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
#include <jimic/core/win32/jimic_config.h>
#elif defined(__linux__) || defined(__LINUX__)
#include <jimic/core/linux/jimic_config.h>
#elif defined(__MINGW__) || defined(__MINGW32__) || defined(__MINGW64__)
#include <jimic/core/mingw/jimic_config.h>
#else
#include <jimic/core/default/jimic_config.h>
#endif

#endif  /* !_JIMIC_CORE_CONFIG_H_ */
