#pragma once
#include "fmath.hpp"
#include "points.hpp"
#include <algorithm>
#include <stdio.h>
#include <iostream>
#define _gamma 5.828427124 // FOUR_GAMMA_SQUARED = sqrt(8)+3;
#define _cstar 0.923879532 // cos(pi/8)
#define _sstar 0.3826834323 // sin(p/8)
#define EPSILON 1e-6

#if defined _MSC_VER
#define restrict_macro __restrict
#else
#define restrict_macro __restrict__
#endif

class
{
    //========= only float variables ===========================
    inline float rsqrt(float x);
    inline float rsqrt1(float x);
    inline float accurateSqrt(float x);
    inline void condSwap(const bool &, float &, float &);
    inline void condNegSwap(const bool &, float &, float &);
    inline void multAB(const float&, const float&, const float&,
                       const float&, const float&, const float&,
                       const float&, const float&, const float&,

                       const float&, const float&, const float&,
                       const float&, const float&, const float&,
                       const float&, const float&, const float&,

                       float&,float&,float&,
                       float&,float&,float&,
                       float&,float&,float&);
    inline void multAtB(const float&, const float&, const float&,
                       const float&, const float&, const float&,
                       const float&, const float&, const float&,

                       const float&, const float&, const float&,
                       const float&, const float&, const float&,
                       const float&, const float&, const float&,

                       float&,float&,float&,
                       float&,float&,float&,
                       float&,float&,float&);
    inline void quatfloatoMat3(const float *,
                           float&, float&, float&,
                           float&, float&, float&,
                           float&, float&, float&);
    inline void approximateGivensQuaternion(const float&, const float&, const float&, float&, float &);
    inline void jacobiConjugation( const int &, const int &, const int &,
                                   float &,
                                   float &, float &,
                                   float &, float &, float &,
                                   float *);
    inline float dist2(const float&, const float&, const float&);
    inline void jacobiEigenanalysis(float&,
                                    float&, float&,
                                    float&, float&, float&,
                                    float *);
    inline void sortSingularValues(float&, float&, float&,
                                   float&, float&, float&,
                                   float&, float&, float&,

                                   float&, float&, float&,
                                   float&, float&, float&,
                                   float&, float&, float&);
    inline void QRGivensQuaternion(const float&, const float&, float &, float&);
    inline void QRDecomposition(float, float, float,
                                float, float, float,
                                float, float, float,

                                float&, float&, float&,
                                float&, float&, float&,
                                float&, float&, float&,

                                float&, float&, float&,
                                float&, float&, float&,
                                float&, float&, float&);
    void svd3x3(const float&, const float&, const float&,
                const float&, const float&, const float&,
                const float&, const float&, const float&,

                float&, float&, float&,
                float&, float&, float&,
                float&, float&, float&,

                float&, float&, float&,
                float&, float&, float&,
                float&, float&, float&,

                float&, float&, float&,
                float&, float&, float&,
                float&, float&, float&);

    //================= best for double variables ====================================================
    inline void cross(float * restrict_macro , const float * restrict_macro , const float * restrict_macro );
    inline void sort3(float * restrict_macro );
    inline void unit3(float * restrict_macro );
    inline void ldubsolve3(float * restrict_macro , const float * restrict_macro , const float * restrict_macro , const int * restrict_macro );
    inline void matmul3(float * restrict_macro , const float * restrict_macro , const float * restrict_macro );
    inline void matvec3(float * restrict_macro , const float * restrict_macro , const float * restrict_macro );
    inline void ata3(float * restrict_macro , const float * restrict_macro );
    inline void aat3(float * restrict_macro , const float * restrict_macro );
    inline void trans3(float * restrict_macro );
    void solvecubic(float * restrict_macro );
    void ldu3(float * restrict_macro , int * restrict_macro );
    void svd3(float * restrict_macro , float * restrict_macro , float * restrict_macro , const float * restrict_macro );

    //================ common function for rmsd computation ===========================================
    inline float dist(const float &, const float &, const float &, const float &, const float &, const float &);
public:
    inline float operator()(const Point &, const Point &);
} kabsch;
typedef decltype(kabsch) Kabsch;


