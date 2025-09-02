import QtQuick
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 550
    height: 700
    visible: true
    title: "Snake Game"
    color: "#edf2f2"

    minimumWidth: width
    minimumHeight: height
    maximumWidth: width
    maximumHeight: height


    property var gameData: snakeController.gameData
    property var gameField: snakeController.gameField
    property var gameNext: snakeController.gameNext
    property bool gameOver: gameField.length === 0
    
    Item {
        anchors.fill: parent
        focus: true
        Keys.onPressed: (event) => {
            let action = -1;
            switch (event.key) {
                case Qt.Key_Enter: action = 0; break;
                case Qt.Key_P: action = 1; break;
                case Qt.Key_Escape: action = 2; break;
                case Qt.Key_Left: action = 3; break;
                case Qt.Key_Right: action = 4; break;
                case Qt.Key_Up: action = 5; break;
                case Qt.Key_Down: action = 6; break;
                case Qt.Key_Space: action = 7; break;
            }
            if (action != -1) {
                snakeController.sendAction(action);
            }
            if (action != 0) {
                snakeController.startGameLoop();
            }
        }
    }

    

    Rectangle{
        id: back
        width: 490
        height: 640
        anchors {
            left: parent.left
            leftMargin: 30
            top: parent.top
            topMargin: 30
        }
        color: "#e8ecec"
        antialiasing: true
        border.color: "#262929"
        border.width: 2
        radius: 10

        Rectangle {
            visible: gameOver
            anchors.fill: parent
            color: "transparent" 
            Rectangle {
                id: game_over_rec
                color: "#edf2f2"
                antialiasing: true
                width: 320
                height: 110
                border.color: "#cdcfcf"
                border.width: 1
                radius: 10
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 230
                }
                Text {
                    id: game_over_txt
                    text: qsTr("GAME OVER")
                    font.pixelSize: 43
                    color: "red"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 30
                    }
                }
            }
            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: game_over_rec.width/2 - 10
                height: 80
                border.color: "#cdcfcf"
                border.width: 1
                radius: 10
                anchors {
                    top: game_over_rec.bottom
                    topMargin: 20
                    left: game_over_rec.left
                }
                Text {
                    id: game_over_score_txt
                    text: qsTr("your score")
                    font.pixelSize: 20
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: game_over_score
                    text: gameData[0]
                    font.pixelSize: 25
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: game_over_score_txt.bottom
                        topMargin: 6
                    }
                }
            }
            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: game_over_rec.width/2 - 10
                height: 80
                border.color: "#cdcfcf"
                border.width: 1
                radius: 10
                anchors {
                    top: game_over_rec.bottom
                    topMargin: 20
                    right: game_over_rec.right
                }
                Text {
                    id: game_over_high_score_txt
                    text: qsTr("high score")
                    font.pixelSize: 20
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: game_over_high_score
                    text: gameData[1]
                    font.pixelSize: 25
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: game_over_high_score_txt.bottom
                        topMargin: 6
                    }
                }
            }
        }

        ColumnLayout {
            id: game_data
            visible: !gameOver
            spacing: 10
            anchors {
                right: parent.right
                rightMargin: 15
                top: parent.top
                topMargin: 23
            }

            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: 140
                height: 80
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10
                Text {
                    id: score_txt
                    text: qsTr("score")
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: score
                    text: gameData[0]
                    font.pixelSize: 25
                    color: gameData[0] >= gameData[1] ? "green" : "black" 
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: score_txt.bottom
                        topMargin: 8
                    }
                }
            }
            
            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: 140
                height: 80
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10

                Text {
                    id: high_score_txt
                    text: qsTr("high score")
                    font.pixelSize: 20                   
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: high_score
                    text: gameData[1]
                    font.pixelSize: 25
                    color: gameData[0] >= gameData[1] ? "green" : "black"                     
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: high_score_txt.bottom
                        topMargin: 5
                    }
                }
            }
            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: 140
                height: 80
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10

                Text {
                    id: level_txt
                    text: qsTr("level")
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: level
                    text: gameData[2]
                    font.pixelSize: 25
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: level_txt.bottom
                        topMargin: 8
                    }
                }
            }
            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: 140
                height: 80
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10

                Text {
                    id: speed_txt
                    text: qsTr("speed")
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: parent.top
                        topMargin: 10
                    }
                }
                Text {
                    id: speed
                    text: gameData[3]
                    font.pixelSize: 25
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: speed_txt.bottom
                        topMargin: 8
                    }
                }
            }

            Rectangle {
                color: "#edf2f2"
                antialiasing: true
                width: 140
                height: 175
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10

                Text {
                    id: next_txt
                    text: qsTr("next")
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignRight
                    anchors {
                        right: parent.right
                        rightMargin: 15
                        top: parent.top
                        topMargin: 10
                    }
                }
                
                Rectangle {
                    id: next
                    width: 120
                    height: 120
                    color: "#edf2f2"
                    anchors {
                        right: parent.right
                        rightMargin: 10
                        top: next_txt.bottom
                        topMargin: 8
                    }

                    GridLayout {
                        anchors.fill: parent
                        columns: 4
                        columnSpacing: 0
                        rowSpacing: 0
                        Repeater {
                            model: gameNext
                            delegate: Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                
                                border.color: "#d9dddd"
                                border.width: 1
                                radius: 8
                                color: {
                                    switch (modelData%10) {
                                        case 1: return "red";
                                        case 2: return "orange";
                                        case 3: return "#fad920";
                                        case 4: return "#c4669d";
                                        case 5: return "green";
                                        case 6: return "blue";
                                        case 7: return "purple";
                                        default: return "#edf2f2";
                                    }
                                }
                            }
                        }
                    }

                    Rectangle{
                        width: next.width
                        height: next.height
                        color: "transparent" 
                        antialiasing: true
                        border.color: "#262929"
                        border.width: 2
                        radius: 8
                    }
                }
            }


            Rectangle {
                color: gameData[4]? "#edf2f2" : "#da362b"
                antialiasing: true
                width: 140
                height: 50
                border.color: "#cdcfcf"
                border.width: 0.5
                radius: 10

                Text {
                    id: pause
                    text: qsTr("PAUSE")
                    font.pixelSize: 20
                    anchors.centerIn: parent
                }
            }
        }
    }

    Rectangle {
        id: field
        visible: !gameOver
        width: 300
        height: 600
        anchors {
            left: back.left
            leftMargin: 20
            top: back.top
            topMargin: 20
        }
        color: "#edf2f2"
        

        GridLayout {
            anchors.fill: parent
            columns: 10
            columnSpacing: 0
            rowSpacing: 0
            Repeater {
                model: gameField
                delegate: Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    
                    border.color: "#d9dddd"
                    border.width: 1
                    radius: 8
                    color: {
                        switch (modelData%10) {
                            case 1: return "red";
                            case 2: return "orange";
                            case 3: return "#fad920";
                            case 4: return "#c4669d";
                            case 5: return "green";
                            case 6: return "blue";
                            case 7: return "purple";
                            default: return "#edf2f2";
                        }
                    }
                }
            }
        }

        Rectangle {
            id: border
            width: parent.width
            height: parent.height
            anchors.centerIn: parent
            antialiasing: true
            color: "transparent" 
            border.color: "#262929"
            border.width: 2
            radius: 8
        }
    }

}
