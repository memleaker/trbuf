# Trbuf

通过Trbuf缓冲区，控制读出的数据。

- 支持读取指定长度的数据。
- 支持遇到指定字符停止。
- 支持遇到指定字符串停止。
- 支持读取结构体。
- 支持读取TLV数据格式。
- 多线程安全。
- RingBuffer可单独使用，且多线程安全。

## 编译

```sh
./auto.sh build
```

## 安装

- 将库安装到/usr/lib目录下
- 将头文件安装到/usr/include/trbuf下

```sh
./auto.sh install
```

## 卸载

```sh
./auto.sh uninstall
```

## 测试

```
cd tests
make
./RingBufferTest
./TrbufTest
```

## 使用

- 要使用Trbuf只需: #include<Trbuf.h>

- 要单独使用RingBuffer只需: #include <RingBuffer.h>

- 编译时指定编译选项: -I /usr/include/trbuf -L /usr/lib -ltrbuf