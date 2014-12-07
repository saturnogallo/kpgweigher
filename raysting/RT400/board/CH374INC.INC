; Define for CH373 & CH374          */
; Website:  http://winchiphead.com  */
; Email:    tech@winchiphead.com    */
; Author:   W.ch 2005.10            */
; V1.3 for CH373/CH374              */

; ********************************************************************************************************************* */
; USB协议层定义 */

; USB的包标识PID, 主机方式可能用到 */
DEF_USB_PID_NULL	EQU     00H							;保留PID, 未定义
DEF_USB_PID_SOF		EQU     05H
DEF_USB_PID_SETUP	EQU     0DH
DEF_USB_PID_IN		EQU     09H
DEF_USB_PID_OUT		EQU     01H
DEF_USB_PID_ACK		EQU     02H
DEF_USB_PID_NAK		EQU     0AH
DEF_USB_PID_STALL	EQU     0EH
DEF_USB_PID_DATA0	EQU     03H
DEF_USB_PID_DATA1	EQU     0BH
DEF_USB_PID_PRE		EQU     0CH

; ********************************************************************************************************************* */
; 芯片定义 */

; 寄存器及缓冲区 */

RAM_ENDP0_SIZE		EQU     008H		; 端点0的缓冲区长度 */
RAM_ENDP0_TRAN		EQU     020H		; 端点0发送缓冲区的起始地址 */
RAM_ENDP0_RECV		EQU     028H		; 端点0接收缓冲区的起始地址 */
RAM_ENDP1_SIZE		EQU     008H		; 端点1的缓冲区长度 */
RAM_ENDP1_TRAN		EQU     030H		; 端点1发送缓冲区的起始地址 */
RAM_ENDP1_RECV		EQU     038H		; 端点1接收缓冲区的起始地址 */
RAM_ENDP2_SIZE		EQU     040H		; 端点2的缓冲区长度 */
RAM_ENDP2_TRAN		EQU     040H		; 端点2发送缓冲区的起始地址 */
RAM_ENDP2_RECV		EQU     0C0H		; 端点2接收缓冲区的起始地址 */
RAM_ENDP2_EXCH		EQU     080H		; 端点2备用缓冲区的起始地址 */
RAM_HOST_TRAN		EQU     040H		; 主机发送缓冲区的起始地址 */
RAM_HOST_RECV		EQU     0C0H		; 主机接收缓冲区的起始地址 */
RAM_HOST_EXCH		EQU     080H		; 主机备用缓冲区的起始地址 */

REG_HUB_SETUP		EQU		002H		; 仅USB主机方式: ROOT-HUB配置 */
REG_HUB_CTRL		EQU		003H		; 仅USB主机方式: ROOT-HUB控制 */
REG_SYS_INFO		EQU     004H		; 系统信息, 只读 */
REG_SYS_CTRL		EQU     005H		; 系统控制, 不受软件复位影响 */
REG_USB_SETUP		EQU     006H		; USB配置 */
REG_INTER_EN		EQU     007H		; 中断使能 */
REG_USB_ADDR		EQU     008H		; USB设备地址 */
REG_INTER_FLAG		EQU     009H		; 中断标志, 只读, 位写1清0 */
REG_USB_STATUS		EQU     00AH		; USB状态, 只读 */
REG_USB_LENGTH		EQU     00BH		; USB长度, 读为当前USB接收长度, 设备方式下写为USB端点2, 主机方式下写为USB主机发送长度 */
REG_USB_ENDP0		EQU     00CH		; 仅USB设备方式: USB端点0控制 */
REG_USB_ENDP1		EQU     00DH		; 仅USB设备方式: USB端点1控制 */
REG_USB_H_PID		EQU     00DH		; 仅USB主机方式: USB主机令牌 */
REG_USB_ENDP2		EQU     00EH		; 仅USB设备方式: USB端点2控制 */
REG_USB_H_CTRL		EQU     00EH		; 仅USB主机方式: USB主机控制 */

; 寄存器的位及常用宏定义 */

