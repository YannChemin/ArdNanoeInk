# Arduino Nano eInk

Display Random DND PNJ characters by uploading the xbm files to Arduino Nano on a 2.7 e-ink display

Please change the absolute path in dnd.ino line 18 to your directory location

~~~C
// From now on value is always "pnj"
#define IMAGE_1_FILE "/home/yann/dev/ArdNanoeInk/pnj.xbm"
~~~

Then ensure that Arduino IDE is installed and run:

~~~shell
bash ./compile.sh
~~~

![Setup](Figure.png "Setup")



![Output](Figure_out.png "Output")
