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


#include <imagedownloader.h>
#include <messages.h>


ImageDownloader::ImageDownloader()
{
    curl_global_init(CURL_GLOBAL_ALL);
}


bool ImageDownloader::canDownloadImage(std::string imgURL)
{
    bool ret = false;
    if (imgURL.substr(0, 7) == std::string("http://")
        || imgURL.substr(0, 8) == std::string("https://"))
        ret = true;
    return ret;
}


u_int32_t ImageDownloader::getImageData(std::string imgURL, std::vector<char> &imgData,
                                        long &responseCode)
{
    if (!canDownloadImage(imgURL))
        return ERROR_GENERIC;


    u_int32_t i_ret = OK;

    CURL *curlHandle = curl_easy_init();

    curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);

#ifdef UNSAFE_HTTPS
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)&imgData);

    curl_easy_setopt(curlHandle, CURLOPT_URL, imgURL.c_str());

    CURLcode res = curl_easy_perform(curlHandle);

    if(res == CURLE_OK)
    {
        curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &responseCode);
        if (responseCode != 200)
            i_ret = IMAGE_DOWNLOADER_HTTP_ERROR;
    }
    else
        i_ret = IMAGE_DOWNLOADER_HTTP_ERROR;

    curl_easy_cleanup(curlHandle);

    return i_ret;
}


size_t ImageDownloader::writeCallback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
    std::vector<char> *imgData = (std::vector<char> *)userdata;

    size_t sizeToWrite = size * nmemb;
    imgData->insert(imgData->end(), ptr, ptr + sizeToWrite);

    return sizeToWrite;
}