BIT_HUB0_EN			EQU     001H		; HUB0端口的USB传输使能: 0-禁止, 1-允许 */
BIT_HUB0_RESET		EQU     002H		; HUB0端口的USB总线复位控制: 0-不复位, 1-复位 */
BIT_HUB0_POLAR		EQU     004H		; HUB0端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
BIT_HUB0_ATTACH		EQU     008H		; HUB0端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */
BIT_HUB1_DX_IN		EQU     010H		; HUB1全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
BIT_HUB2_DX_IN		EQU     020H		; HUB2全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
BIT_HUB_PRE_PID		EQU     040H		; 低速前置包PRE PID输出控制: 0-禁止, 1-允许(外部设备是USB-HUB) */
BIT_HUB_DISABLE		EQU     080H		; 禁止ROOT-HUB根集线器功能: 0-允许(仅USB主机方式), 1-禁止(默认) */

BIT_HUB1_EN			EQU     001H		; HUB1端口的USB传输使能: 0-禁止, 1-允许 */
BIT_HUB1_RESET		EQU     002H		; HUB1端口的USB总线复位控制: 0-不复位, 1-复位 */
BIT_HUB1_POLAR		EQU     004H		; HUB1端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
BIT_HUB1_ATTACH		EQU     008H		; HUB1端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */
BIT_HUB2_EN			EQU     010H		; HUB2端口的USB传输使能: 0-禁止, 1-允许 */
BIT_HUB2_RESET		EQU     020H		; HUB2端口的USB总线复位控制: 0-不复位, 1-复位 */
BIT_HUB2_POLAR		EQU     040H		; HUB2端口的信号极性控制: 0-正极性/全速, 1-负极性/低速 */
BIT_HUB2_ATTACH		EQU     080H		; HUB2端口的USB设备连接状态(只读): 0-尚未连接/断开/拔出, 1-已经连接/插入 */

BIT_INFO_HW_ID		EQU     003H		; 硬件识别位: 总是常量01, 否则说明读操作或硬件连接有误 */
BIT_INFO_USB_DM		EQU     004H		; USB总线UD-引脚的逻辑电平状态: 0-低电平, 1-高电平 */
BIT_INFO_USB_DP		EQU     008H		; USB总线UD+引脚的逻辑电平状态: 0-低电平, 1-高电平 */
BIT_INFO_CLK_8KHZ	EQU		010H		; 硬件8KHz时钟位 */
BIT_INFO_SOF_PRES	EQU     020H		; 硬件1mS定时周期状态,对于USB主机, 1说明将要产生SOF */
BIT_INFO_WAKE_UP	EQU     040H		; 芯片唤醒状态: 0-正在睡眠或唤醒过程中, 1-已唤醒 */
BIT_INFO_POWER_RST	EQU     080H		; 硬件上电复位完成状态: 0-正在复位, 1-复位完成 */

BIT_CTRL_OSCIL_OFF	EQU     001H		; 时钟振荡器控制: 0-允许振荡, 1-停止振荡 */
BIT_CTRL_CLK_12MHZ	EQU		002H		; 输入时钟频率选择: 0-24MHz, 1-12MHz */
BIT_CTRL_USB_POWER	EQU     004H		; V3引脚的USB电源调节器控制: 0-开启, 1-禁用 */
BIT_CTRL_RESET_NOW	EQU     008H		; 芯片软件复位控制: 0-不复位, 1-复位 */
BIT_CTRL_WATCH_DOG	EQU     010H		; RST引脚和RST#引脚的看门狗复位使能: 0-禁用, 1-启用 */
BIT_CTRL_INT_PULSE	EQU     020H		; INT#引脚的中断输出方式: 0-低电平中断, 1-低电平脉冲中断 */
BIT_CTRL_OE_POLAR	EQU     040H		; UEN引脚的USB输出使能极性: 0-高电平使能, 1-低电平使能 */

