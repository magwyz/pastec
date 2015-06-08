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

#ifndef PASTEC_ORBFEATUREEXTRACTOR_H
#define PASTEC_ORBFEATUREEXTRACTOR_H

#include <iostream>
#include <fstream>
#include <list>

#include <opencv2/core/core.hpp>

#include <orbindex.h>
#include <orbwordindex.h>
#include <featureextractor.h>

class ClientConnection;


using namespace cv;
using namespace std;


class ORBFeatureExtractor : public FeatureExtractor
{
public:
    ORBFeatureExtractor(ORBIndex *index, ORBWordIndex *wordIndex);
    virtual ~ORBFeatureExtractor() {}

    u_int32_t processNewImage(unsigned i_imageId, unsigned i_imgSize,
                              char *p_imgData, unsigned &i_nbFeaturesExtracted);

private:
    ORBIndex *index;
    ORBWordIndex *wordIndex;
};

#endif // PASTEC_ORBFEATUREEXTRACTOR_H
