#! /bin/sh
gcc simplest_ffmpeg_remuxer.c -g -o simplest_ffmpeg_remuxer.out -I /usr/local/include -L /usr/local/lib \
-lavformat -lavcodec -lavutil
