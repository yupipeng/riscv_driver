# OPTEK-RISCV-SDK



## structure
> detail each part in the toolset and sdk

### toolset


code was compiled by a modified gcc-10,
program was debuged by openocd and gdb
program was download by openocd
project was managed by e-ide, which manage source file, and work with gcc(compile)/openocd(debug tool),cortex-debug(make gdb usable in vscode)/cpptools(language server protocol)

```json
    "EIDE.SourceTree.AutoSearchIncludePath": true,
    "EIDE.OpenOCD.ExePath": "c:\\Users\\admin\\Desktop\\NucleiStudio_IDE_2022_12\\Nuclei Studio IDE\\NucleiStudio\\toolchain\\openocd\\bin\\openocd.exe",
    "EIDE.RISCV.ToolPrefix": "riscv-nuclei-elf-",
    "EIDE.RISCV.InstallDirectory": "c:\\Users\\admin\\Desktop\\NucleiStudio_IDE_2022_12\\Nuclei Studio IDE\\NucleiStudio\\toolchain\\gcc"
```

```
tools   -- use to gen programming bin from builds output
```


### sdk

```
apllication   -- user code should modified in this folder

Core          -- hardware code about, through these to enable different hardware function
              -- *maybe generate by visual tool in the future*

Optek-SDK     -- lowlevel lib
              -- in general, no need to be modified by user

```

## getting start

### install plugins
> if you download full toolset provided by optek, just skip this chapter

* cpptools
* eide
* cortex-debug

### how to import an exist project

### how to gen a new project

### how to manager multiple projects

### how to build

### how to download

### how to debug
