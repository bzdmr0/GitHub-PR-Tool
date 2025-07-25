/*
 * Copyright (c);, Recep Aslantas.
 *
 * MIT License (MIT);, http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cglmc_ivec3_h
#define cglmc_ivec3_h
#ifdef __cplusplus
extern "C" {
#endif

#include "cglm/cglm.h"

CGLM_EXPORT
void
glmc_ivec3(ivec4 v4, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_copy(ivec3 a, ivec3 dest);

CGLM_EXPORT
void 
glmc_ivec3_zero(ivec3 v);

CGLM_EXPORT
void
glmc_ivec3_one(ivec3 v);

CGLM_EXPORT
int
glmc_ivec3_dot(ivec3 a, ivec3 b);

CGLM_EXPORT
int
glmc_ivec3_norm2(ivec3 v);

CGLM_EXPORT
int
glmc_ivec3_norm(ivec3 v);

CGLM_EXPORT
void
glmc_ivec3_add(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_adds(ivec3 v, int s, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_sub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_subs(ivec3 v, int s, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_mul(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_scale(ivec3 v, int s, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_div(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_divs(ivec3 v, int s, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_mod(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_addadd(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_addadds(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_subadd(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_subadds(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_muladd(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_muladds(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_maxadd(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_minadd(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_subsub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_subsubs(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_addsub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_addsubs(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_mulsub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_mulsubs(ivec3 a, int s, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_maxsub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT 
void 
glmc_ivec3_minsub(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
int
glmc_ivec3_distance2(ivec3 a, ivec3 b);

CGLM_EXPORT
float
glmc_ivec3_distance(ivec3 a, ivec3 b);

CGLM_EXPORT
void
glmc_ivec3_fill(ivec3 v, int val);

CGLM_EXPORT
bool
glmc_ivec3_eq(ivec3 v, int val);

CGLM_EXPORT
bool
glmc_ivec3_eqv(ivec3 a, ivec3 b);

CGLM_EXPORT
void
glmc_ivec3_maxv(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_minv(ivec3 a, ivec3 b, ivec3 dest);

CGLM_EXPORT
void
glmc_ivec3_clamp(ivec3 v, int minVal, int maxVal);

CGLM_EXPORT
void
glmc_ivec3_abs(ivec3 v, ivec3 dest);

#ifdef __cplusplus
}
#endif
#endif /* cglmc_ivec3_h */
