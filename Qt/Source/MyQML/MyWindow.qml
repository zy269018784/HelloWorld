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
    
	LeftView {
		id: leftView
		x: 0
		width: 200
		height: parent.height
		currentIndex: -1
		onCurrentIndexChanged: {
			console.log("currentIndex:", currentIndex)
			if (currentIndex > -1)
			{
				var currentItem = model.get(currentIndex)
				if (stackView.depth > 1) 
					stackView.pop()
				stackView.push(currentItem.source)
				
				console.log("选中的标题:", currentItem.source)
			}
		}
    }
	
	StackView {
        id: stackView
		x: 200
		width: parent.width - 200
		height: parent.height
	}
}