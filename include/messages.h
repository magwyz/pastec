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

#ifndef PASTEC_MESSAGES_H
#define PASTEC_MESSAGES_H

#include <sys/types.h>
#include <string>

using namespace std;


enum MessagesOut
{
    OK =                                0x10010000,
    ERROR_GENERIC =                     0x10020000,
    MISFORMATTED_REQUEST =              0x10020100,
    AUTHENTIFICATION_ERROR =            0x10020200,
    PONG =                              0x10030000,
    TOO_MANY_CLIENTS =                  0x10040000,

    IMAGE_DATA_TOO_BIG =                0x10050100,
    IMAGE_NOT_INDEXED =                 0x10050200,
    IMAGE_NOT_DECODED =                 0x10050400,
    IMAGE_SIZE_TOO_SMALL =              0x10050500,
    IMAGE_NOT_FOUND =                   0x10050700,
    IMAGE_TAG_NOT_FOUND =               0x10050701,
    IMAGE_ADDED =                       0x10050800,
    IMAGE_REMOVED =                     0x10050900,
    IMAGE_TAG_ADDED =                   0x10051000,
    IMAGE_TAG_REMOVED =                 0x10051100,

    INDEX_LOADED =                      0x10060100,
    INDEX_TAGS_LOADED =                 0x10060110,
    INDEX_NOT_FOUND =                   0x10060200,
    INDEX_TAGS_NOT_FOUND =              0x10060210,
    INDEX_WRITTEN =                     0x10060300,
    INDEX_TAGS_WRITTEN =                0x10060310,
    INDEX_NOT_WRITTEN =                 0x10060400,
    INDEX_TAGS_NOT_WRITTEN =            0x10060410,
    INDEX_CLEARED =                     0x10060500,
    INDEX_IMAGE_IDS =                   0x10060600,

    SEARCH_RESULTS =                    0x10070100,
};


class Converter
{
public:
    /**
     * @brief Convert a return code to a string
     * @param r the code.
     * @return the code converted to a string.
     */
    static string codeToString(u_int32_t r)
    {
        string s = "???";
        switch (r)
        {
            case OK: s = "OK"; break;
            case ERROR_GENERIC: s = "ERROR_GENERIC"; break;
            case MISFORMATTED_REQUEST: s = "MISFORMATTED_REQUEST"; break;
            case AUTHENTIFICATION_ERROR: s = "AUTHENTIFICATION_ERROR"; break;
            case PONG: s = "PONG"; break;
            case TOO_MANY_CLIENTS: s = "TOO_MANY_CLIENTS"; break;

            case IMAGE_DATA_TOO_BIG: s = "IMAGE_DATA_TOO_BIG"; break;
            case IMAGE_NOT_INDEXED: s = "IMAGE_NOT_INDEXED"; break;
            case IMAGE_NOT_DECODED: s = "IMAGE_NOT_DECODED"; break;
            case IMAGE_SIZE_TOO_SMALL: s = "IMAGE_SIZE_TOO_SMALL"; break;
            case IMAGE_NOT_FOUND: s = "IMAGE_NOT_FOUND"; break;
            case IMAGE_TAG_NOT_FOUND: s = "IMAGE_TAG_NOT_FOUND"; break;
            case IMAGE_ADDED: s = "IMAGE_ADDED"; break;
            case IMAGE_REMOVED: s = "IMAGE_REMOVED"; break;
            case IMAGE_TAG_ADDED: s = "IMAGE_TAG_ADDED"; break;
            case IMAGE_TAG_REMOVED: s = "IMAGE_TAG_REMOVED"; break;

            case INDEX_LOADED: s = "INDEX_LOADED"; break;
            case INDEX_TAGS_LOADED: s = "INDEX_TAGS_LOADED"; break;
            case INDEX_NOT_FOUND: s = "INDEX_NOT_FOUND"; break;
            case INDEX_TAGS_NOT_FOUND: s = "INDEX_TAGS_NOT_FOUND"; break;
            case INDEX_WRITTEN: s = "INDEX_WRITTEN"; break;
            case INDEX_TAGS_WRITTEN: s = "INDEX_TAGS_WRITTEN"; break;
            case INDEX_NOT_WRITTEN: s = "INDEX_NOT_WRITTEN"; break;
            case INDEX_TAGS_NOT_WRITTEN: s = "INDEX_TAGS_NOT_WRITTEN"; break;
            case INDEX_CLEARED: s = "INDEX_CLEARED"; break;
            case INDEX_IMAGE_IDS: s = "INDEX_IMAGE_IDS"; break;

            case SEARCH_RESULTS: s = "SEARCH_RESULTS"; break;
            default: break;
        }
        return s;
    }
};

#define MSG_INDEX_IMAGE_HEADER_LEN 12
#define MSG_REMOVE_IMAGE_LEN 8


#endif // PASTEC_MESSAGES_H
