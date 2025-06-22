import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import com.company.backend 1.0
import com.company.filereader 1.0

Window {
    id: window
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: "Plagiarism Detector"

    property bool darkMode: false

    // Backend connection
    Backend {
        id: backend
        onComparisonFinished: function(similarityScore, matches) {
            resultModel.clear();
            for (var i = 0; i < matches.length; i++) {
                resultModel.append(matches[i]);
            }
            overallScore.text = `Overall Similarity: ${similarityScore.toFixed(2)}%`;
            overallScore.color = getScoreColor(similarityScore);

            viewTabs.currentIndex = 1;

            // Show warning if high similarity detected
            if (similarityScore > 70) {
                warningDialog.text = `High similarity detected (${similarityScore.toFixed(2)}%). Possible plagiarism!`;
                warningDialog.open();
            }
        }
        onErrorOccurred: function(message) {
            errorDialog.text = message;
            errorDialog.open();
        }
    }

    FileReader {
        id: fileReader
    }

    function getScoreColor(score) {
        if (score > 70) return "#FF5252";
        if (score > 40) return "#FF9800";
        return "#4CAF50";
    }

    // Dialogs
    MessageDialog {
        id: errorDialog
        title: "Error"
        buttons: MessageDialog.Ok
    }

    MessageDialog {
        id: warningDialog
        title: "Warning"
        buttons: MessageDialog.Ok
    }

    // Header
    Rectangle {
        id: toolbar
        width: parent.width
        height: 60
        color: darkMode ? "#333333" : "#ffffff"
        z: 10

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 15
            anchors.rightMargin: 15
            spacing: 20

            Label {
                text: window.title
                font.pixelSize: 22
                font.bold: true
                color: darkMode ? "white" : "black"
                Layout.fillWidth: true
            }

            Button {
                text: darkMode ? "Light Mode" : "Dark Mode"
                onClicked: window.darkMode = !window.darkMode
                background: Rectangle {
                    radius: 5
                    color: parent.down ? (darkMode ? "#555555" : "#e0e0e0") :
                          (parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") :
                                           (darkMode ? "#333333" : "#ffffff"))
                    border.color: darkMode ? "#666666" : "#cccccc"
                    border.width: 1
                }
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: darkMode ? "#444444" : "#e0e0e0"
        }
    }

    // Main content
    Rectangle {
        anchors {
            top: toolbar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        color: darkMode ? "#2d2d2d" : "#f5f5f5"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 20

            TabBar {
                id: viewTabs
                Layout.fillWidth: true
                background: Rectangle {
                    color: darkMode ? "#333333" : "#ffffff"
                    radius: 5
                }

                TabButton {
                    text: "File Comparison"
                    background: Rectangle {
                        color: darkMode ? (viewTabs.currentIndex === 0 ? "#444444" : "#333333") :
                                         (viewTabs.currentIndex === 0 ? "#f0f0f0" : "#ffffff")
                        radius: 5
                    }
                }
                TabButton {
                    text: "Results"
                    background: Rectangle {
                        color: darkMode ? (viewTabs.currentIndex === 1 ? "#444444" : "#333333") :
                                         (viewTabs.currentIndex === 1 ? "#f0f0f0" : "#ffffff")
                        radius: 5
                    }
                }
            }

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: viewTabs.currentIndex

                // File Comparison Tab
                Item {
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 20

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            spacing: 20

                            FileCard {
                                id: fileCard1
                                title: "File 1"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                darkMode: window.darkMode
                            }

                            FileCard {
                                id: fileCard2
                                title: "File 2"
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                darkMode: window.darkMode
                            }
                        }

                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 10

                            Button {
                                text: "Check for Plagiarism"
                                enabled: fileCard1.localFilePath && fileCard2.localFilePath && !backend.processing
                                onClicked: {
                                    if (fileCard1.localFilePath && fileCard2.localFilePath) {
                                        var files = [fileCard1.localFilePath, fileCard2.localFilePath];
                                        backend.processFiles(files);
                                    } else {
                                        errorDialog.text = "Please select both files first";
                                        errorDialog.open();
                                    }
                                }
                                background: Rectangle {
                                    radius: 5
                                    color: parent.down ? (darkMode ? "#555555" : "#e0e0e0") :
                                          (parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") :
                                                           (darkMode ? "#333333" : "#ffffff"))
                                    border.color: darkMode ? "#666666" : "#cccccc"
                                    border.width: 1
                                }
                            }

                            Button {
                                text: "Cancel"
                                visible: backend.processing
                                onClicked: backend.cancelProcessing()
                                background: Rectangle {
                                    radius: 5
                                    color: "#FF5252"
                                    border.color: "#D32F2F"
                                    border.width: 1
                                }
                            }
                        }

                        BusyIndicator {
                            running: backend.processing
                            Layout.alignment: Qt.AlignHCenter
                            visible: running
                        }
                    }
                }

                // Results Tab
                Item {
                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 10

                        Label {
                            id: overallScore
                            font.pixelSize: 18
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                            color: darkMode ? "white" : "black"
                        }

                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            visible: resultView.count > 0

                            ListView {
                                id: resultView
                                width: parent.width
                                model: ListModel { id: resultModel }
                                delegate: resultDelegate
                                spacing: 10
                            }
                        }

                        Label {
                            text: "No comparison results yet.\nSelect two files and click 'Check for Plagiarism' to begin."
                            visible: resultView.count === 0
                            font.pixelSize: 16
                            color: darkMode ? "#aaaaaa" : "#666666"
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillHeight: true
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
        }
    }

    // Result delegate
    Component {
        id: resultDelegate
        Rectangle {
            width: ListView.view.width - 20
            height: column.implicitHeight + 20
            radius: 8
            color: darkMode ? (index % 2 === 0 ? "#333333" : "#3a3a3a") :
                              (index % 2 === 0 ? "#ffffff" : "#f9f9f9")
            border.color: darkMode ? "#444444" : "#e0e0e0"
            border.width: 1
            anchors.horizontalCenter: parent.horizontalCenter

            ColumnLayout {
                id: column
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Label {
                        text: model.file1 ? model.file1.split('/').pop() : "File 1"
                        elide: Text.ElideMiddle
                        font.bold: true
                        color: darkMode ? "white" : "black"
                        Layout.fillWidth: true
                    }

                    Label {
                        text: " ↔ "
                        color: darkMode ? "white" : "black"
                        font.pixelSize: 16
                    }

                    Label {
                        text: model.file2 ? model.file2.split('/').pop() : "File 2"
                        elide: Text.ElideMiddle
                        font.bold: true
                        color: darkMode ? "white" : "black"
                        Layout.fillWidth: true
                    }
                }

                ProgressBar {
                    value: model.score / 100
                    Layout.fillWidth: true
                    background: Rectangle {
                        radius: 3
                        color: darkMode ? "#444444" : "#e0e0e0"
                    }
                    contentItem: Rectangle {
                        radius: 3
                        color: getScoreColor(model.score)
                    }
                }

                Label {
                    text: {
                        let score = model.score || 0;
                        if (score > 70) return "High Plagiarism Risk (" + score.toFixed(2) + "%)";
                        if (score > 40) return "Moderate Similarity (" + score.toFixed(2) + "%)";
                        return "Low Similarity (" + score.toFixed(2) + "%)";
                    }
                    font.bold: true
                    color: getScoreColor(model.score || 0)
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "View Details"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        detailDialog.file1Path = model.file1 || "";
                        detailDialog.file2Path = model.file2 || "";
                        detailDialog.similarityScore = model.score || 0;
                        detailDialog.open();
                    }

                    background: Rectangle {
                        radius: 5
                        color: parent.down ? (darkMode ? "#555555" : "#e0e0e0") :
                              (parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") :
                                               (darkMode ? "#333333" : "#ffffff"))
                        border.color: darkMode ? "#666666" : "#cccccc"
                        border.width: 1
                    }
                }
            }
        }
    }

    // Detailed comparison dialog
    Dialog {
        id: detailDialog
        property string file1Path: ""
        property string file2Path: ""
        property real similarityScore: 0

        title: "Detailed Comparison"
        standardButtons: Dialog.Ok
        width: Math.min(window.width * 0.9, 1000)
        height: Math.min(window.height * 0.9, 700)
        modal: true

        onOpened: {
            if (file1Path && file2Path) {
                file1Text.text = fileReader.readFile(file1Path);
                file2Text.text = fileReader.readFile(file2Path);
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 15

            Label {
                text: "Detailed comparison between:"
                font.bold: true
                color: darkMode ? "white" : "black"
            }

            Label {
                text: (detailDialog.file1Path.split('/').pop() || "File 1") + " and " +
                      (detailDialog.file2Path.split('/').pop() || "File 2")
                color: darkMode ? "white" : "black"
                font.pixelSize: 14
            }

            Label {
                text: "Similarity: " + detailDialog.similarityScore.toFixed(2) + "%"
                font.bold: true
                font.pixelSize: 18
                color: getScoreColor(detailDialog.similarityScore)
                Layout.alignment: Qt.AlignHCenter
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 20

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 5

                    Label {
                        text: "File 1: " + (detailDialog.file1Path.split('/').pop() || "Unknown")
                        font.bold: true
                        color: darkMode ? "white" : "black"
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        TextArea {
                            id: file1Text
                            readOnly: true
                            wrapMode: TextEdit.Wrap
                            font.family: "Courier New"
                            font.pixelSize: 11
                            selectByMouse: true
                            background: Rectangle {
                                color: darkMode ? "#333333" : "#ffffff"
                                border.color: darkMode ? "#555555" : "#cccccc"
                                border.width: 1
                            }
                        }
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 5

                    Label {
                        text: "File 2: " + (detailDialog.file2Path.split('/').pop() || "Unknown")
                        font.bold: true
                        color: darkMode ? "white" : "black"
                    }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        TextArea {
                            id: file2Text
                            readOnly: true
                            wrapMode: TextEdit.Wrap
                            font.family: "Courier New"
                            font.pixelSize: 11
                            selectByMouse: true
                            background: Rectangle {
                                color: darkMode ? "#333333" : "#ffffff"
                                border.color: darkMode ? "#555555" : "#cccccc"
                                border.width: 1
                            }
                        }
                    }
                }
            }
        }
    }
}