inline void Kabsch::cross(float * restrict_macro z, const float * restrict_macro x, const float * restrict_macro y)
{
    z[0] = x[1]*y[2]-x[2]*y[1];
    z[1] = -(x[0]*y[2]-x[2]*y[0]);
    z[2] = x[0]*y[1]-x[1]*y[0];
}
inline void Kabsch::sort3(float * restrict_macro x)
{
    float tmp;

    if (x[0] < x[1])
    {
        tmp = x[0];
        x[0] = x[1];
        x[1] = tmp;
    }
    if (x[1] < x[2])
    {
        if (x[0] < x[2])
        {
            tmp = x[2];
            x[2] = x[1];
            x[1] = x[0];
            x[0] = tmp;
        }
        else
        {
            tmp = x[1];
            x[1] = x[2];
            x[2] = tmp;
        }
    }
}
inline void Kabsch::unit3(float * restrict_macro x)
{
    float tmp = std::sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
    x[0] /= tmp;
    x[1] /= tmp;
    x[2] /= tmp;
}
inline void Kabsch::ldubsolve3(float * restrict_macro x, const float * restrict_macro y, const float * restrict_macro LDU, const int * restrict_macro P)
{
    x[P[2]] = y[2];
    x[P[1]] = y[1] - LDU[3*P[2]+1]*x[P[2]];
    x[P[0]] = y[0] - LDU[3*P[2]+0]*x[P[2]] - LDU[3*P[1]+0]*x[P[1]];
}
inline void Kabsch::matmul3(float * restrict_macro C, const float * restrict_macro A, const float * restrict_macro B)
{
    C[3*0+0] = A[3*0+0]*B[3*0+0] + A[3*1+0]*B[3*0+1] + A[3*2+0]*B[3*0+2];
    C[3*1+0] = A[3*0+0]*B[3*1+0] + A[3*1+0]*B[3*1+1] + A[3*2+0]*B[3*1+2];
    C[3*2+0] = A[3*0+0]*B[3*2+0] + A[3*1+0]*B[3*2+1] + A[3*2+0]*B[3*2+2];

    C[3*0+1] = A[3*0+1]*B[3*0+0] + A[3*1+1]*B[3*0+1] + A[3*2+1]*B[3*0+2];
    C[3*1+1] = A[3*0+1]*B[3*1+0] + A[3*1+1]*B[3*1+1] + A[3*2+1]*B[3*1+2];
    C[3*2+1] = A[3*0+1]*B[3*2+0] + A[3*1+1]*B[3*2+1] + A[3*2+1]*B[3*2+2];

    C[3*0+2] = A[3*0+2]*B[3*0+0] + A[3*1+2]*B[3*0+1] + A[3*2+2]*B[3*0+2];
    C[3*1+2] = A[3*0+2]*B[3*1+0] + A[3*1+2]*B[3*1+1] + A[3*2+2]*B[3*1+2];
    C[3*2+2] = A[3*0+2]*B[3*2+0] + A[3*1+2]*B[3*2+1] + A[3*2+2]*B[3*2+2];
}
inline void Kabsch::matvec3(float * restrict_macro y, const float * restrict_macro A, const float * restrict_macro x)
{
    y[0] = A[3*0+0]*x[0] + A[3*1+0]*x[1] + A[3*2+0]*x[2];
    y[1] = A[3*0+1]*x[0] + A[3*1+1]*x[1] + A[3*2+1]*x[2];
    y[2] = A[3*0+2]*x[0] + A[3*1+2]*x[1] + A[3*2+2]*x[2];
}
inline void Kabsch::ata3(float * restrict_macro AA, const float * restrict_macro A)
{
    AA[3*0+0] = A[3*0+0]*A[3*0+0] + A[3*0+1]*A[3*0+1] + A[3*0+2]*A[3*0+2];
    AA[3*1+0] = A[3*0+0]*A[3*1+0] + A[3*0+1]*A[3*1+1] + A[3*0+2]*A[3*1+2];
    AA[3*2+0] = A[3*0+0]*A[3*2+0] + A[3*0+1]*A[3*2+1] + A[3*0+2]*A[3*2+2];

    AA[3*0+1] = AA[3*1+0];
    AA[3*1+1] = A[3*1+0]*A[3*1+0] + A[3*1+1]*A[3*1+1] + A[3*1+2]*A[3*1+2];
    AA[3*2+1] = A[3*1+0]*A[3*2+0] + A[3*1+1]*A[3*2+1] + A[3*1+2]*A[3*2+2];

    AA[3*0+2] = AA[3*2+0];
    AA[3*1+2] = AA[3*2+1];
    AA[3*2+2] = A[3*2+0]*A[3*2+0] + A[3*2+1]*A[3*2+1] + A[3*2+2]*A[3*2+2];
}
inline void Kabsch::aat3(float * restrict_macro AA, const float * restrict_macro A)
{
    AA[3*0+0] = A[3*0+0]*A[3*0+0] + A[3*1+0]*A[3*1+0] + A[3*2+0]*A[3*2+0];
    AA[3*1+0] = A[3*0+0]*A[3*0+1] + A[3*1+0]*A[3*1+1] + A[3*2+0]*A[3*2+1];
    AA[3*2+0] = A[3*0+0]*A[3*0+2] + A[3*1+0]*A[3*1+2] + A[3*2+0]*A[3*2+2];

    AA[3*0+1] = AA[3*1+0];
    AA[3*1+1] = A[3*0+1]*A[3*0+1] + A[3*1+1]*A[3*1+1] + A[3*2+1]*A[3*2+1];
    AA[3*2+1] = A[3*0+1]*A[3*0+2] + A[3*1+1]*A[3*1+2] + A[3*2+1]*A[3*2+2];

    AA[3*0+2] = AA[3*2+0];
    AA[3*1+2] = AA[3*2+1];
    AA[3*2+2] = A[3*0+2]*A[3*0+2] + A[3*1+2]*A[3*1+2] + A[3*2+2]*A[3*2+2];
}
inline void Kabsch::trans3(float * restrict_macro A)
{
    float tmp;

    tmp = A[3*1+0];
    A[3*1+0] = A[3*0+1];
    A[3*0+1] = tmp;

    tmp = A[3*2+0];
    A[3*2+0] = A[3*0+2];
    A[3*0+2] = tmp;

    tmp = A[3*2+1];
    A[3*2+1] = A[3*1+2];
    A[3*1+2] = tmp;
}
void Kabsch::solvecubic(float * restrict_macro c)
{
    const float sq3d2 = 0.86602540378443864676f, c2d3 = c[2]/3,
            c2sq = c[2]*c[2], Q = (3*c[1]-c2sq)/9,
            R = (c[2]*(9*c[1]-2*c2sq)-27*c[0])/54;
    float tmp, t, sint, cost;

    if (Q < 0)
    {
        /*
         * Instead of computing
         * c_0 = A cos(t) - B
         * c_1 = A cos(t + 2 pi/3) - B
         * c_2 = A cos(t + 4 pi/3) - B
         * Use cos(a+b) = cos(a) cos(b) - sin(a) sin(b)
         * Keeps t small and eliminates 1 function call.
         * cos(2 pi/3) = cos(4 pi/3) = -0.5
         * sin(2 pi/3) = sqrt(3)/2
         * sin(4 pi/3) = -sqrt(3)/2
         */

        tmp = 2*std::sqrt(-Q);
        t = std::acos(R/std::sqrt(-Q*Q*Q))/3;
        cost = tmp*std::cos(t);
        sint = tmp*std::sin(t);

        c[0] = cost - c2d3;

        cost = -0.5f*cost - c2d3;
        sint = sq3d2*sint;

        c[1] = cost - sint;
        c[2] = cost + sint;
    }
    else
    {
        tmp = cbrt(R);
        c[0] = -c2d3 + 2*tmp;
        c[1] = c[2] = -c2d3 - tmp;
    }
}
void Kabsch::ldu3(float * restrict_macro A, int * restrict_macro P)
{
    int tmp;

    P[1] = 1;
    P[2] = 2;

    P[0] =   std::fabs(A[3*1+0]) > std::fabs(A[3*0+0]) ?
            (std::fabs(A[3*2+0]) > std::fabs(A[3*1+0]) ? 2 : 1) :
            (std::fabs(A[3*2+0]) > std::fabs(A[3*0+0]) ? 2 : 0);
    P[P[0]] = 0;

    if (std::fabs(A[3*P[2]+2]) > std::fabs(A[3*P[1]+1]))
    {
        tmp = P[1];
        P[1] = P[2];
        P[2] = tmp;
    }

    if (A[3*P[0]+0] != 0.f)
    {
        A[3*P[1]+0] = A[3*P[1]+0]/A[3*P[0]+0];
        A[3*P[2]+0] = A[3*P[2]+0]/A[3*P[0]+0];
        A[3*P[0]+1] = A[3*P[0]+1]/A[3*P[0]+0];
        A[3*P[0]+2] = A[3*P[0]+2]/A[3*P[0]+0];
    }

    A[3*P[1]+1] = A[3*P[1]+1] - A[3*P[0]+1]*A[3*P[1]+0]*A[3*P[0]+0];

    if (A[3*P[1]+1] != 0.f)
    {
        A[3*P[2]+1] = (A[3*P[2]+1] - A[3*P[0]+1]*A[3*P[2]+0]*A[3*P[0]+0])/A[3*P[1]+1];
        A[3*P[1]+2] = (A[3*P[1]+2] - A[3*P[0]+2]*A[3*P[1]+0]*A[3*P[0]+0])/A[3*P[1]+1];
    }

    A[3*P[2]+2] = A[3*P[2]+2] - A[3*P[0]+2]*A[3*P[2]+0]*A[3*P[0]+0] - A[3*P[1]+2]*A[3*P[2]+1]*A[3*P[1]+1];
}
void Kabsch::svd3(float * restrict_macro U, float * restrict_macro S, float * restrict_macro V, const float * restrict_macro A)
{
    const float thr = 1e-10f;
    int P[3], k;
    float y[3], AA[3][3], LDU[3][3];

    /*
     * Steps:
     * 1) Use eigendecomposition on A^T A to compute V.
     * Since A = U S V^T then A^T A = V S^T S V^T with D = S^T S and V the
     * eigenvalues and eigenvectors respectively (V is orthogonal).
     * 2) Compute U from A and V.
     * 3) Normalize columns of U and V and root the eigenvalues to obtain
     * the singular values.
     */

    /* Compute AA = A^T A */
    ata3((float *)AA, A);

    /* Form the monic characteristic polynomial */
    S[2] = -AA[0][0] - AA[1][1] - AA[2][2];
    S[1] = AA[0][0]*AA[1][1] + AA[2][2]*AA[0][0] + AA[2][2]*AA[1][1] -
            AA[2][1]*AA[1][2] - AA[2][0]*AA[0][2] - AA[1][0]*AA[0][1];
    S[0] = AA[2][1]*AA[1][2]*AA[0][0] + AA[2][0]*AA[0][2]*AA[1][1] + AA[1][0]*AA[0][1]*AA[2][2] -
            AA[0][0]*AA[1][1]*AA[2][2] - AA[1][0]*AA[2][1]*AA[0][2] - AA[2][0]*AA[0][1]*AA[1][2];

    /* Solve the cubic equation. */
    solvecubic(S);

    /* All roots should be positive */
    if (S[0] < 0.f)
            S[0] = 0.f;
    if (S[1] < 0.f)
            S[1] = 0.f;
    if (S[2] < 0.f)
            S[2] = 0.f;

    /* Sort from greatest to least */
    sort3(S);

    /* Form the eigenvector system for the first (largest) eigenvalue */
    std::memcpy(LDU, AA, sizeof(LDU));
    LDU[0][0] -= S[0];
    LDU[1][1] -= S[0];
    LDU[2][2] -= S[0];

    /* Perform LDUP decomposition */
    ldu3((float *)LDU, P);

    /*
     * Write LDU = AA-I*lambda.  Then an eigenvector can be
     * found by solving LDU x = LD y = L z = 0
     * L is invertible, so L z = 0 implies z = 0
     * D is singular since det(AA-I*lambda) = 0 and so
     * D y = z = 0 has a non-unique solution.
     * Pick k so that D_kk = 0 and set y = e_k, the k'th column
     * of the identity matrix.
     * U is invertible so U x = y has a unique solution for a given y.
     * The solution for U x = y is an eigenvector.
     */

    /* Pick the component of D nearest to 0 */
    y[0] = y[1] = y[2] = 0.f;
    k =      std::fabs(LDU[P[1]][1]) < std::fabs(LDU[P[0]][0]) ?
            (std::fabs(LDU[P[2]][2]) < std::fabs(LDU[P[1]][1]) ? 2 : 1) :
            (std::fabs(LDU[P[2]][2]) < std::fabs(LDU[P[0]][0]) ? 2 : 0);
    y[k] = 1.f;

    /* Do a backward solve for the eigenvector */
    ldubsolve3(V+(3*0+0), y, (float *)LDU, P);

    /* Form the eigenvector system for the last (smallest) eigenvalue */
    std::memcpy(LDU, AA, sizeof(LDU));
    LDU[0][0] -= S[2];
    LDU[1][1] -= S[2];
    LDU[2][2] -= S[2];

    /* Perform LDUP decomposition */
    ldu3((float *)LDU, P);

    /*
     * NOTE: The arrangement of the ternary operator output is IMPORTANT!
     * It ensures a different system is solved if there are 3 repeat eigenvalues.
     */

    /* Pick the component of D nearest to 0 */
    y[0] = y[1] = y[2] = 0.f;
    k =      std::fabs(LDU[P[0]][0]) < std::fabs(LDU[P[2]][2]) ?
            (std::fabs(LDU[P[0]][0]) < std::fabs(LDU[P[1]][1]) ? 0 : 1) :
            (std::fabs(LDU[P[1]][1]) < std::fabs(LDU[P[2]][2]) ? 1 : 2);
    y[k] = 1.f;

    /* Do a backward solve for the eigenvector */
    ldubsolve3(V+(3*2+0), y, (float *)LDU, P);

     /* The remaining column must be orthogonal (AA is symmetric) */
    cross(V+(3*1+0), V+(3*2+0), V+(3*0+0));

    /* Count the rank */
    k = (S[0] > thr) + (S[1] > thr) + (S[2] > thr);

    switch (k)
    {
        case 0:
            /*
             * Zero matrix.
             * Since V is already orthogonal, just copy it into U.
             */
            std::memcpy(U, V, 9*sizeof(float));
            break;
        case 1:
            /*
             * The first singular value is non-zero.
             * Since A = U S V^T, then A V = U S.
             * A V_1 = S_11 U_1 is non-zero. Here V_1 and U_1 are
             * column vectors. Since V_1 is known, we may compute
             * U_1 = A V_1.  The S_11 factor is not important as
             * U_1 will be normalized later.
             */
            matvec3(U+(3*0+0), A, V+(3*0+0));

            /*
             * The other columns of U do not contribute to the expansion
             * and we may arbitrarily choose them (but they do need to be
             * orthogonal). To ensure the first cross product does not fail,
             * pick k so that U_k1 is nearest 0 and then cross with e_k to
             * obtain an orthogonal vector to U_1.
             */
            y[0] = y[1] = y[2] = 0.f;
            k =      std::fabs(U[3*0+0]) < std::fabs(U[3*0+2]) ?
                    (std::fabs(U[3*0+0]) < std::fabs(U[3*0+1]) ? 0 : 1) :
                    (std::fabs(U[3*0+1]) < std::fabs(U[3*0+2]) ? 1 : 2);
            y[k] = 1.f;

            cross(U+(3*1+0), y, U+(3*0+0));

            /* Cross the first two to obtain the remaining column */
            cross(U+(3*2+0), U+(3*0+0), U+(3*1+0));
            break;
        case 2:
            /*
             * The first two singular values are non-zero.
             * Compute U_1 = A V_1 and U_2 = A V_2. See case 1
             * for more information.
             */
            matvec3(U+(3*0+0), A, V+(3*0+0));
            matvec3(U+(3*1+0), A, V+(3*1+0));

            /* Cross the first two to obtain the remaining column */
            cross(U+(3*2+0), U+(3*0+0), U+(3*1+0));
            break;
        case 3:
            /*
             * All singular values are non-zero.
             * We may compute U = A V. See case 1 for more information.
             */
            matmul3(U, A, V);
            break;
    }

    /* Normalize the columns of U and V */
    unit3(V+(3*0+0));
    unit3(V+(3*1+0));
    unit3(V+(3*2+0));

    unit3(U+(3*0+0));
    unit3(U+(3*1+0));
    unit3(U+(3*2+0));

    /* S was initially the eigenvalues of A^T A = V S^T S V^T which are squared. */
    S[0] = std::sqrt(S[0]);
    S[1] = std::sqrt(S[1]);
    S[2] = std::sqrt(S[2]);
}






