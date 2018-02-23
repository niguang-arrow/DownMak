# Readme

2018 年 2 月 5 日

首先介绍 anchorlayout.py 文件中的我感兴趣的内容:

```python
from PyQt5.QtCore import QSizeF, Qt
from PyQt5.QtWidgets import (QApplication,
                            QGraphicsAnchorLayout,
                            QGraphicsProxyWidget,
                            QGraphicsScene,
                            QGraphicsView,
                            QGraphicsWidget,
                            QPushButton,
                            QSizePolicy)
```

下面看:

## QGraphicsScene

> The [QGraphicsScene](qgraphicsscene.html) class provides a surface for managing a large number of 2D graphical items.
>
> The class serves as a container for QGraphicsItems. It is used together with [QGraphicsView](qgraphicsview.html) for visualizing graphical items, such as lines, rectangles, text, or even custom items, on a 2D surface. [QGraphicsScene](qgraphicsscene.html) is part of the [Graphics View Framework](graphicsview.html).
>
> [QGraphicsScene](qgraphicsscene.html) also provides functionality that lets you efficiently determine both the location of items, and for determining what items are visible within an arbitrary area on the scene. With the [QGraphicsView](qgraphicsview.html) widget, you can either visualize the whole scene, or zoom in and view only parts of the scene.

也就是说, `QGraphicsScene` 是和 `QGraphicsView` 紧密相连的.



## QGraphicsView

> The [QGraphicsView](qgraphicsview.html) class provides a widget for displaying the contents of a [QGraphicsScene](qgraphicsscene.html).
>
> [QGraphicsView](qgraphicsview.html) visualizes the contents of a [QGraphicsScene](qgraphicsscene.html) in a scrollable viewport. To create a scene with geometrical items, see [QGraphicsScene](qgraphicsscene.html)'s documentation.
>
> To visualize a scene, you start by constructing a [QGraphicsView](qgraphicsview.html) object, passing the address of the scene you want to visualize to [QGraphicsView](qgraphicsview.html)'s constructor.

在 `QGraphicsView` 的构造函数中传入 `QGraphicsScene` 对象.



## QGraphicsAnchorLayout

