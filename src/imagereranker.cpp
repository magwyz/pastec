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

#include <iostream>
#include <cassert>
#include <math.h>

#include <algorithm>
#include <set>

#include "imagereranker.h"


void ImageReranker::rerank(unordered_map<u_int32_t, list<Hit> > &imagesReqHits,
                           unordered_map<u_int32_t, vector<Hit> > &indexHits,
                           priority_queue<SearchResult> &rankedResultsIn,
                           priority_queue<SearchResult> &rankedResultsOut,
                           unsigned i_nbResults)
{
    set<u_int32_t> firstImageIds;

    // Extract the first i_nbResults ranked images.
    getFirstImageIds(rankedResultsIn, i_nbResults, firstImageIds);

    map<u_int32_t, RANSACTask> imgTasks;

    // Compute the histograms.
    map<u_int32_t, Histogram> histograms; // key: the image id, value: the corresponding histogram.

    for (unordered_map<u_int32_t, list<Hit> >::const_iterator it = imagesReqHits.begin();
         it != imagesReqHits.end(); ++it)
    {
        // Try to match all the visual words of the request image.
        const unsigned i_wordId = it->first;
        const list<Hit> &hits = it->second;

        assert(hits.size() == 1);

        // If there is several hits for the same word in the image...
        const u_int16_t i_angle1 = hits.front().i_angle;
        const Point2f point1(hits.front().x, hits.front().y);
        const vector<Hit> &hitIndex = indexHits[i_wordId];

        for (unsigned i = 0; i < hitIndex.size(); ++i)
        {
            const u_int32_t i_imageId = hitIndex[i].i_imageId;
            // Test if the image belongs to the image to rerank.
            if (firstImageIds.find(i_imageId) != firstImageIds.end())
            {
                const u_int16_t i_angle2 = hitIndex[i].i_angle;
                float f_diff = angleDiff(i_angle1, i_angle2);
                unsigned bin = f_diff / 360 * HISTOGRAM_NB_BINS;
                assert(bin < HISTOGRAM_NB_BINS);

                histograms[i_imageId].bins[bin]++;
                histograms[i_imageId].i_total++;

                const Point2f point2(hitIndex[i].x, hitIndex[i].y);
                imgTasks[i_imageId].points1.push_back(point1);
                imgTasks[i_imageId].points2.push_back(point2);
            }
        }
    }

    // Rank the images according to their histogram.
    for (map<unsigned, Histogram>::const_iterator it = histograms.begin();
         it != histograms.end(); ++it)
    {
        const unsigned i_imageId = it->first;
        const Histogram &histogram = it->second;
        unsigned i_maxVal = *max_element(histogram.bins, histogram.bins + HISTOGRAM_NB_BINS);
        if (i_maxVal > 10)
        {
#if 1
            RANSACTask &task = imgTasks[i_imageId];
            assert(task.points1.size() == task.points2.size());

            #define MIN_NB_INLINERS 6

            if (task.points1.size() >= MIN_NB_INLINERS)
            {
                Mat mask;
                findHomography(task.points1, task.points2, CV_RANSAC, 1, mask);

                // Count the number of inliners.
                unsigned i_nbInliners = 0;
                for (unsigned i = 0; i < task.points1.size(); ++i)
                    if (mask.at<uchar>(0, i) == 1)
                        i_nbInliners++;

                if (i_nbInliners >= MIN_NB_INLINERS)
                    rankedResultsOut.push(SearchResult(i_maxVal, i_imageId));
                    //rankedResultsOut.push(SearchResult(i_nbInliners, i_imageId));
            }
#else
            rankedResultsOut.push(SearchResult(i_maxVal, i_imageId));
#endif
        }
    }
}


class Pos {
public:
    Pos(int x, int y) : x(x), y(y) {}

    inline bool operator< (const Pos &rhs) const {
        if (x != rhs.x)
            return x < rhs.x;
        else
            return y < rhs.y;
    }

private:
    int x, y;
};


/**
 * @brief Return the first ids of ranked images.
 * @param rankedResultsIn the ranked images.
 * @param i_nbResults the number of images to return.
 * @param firstImageIds a set to return the image ids.
 */
void ImageReranker::getFirstImageIds(priority_queue<SearchResult> &rankedResultsIn,
                                     unsigned i_nbResults, set<u_int32_t> &firstImageIds)
{
    unsigned i_res = 0;
    while(!rankedResultsIn.empty()
          && i_res < i_nbResults)
    {
        const SearchResult &res = rankedResultsIn.top();
        firstImageIds.insert(res.i_imageId);
        rankedResultsIn.pop();
        i_res++;
    }
}


float ImageReranker::angleDiff(unsigned i_angle1, unsigned i_angle2)
{
    // Convert the angle in the [-180, 180] range.
    float i1 = (float)i_angle1 * 360 / (1 << 16);
    float i2 = (float)i_angle2 * 360 / (1 << 16);

    i1 = i1 <= 180 ? i1 : i1 - 360;
    i2 = i2 <= 180 ? i2 : i2 - 360;

    // Compute the difference between the two angles.
    float diff = i1 - i2;
    if (diff < 0)
        diff += 360;

    assert(diff >= 0);
    assert(diff < 360);

    return diff;
}
