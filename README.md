# time-tracker
Command line time tracker for session/accumulated time.

## Install
This requires ncurses, fmt, and HH's date library to build.

```shell
$ git clone https://github.com/cppcooper/time-tracker.git
$ cd time-tracker
$ git submodule init
$ git submodule update
$ mkdir build
$ cd build
$ cmake .. -G Ninja
$ sudo ninja install
```

## Configuration
The program reads from `~/.config/time-tracking-wage` to load an hourly wage to use for calculations. The value is formatted in cents. eg.
```
# file
3800
# $/hr
$38/hr
```

## Example
```
Time Tracker
- - - - - -
 -        Space   -- clock(in/out).
 - Ctrl + C       -- exit.
                  [CLOCKED IN]
                  [2022-04-16]
Accumulated Time: 36:19:26 - $653.83
Session Time:     05:29:26 - $98.83
```
