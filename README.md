# fastCFWswitch
A tesla based overlay to boot into different payloads on the Nintendo Switch.

## Supported devices
Only Switch consoles with the old Erista SOC are supported, because exosphere uses a bootrom exploit to reboot to a payload.

## Setup
* Setup the [tesla menu](https://github.com/WerWolv/Tesla-Menu) and [nx-ovlloader](https://github.com/WerWolv/ovl-sysmodules)
* Extract the [latest release](https://github.com/Hartie95/fastCFWswitch/releases/latest) to the sd card
* Modify the configuration in `/config/fastCFWSwitch/config.ini`

## Configuration

Attribute   | possible values                                        | Description
----------- | ------------------------------------------------------ | ----------------
`type`      | empty / `section` / `payload`                          | This defines the type of element this section defines, if left empty it will automatically differentiate it by checking if a path is set
`name`      | any string - example: `atmosphere`                     | The entry name, shown in the Overlay
`path`      | a path - example: `/bootloader/hekate.bin`             | The absolute path, based on the root of the sd, to the payload that should be loaded
`bootPos`   | a number                                               | The config position of the config the target payload should load, currently only supported for hekate
`bootId`    | an 1-7 character long string - example `atmos`         | The id of the config entry the target should be loaded, supported for hekate 5.0.2+ 
`ums`       | string from UMS target list - example `sd`             | The ums target, that should automatically be exposed via usb, supported for hekate

### UMS Target
Target           | possible values                            
---------------- | -----------------------------
SD-Card          | `sd`, default fallback
EMMC BOOT0       | `nand_boot0`, `emmc_boot0`
EMMC BOOT1       | `nand_boot1`, `emmc_boot1`
EMMC Main/GPT    | `nand_gpt`, `emmc_gpt`
EmuNAND BOOT0    | `emu_boot0`
EmuNAND BOOT1    | `emu_boot1`
EmuNAND Main/GPT | `emu_gpt`

## Example configuration
```
[CFW]
type=section
name=CFW

[SYSMMC]
name=sysMMC
path=/bootloader/update.bin
bootId=sysnand

[EMUMMC]
name=emuMMC
path=/bootloader/update.bin
bootId=emunand

[ATMOSPHERE]
name=Atmosphere
path=/atmosphere/reboot_payload.bin

[SXOS]
name=SX OS
path=/sxos/reboot_payload.bin

[ANDROID]
name=Android
path=/bootloader/update.bin
bootId=SWANDR

[Ubuntu]
name=Ubuntu
path=/bootloader/update.bin
bootId=SWR-UBU


[TOOLS]
name=Tools

[HEKATE]
name=hekate
path=/bootloader/update.bin

[LOCKPICKRCM]
name=Lockpick RCM
path=/bootloader/payloads/Lockpick_RCM.bin


[UMS]
name=hekate UMS options

[SD_CARD]
name=microSD
path=/bootloader/update.bin
ums=sd

[EMMC_GPT]
name=EMMC GPT 
path=/bootloader/update.bin
ums=emmc_gpt

```

## Screenshots
![Example Menu](../master/media/exampleMenu.jpg?raw=true)


## Credits
* [libtestla](https://github.com/WerWolv/libtesla) by [WerWolv](https://github.com/WerWolv) as base for this overlay
* [libnx](https://github.com/switchbrew/libnx) as base of almost all switch homebrew
* [inih](https://github.com/benhoyt/inih) for the ini parsing lib
