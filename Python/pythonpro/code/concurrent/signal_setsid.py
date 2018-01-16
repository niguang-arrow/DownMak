import os
import signal
import subprocess
import tempfile
import time
import sys


script = '''#!/bin/sh
echo "Shell script in process $$"
set -x
python signal_child.py
'''

script_file = tempfile.NamedTemporaryFile('wt')
script_file.write(script)
script_file.flush()

def show_setting_sid():
    print 'Calling os.setsid() from %s' % os.getpid()
    sys.stdout.flush()
    os.setsid()


proc = subprocess.Popen(
    ['sh', script_file.name],
    close_fds=True,
    preexec_fn=show_setting_sid
)
print 'PARENT: Pausing before signaling %s...' % proc.pid
sys.stdout.flush()
time.sleep(1)
print 'PARENT: Signaling child %s' % proc.pid
sys.stdout.flush()

os.killpg(proc.pid, signal.SIGUSR1)
time.sleep(3)