void Kabsch::svd3x3(// input A
                    const float &a11, const float &a12, const float &a13,
                    const float &a21, const float &a22, const float &a23,
                    const float &a31, const float &a32, const float &a33,
                    // output U
                    float &u11, float &u12, float &u13,
                    float &u21, float &u22, float &u23,
                    float &u31, float &u32, float &u33,
                    // output S
                    float &s11, float &s12, float &s13,
                    float &s21, float &s22, float &s23,
                    float &s31, float &s32, float &s33,
                    // output V
                    float &v11, float &v12, float &v13,
                    float &v21, float &v22, float &v23,
                    float &v31, float &v32, float &v33)
{
    // normal equations matrix
    float AfloatA11, AfloatA12, AfloatA13;
    float AfloatA21, AfloatA22, AfloatA23;
    float AfloatA31, AfloatA32, AfloatA33;

    multAtB(a11,a12,a13,a21,a22,a23,a31,a32,a33,
          a11,a12,a13,a21,a22,a23,a31,a32,a33,
          AfloatA11,AfloatA12,AfloatA13,AfloatA21,AfloatA22,AfloatA23,AfloatA31,AfloatA32,AfloatA33);

    // symmetric eigenalysis
    float qV[4];
    jacobiEigenanalysis( AfloatA11,AfloatA21,AfloatA22, AfloatA31,AfloatA32,AfloatA33,qV);
    quatfloatoMat3(qV,v11,v12,v13,v21,v22,v23,v31,v32,v33);

    float b11, b12, b13;
    float b21, b22, b23;
    float b31, b32, b33;
    multAB(a11,a12,a13,a21,a22,a23,a31,a32,a33,
        v11,v12,v13,v21,v22,v23,v31,v32,v33,
        b11, b12, b13, b21, b22, b23, b31, b32, b33);

    // sort singular values and find V
    sortSingularValues(b11, b12, b13, b21, b22, b23, b31, b32, b33,
                        v11,v12,v13,v21,v22,v23,v31,v32,v33);

    // QR decomposition
    QRDecomposition(b11, b12, b13, b21, b22, b23, b31, b32, b33,
    u11, u12, u13, u21, u22, u23, u31, u32, u33,
    s11, s12, s13, s21, s22, s23, s31, s32, s33
    );
}

