#!/bin/bash
set -x

ps aux | grep -v grep | grep postgres | awk '{print $2}' | while read pid; do kill -9 $pid; done

