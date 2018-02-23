import sys
from PyQt5.QtWidgets import QWidget, QTextEdit
from PyQt5.QtWidgets import QApplication

app = QApplication(sys.argv)
text = QTextEdit()
text.show()
sys.exit(app.exec_())
