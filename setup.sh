#!/bin/sh
cp init.d/lightbrite /etc/init.d/lightbrite
chmod 755 /etc/init.d/lightbrite
update-rc.d lightbrite defaults
