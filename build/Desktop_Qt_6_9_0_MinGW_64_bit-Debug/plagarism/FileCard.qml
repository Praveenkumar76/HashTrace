// FileCard.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

Item {
    id: root
    property string filePath: ""
    property string title: "Untitled"

    width: 300
    height: 200

    Rectangle {
        anchors.fill: parent
        radius: 8
        border.color: palette.mid
        border.width: 1
        color: palette.base

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10

            Label {
                text: root.title
                font.bold: true
                font.pixelSize: 16
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                TextArea {
                    id: textArea
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    font.family: "Courier New"
                    text: filePath !== "" ? fileContent : "No file selected"
                }
            }

            Label {
                text: filePath !== "" ? filePath.split("/").pop() : "No file selected"
                elide: Text.ElideMiddle
                Layout.fillWidth: true
            }
        }
    }

    // Load file content using JavaScript
    property string fileContent: ""

    Component.onCompleted: {
        if (filePath !== "") {
            loadFileContent(filePath)
        }
    }

    function loadFileContent(path) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", path);
        xhr.onreadystatechange = function () {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    fileContent = xhr.responseText;
                } else {
                    fileContent = "Could not read file: " + xhr.status;
                }
            }
        }
        xhr.send();
    }
}
