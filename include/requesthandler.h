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

#ifndef PASTEC_REQUESTHANDLER_H
#define PASTEC_REQUESTHANDLER_H

#include <httpserver.h>

class FeatureExtractor;
class Searcher;
class Index;

using namespace std;

namespace Json {
    class Value;
}


class RequestHandler
{
public:
    RequestHandler(FeatureExtractor *featureExtractor,
                   Searcher *imageSearcher, Index *index, string authKey);
    void handleRequest(ConnectionInfo &conInfo);

private:
    vector<string> parseURI(string uri);
    bool testURIWithPattern(vector<string> parsedURI, string p_pattern[]);
    string JsonToString(Json::Value data);
    Json::Value StringToJson(string str);

    FeatureExtractor *featureExtractor;
    Searcher *imageSearcher;
    Index *index;

    string authKey;
};

#endif // PASTEC_REQUESTHANDLER_H
