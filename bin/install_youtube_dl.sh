#!/bin/bash
####################################
# This script install youtube-dl
# jeraman.info — March 17 2018
####################################
echo "Installing youtube-dl"
curl -L https://yt-dl.org/downloads/latest/youtube-dl -o /usr/local/bin/youtube-dl
chmod a+rx /usr/local/bin/youtube-dl
echo "youtube-dl should be installed by now."
echo "Please make sure to test everything is working fine."
