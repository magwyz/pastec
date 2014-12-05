/*****************************************************************************
 * Copyright (C) 2014 Visualink
 *
 * Authors: Adrien Maglo <adrien@visualink.io>
 *
 * This file is part of Pastec.
 *
 * Pastec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pastec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Pastec.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef PASTEC_IMAGERERANKER_H
#define PASTEC_IMAGERERANKER_H

#include <sys/types.h>

#include <queue>
#include <list>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

#include <opencv2/core/core.hpp>

#include <thread.h>
#include <searchResult.h>
#include <hit.h>

using namespace std;
using namespace std::tr1;
using namespace cv;


class ImageReranker
{
public:
    ImageReranker() {}
    void rerank(unordered_map<u_int32_t, list<Hit> > &imagesReqHits,
                unordered_map<u_int32_t, vector<Hit> > &indexHits,
                priority_queue<SearchResult> &rankedResultsIn,
                priority_queue<SearchResult> &rankedResultsOut,
                unsigned i_nbResults);

private:
    float angleDiff(unsigned i_angle1, unsigned i_angle2);
    void getFirstImageIds(priority_queue<SearchResult> &rankedResultsIn,
                          unsigned i_nbResults, unordered_set<u_int32_t> &firstImageIds);
    cv::Mat findHomography(InputArray _points1, InputArray _points2,
                            int method, double ransacReprojThreshold,
                            OutputArray _mask);
    int cvFindHomography(const CvMat* objectPoints, const CvMat* imagePoints,
                         CvMat* __H, int method, double ransacReprojThreshold,
                         CvMat* mask);
};


// A task that must be performed when the rerankRANSAC function is called.
struct RANSACTask
{
    vector<Point2f> points1;
    vector<Point2f> points2;
};


#define HISTOGRAM_NB_BINS 32
#define DIFF_MIN -360.0f / (2.0f * HISTOGRAM_NB_BINS)

struct Histogram
{
    Histogram() : i_total(0)
    {
        for (unsigned i = 0; i < HISTOGRAM_NB_BINS; ++i)
            bins[i] = 0;
    }
    unsigned bins[HISTOGRAM_NB_BINS];
    unsigned i_total;
};


#define CV_RANSAC 8

template<typename T> int icvCompressPoints( T* ptr, const uchar* mask, int mstep, int count );

class CvModelEstimator2
{
public:
    CvModelEstimator2(int _modelPoints, CvSize _modelSize, int _maxBasicSolutions);
    virtual ~CvModelEstimator2();

    virtual int runKernel( const CvMat* m1, const CvMat* m2, CvMat* model )=0;
    virtual bool runLMeDS( const CvMat* m1, const CvMat* m2, CvMat* model,
                           CvMat* mask, double confidence=0.99, int maxIters=2000 );
    virtual bool runRANSAC( const CvMat* m1, const CvMat* m2, CvMat* model,
                            CvMat* mask, double threshold,
                            double confidence=0.99, int maxIters=2000 );
    virtual bool refine( const CvMat*, const CvMat*, CvMat*, int ) { return true; }
    virtual void setSeed( int64 seed );

protected:
    virtual void computeReprojError( const CvMat* m1, const CvMat* m2,
                                     const CvMat* model, CvMat* error ) = 0;
    virtual int findInliers( const CvMat* m1, const CvMat* m2,
                             const CvMat* model, CvMat* error,
                             CvMat* mask, double threshold );
    virtual bool getSubset( const CvMat* m1, const CvMat* m2,
                            CvMat* ms1, CvMat* ms2, int maxAttempts=1000 );
    virtual bool checkSubset( const CvMat* ms1, int count );

    CvRNG rng;
    int modelPoints;
    CvSize modelSize;
    int maxBasicSolutions;
    bool checkPartialSubsets;
};


class CvHomographyEstimator : public CvModelEstimator2
{
public:
    CvHomographyEstimator( int modelPoints );

    virtual int runKernel( const CvMat* m1, const CvMat* m2, CvMat* model );
    virtual bool refine( const CvMat* m1, const CvMat* m2,
                         CvMat* model, int maxIters );
protected:
    virtual void computeReprojError( const CvMat* m1, const CvMat* m2,
                                     const CvMat* model, CvMat* error );
};

#endif // PASTEC_IMAGERERANKER_H
