#!/usr/bin/python
import pyinotify,subprocess

def onChange(ev):
    print(ev)
    cmd = ['/boot/image/run.sh']
    subprocess.Popen(cmd).communicate()
wm = pyinotify.WatchManager()
#wm.add_watch('/boot/image/image.png', pyinotify.IN_MODIFY, onChange)
wm.add_watch('/boot/image/image.png', pyinotify.IN_CLOSE_WRITE, onChange)
notifier = pyinotify.Notifier(wm)
notifier.loop()

