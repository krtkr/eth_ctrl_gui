/*
 * AbstractModule.hpp
 *
 *  Created on: 4 декабря 2016 г.
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

#ifndef COMMON_ABSTRACTMODULE_HPP_
#define COMMON_ABSTRACTMODULE_HPP_

#include <Common/SharedBuffer.hpp>
#include <OS/Thread.hpp>

namespace Common {

/**
 * Base control variables
 */
struct ControlBase {
	enum Commands {
		NONE = 0,
		START,
		STOP,
		PAUSE
	};
	Commands command;
};

/**
 * Base status variables
 */
struct StatusBase {
	int state;
	// Printable state string
	const char* state_name;
};

template<typename ControlT = ControlBase, typename StatusT = StatusBase>
class AbstractModule: public OS::ThreadInterface {
public:
	typedef Common::SharedBuffer<ControlT> ControlBuffer;
    typedef Common::SharedBuffer<StatusT> StatusBuffer;

protected:
    AbstractModule()
        : m_control()
		, m_status()
	{};

    virtual ~AbstractModule() {}

    /**
     * @brief getID - should return module's ID
     * @return ID of the module in the system
     */
    virtual int getID() = 0;

protected:
	ControlBuffer m_control;
	StatusBuffer m_status;
};

} // namespace Common

#endif /* COMMON_ABSTRACTMODULE_HPP_ */
