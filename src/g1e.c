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


#include <math.h>
#include "cint_bas.h"
#include "misc.h"
#include "g1e.h"


void CINTg1e_index_xyz(int idx[], const int *ng,
                       const int shls[], const int *bas)
{
        const int i_l = bas(ANG_OF, shls[0]);
        const int j_l = bas(ANG_OF, shls[1]);
        const int nfi = CINTlen_cart(i_l);
        const int nfj = CINTlen_cart(j_l);
        int i, j, n;
        int ofx, ofy, ofz;
        int i_nx[CART_MAX], i_ny[CART_MAX], i_nz[CART_MAX];
        int j_nx[CART_MAX], j_ny[CART_MAX], j_nz[CART_MAX];
        int *idy = idx + nfi * nfj;
        int *idz = idx + nfi * nfj * 2;

        CINTcart_comp(i_nx, i_ny, i_nz, i_l);
        CINTcart_comp(j_nx, j_ny, j_nz, j_l);

        ofx = 0;
        ofy = ng[0] * ng[1];
        ofz = ng[0] * ng[1] * 2;
        n = 0;
        for (j = 0; j < nfj; j++) {
                for (i = 0; i < nfi; i++) {
                        idx[n] = ofx + ng[0] * j_nx[j] + i_nx[i]; //(ix,jx,1)
                        idy[n] = ofy + ng[0] * j_ny[j] + i_ny[i]; //(iy,jy,2)
                        idz[n] = ofz + ng[0] * j_nz[j] + i_nz[i]; //(iz,jz,3)
                        n++;
                } // i
        } // j
}


void CINTg_ovlp(double *g, const int *ng,
                const double ai, const double aj,
                const double *ri, const double *rj, const double fac)
{
        const int nmax = ng[0] - 1;
        const int lj = ng[1] - 1;
        const double aij = ai + aj;
        const int dj = ng[0];
        int i, j, ptr;
        double rirj[3], ririj[3];
        double *gx = g;
        double *gy = g + ng[0] * ng[1];
        double *gz = g + ng[0] * ng[1] * 2;

        rirj[0] = ri[0] - rj[0];
        rirj[1] = ri[1] - rj[1];
        rirj[2] = ri[2] - rj[2];
        ririj[0] = ri[0] - (ai * ri[0] + aj * rj[0]) / aij;
        ririj[1] = ri[1] - (ai * ri[1] + aj * rj[1]) / aij;
        ririj[2] = ri[2] - (ai * ri[2] + aj * rj[2]) / aij;

        gx[0] = 1;
        gy[0] = 1;
        gz[0] = fac;
        if (nmax > 0) {
                gx[1] = -ririj[0] * gx[0];
                gy[1] = -ririj[1] * gy[0];
                gz[1] = -ririj[2] * gz[0];
        }

        for (i = 1; i < nmax; i++) {
                gx[i+1] = 0.5 * i / aij * gx[i-1] - ririj[0] * gx[i];
                gy[i+1] = 0.5 * i / aij * gy[i-1] - ririj[1] * gy[i];
                gz[i+1] = 0.5 * i / aij * gz[i-1] - ririj[2] * gz[i];
        }

        for (j = 1; j <= lj; j++) {
                ptr = dj * j;
                for (i = ptr; i <= ptr + nmax - j; i++) {
                        gx[i] = gx[i+1-dj] + rirj[0] * gx[i-dj];
                        gy[i] = gy[i+1-dj] + rirj[1] * gy[i-dj];
                        gz[i] = gz[i+1-dj] + rirj[2] * gz[i-dj];
                }
        }
}

void CINTg_nuc(double *g, const int *ng,
               const double aij, const double *rij,
               const double *ri, const double *rj,
               const double *cr, const double t2, const double fac)
{
        const int nmax = ng[0] - 1;
        const int lj = ng[1] - 1;
        const int dj = ng[0];
        int i, j, ptr;
        double rir0[3], rirj[3];
        double *gx = g;
        double *gy = g + ng[0] * ng[1];
        double *gz = g + ng[0] * ng[1] * 2;

        rir0[0] = ri[0] - (rij[0] + t2 * (cr[0] - rij[0]));
        rir0[1] = ri[1] - (rij[1] + t2 * (cr[1] - rij[1]));
        rir0[2] = ri[2] - (rij[2] + t2 * (cr[2] - rij[2]));
        rirj[0] = ri[0] - rj[0];
        rirj[1] = ri[1] - rj[1];
        rirj[2] = ri[2] - rj[2];

        gx[0] = 1;
        gy[0] = 1;
        gz[0] = fac;
        if (nmax > 0) {
                gx[1] = -rir0[0] * gx[0];
                gy[1] = -rir0[1] * gy[0];
                gz[1] = -rir0[2] * gz[0];
        }

        for (i = 1; i < nmax; i++) {
                gx[i+1] = 0.5 * (1 - t2) * i / aij * gx[i-1] - rir0[0] * gx[i];
                gy[i+1] = 0.5 * (1 - t2) * i / aij * gy[i-1] - rir0[1] * gy[i];
                gz[i+1] = 0.5 * (1 - t2) * i / aij * gz[i-1] - rir0[2] * gz[i];
        }

        for (j = 1; j <= lj; j++) {
                ptr = dj * j;
                for (i = ptr; i <= ptr + nmax - j; i++) {
                        gx[i] = gx[i+1-dj] + rirj[0] * gx[i-dj];
                        gy[i] = gy[i+1-dj] + rirj[1] * gy[i-dj];
                        gz[i] = gz[i+1-dj] + rirj[2] * gz[i-dj];
                }
        }
}