inline void Kabsch::QRDecomposition(// matrix that we want to decompose
                                     float b11, float b12, float b13,
                                     float b21, float b22, float b23,
                                     float b31, float b32, float b33,
                                     // output Q
                                     float &q11, float &q12, float &q13,
                                     float &q21, float &q22, float &q23,
                                     float &q31, float &q32, float &q33,
                                     // output R
                                     float &r11, float &r12, float &r13,
                                     float &r21, float &r22, float &r23,
                                     float &r31, float &r32, float &r33)
{
    float ch1,sh1,ch2,sh2,ch3,sh3;
    float a,b;

    // first givens rotation (ch,0,0,sh)
    QRGivensQuaternion(b11,b21,ch1,sh1);
    a = 1-2*sh1*sh1;
    b = 2*ch1*sh1;
    // apply B = Q' * B
    r11 = a*b11+b*b21;  r12 = a*b12+b*b22;  r13 = a*b13+b*b23;
    r21 = -b*b11+a*b21; r22 = -b*b12+a*b22; r23 = -b*b13+a*b23;
    r31 = b31;          r32 = b32;          r33 = b33;

    // second givens rotation (ch,0,-sh,0)
    QRGivensQuaternion(r11,r31,ch2,sh2);
    a = 1-2*sh2*sh2;
    b = 2*ch2*sh2;
    // apply B = Q' * B;
    b11 = a*r11+b*r31;  b12 = a*r12+b*r32;  b13 = a*r13+b*r33;
    b21 = r21;          b22 = r22;          b23 = r23;
    b31 = -b*r11+a*r31; b32 = -b*r12+a*r32; b33 = -b*r13+a*r33;

    // third givens rotation (ch,sh,0,0)
    QRGivensQuaternion(b22,b32,ch3,sh3);
    a = 1-2*sh3*sh3;
    b = 2*ch3*sh3;
    // R is now set to desired value
    r11 = b11;             r12 = b12;           r13 = b13;
    r21 = a*b21+b*b31;     r22 = a*b22+b*b32;   r23 = a*b23+b*b33;
    r31 = -b*b21+a*b31;    r32 = -b*b22+a*b32;  r33 = -b*b23+a*b33;

    // construct the cumulative rotation Q=Q1 * Q2 * Q3
    // the number of floating point operations for three quaternion multiplications
    // is more or less comparable to the explicit form of the joined matrix.
    // certainly more memory-efficient!
    float sh12 = sh1*sh1;
    float sh22 = sh2*sh2;
    float sh32 = sh3*sh3;

    q11 = (-1+2*sh12)*(-1+2*sh22);
    q12 = 4*ch2*ch3*(-1+2*sh12)*sh2*sh3+2*ch1*sh1*(-1+2*sh32);
    q13 = 4*ch1*ch3*sh1*sh3-2*ch2*(-1+2*sh12)*sh2*(-1+2*sh32);

    q21 = 2*ch1*sh1*(1-2*sh22);
    q22 = -8*ch1*ch2*ch3*sh1*sh2*sh3+(-1+2*sh12)*(-1+2*sh32);
    q23 = -2*ch3*sh3+4*sh1*(ch3*sh1*sh3+ch1*ch2*sh2*(-1+2*sh32));

    q31 = 2*ch2*sh2;
    q32 = 2*ch3*(1-2*sh22)*sh3;
    q33 = (-1+2*sh22)*(-1+2*sh32);
}