BIT_SETP_TRANS_EN	EQU     001H		; 仅USB设备方式: USB设备传输使能: 0-禁止, 1-允许 */
BIT_SETP_PULLUP_EN	EQU     002H		; 仅USB设备方式: USB上拉电阻控制: 0-禁用上拉电阻, 1-启用上拉电阻 */
BIT_SETP_BUS_CTRL	EQU     003H		; 仅USB主机方式: USB总线状态控制: 00-正常/空闲, 01-D+低D-低(总线复位), 10-禁用, 11-D+低D-高(总线恢复) */
;#define	M_SET_USB_BUS_FREE( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x00 )		; 仅USB主机方式: USB总线空闲 */
;#define	M_SET_USB_BUS_RESET( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x01 )		; 仅USB主机方式: USB总线状态控制/D+低D-低(总线复位) */
;#define	M_SET_USB_BUS_RESUME( old )		( (old) & ~ BIT_SETP_BUS_CTRL | 0x03 )		; 仅USB主机方式: USB总线状态控制/D+低D-高(总线恢复) */
BIT_SETP_RAM_MODE	EQU     00CH		; 备用缓冲区应用方式: 00-禁用备用缓冲区, 01-连接接收缓冲区以连续接收128字节, 10-用于发送第二缓冲区, 11-用于接收第二缓冲区 */
;#define	M_SET_RAM_MODE_OFF( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x00 )		; 备用缓冲区方式/禁用备用缓冲区 */
;#define	M_SET_RAM_MODE_128( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x04 )		; 备用缓冲区方式/连接接收缓冲区以连续接收128字节 */
;#define	M_SET_RAM_MODE_2TX( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x08 )		; 备用缓冲区方式/用于发送的第二缓冲区,支持连续发送 */
;#define	M_SET_RAM_MODE_2RX( old )		( (old) & ~ BIT_SETP_RAM_MODE | 0x0C )		; 备用缓冲区方式/用于接收的第二缓冲区,支持连续接收 */
BIT_SETP_LOW_SPEED	EQU     020H		; USB总线传输速度: 0-12Mbps, 1-1.5Mbps */
BIT_SETP_USB_SPEED	EQU     030H		; USB总线速率: 00-全速模式/正极性12Mbps, 11-低速模式/负极性1.5Mbps */
BIT_SETP_LED_ACT	EQU     040H		; 仅USB设备方式: ACT#引脚低电平的激活事件: 0-收发传输过程, 1-USB主机活动 */
BIT_SETP_AUTO_SOF	EQU     040H		; 仅USB主机方式: 自动产生SOF使能: 0-禁止, 1-允许 */
BIT_SETP_HOST_MODE	EQU     080H		; USB主从方式选择: 0-设备方式, 1-主机方式 */

BIT_IE_TRANSFER		EQU     001H		; USB传输完成中断使能, 1有效 */
BIT_IE_BUS_RESET	EQU     002H		; 仅USB设备方式: USB总线复位中断使能, 1有效 */
BIT_IE_DEV_DETECT	EQU     002H		; 仅USB主机方式: USB设备检测中断使能, 1有效 */
BIT_IE_USB_SUSPEND	EQU     004H		; USB总线挂起中断使能, 1有效 */
BIT_IE_USB_RESUME	EQU     008H		; USB总线恢复/唤醒中断使能, 1有效, 0-使能芯片唤醒完成中断, 1-使能USB总线恢复中断 */
BIT_IE_CLK_OUT_DIV	EQU     0F0H		; 可编程时钟的分频除数: 输出频率为48MHz/(该值+1), 例如: 0000-48MHz, 0001-24MHz, 0010-16MHz, 1111-3MHz */
;#define	M_SET_CLK_DIV( old, div )		( (old) & ~ BIT_IE_CLK_OUT_DIV | (div) << 4 )	; 设置时钟输出分频除数 */

BIT_ADDR_USB_DEV	EQU     07FH		; 在设备方式下为自身作为USB设备的地址, 在主机方式下为当前被操作的USB设备地址 */

