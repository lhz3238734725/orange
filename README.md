# orange
从零开始实现salar框架

## 开发时间表
| 时间 | 完成功能 |
| ---  | --- |
| 2025.2.8 ~ 2025.2.10| 日志系统初步完成 |
| 2025.2.11 | 配置系统添加基础配置项 |
| 2025.2.12 | 测试yaml-cpp的使用 |
| 2025.2.13 | yaml-cppj集成到配置系统 |

## 环境搭建
系统：ubutun2004  
依赖库安装：
``` shell
sudo apt install libboost1.67-dev libyaml-cpp-dev g++ -y
```

## 项目目录结构
``` shell
build 中间文件生成目录
bin   可执行文件生成目录
lib   库生成目录
cmake cmake函数目录
src   源码目录
tests 测试代码目录
```