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
#include <signal.h>

#include <httpserver.h>
#include <requesthandler.h>
#include <orb/orbfeatureextractor.h>
#include <orb/orbsearcher.h>
#include <orb/orbwordindex.h>


using namespace std;

HTTPServer *s;

void intHandler(int signum) {
    (void)signum;
    s->stop();
}


void printUsage()
{
    cout << "Usage :" << endl
         << "./PastecIndex [-p portNumber] [-i indexPath] [--forward-index] [--https] [--auth-key AuthKey] visualWordList" << endl;
}


int main(int argc, char** argv)
{
    cout << "Pastec Index v0.0.1" << endl;

    if (argc < 2)
    {
        printUsage();
        return 1;
    }

#define EXIT_IF_LAST_ARGUMENT() \
    if (i == argc - 1)          \
    {                           \
        printUsage();           \
        return 1;    \
    }

    unsigned i_port = 4212;
    string visualWordPath;
    string indexPath(DEFAULT_INDEX_PATH);
    bool buildForwardIndex = false;
    string authKey("");
    bool https = false;

    int i = 1;
    while (i < argc)
    {
        if (string(argv[i]) == "-p")
        {
            EXIT_IF_LAST_ARGUMENT()
            i_port = atoi(argv[++i]);
        }
        else if (string(argv[i]) == "-i")
        {
            EXIT_IF_LAST_ARGUMENT()
            indexPath = argv[++i];
        }
        else if (string(argv[i]) == "--auth-key")
        {
            EXIT_IF_LAST_ARGUMENT()
            authKey = argv[++i];
        }
        else if (string(argv[i]) == "--https")
        {
            https = true;
        }
        else if (string(argv[i]) == "--forward-index")
        {
            buildForwardIndex = true;
        }
        else if (i == argc - 1)
        {
            visualWordPath = argv[i];
        }
        else
        {
            printUsage();
            return 1;
        }
        ++i;
    }

    Index *index = new ORBIndex(indexPath, buildForwardIndex);
    ORBWordIndex *wordIndex = new ORBWordIndex(visualWordPath);
    FeatureExtractor *ife = new ORBFeatureExtractor((ORBIndex *)index, wordIndex);
    Searcher *is = new ORBSearcher((ORBIndex *)index, wordIndex);
    RequestHandler *rh = new RequestHandler(ife, is, index, authKey);
    s = new HTTPServer(rh, i_port, https);

    signal(SIGHUP, intHandler);
    signal(SIGINT, intHandler);

    s->run();

    cout << "Terminating Pastec." << endl;

    delete s;
    delete (ORBSearcher *)is;
    delete (ORBFeatureExtractor *)ife;
    delete (ORBIndex *)index;

    return 0;
}
