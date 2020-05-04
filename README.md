# smbios-thermal-ctl-c
Dell Smbios thermal control C-client

## What it is
Dell distribute smbios-thermal-ctl written on Python and it cpu
intensive when poll. To avoid cpu load i rewrite it to C.

## How to install
```
git clone https://github.com/shinanca/smbios-thermal-ctl-c
cd smbios-thermal-ctl-c
make
sudo make install
```

## Usage

```
Usage: smbios-thermal-c [OPTION]

Options:
   -h, --help                          show this help message and exit
   -i, --info                          This will Display the Supported Features of USTT
   -g, --get-thermal-info              This will Display the Thermal Information of a system
   -s, --set-thermal-mode=THERMAL_MODE Option to set Thermal Mode; balanced, cool-bottom, quiet, performance

```

## Widget
I create fork of [Plasma psates widget](https://github.com/jsalatas/plasma-pstate) to support smbios-thermal-ctl-c: [My fork of pstates widget](https://github.com/shinanca/plasma-pstate)

### Cpu load
Stock plasma-widget:
```
/bin/time ./set_prefs.sh  -read-all
...
0.20user 0.02system 0:00.23elapsed 100%CPU (0avgtext+0avgdata 13668maxresident)k
```

Fork plasma-widget:
```
/bin/time ./set_prefs.sh  -read-all
...
0.06user 0.04system 0:00.10elapsed 100%CPU (0avgtext+0avgdata 4040maxresident)k
```
3xtime faster
