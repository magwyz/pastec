/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

// Adrien Maglo: this file contains modified code from OpenCV.

#include <iostream>

#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <imagereranker.h>


void RANSACThread::getRTMatrix(const Point2f* a, const Point2f* b,
                               int count, Mat& M, bool fullAffine)
{
    CV_Assert( M.isContinuous() );

    if( fullAffine )
    {
        double sa[6][6]={{0.}}, sb[6]={0.};
        Mat A( 6, 6, CV_64F, &sa[0][0] ), B( 6, 1, CV_64F, sb );
        Mat MM = M.reshape(1, 6);

        for( int i = 0; i < count; i++ )
        {
            sa[0][0] += a[i].x*a[i].x;
            sa[0][1] += a[i].y*a[i].x;
            sa[0][2] += a[i].x;

            sa[1][1] += a[i].y*a[i].y;
            sa[1][2] += a[i].y;

            sa[2][2] += 1;

            sb[0] += a[i].x*b[i].x;
            sb[1] += a[i].y*b[i].x;
            sb[2] += b[i].x;
            sb[3] += a[i].x*b[i].y;
            sb[4] += a[i].y*b[i].y;
            sb[5] += b[i].y;
        }

        sa[3][4] = sa[4][3] = sa[1][0] = sa[0][1];
        sa[3][5] = sa[5][3] = sa[2][0] = sa[0][2];
        sa[4][5] = sa[5][4] = sa[2][1] = sa[1][2];

        sa[3][3] = sa[0][0];
        sa[4][4] = sa[1][1];
        sa[5][5] = sa[2][2];

        solve( A, B, MM, DECOMP_EIG );
    }
    else
    {
        double sa[4][4]={{0.}}, sb[4]={0.}, m[4];
        Mat A( 4, 4, CV_64F, sa ), B( 4, 1, CV_64F, sb );
        Mat MM( 4, 1, CV_64F, m );

        for( int i = 0; i < count; i++ )
        {
            sa[0][0] += a[i].x*a[i].x + a[i].y*a[i].y;
            sa[0][2] += a[i].x;
            sa[0][3] += a[i].y;


            sa[2][1] += -a[i].y;
            sa[2][2] += 1;

            sa[3][0] += a[i].y;
            sa[3][1] += a[i].x;
            sa[3][3] += 1;

            sb[0] += a[i].x*b[i].x + a[i].y*b[i].y;
            sb[1] += a[i].x*b[i].y - a[i].y*b[i].x;
            sb[2] += b[i].x;
            sb[3] += b[i].y;
        }

        sa[1][1] = sa[0][0];
        sa[2][1] = sa[1][2] = -sa[0][3];
        sa[3][1] = sa[1][3] = sa[2][0] = sa[0][2];
        sa[2][2] = sa[3][3] = count;
        sa[3][0] = sa[0][3];

        solve( A, B, MM, DECOMP_EIG );

        double* om = M.ptr<double>();
        om[0] = om[4] = m[0];
        om[1] = -m[1];
        om[3] = m[1];
        om[2] = m[2];
        om[5] = m[3];
    }
}


