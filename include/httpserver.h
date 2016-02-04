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

#ifndef PASTEC_HTTPSERVER_H
#define PASTEC_HTTPSERVER_H

#include <vector>
#include <string>
#include <microhttpd.h>

using namespace std;

class RequestHandler;
struct ConnectionInfo;


#define GET             0
#define POST            1
#define DELETE          2
#define PUT             3

class HTTPServer
{
public:
    HTTPServer(RequestHandler *requestHandler, unsigned i_port, bool https);
    ~HTTPServer();
    int run();
    int stop();

private:
    char *loadFile(const char *filename);
    static int answerToConnection(void *cls, MHD_Connection *connection,
                                  const char *url, const char *method,
                                  const char *version, const char *upload_data,
                                  size_t *upload_data_size, void **con_cls);
    static void requestCompleted(void *cls, MHD_Connection *connection,
                                 void **con_cls, MHD_RequestTerminationCode toe);
    static int sendAnswer(struct MHD_Connection *connection, ConnectionInfo &conInfo);
    static int readAuthHeader(void *cls, enum MHD_ValueKind kind,
                              const char *key, const char *value);

    MHD_Daemon *daemon;
    RequestHandler *requestHandler;

    unsigned i_port;
    bool https;

    pthread_cond_t stopCond;
    pthread_mutex_t stopMutex;
    bool b_stop;
};


struct ConnectionInfo
{
    int connectionType;
    string url;
    struct MHD_PostProcessor *postprocessor;
    string answerString;
    int answerCode;
    string authKey;

    vector<char> uploadedData;
};

#endif // PASTEC_HTTPSERVER_H
