# fastCFWswitch
A tesla based overlay to boot into different payloads on the Nintendo Switch.

## Supported devices
Only Switch consoles with the old Erista SOC are supported, because exosphere uses a bootrom exploit to reboot to a payload.

## Setup
* Setup the [tesla menu](https://github.com/WerWolv/Tesla-Menu) and [nx-ovlloader](https://github.com/WerWolv/ovl-sysmodules)
* Extract the [latest release](https://github.com/Hartie95/fastCFWswitch/releases/latest) to the sd card
* Modify the configuration in `/config/fastCFWSwitch/config.ini`

## Configuration

Attribute   | possible values                            | Description
----------- | ------------------------------------------ | ----------------
`type`      | empty / `section` / `payload`              | This defines the type of element this section defines, if left empty it will automatically differentiate it by checking if a path is set
`name`      | any string - example: `atmosphere`         | The entry name, shown in the Overlay
`path`      | a path - example: `/bootloader/hekate.bin` | the absolute path, based on the root of the sd, to the payload that should be loaded
`bootPos`   | a number                                   | the config position of the config the target payload should load, currently only supported for hekate
`bootId`    | an 1-7 character long string - example `atmos` | the id of the config entry the target should be loaded, supported for hekate 5.0.2+ 

## Example configuration
```
[CFWS]
type=section
name=cfws

[SYSNAND]
name=sysnand
path=/payloads/hekate_ctcaer.bin
bootId=sysnand

[EMUNAND]
name=emunand
path=/payloads/hekate_ctcaer.bin
bootId=emunand

[ATMOSPHERE]
name=atmosphere
path=/atmosphere/reboot_payload.bin

[SXOS]
name=SxOS
path=/sxos/reboot_payload.bin

[ANDROID]
name=Android
path=/payloads/hekate_ctcaer.bin
bootPos=2

[TOOLS]
name=Tools

[HEKATE]
name=Hekate
path=/payloads/hekate_ctcaer.bin

[LOCKPICKRCM]
name=Hekate
path=/payloads/Lockpick_RCM.bin

```

## Screenshots
![Example Menu](../master/media/exampleMenu.jpg?raw=true)


## Credits
* [libtestla](https://github.com/WerWolv/libtesla) by [WerWolv](https://github.com/WerWolv) as base for this overlay
* [libnx](https://github.com/switchbrew/libnx) as base of almost all switch homebrew
* [inih](https://github.com/benhoyt/inih) for the ini parsing lib
