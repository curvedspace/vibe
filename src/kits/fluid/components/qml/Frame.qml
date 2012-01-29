
import QtQuick 2.0

Item {
    id: frame

    property alias source: border.source
    property alias margins: frame.margin

    BorderImage {
        id: border
        anchors.fill: parent
    }
}
