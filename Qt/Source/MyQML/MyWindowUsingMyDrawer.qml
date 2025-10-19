import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts
import QtQuick.Controls

Window {
	id: window
    width: 400
    height: 300
    visible: true
    title: qsTr("Hello QML")
    
	MyDrawer {
		id: drawer
    }
}