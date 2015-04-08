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

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <string.h>

#include <httpserver.h>
#include <messages.h>
#include <requesthandler.h>

#define PORT            4213


HTTPServer::HTTPServer(RequestHandler *requestHandler, unsigned i_port)
    : daemon(NULL), requestHandler(requestHandler), i_port(i_port),
      b_stop(false)
{
    pthread_mutex_init(&stopMutex, NULL);
    pthread_cond_init(&stopCond, NULL);
}


HTTPServer::~HTTPServer()
{
    pthread_cond_destroy(&stopCond);
    pthread_mutex_destroy(&stopMutex);
}


int HTTPServer::run()
{
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, i_port, NULL, NULL,
                              &answerToConnection, this,
                              MHD_OPTION_NOTIFY_COMPLETED, requestCompleted,
                              NULL, MHD_OPTION_END);
    if (daemon == NULL)
        return ERROR_GENERIC;

    cout << "Ready to accept querries." << endl;

    pthread_mutex_lock(&stopMutex);
    while (!b_stop)
        pthread_cond_wait(&stopCond, &stopMutex);
    pthread_mutex_unlock(&stopMutex);

    MHD_stop_daemon(daemon);

    return OK;
}


int HTTPServer::stop()
{
    uint64_t i_val = 1;
    pthread_mutex_lock(&stopMutex);
    b_stop = true;
    pthread_cond_signal(&stopCond);
    pthread_mutex_unlock(&stopMutex);
    return OK;
}


int HTTPServer::sendAnswer(struct MHD_Connection *connection, ConnectionInfo &conInfo)
{
    int ret;
    struct MHD_Response *response;

    const char *buffer = conInfo.answerString.c_str();

    response = MHD_create_response_from_buffer(strlen(buffer),
                                               (void *)buffer,
                                               MHD_RESPMEM_MUST_COPY);
    if (!response)
        return MHD_NO;

    ret = MHD_queue_response(connection, conInfo.answerCode, response);
    MHD_destroy_response(response);

    return ret;
}


void HTTPServer::requestCompleted(void *cls, MHD_Connection *connection,
                                  void **conCls, enum MHD_RequestTerminationCode toe)
{
    (void)cls, (void)connection; (void)toe;
    ConnectionInfo *conInfo = (ConnectionInfo *)*conCls;

    if (conInfo == NULL)
        return;

    if (conInfo->connectionType == POST
        && conInfo->postprocessor != NULL)
        MHD_destroy_post_processor(conInfo->postprocessor);

    delete conInfo;
    *conCls = NULL;
}


int HTTPServer::answerToConnection(void *cls, MHD_Connection *connection,
                                   const char *url, const char *method,
                                   const char *version, const char *upload_data,
                                   size_t *upload_data_size, void **conCls)
{
    (void)version;
    HTTPServer *s = (HTTPServer *)cls;
    ConnectionInfo *conInfo;

    if (*conCls == NULL)
    {
        conInfo = new ConnectionInfo();
        if (conInfo == NULL)
            return MHD_NO;

        if (strcmp(method, "POST") == 0)
            conInfo->connectionType = POST;
        else if (strcmp(method, "GET") == 0)
            conInfo->connectionType = GET;
        else if (strcmp(method, "DELETE") == 0)
            conInfo->connectionType = DELETE;
        else if (strcmp(method, "PUT") == 0)
            conInfo->connectionType = PUT;

        conInfo->url = string(url);

        *conCls = (void *) conInfo;

        return MHD_YES;
    }

    conInfo = (ConnectionInfo *)*conCls;

    if (conInfo->connectionType == GET
        || conInfo->connectionType == DELETE)
        s->requestHandler->handleRequest(*conInfo);
    else if (conInfo->connectionType == POST
             || conInfo->connectionType == PUT)
    {
        if (*upload_data_size != 0)
        {
            conInfo->uploadedData.insert(conInfo->uploadedData.end(),
                upload_data, upload_data + *upload_data_size);
            *upload_data_size = 0;

            return MHD_YES;
        }
        else
            s->requestHandler->handleRequest(*conInfo);
    }

    return sendAnswer(connection, *conInfo);
}


