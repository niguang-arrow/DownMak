from threading import Thread
import threading
import time


# def worker(num):
    # print "worker %s" % num
    # return


# threads = []

# for i in range(5):
    # t = Thread(target=worker, args=(i,))
    # threads.append(t)
    # t.start()


# def worker():
    # print threading.currentThread().getName(), 'Starting'
    # time.sleep(2)
    # print threading.currentThread().getName(), 'Exiting'


# def my_service():
    # print threading.currentThread().getName(), 'Starting'
    # time.sleep(3)
    # print threading.currentThread().getName(), 'Exiting'


# t = Thread(name='my_service', target=my_service)
# w = Thread(name='worker', target=worker)
# w2 = Thread(target=worker) # use default name

# w.start()
# w2.start()
# t.start()


import logging

logging.basicConfig(
    level=logging.DEBUG,
    format='[%(levelname)s] (%(threadName)-10s) %(message)s',
)

def worker():
    logging.debug('Starting')
    time.sleep(2)
    logging.debug('Exiting')


def my_service():
    logging.debug('Starting')
    logging.debug('Exiting')

t = Thread(name='my_service', target=my_service)
w = Thread(name='worker', target=worker)
w2 = Thread(target=worker) # use default name

w.setDaemon(True)
w2.setDaemon(True)

w.start()
w2.start()
t.start()

t.join()
w.join()
w2.join()
