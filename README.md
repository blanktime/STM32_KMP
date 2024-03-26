## STM32_KMP

简介：本设备通过与小型继电器相连，通过监测设备log TX 连接STM32 RX 触发串口中断，在中断函数中实现KMP算法实时监测输入log关键字，执行精准掉电。

通过ISP模式下载程序，有两种烧录方法：一种是串口下载，需要USB转串口；另一种是USB下载，只需要一根数据线。均利用STM32CubeProgrammer程序进行烧录。

进入ISP模式的方法

- **方法1**：上电状态下，按住**BOOT0**键和**reset**键，然后松开**reset**键，**0.5**秒后松开**BOOT0**键
- **方法2**：掉电状态下，按住**BOOT0**键，上电后**0.5S**松开**BOOT0**键

**Ø串口连接**

USB转串口 (ex.CH340) TX连接PA10，RX连接PA9，同时不要将MCU的Type-C连接到电脑。选择对应COM口，波特率选择115200，其余默认

![image](https://github.com/blanktime/STM32_KMP/assets/52731000/0391caac-0389-470d-8530-b8cf37ff3f3a)


**ØUSB连接**

1. STM32CubeProgrammer勾选USB模式
![image](https://github.com/blanktime/STM32_KMP/assets/52731000/e51d36fb-17d1-46ca-84bc-8d34b899c386)

2. MCU进入ISP模式，使用USB数据线连接电脑
3. 设备管理器->通用串行总线设备出现STM32 Bootloader或者DFU in FS Mode设备。
4. 如果设备管理器出现的设备是STM Device in DFU Mode，烧录软件将无法正确识别设备，需要右键卸载设备同时勾选删除此设备的驱动程序软件，重新安装驱动

**Ø 下载方法**：选择hex文件，确保MCU已经进入ISP模式，点击Download开始下载

等待下载完成后，点击disconnect，按下STM32的reset键即可运行烧录程序。

本文采用设备：

![image](https://github.com/blanktime/STM32_KMP/assets/52731000/c1e0ab48-e6f2-44a8-b316-0a7e2274b774)
