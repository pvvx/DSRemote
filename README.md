# DSRemote
DSRemote Windows 10 x64 (Desktop_Qt_5_13_2_MinGW_64_bit)
Operate your Rigol oscilloscope from your Windows desktop.
Only LAN connection supported.

Clone from:<br>
[DSRemote written by Teuniz](https://gitlab.com/Teuniz/DSRemote)<br>
[DSRemote)(https://www.teuniz.net/DSRemote/index.html)<br>

Supported devices
---------------

DS6000 and DS1000Z series oscilloscopes.

### MSO4000/DS4000 series

There is some basic support like capturing screenshots.

Not tested yet:

* Record & Playback function
* Wave Inspector
* serial decoding.


DS1000Z series:
---------------
DSRemote assumes that, on your DS1054Z, all options are installed.
If that is not the case it's possible that the software does not work correctly.
On your scope, go to the menu Utility -> Options -> Installed and make sure you have the following options installed:

* DECODER RS232,I2C,SPI Official
* MEM_DEPTH 24M/12M/6M Official
* TRIGGER Timeout,Runt,Window,Delay,Setup,NthEdge,RS232,I2C,SPI Official
* RECORDER Record/Re-play Official
* BANDWIDTH 100M Official

[Готовые исполняемые файлы для Win10x64](https://github.com/pvvx/DSRemote/blob/master/bin/DsRemote.zip)