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

#ifndef PASTEC_SEARCHER_H
#define PASTEC_SEARCHER_H

#include <sys/types.h>
#include <vector>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class ClientConnection;


struct SearchRequest
{
    u_int32_t imageId;
    vector<char> imageData;
    ClientConnection *client;
    vector<u_int32_t> results;
    vector<Rect> boundingRects;
    vector<float> scores;
    vector<string> tags;
};


class Searcher
{
public:
    virtual u_int32_t searchImage(SearchRequest &request) = 0;
    virtual u_int32_t searchSimilar(SearchRequest &request) = 0;
};

#endif // PASTEC_SEARCHER_H
