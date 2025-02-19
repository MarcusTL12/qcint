/*
 * Qcint is a general GTO integral library for computational chemistry
 * Copyright (C) 2014- Qiming Sun <osirpt.sun@gmail.com>
 *
 * This file is part of Qcint.
 *
 * Qcint is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Description: code generated by  gen-code.cl
 */
#include <stdlib.h>
#include <stdio.h>
#include "cint_bas.h"
#include "simd.h"
#include "cart2sph.h"
#include "g1e.h"
#include "g1e_grids.h"
#include "g2e.h"
#include "optimizer.h"
#include "cint1e.h"
#include "cint2e.h"
#include "misc.h"
#include "c2f.h"
static void CINTgout1e_int3c1e_p2(double *RESTRICT gout,
double *RESTRICT g, int *RESTRICT idx, CINTEnvVars *envs, int count) {
CINTg3c1e_ovlp(g, envs, count);
int nf = envs->nf;
int nfc = nf * 1;
int ix, iy, iz, n;
DECLARE_GOUT;
double *RESTRICT g0 = g;
double *RESTRICT g1 = g0 + envs->g_size * 3 * SIMDD;
double *RESTRICT g2 = g1 + envs->g_size * 3 * SIMDD;
double *RESTRICT g3 = g2 + envs->g_size * 3 * SIMDD;
__MD r1;
__MD rs[9];
G1E_D_K(g1, g0, envs->i_l+0, envs->j_l+0, envs->k_l+1);
G1E_D_K(g2, g0, envs->i_l+0, envs->j_l+0, envs->k_l+0);
G1E_D_K(g3, g1, envs->i_l+0, envs->j_l+0, envs->k_l+0);
for (n = 0; n < nf; n++) {
ix = idx[0+n*3];
iy = idx[1+n*3];
iz = idx[2+n*3];
rs[0] = MM_LOAD(g3+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[1] = MM_LOAD(g2+ix*SIMDD) * MM_LOAD(g1+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[2] = MM_LOAD(g2+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g1+iz*SIMDD);
rs[3] = MM_LOAD(g1+ix*SIMDD) * MM_LOAD(g2+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[4] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g3+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[5] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g2+iy*SIMDD) * MM_LOAD(g1+iz*SIMDD);
rs[6] = MM_LOAD(g1+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g2+iz*SIMDD);
rs[7] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g1+iy*SIMDD) * MM_LOAD(g2+iz*SIMDD);
rs[8] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g3+iz*SIMDD);
r1 = - rs[0] - rs[4] - rs[8]; GOUT_SCATTER(gout, n*1+0, r1);
}}
void int3c1e_p2_optimizer(CINTOpt **opt, int *atm, int natm, int *bas, int nbas, double *env) {
int ng[] = {0, 0, 2, 0, 2, 1, 1, 1};
CINTall_3c1e_optimizer(opt, ng, atm, natm, bas, nbas, env);
}
CACHE_SIZE_T int3c1e_p2_cart(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {0, 0, 2, 0, 2, 1, 1, 1};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_p2;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_cart_3c1e);
} // int3c1e_p2_cart
CACHE_SIZE_T int3c1e_p2_sph(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {0, 0, 2, 0, 2, 1, 1, 1};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_p2;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_sph_3c1e);
} // int3c1e_p2_sph
CACHE_SIZE_T int3c1e_p2_spinor(double complex *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {0, 0, 2, 0, 2, 1, 1, 1};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_p2;
return CINT3c1e_spinor_drv(out, dims, &envs, opt, cache, &c2s_sf_3c2e1);
} // int3c1e_p2_spinor
ALL_CINT(int3c1e_p2)
ALL_CINT_FORTRAN_(int3c1e_p2)
static void CINTgout1e_int3c1e_iprinv(double *RESTRICT gout,
double *RESTRICT g, int *RESTRICT idx, CINTEnvVars *envs, int count) {
CINTg3c1e_nuc(g, envs, count, -1);
int nf = envs->nf;
int nrys_roots = envs->nrys_roots;
int nfc = nf * 3;
int ix, iy, iz, n, i;
DECLARE_GOUT;
double *RESTRICT g0 = g;
double *RESTRICT g1 = g0 + envs->g_size * 3 * SIMDD;
__MD r1;
__MD rs[3];
G2E_D_I(g1, g0, envs->i_l+0, envs->j_l, envs->k_l, 0);
for (n = 0; n < nf; n++) {
ix = idx[0+n*3];
iy = idx[1+n*3];
iz = idx[2+n*3];
for (i = 0; i < 3; i++) { rs[i] = MM_SET1(0.); }
for (i = 0; i < nrys_roots; i++) {
rs[0] += MM_LOAD(g1+(ix+i)*SIMDD) * MM_LOAD(g0+(iy+i)*SIMDD) * MM_LOAD(g0+(iz+i)*SIMDD);
rs[1] += MM_LOAD(g0+(ix+i)*SIMDD) * MM_LOAD(g1+(iy+i)*SIMDD) * MM_LOAD(g0+(iz+i)*SIMDD);
rs[2] += MM_LOAD(g0+(ix+i)*SIMDD) * MM_LOAD(g0+(iy+i)*SIMDD) * MM_LOAD(g1+(iz+i)*SIMDD);
}
r1 = + rs[0]; GOUT_SCATTER(gout, n*3+0, r1);
r1 = + rs[1]; GOUT_SCATTER(gout, n*3+1, r1);
r1 = + rs[2]; GOUT_SCATTER(gout, n*3+2, r1);
}}
void int3c1e_iprinv_optimizer(CINTOpt **opt, int *atm, int natm, int *bas, int nbas, double *env) {
int ng[] = {1, 0, 0, 0, 1, 1, 0, 3};
CINTall_3c1e_optimizer(opt, ng, atm, natm, bas, nbas, env);
}
CACHE_SIZE_T int3c1e_iprinv_cart(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 0, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_iprinv;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_cart_3c1e);
} // int3c1e_iprinv_cart
CACHE_SIZE_T int3c1e_iprinv_sph(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 0, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_iprinv;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_sph_3c1e);
} // int3c1e_iprinv_sph
CACHE_SIZE_T int3c1e_iprinv_spinor(double complex *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 0, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_iprinv;
return CINT3c1e_spinor_drv(out, dims, &envs, opt, cache, &c2s_sf_3c2e1i);
} // int3c1e_iprinv_spinor
ALL_CINT(int3c1e_iprinv)
ALL_CINT_FORTRAN_(int3c1e_iprinv)
static void CINTgout1e_int3c1e_ip1(double *RESTRICT gout,
double *RESTRICT g, int *RESTRICT idx, CINTEnvVars *envs, int count) {
CINTg3c1e_ovlp(g, envs, count);
int nf = envs->nf;
int nfc = nf * 3;
int ix, iy, iz, n;
DECLARE_GOUT;
double *RESTRICT g0 = g;
double *RESTRICT g1 = g0 + envs->g_size * 3 * SIMDD;
__MD r1;
__MD rs[3];
G1E_D_I(g1, g0, envs->i_l+0, envs->j_l, envs->k_l);
for (n = 0; n < nf; n++) {
ix = idx[0+n*3];
iy = idx[1+n*3];
iz = idx[2+n*3];
rs[0] = MM_LOAD(g1+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[1] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g1+iy*SIMDD) * MM_LOAD(g0+iz*SIMDD);
rs[2] = MM_LOAD(g0+ix*SIMDD) * MM_LOAD(g0+iy*SIMDD) * MM_LOAD(g1+iz*SIMDD);
r1 = + rs[0]; GOUT_SCATTER(gout, n*3+0, r1);
r1 = + rs[1]; GOUT_SCATTER(gout, n*3+1, r1);
r1 = + rs[2]; GOUT_SCATTER(gout, n*3+2, r1);
}}
void int3c1e_ip1_optimizer(CINTOpt **opt, int *atm, int natm, int *bas, int nbas, double *env) {
int ng[] = {1, 0, 0, 0, 1, 1, 1, 3};
CINTall_3c1e_optimizer(opt, ng, atm, natm, bas, nbas, env);
}
CACHE_SIZE_T int3c1e_ip1_cart(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 1, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_ip1;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_cart_3c1e);
} // int3c1e_ip1_cart
CACHE_SIZE_T int3c1e_ip1_sph(double *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 1, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_ip1;
return CINT3c1e_drv(out, dims, &envs, opt, cache, &c2s_sph_3c1e);
} // int3c1e_ip1_sph
CACHE_SIZE_T int3c1e_ip1_spinor(double complex *out, int *dims, int *shls,
int *atm, int natm, int *bas, int nbas, double *env, CINTOpt *opt, double *cache) {
int ng[] = {1, 0, 0, 0, 1, 1, 1, 3};
CINTEnvVars envs;
CINTinit_int3c1e_EnvVars(&envs, ng, shls, atm, natm, bas, nbas, env);
envs.f_gout = &CINTgout1e_int3c1e_ip1;
return CINT3c1e_spinor_drv(out, dims, &envs, opt, cache, &c2s_sf_3c2e1);
} // int3c1e_ip1_spinor
ALL_CINT(int3c1e_ip1)
ALL_CINT_FORTRAN_(int3c1e_ip1)