inline void Kabsch::QRGivensQuaternion(const float &a1, const float &a2, float &ch, float &sh)
{
    float epsilon = (float)EPSILON;
    float rho = accurateSqrt(a1*a1 + a2*a2);

    sh = rho > epsilon ? a2 : (float)0.;
    ch = std::fabs(a1) + std::fmax(rho,epsilon);
    bool b = a1 < 0;
    condSwap(b,sh,ch);
    float w = rsqrt(ch*ch+sh*sh);
    ch *= w;
    sh *= w;
}

inline void Kabsch::sortSingularValues(// matrix that we want to decompose
                                        float &b11, float &b12, float &b13,
                                        float &b21, float &b22, float &b23,
                                        float &b31, float &b32, float &b33,
                                        //sort V simultaneously
                                        float &v11, float &v12, float &v13,
                                        float &v21, float &v22, float &v23,
                                        float &v31, float &v32, float &v33)
{
    float rho1 = dist2(b11,b21,b31);
    float rho2 = dist2(b12,b22,b32);
    float rho3 = dist2(b13,b23,b33);
    bool c;
    c = rho1 < rho2;
    condNegSwap(c,b11,b12); condNegSwap(c,v11,v12);
    condNegSwap(c,b21,b22); condNegSwap(c,v21,v22);
    condNegSwap(c,b31,b32); condNegSwap(c,v31,v32);
    condSwap(c,rho1,rho2);
    c = rho1 < rho3;
    condNegSwap(c,b11,b13); condNegSwap(c,v11,v13);
    condNegSwap(c,b21,b23); condNegSwap(c,v21,v23);
    condNegSwap(c,b31,b33); condNegSwap(c,v31,v33);
    condSwap(c,rho1,rho3);
    c = rho2 < rho3;
    condNegSwap(c,b12,b13); condNegSwap(c,v12,v13);
    condNegSwap(c,b22,b23); condNegSwap(c,v22,v23);
    condNegSwap(c,b32,b33); condNegSwap(c,v32,v33);
}

