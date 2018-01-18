from pprint import pprint as pp
from PyQt5.QtCore import QDate, QTime, QDateTime, Qt


now = QDate.currentDate()
# pp(dir(QDate))

print(now.toString(Qt.ISODate))
print(now.toString(Qt.DefaultLocaleLongDate))
print(now.daysInMonth())


datetime = QDateTime.currentDateTime()

print(datetime.toString())

time = QTime.currentTime()

print(time.toString(Qt.ISODate))
print(time.toString(Qt.DefaultLocaleLongDate))
