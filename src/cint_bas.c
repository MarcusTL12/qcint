/*
 * Qcint is a general GTO integral library for computational chemistry
 * Copyright (C) 2014 Qiming Sun <osirpt.sun@gmail.com>
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
 * basic cGTO function
 */

#include "cint_bas.h"

/*
 * No. components of a Cartesian GTO, = (l+1)*(l+2)/2
 */
int CINTlen_cart(const int l)
{
        switch (l) {
                case 0:
                        return 1;
                case 1:
                        return 3;
                case 2:
                        return 6;
                case 3:
                        return 10;
                case 4:
                        return 15;
                default:
                        return (l + 1) * (l + 2) / 2;
        }
}

int CINTlen_spinor(const int bas_id, const int *bas)
{
        if (0 == bas(KAPPA_OF, bas_id)) {
                return 4 * bas(ANG_OF, bas_id) + 2;
        } else if (bas(KAPPA_OF, bas_id) < 0) {
                return 2 * bas(ANG_OF, bas_id) + 2;
        } else {
                return 2 * bas(ANG_OF, bas_id);
        }
}

/* 
 * Num. of contracted cartesian GTO = 2j+1 * n_contraction
 */
int CINTcgtos_cart(const int bas_id, const int *bas)
{
        return CINTlen_cart(bas(ANG_OF, bas_id)) * bas(NCTR_OF, bas_id);
}
int CINTcgto_cart(const int bas_id, const int *bas)
{
        return CINTlen_cart(bas(ANG_OF, bas_id)) * bas(NCTR_OF, bas_id);
}

/* 
 * Num. of contracted spheric GTO = 2j+1 * n_contraction
 */
int CINTcgtos_spheric(const int bas_id, const int *bas)
{
        return (bas(ANG_OF, bas_id) * 2 + 1) * bas(NCTR_OF, bas_id);
}
int CINTcgto_spheric(const int bas_id, const int *bas)
{
        return (bas(ANG_OF, bas_id) * 2 + 1) * bas(NCTR_OF, bas_id);
}

/* 
 * Num. of contracted spinor GTO
 */
int CINTcgtos_spinor(const int bas_id, const int *bas)
{
        return CINTlen_spinor(bas_id, bas) * bas(NCTR_OF, bas_id);
}
int CINTcgto_spinor(const int bas_id, const int *bas)
{
        return CINTlen_spinor(bas_id, bas) * bas(NCTR_OF, bas_id);
}

/*
 * tot. primitive atomic spheric GTOs in a shell
 */
int CINTtot_pgto_spheric(const int *bas, const int nbas)
{
        int i;
        int s = 0;

        for (i = 0; i < nbas; i++) {
                s += (bas(ANG_OF, i) * 2 + 1)
                        * bas(NPRIM_OF, i);
        }
        return s;
}

/*
 * tot. primitive atomic spinors in a shell
 */
int CINTtot_pgto_spinor(const int *bas, const int nbas)
{
        int i;
        int s = 0;

        for (i = 0; i < nbas; i++) {
                s += CINTlen_spinor(i, bas) * bas(NPRIM_OF, i);
        }
        return s;
}

static int tot_cgto_accum(int (*f)(), const int *bas, const int nbas)
{
        int i;
        int s = 0;

        for (i = 0; i < nbas; i++) {
                s += (*f)(i, bas);
        }
        return s;
}
/*
 * tot. contracted atomic spheric GTOs in a shell
 */
int CINTtot_cgto_spheric(const int *bas, const int nbas)
{
        return tot_cgto_accum(&CINTcgto_spheric, bas, nbas);
}

/*
 * tot. contracted atomic spinors in a shell
 */
int CINTtot_cgto_spinor(const int *bas, const int nbas)
{
        return tot_cgto_accum(&CINTcgto_spinor, bas, nbas);
}

/*
 * tot. contracted atomic spinors in a shell
 */
int CINTtot_cgto_cart(const int *bas, const int nbas)
{
        return tot_cgto_accum(&CINTcgto_cart, bas, nbas);
}

static void shells_cgto_offset(int (*f)(), int ao_loc[],
                               const int *bas, const int nbas)
{
        int i, s;
        for (i = 0, s = 0; i < nbas; i++) {
                ao_loc[i] = s;
                s += (*f)(i, bas);
        }
}
/*
 * offset of each shell for real spheric GTOs
 */
void CINTshells_cart_offset(int ao_loc[], const int *bas, const int nbas)
{
        shells_cgto_offset(&CINTcgto_cart, ao_loc, bas, nbas);
}

/*
 * offset of each shell for real spheric GTOs
 */
void CINTshells_spheric_offset(int ao_loc[], const int *bas, const int nbas)
{
        shells_cgto_offset(&CINTcgto_spheric, ao_loc, bas, nbas);
}

/*
 * offset of each shell for AO spinors
 */
void CINTshells_spinor_offset(int ao_loc[], const int *bas, const int nbas)
{
        shells_cgto_offset(&CINTcgto_spinor, ao_loc, bas, nbas);
}


/*
 * GTO = x^{nx}y^{ny}z^{nz}e^{-ar^2}
 */
void CINTcart_comp(int *nx, int *ny, int *nz, const int lmax)
{
        int inc = 0;
        int lx, ly, lz;

        for (lx = lmax; lx >= 0; lx--) {
                for (ly = lmax - lx; ly >= 0; ly--) {
                        lz = lmax - lx - ly;
                        nx[inc] = lx;
                        ny[inc] = ly;
                        nz[inc] = lz;
                        inc++;
                }
        }
}

