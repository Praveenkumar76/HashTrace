import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import com.company.backend 1.0

Window {
    id: window
    width: 1280
    height: 720
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: "Plagiarism Detector"
    color: darkMode ? "#2d2d2d" : "#f5f5f5"

    property bool darkMode: false

    // Busy indicator for processing
    BusyIndicator {
        id: busyIndicator
        running: false
        anchors.centerIn: parent
        z: 100
        width: 64
        height: 64
        visible: running
    }

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
            busyIndicator.running = false;

            // Show warning if high similarity detected
            if (similarityScore > 70) {
                warningDialog.text = `High similarity detected (${similarityScore.toFixed(2)}%). Possible plagiarism!`;
                warningDialog.open();
            }
        }
        onProcessingChanged: busyIndicator.running = processing
        onErrorOccurred: function(message) {
            errorDialog.text = message;
            errorDialog.open();
            busyIndicator.running = false;
        }
    }

    function getScoreColor(score) {
        if (score > 70) return "red";
        if (score > 40) return "orange";
        return "green";
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

            ToolButton {
                icon.source: "icons/menu.svg"
                icon.color: darkMode ? "white" : "black"
                onClicked: drawer.open()
            }

            Label {
                text: window.title
                font.pixelSize: 22
                font.bold: true
                color: darkMode ? "white" : "black"
                Layout.fillWidth: true
            }

            ToolButton {
                icon.source: "icons/theme.svg"
                icon.color: darkMode ? "white" : "black"
                onClicked: window.darkMode = !window.darkMode
            }
        }

        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: darkMode ? "#444444" : "#e0e0e0"
        }
    }

    // Navigation drawer
    Drawer {
        id: drawer
        width: Math.min(window.width * 0.33, 300)
        height: window.height
        edge: Qt.LeftEdge
        dim: true
        background: Rectangle {
            color: darkMode ? "#333333" : "#ffffff"
        }

        ColumnLayout {
            width: parent.width
            spacing: 0

            ItemDelegate {
                text: "Compare Files"
                icon.source: "icons/compare.svg"
                icon.color: darkMode ? "white" : "black"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(comparePage);
                    drawer.close();
                }
                background: Rectangle {
                    color: parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") : "transparent"
                }
            }

            ItemDelegate {
                text: "History"
                icon.source: "icons/history.png"
                icon.color: darkMode ? "white" : "black"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(historyPage);
                    drawer.close();
                }
                background: Rectangle {
                    color: parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") : "transparent"
                }
            }

            ItemDelegate {
                text: "Settings"
                icon.source: "icons/settings.svg"
                icon.color: darkMode ? "white" : "black"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(settingsPage);
                    drawer.close();
                }
                background: Rectangle {
                    color: parent.hovered ? (darkMode ? "#444444" : "#f0f0f0") : "transparent"
                }
            }
        }
    }

    // Main content area
    StackView {
        id: stackView
        anchors {
            top: toolbar.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        initialItem: comparePage
    }

    // Pages
    Component {
        id: comparePage
        ColumnLayout {
            spacing: 20
            anchors.margins: 20

            TabBar {
                id: viewTabs
                Layout.fillWidth: true
                background: Rectangle {
                    color: darkMode ? "#333333" : "#ffffff"
                }

                TabButton {
                    text: "File Comparison"
                    background: Rectangle {
                        color: darkMode ? (viewTabs.currentIndex === 0 ? "#444444" : "#333333") :
                                         (viewTabs.currentIndex === 0 ? "#f0f0f0" : "#ffffff")
                    }
                }
                TabButton {
                    text: "Results"
                    background: Rectangle {
                        color: darkMode ? (viewTabs.currentIndex === 1 ? "#444444" : "#333333") :
                                         (viewTabs.currentIndex === 1 ? "#f0f0f0" : "#ffffff")
                    }
                }
            }

            SwipeView {
                id: swipeView
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentIndex: viewTabs.currentIndex
                interactive: false

                Item {
                    RowLayout {
                        anchors.fill: parent
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
                }

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
                            text: "No comparison results yet"
                            visible: resultView.count === 0
                            font.pixelSize: 16
                            color: darkMode ? "#aaaaaa" : "#666666"
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillHeight: true
                        }
                    }
                }
            }

            Button {
                text: "Select Files"
                icon.source: "icons/add_file.png"
                Layout.alignment: Qt.AlignHCenter
                onClicked: fileDialog.open()
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

    Component {
        id: historyPage
        ScrollView {
            Label {
                text: "Comparison history will appear here"
                anchors.centerIn: parent
                color: darkMode ? "white" : "black"
            }
            background: Rectangle {
                color: darkMode ? "#2d2d2d" : "#f5f5f5"
            }
        }
    }

    Component {
        id: settingsPage
        ScrollView {
            ColumnLayout {
                width: parent.width - 40
                anchors.centerIn: parent
                spacing: 20

                Label {
                    text: "Settings"
                    font.pixelSize: 24
                    Layout.alignment: Qt.AlignHCenter
                    color: darkMode ? "white" : "black"
                }

                Switch {
                    text: "Dark Mode"
                    checked: window.darkMode
                    onCheckedChanged: window.darkMode = checked
                    Layout.alignment: Qt.AlignHCenter
                }
            }
            background: Rectangle {
                color: darkMode ? "#2d2d2d" : "#f5f5f5"
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select files to compare"
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Source files (*.cpp *.h *.py *.java *.js)", "Text files (*.txt)", "All files (*)"]
        onAccepted: {
            var urls = selectedFiles;
            if (urls.length >= 2) {
                fileCard1.filePath = urls[0];
                fileCard2.filePath = urls[1];
                backend.processFiles(urls);
                busyIndicator.running = true;
            }
        }
    }

    // Result delegate
    Component {
        id: resultDelegate
        Rectangle {
            width: ListView.view.width - 20
            height: column.implicitHeight + 20
            radius: 5
            color: darkMode ? (index % 2 === 0 ? "#333333" : "#3a3a3a") :
                              (index % 2 === 0 ? "#ffffff" : "#f9f9f9")
            border.color: darkMode ? "#444444" : "#e0e0e0"
            border.width: 1
            anchors.horizontalCenter: parent.horizontalCenter

            ColumnLayout {
                id: column
                anchors.fill: parent
                anchors.margins: 10
                spacing: 5

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    Label {
                        text: file1.split('/').pop()
                        elide: Text.ElideMiddle
                        font.bold: true
                        color: darkMode ? "white" : "black"
                        Layout.fillWidth: true
                    }

                    Label {
                        text: " ↔ "
                        color: darkMode ? "white" : "black"
                    }

                    Label {
                        text: file2.split('/').pop()
                        elide: Text.ElideMiddle
                        font.bold: true
                        color: darkMode ? "white" : "black"
                        Layout.fillWidth: true
                    }
                }

                ProgressBar {
                    value: score / 100
                    Layout.fillWidth: true
                    background: Rectangle {
                        radius: 3
                        color: darkMode ? "#444444" : "#e0e0e0"
                    }
                    contentItem: Rectangle {
                        radius: 3
                        color: getScoreColor(score)
                    }
                }

                Label {
                    text: "Similarity Score: " + score.toFixed(2) + "%"
                    font.bold: true
                    color: getScoreColor(score)
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    text: "View Details"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        detailDialog.file1Path = file1;
                        detailDialog.file2Path = file2;
                        detailDialog.similarityScore = model.score;
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
        width: Math.min(window.width * 0.8, 800)
        height: Math.min(window.height * 0.8, 600)
        modal: true

        background: Rectangle {
            color: darkMode ? "#333333" : "#ffffff"
            radius: 5
        }

        ColumnLayout {
            width: parent.width
            spacing: 15

            Label {
                text: "Detailed comparison between:"
                font.bold: true
                color: darkMode ? "white" : "black"
            }

            Label {
                text: detailDialog.file1Path.split('/').pop() + " and " + file2Path.split('/').pop()
                color: darkMode ? "white" : "black"
                font.pixelSize: 16
            }

            Label {
                text: "Similarity: " + detailDialog.similarityScore.toFixed(2) + "%"
                font.bold: true
                font.pixelSize: 18
                color: getScoreColor(score)
                Layout.alignment: Qt.AlignHCenter
            }

            // Add more detailed comparison info here as needed
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: false // Change to true when adding actual comparison content
            }
        }
    }
}
