/*
 * Mutex.hpp
 *
 *  Created on: 16 сентября 2016 г.
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

#ifndef CODE_OS_INCLUDE_MUTEX_HPP_
#define CODE_OS_INCLUDE_MUTEX_HPP_

namespace OS {

class Mutex {
public:
	Mutex();
	virtual ~Mutex();

	typedef enum {
		NO_ERROR = 0,
		INVALID_MUTEX,
		OS_ERROR,
		TIMEOUT,
	} mutex_errors_t;

	/**
	 * @brief Lock mutex, wait for release if already locked. Unlimited wait time.
	 *
	 * @return error code from mutex_errors_t
	 */
	mutex_errors_t LockWait();

	/**
	 * @brief Release locked mutex
	 *
	 * @return error code from mutex_errors_t
	 */
	mutex_errors_t Release();
private:
	/**
	 * OS dependent data pointer
	 */
	void* m_mutex;
};

/**
 * @brief Scoped lock
 */
class MutexLock {
public:
	MutexLock(Mutex& mutex)
		: m_mutex(mutex) {
		m_mutex.LockWait();
	}
	~MutexLock() {
		m_mutex.Release();
	}

private:
	Mutex& m_mutex;
};

} // namespace OS

#endif /* CODE_OS_INCLUDE_MUTEX_HPP_ */
