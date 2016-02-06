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
#include <set>
#include <unordered_set>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <orbfeatureextractor.h>
#include <messages.h>
#include <imageloader.h>


ORBFeatureExtractor::ORBFeatureExtractor(ORBIndex *index, ORBWordIndex *wordIndex)
    : index(index), wordIndex(wordIndex)
{ }


u_int32_t ORBFeatureExtractor::processNewImage(unsigned i_imageId, unsigned i_imgSize,
                                               char *p_imgData, unsigned &i_nbFeaturesExtracted)
{
    Mat img;
    u_int32_t i_ret = ImageLoader::loadImage(i_imgSize, p_imgData, img);
    if (i_ret != OK)
        return i_ret;

    //equalizeHist( img, img );

    vector<KeyPoint> keypoints;
    Mat descriptors;

    ORB(2000, 1.02, 100)(img, noArray(), keypoints, descriptors);
    i_nbFeaturesExtracted = keypoints.size();

    unsigned i_nbKeyPoints = 0;
    list<HitForward> imageHits;
    unordered_set<u_int32_t> matchedWords;
    for (unsigned i = 0; i < keypoints.size(); ++i)
    {
        i_nbKeyPoints++;

        // Recording the angle on 16 bits.
        u_int16_t angle = keypoints[i].angle / 360 * (1 << 16);
        u_int16_t x = keypoints[i].pt.x;
        u_int16_t y = keypoints[i].pt.y;

        vector<int> indices(1);
        vector<int> dists(1);
        wordIndex->knnSearch(descriptors.row(i), indices, dists, 1);

        for (unsigned j = 0; j < indices.size(); ++j)
        {
            const unsigned i_wordId = indices[j];
            if (matchedWords.find(i_wordId) == matchedWords.end())
            {
                HitForward newHit;
                newHit.i_wordId = i_wordId;
                newHit.i_imageId = i_imageId;
                newHit.i_angle = angle;
                newHit.x = x;
                newHit.y = y;
                imageHits.push_back(newHit);
                matchedWords.insert(i_wordId);
            }
        }
    }

#if 0
    // Draw keypoints.
    Mat img_res;
    drawKeypoints(img, keypoints, img_res, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    // Show the image.
    imshow("Keypoints 1", img_res);
    waitKey();
#endif

    // Record the hits.
    return index->addImage(i_imageId, imageHits);
}
