# 基于 RTMP 协议实现的家庭监控系统



## 简介

该项目是本人的毕业设计，用于快速搭建一套简洁的家庭监控系统。设备端即摄像头是利用闲置手机安装 APP 的方式实现，通过 Android Studio 开发，主要任务是**采集音视频数据、编码压缩、发送 RTMP 流、时间水印、发送设备心跳、视频 I 帧加密**。监控端则是采用 Qt 进行开发，通过设备端生成的注册码添加设备，支持**实时监控、录像回放、语音对讲**。

整个系统基于 **RTMP** 协议进行实时的视频、音频流传输，并利用 **gRPC** 框架搭建了一套客户端服务器即当前仓库，为设备端和PC端提供接口服务，并对设备进行录像存储。另外，本项目还使用到了 **libx264、libfaac** 对视频帧、音频帧进行编码压缩，并通过 **FFmpeg** 进行解码播放。设备端及 PC 端的仓库地址如下。

设备端仓库: https://github.com/YUPANZHAO/AndroidCamera

PC端仓库: https://github.com/YUPANZHAO/MiniSurveillance



## 运行展示

<img src="https://img1.imgtp.com/2023/02/18/jhozcRYv.jpg" width="50%" style="float:left" /><img src="https://img1.imgtp.com/2023/02/18/JdzJ8TOF.png" width="50%" style="float:left" /><img src="https://img1.imgtp.com/2023/02/18/gvOHsx8l.png" width="50%" style="float:left" /><img src="https://img1.imgtp.com/2023/02/18/mY8sQDj2.png" width="50%" style="float:left" />