BIT_IF_INTER_FLAG	EQU     00FH		; 所有的USB中断标志 */
BIT_IF_TRANSFER		EQU     001H		; USB传输完成中断标志, 1有效, 向该位写1清标志, 该位在每次USB传输完成后自动置1 */
BIT_IF_BUS_RESET	EQU     002H		; 仅USB设备方式: USB总线复位中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线复位时自动置1 */
BIT_IF_DEV_DETECT	EQU     002H		; 仅USB主机方式: USB设备插拔检测中断标志, 1有效, 向该位写1清标志, 该位在检测到USB设备插拔后自动置1 */
BIT_IF_USB_SUSPEND	EQU     004H		; USB总线挂起中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线挂起时自动置1 */
BIT_IF_WAKE_UP		EQU     008H		; 芯片唤醒完成中断标志, 1有效, 向该位写1清标志, 该位在芯片唤醒完成后自动置1 */
BIT_IF_USB_RESUME	EQU     008H		; USB总线恢复/唤醒中断标志, 1有效, 向该位写1清标志, 该位在检测到USB总线恢复时自动置1 */
BIT_IF_USB_PAUSE	EQU     010H		; USB传输暂停标志, 1有效, 向该位写1清标志, 该位在每次USB传输完成后自动置1 */
BIT_IF_DEV_ATTACH	EQU     020H		; USB设备连接状态: 0-尚未连接/断开/拔出, 1-至少有一个USB设备已经连接/插入 */
BIT_IF_USB_OE		EQU     040H		; UEN引脚的USB输出使能状态: 0-UEN引脚为低电平, 1-UEN引脚为高电平 */
BIT_IF_USB_DX_IN	EQU     080H		; HUB0全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */
BIT_HUB0_DX_IN		EQU		080H		; HUB0全速正极性时UD+引脚/低速负极性时UD-引脚的采样状态: 0-低电平,速度失配, 1-高电平,速度匹配 */

BIT_STAT_THIS_ENDP	EQU     003H		; 仅USB设备方式: USB传输的目的端点号: 00-端点0, 01-端点1, 10-端点2, 11-保留 */
BIT_STAT_THIS_PID	EQU     00CH		; 仅USB设备方式: USB传输的事务/令牌PID: 00-OUT事务, 01-保留, 10-IN事务, 11-SETUP事务 */
BIT_STAT_PID_ENDP	EQU     00FH		; 仅USB设备方式: USB传输的事务和端点号,参考后面的USB_INT_EP*定义 */
BIT_STAT_DEV_RESP	EQU     00FH		; 仅USB主机方式: USB设备的应答PID: XX00=错误或超时,其它值-同PID定义,参考后面的USB_INT_RET_*定义 */
;M_IS_HOST_TIMEOUT( status )		( ( (status) & 0x03 ) == 0 )		; 检查USB主机状态是否为应答超时/出错 */
;M_IS_HOST_IN_DATA( status )		( ( (status) & BIT_STAT_DEV_RESP & ~ ( DEF_USB_PID_DATA0 ^ DEF_USB_PID_DATA1 ) ) == ( DEF_USB_PID_DATA0 & DEF_USB_PID_DATA1 ) )	; 检查是否返回DATA0或者DATA1 */
BIT_STAT_TOG_MATCH	EQU     010H		; 指示当前的传输是否成功/当前接收的数据包是否同步: 0-不同步, 1-同步 */
BIT_STAT_BUS_RESET	EQU     020H		; 当前USB总线复位状态: 0-没有复位, 1-正在复位 */
BIT_STAT_SUSPEND	EQU     040H		; 当前USB总线挂起状态: 0-总线有活动, 1-总线挂起 */
BIT_STAT_SIE_FREE	EQU     080H		; 当前USB接口引擎SIE的状态: 0=忙/正在传输, 1=空闲/等待 */

BIT_EP0_TRAN_RESP	EQU     00FH		; 仅USB设备方式: 端点0发送响应: 0000~1000-应答数据长度0~8, 1110-应答NAK, 1111-应答STALL,其它值-禁用 */
;#define	M_SET_EP0_TRAN_ACK( old, len )	( (old) & ~ BIT_EP0_TRAN_RESP | (len) & 0x0F )	; 仅USB设备方式: 端点0发送响应/应答ACK */
;#define	M_SET_EP0_TRAN_NAK( old )		( (old) & ~ BIT_EP0_TRAN_RESP | 0x0E )		; 仅USB设备方式: 端点0发送响应/应答NAK */
;#define	M_SET_EP0_TRAN_STA( old )		( (old) & ~ BIT_EP0_TRAN_RESP | 0x0F )		; 仅USB设备方式: 端点0发送响应/应答STALL */
BIT_EP0_RECV_RESP	EQU     030H		; 仅USB设备方式: 端点0接收响应: 00-应答ACK, 01-禁用, 10-应答NAK, 11-应答STALL */
;#define	M_SET_EP0_RECV_ACK( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x00 )		; 仅USB设备方式: 端点0接收响应/应答ACK */
;#define	M_SET_EP0_RECV_NAK( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x20 )		; 仅USB设备方式: 端点0接收响应/应答NAK */
;#define	M_SET_EP0_RECV_STA( old )		( (old) & ~ BIT_EP0_RECV_RESP | 0x30 )		; 仅USB设备方式: 端点0接收响应/应答STALL */
BIT_EP0_TRAN_TOG	EQU     040H		; 仅USB设备方式: 端点0发送同步标志: 0-DATA0, 1-DATA1 */
BIT_EP0_RECV_TOG	EQU     080H		; 仅USB设备方式: 端点0接收同步标志: 0-DATA0, 1-DATA1 */

