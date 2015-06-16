/* CH374芯片 硬件抽象层 V1.0 */
/* 提供基本子程序,提供中断查询子程序 */

#include	"HAL.H"

/* 延时指定微秒时间,根据单片机主频调整,不精确 */
void	mDelayuS( UINT8 us )
{
	while ( us -- );  // 24MHz MCS51
}

/* 延时指定毫秒时间 */
void	mDelaymS( UINT8 ms )
{
	while ( ms -- ) {
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
	}
}

// 查询CH374中断(INT#低电平)
BOOL	Query374Interrupt( void )
{
#ifdef CH374_INT_WIRE
//	return( CH374_INT_WIRE ? FALSE : TRUE );  /* 如果连接了CH374的中断引脚则直接查询中断引脚 */
#else
	return( Read374Byte( REG_INTER_FLAG ) & BIT_IF_INTER_FLAG ? TRUE : FALSE );  /* 如果未连接CH374的中断引脚则查询中断标志寄存器 */
#endif
}

/* no use , because we don't use INT# PIN
 等待CH374中断(INT#低电平)，超时则返回ERR_USB_UNKNOWN
UINT8	Wait374Interrupt( void )
{
	UINT16	i;
	for ( i = 0; i < 10000; i ++ ) {  // 计数防止超时
		if ( Query374Interrupt( ) ) return( 0 );
	}
	return( ERR_USB_UNKNOWN );  // 不应该发生的情况
}
*/
