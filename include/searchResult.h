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

#ifndef PASTEC_SEARCHRESULT_H
#define PASTEC_SEARCHRESULT_H

#include <opencv2/core/core.hpp>

using namespace cv;


struct SearchResult
{
    SearchResult(float f_weight, unsigned i_imageId, Rect boundingRect)
        : f_weight(f_weight), i_imageId(i_imageId),
          boundingRect(boundingRect)
    {}

    bool operator< (const SearchResult &res) const
    {
        return f_weight < res.f_weight;
    }

    float f_weight;
    unsigned i_imageId;
    Rect boundingRect;
};

#endif // PASTEC_SEARCHRESULT_H
