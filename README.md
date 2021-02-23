This project is developed using Tuya SDK, which enables you to quickly develop branded apps connecting and controlling smart scenarios of many devices.For more information, please check Tuya Developer Website.
# IEMCS
参加涂鸦和立创的活动。一个室内环境控制器
室内环境控制器,这个作品可以检测室内温湿度，光强，烟雾浓度（或者说可燃气体）并可以同这些值做出相应的控制，比如恒温恒湿控制，恒光控制，烟雾报警。并可以通过涂鸦联网模块连接到云端，实现远程。比如电动车充电的时候可以监控其环境，可以对因充电导致的燃烧进行报警，同时也可以将温湿度控制在一个合理的区间提高电池的寿命。
# 硬件
* 主控：STM32F103C8T6
* 温湿度传感器：SHT30
* 光照传感器：OPT3001
* 烟雾传感器：MQ-2（可外接）
* 联网模块：WB3S WiFi&BLE 双协议模组（SDK:模块通用固件方案1.1.71）
# 软件设计
## 相关说明
* 开发环境：Keil uVision5（编译器AC5）
* 语言：C
* 目录结构
```
IEMCS(FREERTOS)
├─Doc
├─Libraries
│  ├─CMSIS
│  └─STM32F1xx_HAL_Driver
├─Project
│  ├─.eide
│  ├─.vscode
│  ├─DebugConfig
│  ├─Listings
│  ├─Objects
│  └─RTE
└─User
    ├─FreeRTOS
    ├─GUI
    ├─Hardware
    └─tuya_sdk
```
用Keil uVision5打开Project文件夹下的IECS.uvprojx文件即可打开工程
## 软件设计思路图
![思维导图](https://github.com/xiaoengineer/IEMCS/blob/main/img/%E8%AE%BE%E8%AE%A1%E5%AF%BC%E5%9B%BE.png?raw=ture)
# 作品相关图片
![整体](https://github.com/xiaoengineer/IEMCS/blob/main/img/%E6%95%B4%E4%BD%93.jpg?rwa=ture)
![界面展示](https://github.com/xiaoengineer/IEMCS/blob/main/img/%E7%95%8C%E9%9D%A2%E5%B1%95%E7%A4%BA.png?raw=ture)
# License
MIT