BIT_EP1_TRAN_RESP	EQU     00FH		; 仅USB设备方式: 端点1发送响应: 0000~1000-应答数据长度0~8, 1110-应答NAK, 1111-应答STALL,其它值-禁用 */
;#define	M_SET_EP1_TRAN_ACK( old, len )	( (old) & ~ BIT_EP1_TRAN_RESP | (len) & 0x0F )	; 仅USB设备方式: 端点1发送响应/应答ACK */
;#define	M_SET_EP1_TRAN_NAK( old )		( (old) & ~ BIT_EP1_TRAN_RESP | 0x0E )		; 仅USB设备方式: 端点1发送响应/应答NAK */
;#define	M_SET_EP1_TRAN_STA( old )		( (old) & ~ BIT_EP1_TRAN_RESP | 0x0F )		; 仅USB设备方式: 端点1发送响应/应答STALL */
BIT_EP1_RECV_RESP	EQU     030H		; 仅USB设备方式: 端点1接收响应: 00-应答ACK, 01-禁用, 10-应答NAK, 11-应答STALL */
;#define	M_SET_EP1_RECV_ACK( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x00 )		; 仅USB设备方式: 端点1接收响应/应答ACK */
;#define	M_SET_EP1_RECV_NAK( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x20 )		; 仅USB设备方式: 端点1接收响应/应答NAK */
;#define	M_SET_EP1_RECV_STA( old )		( (old) & ~ BIT_EP1_RECV_RESP | 0x30 )		; 仅USB设备方式: 端点1接收响应/应答STALL */
BIT_EP1_TRAN_TOG	EQU     040H		; 仅USB设备方式: 端点1发送同步标志: 0-DATA0, 1-DATA1 */
BIT_EP1_RECV_TOG	EQU     080H		; 仅USB设备方式: 端点1接收同步标志: 0-DATA0, 1-DATA1 */

BIT_HOST_PID_ENDP	EQU     00FH		; 仅USB主机方式: 目的端点号: 0000~1111-端点号0~15 */
BIT_HOST_PID_TOKEN	EQU     0F0H		; 仅USB主机方式: 指定事务/令牌PID: 1101-SETUP事务, 0001-OUT事务, 1001-IN事务, 0101-SOF包,其它值-禁用 */
;M_MK_HOST_PID_ENDP( pid, endp )	( (pid) << 4 | (endp) & BIT_HOST_PID_ENDP )		; 用事务/令牌PID和目的端点号生成USB主机令牌数据 */

