from PyQt5.QtCore import (Qt,
                         QRect,
                         QRectF,
                         QSize,
                         QSizeF)
from PyQt5.QtGui import (QFont,
                        QIcon,
                        QPixmap)
from PyQt5.QtWidgets import (QAction,
                            QApplication,
                            QGraphicsItem,
                            QGraphicsScene,
                            QGridLayout,
                            QHBoxLayout,
                            QLabel,
                            QWidget,
                            QToolButton)


class ToolBoxItem(QWidget):
    def __init__(self, iconpath, description):
        super(ToolBoxItem, self).__init__()
        self.icon = QIcon(QPixmap(iconpath).scaled(30, 30))

        self.button = QToolButton()
        self.button.setCheckable(True) # a toggle button
        self.button.setIcon(self.icon)
        self.button.setIconSize(QSize(50, 50))
        layout = QGridLayout() 
        layout.addWidget(self.button, 0, 0, Qt.AlignHCenter)
        layout.addWidget(QLabel(description), 1, 0, Qt.AlignCenter)
        self.setLayout(layout)


if __name__ == '__main__':
    import sys

    app = QApplication(sys.argv)
    item = ToolBoxItem('./images/textitem.png', 'text')
    item.show()
    sys.exit(app.exec_())