void CINTnabla1i_1e(double *f, const double *g, const int *ng,
                    const int li, const int lj, const double ai)
{
        int i, j, ptr;
        const double *gx = g;
        const double *gy = g + ng[0] * ng[1];
        const double *gz = g + ng[0] * ng[1] * 2;
        double *fx = f;
        double *fy = f + ng[0] * ng[1];
        double *fz = f + ng[0] * ng[1] * 2;

        for (j = 0; j <= lj; j++) {
                ptr = ng[0] * j;
                //f(...,0,...) = -2*ai*g(...,1,...)
                fx[ptr] = - 2 * ai * gx[ptr+1];
                fy[ptr] = - 2 * ai * gy[ptr+1];
                fz[ptr] = - 2 * ai * gz[ptr+1];
                //f(...,i,...) = i*g(...,i-1,...)-2*ai*g(...,i+1,...)
                for (i = 1; i <= li; i++) {
                        fx[ptr+i] = i * gx[ptr+i-1] - 2 * ai * gx[ptr+i+1];
                        fy[ptr+i] = i * gy[ptr+i-1] - 2 * ai * gy[ptr+i+1];
                        fz[ptr+i] = i * gz[ptr+i-1] - 2 * ai * gz[ptr+i+1];
                }
        }
}

void CINTnabla1j_1e(double *f, const double *g, const int *ng,
                    const int li, const int lj, const double aj)
{
        int i, j, ptr;
        const int dj = ng[0];
        const double *gx = g;
        const double *gy = g + ng[0] * ng[1];
        const double *gz = g + ng[0] * ng[1] * 2;
        double *fx = f;
        double *fy = f + ng[0] * ng[1];
        double *fz = f + ng[0] * ng[1] * 2;

        //f(...,0,...) = -2*aj*g(...,1,...)
        for (i = 0; i <= li; i++) {
                fx[i] = - 2 * aj * gx[i+dj];
                fy[i] = - 2 * aj * gy[i+dj];
                fz[i] = - 2 * aj * gz[i+dj];
        }
        //f(...,j,...) = j*g(...,j-1,...)-2*aj*g(...,j+1,...)
        for (j = 1; j <= lj; j++) {
                ptr = dj * j;
                for (i = 0; i <= li; i++) {
                        fx[ptr+i] = j * gx[ptr+i-dj] - 2 * aj * gx[ptr+i+dj];
                        fy[ptr+i] = j * gy[ptr+i-dj] - 2 * aj * gy[ptr+i+dj];
                        fz[ptr+i] = j * gz[ptr+i-dj] - 2 * aj * gz[ptr+i+dj];
                }
        }
}

/*
 * < x^1 i | j >
 * ri is the shift from the center R_O to the center of |i>
 * r - R_O = (r-R_i) + ri, ri = R_i - R_O
 */
void CINTx1i_1e(double *f, const double *g, const int *ng,
                const int li, const int lj, const double ri[3])
{
        int i, j, ptr;
        const double *gx = g;
        const double *gy = g + ng[0] * ng[1];
        const double *gz = g + ng[0] * ng[1] * 2;
        double *fx = f;
        double *fy = f + ng[0] * ng[1];
        double *fz = f + ng[0] * ng[1] * 2;

        for (j = 0; j <= lj; j++) {
                ptr = ng[0] * j;
                //f(...,0:li,...) = g(...,1:li+1,...) + ri(1)*g(...,0:li,...)
                for (i = ptr; i <= ptr + li; i++) {
                        fx[i] = gx[i+1] + ri[0] * gx[i];
                        fy[i] = gy[i+1] + ri[1] * gy[i];
                        fz[i] = gz[i+1] + ri[2] * gz[i];
                }
        }
}

void CINTx1j_1e(double *f, const double *g, const int *ng,
                const int li, const int lj, const double rj[3])
{
        int i, j, ptr;
        const int dj = ng[0];
        const double *gx = g;
        const double *gy = g + ng[0] * ng[1];
        const double *gz = g + ng[0] * ng[1] * 2;
        double *fx = f;
        double *fy = f + ng[0] * ng[1];
        double *fz = f + ng[0] * ng[1] * 2;

        for (j = 0; j <= lj; j++) {
                ptr = dj * j;
                //f(...,0:lj,...) = g(...,1:lj+1,...) + rj(1)*g(...,0:lj,...)
                for (i = ptr; i <= ptr + li; i++) {
                        fx[i] = gx[i+dj] + rj[0] * gx[i];
                        fy[i] = gy[i+dj] + rj[1] * gy[i];
                        fz[i] = gz[i+dj] + rj[2] * gz[i];
                }
        }
}


/*
 * gc    contracted GTO integral
 * nf    number of primitive integral
 * gp    primitive GTO integral
 * inc   increment of gp
 * shl   nth shell
 * ip    ith-1 primitive GTO
 */
void CINTprim_to_ctr(double *gc, const int nf, const double *gp,
                     const int inc, const int nprim,
                     const int nctr, const double *pcoeff)
{
        const int INC1 = 1;
        int n, i;
        double *pgc = gc;

        for (i = 0; i < inc; i++) {
                //dger(nf, nctr, 1.d0, gp(i+1), inc, env(ptr), nprim, gc(1,i*nctr+1), nf)
                for (n = 0; n < nctr; n++) {
                        if (pcoeff[nprim*n] != 0) {
                                daxpy_(&nf, pcoeff+nprim*n, gp+i, &inc, pgc, &INC1);
                        }
                        // next cgto block
                        pgc += nf;
                }
        }
}

/*
 * to optimize memory copy in cart2sph.c, remove the common factor for s
 * and p function in cart2sph
 */
double CINTcommon_fac_sp(int l)
{
        switch (l) {
                case 0: return 0.282094791773878143;
                case 1: return 0.488602511902919921;
                default: return 1;
        }
}
