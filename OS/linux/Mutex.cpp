/*
 * Mutex.cpp
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

#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>

#include <OS/Mutex.hpp>

namespace OS {

Mutex::Mutex()
		: m_mutex(0) {
	errno = 0;
	m_mutex = malloc(sizeof(pthread_mutex_t));
	if(m_mutex != NULL) {
		pthread_mutex_t* tPthreadMutex = (pthread_mutex_t*)m_mutex;
		errno = 0;
		if(pthread_mutex_init(tPthreadMutex, NULL) != 0) {
			perror("Unable to create pthread mutex");
			free(m_mutex);
			m_mutex = 0;
		}
	} else {
		perror("Unable to allocate memory for new Mutex");
	}
};

Mutex::~Mutex() {
	pthread_mutex_t* tPthreadMutex = (pthread_mutex_t*)m_mutex;
	if(tPthreadMutex != NULL) {
		pthread_mutex_destroy(tPthreadMutex);
		free(m_mutex);
		m_mutex = 0;
	}
};

Mutex::mutex_errors_t Mutex::LockWait() {
	pthread_mutex_t* tPthreadMutex = (pthread_mutex_t*)m_mutex;
	if(tPthreadMutex != NULL) {
		errno = 0;
		if(pthread_mutex_lock(tPthreadMutex) != 0) {
			perror("Unable to lock pthread mutex");
			return (OS_ERROR);
		} else {
			return (NO_ERROR);
		}
	} else {
		return (INVALID_MUTEX);
	}
};

Mutex::mutex_errors_t Mutex::Release() {
	pthread_mutex_t* tPthreadMutex = (pthread_mutex_t*)m_mutex;
	if(tPthreadMutex != NULL) {
		errno = 0;
		if(pthread_mutex_unlock(tPthreadMutex) != 0) {
			perror("Unable to release pthread mutex");
			return (OS_ERROR);
		} else {
			return (NO_ERROR);
		}
	} else {
		return (INVALID_MUTEX);
	}
};

} // namespace OS
