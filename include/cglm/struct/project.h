/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef cglms_projects_h
#define cglms_projects_h

#include "cglm/common.h"
#include "cglm/types-struct.h"
#include "cglm/project.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

#ifndef CGLM_CLIPSPACE_INCLUDE_ALL
#  if CGLM_CONFIG_CLIP_CONTROL & CGLM_CLIP_CONTROL_ZO_BIT
#    include "clipspace/project_zo.h"
#  elif CGLM_CONFIG_CLIP_CONTROL & CGLM_CLIP_CONTROL_NO_BIT
#    include "cglm/struct/clipspace/project_no.h"
#  endif
#else
#  include "clipspace/project_zo.h"
#  include "clipspace/project_no.h"
#endif

/*!
 * @brief maps the specified viewport coordinates into specified space [1]
 *        the matrix should contain projection matrix.
 *
 * if you don't have ( and don't want to have ) an inverse matrix then use
 * glm_unproject version. You may use existing inverse of matrix in somewhere
 * else, this is why glm_unprojecti exists to save save inversion cost
 *
 * [1] space:
 *  1- if m = invProj:     View Space
 *  2- if m = invViewProj: World Space
 *  3- if m = invMVP:      Object Space
 *
 * You probably want to map the coordinates into object space
 * so use invMVP as m
 *
 * Computing viewProj:
 *   glm_mat4_mul(proj, view, viewProj);
 *   glm_mat4_mul(viewProj, model, MVP);
 *   glm_mat4_inv(viewProj, invMVP);
 *
 * @param[in]  pos      point/position in viewport coordinates
 * @param[in]  invMat   matrix (see brief)
 * @param[in]  vp       viewport as [x, y, width, height]
 * @returns             unprojected coordinates
 */
CGLM_INLINE
vec3s
glms_unprojecti(vec3s pos, mat4s invMat, vec4s vp) {
  vec3s r;
  glm_unprojecti(pos.raw, invMat.raw, vp.raw, r.raw);
  return r;
}

/*!
 * @brief maps the specified viewport coordinates into specified space [1]
 *        the matrix should contain projection matrix.
 *
 * this is same as glm_unprojecti except this function get inverse matrix for
 * you.
 *
 * [1] space:
 *  1- if m = proj:     View Space
 *  2- if m = viewProj: World Space
 *  3- if m = MVP:      Object Space
 *
 * You probably want to map the coordinates into object space
 * so use MVP as m
 *
 * Computing viewProj and MVP:
 *   glm_mat4_mul(proj, view, viewProj);
 *   glm_mat4_mul(viewProj, model, MVP);
 *
 * or in struct api:
 *   MVP = mat4_mul(mat4_mul(proj, view), model)
 *
 * @param[in]  pos      point/position in viewport coordinates
 * @param[in]  m        matrix (see brief)
 * @param[in]  vp       viewport as [x, y, width, height]
 * @returns             unprojected coordinates
 */
CGLM_INLINE
vec3s
glms_unproject(vec3s pos, mat4s m, vec4s vp) {
  vec3s r;
  glm_unproject(pos.raw, m.raw, vp.raw, r.raw);
  return r;
}

/*!
 * @brief map object coordinates to window coordinates
 *
 * Computing MVP:
 *   glm_mat4_mul(proj, view, viewProj);
 *   glm_mat4_mul(viewProj, model, MVP);
 *
 * or in struct api:
 *   MVP = mat4_mul(mat4_mul(proj, view), model)
 *
 * @param[in]  pos      object coordinates
 * @param[in]  m        MVP matrix
 * @param[in]  vp       viewport as [x, y, width, height]
 * @returns projected coordinates
 */
CGLM_INLINE
vec3s
glms_project(vec3s pos, mat4s m, vec4s vp) {
  vec3s r;
  glm_project(pos.raw, m.raw, vp.raw, r.raw);
  return r;
}

/*!
 * @brief map object's z coordinate to window coordinates
 *
 * Computing MVP:
 *   glm_mat4_mul(proj, view, viewProj);
 *   glm_mat4_mul(viewProj, model, MVP);
 *
 * or in struct api:
 *   MVP = mat4_mul(mat4_mul(proj, view), model)
 *
 * @param[in]  v  object coordinates
 * @param[in]  m  MVP matrix
 *
 * @returns projected z coordinate
 */
CGLM_INLINE
float
glms_project_z(vec3s v, mat4s m) {
#if CGLM_CONFIG_CLIP_CONTROL & CGLM_CLIP_CONTROL_ZO_BIT
  return glms_project_z_zo(v, m);
#elif CGLM_CONFIG_CLIP_CONTROL & CGLM_CLIP_CONTROL_NO_BIT
  return glms_project_z_no(v, m);
#endif
}

/*!
 * @brief define a picking region
 *
 * @param[in]  center   center [x, y] of a picking region in window coordinates
 * @param[in]  size     size [width, height] of the picking region in window coordinates
 * @param[in]  vp       viewport as [x, y, width, height]
 * @returns projected coordinates
 */
CGLM_INLINE
mat4s
glms_pickmatrix(vec2s center, vec2s size, vec4s vp) {
  mat4s res;
  glm_pickmatrix(center.raw, size.raw, vp.raw, res.raw);
  return res;
}

#endif /* cglms_projects_h */
