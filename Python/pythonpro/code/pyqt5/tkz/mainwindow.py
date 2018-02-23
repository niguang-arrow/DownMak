from PyQt5.QtCore import (Qt,
                         QRect,
                         QRectF,)
from PyQt5.QtGui import (QFont,
                        QIcon)
from PyQt5.QtWidgets import (QAction,
                            QApplication,
                            QGraphicsItem,
                            QGraphicsScene,
                            QGraphicsView,
                            QGridLayout,
                            QHBoxLayout,
                            QLabel,
                            QMainWindow,
                            QMenu,
                            QWidget,
                            QSizePolicy,
                            QToolBox)
import resource
import shortcut

from toolbox import ToolBoxItem


class MainWindow(QMainWindow):
    def __init__(self):
        super(QMainWindow, self).__init__()

        self.createAction()
        self.createMenus()
        self.createToolbars()

        self.scene = QGraphicsScene(self)
        self.scene.setSceneRect(QRectF(0, 0, 5000, 5000))
        self.view = QGraphicsView(self.scene)

        self.createToolBox()

        centralLayout = QHBoxLayout()
        centralLayout.addWidget(self.toolBox)
        centralLayout.addWidget(self.view)

        widget = QWidget()
        widget.setLayout(centralLayout)

        self.setCentralWidget(widget)
        self.setWindowTitle("tkz")

    def createAction(self):
        self.exitAction = QAction(QIcon(resource.exitIconPath),
                                 "&Exit",
                                 self,
                                 shortcut=shortcut.keyw['exitAction'],
                                 statusTip="exit the application",
                                 triggered=self.close)


    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu("&File")
        self.fileMenu.addAction(self.exitAction)

    def createToolbars(self):
        pass

    def createToolBox(self):
        textButton = ToolBoxItem(resource.textItemPath, 'text')
        textButton1 = ToolBoxItem(resource.textItemPath, 'text')
        textButton2 = ToolBoxItem(resource.textItemPath, 'text')
        textButton3 = ToolBoxItem(resource.textItemPath, 'text')

        itemWidget = QWidget()
        itemLayout = QGridLayout()
        itemLayout.addWidget(textButton, 0, 0)
        itemLayout.addWidget(textButton1, 0, 1)
        itemLayout.addWidget(textButton2, 1, 0)
        itemLayout.addWidget(textButton3, 1, 1)
        itemLayout.setRowStretch(3, 10)
        itemLayout.setColumnStretch(2, 10)
        itemWidget.setLayout(itemLayout)

        self.toolBox = QToolBox()
        self.toolBox.setSizePolicy(QSizePolicy(QSizePolicy.Maximum, QSizePolicy.Ignored))
        self.toolBox.setMinimumWidth(itemWidget.sizeHint().width())
        self.toolBox.addItem(itemWidget, "Basic Shapes")


if __name__ == '__main__':

    import sys

    app = QApplication(sys.argv)
    
    tkz = MainWindow()
    tkz.show()
    sys.exit(app.exec_())
