
#ifndef _JIMIC_MATH_INT_LOG10_H_
#define _JIMIC_MATH_INT_LOG10_H_

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "jimic/core/jimic_def.h"

#ifdef __cplusplus
extern "C" {
#endif

JMC_DECLARE_NONSTD(unsigned int)
jmc_uint_log10_sys(uint32_t val);

JMC_DECLARE_NONSTD(unsigned int)
jmc_uint_log10_fast(uint32_t val);

JMC_DECLARE_NONSTD(unsigned int)
jmc_uint_log10(uint32_t val);

#ifdef _WIN64

JMC_DECLARE_NONSTD(unsigned int)
jmc_uint64_log10(uint64_t val);

#endif

#ifdef __cplusplus
}
#endif

#endif  /* !_JIMIC_MATH_INT_LOG10_H_ */