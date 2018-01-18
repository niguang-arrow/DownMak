import sys
# from PyQt5.QtWidgets import QApplication, QWidget
from pprint import pprint as pp

from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5.QtGui import QIcon, QFont
from PyQt5.QtWidgets import (QWidget, QToolTip, QPushButton, QApplication)

class Example(QWidget):
    def __init__(self):
        super(Example, self).__init__()

        self.initUI()


    def initUI(self):
        self.setGeometry(300, 300, 300, 220)
        self.setWindowTitle('Icon')
        self.setWindowIcon(QIcon('/home/ieric/Pictures/ted02.jpg'))

        self.show()


class Example2(QWidget):
    def __init__(self):
        super(Example2, self).__init__()

        self.initUI()

    def initUI(self):
        QToolTip.setFont(QFont('SansSerif', 10))
        self.setToolTip('This is a <b>QWidget</b> widget')

        btn = QPushButton('Button', self)
        btn.setToolTip('This is a <b>QPushButton</b> widget')
        btn.resize(btn.sizeHint())
        btn.move(50, 50)

        self.setGeometry(300, 300, 300, 200)
        self.setWindowTitle('Tooltips')
        self.show()


if __name__ == '__main__':

    app = QApplication(sys.argv)


    ex = Example2()

    sys.exit(app.exec_())
