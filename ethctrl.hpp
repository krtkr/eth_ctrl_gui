/*
 * ethctrl.hpp
 *
 *  Created on: 5 декабря 2016 г.
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

#ifndef ETHCTRL_HPP
#define ETHCTRL_HPP

#include <Common/AbstractModule.hpp>

#include "eth_ctrl.h"

struct EthCtrlControl: public Common::ControlBase {
    const char* nic_name;
    int module_id;
    unsigned int period_ms;
};

struct EthCtrlStatus: public Common::StatusBase {
    eth_ctrl_error_code_t error_code;
    const char* error_message;
    bool connected;
};

typedef Common::SharedBuffer<eth_ctrl_rx_data_t> EthCtrlRxData;
typedef Common::SharedBuffer<eth_ctrl_tx_data_t> EthCtrlTxData;

class EthCtrl: public Common::AbstractModule<EthCtrlControl, EthCtrlStatus>
{
public:
    using Common::AbstractModule<EthCtrlControl, EthCtrlStatus>::ControlBuffer;
    using Common::AbstractModule<EthCtrlControl, EthCtrlStatus>::StatusBuffer;

    typedef enum {
            IDLE = 0,
            CONNECTING,
            SENDING_FRAME,
            RECEIVING_FRAME,
            WAITING,
            ERROR,
    } eth_ctrl_state_t;

    EthCtrl();
    virtual ~EthCtrl();

    int getID() {
        return 12;
    }

    bool operator()();

private:
    eth_ctrl_conn_t m_eth_ctrl_conn;

    EthCtrlRxData m_rxData;
    EthCtrlTxData m_txData;
};

#endif // ETHCTRL_HPP
