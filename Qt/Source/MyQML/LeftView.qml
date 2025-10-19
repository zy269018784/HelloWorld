import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts
import QtQuick.Controls

ListView
{
	id: myListView
	currentIndex: -1
	model: ListModel {
		ListElement { title: qsTr("MyButton 1"); source: "MyButton.qml" }
		ListElement { title: qsTr("MyButton 2"); source: "MyButton2.qml" }
	}
	delegate: ItemDelegate {
		text: title
		onClicked: {
			console.log("点击了项目:", index, "当前currentIndex:", myListView.currentIndex)
			myListView.currentIndex = index
			console.log("设置后currentIndex:", myListView.currentIndex)
		}
    }
}