#ifndef _ZLG7289_H_
#define _ZLG7289_H_

//执行ZLG7289 纯指令
extern void ZLG7289_cmd(char cmd);
//执行ZLG7289 带数据指令
extern void ZLG7289_cmd_dat(char cmd, char dat);
//以下是ZLG7289 的用户指令集
//复位（清除）指令
#define ZLG7289_Reset() ZLG7289_cmd(0xA4)
//测试指令
#define ZLG7289_Test() ZLG7289_cmd(0xBF)
//左移指令
#define ZLG7289_SHL() ZLG7289_cmd(0xA0)
//右移指令
#define ZLG7289_SHR() ZLG7289_cmd(0xA1)
//循环左移指令
#define ZLG7289_ROL() ZLG7289_cmd(0xA2)
//循环右移指令
#define ZLG7289_ROR() ZLG7289_cmd(0xA3)
//下载数据
extern void ZLG7289_Download(unsigned char mod, char x, bit dp, char dat);
//闪烁控制
// x 的8 个位分别对应数码管的8 个位，0－闪烁，1－不闪烁
#define ZLG7289_Flash(x) ZLG7289_cmd_dat(0x88,(x))
//消隐控制
// x 的8 个位分别对应数码管的8 个位，0－消隐，1－显示
#define ZLG7289_Hide(x) ZLG7289_cmd_dat(0x98,(x))
//段点亮指令
//seg=0～63，8 只数码管被看成64 只独立的LED
//每只数码管中各段的点亮顺序按照“g，f，e，d，c，b，a，dp”进行
#define ZLG7289_SegOn(seg) ZLG7289_cmd_dat(0xE0,(seg))
//段关闭指令
//seg=0～63，8 只数码管被看成64 只独立的LED
//每只数码管中各段的熄灭顺序按照“g，f，e，d，c，b，a，dp”进行
#define ZLG7289_SegOff(seg) ZLG7289_cmd_dat(0xC0,(seg))
//执行ZLG7289 键盘命令
extern char ZLG7289_Key() reentrant;
//ZLG7289 初始化
extern void ZLG7289_Init(unsigned char t);
#endif //_ZLG7289_H_
