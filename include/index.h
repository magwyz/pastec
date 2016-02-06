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

#ifndef PASTEC_INDEX_H
#define PASTEC_INDEX_H

#include <sys/types.h>
#include <list>
#include <string>

using namespace std;

class HitForward;

#define DEFAULT_INDEX_PATH "backwardIndex.dat"
#define DEFAULT_INDEX_TAGS_PATH "indexTags.dat"

class Index
{
public:
    virtual u_int32_t addTag(const unsigned i_imageId, const string tag) = 0;
    virtual u_int32_t removeImage(const unsigned i_imageId) = 0;
    virtual u_int32_t removeTag(const unsigned i_imageId) = 0;
    virtual u_int32_t getTag(unsigned i_imageId, string &tag) = 0;
    virtual u_int32_t write(string backwardIndexPath) = 0;
    virtual u_int32_t clear() = 0;
    virtual u_int32_t load(string backwardIndexPath) = 0;
    virtual u_int32_t getImageIds(vector<u_int32_t> &imageIds) = 0;

    virtual u_int32_t loadTags(string indexTagsPath) = 0;
    virtual u_int32_t writeTags(string indexTagsPath) = 0;
};

#endif // PASTEC_INDEX_H
