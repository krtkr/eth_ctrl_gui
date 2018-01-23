/*
 * mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sb_sd0_period_editingFinished();

    void on_sb_sd1_period_editingFinished();

    void on_sb_sd2_period_editingFinished();

    void on_sb_sd3_period_editingFinished();

    void on_sb_sd0_step_length_editingFinished();

    void on_sb_sd1_step_length_editingFinished();

    void on_sb_sd2_step_length_editingFinished();

    void on_sb_sd3_step_length_editingFinished();

    void on_cb_sd0_enable_stateChanged(int arg1);

    void on_cb_sd1_enable_stateChanged(int arg1);

    void on_cb_sd2_enable_stateChanged(int arg1);

    void on_cb_sd3_enable_stateChanged(int arg1);

    void on_cb_sd0_move_stateChanged(int arg1);

    void on_cb_sd1_move_stateChanged(int arg1);

    void on_cb_sd2_move_stateChanged(int arg1);

    void on_cb_sd3_move_stateChanged(int arg1);

    void on_cb_sd0_dir_stateChanged(int arg1);

    void on_cb_sd1_dir_stateChanged(int arg1);

    void on_cb_sd2_dir_stateChanged(int arg1);

    void on_cb_sd3_dir_stateChanged(int arg1);

    void on_cb_sd0_reset_stateChanged(int arg1);

    void on_cb_sd1_reset_stateChanged(int arg1);

    void on_cb_sd2_reset_stateChanged(int arg1);

    void on_cb_sd3_reset_stateChanged(int arg1);

    void on_cb_qei0_reset_stateChanged(int arg1);

    void on_cb_qei1_reset_stateChanged(int arg1);

    void on_cb_qei2_reset_stateChanged(int arg1);

    void on_cb_qei3_reset_stateChanged(int arg1);

    void on_le_gpo_editingFinished();

    void on_pb_start_clicked();

    void on_pb_stop_clicked();

    void on_le_nic_name_editingFinished();

    void on_sb_module_id_editingFinished();

    void on_dsb_period_editingFinished();

    void update();

    void on_cb_write_stateChanged(int arg1);

    void on_cb_sd0_invert_enable_stateChanged(int arg1);

    void on_cb_sd1_invert_enable_stateChanged(int arg1);

    void on_cb_sd2_invert_enable_stateChanged(int arg1);

    void on_cb_sd3_invert_enable_stateChanged(int arg1);

    void on_cb_sd0_invert_step_stateChanged(int arg1);

    void on_cb_sd1_invert_step_stateChanged(int arg1);

    void on_cb_sd2_invert_step_stateChanged(int arg1);

    void on_cb_sd3_invert_step_stateChanged(int arg1);

    void on_cb_sd0_invert_dir_stateChanged(int arg1);

    void on_cb_sd1_invert_dir_stateChanged(int arg1);

    void on_cb_sd2_invert_dir_stateChanged(int arg1);

    void on_cb_sd3_invert_dir_stateChanged(int arg1);

    void on_cb_qei0_invert_a_stateChanged(int arg1);

    void on_cb_qei1_invert_a_stateChanged(int arg1);

    void on_cb_qei2_invert_a_stateChanged(int arg1);

    void on_cb_qei3_invert_a_stateChanged(int arg1);

    void on_cb_qei0_invert_b_stateChanged(int arg1);

    void on_cb_qei1_invert_b_stateChanged(int arg1);

    void on_cb_qei2_invert_b_stateChanged(int arg1);

    void on_cb_qei3_invert_b_stateChanged(int arg1);

    void on_cb_qei0_invert_z_stateChanged(int arg1);

    void on_cb_qei1_invert_z_stateChanged(int arg1);

    void on_cb_qei2_invert_z_stateChanged(int arg1);

    void on_cb_qei3_invert_z_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    char nic_name_buffer[1024];
    QTimer *timer;

};

#endif // MAINWINDOW_H
