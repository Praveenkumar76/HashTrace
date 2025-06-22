import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import com.company.backend 1.0
import com.company.filereader 1.0

Rectangle {
    id: fileCard
    property string filePath: ""
    property string localFilePath: ""
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

    FileReader {
        id: fileReader
        onErrorOccurred: function(message) {
            hasError = true;
            fileContent = "Error: " + message;
            isLoading = false;
        }
    }

    onFilePathChanged: {
        if (filePath) {
            isLoading = true;
            hasError = false;

            // Store local file path for backend processing
            if (filePath.startsWith("file:///")) {
                localFilePath = filePath;
            } else if (filePath.startsWith("/")) {
                localFilePath = "file://" + filePath;
            } else {
                localFilePath = "file:///" + filePath;
            }

            // Read original content
            fileContent = fileReader.readFile(filePath);

            if (fileContent === "" || fileContent.startsWith("Error:")) {
                hasError = true;
                if (fileContent === "") {
                    fileContent = "Error: Could not read file";
                }
            } else {
                // Get processed content from backend
                processedContent = backend.getProcessedContent(localFilePath);
                if (processedContent === "") {
                    console.warn("Warning: Could not get processed content for", filePath);
                    processedContent = "Processing failed - using original content";
                }
            }
            isLoading = false;
        } else {
            fileContent = "";
            processedContent = "";
            localFilePath = "";
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

            // Original content view
            ScrollView {
                clip: true

                Rectangle {
                    width: Math.max(originalText.implicitWidth + 35, parent.width)
                    height: Math.max(originalText.implicitHeight, parent.height)
                    color: "transparent"

                    // Line numbers background
                    Rectangle {
                        id: lineNumberBg
                        width: 30
                        height: parent.height
                        color: darkMode ? "#444444" : "#EEEEEE"
                        border.color: darkMode ? "#555555" : "#DDDDDD"
                        border.width: 1

                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            font.family: "Courier New"
                            font.pixelSize: 12
                            color: darkMode ? "#AAAAAA" : "#666666"
                            verticalAlignment: Text.AlignTop
                            text: {
                                if (!originalText.text) return "1";
                                let lines = originalText.text.split('\n').length;
                                let numbers = [];
                                for (let i = 1; i <= lines; i++) {
                                    numbers.push(i);
                                }
                                return numbers.join('\n');
                            }
                        }
                    }

                    TextArea {
                        id: originalText
                        anchors.left: lineNumberBg.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 5
                        text: fileContent
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        font.family: "Courier New"
                        font.pixelSize: 12
                        color: darkMode ? "#E0E0E0" : "#333333"
                        background: Rectangle {
                            color: "transparent"
                        }
                        selectByMouse: true
                    }
                }
            }

            // Processed content view
            ScrollView {
                clip: true

                Rectangle {
                    width: Math.max(processedText.implicitWidth + 35, parent.width)
                    height: Math.max(processedText.implicitHeight, parent.height)
                    color: "transparent"

                    // Line numbers background
                    Rectangle {
                        id: processedLineNumberBg
                        width: 30
                        height: parent.height
                        color: darkMode ? "#444444" : "#EEEEEE"
                        border.color: darkMode ? "#555555" : "#DDDDDD"
                        border.width: 1

                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            font.family: "Courier New"
                            font.pixelSize: 12
                            color: darkMode ? "#AAAAAA" : "#666666"
                            verticalAlignment: Text.AlignTop
                            text: {
                                if (!processedText.text) return "1";
                                let lines = processedText.text.split('\n').length;
                                let numbers = [];
                                for (let i = 1; i <= lines; i++) {
                                    numbers.push(i);
                                }
                                return numbers.join('\n');
                            }
                        }
                    }

                    TextArea {
                        id: processedText
                        anchors.left: processedLineNumberBg.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: 5
                        text: processedContent
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        font.family: "Courier New"
                        font.pixelSize: 12
                        color: darkMode ? "#E0E0E0" : "#333333"
                        background: Rectangle {
                            color: "transparent"
                        }
                        selectByMouse: true
                    }
                }
            }
        }

        Label {
            text: {
                if (filePath) {
                            let fileName = filePath.split("/").pop();
                            let info = fileReader.getFileInfo(filePath);
                            return fileName + (info ? " | " + info : "");
                        }
                        return "Click to select file";  // This is your existing hint
                    }
                    // Make the hint more visible when no file is selected
                    font.italic: !filePath
                    color: !filePath ? (darkMode ? "#64B5F6" : "#1976D2") :
                          (darkMode ? "#BDBDBD" : "#757575")
            elide: Text.ElideMiddle
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
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
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: internalFileDialog.open()
        cursorShape: Qt.PointingHandCursor

        // Visual feedback on hover
        onEntered: parent.border.color = parent.darkMode ? "#64B5F6" : "#1976D2"
        onExited: parent.border.color = parent.hasError ? "red" :
                 (parent.fileContent ? "#4CAF50" : "#9E9E9E")
    }

    BusyIndicator {
        running: parent.isLoading
        anchors.centerIn: parent
        width: 48
        height: 48
        visible: running
        z: 10
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
