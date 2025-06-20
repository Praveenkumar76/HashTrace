import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Rectangle {
    property string filePath: ""
    property string title: "Untitled"
    property string fileContent: ""
    property bool isLoading: false

    width: 300
    height: 200
    radius: 8
    border.color: fileContent ? "green" : "red"
    border.width: 2
    color: "white"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Label {
            text: title
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
                text: fileContent
            }
        }

        Label {
            text: filePath ? filePath.split("/").pop() : "No file selected"
            elide: Text.ElideMiddle
            Layout.fillWidth: true
        }
    }

    FileDialog {
        id: internalFileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            filePath = internalFileDialog.currentFile
            loadFileContent()
        }
    }

    function loadFileContent() {
        if (filePath) {
            isLoading = true;
            var xhr = new XMLHttpRequest();
            xhr.open("GET", filePath);
            xhr.onreadystatechange = function() {
                isLoading = false;
                if (xhr.readyState === XMLHttpRequest.DONE) {
                    if (xhr.status === 200) {
                        fileContent = xhr.responseText;
                    } else {
                        fileContent = "Error loading file";
                    }
                }
            };
            xhr.send();
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: internalFileDialog.open()
    }

    BusyIndicator {
        running: parent.isLoading
        anchors.centerIn: parent
    }
}