inline void Kabsch::jacobiEigenanalysis(// symmetric matrix
                                        float &s11,
                                        float &s21, float &s22,
                                        float &s31, float &s32, float &s33,
                                        // quaternion representation of V
                                        float * qV)
{
    qV[3] = (float)1.; qV[0] = (float)0.; qV[1] = (float)0.; qV[2] = (float)0.; // follow same indexing convention as GLM
    for (int i = 0; i < 4; ++i)
    {
        // we wish to eliminate the maximum off-diagonal element
        // on every iteration, but cycling over all 3 possible rotations
        // in fixed order (p,q) = (1,2) , (2,3), (1,3) still retains
        //  asymptotic convergence
        jacobiConjugation(0,1,2,s11,s21,s22,s31,s32,s33,qV); // p,q = 0,1
        jacobiConjugation(1,2,0,s11,s21,s22,s31,s32,s33,qV); // p,q = 1,2
        jacobiConjugation(2,0,1,s11,s21,s22,s31,s32,s33,qV); // p,q = 0,2
    }
}

inline float Kabsch::dist2(const float &x, const float &y, const float &z)
{
    return x*x + y*y + z*z;
}

inline void Kabsch::jacobiConjugation(const int &x, const int &y, const int &z,
                                      float &s11,
                                      float &s21, float &s22,
                                      float &s31, float &s32, float &s33,
                                      float *qV)
{
    float ch,sh;
    approximateGivensQuaternion(s11,s21,s22,ch,sh);

    float scale = ch*ch+sh*sh;
    float a = (ch*ch-sh*sh)/scale;
    float b = (2*sh*ch)/scale;

    // make temp copy of S
    float _s11 = s11;
    float _s21 = s21; float _s22 = s22;
    float _s31 = s31; float _s32 = s32; float _s33 = s33;

    // perform conjugation S = Q'*S*Q
    // Q already implicitly solved from a, b
    s11 = a*(a*_s11 + b*_s21) + b*(a*_s21 + b*_s22);
    s21 = a*(-b*_s11 + a*_s21) + b*(-b*_s21 + a*_s22);  s22 = -b*(-b*_s11 + a*_s21) + a*(-b*_s21 + a*_s22);
    s31 = a*_s31 + b*_s32;                              s32 = -b*_s31 + a*_s32;                             s33=_s33;

    // update cumulative rotation qV
    float tmp[3];
    tmp[0] = qV[0]*sh;
    tmp[1] = qV[1]*sh;
    tmp[2] = qV[2]*sh;
    sh *= qV[3];

    qV[0] *= ch;
    qV[1] *= ch;
    qV[2] *= ch;
    qV[3] *= ch;

    // (x,y,z) corresponds to ((0,1,2),(1,2,0),(2,0,1))
    // for (p,q) = ((0,1),(1,2),(0,2))
    qV[z] += sh;
    qV[3] -= tmp[z]; // w
    qV[x] += tmp[y];
    qV[y] -= tmp[x];

    // re-arrange matrix for next iteration
    _s11 = s22;
    _s21 = s32; _s22 = s33;
    _s31 = s21; _s32 = s31; _s33 = s11;
    s11 = _s11;
    s21 = _s21; s22 = _s22;
    s31 = _s31; s32 = _s32; s33 = _s33;
}

inline void Kabsch::approximateGivensQuaternion(const float &a11, const float &a12, const float &a22, float &ch, float &sh)
{
    /*
     * Given givens angle computed by approximateGivensAngles,
     * compute the corresponding rotation quaternion.
     */
    ch = 2*(a11-a22);
    sh = a12;
    bool b = _gamma*sh*sh < ch*ch;
    // fast rsqrt function suffices
    // rsqrt2 (https://code.google.com/p/lppython/source/browse/algorithm/HDcode/newCode/rsqrt.c?r=26)
    // is even faster but results in too much error
    float w = rsqrt(ch*ch+sh*sh);
    ch = b ? w*ch : (float)_cstar;
    sh = b ? w*sh : (float)_sstar;
}

inline void Kabsch::quatfloatoMat3(const float *qV,
                               float &m11, float &m12, float &m13,
                               float &m21, float &m22, float &m23,
                               float &m31, float &m32, float &m33)
{
    float w = qV[3];
    float x = qV[0];
    float y = qV[1];
    float z = qV[2];

    float qxx = x*x;
    float qyy = y*y;
    float qzz = z*z;
    float qxz = x*z;
    float qxy = x*y;
    float qyz = y*z;
    float qwx = w*x;
    float qwy = w*y;
    float qwz = w*z;

    m11 = 1 - 2*(qyy + qzz);    m12 = 2*(qxy - qwz);        m13 = 2*(qxz + qwy);
    m21 = 2*(qxy + qwz);        m22 = 1 - 2*(qxx + qzz);    m23 = 2*(qyz - qwx);
    m31 = 2*(qxz - qwy);        m32 = 2*(qyz + qwx);        m33 = 1 - 2*(qxx + qyy);
}

inline void Kabsch::multAtB(const float& a11, const float& a12, const float& a13,
                            const float& a21, const float& a22, const float& a23,
                            const float& a31, const float& a32, const float& a33,
                            //
                            const float& b11, const float& b12, const float& b13,
                            const float& b21, const float& b22, const float& b23,
                            const float& b31, const float& b32, const float& b33,
                            //
                            float &m11, float &m12, float &m13,
                            float &m21, float &m22, float &m23,
                            float &m31, float &m32, float &m33)
{
    m11 = a11*b11 + a21*b21 + a31*b31;  m12 = a11*b12 + a21*b22 + a31*b32;  m13 = a11*b13 + a21*b23 + a31*b33;
    m21 = a12*b11 + a22*b21 + a32*b31;  m22 = a12*b12 + a22*b22 + a32*b32;  m23 = a12*b13 + a22*b23 + a32*b33;
    m31 = a13*b11 + a23*b21 + a33*b31;  m32 = a13*b12 + a23*b22 + a33*b32;  m33 = a13*b13 + a23*b23 + a33*b33;
}

