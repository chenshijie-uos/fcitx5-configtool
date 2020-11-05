/*
 * SPDX-FileCopyrightText: 2020~2020 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */
#include "mainwindow.h"
#include "logging.h"
#include <fcitx-utils/i18n.h>
#include <fcitxqti18nhelper.h>
#include <verticalscrollarea.h>
#include <QVBoxLayout>

namespace fcitx {
namespace kcm {

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent), dbus_(new DBusProvider(this)),
      errorOverlay_(new ErrorOverlay(dbus_, this)),
      impage_(new IMPage(dbus_, this)),
      addonPage_(new AddonSelector(this, dbus_)),
      configPage_(new ConfigWidget("fcitx://config/global", dbus_, this))
{
    initUI();

    connect(impage_, &IMPage::changed, this, [this]() {
        qCDebug(KCM_FCITX5) << "IMPage changed";
        emit changed(true);
    });
    connect(addonPage_, &AddonSelector::changed, this, [this]() {
        qCDebug(KCM_FCITX5) << "AddonSelector changed";
        emit changed(true);
    });
    auto configPageWrapper = new VerticalScrollArea;
    configPageWrapper->setWidget(configPage_);
    pageWidget->addTab(impage_, _("Input Method"));
    pageWidget->addTab(configPageWrapper, _("Global Options"));
    pageWidget->addTab(addonPage_, _("Addons"));
    connect(configPage_, &ConfigWidget::changed, this,
    [this]() { emit changed(true); });

    connect(this, &MainWindow::changed, this, &MainWindow::handleChanged);

    connect(buttonBox, &QDialogButtonBox::clicked, this, &MainWindow::clicked);
    load();
}

void MainWindow::initUI()
{
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;


    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("MainWindow"));
    this->resize(800, 600);
    QIcon icon(QIcon::fromTheme(QString::fromUtf8("fcitx")));
    this->setWindowIcon(icon);
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    pageWidget = new QTabWidget(centralwidget);
    pageWidget->setObjectName(QString::fromUtf8("pageWidget"));

    verticalLayout->addWidget(pageWidget);

    buttonBox = new QDialogButtonBox(centralwidget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setStandardButtons(QDialogButtonBox::Apply | QDialogButtonBox::Close | QDialogButtonBox::Ok | QDialogButtonBox::Reset | QDialogButtonBox::RestoreDefaults);

    verticalLayout->addWidget(buttonBox);

    this->setCentralWidget(centralwidget);
    QMetaObject::connectSlotsByName(this);

    this->setWindowTitle(fcitx::tr2fcitx("Fcitx Configuration", nullptr));

}

void MainWindow::handleChanged(bool changed)
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(changed);
    buttonBox->button(QDialogButtonBox::Apply)->setEnabled(changed);
    buttonBox->button(QDialogButtonBox::Reset)->setEnabled(changed);
}

void MainWindow::load()
{
    impage_->load();
    addonPage_->load();
    configPage_->load();
    emit changed(false);
}

void MainWindow::save()
{
    impage_->save();
    addonPage_->save();
    configPage_->save();
    emit changed(false);
}

void MainWindow::defaults()
{
    configPage_->buttonClicked(QDialogButtonBox::RestoreDefaults);
    emit changed(true);
}

void MainWindow::clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton standardButton =
        buttonBox->standardButton(button);
    if (standardButton == QDialogButtonBox::Apply ||
            standardButton == QDialogButtonBox::Ok) {
        save();
    } else if (standardButton == QDialogButtonBox::Close) {
        qApp->quit();
    } else if (standardButton == QDialogButtonBox::Reset) {
        load();
    } else if (standardButton == QDialogButtonBox::RestoreDefaults) {
        defaults();
    }
}

} // namespace kcm
} // namespace fcitx
