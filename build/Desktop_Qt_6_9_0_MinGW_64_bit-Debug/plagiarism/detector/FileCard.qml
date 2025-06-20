import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import com.company.backend 1.0

Rectangle {
    id: fileCard
    property string filePath: ""
    property string title: "Untitled"
    property string fileContent: ""
    property bool isLoading: false
    property string processedContent: ""
    property bool darkMode: false
    property bool hasError: false

    width: 300
    height: 400
    radius: 8
    border.color: hasError ? "red" : (fileContent ? "#4CAF50" : "#9E9E9E")
    border.width: 2
    color: darkMode ? "#424242" : "#FFFFFF"

    onFilePathChanged: {
        if (filePath) {
            isLoading = true;
            hasError = false;
            var content = backend.getProcessedContent(filePath);
            if (content === "") {
                hasError = true;
                fileContent = "Error processing file";
            } else {
                processedContent = content;
            }
            isLoading = false;
        } else {
            fileContent = "";
            processedContent = "";
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        Label {
            text: title
            font.bold: true
            font.pixelSize: 16
            color: darkMode ? "#FFFFFF" : "#333333"
            Layout.alignment: Qt.AlignHCenter
        }

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            background: Rectangle {
                color: "transparent"
            }

            TabButton {
                text: "Original"
                background: Rectangle {
                    color: darkMode ? (tabBar.currentIndex === 0 ? "#616161" : "transparent") :
                                      (tabBar.currentIndex === 0 ? "#EEEEEE" : "transparent")
                }
            }
            TabButton {
                text: "Processed"
                background: Rectangle {
                    color: darkMode ? (tabBar.currentIndex === 1 ? "#616161" : "transparent") :
                                      (tabBar.currentIndex === 1 ? "#EEEEEE" : "transparent")
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            ScrollView {
                clip: true
                TextArea {
                    id: originalText
                    text: fileContent
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    font.family: "Courier New"
                    font.pixelSize: 12
                    color: darkMode ? "#E0E0E0" : "#333333"
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }

            ScrollView {
                clip: true
                TextArea {
                    id: processedText
                    text: processedContent
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    font.family: "Courier New"
                    font.pixelSize: 12
                    color: darkMode ? "#E0E0E0" : "#333333"
                    background: Rectangle {
                        color: "transparent"
                    }
                }
            }
        }

        Label {
            text: filePath ? filePath.split("/").pop() : "No file selected"
            elide: Text.ElideMiddle
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            color: darkMode ? "#BDBDBD" : "#757575"
            font.pixelSize: 12
        }
    }

    FileDialog {
        id: internalFileDialog
        title: "Select File"
        fileMode: FileDialog.OpenFile
        nameFilters: [
            "Source files (*.cpp *.h *.py *.java *.js *.txt)",
            "All files (*)"
        ]
        onAccepted: {
            filePath = selectedFile;
            loadFileContent();
        }
    }

    function loadFileContent() {
        if (!filePath) return;

        isLoading = true;
        hasError = false;

        var xhr = new XMLHttpRequest();
        xhr.open("GET", Qt.resolvedUrl(filePath));
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                isLoading = false;
                if (xhr.status === 200) {
                    fileContent = xhr.responseText;
                    processedContent = backend.getProcessedContent(filePath);
                    if (processedContent === "") {
                        hasError = true;
                        fileContent = "Error processing file";
                    }
                } else {
                    hasError = true;
                    fileContent = "Error loading file";
                    processedContent = "";
                }
            }
        };
        xhr.send();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: internalFileDialog.open()
        cursorShape: Qt.PointingHandCursor
    }

    BusyIndicator {
        running: parent.isLoading
        anchors.centerIn: parent
        width: 48
        height: 48
        visible: running
    }

    states: [
        State {
            name: "error"
            when: hasError
            PropertyChanges {
                target: fileCard
                border.color: "red"
            }
        },
        State {
            name: "loaded"
            when: fileContent && !hasError
            PropertyChanges {
                target: fileCard
                border.color: "#4CAF50"
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "error"
            ColorAnimation { duration: 300 }
        },
        Transition {
            from: ""
            to: "loaded"
            ColorAnimation { duration: 300 }
        }
    ]
}
