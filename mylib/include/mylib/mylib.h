#ifndef MYLIB_H
#define MYLIB_H

#include <mylib/mylib_export.h>

#ifdef __cplusplus
extern "C" {
#endif

int MYLIB_EXPORT mylib_sum(const int n, const int* src);

#ifdef __cplusplus
}
#endif

#endif // MYLIB_H
