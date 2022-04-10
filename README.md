# time-tracker
Command line time tracker for session/accumulated time.

## Install
This requires ncurses to build.

```shell
$ git clone https://github.com/cppcooper/time-tracker.git
$ cd time-tracker
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
[Thu 00:20:18] /d/projects/timetracker/build/bin/TimeTracker/x64_Release (master)
 coope@myhost $ ./TimeTracker.exe 15
[Thu 00:20:28]

Time Tracker
-----------
 -        Space   -- pause/resume tracking.
 -        Insert  -- load accumulated time.
 -        BkSpace -- don't show accumulated time.
 - Ctrl + BkSpace -- reset accumulated time.
 - Ctrl + C       -- stop tracking and exit.


Accumulated Time: 00:15:45
Session Time: 00:15:02
```
