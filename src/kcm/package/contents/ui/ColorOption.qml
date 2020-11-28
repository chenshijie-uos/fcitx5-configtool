/*
 * SPDX-FileCopyrightText: 2020~2020 CSSlayer <wengxt@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 */
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Dialogs 1.1 as QtDialogs
import org.kde.kirigami 2.10 as Kirigami

Row {
    property variant rawValue
    property bool needsSave: button.text != rawValue

    function load(rawValue) {
        colorDialog.color = kcm.parseColor(rawValue)
    }

    function save() {
        rawValue = button.text
    }

    Component.onCompleted: {
        load(rawValue)
    }

    Rectangle {
        height: button.height
        width: height
        color: colorDialog.color
    }

    Button {
        id: button
        icon.name: "document-edit"
        text: kcm.colorToString(colorDialog.color)
        implicitWidth: Kirigami.Units.gridUnit * 4
        onClicked: colorDialog.open()

        QtDialogs.ColorDialog {
            id: colorDialog
            modality: Qt.ApplicationModal
            title: i18nc("@title:window", "Select Color")
            showAlphaChannel: true
        }
    }
}
