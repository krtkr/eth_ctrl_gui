/*
 * ethctrl.cpp
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

#include "ethctrl.hpp"

static const char* state_names[6] = { "idle", "connecting", "sending frame", "receiving frame", "waiting", "error" };

EthCtrl::EthCtrl()
    : Common::AbstractModule<EthCtrlControl, EthCtrlStatus>()
    , m_eth_ctrl_conn()
    , m_rxData()
    , m_txData()
{
    m_eth_ctrl_conn.connection = 0;
}

EthCtrl::~EthCtrl()
{
    if (m_eth_ctrl_conn.connection) {
        eth_ctrl_close(&m_eth_ctrl_conn);
    }
}

bool EthCtrl::operator()()
{
    while (!this->getStopRequest()) {
        m_control.fetch();
        switch (m_status.local_value.state) {
            case SENDING_FRAME:
                if(m_control.local_value.command == Common::ControlBase::STOP) {
                    m_status.local_value.state = IDLE;
                } else {
                    m_txData.fetch();
                    if (eth_ctrl_send(&m_eth_ctrl_conn, &m_txData.local_value, m_control.local_value.module_id) != 0) {
                        m_status.local_value.state = ERROR;
                    } else {
                        m_status.local_value.state = RECEIVING_FRAME;
                    }
                }
                break;
            case RECEIVING_FRAME:
                if(m_control.local_value.command == Common::ControlBase::STOP) {
                    m_status.local_value.state = IDLE;
                } else {
                    if (eth_ctrl_recv(&m_eth_ctrl_conn, &m_rxData.local_value) != m_control.local_value.module_id) {
                        m_status.local_value.state = ERROR;
                    } else {
                        m_status.local_value.state = WAITING;
                        m_rxData.commit();
                    }
                }
                break;
            case WAITING:
                if(m_control.local_value.command == Common::ControlBase::STOP) {
                    m_status.local_value.state = IDLE;
                } else {
                    Sleep_ms(m_control.local_value.period_ms);
                    m_status.local_value.state = SENDING_FRAME;
                }
                break;
            case CONNECTING:
                if(m_control.local_value.command == Common::ControlBase::STOP) {
                    m_status.local_value.state = IDLE;
                } else {
                    m_status.local_value.error_code = (eth_ctrl_error_code_t)eth_ctrl_open(&m_eth_ctrl_conn, m_control.local_value.nic_name);
                    m_status.local_value.error_message = eth_ctrl_error_msgs[m_status.local_value.error_code];
                    m_status.commit();
                    if (m_status.local_value.error_code == ETH_CTRL_NO_ERROR) {
                        m_status.local_value.state = SENDING_FRAME;
                        m_status.local_value.connected = true;
                    } else {
                        Sleep_ms(500);
                    }
                }
                break;
            case ERROR:
                if(m_control.local_value.command == Common::ControlBase::STOP) {
                    m_status.local_value.state = IDLE;
                } else {
                    Sleep_ms(500);
                }
                break;
            default:
            case IDLE:
                if(m_control.local_value.command == Common::ControlBase::START) {
                    m_status.local_value.state = CONNECTING;
                } else if (m_status.local_value.connected) {
                    m_status.local_value.connected = false;
                    eth_ctrl_close(&m_eth_ctrl_conn);
                } else {
                    Sleep_ms(500);
                }
                break;
        }
        m_status.local_value.state_name = state_names[m_status.local_value.state];
        m_status.commit();
    }

    return true;
}