> The [QGraphicsAnchorLayout](qgraphicsanchorlayout.html) class provides a layout where one can anchor widgets together in Graphics View. 
>
> The anchor layout allows developers to specify how widgets should be placed relative to each other, and to the layout itself. The specification is made by adding anchors to the layout by calling [addAnchor](qgraphicsanchorlayout.html#addAnchor)(), [addAnchors](qgraphicsanchorlayout.html#addAnchors)() or [addCornerAnchors](qgraphicsanchorlayout.html#addCornerAnchors)().

后面的 `QGraphicsWidget` 可以使用 `setLayout` 设置 layout 为 `QGraphicsAnchorLayout` 对象.

+ `addAnchor`: 

  ```cpp
  QGraphicsAnchor *
  addAnchor(QGraphicsLayoutItem *firstItem, 
            Qt::AnchorPoint firstEdge, 
            QGraphicsLayoutItem *secondItem, 
            Qt::AnchorPoint secondEdge)
  ```

  Creates an anchor between the edge firstEdge of item firstItem and the edge secondEdge of item secondItem. The spacing of the anchor is picked up from the style.



## QGraphicsProxyWidget

> The [QGraphicsProxyWidget](qgraphicsproxywidget.html) class provides a proxy layer for embedding a [QWidget](qwidget.html) in a [QGraphicsScene](qgraphicsscene.html).
>
> [QGraphicsProxyWidget](qgraphicsproxywidget.html) embeds [QWidget](qwidget.html)-based widgets, for example, a [QPushButton](qpushbutton.html), [QFontComboBox](qfontcombobox.html), or even [QFileDialog](qfiledialog.html), into [QGraphicsScene](qgraphicsscene.html). 

看来基于 `QWidget` 的组件是不能直接用在 `QGraphicsScene` 中的, 所以需要用 `QGraphicsProxyWidget` 封装.

## QGraphicsWidget

> The [QGraphicsWidget](graphicsview.html#qgraphicswidget) class is the base class for all widget items in a [QGraphicsScene](qgraphicsscene.html).
>
> [QGraphicsWidget](graphicsview.html#qgraphicswidget) is an extended base item that provides extra functionality over [QGraphicsItem](qgraphicsitem.html). 
>
> Unlike [QGraphicsItem](qgraphicsitem.html), [QGraphicsWidget](graphicsview.html#qgraphicswidget) is not an abstract class; you can create instances of a [QGraphicsWidget](graphicsview.html#qgraphicswidget) without having to subclass it. This approach is useful for widgets that only serve the purpose of organizing child widgets into a layout.
>
> [QGraphicsWidget](graphicsview.html#qgraphicswidget) can be used as a base item for your own custom item if you require advanced input focus handling, e.g., tab focus and activation, or layouts.

其构造函数为:

```cpp
QGraphicsWidget(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags())
```

也就是说, `QGraphicsScene` 对象中的 item 为基于 `QGraphicsWidget` 的组件.



## QSizeF

> The [QSizeF](qsizef.html) class defines the size of a two-dimensional object using floating point precision.



## 综上

+ 先创建一个 `QGraphicsScene`, 使用 `addItem` 可以添加 `QGraphicsItem` 或 `QGraphicsWidget`, 最后使用 `QGraphicsView` 来显示 `QGraphicsScene`;
+ 对于 `QGraphicsWidget`, 可以使用 `setLayout` 设置 `QGraphicsAnchorLayout`
+ 一般 `QGraphicsScene` 中装载的是 `QGraphicsWidget`, 对于 `QWidget` 对象, 需要使用 `QGraphicsProxyWidget` 来封装一下.
+ `QGraphicsAnchorLayout` 可以使用 `addAnchor` 来排列放置 `QGraphicsWidget` 与 `QGraphicsProxyWidget` 对象.
+ 不过说实话, 设置 anchor 真心挺复杂的, 初看的话还是觉得不方便.



2018 年 2 月 6 日

介绍 `elasticnodes.py` 中的内容:

```python
from PyQt5.QtCore import (qAbs,
                         QLineF,
                         QPointF,
                         QRectF,
                         QSizeF,
                         qrand,
                         qsrand,
                         Qt,
                         QTime)
from PyQt5.QtGui import (QBrush,
                        QColor,
                        QLinearGradient,
                        QPainter,
                        QPainterPath,
                        QPen,
                        QPolygonF,
                        QRadialGradient)
from PyQt5.QtWidgets import (QApplication,
                            QGraphicsItem,
                            QGraphicsScene,
                            QGraphicsView,
                            QStyle)
```



## qrand

> Thread-safe version of the standard C++ rand() function.

使用方法: `int qrand()`



## qsrand

> Thread-safe version of the standard C++ srand() function.

使用方法: `void qsrand(seed)`, 一般代码中这样使用:

```python
qsrand(QTime(0,0,0).secsTo(QTime.currentTime()))
```

看下面的 QTime, `secsTo` 返回的是 int.

## QTime

> The [QTime](qtime.html) class provides clock time functions.
>
> [QTime](qtime.html) uses the 24-hour clock format; it has no concept of AM/PM. Unlike [QDateTime](qdatetime.html), [QTime](qtime.html) knows nothing about time zones or daylight-saving time (DST).

构造函数:

> QTime::QTime(int h, int m, int s = 0, int ms = 0)
>
> Constructs a time with hour h, minute m, seconds s and milliseconds ms.

静态函数:

> [static][QTime] QTime QTime::currentTime()
>
> Returns the current time as reported by the system clock.

`secsTo`:

> int QTime::secsTo(const [QTime](qtime.html#QTime) &t) const
>
> Returns the number of seconds from this time to t. If t is earlier than this time, the number of seconds returned is negative.



## QGraphicsScene

其构造函数中还有 `QGraphicsScene(QRect, parent)`, 另外可以使用 `setSceneRect` 对 `QRect` 进行修改.

+ `setItemIndexMethod`

  用于设置 item 的 add, move, remove 的算法, 有 `BspTreeIndex` 与 `NoIndex` 两种, 前者的时间复杂度为 logarithmic, 后者是线性的. 具体看手册.

## QGraphicsView

它可以使用 `setScene` 设置 `QGraphicsScene`, 或者在构造函数中就传入 scene 对象.

+ `setCacheMode`

  设置 View 的哪些部分是 cached, 这样可以加快渲染速度, 比如 Texture, gradient, 以及 alpha blended backgrounds.

+ `setViewportUpdateMode`

  确定 viewport 更新它的内容的方法. 在某些情况下可以提高渲染的性能. 默认是 `MinimalViewportUpdate`, QGraphicsView will update as small an area of viewport as possible when the contents change.

  当前例子中使用的是 `QGraphicsView.BoundingRectViewportUpdate`, 

  > The bounding rectangle of all changes in the viewport will be redrawn.

+ `setRenderHint`

  确定默认的视图渲染提示 (render hints)

+ `setTransformationAnchor`

  这个属性确定 how the view should position the scene during transformations. 默认是 `AnchorViewCenter`, 代码中使用的是 `AnchorUnderMouse` : the point under the mouse is used as the anchor.

+ `setResizeAnchor`

  和上面类似, 但处理当 view 被 resize 时的情况. 使用默认值, `AnchorViewCenter`.

+ `setWindowTitle`

  设置窗口名字

+ `scale`

  > Scales the current view transformation by (sx, sy).

+ `transform`

  > [Q](../qtgui/qtransform.html)Transform QGraphicsView::transform() const
  >
  > Returns the current transformation matrix for the view. If no current transformation is set, the identity matrix is returned.

+ `setMinimumSize`

  这个应该是从 `QWidget` 继承来的. 设置 View 的 minimum size.

+ `sceneRect`

  ```cpp
  QRectF 
  sceneRect() const
  ```

  返回 View 中 Scene 的矩形大小.

  + `QRectF` 还有 `intersects`, `contains` 等方法.
  + `intersects` 与 `contains` 方法返回的是 bool 值, 但是 `intersected` 返回的是交集的矩形.
  + `translated` 返回的是平移后的矩形.

+ ​



## QPainter

+ `fillRect`
+ `setBrush`
+ `drawRect`
+ `font()`
+ `setFont`
+ `setPen`
+ `drawText`
+ `setPen`



## QFont

+ `setBold`
+ `setPointSize`



## Color

- `Qt.darkGreen`
- `Qt.darkGray`
- `Qt.white`
- `Qt.lightGray`
- `Qt.black`



## QObject::startTimer

```cpp
int QObject::startTimer(int interval, Qt::TimerType timerType = Qt::CoarseTimer)
```

> Starts a timer and returns a timer identifier, or returns zero if it could not start a timer.
>
> A timer event will occur every interval milliseconds until [killTimer](qobject.html#killTimer)() is called. If interval is 0, then the timer event occurs once every time there are no more window system events to process.



## timerEvent

在 `elasticnodes.py` 文件中, 使用 `timerEvent` 方法来处理 node 的位置发生变化的情况.

```python
    def timerEvent(self, event):
        # 找出 scene 中的所有 node
        nodes = [item for item in self.scene().items() if isinstance(item, Node)]
		
        # calculateForces 实现没有被 mouse 移动的其他 node 是以弹性的方式移动,
        # 而被 mouse 移动的 node 则是线性的移动, 这个函数的细节稍微复杂一些,
        # 但最后的结果是每个 node 在一个时间间隔内移动的新的位置.
        for node in nodes:
            node.calculateForces()
		
        # 根据 calculateForces() 计算出来的新位置, advance() 实际设置 node 的
        # 新位置, 同时, 如果 node 的位置没有发生变化, 那么其返回 False, 否则返回 True
        # 当 advance() 返回 True 时, 说明 items moved, 设置 itemsMoved=True
        itemsMoved = False
        for node in nodes:
            if node.advance():
                itemsMoved = True
		# 当 node 不再移动的时候, 此时使用 killTimer, 注意到上面关于 QObject::startTimer
        # 的注释, A timer event will occur every interval milliseconds until killTimer() is called.
        # 也就是说, 此时我们需要找到 timer event 发生的起点, 这个定义在 Node 类中的 itemChange 方法中,
        # 当 node 变化时, 调整 edge 的位置, 同时每 1/25s 发生一个 timer event, 即每 1/25s 都要执行 timerEvent
        # 方法, 直到 itemMoved 为 False(即 node 不再移动, 静止的时候)
        if not itemsMoved:
            self.killTimer(self.timerId)
            self.timerId = 0
```

总结一遍: 先要看 Node 类中的 `itemChange` 方法, 判断当有 node 的位置发生变化时, 调整所有的 edge, 之后启动 GraphWidget 中的 `timerEvent`, 不断地调整所有 node 的位置. 当每一个 node 到达自己最终的位置时, 将结束 `timerEvent`, 也就是不用再调整 node 的位置了.

看 Qt Assistant, 查找 `Elastic Nodes Example`, 可以看到更为详细的介绍:

> The timer event handler's job is to run the whole force calculation machinery as a smooth animation. Each time the timer is triggered, the handler will find all nodes in the scene, and call Node::calculateForces() on each node, one at a time. Then, in a final step it will call Node::advance() to move all nodes to their new positions. By checking the return value of advance(), we can decide if the grid stabilized (i.e., no nodes moved). If so, we can stop the timer.



























































