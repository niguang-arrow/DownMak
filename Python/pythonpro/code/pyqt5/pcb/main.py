from PyQt5.QtCore import (Qt,
                         QSize,
                         QPoint,
                         QRect,)
from PyQt5.QtGui import (QImage,
                         QPainter,
                         QPen,
                         QBrush,
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
        self.setMinimumWidth(500)

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
        self.name = name
        self.nameLabel = QLabel(name)
        self.imageLabel = QLabel()
        self.imageLabel.setFrameShape(QFrame.StyledPanel)
        self.imageLabel.setFrameShadow(QFrame.Sunken)
        self.imageLabel.setLineWidth(2)
        self.imageLabel.setMinimumSize(self.imageSize)
        self.path = '.' # pwd, used for QFileDialog

        self.leftMousePress = False
        self.curDisplayPixmap = None

        layout = QGridLayout()
        layout.addWidget(self.nameLabel, 0, 0, 1, 1)
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
        # self.imageLabel.clear()
        # self.parent().parent().zoomArea.imageLabel.clear()

        imageFile, _ = QFileDialog.getOpenFileName(self,
                "Choose an image file to open", self.path, "Images (*.*)")

        if imageFile != '':
            self.openImageFile(imageFile)
            self.path = imageFile
            if not os.path.exists("result.jpg"):
                Segmentation.Seg(str(self.path))
            try:
                # if matlab has outputs more than 1, nout should be specified.
                self.bbox, self.exactPos, self.roiPos = mlab.rectangle('result.jpg', nout=3)
            except IOError as e:
                raise e

    def openImageFile(self, imageFile):
        """ Load the image from the file given.
        """
        self.originalImage = QImage()

        if self.originalImage.load(imageFile):
            self.scaledImage = self.originalImage.scaled(self.imageSize.width(),
                                    self.imageSize.height()) #Qt.KeepAspectRatio
            self.wScaleFactor = float(self.scaledImage.width()) / self.originalImage.width()
            self.hScaleFactor = float(self.scaledImage.height()) / self.originalImage.height()
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

    
    def toStr(self, *args):
        result = "Postion: {} {} {} {}".format(*args)
        return result

    def detectImage(self):
        if not os.path.exists("result.jpg"):
            return
        self.resultPixmap = QPixmap("result.jpg")
        self.originalPixmap = QPixmap.fromImage(self.originalImage)
        originPainter = QPainter(self.originalPixmap)
        painter = QPainter(self.resultPixmap)
        brush = QBrush(Qt.blue)
        pen = QPen(Qt.red)
        pen.setWidth(2)
        originPainter.setPen(pen)
        painter.setPen(pen)
        for index, ((x0, y0, w0, h0), (x1, y1, w1, h1), (x2, y2, w2, h2)) in \
                            enumerate(zip(self.bbox, self.exactPos, self.roiPos)):
            if (index < 3):
                self.parent().parent().dataValue.append(('Screw', self.toStr(x1, y1, w1, h1)))
            else:
                self.parent().parent().dataValue.append(('Spot', self.toStr(x1, y1, w1, h1)))
            # originPainter.fillRect(x0, y0, w0, h0, brush)
            originPainter.drawRect(x1, y1, w1, h1)
            painter.drawRect(x2, y2, w2, h2)

        self.originalPixmap.save("originWithRect.jpg", "JPG")
        self.resultPixmap.save("resultWithRect.jpg", "JPG")
        self.scaledImage = QPixmap("originWithRect.jpg").scaled(self.imageSize.width(), self.imageSize.height()).toImage()
        self.parent().parent().originImage.imageLabel.setPixmap(
            QPixmap("originWithRect.jpg").scaled(self.imageSize.width(), self.imageSize.height()))
        self.parent().parent().zoomArea.imageLabel.setPixmap(
            QPixmap("resultWithRect.jpg").scaled(self.imageSize.width(), self.imageSize.height()))
        self.parent().parent().tableView.createTable(self.parent().parent().dataValue)

    # whether the mouse is in an rect
    def inRect(self, rectArea, x, y):
        if (x > rectArea[0]) and (x < rectArea[0] + rectArea[2]):
            if (y > rectArea[1]) and (y < rectArea[1] + rectArea[3]):
                return True
        return False

    def mousePressEvent(self, event):
        if self.name != "Original Image":
            return;

        # an image must be loaded first
        if hasattr(self, 'originalImage'):
            if event.button() == Qt.LeftButton:
                if hasattr(self, 'wScaleFactor'):
                    self.leftMousePress = True
                    self.startRectPosx = max(event.x() - self.imageLabel.x(), 0)
                    self.startRectPosy = max(event.y() - self.imageLabel.y(), 0)
                    self.endRectPosx = min(event.x() - self.imageLabel.x(), self.originalImage.width())
                    self.endRectPosy = min(event.y() - self.imageLabel.y(), self.originalImage.height())
                    self.startOriginPosx = max((event.x() - self.imageLabel.x()) / self.wScaleFactor, 0)
                    self.startOriginPosy = max((event.y() - self.imageLabel.y()) / self.hScaleFactor, 0)
                    self.update()
                    for rect in self.exactPos:
                        if self.inRect(rect, self.startOriginPosx, self.startOriginPosy):
                            if hasattr(self, 'originalPixmap'):
                                self.parent().parent().zoomArea.imageLabel.setPixmap(
                                    self.originalPixmap.copy(
                                        rect[0], rect[1],
                                        rect[2], rect[3]
                                    ).scaledToHeight(self.imageSize.height()))
                            else:
                                self.parent().parent().zoomArea.imageLabel.setPixmap(
                                    QPixmap.fromImage(self.originalImage).copy(
                                        rect[0], rect[1],
                                        rect[2], rect[3]
                                    ).scaledToHeight(self.imageSize.height()))

    def paintEvent(self, event):
        if self.name != "Original Image":
            return

        # an image must be loaded first
        if hasattr(self, 'originalImage'):
            # leftbutton is pressed and startOriginPos is attained.
            if self.leftMousePress:
                painter = QPainter(self.imageLabel.pixmap())
                painter.drawPixmap(self.imageLabel.pixmap().rect(), QPixmap.fromImage(self.scaledImage))
                painter.setPen(QPen(Qt.blue, 2))
                selectedRect = QRect(self.startRectPosx,
                    self.startRectPosy,
                    self.endRectPosx - self.startRectPosx,
                    self.endRectPosy - self.startRectPosy).normalized()
                # if (self.startRectPosx <= self.endRectPosx) and \
                   # (self.startRectPosy <= self.endRectPosy):
                painter.drawRect(selectedRect)
                # self.imageLabel.setPixmap(self.imageLabel.pixmap())


    def mouseMoveEvent(self, event):
        if self.name != "Original Image":
            return;

        # an image must be loaded first
        if hasattr(self, 'originalImage'):
            # leftbutton is pressed and startOriginPos is attained.
            if self.leftMousePress:
                self.endRectPosx = min(event.x() - self.imageLabel.x(), self.originalImage.width())
                self.endRectPosy = min(event.y() - self.imageLabel.y(), self.originalImage.height())
                self.update()


    def mouseReleaseEvent(self, event):
        if self.name != "Original Image":
            return;

        # an image must be loaded first
        if hasattr(self, 'originalImage'):
            # leftbutton is pressed and startOriginPos is attained.
            if self.leftMousePress:
                if event.button() == Qt.LeftButton:
                    self.endRectPosx = min(event.x() - self.imageLabel.x(), self.originalImage.width())
                    self.endRectPosy = min(event.y() - self.imageLabel.y(), self.originalImage.height())
                    self.endOriginPosx = min((event.x() - self.imageLabel.x()) / self.wScaleFactor, self.originalImage.width())
                    self.endOriginPosy = min((event.y() - self.imageLabel.y()) / self.hScaleFactor, self.originalImage.height())
                    self.leftMousePress = False
                    self.updateZoomArea()
                    self.update()


    def updateZoomArea(self):
        if hasattr(self, 'startOriginPosx') and \
           hasattr(self, 'endOriginPosx'):
            if (self.startOriginPosx < self.endOriginPosx) and \
               (self.startOriginPosy < self.endOriginPosy):
                if hasattr(self, 'originalPixmap'):
                    self.parent().parent().zoomArea.imageLabel.setPixmap(
                        self.originalPixmap.copy(
                            self.startOriginPosx, self.startOriginPosy,
                            self.endOriginPosx - self.startOriginPosx,
                            self.endOriginPosy - self.startOriginPosy
                        ).scaledToHeight(self.imageSize.height()))
                else:
                    self.parent().parent().zoomArea.imageLabel.setPixmap(
                        QPixmap.fromImage(self.originalImage).copy(
                            self.startOriginPosx, self.startOriginPosy,
                            self.endOriginPosx - self.startOriginPosx,
                            self.endOriginPosy - self.startOriginPosy
                        ).scaledToHeight(self.imageSize.height()))


class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.dataValue = []
        self.setWindowTitle("PCB Detection")
        self.setCentralWidget(self.createCentralWidget())

    def createCentralWidget(self):
        frame = QFrame(self)
        frame.setFrameShape(QFrame.Box)
        frame.setLineWidth(0)
        grid = QGridLayout(frame)
        grid.setSpacing(8)
        grid.setContentsMargins(4, 4, 4, 4)
        self.layout().setSizeConstraint(QLayout.SetFixedSize)

        imageSize = QSize(968, 368)
        self.originImage = DisplayArea(frame, "Original Image", imageSize, True)
        self.zoomArea = DisplayArea(frame, "Zoom Area", imageSize)
        header = ["Name", "Property"];
        self.tableView = PropertyView(frame, header)

        # data = [(str(i), str(j)) for i, j in enumerate(range(20))]
        # self.tableView.createTable(data)

        grid.addWidget(self.originImage, 0, 0, Qt.AlignTop)
        grid.addWidget(self.zoomArea, 1, 0, Qt.AlignTop)
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