BIT_EP2_TRAN_RESP	EQU     003H		; 仅USB设备方式: 端点2发送响应: 00-应答数据, 01-同步/等时传输, 10-应答NAK, 11-应答STALL */
;#define	M_SET_EP2_TRAN_ACK( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x00 )		; 仅USB设备方式: 端点2发送响应/应答ACK */
;#define	M_SET_EP2_TRAN_ISO( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x01 )		; 仅USB设备方式: 端点2发送响应/同步/等时传输/无需应答 */
;#define	M_SET_EP2_TRAN_NAK( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x02 )		; 仅USB设备方式: 端点2发送响应/应答NAK */
;#define	M_SET_EP2_TRAN_STA( old )		( (old) & ~ BIT_EP2_TRAN_RESP | 0x03 )		; 仅USB设备方式: 端点2发送响应/应答STALL */
BIT_EP2_RECV_RESP	EQU     030H		; 仅USB设备方式: 端点2接收响应: 00-应答ACK, 01-同步/等时传输, 10-应答NAK, 11-应答STALL */
;#define	M_SET_EP2_RECV_ACK( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x00 )		; 仅USB设备方式: 端点2接收响应/应答ACK */
;#define	M_SET_EP2_RECV_ISO( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x10 )		; 仅USB设备方式: 端点2接收响应/同步/等时传输/不作应答 */
;#define	M_SET_EP2_RECV_NAK( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x20 )		; 仅USB设备方式: 端点2接收响应/应答NAK */
;#define	M_SET_EP2_RECV_STA( old )		( (old) & ~ BIT_EP2_RECV_RESP | 0x30 )		; 仅USB设备方式: 端点2接收响应/应答STALL */
BIT_EP2_TRAN_TOG	EQU     040H		; 仅USB设备方式: 端点2发送同步标志: 0-DATA0, 1-DATA1 */
BIT_EP2_RECV_TOG	EQU     080H		; 仅USB设备方式: 端点2接收同步标志: 0-DATA0, 1-DATA1 */

BIT_HOST_TRAN_ISO	EQU     001H		; 仅USB主机方式: 主机发送的传输类型: 0-控制/批量/中断传输, 1-同步/等时传输 */
BIT_HOST_START		EQU     008H		; 仅USB主机方式: 主机传输启动控制: 0-暂停, 1-启动传输,完成后自动清0 */
BIT_HOST_RECV_ISO	EQU     010H		; 仅USB主机方式: 主机接收的传输类型: 0-控制/批量/中断传输, 1-同步/等时传输 */
BIT_HOST_TRAN_TOG	EQU     040H		; 仅USB主机方式: 主机发送同步标志: 0-DATA0, 1-DATA1 */
BIT_HOST_RECV_TOG	EQU     080H		; 仅USB主机方式: 主机接收同步标志: 0-DATA0, 1-DATA1 */

; 状态及命令等常量 */

CMD_SPI_374READ		EQU     0C0H		; SPI读操作命令 */
CMD_SPI_374WRITE	EQU     080H		; SPI写操作命令 */

; ********************************************************************************************************************* */
; USB状态 */

; 以下状态代码用于USB设备方式 */
;   位4指示当前接收的数据包是否同步, 0=不同步, 1-同步 */
;   位3-位2指示当前事务, 00=OUT, 10=IN, 11=SETUP */
;   位1-位0指示当前端点, 00=端点0, 01=端点1, 10=端点2 */
USB_INT_EP0_SETUP	EQU     0CH							;USB端点0的SETUP
USB_INT_EP0_OUT		EQU     00H							;USB端点0的OUT
USB_INT_EP0_IN		EQU     08H							;USB端点0的IN
USB_INT_EP1_OUT		EQU     01H							;USB端点1的OUT
USB_INT_EP1_IN		EQU     09H							;USB端点1的IN
USB_INT_EP2_OUT		EQU     02H							;USB端点2的OUT
USB_INT_EP2_IN		EQU     0AH							;USB端点2的IN

; 以下状态代码用于USB主机方式 */
;   位4指示当前接收的数据包是否同步, 0=不同步, 1-同步 */
;   位3-位0指示USB设备的应答: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=应答错误或者超时无应答 */
USB_INT_RET_ACK		EQU     DEF_USB_PID_ACK		; 错误:对于OUT/SETUP事务返回ACK */
USB_INT_RET_NAK		EQU     DEF_USB_PID_NAK		; 错误:返回NAK */
USB_INT_RET_STALL	EQU     DEF_USB_PID_STALL	; 错误:返回STALL */
USB_INT_RET_DATA0	EQU     DEF_USB_PID_DATA0	; 错误:对于IN事务返回DATA0 */
USB_INT_RET_DATA1	EQU     DEF_USB_PID_DATA1	; 错误:对于IN事务返回DATA1 */
USB_INT_RET_TOUT	EQU     000H		; 错误:应答错误或者超时无应答 */
USB_INT_RET_TOUT1	EQU     004H		; 错误:应答错误或者超时无应答 */
USB_INT_RET_TOUT2	EQU     008H		; 错误:应答错误或者超时无应答 */
USB_INT_RET_TOUT3	EQU     00CH		; 错误:应答错误或者超时无应答 */


