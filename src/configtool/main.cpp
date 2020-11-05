/*
 * SPDX-FileCopyrightText: 2020~2020 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */
#include "mainwindow.h"
#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("fcitx5-configtool-dtk");
    a.setApplicationVersion("1.0");
//    a.setProductIcon(QIcon(":/images/logo.svg"));
    a.setProductName("fcitx5-configtool-dtk");
    a.setApplicationDescription("This is a dtk fcitx5 configtool application.");

    a.loadTranslator();
    a.setApplicationDisplayName(QCoreApplication::translate("Main", "fcitx5-configtool-dtk"));

    // 保存程序的窗口主题设置
    DApplicationSettings as;
    Q_UNUSED(as)

    fcitx::registerFcitxQtDBusTypes();

    fcitx::kcm::MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
