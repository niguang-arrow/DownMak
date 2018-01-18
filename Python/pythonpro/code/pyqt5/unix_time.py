from pprint import pprint as pp
from PyQt5.QtCore import QDateTime, Qt


now = QDateTime.currentDateTime()

pp(dir(now))
unix_time = now.toMSecsSinceEpoch()
print(unix_time)

d = QDateTime.fromMSecsSinceEpoch(unix_time)
print(d.toString(Qt.ISODate))

