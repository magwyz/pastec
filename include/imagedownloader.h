/*****************************************************************************
 * Copyright (C) 2018 Adrien Maglo
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

#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <string>
#include <vector>

#include <curl/curl.h>

//#define UNSAFE_HTTPS


class ImageDownloader
{
public:
    ImageDownloader();

    bool canDownloadImage(std::string imgURL);
    u_int32_t getImageData(std::string imgURL, std::vector<char> &imgData,
                           long &responseCode);

private:
    static size_t writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

};


#endif // IMAGEDOWNLOADER_H
