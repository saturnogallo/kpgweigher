主控板Bootloader（Mega64）
1. 对于未装载bootloader的裸板，需使用编程器将bootloader烧写到bootloader区域。设置熔丝BOOTSZ=10（1KW bootloader区域），
同时设置熔丝BOOTRST位，设定芯片上电起动从BOOTLOADER区的起始地址处开始。
 
2. 通过主控板COM0口与PC相连。
3. 通讯设置 9600bps，8位数据，无校验，两位停止位，无flow控制。
4. 使用XP自带超级终端进行程序下载，使用Xmodem协议。
5. 只支持二进制文件下载，需要先使用hex2bin转换工具将编译后生成的hex文件转为*.bin文件再进行烧录。
6. 上电后运行bootloader，超级终端里提示3秒钟内按‘d'可更新应用程序，若3秒钟无’d'按下，系统开始切换到应用程序区执行。
 