; 以下状态代码1XH用于USB主机方式的操作状态代码, 可以自行定义任意值, 下面的定义仅为了兼容CH375 */
USB_INT_SUCCESS		EQU     14H							;USB事务或者传输操作成功
USB_INT_CONNECT		EQU     15H							;检测到USB设备连接事件
USB_INT_DISCONNECT	EQU     16H							;检测到USB设备断开事件
USB_INT_BUF_OVER	EQU     17H							;USB控制传输的数据太多, 缓冲区溢出
USB_INT_DISK_READ	EQU     1DH							;USB存储器读数据块, 请求数据读出
USB_INT_DISK_WRITE	EQU     1EH							;USB存储器写数据块, 请求数据写入
USB_INT_DISK_ERR	EQU     1FH							;USB存储器操作失败

; ********************************************************************************************************************* */
; 常用USB结构和相关常量 */

; USB请求类型 */
DEF_USB_REQ_READ	EQU     80H							;控制读操作
DEF_USB_REQ_WRITE	EQU     00H							;控制写操作
DEF_USB_REQ_TYPE	EQU     60H							;控制请求类型
DEF_USB_REQ_STAND	EQU     00H							;标准请求
DEF_USB_REQ_CLASS	EQU     20H							;设备类请求
DEF_USB_REQ_VENDOR	EQU     40H							;厂商请求
DEF_USB_REQ_RESERVE	EQU     60H							;保留请求

; USB标准设备请求, RequestType的位6位5=00(Standard) */
DEF_USB_CLR_FEATURE	EQU    01H
DEF_USB_SET_FEATURE	EQU    03H
DEF_USB_GET_STATUS	EQU    00H
DEF_USB_SET_ADDRESS	EQU    05H
DEF_USB_GET_DESCR	EQU    06H
DEF_USB_SET_DESCR	EQU    07H
DEF_USB_GET_CONFIG	EQU    08H
DEF_USB_SET_CONFIG	EQU    09H
DEF_USB_GET_INTERF	EQU    0AH
DEF_USB_SET_INTERF	EQU    0BH
DEF_USB_SYNC_FRAME	EQU    0CH

; 描述符类型 */
USB_DEVICE_DESCR_TYPE		EQU     001H
USB_CONFIG_DESCR_TYPE		EQU     002H
USB_STRING_DESCR_TYPE		EQU     003H
USB_INTERF_DESCR_TYPE		EQU     004H
USB_ENDP_DESCR_TYPE			EQU     005H
USB_POWER_DESCR_TYPE		EQU     006H
USB_HID_DESCR_TYPE			EQU     021H
USB_REPORT_DESCR_TYPE		EQU     022H
USB_PHYSIC_DESCR_TYPE		EQU     023H

; 端点类型和属性 */
USB_ENDP_DIR_MASK			EQU     080H
USB_ENDP_TYPE_MASK			EQU     003H
USB_ENDP_TYPE_CTRL			EQU     000H
USB_ENDP_TYPE_ISOCH			EQU     001H
USB_ENDP_TYPE_BULK			EQU     002H
USB_ENDP_TYPE_INTER			EQU     003H

; 设备类别 */
USB_DEV_CLASS_RESERVED		EQU     000H
USB_DEV_CLASS_AUDIO			EQU     001H
USB_DEV_CLASS_COMMUNIC		EQU     002H
USB_DEV_CLASS_HUMAN_IF		EQU     003H
USB_DEV_CLASS_MONITOR		EQU     004H
USB_DEV_CLASS_PHYSIC_IF		EQU     005H
USB_DEV_CLASS_POWER			EQU     006H
USB_DEV_CLASS_PRINTER		EQU     007H
USB_DEV_CLASS_STORAGE		EQU     008H
USB_DEV_CLASS_HUB			EQU     009H
USB_DEV_CLASS_VEN_SPEC		EQU     0FFH

DEFAULT_ENDP0_SIZE			EQU		08H				; 默认的端点0最大包长度 */
