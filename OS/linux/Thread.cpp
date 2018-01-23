/*
 * Thread.cpp
 *
 *  Created on: 17 сентября 2016 г.
 *      Author: krtkr
 *
 * MIT License
 *
 * Copyright (c) 2016 Kirill Kranke (krtkr), kranke.kirill@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <malloc.h>

#include <OS/Thread.hpp>

namespace OS {

struct os_thread_data {
	pthread_cond_t cv;
	pthread_mutex_t mp;
	pthread_t th;
};

void * thread_handler(void * ptUserData) {
	(*(Thread*)ptUserData)();
	pthread_exit(NULL);
};

void ThreadInterface::kick() {
	if(m_thread) {
		m_thread->kick();
	} else {
		fprintf(stderr, "Attempt to call ThreadInterface::kick without corresponding Thread instance\n");
	}
};

bool ThreadInterface::Sleep_ms(unsigned int time_ms) {
	if(m_thread) {
		return m_thread->Sleep_ms(time_ms);
	} else {
		fprintf(stderr, "Attempt to call ThreadInterface::Sleep_us without corresponding Thread instance\n");
		return false;
	}
};

Thread::Thread(ThreadInterface& iface)
		: m_iface(iface)
		, m_thread(0)
		, m_state(STOPPED) {
	errno = 0;
	m_thread = malloc(sizeof(os_thread_data));
	if(m_thread) {
		os_thread_data* os = (os_thread_data*)m_thread;
		pthread_condattr_t attr;
		pthread_condattr_init(&attr);
		pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
		pthread_cond_init(&os->cv, &attr);

		pthread_mutex_init(&os->mp, NULL);
	} else {
		perror("Unable to allocate memory for new Thread");
	}
	m_iface.m_thread = this;
};

Thread::~Thread() {
	os_thread_data* os = (os_thread_data*)m_thread;
	if(m_state == OPERATING) {
		fprintf(stderr, "Tread did not stop correctly, calling cance\n");
		errno = 0;
		if(pthread_cancel(os->th)) {
			perror("Error while canceling thread");
		}
	}
	if(m_thread) {
		pthread_join(os->th, NULL);
		pthread_cond_destroy(&os->cv);
		pthread_mutex_destroy(&os->mp);
		free(m_thread);
		m_thread = 0;
	}
	m_iface.m_thread = 0;
};

Thread::thread_error_t Thread::start() {
	os_thread_data* os = (os_thread_data*)m_thread;
	if(os) {
		if(m_state != OPERATING) {
			errno = 0;
			if(pthread_create(&os->th, NULL, thread_handler, (void*)this)) {
				perror("Unable to create thread");
				return(OS_ERROR);
			} else {
				return(NO_ERROR);
			}
		} else {
			return(THREAD_PENDING);
		}
	} else {
		return(INVALID_THREAD);
	}
};

void Thread::stop() {
	if(m_state == OPERATING) {
		m_iface.setStopRequest(true);
		kick();
	}
};

void Thread::kick() {
	os_thread_data* os = (os_thread_data*)m_thread;
	if(os) {
		errno = 0;
		if (pthread_cond_signal(&os->cv)) {
			perror("Error on pthread_cond_signal");
		}
	} else {
		fprintf(stderr, "Error, thread is not created");
	}
};

void Thread::operator()() {
	m_state = OPERATING;
	m_iface.setStopRequest(false);
	if(m_iface()) {
		m_state = FINISHED;
	} else {
		m_state = THREAD_ERROR;
	}
};

bool Thread::Sleep_ms(unsigned int time_ms) {
	os_thread_data* os = (os_thread_data*)m_thread;
	if(os) {
		timespec tp;
		timespec now;
		int retVal;
		clock_gettime(CLOCK_MONOTONIC, &now);
        tp.tv_nsec = (now.tv_nsec + time_ms % 1000 * 1000000) % 1000000000;
        tp.tv_sec = now.tv_sec + time_ms / 1000 + (now.tv_nsec + time_ms % 1000 * 1000000) / 1000000000;
		errno = 0;
		retVal = pthread_cond_timedwait(&os->cv, &os->mp, &tp);
		if (retVal == ETIMEDOUT) {
			return true;
		} else {
			if (retVal ==  EINVAL) {
				perror("Error on pthread_cond_timedwait");
			}
			return false;
		}
	} else {
		fprintf(stderr, "Error, thread is not created");
		return false;
	}
};

} // namespace OS
