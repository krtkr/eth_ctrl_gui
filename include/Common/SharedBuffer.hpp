/*
 * SharedBuffer.hpp
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

#ifndef COMMON_SHAREDBUFFER_HPP_
#define COMMON_SHAREDBUFFER_HPP_

#include <string.h>

#include <OS/Mutex.hpp>

namespace Common {

/**
 * While no one asked anything more complicated use this stupid shared buffer
 */
template <typename T>
class SharedBuffer {
public:
	SharedBuffer()
		: local_value()
		, m_mutex()
	{
	};

	~SharedBuffer() {};

	/**
	 * @brief Copy data from local to shared.
	 *
	 * @return nothing
	 */
	void commit() {
		OS::MutexLock lock(m_mutex);
		memcpy(&m_shared_value, &local_value, sizeof(m_shared_value));
	};

	/**
	 * @brief Copy data from shared to local.
	 *
	 * @return nothing
	 */
	void fetch() {
		OS::MutexLock lock(m_mutex);
		memcpy(&local_value, &m_shared_value, sizeof(m_shared_value));
	};

	/**
	 * Use this local copy
	 */
	T local_value;

private:
	OS::Mutex m_mutex;

	/**
	 * Shared storage. Though it might be anything, we use static variable while
	 * all program is a single process. We could use shared memory here, for
	 * example.
	 */
	static T m_shared_value;
};

template <typename T> T SharedBuffer<T>::m_shared_value;

} // namespace Common

#endif /* COMMON_SHAREDBUFFER_HPP_ */