cv::Mat RANSACThread::pastecEstimateRigidTransform(InputArray src1, InputArray src2,
                                                   bool fullAffine)
{
    Mat M(2, 3, CV_64F), A = src1.getMat(), B = src2.getMat();

    const int RANSAC_MAX_ITERS = 5000;
    const int RANSAC_SIZE0 = 3;

    std::vector<Point2f> pA, pB;
    std::vector<int> good_idx;

    int i, j, k, k1;

    RNG rng((uint64)-1);
    int good_count = 0;

    assert( A.size() == B.size() );

    assert( A.type() == B.type() );

    int count = A.checkVector(2);

    if( count > 0 )
    {
        A.reshape(2, count).convertTo(pA, CV_32F);
        B.reshape(2, count).convertTo(pB, CV_32F);
    }

    good_idx.resize(count);

    if( count < RANSAC_SIZE0 )
        return Mat();

    Rect bRect;
    vector<cv::Point2f> bRectPoints(4);
    vector<cv::Point2f> bRectPointsProj(4);

    // RANSAC stuff:
    // 1. find the consensus
    for( k = 0; k < RANSAC_MAX_ITERS; k++ )
    {
        int idx[RANSAC_SIZE0];
        Point2f a[RANSAC_SIZE0];
        Point2f b[RANSAC_SIZE0];

        // choose random 3 non-complanar points from A & B
        for( i = 0; i < RANSAC_SIZE0; i++ )
        {
            for( k1 = 0; k1 < RANSAC_MAX_ITERS; k1++ )
            {
                idx[i] = rng.uniform(0, count);

                for( j = 0; j < i; j++ )
                {
                    if( idx[j] == idx[i] )
                        break;
                    // check that the points are not very close one each other
                    float u = pA[idx[i]].x - pA[idx[j]].x;
                    float v = pA[idx[i]].y - pA[idx[j]].y;
                    if( u * u + v * v < 20 * 20 )
                        break;
                    u = pB[idx[i]].x - pB[idx[j]].x;
                    v = pB[idx[i]].y - pB[idx[j]].y;
                    if( u * u + v * v < 20 * 20 )
                        break;
                }

                if( j < i )
                    continue;

                if( i+1 == RANSAC_SIZE0 )
                {
                    // additional check for non-complanar vectors
                    a[0] = pA[idx[0]];
                    a[1] = pA[idx[1]];
                    a[2] = pA[idx[2]];

                    b[0] = pB[idx[0]];
                    b[1] = pB[idx[1]];
                    b[2] = pB[idx[2]];

                    double dax1 = a[1].x - a[0].x, day1 = a[1].y - a[0].y;
                    double dax2 = a[2].x - a[0].x, day2 = a[2].y - a[0].y;
                    double dbx1 = b[1].x - b[0].x, dby1 = b[1].y - b[0].y;
                    double dbx2 = b[2].x - b[0].x, dby2 = b[2].y - b[0].y;
                    const double eps = 0.2;

                    if( fabs(dax1*day2 - day1*dax2) < eps*std::sqrt(dax1*dax1+day1*day1)*std::sqrt(dax2*dax2+day2*day2) ||
                        fabs(dbx1*dby2 - dby1*dbx2) < eps*std::sqrt(dbx1*dbx1+dby1*dby1)*std::sqrt(dbx2*dbx2+dby2*dby2) )
                        continue;
                }
                break;
            }

            if( k1 >= RANSAC_MAX_ITERS )
                break;
        }

        if( i < RANSAC_SIZE0 )
            continue;

        // estimate the transformation using 3 points
        getRTMatrix( a, b, 3, M, fullAffine );

        bRect = boundingRect(src1);
        bRectPoints[0] = Point2f(bRect.x, bRect.y);
        bRectPoints[1] = Point2f(bRect.x + bRect.width, bRect.y);
        bRectPoints[2] = Point2f(bRect.x + bRect.width, bRect.y + bRect.height);
        bRectPoints[3] = Point2f(bRect.x, bRect.y + bRect.height);

        transform(bRectPoints, bRectPointsProj, M);

        bool b_transformOk = true;
        for (unsigned l = 0; l < 4; ++l)
        {
            Point2f v1 = bRectPointsProj[(l + 2) % 4] - bRectPointsProj[(l + 1) % 4];
            Point2f v2 = bRectPointsProj[l] - bRectPointsProj[(l + 1) % 4];

            float angle = atan2(v1.x * v2.y - v1.y * v2.x, v1.x * v2.x + v1.y * v2.y);
            if (angle < 20 * M_PI / 180 || angle > 135 * M_PI / 180 || norm(v1) < 100)
            {
                b_transformOk = false;
                break;
            }
        }

        if (!b_transformOk)
            continue;

        const double* m = M.ptr<double>();
        for( i = 0, good_count = 0; i < count; i++ )
        {
            float a = m[0]*pA[i].x + m[1]*pA[i].y + m[2] - pB[i].x;
            float b = m[3]*pA[i].x + m[4]*pA[i].y + m[5] - pB[i].y;
            //cout << sqrt(a * a + b * b) << ": " << a * a + b * b << endl;
            if( a * a + b * b < 3 * 3 )
                good_idx[good_count++] = i;
        }

        if(good_count >= RANSAC_MIN_INLINERS)
            break;
    }

    if( k >= RANSAC_MAX_ITERS )
        return Mat();

    if( good_count < count )
    {
        for( i = 0; i < good_count; i++ )
        {
            j = good_idx[i];
            pA[i] = pA[j];
            pB[i] = pB[j];
        }
    }

    getRTMatrix( &pA[0], &pB[0], good_count, M, fullAffine );

    return M;
}
