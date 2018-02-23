# PyQt5 API

2018 年 2 月 1 日

```python
from PyQt5.QtCore import Qt,
						 QFile,
    					 QIODevice,
        				 QTextStream,
            			 QSizeF

from PyQt5.QtWidgets import QApplication
from PyQt5.QtWidgets import QWidget,
							QGridLayout,
                            QHBoxLayout,
                            QVBoxLayout,
    						QLineEdit,
        					QTextEdit,
            				QLabel,
                			QMessageBox,                    		
                        	QPushButton,
                            QDialog,
                            QFileDialog
```



在给每个组件设置名字的时候, 会看到比如 `QPushButton("&Edit")` 中 `&` 的使用, 这样我们能使用快捷键操作对应的控件. 之后我们可以在生成的按钮中看到 `Edit` 中的 `E` 下方会出现一个下划线, 使用 `Alt + E` 就能操作这个按钮了.



## QApplication

+ `QApplication(sys.argv)`
+ `sys.exit(app.exec_())` : 进入事件循环, 由于 `exec` 是 Python 的关键字, 所以使用下划线.



## QWidget

+ `setLayout` : 设置布局对象, 比如 `QGridLayout`
+ `setWindowTitle` : 设置窗口名字
+ `show` : 显示组件
+ `hide` : 隐藏组件
+ `setFocus()` : 设置组件 focus, 参数为 Focus 变化的原因, 是一个 `Qt.FocusReason` 对象, 比如 `Qt.OtherFocusReason`.
+ `setEnabled()` : 参数为 bool, 设置组件是否为 enabled, 一般来说(因为有例外情况, 见 Qt Assistant)一个 enabled 的组件能处理鼠标和键盘事件, 而一个 disabled 的组件不能.

## QGridLayout

+ `addWidget` : 向布局对象中添加组件, 比如 `QLabel`, `QLineEdit` 等.
+ `addLayout` : 添加 Layout, 比如 QVBoxLayout.
+ 用于指示组件的位置:
  + `Qt.AlignTop`



## QVBoxLayout

+ 参考 QGridLayout
+ `addStretch` : Adds a stretchable space (a QSpacerItem) with zero minimum size and stretch factor stretch to the end of this box layout.



## QPushButton

- 按钮
- `self.clicked.connect(callable)` : 按钮点击之后会调用 `callable` 方法.





## QLineEdit

+ 单行文本框
+ `setReadOnly(bool)` : 设置是否只读 (`self.setReadOnly(True)`)
+ `text()` : 返回文本
+ `clear()` : 清除文本
+ `setText()` : 设置文本



## QTextEdit

+ 多行文本框
+ `setReadOnly` : 设置是否只读 (`self.setReadOnly(True)`)
+ `toPlainText()` : 返回文本, 注意与 QLineText 的 `text()` 区别
+ `clear()` : 清除文本



## QMessageBox

+ 继承于 QDialog
+ `QMessageBox.information()` : 静态公有成员
+ `QMessageBox.question()` : 静态公有成员, 最后一个参数可以是 `QMessageBox.Yes | QMessageBox.No`





## QDialog

+ 对话框

+ 主要注意 `self.accept` : 在 PyQt 自带的例子中 `Address Book/part5.py` 中, 有这样的代码:

  ```python
  self.findButton.clicked.connect(self.findClicked)
  self.findButton.clicked.connect(self.accept)
  ```

  当 `findButton` 被点击之后, 首先会执行 `self.findClicked`, 之后, 会执行 `self.accept`; 根据 Qt 的手册, `accept()` 方法为 Public Slots, 为 `virtual void` 类型. 而要激活 `accept` 函数, 应该需要 QDialog 上的 `Accepted` 按钮被执行, 所以有下面的判断, 当 `self.diaglog.exec_()` 事件执行代码的结果 `exec_()` 为 `QDialog.Accepted` 时, 采取对应的处理方法.

  ```python
   # 代码来自 findContact 方法
      	if self.dialog.exec_() == QDialog.Accepted:
              contactName = self.dialog.getFindText()

              if contactName in self.contacts:
                  self.nameLine.setText(contactName)
                  self.addressText.setText(self.contacts[contactName])
              else:
                  QMessageBox.information(self, "Contact Not Found",
                          "Sorry, \"%s\" is not in your address book." % contactName)
                  return
  ```

  ​

## QFileDialog

+ `QFileDialog.getSaveFileName` : 静态公有成员

  ```python
          fileName, _ = QFileDialog.getSaveFileName(self, "Save Address Book",
                  '', "Address Book (*.abk);;All Files (*)")

          if not fileName:
              return

          try:
              out_file = open(str(fileName), 'wb')
          except IOError:
              QMessageBox.information(self, "Unable to open file",
                      "There was an error opening \"%s\"" % fileName)
              return
  ```

+ `QFileDialog.getOpenFileName`



## QFile, QIODevice, QFileStream

+ 关于这些类的使用, 可以参考 `Address Book/part7.py` 中的 `exportAsVCard` 方法:

  ```python
          fileName, _ = QFileDialog.getSaveFileName(self, "Export Contact", '',
                  "vCard Files (*.vcf);;All Files (*)")

          if not fileName:
              return

          out_file = QFile(fileName)

          if not out_file.open(QIODevice.WriteOnly):
              QMessageBox.information(self, "Unable to open file",
                      out_file.errorString())
              return

          out_s = QTextStream(out_file)

          out_s << 'BEGIN:VCARD' << '\n'
          out_s << 'VERSION:2.1' << '\n'
          out_s << 'N:' << lastName << ';' << firstName << '\n'
          out_s << 'FN:' << ' '.join(nameList) << '\n'

          address.replace(';', '\\;')
          address.replace('\n', ';')
          address.replace(',', ' ')

          out_s << 'ADR;HOME:;' << address << '\n'
          out_s << 'END:VCARD' << '\n'

          QMessageBox.information(self, "Export Successful",
                  "\"%s\" has been exported as a vCard." % name)
  ```

  让将文件对象转换为流对象, 可以处理不同类型的文件.