inline void Kabsch::multAB( const float& a11, const float& a12, const float& a13,
                            const float& a21, const float& a22, const float& a23,
                            const float& a31, const float& a32, const float& a33,
                            //
                            const float& b11, const float& b12, const float& b13,
                            const float& b21, const float& b22, const float& b23,
                            const float& b31, const float& b32, const float& b33,
                            //
                            float &m11, float &m12, float &m13,
                            float &m21, float &m22, float &m23,
                            float &m31, float &m32, float &m33)
{
    m11 = a11*b11 + a12*b21 + a13*b31;  m12 = a11*b12 + a12*b22 + a13*b32;  m13 = a11*b13 + a12*b23 + a13*b33;
    m21 = a21*b11 + a22*b21 + a23*b31;  m22 = a21*b12 + a22*b22 + a23*b32;  m23 = a21*b13 + a22*b23 + a23*b33;
    m31 = a31*b11 + a32*b21 + a33*b31;  m32 = a31*b12 + a32*b22 + a33*b32;  m33 = a31*b13 + a32*b23 + a33*b33;
}

inline void Kabsch::condNegSwap(const bool &c, float &X, float &Y)
{
    // used in step 2 and 3
    float Z = -X;
    X = c ? Y : X;
    Y = c ? Z : Y;
}

inline void Kabsch::condSwap(const bool &c, float &X, float &Y)
{
    // used in step 2
    float Z = X;
    X = c ? Y : X;
    Y = c ? Z : Y;
}

inline float Kabsch::accurateSqrt(float x)
{
    return x * rsqrt1(x);
}

inline float Kabsch::rsqrt1(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int *)&x;          // View x as an int.
    i = 0x5f37599e - (i >> 1);   // Initial guess.
    x = *(float *)&i;            // View i as float.
    x = x*(1.5f - xhalf*x*x);    // Newton step.
    x = x*(1.5f - xhalf*x*x);    // Newton step again.
   return x;
}

inline float Kabsch::rsqrt(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int *)&x;          // View x as an int.
//  i = 0x5f3759df - (i >> 1);   // Initial guess (traditional).
    i = 0x5f375a82 - (i >> 1);   // Initial guess (slightly better).
    x = *(float *)&i;            // View i as float.
    x = x*(1.5f - xhalf*x*x);    // Newton step.
//  x = x*(1.5008908 - xhalf*x*x);  // Newton step for a balanced error.
   return x;
}

inline float Kabsch::dist(const float &x1, const float &y1, const float &z1, const float &x2, const float &y2, const float &z2)
{
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2);
}

