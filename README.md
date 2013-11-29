MP3Project
==========
具体接线请到http://www.geek-workshop.com/thread-982-1-1.html

it seems a hardware limit to the project, considering switch to Due.

reminder
0x00:0000 1000 0000 0000=0x0800
the datasheet is reverse.
3-jump out of wav decoing--not used yet
5-selftest--not used yet
6-stream mode--not used yet
12,13,14-record function--not used yet

0x02:0000 0000 0000 0000=0x0000
need precise adujustment, shut down for now

0x03:1111 1000 0000 0000=0xF800

0x04,0x05:no need

0x06,0x07,0x0A,0x0C,0x0D,0x0E,0x0F:
advance usage, don't know how to use yet.

0x08,0x09:
read the header of the music. no use yet.

0x0B: volume control