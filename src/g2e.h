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

#include "config.h"
#include "cint_const.h"
#include "g1e.h"

#ifndef HAVE_BC
#define HAVE_BC
struct _BC {
        double c00[MXRYSROOTS*3];
        double c0p[MXRYSROOTS*3];
        double b01[MXRYSROOTS];
        double b00[MXRYSROOTS];
        double b10[MXRYSROOTS];
};
#endif

void CINTg2e_index_xyz(FINT *idx, const CINTEnvVars *envs);

FINT CINTinit_int2e_EnvVars(CINTEnvVars *envs, const FINT ng[],
                           const FINT *shls,
                           const FINT *atm, const FINT natm,
                           const FINT *bas, const FINT nbas, const double *env);

void CINTg0_2e(double *g, const double fac, const CINTEnvVars *envs);
void CINTg0_2e_2d(double *g, struct _BC *bc, const CINTEnvVars *envs);

double CINTg0_2e_ssss(const double fac, const CINTEnvVars *envs);

void CINTnabla1i_2e(double *f, const double *g,
                    const FINT li, const FINT lj, const FINT lk, const FINT ll,
                    const CINTEnvVars *envs);

void CINTnabla1j_2e(double *f, const double *g,
                    const FINT li, const FINT lj, const FINT lk, const FINT ll,
                    const CINTEnvVars *envs);

void CINTnabla1k_2e(double *f, const double *g,
                    const FINT li, const FINT lj, const FINT lk, const FINT ll,
                    const CINTEnvVars *envs);

void CINTnabla1l_2e(double *f, const double *g,
                    const FINT li, const FINT lj, const FINT lk, const FINT ll,
                    const CINTEnvVars *envs);

void CINTx1i_2e(double *f, const double *g, const double *ri,
                const FINT li, const FINT lj, const FINT lk, const FINT ll,
                const CINTEnvVars *envs);

void CINTx1j_2e(double *f, const double *g, const double *rj,
                const FINT li, const FINT lj, const FINT lk, const FINT ll,
                const CINTEnvVars *envs);

void CINTx1k_2e(double *f, const double *g, const double *rk,
                const FINT li, const FINT lj, const FINT lk, const FINT ll,
                const CINTEnvVars *envs);

void CINTx1l_2e(double *f, const double *g, const double *rl,
                const FINT li, const FINT lj, const FINT lk, const FINT ll,
                const CINTEnvVars *envs);


#define G2E_D_I(f, g, li, lj, lk, ll)   CINTnabla1i_2e(f, g, li, lj, lk, ll, envs)
#define G2E_D_J(f, g, li, lj, lk, ll)   CINTnabla1j_2e(f, g, li, lj, lk, ll, envs)
#define G2E_D_K(f, g, li, lj, lk, ll)   CINTnabla1k_2e(f, g, li, lj, lk, ll, envs)
#define G2E_D_L(f, g, li, lj, lk, ll)   CINTnabla1l_2e(f, g, li, lj, lk, ll, envs)
/* r-R_0, R_0 is (0,0,0) */
#define G2E_R0I(f, g, li, lj, lk, ll)   CINTx1i_2e(f, g, ri, li, lj, lk, ll, envs)
#define G2E_R0J(f, g, li, lj, lk, ll)   CINTx1j_2e(f, g, rj, li, lj, lk, ll, envs)
#define G2E_R0K(f, g, li, lj, lk, ll)   CINTx1k_2e(f, g, rk, li, lj, lk, ll, envs)
#define G2E_R0L(f, g, li, lj, lk, ll)   CINTx1l_2e(f, g, rl, li, lj, lk, ll, envs)
/* r-R_C, R_C is common origin */
#define G2E_RCI(f, g, li, lj, lk, ll)   CINTx1i_2e(f, g, dri, li, lj, lk, ll, envs)
#define G2E_RCJ(f, g, li, lj, lk, ll)   CINTx1j_2e(f, g, drj, li, lj, lk, ll, envs)
#define G2E_RCK(f, g, li, lj, lk, ll)   CINTx1k_2e(f, g, drk, li, lj, lk, ll, envs)
#define G2E_RCL(f, g, li, lj, lk, ll)   CINTx1l_2e(f, g, drl, li, lj, lk, ll, envs)
/* origin from center of each basis
 * x1[ijkl]_2e(f, g, ng, li, lj, lk, ll, 0d0) */
#define G2E_R_I(f, g, li, lj, lk, ll)   f = g + envs->g_stride_i
#define G2E_R_K(f, g, li, lj, lk, ll)   f = g + envs->g_stride_k
#define G2E_R_L(f, g, li, lj, lk, ll)   f = g + envs->g_stride_l
#define G2E_R_J(f, g, li, lj, lk, ll)   f = g + envs->g_stride_j
