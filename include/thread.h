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

#ifndef PASTEC_THREAD_H
#define PASTEC_THREAD_H

#include <pthread.h>
#include <assert.h>


class Thread
{
public:
    Thread() : b_mustStop(false) {}
    virtual ~Thread() {}
    void join()
    {
        b_mustStop = true;
        int i_ret = pthread_join(thread, NULL);
        assert(i_ret == 0);
    }

    bool start()
    {
        pthread_create(&thread, NULL, &runThread, this);
        return true;
    }

protected:
    static void *runThread(void *p)
    {
        Thread *newThread = (Thread *)p;
        newThread->b_mustStop = false;
        return newThread->run();
    }

    bool b_mustStop;

private:
    virtual void *run() = 0;

    pthread_t thread;
};

#endif // PASTEC_THREAD_H
