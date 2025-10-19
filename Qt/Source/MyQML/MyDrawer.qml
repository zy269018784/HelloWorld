import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts
import QtQuick.Controls


Drawer {
	width: 0.66 * parent.width
	height: parent.height
	Label {
		text: "Content goes here!"
		anchors.centerIn: parent
	}
}
