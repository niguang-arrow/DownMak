from PyQt5.QtCore import (Qt,
                         QSize,)
from PyQt5.QtGui import (QImage,
                         QPainter,
                         QPen,
                         QPixmap,)
from PyQt5.QtWidgets import (QApplication,
                          QMainWindow,
                          QFrame,
                          QLabel,
                          QPushButton,
                          QTableWidget,
                          QTableWidgetItem,
                          QFileDialog,
                          QMessageBox,
                          QLayout,
                          QHBoxLayout,
                          QVBoxLayout,
                          QGridLayout,)
import Segmentation
import numpy as np
from mlabwrap import mlab
import os.path

"""
@params:
    parent : default MainWindow
    header : list, property list, i.e. ["Name", "Property"]
"""
class PropertyView(QTableWidget):
    def __init__(self, parent, header):
        super(PropertyView, self).__init__(10, len(header), parent)

        # self.setColumnCount(len(header))
        self.setHorizontalHeaderLabels(header)
        self.verticalHeader().setVisible(False)
        self.horizontalHeader().setStretchLastSection(True)

    # len(data[0]) must be equal to len(header)
    def createTable(self, data): # data : [(a1, b1, c1), (a2, b2, c2)]
        self.setRowCount(len(data))
        for row, pairs in enumerate(data):
            for col, item in enumerate(pairs):
                Item = QTableWidgetItem(item) 
                self.setItem(row, col, Item)

        # self.resizeColumnToContents(0)

class Button(QPushButton):
    def __init__(self, name, parent, triggerFunc, enabled=True, size=QSize(120, 42)):
        super(Button, self).__init__(name, parent)
        self.setEnabled(enabled)
        self.setMinimumSize(size)
        if triggerFunc is not None:
            self.clicked.connect(triggerFunc)

"""
@params:
    parent: QWidget 
    name : QString, Original Image | Zoom Area
    imageSize : QSize, size of area used for displaying images
"""
class DisplayArea(QFrame):
    def __init__(self, parent, name, imageSize, withButton=False):
        super(DisplayArea, self).__init__(parent)

        self.imageSize = imageSize
        self.nameLabel = QLabel(name)
        self.imageLabel = QLabel()
        self.imageLabel.setFrameShape(QFrame.StyledPanel)
        self.imageLabel.setFrameShadow(QFrame.Sunken)
        self.imageLabel.setLineWidth(2)
        self.imageLabel.setMinimumSize(self.imageSize)
        self.path = '.' # pwd, used for QFileDialog

        layout = QGridLayout()
        layout.addWidget(self.nameLabel, 0, 0)
        layout.addWidget(self.imageLabel, 1, 0, 1, 2)
        if withButton:
            self.loadButton = Button("L&oad", parent, self.loadImage)
            self.detectButton = Button("&Detect", parent, self.detectImage, False)
            layout.addWidget(self.loadButton, 2, 0, Qt.AlignLeft)
            layout.addWidget(self.detectButton, 2, 1, Qt.AlignRight)
        self.setLayout(layout)

    def loadImage(self):
        """ Specify an image file.
            If a file is selected, the appropriate function is called to process
            and display it.
        """
        self.imageLabel.clear()
        self.parent().parent().zoomArea.imageLabel.clear()

        imageFile, _ = QFileDialog.getOpenFileName(self,
                "Choose an image file to open", self.path, "Images (*.*)")

        if imageFile != '':
            self.openImageFile(imageFile)
            self.path = imageFile
            # if not os.path.exists("result.jpg"):
            Segmentation.Seg(str(self.path))
            try:
                self.position = mlab.rectangle("result.jpg")
            except IOError as e:
                raise e

    def openImageFile(self, imageFile):
        """ Load the image from the file given.
        """
        originalImage = QImage()

        if originalImage.load(imageFile):
            self.scaledImage = originalImage.scaled(self.imageSize.width(), self.imageSize.height()) #Qt.KeepAspectRatio

            self.setImage(self.scaledImage)
        else:
            QMessageBox.warning(self, "Cannot open file",
                    "The selected file could not be opened.",
                    QMessageBox.Cancel, QMessageBox.NoButton,
                    QMessageBox.NoButton)

    def setImage(self, image):
        """ View  the original image,
            and enables the Detect button.
        """
        self.imageLabel.setPixmap(QPixmap.fromImage(image))
        self.detectButton.setEnabled(True)


    def detectImage(self):
        if not os.path.exists("result.jpg"):
            return
        self.resultPixmap = QPixmap("result.jpg")
        painter = QPainter(self.resultPixmap)
        pen = QPen(Qt.red)
        pen.setWidth(2)
        painter.setPen(pen)
        for (x, y, w, h) in self.position:
            painter.drawRect(x, y, w, h)
        # self.parent().parent().originImage.imageLabel.setPixmap(
            # self.resultPixmap)
        self.resultPixmap.save("rectangle.jpg", "JPG")
        self.parent().parent().zoomArea.imageLabel.setPixmap(
            QPixmap("rectangle.jpg").scaled(self.imageSize.width(), self.imageSize.height()))


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.setWindowTitle("PCB Detection")
        self.setCentralWidget(self.createCentralWidget())

    def createCentralWidget(self):
        frame = QFrame(self)
        frame.setFrameShape(QFrame.Box)
        frame.setLineWidth(0)
        grid = QGridLayout(frame)
        grid.setSpacing(8)
        grid.setContentsMargins(4, 4, 4, 4)

        imageSize = QSize(768, 348)
        self.originImage = DisplayArea(frame, "Original Image", imageSize, True)
        self.zoomArea = DisplayArea(frame, "Zoom Area", imageSize)
        header = ["Name", "Property"];
        self.tableView = PropertyView(frame, header)

        # data = [(str(i), str(j)) for i, j in enumerate(range(20))]
        # self.tableView.createTable(data)

        grid.addWidget(self.originImage, 0, 0, Qt.AlignTop)
        grid.addWidget(self.zoomArea, 1, 0)
        grid.addWidget(self.tableView, 0, 1, 2, 1)
        grid.setColumnStretch(0, 2) # 2/3
        grid.setColumnStretch(1, 1) # 1/3

        return frame


if __name__ == '__main__':

    import sys
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
