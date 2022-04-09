# Bluetooth-Controlled LED

[![Compile Master Code](https://github.com/JerryAZR/BluetoothLED/actions/workflows/compile-master.yml/badge.svg)](https://github.com/JerryAZR/BluetoothLED/actions/workflows/compile-master.yml)
[![Compile Slave Code](https://github.com/JerryAZR/BluetoothLED/actions/workflows/compile-slave.yml/badge.svg)](https://github.com/JerryAZR/BluetoothLED/actions/workflows/compile-slave.yml)

More to be added later

## Parts

### Controller

* **Seeeduino XIAO BLE (Sense)**

    BLE would be enough for the basic project, but Sense comes with more sensors
which may be useful if we want to build extensions.

* **Battery Housing**

    Seeeduino XIAO requires 5V power supply to work properly 

* **KY-040-like Rotary Encoder**

    The program is designed to work with encoders that have a CLK (clock) pin,
a DT (data) pin, and a SW (switch) pin. The CLK and DT signals are 90 degree
out of phase when operating.

* **Display**

    Seeeduino XIAO BLE (Sense) has built in support for UART/I2C/SPI, so any
display module supporting those protocols would technically work. But do note
that Seeeduino XIAO only has 11 digital pins.

* **MAX6817** (switch debouncer, optional)

    Software debouncing is already pretty good. Although a hardware debouncer
would simplify the software control logic, it wouldn't have much effect on the
performance.

### Receiver

* **Arduino Nano + HM10**

    Can be replaced by another Seeeduino XIAO BLE. Sensor is useless on the
receiver side. HM10 can be replaced by other similar BLE-UART module
(HM-05 and 06 are classic Bluetooth modules so they may not work).

* **IRL540** (logic level MOSFET, need one for each control line)

    This component allows us to control the power line (on/off) with a 5V
signal, so we need one for each control line. Similar MOSFETs like IRL530
and IRL520 would also work. If none of these are available, IRF540/530/520
could be used, but the performance may not be ideal.

* **Power Supply & Regulator**

    The LED runs on 24V and the Arduino Nano runs on 5V (if using the 5V pin)
or 7 - 12V (if using the VIN pin)

* **Fuse**

    It never hurts to be careful

### Generic

Resistors, capacitors, wires, pin headers, and other commonly used components

### 控制器部分

* **Seeeduino XIAO BLE (Sense)**

    BLE（没有Sense）版本要实现基础功能已经足够了。Sense 版本多了不少内置传感器，如果
之后想在这个项目之上做拓展的话可能会有用。

* **电池盒**

    Seeeduino XIAO需要5V供电。

* **旋转编码器**

    这个项目目前的软件针对的是三个针脚的旋转编码器（CLK，DT，SW）。其中CLK和DT在工作
时会有90°的相位差，用于读取旋转的方向和速度。SW则对应的是旋钮是否被按下。

* **屏幕**

    Seeeduino XIAO BLE (Sense) 自带 UART/I2C/SPI 支持, 所以使用这些协议的显示器理论上都能用。
建议选择耗电量小的屏幕。另外 Seeeduino XIAO 只有11个数字针脚，假如之后要拓展这个项目的话
最好把针脚用量也纳入考虑。

* **MAX6817** (switch debouncer, 可选)

    Software debouncing is already pretty good. Although a hardware debouncer
would simplify the software control logic, it wouldn't have much effect on the
performance.

    （我不知道这段该怎么翻译）简单来说机械开关（编码器同理）在状态变换的时候会出现信号不稳定
的情况，导致软件部分的逻辑判断出现错误。我现在已经在用的软件解决方案效果不错。也可以使用硬件
方案来替代。虽然能够简化控制逻辑但我不认为会对实际表现有什么影响。

### 接收器部分

* **Arduino Nano + HM10**

    也可以用第二个 Seeeduino XIAO BLE. HM10 可以用其他类似的 BLE-UART 模块代替
(HM-05 和 06 是传统蓝牙，应该不能用).

* **IRL540** (logic level MOSFET, 每条控制线需要一个)

    相当于是5V控制的电控开关。在项目中我们会借由这个原件来使用PWM控制LED亮度
其他类似的 MOSFET 比如 IRL530，IRL520 也行。实在没有的话可以用 IRF540/530/520
来替代，不过IRF系列未必能达到我们的理想效果。

* **保险丝**

    小心一点总没错。

### 通用

电容，电阻，电线，针脚，以及其他电路中常用的元件。

