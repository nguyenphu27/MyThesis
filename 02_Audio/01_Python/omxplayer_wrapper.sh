#!/bin/bash

# Abort startup if another instance was found    
pidof /path-to-omxplayer > /dev/null && {
  echo Sorry. Only one instance allowed.
  exit 1
}

exit 0