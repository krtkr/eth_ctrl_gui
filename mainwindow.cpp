/*
 * mainwindow.cpp
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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ethctrl.hpp"

#include <QCloseEvent>

EthCtrl::ControlBuffer controlBuffer;
EthCtrl::StatusBuffer statusBuffer;
EthCtrlRxData rxData;
EthCtrlTxData txData;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    controlBuffer.local_value.nic_name = nic_name_buffer;
    controlBuffer.local_value.period_ms = (unsigned int)(ui->dsb_period->value() * 1000.0);
    controlBuffer.local_value.module_id = ui->sb_module_id->value();
    controlBuffer.commit();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    timer->start(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    statusBuffer.fetch();
    rxData.fetch();

    ui->le_state->setText(QString(statusBuffer.local_value.state_name));

    ui->le_error_code->setText(QString("%1").arg(statusBuffer.local_value.error_code));
    ui->le_error_msg->setText(QString(statusBuffer.local_value.error_message));
    if(statusBuffer.local_value.connected) {
        ui->le_nic_name->setEnabled(false);
        ui->sb_module_id->setEnabled(false);
        ui->dsb_period->setEnabled(false);
    } else {
        ui->le_nic_name->setEnabled(true);
        ui->sb_module_id->setEnabled(true);
        ui->dsb_period->setEnabled(true);
    }

    ui->le_sd0_steps->setText(QString("%1").arg(rxData.local_value.steps[0]));
    ui->le_sd1_steps->setText(QString("%1").arg(rxData.local_value.steps[1]));
    ui->le_sd2_steps->setText(QString("%1").arg(rxData.local_value.steps[2]));
    ui->le_sd3_steps->setText(QString("%1").arg(rxData.local_value.steps[3]));

    ui->le_qei0_count->setText(QString("%1").arg(rxData.local_value.qei[0]));
    ui->le_qei1_count->setText(QString("%1").arg(rxData.local_value.qei[1]));
    ui->le_qei2_count->setText(QString("%1").arg(rxData.local_value.qei[2]));
    ui->le_qei3_count->setText(QString("%1").arg(rxData.local_value.qei[3]));

    ui->le_qei0_z_count->setText(QString("%1").arg(rxData.local_value.qei_z[0]));
    ui->le_qei1_z_count->setText(QString("%1").arg(rxData.local_value.qei_z[1]));
    ui->le_qei2_z_count->setText(QString("%1").arg(rxData.local_value.qei_z[2]));
    ui->le_qei3_z_count->setText(QString("%1").arg(rxData.local_value.qei_z[3]));

    ui->le_gpi->setText(QString("0x%1").arg(rxData.local_value.gpi, 8, 16, QLatin1Char('0')));

    timer->start(100);
}

void MainWindow::on_sb_sd0_period_editingFinished()
{
    txData.local_value.period[0] = ui->sb_sd0_period->value();
    txData.commit();
}

void MainWindow::on_sb_sd1_period_editingFinished()
{
    txData.local_value.period[1] = ui->sb_sd1_period->value();
    txData.commit();
}

void MainWindow::on_sb_sd2_period_editingFinished()
{
    txData.local_value.period[2] = ui->sb_sd2_period->value();
    txData.commit();
}

void MainWindow::on_sb_sd3_period_editingFinished()
{
    txData.local_value.period[3] = ui->sb_sd3_period->value();
    txData.commit();
}

void MainWindow::on_sb_sd0_step_length_editingFinished()
{
    txData.local_value.step_length[0] = ui->sb_sd0_step_length->value();
    txData.commit();
}

void MainWindow::on_sb_sd1_step_length_editingFinished()
{
    txData.local_value.step_length[1] = ui->sb_sd1_step_length->value();
    txData.commit();
}

void MainWindow::on_sb_sd2_step_length_editingFinished()
{
    txData.local_value.step_length[2] = ui->sb_sd2_step_length->value();
    txData.commit();
}

void MainWindow::on_sb_sd3_step_length_editingFinished()
{
    txData.local_value.step_length[3] = ui->sb_sd3_step_length->value();
    txData.commit();
}

void MainWindow::on_cb_sd0_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_move_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_MOVE_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_MOVE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_move_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_MOVE_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_MOVE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_move_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_MOVE_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_MOVE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_move_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_MOVE_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_MOVE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_RESET_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_RESET_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_RESET_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_RESET_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei0_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[0] |= QEI_CONTROL_RESET_MASK;
    } else {
        txData.local_value.qei_control[0] &= ~QEI_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei1_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[1] |= QEI_CONTROL_RESET_MASK;
    } else {
        txData.local_value.qei_control[1] &= ~QEI_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei2_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[2] |= QEI_CONTROL_RESET_MASK;
    } else {
        txData.local_value.qei_control[2] &= ~QEI_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei3_reset_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[3] |= QEI_CONTROL_RESET_MASK;
    } else {
        txData.local_value.qei_control[3] &= ~QEI_CONTROL_RESET_MASK;
    }
    txData.commit();
}

void MainWindow::on_le_gpo_editingFinished()
{
    bool ok;
    uint value = ui->le_gpo->text().toUInt(&ok, 0);
    if(ok) {
        txData.local_value.gpo = value;
        txData.commit();
    }
}

void MainWindow::on_pb_start_clicked()
{
    controlBuffer.local_value.command = Common::ControlBase::START;
    controlBuffer.commit();
}

void MainWindow::on_pb_stop_clicked()
{
    controlBuffer.local_value.command = Common::ControlBase::STOP;
    controlBuffer.commit();
}

void MainWindow::on_le_nic_name_editingFinished()
{
    strcpy(nic_name_buffer, ui->le_nic_name->text().toLatin1().constData());
}

void MainWindow::on_sb_module_id_editingFinished()
{
    controlBuffer.local_value.module_id = ui->sb_module_id->value();
    controlBuffer.commit();
}

void MainWindow::on_dsb_period_editingFinished()
{
    controlBuffer.local_value.period_ms = (unsigned int)(ui->dsb_period->value() * 1000.0);
    controlBuffer.commit();
}

void MainWindow::on_cb_write_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.write = WRITE_SYMBOL;
    } else {
        txData.local_value.write = 0;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_invert_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_invert_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_invert_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_invert_enable_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_INVERT_ENABLE_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_invert_step_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_INVERT_STEP_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_INVERT_STEP_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_invert_step_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_INVERT_STEP_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_INVERT_STEP_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_invert_step_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_INVERT_STEP_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_INVERT_STEP_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_invert_step_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_INVERT_STEP_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_INVERT_STEP_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd0_invert_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[0] |= STEP_DIR_CONTROL_INVERT_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[0] &= ~STEP_DIR_CONTROL_INVERT_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd1_invert_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[1] |= STEP_DIR_CONTROL_INVERT_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[1] &= ~STEP_DIR_CONTROL_INVERT_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd2_invert_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[2] |= STEP_DIR_CONTROL_INVERT_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[2] &= ~STEP_DIR_CONTROL_INVERT_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_sd3_invert_dir_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.step_dir_control[3] |= STEP_DIR_CONTROL_INVERT_DIR_MASK;
    } else {
        txData.local_value.step_dir_control[3] &= ~STEP_DIR_CONTROL_INVERT_DIR_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei0_invert_a_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[0] |= QEI_CONTROL_INVERT_A_MASK;
    } else {
        txData.local_value.qei_control[0] &= ~QEI_CONTROL_INVERT_A_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei1_invert_a_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[1] |= QEI_CONTROL_INVERT_A_MASK;
    } else {
        txData.local_value.qei_control[1] &= ~QEI_CONTROL_INVERT_A_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei2_invert_a_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[2] |= QEI_CONTROL_INVERT_A_MASK;
    } else {
        txData.local_value.qei_control[2] &= ~QEI_CONTROL_INVERT_A_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei3_invert_a_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[3] |= QEI_CONTROL_INVERT_A_MASK;
    } else {
        txData.local_value.qei_control[3] &= ~QEI_CONTROL_INVERT_A_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei0_invert_b_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[0] |= QEI_CONTROL_INVERT_B_MASK;
    } else {
        txData.local_value.qei_control[0] &= ~QEI_CONTROL_INVERT_B_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei1_invert_b_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[1] |= QEI_CONTROL_INVERT_B_MASK;
    } else {
        txData.local_value.qei_control[1] &= ~QEI_CONTROL_INVERT_B_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei2_invert_b_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[2] |= QEI_CONTROL_INVERT_B_MASK;
    } else {
        txData.local_value.qei_control[2] &= ~QEI_CONTROL_INVERT_B_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei3_invert_b_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[3] |= QEI_CONTROL_INVERT_B_MASK;
    } else {
        txData.local_value.qei_control[3] &= ~QEI_CONTROL_INVERT_B_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei0_invert_z_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[0] |= QEI_CONTROL_INVERT_Z_MASK;
    } else {
        txData.local_value.qei_control[0] &= ~QEI_CONTROL_INVERT_Z_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei1_invert_z_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[1] |= QEI_CONTROL_INVERT_Z_MASK;
    } else {
        txData.local_value.qei_control[1] &= ~QEI_CONTROL_INVERT_Z_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei2_invert_z_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[2] |= QEI_CONTROL_INVERT_Z_MASK;
    } else {
        txData.local_value.qei_control[2] &= ~QEI_CONTROL_INVERT_Z_MASK;
    }
    txData.commit();
}

void MainWindow::on_cb_qei3_invert_z_stateChanged(int arg1)
{
    if(arg1) {
        txData.local_value.qei_control[3] |= QEI_CONTROL_INVERT_Z_MASK;
    } else {
        txData.local_value.qei_control[3] &= ~QEI_CONTROL_INVERT_Z_MASK;
    }
    txData.commit();
}
