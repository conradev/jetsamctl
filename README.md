# jetsamctl

A simple command line tool for jailbroken devices to modify the Jetsam configuration for a process.

It uses the same syscalls as launchd does when spawning a new process. This would not have been possible without Jonathan Levin's [fantastic article](http://www.newosxbook.com/articles/MemoryPressure.html) on Jetsam.

### Installation

The latest Debian package can be downloaded from the [Releases](https://github.com/conradev/jetsamctl/releases) section.

### Usage

You can use `jetsamctl` to set the per process memory limit or Jetsam priority for a process.

Limit the Phone app to 48 MB of memory:

```
# jetsamctl -l 48 MobilePhone
```

Set the priority of the Photos app to that of an iOS keyboard extension:

```
# jetsamctl -p 8 MobileSlideShow
```

### Priorities

Here is a table of all of the priorities and their numerical values:

| Priority | Value | Examples |
|:--|:--:|:--:|
| Idle | 0 | |
| Idle (Deferred) | 1 | |
| Background (Opportunistic) | 2 | |
| Background | 3 | |
| Mail | 4 | Apple Mail |
| Phone | 5 | Phone app |
| UI Support | 8 | Keyboard extension |
| Foreground Support | 9 | Share extension |
| Foreground | 10 | Foreground application |
| Audio and Accessory | 12 | |
| Conductor | 13 | |
| Home | 16 | SpringBoard |
| Executive | 17 | |
| Important | 18 | |
| Critical | 19 | |


## License

`jetsamctl` is available under the MIT license. See the LICENSE file for more info.
