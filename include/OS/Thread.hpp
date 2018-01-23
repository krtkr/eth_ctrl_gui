/*
 * Thread.hpp
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

#ifndef CODE_OS_INCLUDE_THREAD_HPP_
#define CODE_OS_INCLUDE_THREAD_HPP_

namespace OS {

class Thread;

class ThreadInterface {

friend class Thread;

public:
	/**
	 * @brief Kick sleeping thread if any.
	 *
	 * @return nothing
	 */
	void kick();

protected:
	ThreadInterface()
		: m_stop_request(false)
		, m_thread(0)
	{};
	virtual ~ThreadInterface() {};

	/**
	 * @brief Implement work cycle in this function, use 'm_stop_request' to
	 * check for stop request from world. Return from this function ASAP
	 * on stop request.
	 *
	 * @return true on successful finish, false on error
	 */
	virtual bool operator()() = 0;

	/**
	 * @brief Suspend calling thread
	 *
	 * @param [in]time_ms sleep time in milliseconds
	 *
	 * @return false if awaken, true otherwise
	 */
	bool Sleep_ms(unsigned int time_ms);

	/**
	 * @brief Poll stop request inside of operator() function implementation to
	 * process proper shutdown of your process.
	 *
	 * @return stop request
	 */
	bool getStopRequest() {
		return m_stop_request;
	}

private:
	/**
	 * @brief This function will be called from threads controller to set
	 * start/stop request.
	 *
	 * @param [in]timeout_us timeout in microseconds
	 *
	 * @return nothing
	 */
	void setStopRequest(bool stop_request) {
		m_stop_request = stop_request;
	};

	bool m_stop_request;

	Thread* m_thread;
};

class Thread {

friend class ThreadInterface;

public:
	Thread(ThreadInterface& iface);
	~Thread();

	typedef enum {
		NO_ERROR = 0,
		INVALID_THREAD,
		OS_ERROR,
		THREAD_PENDING,
	} thread_error_t;

	typedef enum {
		STOPPED = 0,
		OPERATING,
		FINISHED,
		THREAD_ERROR,
	} thread_state_t;

	/**
	 * @brief Start thread.
	 *
	 * @return error code from thread_error_t
	 */
	thread_error_t start();

	/**
	 * @brief Get current status of thread.
	 *
	 * @return status code from thread_state_t
	 */
	thread_state_t state() {
		return m_state;
	}

	/**
	 * @brief Stop thread. This function will attempt to stop thread normally
	 * by calling corresponding stop request.
	 *
	 * @return nothing
	 */
	void stop();

	/**
	 * @brief Kick sleeping thread if any.
	 *
	 * @return nothing
	 */
	void kick();

	/**
	 * @brief Thread main cycle, will be called within new thread.
	 *
	 * @return nothing
	 */
	void operator()();

private:
	/**
	 * @brief Suspend calling thread
	 *
	 * @param [in]time_ms sleep time in milliseconds
	 *
	 * @return false if awaken, true otherwise
	 */
	bool Sleep_ms(unsigned int time_ms);

	ThreadInterface& m_iface;

	/**
	 * OS dependent data pointer
	 */
	void* m_thread;

	thread_state_t m_state;
};

} // namespace OS

#endif /* CODE_OS_INCLUDE_THREAD_HPP_ */
