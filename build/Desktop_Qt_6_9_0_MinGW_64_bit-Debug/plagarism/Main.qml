import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform
import QtQuick.Dialogs
import com.company.backend 1.0

Window {
    id: window
    width: 1280
    height: 720
    visible: true
    title: "ModernUI Plagiarism Detector"
    color: palette.window

    SystemPalette { id: palette }

    // Dark/light mode toggle
    property bool darkMode: false
    onDarkModeChanged: {
        palette.colorGroup = darkMode ? Palette.Active : Palette.Disabled
    }

    Backend {
        id: backend
        onComparisonFinished: (similarityScore, matches) => {
            resultModel.clear();
            for (var i = 0; i < matches.length; i++) {
                resultModel.append(matches[i]);
            }
            overallScore.text = `Overall Similarity: ${similarityScore.toFixed(2)}%`;
            busyIndicator.running = false;
        }
        onProcessingChanged: busyIndicator.running = processing
        onErrorOccurred: (message) => {
            errorDialog.text = message;
            errorDialog.open();
            busyIndicator.running = false;
        }
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
        buttons: MessageDialog.Ok
    }

    // Custom toolbar implementation for Window
    Rectangle {
        id: toolbar
        width: parent.width
        height: 50
        color: palette.window
        z: 10

        RowLayout {
            anchors.fill: parent
            spacing: 10
            //padding: 10

            ToolButton {
                icon.source: "qrc:/icons/menu.svg"
                onClicked: drawer.open()
            }

            Label {
                text: window.title
                font.pixelSize: 20
                font.bold: true
                Layout.fillWidth: true
            }

            ToolButton {
                icon.source: "qrc:/icons/theme.svg"
                onClicked: window.darkMode = !window.darkMode
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width * 0.33, 300)
        height: window.height
        edge: Qt.LeftEdge

        ColumnLayout {
            width: parent.width
            spacing: 2

            ItemDelegate {
                text: "Compare Files"
                icon.source: "qrc:/icons/compare.svg"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(comparePage);
                    drawer.close();
                }
            }

            ItemDelegate {
                text: "History"
                icon.source: "qrc:/icons/history.svg"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(historyPage);
                    drawer.close();
                }
            }

            ItemDelegate {
                text: "Settings"
                icon.source: "qrc:/icons/settings.svg"
                Layout.fillWidth: true
                onClicked: {
                    stackView.replace(settingsPage);
                    drawer.close();
                }
            }
        }
    }

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

    Component {
        id: comparePage
        ColumnLayout {
            spacing: 20
            anchors.margins: 10

            RowLayout {
                Layout.fillWidth: true
                spacing: 20

                FileCard {
                    id: fileCard1
                    title: "File 1"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                }

                FileCard {
                    id: fileCard2
                    title: "File 2"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                }
            }

            Button {
                text: "Select Files"
                icon.source: "qrc:/icons/add_file.svg"
                Layout.alignment: Qt.AlignHCenter
                onClicked: fileDialog.open()
            }

            Label {
                id: overallScore
                font.pixelSize: 18
                Layout.alignment: Qt.AlignHCenter
            }

            BusyIndicator {
                id: busyIndicator
                running: false
                Layout.alignment: Qt.AlignHCenter
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                visible: resultView.count > 0

                ListView {
                    id: resultView
                    width: parent.width
                    model: ListModel { id: resultModel }
                    //delegate: ResultDelegate {}
                    spacing: 5
                }
            }
        }
    }

    Component {
        id: historyPage
        ScrollView {
            // History implementation would go here
            Label {
                text: "Comparison history will appear here"
                anchors.centerIn: parent
            }
        }
    }

    Component {
        id: settingsPage
        ScrollView {
            // Settings implementation would go here
            ColumnLayout {
                width: parent.width
                Label {
                    text: "Settings"
                    font.pixelSize: 24
                    Layout.alignment: Qt.AlignHCenter
                }

                Switch {
                    text: "Dark Mode"
                    checked: window.darkMode
                    onCheckedChanged: window.darkMode = checked
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Select files to compare"
        fileMode: FileDialog.OpenFiles
        nameFilters: ["Source files (*.cpp *.h *.py *.java *.js)", "All files (*)"]
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

    // ResultDelegate.qml would be a separate component, but here's the inline version
    Component {
        id: resultDelegate
        Rectangle {
            width: ListView.view.width
            height: 60
            radius: 5
            color: index % 2 === 0 ? palette.base : palette.alternateBase

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Label {
                    text: file1.split('/').pop()
                    elide: Text.ElideMiddle
                    Layout.preferredWidth: parent.width * 0.35
                }

                Label {
                    text: "vs"
                    horizontalAlignment: Text.AlignHCenter
                    Layout.preferredWidth: parent.width * 0.1
                }

                Label {
                    text: file2.split('/').pop()
                    elide: Text.ElideMiddle
                    Layout.preferredWidth: parent.width * 0.35
                }

                ProgressBar {
                    value: score / 100
                    Layout.preferredWidth: 100
                }

                Label {
                    text: score.toFixed(2) + "%"
                    font.bold: true
                }
            }
        }
    }
}