inline float Kabsch::operator()(const Point &p, const Point &q)
{
    if(p.n != q.n){std::cerr << "Invalid number of points. In Kabsch algorithm points P and Q must have the same size." << std::endl; exit(1);}
    float   mpx = 0.f, mpy = 0.f, mpz = 0.f,
        mqx = 0.f, mqy = 0.f, mqz = 0.f,
        spx = 0.f, spy = 0.f, spz = 0.f,
        sqx = 0.f, sqy = 0.f, sqz = 0.f,
        delta_px, delta_py, delta_pz,
        delta_qx, delta_qy, delta_qz,

        C11 = 0.f, C12 = 0.f, C13 = 0.f, C21 = 0.f, C22 = 0.f, C23 = 0.f, C31 = 0.f, C32 = 0.f, C33 = 0.f,
        U11, U12, U13, U21, U22, U23, U31, U32, U33,
        S11, S12, S13, S21, S22, S23, S31, S32, S33,
        V11, V12, V13, V21, V22, V23, V31, V32, V33,
        R11, R12, R13,
        R21, R22, R23,
        R31, R32, R33,
        //C[3][3], U[3][3], S[3], V[3][3], R[3][3],

        tmpx, tmpy, tmpz,
        scale;
    bool d;
    //C[0][0] = 0.f; C[0][1] = 0.f; C[0][2] = 0.f;
    //C[1][0] = 0.f; C[1][1] = 0.f; C[1][2] = 0.f;
    //C[2][0] = 0.f; C[2][1] = 0.f; C[2][2] = 0.f;


    for(int i = 0; i < p.n; ++i)
    {
        delta_px = p.x[i] - mpx; delta_py = p.y[i] - mpy, delta_pz = p.z[i] - mpz;
        mpx += delta_px / (i+1);    mpy += delta_py / (i+1);    mpz += delta_pz / (i+1);
        spx += delta_px * (p.x[i] - mpx); spy += delta_py * (p.y[i] - mpy); spz += delta_pz * (p.z[i] - mpz);

        delta_qx = q.x[i] - mqx; delta_qy = q.y[i] - mqy, delta_qz = q.z[i] - mqz;
        mqx += delta_qx / (i+1);    mqy += delta_qy / (i+1);    mqz += delta_qz / (i+1);
        sqx += delta_qx * (q.x[i] - mqx); sqy += delta_qy * (q.y[i] - mqy); sqz += delta_qz * (q.z[i] - mqz);
    }

    for (int i = 0; i < p.n; ++i)
    {
        C11 += (p.x[i] - mpx) * (q.x[i] - mqx); C12 += (p.x[i] - mpx) * (q.y[i] - mqx); C13 += (p.x[i] - mpx) * (q.z[i] - mqx);
        C21 += (p.y[i] - mpy) * (q.x[i] - mqy); C22 += (p.y[i] - mpy) * (q.y[i] - mqy); C23 += (p.y[i] - mpy) * (q.z[i] - mqy);
        C31 += (p.z[i] - mpz) * (q.x[i] - mqz); C32 += (p.z[i] - mpz) * (q.y[i] - mqz); C33 += (p.z[i] - mpz) * (q.z[i] - mqz);
        //C[0][0] += (p.x[i] - mpx) * (q.x[i] - mqx); C[0][1] += (p.x[i] - mpx) * (q.y[i] - mqx); C[0][2] += (p.x[i] - mpx) * (q.z[i] - mqx);
        //C[1][0] += (p.y[i] - mpy) * (q.x[i] - mqy); C[1][1] += (p.y[i] - mpy) * (q.y[i] - mqy); C[1][2] += (p.y[i] - mpy) * (q.z[i] - mqy);
        //C[2][0] += (p.z[i] - mpz) * (q.x[i] - mqz); C[2][1] += (p.z[i] - mpz) * (q.y[i] - mqz); C[2][2] += (p.z[i] - mpz) * (q.z[i] - mqz);
    }
    //svd3((float *)U, S, (float *)V, (float *)C);
    svd3x3(C11, C12, C13, C21, C22, C23, C31, C32, C33,
           U11, U12, U13, U21, U22, U23, U31, U32, U33,
           S11, S12, S13, S21, S22, S23, S31, S32, S33,
           V11, V12, V13, V21, V22, V23, V31, V32, V33);

    //d = ( (V[0][0]*V[1][1]*V[2][2] + V[0][1]*V[1][2]*V[2][0] + V[0][2]*V[1][0]*V[2][1] - V[0][2]*V[1][1]*V[2][0] - V[0][0]*V[1][2]*V[2][1] - V[0][1]*V[1][0]*V[2][2]) *
    //      (U[0][0]*U[1][1]*U[2][2] + U[0][1]*U[1][2]*U[2][0] + U[0][2]*U[1][0]*U[2][1] - U[0][2]*U[1][1]*U[2][0] - U[0][0]*U[1][2]*U[2][1] - U[0][1]*U[1][0]*U[2][2])
    //  < 0.f);
    d = (   (V11*V22*V33 + V12*V23*V31 + V13*V21*V32 - V13*V22*V31 - V11*V23*V32 - V12*V21*V33) *
            (U11*U22*U33 + U12*U23*U31 + U13*U21*U32 - U13*U22*U31 - U11*U23*U32 - U12*U21*U33)
         < 0.f);

    if(d)
    {
        //S[2] = -S[2];
        S33 = -S33;
        //U[0][2] = -U[0][2];
        //U[1][2] = -U[1][2];
        //U[2][2] = -U[2][2];
        U13 = -U13;
        U23 = -U23;
        U33 = -U33;
    }
    R11 = U11*V11 + U12*V12 + U13*V13; R12 = U11*V21 + U12*V22 + U13*V23; R13 = U11*V31 + U12*V32 + U13*V33;
    R21 = U21*V11 + U22*V12 + U23*V13; R22 = U21*V21 + U22*V22 + U23*V23; R23 = U21*V31 + U22*V32 + U23*V33;
    R31 = U31*V11 + U32*V12 + U33*V13; R32 = U31*V21 + U32*V22 + U33*V23; R33 = U31*V31 + U32*V32 + U33*V33;
    //R[0][0] = U[0][0]*V[0][0] + U[0][1]*V[0][1] + U[0][2]*V[0][2]; R[0][1] = U[0][0]*V[1][0] + U[0][1]*V[1][1] + U[0][2]*V[1][2]; R[0][2] = U[0][0]*V[2][0] + U[0][1]*V[2][1] + U[0][2]*V[2][2];
    //R[1][0] = U[1][0]*V[0][0] + U[1][1]*V[0][1] + U[1][2]*V[0][2]; R[1][1] = U[1][0]*V[1][0] + U[1][1]*V[1][1] + U[1][2]*V[1][2]; R[1][2] = U[1][0]*V[2][0] + U[1][1]*V[2][1] + U[1][2]*V[2][2];
    //R[2][0] = U[2][0]*V[0][0] + U[2][1]*V[0][1] + U[2][2]*V[0][2]; R[2][1] = U[2][0]*V[1][0] + U[2][1]*V[1][1] + U[2][2]*V[1][2]; R[2][2] = U[2][0]*V[2][0] + U[2][1]*V[2][1] + U[2][2]*V[2][2];

    scale = (S11 + S22 + S33) / (spx + spy + spz);
    //scale = (S[0] + S[1] + S[2]) / (spx + spy + spz);
    float rmsd = 0.f;
    for (int i = 0; i < p.n; ++i)
    {
        tmpx = p.x[i]; tmpy = p.y[i]; tmpz = p.z[i];
        p.x[i] = ( (tmpx - mpx) * R11 + (tmpy - mpy) * R21 + (tmpz - mpz) * R13 ) * scale;
        p.y[i] = ( (tmpx - mpx) * R12 + (tmpy - mpy) * R22 + (tmpz - mpz) * R23 ) * scale;
        p.z[i] = ( (tmpx - mpx) * R13 + (tmpy - mpy) * R23 + (tmpz - mpz) * R33 ) * scale;
        //p.x[i] = ( (tmpx - mpx) * R[0][0] + (tmpy - mpy) * R[1][0] + (tmpz - mpz) * R[0][2] ) * scale;
        //p.y[i] = ( (tmpx - mpx) * R[0][1] + (tmpy - mpy) * R[1][1] + (tmpz - mpz) * R[1][2] ) * scale;
        //p.z[i] = ( (tmpx - mpx) * R[0][2] + (tmpy - mpy) * R[1][2] + (tmpz - mpz) * R[2][2] ) * scale;
        rmsd += dist(p.x[i], p.y[i], p.z[i], q.x[i] - mqx, q.y[i] - mqy, q.z[i] - mqz);
    }
    return std::sqrt(rmsd / p.n);
}
