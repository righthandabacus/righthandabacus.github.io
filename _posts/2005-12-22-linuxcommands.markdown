---
layout: post
title: "Off-the-shelf GNU/Linux (or Debian) Commands"
date: 2005-12-22 00:07:08 +0800
lastedit: Fri, 18 May 2018 16:42:53 -0400
tags: system reference
category: blog
---
# Finding duplicate files in the tree

    find * -type f -exec md5sum \{\} \; | sort | uniq -D -W1 | less

# Add quota support to new partition

    quotacheck -gc -F vfsv0 /mount_point

# Refresh framebuffer fonts

    consolechars -f /usr/share/consolefonts/default8x16.psf.gz

# Accumulator

    echo $[ `ps aux | grep httpd | awk '{print $6}' | sed -e 's/$/+/'` 0 ]

# Reverse cat, equivalent to GNU tac command

Use any of the following three:

```bash
sed -ne '1!G;h;$p' FILE
awk '{ lines[NR]=$0 }end {for(i=NR;i>0;i--) printlines }' FILE
perl -e 'print reverse <>' FILE
```

# Reset the terminal

Keyboard press Ctrl-V Ctrl-O

# Epoch to String

    date -d'19700101 UTC 123456sec'

# String to Epoch

    date -d'2004-1-1 HKT 0:0:0' +%s

# Killing blank lines and comments from a file

    perl -pe '$_="" if (m/^\s*#/ || m/^$/)' FILE

# UNIX newline to DOS newline

    perl -pe 's/$/\r/' FILE

# DOS newline to UNIX newline

    perl -pe 's/\r//' FILE

# Burning CD

```bash
mkisofs -JlqR -joliet-long -o file.iso $SOURCE_LIST
sudo cdrecord -dev=ATAPI:1,0,0 -driveropts=burnfree -gracetime=0 file.iso
```
# Transcoding a MPEG movie from VCD to H.264+AAC

Tools used: `mencoder` (from mplayer, to extract and encode video into H.264 format), `ffmpeg` (to create AAC audio), `mp4box` (to multiplex video and audio)

```bash
mencoder $MPEG_FILE -ovc x264 -nosound -ni -sws 0 -x264encopts \
    threads=2:me=2:qp_constant=22:i4x4:keyint=120:nocabac:deblock:nob_adapt:bframes=1 \
    -ofps 25 -vop scale=320:240,pp=0x33,harddup -of rawvideo \
    -o $VIDEO_FILE
ffmpeg -i $VIDEO_FILE -i $MPEG_FILE -y -vn -f mp4 -acodec aac -ab 128 \
    -ar 48000 -ac 2 -map 1.1:0.0 $AAC_FILE
mp4box  -fps 25 -add $VIDEO_FILE -add $AAC_FILE -new $MP4_FILE
rm $VIDEO_FILE $AAC_FILE
```
or in two-pass:

```bash
mencoder $MPEG_FILE -ovc x264 -nosound -ni -sws 0 -x264encopts \
    threads=2:me=2:bitrate=297:qp_min=22:qp_max=51:i4x4:keyint=120:cabac:deblock:nob_adapt:bframes=1:pass=1 \
    -passlogfile $LOG_FILE -ofps 25 -vop scale=352:288,pp=0x33,harddup -of rawvideo \
    -o $VIDEO_FILE
mencoder $MPEG_FILE -ovc x264 -nosound -ni -sws 0 -x264encopts \
    threads=2:me=2:bitrate=297:qp_min=22:qp_max=51:i4x4:keyint=120:cabac:deblock:nob_adapt:bframes=1:pass=2 \
    -passlogfile $LOG_FILE -ofps 25 -vop scale=352:288,pp=0x33,harddup -of rawvideo \
    -o $VIDEO_FILE
ffmpeg -i $VIDEO_FILE -i $MPEG_FILE -y -vn -f mp4 -acodec aac -ab 128 \
    -ar 44100 -ac 2 -map 1.1:0.0 $AAC_FILE
mp4box -fps 25 -add $VIDEO_FILE -add $AAC_FILE -new $MP4_FILE
rm $VIDEO_FILE $AAC_FILE
```

This is the same thing from ffmpegX:

```bash
printf "Encoding started on " && date &&
ffmpeg -i $SOURCE_FILE -y \
    -itsoffset 0.5 -threads 1 -f psp -title 'Movie' \
    -vcodec h264 -b 296 -s 352x240 -r ntsc -g 250 -qmin 10 -qmax 51 \
    -acodec aac -ab 48 -ar 44100 -ac 2 -dct 8x8 -coder 1 \
    $OUTPUT_FILE
printf "Encoding completed on " && date && printf "\a"
```

```bash
ffmpeg -i $SOURCE_FILE -an -f yuv4mpegpipe -s 640x480 -r 30000/1001 - |
x264 -v -A i4x4 -b 0 --qpmin 0 --qpmax 51 -B 791 \
    --me hex --threads 2 --level 51 --fps 30000/1001 \
    -o $VIDEO_FILE - 640x480 &&
ffmpeg -i $VIDEO_FILE -i $SOURCE_FILE -y -vn -f mp4 \
    -acodec aac -ab 128 -ar 48000 -ac 2 -map 1.1:0.0 $AUDIO_FILE &&
mp4box -fps 30000:1001 -add $VIDEO_FILE -add $AUDIO_FILE -new $OUTPUT_FILE
rm $VIDEO_FILE $AUDIO_FILE
```

# Transcoding using mencoder

```bash
mencoder \
    -vf scale -zoom -xy 640 -aspect 4:3 \  # Scale the video to 640x480
    -oac mp3lame \                         # Audio encode using MP3
    -ovc lavc \                            # Video encode using libavcodec
    -lavcopts vcodec=mpeg4:mbd=1:vbitrate=1500:threads=2 \
         # libavcodec options: MPEG4, bitrate, threads during encoding, etc.
    -o OUTPUT.AVI \                        # Output file
    INPUT1.mpg INPUT2.mpg ...              # Input files
```

# Base64 Encoding and Decoding

```bash
perl -m'MIME::Base64' -pe '$_=MIME::Base64::decode($_);' base64.txt > binary.zip
perl -m'MIME::Base64' -pe '$_=MIME::Base64::encode($_);' binary.zip > base64.txt
```

# Crop a range of lines from a text file

From line 1234 to line 1357 of a large text file, use any of these four:

```bash
head -1357 input.txt | tail $[ 1357-1234 ]
sed -n 1234,1357p input.txt
sed -n '1234,1357p;1358q' input.txt
awk 'NR>=1234&&NR<=1357' input.txt
```

`sed` option `-n` prohibits implicit output, the `p` command prints the lines,
`q` command quit the stream upon a particular line.
(Source: [stackoverflow question](https://stackoverflow.com/questions/83329/how-can-i-extract-a-predetermined-range-of-lines-from-a-text-file-on-unix#comment20465347_83347))

# "Open with" associations in GUI

Right click "open with" menu is populated from a combination of `mimeinfo.cache`
and `mimeapps.list` files in the system. The file `mimeinfo.cache` is generated
automatically from a bunch of `defaults.list`. The location of these files are
(in precedence order):

1. `~/.local/share/applications/mimeapps.list`
2. `~/.local/share/applications/defaults.list`
3. `/usr/local/share/applications/defaults.list`
4. `/usr/share/applications/defaults.list`
5. `/usr/share/applications/mimeinfo.cache`

([Source](https://lkubaski.wordpress.com/2012/10/29/understanding-file-associations-in-lxde-and-pcmanfm/))
