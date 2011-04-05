
;CodeVisionAVR C Compiler V1.25.3 Standard
;(C) Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega64
;Program type           : Application
;Clock frequency        : 14.745600 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : float, width, precision
;(s)scanf features      : long, width
;External SRAM size     : 0
;Data Stack size        : 1024 byte(s)
;Heap size              : 0 byte(s)
;Promote char to int    : No
;char is unsigned       : Yes
;8 bit enums            : Yes
;Word align FLASH struct: No
;Enhanced core instructions    : On
;Smart register allocation : On
;Automatic register allocation : On

	#pragma AVRPART ADMIN PART_NAME ATmega64
	#pragma AVRPART MEMORY PROG_FLASH 65536
	#pragma AVRPART MEMORY EEPROM 2048
	#pragma AVRPART MEMORY INT_SRAM SIZE 4096
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU USR=0xB
	.EQU UDR=0xC
	.EQU SPSR=0xE
	.EQU SPDR=0xF
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU EECR=0x1C
	.EQU EEDR=0x1D
	.EQU EEARL=0x1E
	.EQU EEARH=0x1F
	.EQU WDTCR=0x21
	.EQU MCUCR=0x35
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU XMCRA=0x6D
	.EQU XMCRB=0x6C

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __se_bit=0x20
	.EQU __sm_mask=0x1C
	.EQU __sm_adc_noise_red=0x08
	.EQU __sm_powerdown=0x10
	.EQU __sm_powersave=0x18
	.EQU __sm_standby=0x14
	.EQU __sm_ext_standby=0x1C

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM
	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM
	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM
	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+@1
	ANDI R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ANDI R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+@1
	ORI  R30,LOW(@2)
	STS  @0+@1,R30
	LDS  R30,@0+@1+1
	ORI  R30,HIGH(@2)
	STS  @0+@1+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __CLRD1S
	LDI  R30,0
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+@1)
	LDI  R31,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	LDI  R22,BYTE3(2*@0+@1)
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+@1)
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+@2)
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+@3)
	LDI  R@1,HIGH(@2+@3)
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+@3)
	LDI  R@1,HIGH(@2*2+@3)
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+@1
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+@1
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	LDS  R22,@0+@1+2
	LDS  R23,@0+@1+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+@2
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+@3
	LDS  R@1,@2+@3+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+@1
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+@1
	LDS  R27,@0+@1+1
	LDS  R24,@0+@1+2
	LDS  R25,@0+@1+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+@1,R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+@1,R30
	STS  @0+@1+1,R31
	STS  @0+@1+2,R22
	STS  @0+@1+3,R23
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+@1,R0
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+@1,R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+@1,R@2
	STS  @0+@1+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+@1
	LDS  R31,@0+@1+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+@1)
	LDI  R31,HIGH(2*@0+@1)
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	LDI  R26,LOW(@0+@1)
	LDI  R27,HIGH(@0+@1)
	CALL __EEPROMRDW
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R26,SPL
	IN   R27,SPH
	ADIW R26,@0+1
	LD   R30,X+
	LD   R31,X
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __CLRD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	CLR  R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z+,R0
	ST   Z,R0
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R@1
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

	.CSEG
	.ORG 0

	.INCLUDE "main.vec"
	.INCLUDE "main.inc"

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30
	STS  XMCRB,R30

;DISABLE WATCHDOG
	LDI  R31,0x18
	OUT  WDTCR,R31
	OUT  WDTCR,R30

;CLEAR R2-R14
	LDI  R24,13
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(0x1000)
	LDI  R25,HIGH(0x1000)
	LDI  R26,LOW(0x100)
	LDI  R27,HIGH(0x100)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;STACK POINTER INITIALIZATION
	LDI  R30,LOW(0x10FF)
	OUT  SPL,R30
	LDI  R30,HIGH(0x10FF)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(0x500)
	LDI  R29,HIGH(0x500)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x500
;       1 /*****************************************************
;       2 This program was produced by the
;       3 CodeWizardAVR V1.24.6 Professional
;       4 Automatic Program Generator
;       5 ?Copyright 1998-2005 Pavel Haiduc, HP InfoTech s.r.l.
;       6 http://www.hpinfotech.com
;       7 e-mail:office@hpinfotech.com
;       8 
;       9 Project :
;      10 Version :
;      11 Date    : 2007-1-15
;      12 Author  : F4CG
;      13 Company : F4CG
;      14 Comments:
;      15 
;      16 
;      17 Chip type           : ATmega64
;      18 Program type        : Application
;      19 Clock frequency     : 14.745600 MHz
;      20 Memory model        : Small
;      21 External SRAM size  : 0
;      22 Data Stack size     : 1024
;      23 *****************************************************/
;      24 
;      25 #include <mega64.h>
;      26 #include "uart.h"
;      27 #include "init.h"
;      28 #include "16c554.h"
;      29 #include "global.h"
;      30 #include "utili.h"
;      31 #include "window.h"
;      32 #include "lcd.h"
;      33 #include "key.h"
;      34 #include "wendu.h"
;      35 #include "scanner.h"
;      36 #include "uart.h"
;      37 #include "stdlib.h"
;      38 extern void pgmain_handler(uchar);	//running menu
;      39 extern void pgrconfig_handler(uchar);	//main menu of R config
;      40 extern void pgtconfig_handler(uchar);	//main menu of T config
;      41 extern void pgchset_handler(uchar);	//menu of channel probe setup
;      42 extern void pgprblist_handler(uchar);	//list of probe selection
;      43 extern void pgprbtypelist_handler(uchar); //select probe type
;      44 extern void pgprbset_handler(uchar);	//menu of probe setup
;      45 extern void pgprbconfig_handler(uchar);  //config of probe
;      46 extern void pgboottype_handler(uchar msg) ; //boot up config
;      47 extern void com1_putstr(char *s);
;      48 extern unsigned int strlen(char *s);
;      49 RUNDATA rundata;
_rundata:
	.BYTE 0xC8
;      50 SYSDATA eeprom sysdata = {

	.ESEG
_sysdata:
;      51 	0,0,100,3,
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x42C8
	.DW  0x3
;      52 	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      53 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      54 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	.DB  0x0
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
;      55 	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      56 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      57 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
	.DB  0x0
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
;      58 	1,1};
	.DB  0x1
	.DB  0x1
;      59 PRBDATA	eeprom tprbdata = {
_tprbdata:
;      60        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      61 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      62 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      63 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      64 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      65 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      66 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      67 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      68 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      69 	{"11","","","","","","","",
;      70 	"","","","","","","","",
;      71 	"","","","","","","",""},
	.DB  0x31
	.DB  0x31
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
;      72        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      73 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      74 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
	.DB  0x3
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
;      75 };
;      76 PRBDATA	eeprom rprbdata = {
_rprbdata:
;      77        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      78 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      79 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      80 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      81 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      82 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      83 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      84 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      85 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      86 	{"11","","","","","","","",
;      87 	"","","","","","","","",
;      88 	"","","","","","","",""},
	.DB  0x31
	.DB  0x31
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
	.DB  0x0
;      89        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      90 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      91 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
	.DB  0x3
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
	.DB  0xFF
;      92 };
;      93 
;      94 char  strbuf[50];        //buffer for menu display

	.DSEG
_strbuf:
	.BYTE 0x32
;      95 uchar key;
;      96 
;      97 
;      98 
;      99 LABEL flash bootup = {LBL_HZ16,30,30,7,"正在启动..."};

	.CSEG
;     100 LABEL flash modify = {LBL_HZ16,30,30,8,"正在更新数据..."};
;     101 
;     102 LABEL flash lbldbg = {LBL_HZ6X8,10,30,8,strbuf};
;     103 
;     104 uchar nextwin = 0;
;     105 
;     106 int   curr_ch = 1;	//index of current channel in menu window

	.DSEG
;     107 int   curr_prb = 1;	//index of current probe selection in menu window
;     108 
;     109 int ch_to_search = 0;
;     110 
;     111 long dlg_cnt = 0;        //delay count time display
_dlg_cnt:
	.BYTE 0x4
;     112 uchar phase = 0;       //state machine index
;     113 double valuep;
_valuep:
	.BYTE 0x4
;     114 
;     115 /**************************************************************************************/
;     116 //                      Timer 1 overflow interrupt service routine
;     117 /**************************************************************************************/
;     118 interrupt [TIM1_OVF] void timer1_ovf_isr(void)
;     119 {

	.CSEG
_timer1_ovf_isr:
;     120 // Place your flash here
;     121 }
	RETI
;     122 
;     123 void InitLED()
;     124 {
_InitLED:
;     125 /*
;     126     PORTB.7=1;
;     127     PORTB.6=1;
;     128     PORTB.5=1;
;     129     PORTB.4=1;
;     130     PORTD.7=1;
;     131     PORTD.6=0;
;     132 */
;     133 }
	RET
;     134 u8 ToByte(char l, char h)
;     135 {
;     136 	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
;	l -> Y+1
;	h -> Y+0
;     137 	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
;     138 	return (h<<4)|l;
;     139 }
;     140 
;     141 /**************************************************************************************/
;     142 //                              Global Variable
;     143 /**************************************************************************************/
;     144 
;     145 /**************************************************************************************/
;     146 //                               Watchdog
;     147 // Enable watchdog timer.
;     148 // Timeout limit WDTCR[2:0]:
;     149 // 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
;     150 // 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
;     151 /**************************************************************************************/
;     152 #define WDCE 4
;     153 #define WDE  3
;     154 
;     155 
;     156 
;     157 void Turn_on_Watchdog()
;     158 {
;     159   #asm("cli")
;     160   #asm("wdr")                    // reset watchdog timer to avoid timeout reset
;     161   WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
;     162   WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
;     163   #asm("sei")                    // Enable global interrupt.
;     164 }
;     165 
;     166 char namebuf[8];

	.DSEG
_namebuf:
	.BYTE 0x8
;     167 char* tname2b(u8 i)
;     168 {

	.CSEG
_tname2b:
;     169         u8 j=0;
;     170         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0xE:
	CPI  R17,8
	BRSH _0xF
;     171         {
;     172             namebuf[j] = tprbdata.name[i][j];
	CALL SUBOPT_0x0
	MOVW R0,R30
	__POINTW2MN _tprbdata,288
	CALL SUBOPT_0x1
;     173             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0xF
;     174                 break;
;     175 
;     176             if(namebuf[j] < '0' || namebuf[j] > '9')
	CALL SUBOPT_0x0
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x12
	CALL SUBOPT_0x0
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x11
_0x12:
;     177             {
;     178                 namebuf[0] = '\0';
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     179                 break;
	RJMP _0xF
;     180             }
;     181 
;     182         }
_0x11:
	SUBI R17,-1
	RJMP _0xE
_0xF:
;     183         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x45C
;     184 }
;     185 
;     186 char* rname2b(u8 i)
;     187 {
_rname2b:
;     188         u8 j=0;
;     189         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x15:
	CPI  R17,8
	BRSH _0x16
;     190         {
;     191             namebuf[j] = rprbdata.name[i][j];
	CALL SUBOPT_0x0
	MOVW R0,R30
	__POINTW2MN _rprbdata,288
	CALL SUBOPT_0x1
;     192             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x16
;     193                 break;
;     194 
;     195             if(namebuf[j] < '0' || namebuf[j] > '9')
	CALL SUBOPT_0x0
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x19
	CALL SUBOPT_0x0
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x18
_0x19:
;     196             {
;     197                 namebuf[0] = '\0';
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     198                 break;
	RJMP _0x16
;     199             }
;     200 
;     201         }
_0x18:
	SUBI R17,-1
	RJMP _0x15
_0x16:
;     202         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x45C
;     203 }
;     204 void sleepms(u16 ms)
;     205 {
_sleepms:
;     206     while(ms-- > 0)
;	ms -> Y+0
_0x1B:
	LD   R26,Y
	LDD  R27,Y+1
	SBIW R26,1
	ST   Y,R26
	STD  Y+1,R27
	ADIW R26,1
	CALL __CPW02
	BRLO _0x1B
;     207         ;
;     208 }
_0x45C:
	ADIW R28,2
	RET
;     209 
;     210 
;     211 /**************************************************************************************/
;     212 //      board init related function.
;     213 /**************************************************************************************/
;     214 void init_var()
;     215 {
_init_var:
;     216 }
	RET
;     217 //state machine of therm type
;     218 //phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
;     219 //phase 1:get the reading and update the display
;     220 //phase 2: move to next channel
;     221 uchar therm_state()
;     222 {
_therm_state:
;     223         uchar i;
;     224 	i = sysdata.tid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	CALL SUBOPT_0x2
	MOV  R17,R30
;     225 	if(i == INVALID_PROBE)
	CPI  R17,255
	BRNE _0x1E
;     226 	{
;     227         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x3
;     228         	phase = 2;
;     229 	}
;     230 	if(phase == 0)
_0x1E:
	TST  R13
	BRNE _0x1F
;     231 	{
;     232 		if((tprbdata.type[i] <= PRBTYPE_R) && (tprbdata.type[i] >= PRBTYPE_K))
	CALL SUBOPT_0x4
	CPI  R30,LOW(0xA)
	BRSH _0x21
	CALL SUBOPT_0x4
	CPI  R30,LOW(0x3)
	BRSH _0x22
_0x21:
	RJMP _0x20
_0x22:
;     233 		{
;     234 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0x5
;     235 			dlg_cnt = ONESEC;
	__GETD1N 0x493E0
	CALL SUBOPT_0x6
;     236 			phase = 1;
	LDI  R30,LOW(1)
	MOV  R13,R30
;     237 			return 0;
	LDI  R30,LOW(0)
	RJMP _0x45B
;     238 		}else{
_0x20:
;     239 			rundata.temperature[ch_to_search] = -9999; //invalid data
	CALL SUBOPT_0x3
;     240 			phase = 2;
;     241 		}
;     242 	}
;     243 	if(phase == 1)
_0x1F:
	LDI  R30,LOW(1)
	CP   R30,R13
	BRNE _0x24
;     244 	{
;     245 		rundata.reading[ch_to_search] = nav_read();
	CALL SUBOPT_0x7
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL _nav_read
	POP  R26
	POP  R27
	CALL SUBOPT_0x8
;     246                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
	CALL SUBOPT_0x9
	CALL SUBOPT_0x7
	CALL SUBOPT_0xA
	CALL SUBOPT_0xB
;     247                 prints(strbuf,strlen(strbuf),PORT_PC);
;     248 
;     249 		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x7
	CALL SUBOPT_0xA
	CALL SUBOPT_0x4
	ST   -Y,R30
	CALL _MValueToTValue
	POP  R26
	POP  R27
	CALL SUBOPT_0x8
;     250                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0xC
	CALL SUBOPT_0xD
;     251                 prints(strbuf,strlen(strbuf),PORT_PC);
;     252 		phase = 2;
	LDI  R30,LOW(2)
	MOV  R13,R30
;     253 	}
;     254 	if(phase == 2)
_0x24:
	LDI  R30,LOW(2)
	CP   R30,R13
	BRNE _0x25
;     255 	{
;     256 		pgmain_handler(MSG_REFRESH);
	LDI  R30,LOW(254)
	ST   -Y,R30
	CALL _pgmain_handler
;     257 		phase = 0;
	CLR  R13
;     258 	}
;     259 	return 1;       //one reading is updated
_0x25:
	LDI  R30,LOW(1)
	RJMP _0x45B
;     260 }
;     261 //state machine of bore measure
;     262 //phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
;     263 //phase1 get reading of rs+, set to nktt, add delay
;     264 //phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
;     265 //phase3 get reading of rx-, set to pktt, add delay
;     266 //phase4 get reading of rx+, switch to rx, caculate
;     267 //phase5 update display and move to next channel
;     268 uchar bore_state()
;     269 {
_bore_state:
;     270         uchar i;
;     271 	i = sysdata.rid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	__POINTW1MN _sysdata,38
	ADD  R30,R10
	ADC  R31,R11
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R17,R30
;     272 	if(i == INVALID_PROBE)
	CPI  R17,255
	BRNE _0x26
;     273 	{
;     274         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0xE
;     275         	phase = 5;
;     276 	}
;     277 	if(phase == 0)
_0x26:
	TST  R13
	BRNE _0x27
;     278 	{
;     279 		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0xF
	CPI  R30,LOW(0xF3)
	BRSH _0x29
	CALL SUBOPT_0xF
	CPI  R30,LOW(0xF1)
	BRSH _0x2A
_0x29:
	RJMP _0x28
_0x2A:
;     280 		{
;     281 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0x5
;     282 			SET_PKTT;
	CALL SUBOPT_0x10
;     283 			SET_TORX;
	CALL SUBOPT_0x11
;     284 			dlg_cnt =  ONESEC * sysdata.ktime;
;     285 			if(IS_MODE_KTT)
	CALL SUBOPT_0x12
	BRNE _0x2B
;     286 				phase = 1;
	LDI  R30,LOW(1)
	RJMP _0x45D
;     287 			else
_0x2B:
;     288 				phase = 2;
	LDI  R30,LOW(2)
_0x45D:
	MOV  R13,R30
;     289 			return 0;
	LDI  R30,LOW(0)
	RJMP _0x45B
;     290 		}else{
_0x28:
;     291 			rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0xE
;     292 			phase = 5;
;     293 		}
;     294 
;     295 	}
;     296 	if(phase == 1)  //-stdV in ktt mode
_0x27:
	LDI  R30,LOW(1)
	CP   R30,R13
	BRNE _0x2E
;     297 	{
;     298 		rundata.stdV = nav_read();
	CALL SUBOPT_0x13
;     299 		SET_NKTT;
	LDI  R30,LOW(105)
	CALL SUBOPT_0x14
;     300 		dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x15
;     301 		phase = 2;
	LDI  R30,LOW(2)
	MOV  R13,R30
;     302 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x45B
;     303 	}
;     304 	if(phase == 2) //final stdV
_0x2E:
	LDI  R30,LOW(2)
	CP   R30,R13
	BRNE _0x2F
;     305 	{
;     306 		if(IS_MODE_KTT)
	CALL SUBOPT_0x12
	BRNE _0x30
;     307 		{
;     308 			rundata.stdV = (rundata.stdV + nav_read());
	CALL _nav_read
	__GETD2MN _rundata,196
	CALL __ADDF12
	__PUTD1MN _rundata,196
;     309 			phase = 3;
	LDI  R30,LOW(3)
	RJMP _0x45E
;     310 		}else{
_0x30:
;     311 			rundata.stdV = nav_read();
	CALL SUBOPT_0x13
;     312 			phase = 4;
	LDI  R30,LOW(4)
_0x45E:
	MOV  R13,R30
;     313 		}
;     314 		SET_TORX;
	CALL SUBOPT_0x11
;     315 		dlg_cnt = ONESEC * sysdata.ktime;
;     316 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x45B
;     317 	}
;     318 	if(phase == 3)  //-V on rx in ktt mode
_0x2F:
	LDI  R30,LOW(3)
	CP   R30,R13
	BRNE _0x32
;     319 	{
;     320 		valuep = nav_read();
	CALL _nav_read
	STS  _valuep,R30
	STS  _valuep+1,R31
	STS  _valuep+2,R22
	STS  _valuep+3,R23
;     321 		SET_PKTT;
	CALL SUBOPT_0x10
;     322 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x15
;     323 		phase = 4;
	LDI  R30,LOW(4)
	MOV  R13,R30
;     324 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x45B
;     325 	}
;     326 	if(phase == 4) // final V on rx
_0x32:
	LDI  R30,LOW(4)
	CP   R30,R13
	BREQ PC+3
	JMP _0x33
;     327 	{
;     328 		if(IS_MODE_KTT){
	CALL SUBOPT_0x12
	BRNE _0x34
;     329 			valuep = (valuep + nav_read());
	CALL _nav_read
	CALL SUBOPT_0x16
	CALL __ADDF12
	RJMP _0x45F
;     330 		}else{
_0x34:
;     331 			valuep = nav_read();
	CALL _nav_read
_0x45F:
	STS  _valuep,R30
	STS  _valuep+1,R31
	STS  _valuep+2,R22
	STS  _valuep+3,R23
;     332 		}
;     333 		if(rundata.stdV != 0)
	CALL SUBOPT_0x17
	CALL __CPD10
	BRNE PC+3
	JMP _0x36
;     334 		{
;     335 			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
	CALL SUBOPT_0x7
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x18
	CALL SUBOPT_0x16
	CALL SUBOPT_0x19
	CALL SUBOPT_0x17
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x1A
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x1B
	POP  R26
	POP  R27
	CALL __PUTDP1
;     336 			if(rundata.reading[ch_to_search] > 0)
	CALL SUBOPT_0x7
	CALL SUBOPT_0x1C
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD02
	BRGE _0x37
;     337 			{
;     338                                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x9
	CALL SUBOPT_0x1E
	CALL __PUTPARD1
	CALL SUBOPT_0xB
;     339                                 prints(strbuf,strlen(strbuf),PORT_PC);
;     340 
;     341 			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x7
	CALL SUBOPT_0xA
	ST   -Y,R17
	RCALL _RValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     342                                 rundata.Rx = rundata.reading[ch_to_search];
	CALL SUBOPT_0x7
	CALL SUBOPT_0x1C
	__PUTD1MN _rundata,192
;     343                                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xC
	CALL SUBOPT_0xD
;     344                                 prints(strbuf,strlen(strbuf),PORT_PC);
;     345 			}
;     346 		}else{
_0x37:
	RJMP _0x38
_0x36:
;     347 		        rundata.temperature[ch_to_search] = -9999;
	__POINTW2MN _rundata,96
	MOVW R30,R10
	CALL SUBOPT_0x1F
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
;     348 		}
_0x38:
;     349 		phase = 5;
	LDI  R30,LOW(5)
	MOV  R13,R30
;     350 	}
;     351 	if(phase == 5) //got one reading
_0x33:
	LDI  R30,LOW(5)
	CP   R30,R13
	BRNE _0x39
;     352 	{
;     353 		pgmain_handler(MSG_REFRESH);
	LDI  R30,LOW(254)
	ST   -Y,R30
	CALL _pgmain_handler
;     354 		phase = 0;
	CLR  R13
;     355 	}
;     356 	return 1;
_0x39:
	LDI  R30,LOW(1)
_0x45B:
	LD   R17,Y+
	RET
;     357 }
;     358 static uchar tA = 0xff;

	.DSEG
_tA_G1:
	.BYTE 0x1
;     359 static uchar tB = 0xff;
_tB_G1:
	.BYTE 0x1
;     360 void testA(uchar data)
;     361 {

	.CSEG
_testA:
;     362         tA = data;
;	data -> Y+0
	LD   R30,Y
	STS  _tA_G1,R30
;     363 }
	ADIW R28,1
	RET
;     364 void testB(uchar data)
;     365 {
;     366         tB = data;
;	data -> Y+0
;     367 }
;     368 /**************************************************************************************/
;     369 //                              Main Function Loop
;     370 /**************************************************************************************/
;     371 
;     372 void main(void)
;     373 {
_main:
;     374     u16 i;
;     375     double dt;
;     376 
;     377     // RS485 Node
;     378     init_var();	//init data structure
	SBIW R28,4
;	i -> R16,R17
;	dt -> Y+0
	CALL _init_var
;     379     // System Initialization
;     380     Init_Port();
	RCALL _Init_Port
;     381 //    Init_Timers();
;     382 //    Init_Ex_Interrupt();
;     383     Init_UART();
	RCALL _Init_UART
;     384     Enable_XMEM();
	RCALL _Enable_XMEM
;     385     Init_554();
	RCALL _Init_554
;     386     InitLED();
	CALL _InitLED
;     387     Key_Init();
	RCALL _Key_Init
;     388     // Global enable interrupts
;     389     WDTCR = 0x00; //disable dog watch
	LDI  R30,LOW(0)
	OUT  0x21,R30
;     390     #asm("sei")
	sei
;     391     /*********************************************************************/
;     392     // System hardware dection
;     393     /*********************************************************************/
;     394     // intialize LED.
;     395     nextwin = 0;
	CLR  R4
;     396     //init the DMM
;     397     nav_command(NAV_INIT);
	CALL SUBOPT_0x20
;     398     sleepms(2000);
;     399     nav_command(NAV_1V);
;     400     sleepms(2000);
;     401     nav_command(NAV_SLOWMODE);
;     402     sleepms(2000);
;     403     nav_command(NAV_AFLTON);
;     404 
;     405 
;     406     if(sysdata.Rs1 < 0.01)
	CALL SUBOPT_0x18
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3C23D70A
	CALL __CMPF12
	BRSH _0x3C
;     407     {
;     408         sysdata.Rs1 = 100;
	__POINTW2MN _sysdata,8
	__GETD1N 0x42C80000
	CALL __EEPROMWRD
;     409     }
;     410     sleepms(2000);
_0x3C:
	CALL SUBOPT_0x21
;     411     LCD_Init();
	CALL _LCD_Init
;     412     wnd_msgbox(&bootup);
	LDI  R30,LOW(_bootup*2)
	LDI  R31,HIGH(_bootup*2)
	CALL SUBOPT_0x22
;     413 
;     414     sleepms(2*ONESEC); //wait until all the node is ready after power up
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	CALL SUBOPT_0x23
;     415     if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x3D
;     416     {
;     417         if(key == KEY_NUM1) //R0
	LDI  R30,LOW(49)
	CP   R30,R5
	BRNE _0x3E
;     418         {
;     419                 window_setup(10);
	CALL SUBOPT_0x24
;     420                 sprintf(strbuf,"请输入铂电阻R0阻值");
	__POINTW1FN _0,42
	CALL SUBOPT_0x25
;     421 		sysdata.R0 = wnd_floatinput(sysdata.R0);
	CALL SUBOPT_0x1A
	CALL SUBOPT_0x26
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMWRD
;     422         }
;     423         if(key == KEY_NUM2) //Rs1
_0x3E:
	LDI  R30,LOW(50)
	CP   R30,R5
	BRNE _0x3F
;     424         {
;     425                 window_setup(10);
	CALL SUBOPT_0x24
;     426                 sprintf(strbuf,"请输入内标准阻值");
	__POINTW1FN _0,61
	CALL SUBOPT_0x25
;     427 		sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x18
	CALL SUBOPT_0x26
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;     428         }
;     429     }
_0x3F:
;     430 
;     431     State_Init();
_0x3D:
	CALL _State_Init
;     432 
;     433 
;     434 
;     435     SET_BORE_MODE;
	CALL SUBOPT_0x27
;     436 
;     437     SET_PKTT;
	CALL SUBOPT_0x10
;     438     SET_TORS;
	CALL SUBOPT_0x28
;     439 
;     440 	 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;     441 	 key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     442 
;     443 	 curr_ch = 1; //channel for display
	CALL SUBOPT_0x29
;     444 	 while(1)
_0x40:
;     445 	 {
;     446  	 	if(nextwin != 0)
	TST  R4
	BREQ _0x43
;     447 		{
;     448 			SwitchWindow(nextwin);
	ST   -Y,R4
	RCALL _SwitchWindow
;     449 			(*curr_window)(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	__CALL1MN _curr_window,0
;     450 			nextwin = 0;
	CLR  R4
;     451 		}
;     452 		if(key != KEY_INVALID)
_0x43:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE PC+3
	JMP _0x44
;     453 		{
;     454 			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
	LDI  R30,LOW(97)
	CP   R30,R5
	BREQ _0x46
	LDI  R30,LOW(98)
	CP   R30,R5
	BREQ _0x46
	LDI  R30,LOW(99)
	CP   R30,R5
	BREQ _0x46
	LDI  R30,LOW(100)
	CP   R30,R5
	BREQ _0x46
	RJMP _0x45
_0x46:
;     455 			{
;     456 				if(curr_window == pgmain_handler)
	CALL SUBOPT_0x2A
	BRNE _0x48
;     457 				{
;     458 					LCD_Cls();
	RCALL _LCD_Cls
;     459 					wnd_msgbox(&modify);
	LDI  R30,LOW(_modify*2)
	LDI  R31,HIGH(_modify*2)
	CALL SUBOPT_0x22
;     460 				}
;     461 				if(key == KEY_BTN1) //mode switch
_0x48:
	LDI  R30,LOW(97)
	CP   R30,R5
	BRNE _0x49
;     462 				{
;     463 					if(IS_BORE_MODE){
	CALL SUBOPT_0x2B
	BRNE _0x4A
;     464 						SET_THERM_MODE;
	CALL SUBOPT_0x2C
;     465                                                 SET_TORS;
;     466                                                 nav_command(NAV_120MV);
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _nav_command
;     467 					}else{
	RJMP _0x4B
_0x4A:
;     468 						SET_BORE_MODE;
	CALL SUBOPT_0x27
;     469                                                 SET_TORX;
	LDI  R30,LOW(106)
	CALL SUBOPT_0x14
;     470                                                 nav_command(NAV_INIT);
	CALL SUBOPT_0x20
;     471                                                 sleepms(2000);
;     472                                                 nav_command(NAV_1V);
;     473                                                 sleepms(2000);
;     474                                                 nav_command(NAV_SLOWMODE);
;     475                                                 sleepms(2000);
;     476                                                 nav_command(NAV_AFLTON);
;     477                                                 sleepms(2000);
	CALL SUBOPT_0x21
;     478 					}
_0x4B:
;     479 					dlg_cnt = 0;
	LDI  R30,0
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R30
	STS  _dlg_cnt+2,R30
	STS  _dlg_cnt+3,R30
;     480 					phase = 0;      //reset the state machine
	CLR  R13
;     481 
;     482 					SET_PKTT;
	CALL SUBOPT_0x10
;     483 				}
;     484 				if(key == KEY_BTN2) //auto ktt or not
_0x49:
	LDI  R30,LOW(98)
	CP   R30,R5
	BRNE _0x4C
;     485 				{
;     486 					if(IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x4D
;     487 					{
;     488 						if((IS_MODE_KTT)){
	CALL SUBOPT_0x12
	BRNE _0x4E
;     489 							CLR_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	RJMP _0x460
;     490 						}else{
_0x4E:
;     491 							SET_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	CALL SUBOPT_0x14
;     492 							SET_PKTT;
	LDI  R30,LOW(105)
_0x460:
	ST   -Y,R30
	CALL SUBOPT_0x2D
;     493 						}
;     494 					}
;     495 				}
_0x4D:
;     496 				if(key == KEY_BTN3) //thermal probe type
_0x4C:
	LDI  R30,LOW(99)
	CP   R30,R5
	BRNE _0x50
;     497 				{
;     498 
;     499 					display_buttons(KEY_BTN3,1);
	ST   -Y,R30
	CALL SUBOPT_0x2D
;     500 					if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x51
;     501 					{
;     502 					        i = sysdata.tid[ch_to_search];
	CALL SUBOPT_0x2
	MOV  R16,R30
	CLR  R17
;     503 					        if(i != INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x52
;     504 					        {
;     505                 					if((tprbdata.type[i] >= PRBTYPE_K) &&\
;     506 	                				   (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x2F
	CPI  R30,LOW(0x3)
	BRLO _0x54
	CALL SUBOPT_0x2F
	CPI  R30,LOW(0xA)
	BRLO _0x55
_0x54:
	RJMP _0x53
_0x55:
;     507 		                			{
;     508 			                			if(tprbdata.type[i] == PRBTYPE_R)
	CALL SUBOPT_0x2F
	CPI  R30,LOW(0x9)
	BRNE _0x56
;     509 				                			tprbdata.type[i] = PRBTYPE_K;
	__POINTW1MN _tprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	LDI  R30,LOW(3)
	RJMP _0x461
;     510 					                	else
_0x56:
;     511 						                	tprbdata.type[i] +=1;
	CALL SUBOPT_0x2F
	SUBI R30,-LOW(1)
_0x461:
	CALL __EEPROMWRB
;     512         					        }
;     513         					}
_0x53:
;     514                                         }
_0x52:
;     515 					display_buttons(KEY_BTN3,0);
_0x51:
	LDI  R30,LOW(99)
	CALL SUBOPT_0x14
;     516 
;     517 				}
;     518 				if(key == KEY_BTN4) //remove zero
_0x50:
	LDI  R30,LOW(100)
	CP   R30,R5
	BRNE _0x58
;     519 				{
;     520 					display_buttons(KEY_BTN4,1);
	ST   -Y,R30
	CALL SUBOPT_0x2D
;     521 					if(IS_BORE_MODE){
	CALL SUBOPT_0x2B
	BRNE _0x59
;     522 					        sysdata.R0 = rundata.Rx;
	CALL SUBOPT_0x1E
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMWRD
;     523 					}else{
	RJMP _0x5A
_0x59:
;     524 					        nav_command(NAV_ZEROON);
	LDI  R30,LOW(8)
	ST   -Y,R30
	RCALL _nav_command
;     525 					        sleepms(1000);
	LDI  R30,LOW(1000)
	LDI  R31,HIGH(1000)
	CALL SUBOPT_0x23
;     526 					}
_0x5A:
;     527 					display_buttons(KEY_BTN4,0);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x14
;     528 				}
;     529 				if(curr_window == pgmain_handler)       //redraw the running window
_0x58:
	CALL SUBOPT_0x2A
	BRNE _0x5B
;     530 				{
;     531 					pgmain_handler(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _pgmain_handler
;     532 				}
;     533 			}else{
_0x5B:
	RJMP _0x5C
_0x45:
;     534 				(*curr_window)(key);
	ST   -Y,R5
	__CALL1MN _curr_window,0
;     535 			}
_0x5C:
;     536 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     537 	  	}else{
	RJMP _0x5D
_0x44:
;     538 			if(curr_window != pgmain_handler)
	CALL SUBOPT_0x2A
	BREQ _0x5E
;     539 				continue;
	RJMP _0x40
;     540 			if(dlg_cnt > 1)
_0x5E:
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	__CPD2N 0x2
	BRLT _0x5F
;     541 			{
;     542 				dlg_cnt--;
	LDS  R30,_dlg_cnt
	LDS  R31,_dlg_cnt+1
	LDS  R22,_dlg_cnt+2
	LDS  R23,_dlg_cnt+3
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	CALL SUBOPT_0x6
;     543 				continue;
	RJMP _0x40
;     544 			}
;     545 			if((IS_THERM_MODE))
_0x5F:
	CALL SUBOPT_0x2E
	BRNE _0x60
;     546 			{
;     547 				if(therm_state() == 0)
	CALL _therm_state
	CPI  R30,0
	BRNE _0x61
;     548 				        continue;
	RJMP _0x40
;     549 			}else{
_0x61:
	RJMP _0x62
_0x60:
;     550 				if(bore_state() == 0)
	CALL _bore_state
	CPI  R30,0
	BRNE _0x63
;     551 				        continue;
	RJMP _0x40
;     552 			}
_0x63:
_0x62:
;     553                         //shift to next channel
;     554                         while(true)
_0x64:
;     555                         {
;     556                                 ch_to_search += 1;
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
;     557         			if(ch_to_search >= MAX_CH_NUM)
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R10,R30
	CPC  R11,R31
	BRLT _0x67
;     558         			{
;     559 	        			ch_to_search = 0;
	CLR  R10
	CLR  R11
;     560 	        			break;
	RJMP _0x66
;     561 	        		}
;     562 	        		if(IS_THERM_MODE)
_0x67:
	CALL SUBOPT_0x2E
	BRNE _0x68
;     563 	        		{
;     564 	        		        i = sysdata.tid[ch_to_search];
	__POINTW1MN _sysdata,14
	RJMP _0x462
;     565 	        		}else{
_0x68:
;     566 	        		        i = sysdata.rid[ch_to_search];
	__POINTW1MN _sysdata,38
_0x462:
	ADD  R30,R10
	ADC  R31,R11
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R16,R30
	CLR  R17
;     567 	        		}
;     568 	        		if(i == INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x64
;     569 	        		        continue;
;     570 	        		if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x6B
;     571 	        		{
;     572        					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x2F
	CPI  R30,LOW(0x3)
	BRLO _0x6D
	CALL SUBOPT_0x2F
	CPI  R30,LOW(0xA)
	BRLO _0x6E
_0x6D:
	RJMP _0x6C
_0x6E:
;     573        					        break;
	RJMP _0x66
;     574 	        		}else{
_0x6C:
	RJMP _0x6F
_0x6B:
;     575        		        		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0x30
	CPI  R30,LOW(0xF3)
	BRSH _0x71
	CALL SUBOPT_0x30
	CPI  R30,LOW(0xF1)
	BRSH _0x72
_0x71:
	RJMP _0x70
_0x72:
;     576        		        		        break;
	RJMP _0x66
;     577 	        		}
_0x70:
_0x6F:
;     578 	                }
	RJMP _0x64
_0x66:
;     579 		}
_0x5D:
;     580 	}
	RJMP _0x40
;     581 }
_0x73:
	RJMP _0x73
;     582 
;     583 WINDOW flash wins[]={
;     584 	{PG_MAIN,	pgmain_handler},	//running menu
;     585 	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
;     586 	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
;     587 	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
;     588 	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
;     589 	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
;     590 	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
;     591 	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
;     592 	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
;     593 };
;     594 
;     595 
;     596 void SwitchWindow(uchar page)
;     597 {
_SwitchWindow:
;     598 	u8 i;
;     599 	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	ST   -Y,R17
;	page -> Y+1
;	i -> R17
	LDI  R17,LOW(0)
_0x75:
	CPI  R17,9
	BRSH _0x76
;     600 	{
;     601 		if(wins[i].page_id == page)
	LDI  R30,LOW(3)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_wins*2)
	SBCI R31,HIGH(-_wins*2)
	LPM  R30,Z
	MOV  R26,R30
	LDD  R30,Y+1
	CP   R30,R26
	BRNE _0x77
;     602 		{
;     603 			curr_window = wins[i].handler;
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_wins,1
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	CALL SUBOPT_0x31
	STS  _curr_window,R30
	STS  _curr_window+1,R31
;     604 			return;
	LDD  R17,Y+0
	RJMP _0x45A
;     605 		}
;     606 	}
_0x77:
	SUBI R17,-1
	RJMP _0x75
_0x76:
;     607 }
	LDD  R17,Y+0
_0x45A:
	ADIW R28,2
	RET
;     608 /*********************************************************************************/
;     609 //                         UART0 and UART1 Subroutines
;     610 /*********************************************************************************/
;     611 #include <mega64.h>
;     612 #include "uart.h"
;     613 #include "16c554.h"
;     614 #include "global.h"
;     615 
;     616 //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
;     617 //extern u8 x_modem_databuf[];                       /* store data sent from PC */
;     618 //extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
;     619 //extern u8 x_modem_file_being_transferred;
;     620 //extern u8 x_modem_file_transfer_complete;
;     621 
;     622 /*********************************************************************************/
;     623 //                     UART0  Variables & Buffer Definition
;     624 /*********************************************************************************/
;     625 u8 volatile tx_buffer0[TX_BUFFER_SIZE0];

	.DSEG
_tx_buffer0:
	.BYTE 0x80
;     626 u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
;     627 
;     628 /*********************************************************************************/
;     629 //                     UART1  Variables & Buffer Definition
;     630 /*********************************************************************************/
;     631 u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
_tx_buffer1:
	.BYTE 0x80
;     632 u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;
_tx_wr_index1:
	.BYTE 0x1
_tx_rd_index1:
	.BYTE 0x1
_tx_counter1:
	.BYTE 0x1
;     633 
;     634 /*
;     635  * 0-9,a-f,A-F char to hex conversion
;     636  * otherwise 0xff will be returned.
;     637  */
;     638 u8 ishexchar(u8 dat){

	.CSEG
;     639         if( (dat >= '0') && (dat <= '9'))
;	dat -> Y+0
;     640                 return dat-'0';
;     641         if( (dat >= 'a') && (dat <= 'f'))
;     642                 return dat-'a'+0x0a;
;     643         if( (dat >= 'A') && (dat <= 'F'))
;     644                 return dat-'A'+0x0a;
;     645         return 0xff;
;     646 }
;     647 
;     648 
;     649 
;     650 interrupt [USART0_RXC] void usart0_rx_isr(void)
;     651 {
_usart0_rx_isr:
	CALL SUBOPT_0x32
;     652    u8 status,data;
;     653    //read status and data
;     654    status=UCSR0A;
;	status -> R17
;	data -> R16
	IN   R17,11
;     655    data=UDR0;
	IN   R16,12
;     656    cm0_push(data);
	ST   -Y,R16
	RCALL _kbd_uart_push
;     657 }
	CALL SUBOPT_0x33
	RETI
;     658 
;     659 /*********************************************************************************/
;     660 //                 USART0 Transmitter interrupt service routine
;     661 /*********************************************************************************/
;     662 interrupt [USART0_TXC] void usart0_tx_isr(void)
;     663 {
_usart0_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     664 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0x81
;     665    {
;     666    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
;     667    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	CALL SUBOPT_0x34
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	OUT  0xC,R30
;     668    tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_rd_index0
	ANDI R30,0x7F
	STS  _tx_rd_index0,R30
;     669    };
_0x81:
;     670 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     671 
;     672 /*********************************************************************************/
;     673 //                      UART1 Receiver interrupt service routine
;     674 /*********************************************************************************/
;     675 interrupt [USART1_RXC] void usart1_rx_isr(void)
;     676 {
_usart1_rx_isr:
	CALL SUBOPT_0x32
;     677    u8 status,data;
;     678 
;     679    //read status and data
;     680    status=UCSR1A;
;	status -> R17
;	data -> R16
	LDS  R17,155
;     681    data=UDR1;
	LDS  R16,156
;     682    cm1_push(data);
	ST   -Y,R16
	RCALL _nav_uart_push
;     683 }
	CALL SUBOPT_0x33
	RETI
;     684 
;     685 /*********************************************************************************/
;     686 //                   USART1 Transmitter interrupt service routine
;     687 /*********************************************************************************/
;     688 interrupt [USART1_TXC] void usart1_tx_isr(void)
;     689 {
_usart1_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     690 if (tx_counter1)
	LDS  R30,_tx_counter1
	CPI  R30,0
	BREQ _0x82
;     691    {
;     692    --tx_counter1;
	SUBI R30,LOW(1)
	STS  _tx_counter1,R30
;     693    UDR1=tx_buffer1[tx_rd_index1++];
	LDS  R30,_tx_rd_index1
	SUBI R30,-LOW(1)
	STS  _tx_rd_index1,R30
	CALL SUBOPT_0x34
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R30,Z
	STS  156,R30
;     694    tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_rd_index1
	ANDI R30,0x7F
	STS  _tx_rd_index1,R30
;     695    };
_0x82:
;     696 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     697 
;     698 void com0_putc(char c)
;     699 {
_com0_putc:
;     700 if(DEBUG == 1)
;	c -> Y+0
;     701         return;
;     702 while (tx_counter0 == TX_BUFFER_SIZE0);
_0x84:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x80)
	BREQ _0x84
;     703 #asm("cli")
	cli
;     704 if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0x88
	SBIC 0xB,5
	RJMP _0x87
_0x88:
;     705    {
;     706    tx_buffer0[tx_wr_index0++]=c;
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	CALL SUBOPT_0x34
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
;     707    tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_wr_index0
	ANDI R30,0x7F
	STS  _tx_wr_index0,R30
;     708    ++tx_counter0;
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
;     709    }
;     710 else
	RJMP _0x8A
_0x87:
;     711    UDR0=c;
	LD   R30,Y
	OUT  0xC,R30
;     712 #asm("sei")
_0x8A:
	sei
;     713 }
	ADIW R28,1
	RET
;     714 
;     715 
;     716 void com1_putc(char c)
;     717 {
_com1_putc:
;     718 while (tx_counter1 == TX_BUFFER_SIZE1);
;	c -> Y+0
_0x8B:
	LDS  R26,_tx_counter1
	CPI  R26,LOW(0x80)
	BREQ _0x8B
;     719 #asm("cli")
	cli
;     720 if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter1
	CPI  R30,0
	BRNE _0x8F
	LDS  R30,155
	ANDI R30,LOW(0x20)
	BRNE _0x8E
_0x8F:
;     721    {
;     722    tx_buffer1[tx_wr_index1++]=c;
	LDS  R30,_tx_wr_index1
	SUBI R30,-LOW(1)
	STS  _tx_wr_index1,R30
	CALL SUBOPT_0x34
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R26,Y
	STD  Z+0,R26
;     723    tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_wr_index1
	ANDI R30,0x7F
	STS  _tx_wr_index1,R30
;     724    ++tx_counter1;
	LDS  R30,_tx_counter1
	SUBI R30,-LOW(1)
	STS  _tx_counter1,R30
;     725    }
;     726 else
	RJMP _0x91
_0x8E:
;     727    UDR1=c;
	LD   R30,Y
	STS  156,R30
;     728 #asm("sei")
_0x91:
	sei
;     729 }
	ADIW R28,1
	RET
;     730 
;     731 /*
;     732 void com0_puthex(u8 a)
;     733 {
;     734 	unsigned char h,l;
;     735 	h = (a & 0xf0) >> 4;
;     736 	l = (a & 0x0f);
;     737 	if(h <= 9)
;     738 		com0_putc(h+'0');
;     739 	else
;     740 		com0_putc(h+'A'-0x0a);
;     741 	if(l <= 9)
;     742 		com0_putc(l+'0');
;     743 	else
;     744 		com0_putc(l+'A'-0x0a);
;     745 }
;     746 */
;     747 
;     748 void com0_putstr(u8 *ptr){
;     749         while(*ptr != 0x00){
;	*ptr -> Y+0
;     750                 com0_putc(*ptr++);
;     751         }
;     752 }
;     753 
;     754 void com1_puthex(u8 a)
;     755 {
;     756 	unsigned char h,l;
;     757 	h = (a & 0xf0) >> 4;
;	a -> Y+2
;	h -> R17
;	l -> R16
;     758 	l = (a & 0x0f);
;     759 	if(h <= 9)
;     760 		com1_putc(h+'0');
;     761 	else
;     762 		com1_putc(h+'A'-0x0a);
;     763 	if(l <= 9)
;     764 		com1_putc(l+'0');
;     765 	else
;     766 		com1_putc(l+'A'-0x0a);
;     767 }
;     768 void com1_putstr(u8 *ptr){
;     769         while(*ptr != 0x00){
;	*ptr -> Y+0
;     770                 com1_putc(*ptr++);
;     771         }
;     772 }
;     773 /*
;     774 void com0_mputs(u8 *buf, u8 size){
;     775         while(size-- > 0){
;     776                 com0_puthex(*buf++);
;     777         }
;     778 }
;     779 */
;     780 void com1_mputs(u8 *buf, u8 size){
;     781         while(size-- > 0){
;	*buf -> Y+1
;	size -> Y+0
;     782                 com1_puthex(*buf++);
;     783         }
;     784 }
;     785 
;     786 /*********************************************************************************/
;     787 //                              Intialize UART
;     788 /*********************************************************************************/
;     789 void Init_UART()
;     790 {
_Init_UART:
;     791    // USART0 initialization
;     792    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     793    // USART0 Receiver: On
;     794    // USART0 Transmitter: On
;     795    // USART0 Mode: Asynchronous
;     796    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     797       UCSR0A=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;     798       UCSR0B=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     799       UCSR0C=0x06;
	LDI  R30,LOW(6)
	STS  149,R30
;     800       UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  144,R30
;     801       UBRR0L=0x0F;
	LDI  R30,LOW(15)
	OUT  0x9,R30
;     802 
;     803    // USART1 initialization
;     804    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     805    // USART1 Receiver: On
;     806    // USART1 Transmitter: On
;     807    // USART1 Mode: Asynchronous
;     808    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     809       UCSR1A=0x00;
	LDI  R30,LOW(0)
	STS  155,R30
;     810       UCSR1B=0xD8;
	LDI  R30,LOW(216)
	STS  154,R30
;     811       UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
;     812       UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
;     813       UBRR1L=0x0F;
	LDI  R30,LOW(15)
	STS  153,R30
;     814 
;     815    // Initialize buffer variables
;     816       tx_wr_index0 = 0;
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
;     817       tx_rd_index0 = 0;
	STS  _tx_rd_index0,R30
;     818       tx_counter0  = 0;
	STS  _tx_counter0,R30
;     819 
;     820       tx_wr_index1 = 0;
	STS  _tx_wr_index1,R30
;     821       tx_rd_index1 = 0;
	STS  _tx_rd_index1,R30
;     822       tx_counter1  = 0;
	STS  _tx_counter1,R30
;     823 }
	RET
;     824 /********************************************************************************/
;     825 //                             TL16C554 Driver
;     826 // Controller:  MEGA64
;     827 // 16C554 clk:  7.3728MHZ
;     828 //
;     829 // Schematic Description:
;     830 // 16C554 <---> MEGA64
;     831 // INTA   <---> INT4
;     832 // INTB   <---> INT5
;     833 // INTC   <---> INT6
;     834 // INTD   <---> INT7
;     835 // CSA#   <---> A15
;     836 // CSB#   <---> A14
;     837 // CSC#   <---> A13
;     838 // CSD#   <---> A12
;     839 //   A0   <---> A8
;     840 //   A1   <---> A9
;     841 //   A2   <---> A10
;     842 // RESET  <---> PB0
;     843 // INTN#  <---> PE2
;     844 
;     845 // Coded by: fd_lxh@yahoo.com.cn
;     846 /********************************************************************************/
;     847 #include "mega64.h"
;     848 #include "global.h"
;     849 #include "16c554.h"
;     850 #include "uart.h"
;     851 
;     852 #define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
;     853 #define IER_DATA 0b00000001
;     854 // LCR.7 must be cleared to 0 when accessing RBR/THR/IER
;     855 // LCR.7 must be set to 1 when accessing divisor latch
;     856 #define LCR_DATA 0b00011011   // Even Parity 8 data bit, 1 stop bit
;     857 #define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
;     858 #define DLM_DATA 0x0
;     859 #define MCR_DATA 0b00001000
;     860 
;     861 #define min(A,B)        ((A<B)?(A):(B))
;     862 /********************************************************************************/
;     863 //               TLC16C554 UART PORT A Interrupt Subroutine
;     864 /********************************************************************************/
;     865 interrupt [EXT_INT4] void ext_int4_isr(void)
;     866 {
_ext_int4_isr:
	CALL SUBOPT_0x35
;     867    u8 rundata;
;     868    switch(UC_554A_IIR & 0xF)
;	rundata -> R17
	LDS  R30,29184
	ANDI R30,LOW(0xF)
;     869    {
;     870       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xA2
;     871           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     872           break;
	RJMP _0xA1
;     873       case 0x4:                         // Receiver data available or trigger level reached
_0xA2:
	CPI  R30,LOW(0x4)
	BRNE _0xA4
;     874           rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,28672
;     875           cm_pushA(rundata);
	ST   -Y,R17
	RCALL _pc_uart_push
;     876           return;
	CALL SUBOPT_0x36
	RETI
;     877       default:
_0xA4:
;     878           break;
;     879    }
_0xA1:
;     880 
;     881    rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,29952
;     882 }
	CALL SUBOPT_0x36
	RETI
;     883 /********************************************************************************/
;     884 //               TLC16C554 UART PORT B Interrupt Subroutine
;     885 /********************************************************************************/
;     886 // External Interrupt 5 service routine
;     887 interrupt [EXT_INT5] void ext_int5_isr(void)
;     888 {
_ext_int5_isr:
	CALL SUBOPT_0x35
;     889    u8 rundata;
;     890    switch(UC_554B_IIR & 0xF)
;	rundata -> R17
	LDS  R30,45568
	ANDI R30,LOW(0xF)
;     891    {
;     892       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xA8
;     893           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     894           break;
	RJMP _0xA7
;     895       case 0x4:                         // Receiver data available or trigger level reached
_0xA8:
	CPI  R30,LOW(0x4)
	BRNE _0xAA
;     896           rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,45056
;     897           cm_pushB(rundata);
	ST   -Y,R17
	RCALL _scanner_uart_push
;     898           return;
	CALL SUBOPT_0x36
	RETI
;     899       default:
_0xAA:
;     900           break;
;     901    }
_0xA7:
;     902    rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,46336
;     903 }
	CALL SUBOPT_0x36
	RETI
;     904 /********************************************************************************/
;     905 //               TLC16C554 UART PORT C Interrupt Subroutine
;     906 /********************************************************************************/
;     907 // External Interrupt 6 service routine
;     908 interrupt [EXT_INT6] void ext_int6_isr(void)
;     909 {
_ext_int6_isr:
	CALL SUBOPT_0x35
;     910    u8 rundata;
;     911    switch(UC_554C_IIR & 0xF)
;	rundata -> R17
	LDS  R30,53760
	ANDI R30,LOW(0xF)
;     912    {
;     913       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xAE
;     914           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     915           break;
	RJMP _0xAD
;     916       case 0x4:                         // Receiver data available or trigger level reached
_0xAE:
	CPI  R30,LOW(0x4)
	BRNE _0xB0
;     917           rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,53248
;     918           cm_pushC(rundata);
	ST   -Y,R17
	CALL _testA
;     919           return;
	CALL SUBOPT_0x36
	RETI
;     920       default:
_0xB0:
;     921           break;
;     922    }
_0xAD:
;     923 
;     924    rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,54528
;     925 }
	CALL SUBOPT_0x36
	RETI
;     926 
;     927 /********************************************************************************/
;     928 //               TLC16C554 UART PORT D Interrupt Subroutine
;     929 /********************************************************************************/
;     930 // External Interrupt 7 service routine
;     931 interrupt [EXT_INT7] void ext_int7_isr(void)
;     932 {
_ext_int7_isr:
	CALL SUBOPT_0x35
;     933    u8 rundata;
;     934    switch(UC_554D_IIR & 0xF)
;	rundata -> R17
	LDS  R30,57856
	ANDI R30,LOW(0xF)
;     935    {
;     936       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xB4
;     937           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     938           break;
	RJMP _0xB3
;     939       case 0x4:                         // Receiver data available or trigger level reached
_0xB4:
	CPI  R30,LOW(0x4)
	BRNE _0xB6
;     940           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     941           cm_pushD(rundata);
	ST   -Y,R17
	CALL _testA
;     942           return;
	CALL SUBOPT_0x36
	RETI
;     943       default:
_0xB6:
;     944           break;
;     945    }
_0xB3:
;     946    rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,58624
;     947 }
	CALL SUBOPT_0x36
	RETI
;     948 
;     949 /********************************************************************************/
;     950 //                             Reset 16C554
;     951 /********************************************************************************/
;     952 void Reset_554(void)
;     953 {
_Reset_554:
;     954    // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
;     955       PORTB.0 = 1;
	SBI  0x18,0
;     956       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x23
;     957       PORTB.0 = 0;     // Recover from reset
	CBI  0x18,0
;     958       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x23
;     959 }
	RET
;     960 /********************************************************************************/
;     961 //                             Initialize 16C554
;     962 /********************************************************************************/
;     963 void Init_554(void)
;     964 {
_Init_554:
;     965       Reset_554();  // Reset 554
	CALL _Reset_554
;     966 
;     967    /********************************************************************/
;     968    //                      Initialize UART A
;     969    /********************************************************************/
;     970    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;     971       UC_554A_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  29440,R30
;     972       UC_554A_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  28672,R30
;     973       UC_554A_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  28928,R30
;     974 
;     975    // Set Line Control Register:
;     976    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;     977       UC_554A_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  29440,R30
;     978 
;     979    // Set FIFO Control Register:
;     980    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;     981       UC_554A_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  29184,R30
;     982 
;     983    // Set Modem Control Register:
;     984    // Enable enternal interrupt
;     985       UC_554A_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  29696,R30
;     986 
;     987    // Set Interrupt Enable Register:
;     988    // Enable Received data available / timeout / transmitter holding register empty &
;     989    // Receiver line status interrupt
;     990       UC_554A_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  28928,R30
;     991 
;     992    /********************************************************************/
;     993    //                      Initialize UART B
;     994    /********************************************************************/
;     995    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;     996    // LCR.7 must be set to "1" before setting baud rate
;     997       UC_554B_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  45824,R30
;     998       UC_554B_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  45056,R30
;     999       UC_554B_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  45312,R30
;    1000 
;    1001    // Set Line Control Register:
;    1002    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1003       UC_554B_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  45824,R30
;    1004 
;    1005    // Set FIFO Control Register:
;    1006    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1007       UC_554B_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  45568,R30
;    1008 
;    1009    // Set Modem Control Register:
;    1010    // Enable enternal interrupt
;    1011       UC_554B_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  46080,R30
;    1012 
;    1013    // Set Interrupt Enable Register:
;    1014    // Enable Received data available / timeout / transmitter holding register empty &
;    1015    // Receiver line status interrupt
;    1016       UC_554B_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  45312,R30
;    1017 
;    1018    /********************************************************************/
;    1019    //                      Initialize UART C
;    1020    /********************************************************************/
;    1021    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1022       UC_554C_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  54016,R30
;    1023       UC_554C_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  53248,R30
;    1024       UC_554C_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  53504,R30
;    1025 
;    1026    // Set Line Control Register:
;    1027    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1028       UC_554C_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  54016,R30
;    1029 
;    1030    // Set FIFO Control Register:
;    1031    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1032       UC_554C_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  53760,R30
;    1033 
;    1034    // Set Modem Control Register:
;    1035    // Enable enternal interrupt
;    1036       UC_554C_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  54272,R30
;    1037 
;    1038    // Set Interrupt Enable Register:
;    1039    // Enable Received data available / timeout / transmitter holding register empty &
;    1040    // Receiver line status interrupt
;    1041       UC_554C_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  53504,R30
;    1042 
;    1043    /********************************************************************/
;    1044    //                      Initialize UART D
;    1045    /********************************************************************/
;    1046    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1047       UC_554D_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  58112,R30
;    1048       UC_554D_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  57344,R30
;    1049       UC_554D_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  57600,R30
;    1050 
;    1051    // Set Line Control Register:
;    1052    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1053       UC_554D_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  58112,R30
;    1054 
;    1055    // Set FIFO Control Register:
;    1056    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1057       UC_554D_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  57856,R30
;    1058 
;    1059    // Set Modem Control Register:
;    1060    // Enable enternal interrupt
;    1061       UC_554D_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  58368,R30
;    1062 
;    1063    // Set Interrupt Enable Register:
;    1064    // Enable Received data available / timeout / transmitter holding register empty &
;    1065    // Receiver line status interrupt
;    1066       UC_554D_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  57600,R30
;    1067 }
	RET
;    1068 
;    1069 
;    1070 /********************************************************************************/
;    1071 //                 Send multi-bytes (less than 16 bytes) via UART
;    1072 // Function:
;    1073 // Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
;    1074 // format:
;    1075 // print_554(u8 *str, char uart_port)
;    1076 // arg1: string to be sent. arg2: port (A B,C,D)
;    1077 /********************************************************************************/
;    1078 void prints(u8 *str, u8 length, char uart_port)
;    1079 {
_prints:
;    1080     u8 len, i;
;    1081     len = length & 0x0F;        //15bytes at most
	ST   -Y,R17
	ST   -Y,R16
;	*str -> Y+4
;	length -> Y+3
;	uart_port -> Y+2
;	len -> R17
;	i -> R16
	LDD  R30,Y+3
	ANDI R30,LOW(0xF)
	MOV  R17,R30
;    1082     switch(uart_port)
	LDD  R30,Y+2
;    1083     {
;    1084        case SPORTA:
	CPI  R30,0
	BRNE _0xBA
;    1085            // wait if data still being transmitted in UART
;    1086            // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
;    1087            // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
;    1088            // check if FIFO is empty by LSR5.
;    1089            // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
;    1090            // UC_554A_LSR is not a register name,
;    1091            // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
;    1092            // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
;    1093            // here in 16c554.h, a bracket is added
;    1094            while(len)
_0xBB:
	CPI  R17,0
	BREQ _0xBD
;    1095            {
;    1096               while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xBE:
	LDS  R30,29952
	ANDI R30,LOW(0x20)
	BREQ _0xBE
;    1097               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xC2:
	CPI  R17,16
	BRSH _0xC4
	MOV  R30,R17
	RJMP _0xC5
_0xC4:
	LDI  R30,LOW(16)
_0xC5:
	CP   R16,R30
	BRSH _0xC3
;    1098                   UC_554A_THR = *str++;
	CALL SUBOPT_0x37
	STS  28672,R30
;    1099               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xC2
_0xC3:
	CPI  R17,16
	BRSH _0xC7
	MOV  R30,R17
	RJMP _0xC8
_0xC7:
	LDI  R30,LOW(16)
_0xC8:
	SUB  R17,R30
;    1100            }
	RJMP _0xBB
_0xBD:
;    1101            break;
	RJMP _0xB9
;    1102        case SPORTB:
_0xBA:
	CPI  R30,LOW(0x1)
	BRNE _0xCA
;    1103            while(len)
_0xCB:
	CPI  R17,0
	BREQ _0xCD
;    1104            {
;    1105               while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xCE:
	LDS  R30,46336
	ANDI R30,LOW(0x20)
	BREQ _0xCE
;    1106               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xD2:
	CPI  R17,16
	BRSH _0xD4
	MOV  R30,R17
	RJMP _0xD5
_0xD4:
	LDI  R30,LOW(16)
_0xD5:
	CP   R16,R30
	BRSH _0xD3
;    1107                   UC_554B_THR = *str++;
	CALL SUBOPT_0x37
	STS  45056,R30
;    1108               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xD2
_0xD3:
	CPI  R17,16
	BRSH _0xD7
	MOV  R30,R17
	RJMP _0xD8
_0xD7:
	LDI  R30,LOW(16)
_0xD8:
	SUB  R17,R30
;    1109            }
	RJMP _0xCB
_0xCD:
;    1110            break;
	RJMP _0xB9
;    1111        case SPORTC:
_0xCA:
	CPI  R30,LOW(0x2)
	BRNE _0xDA
;    1112            while(len)
_0xDB:
	CPI  R17,0
	BREQ _0xDD
;    1113            {
;    1114               while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xDE:
	LDS  R30,54528
	ANDI R30,LOW(0x20)
	BREQ _0xDE
;    1115               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xE2:
	CPI  R17,16
	BRSH _0xE4
	MOV  R30,R17
	RJMP _0xE5
_0xE4:
	LDI  R30,LOW(16)
_0xE5:
	CP   R16,R30
	BRSH _0xE3
;    1116                   UC_554C_THR = *str++;
	CALL SUBOPT_0x37
	STS  53248,R30
;    1117               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xE2
_0xE3:
	CPI  R17,16
	BRSH _0xE7
	MOV  R30,R17
	RJMP _0xE8
_0xE7:
	LDI  R30,LOW(16)
_0xE8:
	SUB  R17,R30
;    1118            }
	RJMP _0xDB
_0xDD:
;    1119 
;    1120            break;
	RJMP _0xB9
;    1121        case SPORTD:
_0xDA:
	CPI  R30,LOW(0x3)
	BRNE _0xEA
;    1122            while(len)
_0xEB:
	CPI  R17,0
	BREQ _0xED
;    1123            {
;    1124               while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xEE:
	LDS  R30,58624
	ANDI R30,LOW(0x20)
	BREQ _0xEE
;    1125               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xF2:
	CPI  R17,16
	BRSH _0xF4
	MOV  R30,R17
	RJMP _0xF5
_0xF4:
	LDI  R30,LOW(16)
_0xF5:
	CP   R16,R30
	BRSH _0xF3
;    1126                   UC_554D_THR = *str++;
	CALL SUBOPT_0x37
	STS  57344,R30
;    1127               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xF2
_0xF3:
	CPI  R17,16
	BRSH _0xF7
	MOV  R30,R17
	RJMP _0xF8
_0xF7:
	LDI  R30,LOW(16)
_0xF8:
	SUB  R17,R30
;    1128            }
	RJMP _0xEB
_0xED:
;    1129            break;
	RJMP _0xB9
;    1130        case SPORT0:
_0xEA:
	CPI  R30,LOW(0x4)
	BRNE _0xFA
;    1131            while(len-- > 0){
_0xFB:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0xFD
;    1132                 com0_putc(*str++);
	CALL SUBOPT_0x37
	ST   -Y,R30
	CALL _com0_putc
;    1133            }
	RJMP _0xFB
_0xFD:
;    1134            break;
	RJMP _0xB9
;    1135        case SPORT1:
_0xFA:
	CPI  R30,LOW(0x5)
	BRNE _0x102
;    1136            while(len-- > 0){
_0xFF:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x101
;    1137                 com1_putc(*str++);
	CALL SUBOPT_0x37
	ST   -Y,R30
	CALL _com1_putc
;    1138            }
	RJMP _0xFF
_0x101:
;    1139            break;
	RJMP _0xB9
;    1140        default:
_0x102:
;    1141            PORTC = 0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1142            break;
;    1143     }
_0xB9:
;    1144 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    1145 
;    1146 
;    1147 // System Initialization file
;    1148 
;    1149 #include <mega64.h>
;    1150 #include "global.h"
;    1151 /**************************************************************************************/
;    1152 //                              Initialize Port A-G
;    1153 /**************************************************************************************/
;    1154 void Init_Port()
;    1155 {
_Init_Port:
;    1156  // Input/Output Ports initialization
;    1157  // Port A initialization
;    1158  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;    1159  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1160     PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;    1161     DDRA=0x00;
	OUT  0x1A,R30
;    1162 
;    1163  // Port B initialization
;    1164  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
;    1165  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
;    1166  // Use PORTB[7:4] as LED output, 0 -> LED ON
;    1167     PORTB=0x01;
	LDI  R30,LOW(1)
	OUT  0x18,R30
;    1168     DDRB=0xF1;
	LDI  R30,LOW(241)
	OUT  0x17,R30
;    1169 
;    1170  // Port C initialization
;    1171  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
;    1172  // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
;    1173     PORTC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1174     DDRC=0xFF;
	LDI  R30,LOW(255)
	OUT  0x14,R30
;    1175 
;    1176  // Port D initialization
;    1177  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
;    1178  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1179  // Use PORTD[7:6] as LED output, 0 -> LED ON
;    1180  // PORTD.0-1 are set as input of external interrupt 0/1 (see interrupt.c)
;    1181     PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1182     DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
;    1183 
;    1184  // Port E initialization
;    1185  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
;    1186  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
;    1187     PORTE=0x00;
	LDI  R30,LOW(0)
	OUT  0x3,R30
;    1188     DDRE=0x04;
	LDI  R30,LOW(4)
	OUT  0x2,R30
;    1189 
;    1190  // Port F initialization
;    1191  // Input
;    1192  // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
;    1193     PORTF=0x00;
	LDI  R30,LOW(0)
	STS  98,R30
;    1194     DDRF=0x00;
	STS  97,R30
;    1195 
;    1196  // Port G initialization
;    1197  // Func4=out Func3=out Func2=out Func1=out Func0=out
;    1198  // State4=0 State3=0 State2=0 State1=0 State0=0
;    1199     PORTG=0xFF;
	LDI  R30,LOW(255)
	STS  101,R30
;    1200     DDRG=0x1F;
	LDI  R30,LOW(31)
	STS  100,R30
;    1201 }
	RET
;    1202 
;    1203 /**************************************************************************************/
;    1204 //                              Initialize Timers
;    1205 /**************************************************************************************/
;    1206 void Init_Timers()
;    1207 {
;    1208  // Timer/Counter 0 initialization
;    1209  // Clock source: System Clock
;    1210  // Clock value: 14.400 kHz
;    1211  // Mode: Normal top=FFh
;    1212  // OC0 output: Disconnected
;    1213     ASSR=0x00;
;    1214     TCCR0=0x07;
;    1215     TCNT0=0x00;
;    1216     OCR0=0x00;
;    1217 
;    1218  // Timer/Counter 1 initialization
;    1219  // Clock source: System Clock
;    1220  // Clock value: 14.400 kHz
;    1221  // Mode: Normal top=FFFFh
;    1222  // OC1A output: Discon.
;    1223  // OC1B output: Discon.
;    1224  // OC1C output: Discon.
;    1225  // Noise Canceler: Off
;    1226  // Input Capture on Falling Edge
;    1227  // Timer 1 Overflow Interrupt: On
;    1228  // Input Capture Interrupt: Off
;    1229  // Compare A Match Interrupt: Off
;    1230  // Compare B Match Interrupt: Off
;    1231  // Compare C Match Interrupt: Off
;    1232     TCCR1A=0x00;
;    1233     TCCR1B=0x05;
;    1234     TCNT1H=0x00;
;    1235     TCNT1L=0x00;
;    1236     ICR1H=0x00;
;    1237     ICR1L=0x00;
;    1238     OCR1AH=0x00;
;    1239     OCR1AL=0x00;
;    1240     OCR1BH=0x00;
;    1241     OCR1BL=0x00;
;    1242     OCR1CH=0x00;
;    1243     OCR1CL=0x00;
;    1244 
;    1245  // Timer/Counter 2 initialization
;    1246  // Clock source: System Clock
;    1247  // Clock value: Timer 2 Stopped
;    1248  // Mode: Normal top=FFh
;    1249  // OC2 output: Disconnected
;    1250     TCCR2=0x00;
;    1251     TCNT2=0x00;
;    1252     OCR2=0x00;
;    1253 
;    1254  // Timer/Counter 3 initialization
;    1255  // Clock source: System Clock
;    1256  // Clock value: Timer 3 Stopped
;    1257  // Mode: Normal top=FFFFh
;    1258  // Noise Canceler: Off
;    1259  // Input Capture on Falling Edge
;    1260  // OC3A output: Discon.
;    1261  // OC3B output: Discon.
;    1262  // OC3C output: Discon.
;    1263  // Timer 3 Overflow Interrupt: Off
;    1264  // Input Capture Interrupt: Off
;    1265  // Compare A Match Interrupt: Off
;    1266  // Compare B Match Interrupt: Off
;    1267  // Compare C Match Interrupt: Off
;    1268     TCCR3A=0x00;
;    1269     TCCR3B=0x00;
;    1270     TCNT3H=0x00;
;    1271     TCNT3L=0x00;
;    1272     ICR3H=0x00;
;    1273     ICR3L=0x00;
;    1274     OCR3AH=0x00;
;    1275     OCR3AL=0x00;
;    1276     OCR3BH=0x00;
;    1277     OCR3BL=0x00;
;    1278     OCR3CH=0x00;
;    1279     OCR3CL=0x00;
;    1280 
;    1281  // Timer(s)/Counter(s) Interrupt(s) initialization
;    1282     TIMSK=0x05;
;    1283     ETIMSK=0x00;
;    1284 }
;    1285 
;    1286 /**************************************************************************************/
;    1287 //                              Enable External Memory
;    1288 /**************************************************************************************/
;    1289 void Enable_XMEM()
;    1290 {
_Enable_XMEM:
;    1291  // External SRAM page configuration:
;    1292  // 1100h - 7FFFh / 8000h - FFFFh
;    1293  // Lower page wait state(s): 2r/w +1 addr
;    1294  // Upper page wait state(s): 2r/w + 1 addr
;    1295     MCUCR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x35,R30
;    1296     //MCUCR = 0x80;
;    1297 
;    1298     XMCRA=0x4E;
	LDI  R30,LOW(78)
	STS  109,R30
;    1299     //XMCRA=0x0A;
;    1300  // PC7:0 can not be released
;    1301     XMCRB=0x80;
	LDI  R30,LOW(128)
	STS  108,R30
;    1302     // XMCRB = 0x07;
;    1303 }
	RET
;    1304 
;    1305 /**************************************************************************************/
;    1306 //                              Initialize External Interrupt
;    1307 /**************************************************************************************/
;    1308 void Init_Ex_Interrupt()
;    1309 {
;    1310  // External Interrupt(s) initialization
;    1311  // INT0: Off
;    1312  // INT1: Off
;    1313  // INT2: Off
;    1314  // INT3: Off
;    1315  // INT4: On
;    1316  // INT4 Mode: Rising Edge
;    1317  // INT5: On
;    1318  // INT5 Mode: Rising Edge
;    1319  // INT6: On
;    1320  // INT6 Mode: Rising Edge
;    1321  // INT7: On
;    1322  // INT7 Mode: Rising Edge
;    1323     EICRA=0x00;
;    1324     EICRB=0xFF;
;    1325     EIMSK=0xF0;
;    1326     EIFR=0xF0;
;    1327 }
;    1328 
;    1329 /**************************************************************************************/
;    1330 //                              Analog Comparator initialization
;    1331 /**************************************************************************************/
;    1332 void Init_ADC()
;    1333 {
;    1334  // Analog Comparator initialization
;    1335  // Analog Comparator: Off
;    1336  // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1337     ACSR=0x80;
;    1338     SFIOR=0x00;
;    1339 }
;    1340 #include "global.h"
;    1341 #include "wendu.h"
;    1342 #include "utili.h"
;    1343 #include "math.h"
;    1344 #define logl log
;    1345 #define powl pow
;    1346 
;    1347 double RValueToTValue(double r, u8 prbid)
;    1348 {
_RValueToTValue:
;    1349 
;    1350 	double ac,bc,cc,tlow,tup,rnew,tnew;
;    1351 	int count;
;    1352 
;    1353 	if(rprbdata.type[prbid] == PRBTYPE_PT100)
	SBIW R28,28
	ST   -Y,R17
	ST   -Y,R16
;	r -> Y+31
;	prbid -> Y+30
;	ac -> Y+26
;	bc -> Y+22
;	cc -> Y+18
;	tlow -> Y+14
;	tup -> Y+10
;	rnew -> Y+6
;	tnew -> Y+2
;	count -> R16,R17
	CALL SUBOPT_0x38
	CPI  R30,LOW(0xF1)
	BRNE _0x103
;    1354 		r = r/100.0;
	CALL SUBOPT_0x39
	__GETD1N 0x42C80000
	CALL SUBOPT_0x3A
;    1355 	else if(rprbdata.type[prbid] == PRBTYPE_PT25)
	RJMP _0x104
_0x103:
	CALL SUBOPT_0x38
	CPI  R30,LOW(0xF2)
	BRNE _0x105
;    1356 		r = r/25.0;
	CALL SUBOPT_0x39
	__GETD1N 0x41C80000
	CALL SUBOPT_0x3A
;    1357 	else
	RJMP _0x106
_0x105:
;    1358 		return -9999.999;
	__GETD1N 0xC61C3FFF
	RJMP _0x459
;    1359 
;    1360 	ac = rprbdata.param1[prbid];
_0x106:
_0x104:
	LDD  R30,Y+30
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL SUBOPT_0x3B
	CALL __EEPROMRDD
	__PUTD1S 26
;    1361 	bc = rprbdata.param2[prbid];
	__POINTW2MN _rprbdata,96
	LDD  R30,Y+30
	CALL SUBOPT_0x3B
	CALL __EEPROMRDD
	__PUTD1S 22
;    1362 	cc = rprbdata.param3[prbid];
	__POINTW2MN _rprbdata,192
	LDD  R30,Y+30
	CALL SUBOPT_0x3B
	CALL __EEPROMRDD
	__PUTD1S 18
;    1363 
;    1364 
;    1365 	//set the search range of T between GetT(r) +/- 1 degree
;    1366 	tlow = GetT(r) - 1;
	__GETD1S 31
	CALL __PUTPARD1
	RCALL _GetT
	CALL SUBOPT_0x3C
	CALL SUBOPT_0x3D
;    1367 	tup = tlow + 2;
	__GETD1S 14
	__GETD2N 0x40000000
	CALL __ADDF12
	CALL SUBOPT_0x3E
;    1368 
;    1369 	count = 0;
	__GETWRN 16,17,0
;    1370 
;    1371 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x107:
	CALL SUBOPT_0x3F
	CALL SUBOPT_0x40
	CALL SUBOPT_0x41
	BREQ PC+2
	BRCC PC+3
	JMP  _0x10A
	CALL SUBOPT_0x42
	BRLT _0x10B
_0x10A:
	RJMP _0x109
_0x10B:
;    1372 	{
;    1373 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x43
;    1374 		rnew = GetWr(tnew);
	RCALL _GetWr
	CALL SUBOPT_0x44
;    1375 		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
	CALL SUBOPT_0x45
	CALL SUBOPT_0x46
	__GETD2S 26
	CALL SUBOPT_0x47
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x48
	__GETD2S 22
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x48
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x48
	__GETD2S 18
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x48
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x48
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDF12
	CALL SUBOPT_0x49
;    1376 		if(r < rnew)
	CALL SUBOPT_0x39
	CALL __CMPF12
	BRSH _0x10C
;    1377 			tup = tnew;
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x3E
;    1378 		else
	RJMP _0x10D
_0x10C:
;    1379 			tlow = tnew;
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x3D
;    1380 	}
_0x10D:
	RJMP _0x107
_0x109:
;    1381 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x4B
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x4C
	CALL SUBOPT_0x4D
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x4F
_0x459:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,35
	RET
;    1382 
;    1383 
;    1384 //	return r+1;
;    1385 }
;    1386 
;    1387 flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
;    1388 flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
;    1389 flash u8 sizeAi = sizeof(Ai)/sizeof(double);
;    1390 flash u8 sizeCi = sizeof(Ci)/sizeof(double);
;    1391 
;    1392 double GetWr(double t)
;    1393 {
_GetWr:
;    1394 	double result;
;    1395 	double pert;
;    1396 	int i;
;    1397 	if(t < 0)
	CALL SUBOPT_0x50
;	t -> Y+10
;	result -> Y+6
;	pert -> Y+2
;	i -> R16,R17
	BRGE _0x10E
;    1398 	{
;    1399 		result = Ai[0];
	LDI  R30,LOW(_Ai*2)
	LDI  R31,HIGH(_Ai*2)
	CALL __GETD1PF
	CALL SUBOPT_0x44
;    1400 		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
	CALL SUBOPT_0x40
	__GETD2N 0x43889333
	CALL SUBOPT_0x4E
	__GETD1N 0x4388947B
	CALL __DIVF21
	CALL SUBOPT_0x51
	__GETD2N 0x3FC00000
	CALL SUBOPT_0x4E
	__GETD1N 0x3FC00000
	CALL SUBOPT_0x52
;    1401 		t = pert;
	CALL SUBOPT_0x53
;    1402 		for(i=1;i<sizeAi;i++)
_0x110:
	LDI  R30,LOW(_sizeAi*2)
	LDI  R31,HIGH(_sizeAi*2)
	LPM  R30,Z
	CALL SUBOPT_0x54
	BRGE _0x111
;    1403 		{
;    1404 			result = result + Ai[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ai*2)
	LDI  R27,HIGH(_Ai*2)
	CALL SUBOPT_0x55
	CALL SUBOPT_0x56
;    1405 			t = t*pert;
;    1406 		}
	__ADDWRN 16,17,1
	RJMP _0x110
_0x111:
;    1407 		result = exp(result);
	CALL SUBOPT_0x57
	CALL SUBOPT_0x58
	CALL SUBOPT_0x44
;    1408 	}else{
	RJMP _0x112
_0x10E:
;    1409 			 result = Ci[0];
	LDI  R30,LOW(_Ci*2)
	LDI  R31,HIGH(_Ci*2)
	CALL __GETD1PF
	CALL SUBOPT_0x44
;    1410 			 pert = t/481.0 - 1;
	CALL SUBOPT_0x4C
	__GETD1N 0x43F08000
	CALL __DIVF21
	CALL SUBOPT_0x3C
	CALL SUBOPT_0x59
;    1411 			 t = pert;
	CALL SUBOPT_0x53
;    1412 			 for(i=1;i<sizeCi;i++)
_0x114:
	LDI  R30,LOW(_sizeCi*2)
	LDI  R31,HIGH(_sizeCi*2)
	LPM  R30,Z
	CALL SUBOPT_0x54
	BRGE _0x115
;    1413 			 {
;    1414 				 result = result + Ci[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ci*2)
	LDI  R27,HIGH(_Ci*2)
	CALL SUBOPT_0x55
	CALL SUBOPT_0x56
;    1415 				 t = t*pert;
;    1416 			 }
	__ADDWRN 16,17,1
	RJMP _0x114
_0x115:
;    1417 	}
_0x112:
;    1418 	return result;
	CALL SUBOPT_0x57
	RJMP _0x458
;    1419 }
;    1420 flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
;    1421 flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
;    1422 flash u8 sizeDi = sizeof(Di)/sizeof(double);
;    1423 flash u8 sizeBi = sizeof(Bi)/sizeof(double);
;    1424 
;    1425 double GetT(double w)
;    1426 {
_GetT:
;    1427 	double perw;
;    1428 	double result;
;    1429 	int i;
;    1430 
;    1431 	if(w >= 0)
	CALL SUBOPT_0x50
;	w -> Y+10
;	perw -> Y+6
;	result -> Y+2
;	i -> R16,R17
	BRLT _0x116
;    1432 	{
;    1433 		perw = (w-2.64)/1.64;
	CALL SUBOPT_0x4C
	__GETD1N 0x4028F5C3
	CALL SUBOPT_0x1B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3FD1EB85
	CALL SUBOPT_0x5A
;    1434 		w = perw;
	CALL SUBOPT_0x3E
;    1435 		result = Di[0];
	LDI  R30,LOW(_Di*2)
	LDI  R31,HIGH(_Di*2)
	CALL SUBOPT_0x5B
;    1436 		for(i=1;i<sizeDi;i++)
_0x118:
	LDI  R30,LOW(_sizeDi*2)
	LDI  R31,HIGH(_sizeDi*2)
	LPM  R30,Z
	CALL SUBOPT_0x54
	BRGE _0x119
;    1437 		{
;    1438 			result = result + Di[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Di*2)
	LDI  R27,HIGH(_Di*2)
	CALL SUBOPT_0x5C
;    1439 			w = w*perw;
;    1440 		}
	__ADDWRN 16,17,1
	RJMP _0x118
_0x119:
;    1441 	}else{
	RJMP _0x11A
_0x116:
;    1442 		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
	CALL SUBOPT_0x40
	CALL __PUTPARD1
	__GETD1N 0x3E2AAAAB
	CALL __PUTPARD1
	CALL _pow
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F266666
	CALL SUBOPT_0x1B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3EB33333
	CALL SUBOPT_0x5A
;    1443 		w = perw;
	CALL SUBOPT_0x3E
;    1444 		result = Bi[0];
	LDI  R30,LOW(_Bi*2)
	LDI  R31,HIGH(_Bi*2)
	CALL SUBOPT_0x5B
;    1445 		for(i=1;i<sizeBi;i++)
_0x11C:
	LDI  R30,LOW(_sizeBi*2)
	LDI  R31,HIGH(_sizeBi*2)
	LPM  R30,Z
	CALL SUBOPT_0x54
	BRGE _0x11D
;    1446 		{
;    1447 			result = result + Bi[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Bi*2)
	LDI  R27,HIGH(_Bi*2)
	CALL SUBOPT_0x5C
;    1448 			w = w*perw;
;    1449 		}
	__ADDWRN 16,17,1
	RJMP _0x11C
_0x11D:
;    1450 		result = 273.15*result - 273.5;
	CALL SUBOPT_0x4A
	__GETD2N 0x43889333
	CALL SUBOPT_0x19
	__GETD1N 0x4388C000
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x59
;    1451 	}
_0x11A:
;    1452 	return result;
	CALL SUBOPT_0x4A
_0x458:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
;    1453 }
;    1454 
;    1455 flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
;    1456 flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};
;    1457 
;    1458 flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
;    1459 flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
;    1460 flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};
;    1461 
;    1462 flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};
;    1463 
;    1464 flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
;    1465 flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};
;    1466 
;    1467 flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
;    1468 flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};
;    1469 
;    1470 flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
;    1471 flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};
;    1472 
;    1473 flash double BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1474 flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};
;    1475 
;    1476 flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1477 flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};
;    1478 
;    1479 flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
;    1480 flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};
;    1481 
;    1482 flash int TLowLen = sizeof(TLow)/sizeof(double);
;    1483 flash int THighLen = sizeof(THigh)/sizeof(double);
;    1484 
;    1485 flash int SLowLen = sizeof(SLow)/sizeof(double);
;    1486 flash int SMedLen = sizeof(SMed)/sizeof(double);
;    1487 flash int SHighLen = sizeof(SHigh)/sizeof(double);
;    1488 
;    1489 flash int RLowLen = sizeof(RLow)/sizeof(double);
;    1490 flash int RMedLen = sizeof(RMed)/sizeof(double);
;    1491 flash int RHighLen = sizeof(RHigh)/sizeof(double);
;    1492 
;    1493 flash int NLowLen = sizeof(NLow)/sizeof(double);
;    1494 flash int NHighLen = sizeof(NHigh)/sizeof(double);
;    1495 
;    1496 flash int JLowLen = sizeof(JLow)/sizeof(double);
;    1497 flash int JHighLen = sizeof(JHigh)/sizeof(double);
;    1498 
;    1499 flash int BLowLen = sizeof(BLow)/sizeof(double);
;    1500 flash int BHighLen = sizeof(BHigh)/sizeof(double);
;    1501 
;    1502 flash int ELowLen = sizeof(ELow)/sizeof(double);
;    1503 flash int EHighLen = sizeof(EHigh)/sizeof(double);
;    1504 
;    1505 flash int KLowLen = sizeof(KLow)/sizeof(double);
;    1506 flash int KHighLen = sizeof(KHigh)/sizeof(double);
;    1507 
;    1508 flash double *coef;

	.DSEG
_coef:
	.BYTE 0x2
;    1509 
;    1510 double GetThmoVolt(double t,char type)
;    1511 {

	.CSEG
_GetThmoVolt:
;    1512 	double result,pert;
;    1513 	int i;
;    1514 	int len = 0;
;    1515 	switch(type)
	SBIW R28,8
	CALL __SAVELOCR4
;	t -> Y+13
;	type -> Y+12
;	result -> Y+8
;	pert -> Y+4
;	i -> R16,R17
;	len -> R18,R19
	LDI  R18,0
	LDI  R19,0
	LDD  R30,Y+12
;    1516 	{
;    1517 		case 'T':
	CPI  R30,LOW(0x54)
	BRNE _0x121
;    1518 			coef = (t < 0) ? TLow : THigh;
	CALL SUBOPT_0x5D
	BRGE _0x122
	LDI  R30,LOW(_TLow*2)
	LDI  R31,HIGH(_TLow*2)
	RJMP _0x123
_0x122:
	LDI  R30,LOW(_THigh*2)
	LDI  R31,HIGH(_THigh*2)
_0x123:
	CALL SUBOPT_0x5E
;    1519 			len = (t < 0) ? TLowLen : THighLen;
	BRGE _0x125
	LDI  R30,LOW(_TLowLen*2)
	LDI  R31,HIGH(_TLowLen*2)
	CALL __GETW1PF
	RJMP _0x126
_0x125:
	LDI  R30,LOW(_THighLen*2)
	LDI  R31,HIGH(_THighLen*2)
	CALL __GETW1PF
_0x126:
	MOVW R18,R30
;    1520 			break;
	RJMP _0x120
;    1521 		case 'K':
_0x121:
	CPI  R30,LOW(0x4B)
	BRNE _0x128
;    1522 			coef = (t < 0) ? KLow : KHigh;
	CALL SUBOPT_0x5D
	BRGE _0x129
	LDI  R30,LOW(_KLow*2)
	LDI  R31,HIGH(_KLow*2)
	RJMP _0x12A
_0x129:
	LDI  R30,LOW(_KHigh*2)
	LDI  R31,HIGH(_KHigh*2)
_0x12A:
	CALL SUBOPT_0x5E
;    1523 			len = (t < 0) ? KLowLen : KHighLen;
	BRGE _0x12C
	LDI  R30,LOW(_KLowLen*2)
	LDI  R31,HIGH(_KLowLen*2)
	CALL __GETW1PF
	RJMP _0x12D
_0x12C:
	LDI  R30,LOW(_KHighLen*2)
	LDI  R31,HIGH(_KHighLen*2)
	CALL __GETW1PF
_0x12D:
	MOVW R18,R30
;    1524 			break;
	RJMP _0x120
;    1525 		case 'N':
_0x128:
	CPI  R30,LOW(0x4E)
	BRNE _0x12F
;    1526 			coef = (t < 0) ? NLow : NHigh;
	CALL SUBOPT_0x5D
	BRGE _0x130
	LDI  R30,LOW(_NLow*2)
	LDI  R31,HIGH(_NLow*2)
	RJMP _0x131
_0x130:
	LDI  R30,LOW(_NHigh*2)
	LDI  R31,HIGH(_NHigh*2)
_0x131:
	CALL SUBOPT_0x5E
;    1527 			len = (t < 0) ? NLowLen : NHighLen;
	BRGE _0x133
	LDI  R30,LOW(_NLowLen*2)
	LDI  R31,HIGH(_NLowLen*2)
	CALL __GETW1PF
	RJMP _0x134
_0x133:
	LDI  R30,LOW(_NHighLen*2)
	LDI  R31,HIGH(_NHighLen*2)
	CALL __GETW1PF
_0x134:
	MOVW R18,R30
;    1528 			break;
	RJMP _0x120
;    1529 		case 'E':
_0x12F:
	CPI  R30,LOW(0x45)
	BRNE _0x136
;    1530 			coef = (t < 0) ? ELow : EHigh;
	CALL SUBOPT_0x5D
	BRGE _0x137
	LDI  R30,LOW(_ELow*2)
	LDI  R31,HIGH(_ELow*2)
	RJMP _0x138
_0x137:
	LDI  R30,LOW(_EHigh*2)
	LDI  R31,HIGH(_EHigh*2)
_0x138:
	CALL SUBOPT_0x5E
;    1531 			len = (t < 0) ? ELowLen : EHighLen;
	BRGE _0x13A
	LDI  R30,LOW(_ELowLen*2)
	LDI  R31,HIGH(_ELowLen*2)
	CALL __GETW1PF
	RJMP _0x13B
_0x13A:
	LDI  R30,LOW(_EHighLen*2)
	LDI  R31,HIGH(_EHighLen*2)
	CALL __GETW1PF
_0x13B:
	MOVW R18,R30
;    1532 			break;
	RJMP _0x120
;    1533 		case 'B':
_0x136:
	CPI  R30,LOW(0x42)
	BRNE _0x13D
;    1534 			coef = (t < 630.615) ? BLow : BHigh;
	CALL SUBOPT_0x5F
	BRSH _0x13E
	LDI  R30,LOW(_BLow*2)
	LDI  R31,HIGH(_BLow*2)
	RJMP _0x13F
_0x13E:
	LDI  R30,LOW(_BHigh*2)
	LDI  R31,HIGH(_BHigh*2)
_0x13F:
	CALL SUBOPT_0x60
;    1535 			len = (t < 630.615) ? BLowLen : BHighLen;
	CALL SUBOPT_0x5F
	BRSH _0x141
	LDI  R30,LOW(_BLowLen*2)
	LDI  R31,HIGH(_BLowLen*2)
	CALL __GETW1PF
	RJMP _0x142
_0x141:
	LDI  R30,LOW(_BHighLen*2)
	LDI  R31,HIGH(_BHighLen*2)
	CALL __GETW1PF
_0x142:
	MOVW R18,R30
;    1536 			break;
	RJMP _0x120
;    1537 		case 'J':
_0x13D:
	CPI  R30,LOW(0x4A)
	BRNE _0x144
;    1538 			coef = (t < 760) ? JLow : JHigh;
	CALL SUBOPT_0x61
	BRSH _0x145
	LDI  R30,LOW(_JLow*2)
	LDI  R31,HIGH(_JLow*2)
	RJMP _0x146
_0x145:
	LDI  R30,LOW(_JHigh*2)
	LDI  R31,HIGH(_JHigh*2)
_0x146:
	CALL SUBOPT_0x60
;    1539 			len = (t < 760) ? JLowLen : JHighLen;
	CALL SUBOPT_0x61
	BRSH _0x148
	LDI  R30,LOW(_JLowLen*2)
	LDI  R31,HIGH(_JLowLen*2)
	CALL __GETW1PF
	RJMP _0x149
_0x148:
	LDI  R30,LOW(_JHighLen*2)
	LDI  R31,HIGH(_JHighLen*2)
	CALL __GETW1PF
_0x149:
	MOVW R18,R30
;    1540 			break;
	RJMP _0x120
;    1541 
;    1542 		case 'S':
_0x144:
	CPI  R30,LOW(0x53)
	BRNE _0x14B
;    1543 			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
	CALL SUBOPT_0x62
	CALL SUBOPT_0x63
	BRSH _0x14C
	LDI  R30,LOW(_SLow*2)
	LDI  R31,HIGH(_SLow*2)
	RJMP _0x14D
_0x14C:
	CALL SUBOPT_0x64
	BRSH _0x14F
	LDI  R30,LOW(_SMed*2)
	LDI  R31,HIGH(_SMed*2)
	RJMP _0x150
_0x14F:
	LDI  R30,LOW(_SHigh*2)
	LDI  R31,HIGH(_SHigh*2)
_0x150:
_0x14D:
	CALL SUBOPT_0x65
;    1544 			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
	BRSH _0x152
	LDI  R30,LOW(_SLowLen*2)
	LDI  R31,HIGH(_SLowLen*2)
	CALL __GETW1PF
	RJMP _0x153
_0x152:
	CALL SUBOPT_0x64
	BRSH _0x155
	LDI  R30,LOW(_SMedLen*2)
	LDI  R31,HIGH(_SMedLen*2)
	CALL __GETW1PF
	RJMP _0x156
_0x155:
	LDI  R30,LOW(_SHighLen*2)
	LDI  R31,HIGH(_SHighLen*2)
	CALL __GETW1PF
_0x156:
_0x153:
	MOVW R18,R30
;    1545 			break;
	RJMP _0x120
;    1546 		case 'R':
_0x14B:
	CPI  R30,LOW(0x52)
	BRNE _0x165
;    1547 			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
	CALL SUBOPT_0x62
	CALL SUBOPT_0x63
	BRSH _0x159
	LDI  R30,LOW(_RLow*2)
	LDI  R31,HIGH(_RLow*2)
	RJMP _0x15A
_0x159:
	CALL SUBOPT_0x64
	BRSH _0x15C
	LDI  R30,LOW(_RMed*2)
	LDI  R31,HIGH(_RMed*2)
	RJMP _0x15D
_0x15C:
	LDI  R30,LOW(_RHigh*2)
	LDI  R31,HIGH(_RHigh*2)
_0x15D:
_0x15A:
	CALL SUBOPT_0x65
;    1548 			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
	BRSH _0x15F
	LDI  R30,LOW(_RLowLen*2)
	LDI  R31,HIGH(_RLowLen*2)
	CALL __GETW1PF
	RJMP _0x160
_0x15F:
	CALL SUBOPT_0x64
	BRSH _0x162
	LDI  R30,LOW(_RMedLen*2)
	LDI  R31,HIGH(_RMedLen*2)
	CALL __GETW1PF
	RJMP _0x163
_0x162:
	LDI  R30,LOW(_RHighLen*2)
	LDI  R31,HIGH(_RHighLen*2)
	CALL __GETW1PF
_0x163:
_0x160:
	MOVW R18,R30
;    1549 			break;
	RJMP _0x120
;    1550 		default:
_0x165:
;    1551 			return 0.0;
	CALL SUBOPT_0x66
	RJMP _0x457
;    1552 	}
_0x120:
;    1553 	if (len == 0)
	MOV  R0,R18
	OR   R0,R19
	BRNE _0x166
;    1554 		return 0.0;
	CALL SUBOPT_0x66
	RJMP _0x457
;    1555 
;    1556 	result = coef[0];
_0x166:
	LDS  R30,_coef
	LDS  R31,_coef+1
	CALL __GETD1PF
	CALL SUBOPT_0x67
;    1557 	if(type == 'K')
	LDD  R26,Y+12
	CPI  R26,LOW(0x4B)
	BRNE _0x167
;    1558 	{
;    1559 		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	CALL SUBOPT_0x68
	__GETD2N 0xB8F82F06
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x68
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x69
	__GETD2N 0x3DF2E34C
	CALL SUBOPT_0x6A
;    1560 	}
;    1561 
;    1562 	pert = t;
_0x167:
	__GETD1S 13
	CALL SUBOPT_0x6B
;    1563 	for(i=1;i< len ;i++)
	__GETWRN 16,17,1
_0x169:
	__CPWRR 16,17,18,19
	BRGE _0x16A
;    1564 	{
;    1565 		result = result + coef[i] * t;
	MOVW R30,R16
	LDS  R26,_coef
	LDS  R27,_coef+1
	CALL SUBOPT_0x6C
	CALL __GETD1PF
	CALL SUBOPT_0x62
	CALL SUBOPT_0x6A
;    1566 		t = t*pert;
	CALL SUBOPT_0x6D
	CALL SUBOPT_0x62
	CALL __MULF12
	__PUTD1S 13
;    1567 	}
	__ADDWRN 16,17,1
	RJMP _0x169
_0x16A:
;    1568 	return result;
	CALL SUBOPT_0x6E
_0x457:
	CALL __LOADLOCR4
	ADIW R28,17
	RET
;    1569 }
;    1570 
;    1571 double MValueToTValue(double r,char type)
;    1572 {
_MValueToTValue:
;    1573 	double tlow;
;    1574 	double tup;
;    1575 	double rnew;
;    1576 	double tnew;
;    1577 	int count = 0;
;    1578 	return r+1;
	SBIW R28,16
	ST   -Y,R17
	ST   -Y,R16
;	r -> Y+19
;	type -> Y+18
;	tlow -> Y+14
;	tup -> Y+10
;	rnew -> Y+6
;	tnew -> Y+2
;	count -> R16,R17
	LDI  R16,0
	LDI  R17,0
	__GETD1S 19
	CALL SUBOPT_0x6F
	RJMP _0x456
;    1579 	switch(type)
;    1580 	{
;    1581 		case 'T':
;    1582 			tlow =  -270;	tup = 400;
;    1583 			break;
;    1584 		case 'K':
;    1585 			tlow =  -270;	tup = 1372;
;    1586 			break;
;    1587 		case 'N':
;    1588 			tlow =  -270;	tup = 1300;
;    1589 			break;
;    1590 		case 'E':
;    1591 			tlow =  -270;	tup = 1000;
;    1592 			break;
;    1593 		case 'B':
_0x171:
	CPI  R30,LOW(0x42)
	BRNE _0x172
;    1594 			tlow =  0;	tup = 1280;
	__CLRD1S 14
	__GETD1N 0x44A00000
	CALL SUBOPT_0x3E
;    1595 			break;
	RJMP _0x16D
;    1596 		case 'J':
_0x172:
	CPI  R30,LOW(0x4A)
	BRNE _0x173
;    1597 			tlow =  -210;	tup = 1200;
	__GETD1N 0xC3520000
	CALL SUBOPT_0x3D
	__GETD1N 0x44960000
	CALL SUBOPT_0x3E
;    1598 			break;
	RJMP _0x16D
;    1599 		case 'S':
_0x173:
	CPI  R30,LOW(0x53)
	BRNE _0x174
;    1600 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x71
;    1601 			break;
	RJMP _0x16D
;    1602 		case 'R':
_0x174:
	CPI  R30,LOW(0x52)
	BRNE _0x176
;    1603 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x71
;    1604 			break;
	RJMP _0x16D
;    1605 		default:
_0x176:
;    1606 			return -9999.9999;
	__GETD1N 0xC61C4000
	RJMP _0x456
;    1607 	}
_0x16D:
;    1608 
;    1609 	while((tup - tlow > 0.00005) || (count++ < 100))
_0x177:
	CALL SUBOPT_0x3F
	CALL SUBOPT_0x40
	CALL SUBOPT_0x41
	BREQ PC+4
	BRCS PC+3
	JMP  _0x17A
	CALL SUBOPT_0x42
	BRGE _0x179
_0x17A:
;    1610 	{
;    1611 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x43
;    1612 		rnew = GetThmoVolt(tnew,type);
	LDD  R30,Y+22
	ST   -Y,R30
	CALL _GetThmoVolt
	CALL SUBOPT_0x49
;    1613 
;    1614 		if(r < rnew)
	__GETD2S 19
	CALL __CMPF12
	BRSH _0x17C
;    1615 			tup = tnew;
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x3E
;    1616 		else
	RJMP _0x17D
_0x17C:
;    1617 			tlow = tnew;
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x3D
;    1618 	}
_0x17D:
	RJMP _0x177
_0x179:
;    1619 
;    1620 	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x4B
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x4C
	CALL SUBOPT_0x4D
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x4F
_0x456:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,23
	RET
;    1621 }
;    1622 
;    1623 #include "font.h"
;    1624 
;    1625 #include <MATH.H>
;    1626 #include "typedef.h"
;    1627 #include "global.h"
;    1628 #include "utili.h"
;    1629 #include "key.h"
;    1630 extern uchar key;
;    1631 extern u8 lcd_buffer;
;    1632 /*
;    1633         Communication with LCD/KBD board,
;    1634         Incoming command format:
;    1635                 CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
;    1636                 key:            key pressed
;    1637         Out command format:
;    1638                 CMDO_DISPLAYBTN,LED_STATUS      update the led status
;    1639 */
;    1640 /**************************主程序********************************/
;    1641 void Key_Init()
;    1642 {
_Key_Init:
;    1643 	key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    1644 }
	RET
;    1645 
;    1646 #include <string.h>
;    1647 #include "lcd.h"
;    1648 #include "font.h"
;    1649 #include "global.h"
;    1650 #include "utili.h"
;    1651 
;    1652 
;    1653 
;    1654 extern uchar key;
;    1655 static uchar kbd_state;

	.DSEG
_kbd_state_G8:
	.BYTE 0x1
;    1656 void kbd_uart_push(uchar data)
;    1657 {

	.CSEG
_kbd_uart_push:
;    1658         if(data == STATUS_IDLE||
;	data -> Y+0
;    1659            data == STATUS_ERR||
;    1660            data == STATUS_DRAW||
;    1661            data == STATUS_DONE) //lcd acknowledge
	LD   R26,Y
	CPI  R26,LOW(0xF0)
	BREQ _0x17F
	CPI  R26,LOW(0xF1)
	BREQ _0x17F
	CPI  R26,LOW(0xF2)
	BREQ _0x17F
	CPI  R26,LOW(0xF3)
	BRNE _0x17E
_0x17F:
;    1662         {
;    1663                 kbd_state = data;
	LD   R30,Y
	STS  _kbd_state_G8,R30
;    1664                 return;
	RJMP _0x452
;    1665         }
;    1666         if(key == KEY_INVALID)
_0x17E:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE _0x181
;    1667                 key = data;
	LDD  R5,Y+0
;    1668 }
_0x181:
	RJMP _0x452
;    1669 static u8 lcdcmd[40];

	.DSEG
_lcdcmd_G8:
	.BYTE 0x28
;    1670 
;    1671 static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
_head_G8:
	.BYTE 0x3
;    1672 static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};
_tail_G8:
	.BYTE 0x3
;    1673 
;    1674 static u8 hex2ch[2];
_hex2ch_G8:
	.BYTE 0x2
;    1675 void hex2char(uchar a)
;    1676 {

	.CSEG
_hex2char:
;    1677         uchar h,l;
;    1678 	h = (a & 0xf0) >> 4;
	ST   -Y,R17
	ST   -Y,R16
;	a -> Y+2
;	h -> R17
;	l -> R16
	LDD  R30,Y+2
	ANDI R30,LOW(0xF0)
	SWAP R30
	ANDI R30,0xF
	MOV  R17,R30
;    1679 	l = (a & 0x0f);
	LDD  R30,Y+2
	ANDI R30,LOW(0xF)
	MOV  R16,R30
;    1680 	if(h <= 9)
	CPI  R17,10
	BRSH _0x184
;    1681 		hex2ch[0] = (h+'0');
	MOV  R30,R17
	SUBI R30,-LOW(48)
	RJMP _0x463
;    1682 	else
_0x184:
;    1683 		hex2ch[0] = (h+'A'-0x0a);
	MOV  R30,R17
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x463:
	STS  _hex2ch_G8,R30
;    1684 	if(l <= 9)
	CPI  R16,10
	BRSH _0x186
;    1685 		hex2ch[1] = (l+'0');
	MOV  R30,R16
	SUBI R30,-LOW(48)
	RJMP _0x464
;    1686 	else
_0x186:
;    1687 		hex2ch[1] = (l+'A'-0x0a);
	MOV  R30,R16
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x464:
	__PUTB1MN _hex2ch_G8,1
;    1688 }
	RJMP _0x455
;    1689 void onecmd(uchar len)
;    1690 {
_onecmd:
;    1691      uchar i = 0;
;    1692      uchar sum;
;    1693      sum = 0;
	ST   -Y,R17
	ST   -Y,R16
;	len -> Y+2
;	i -> R17
;	sum -> R16
	LDI  R17,0
	LDI  R16,LOW(0)
;    1694      prints(head,3,PORT_KBD); //send head
	LDI  R30,LOW(_head_G8)
	LDI  R31,HIGH(_head_G8)
	CALL SUBOPT_0x72
;    1695      for(i = 0;i<len;i++)     //caculate cksum
	LDI  R17,LOW(0)
_0x189:
	LDD  R30,Y+2
	CP   R17,R30
	BRSH _0x18A
;    1696      {
;    1697         hex2char(lcdcmd[i]);
	CALL SUBOPT_0x73
	CALL SUBOPT_0x74
;    1698 	prints(hex2ch,2,PORT_KBD);
;    1699         sum = sum + lcdcmd[i];
	CALL SUBOPT_0x73
	ADD  R16,R30
;    1700      }
	SUBI R17,-1
	RJMP _0x189
_0x18A:
;    1701      lcdcmd[i] = 0xff - sum;
	MOV  R26,R17
	LDI  R27,0
	SUBI R26,LOW(-_lcdcmd_G8)
	SBCI R27,HIGH(-_lcdcmd_G8)
	LDI  R30,LOW(255)
	SUB  R30,R16
	ST   X,R30
;    1702      hex2char(lcdcmd[i]);
	CALL SUBOPT_0x73
	CALL SUBOPT_0x74
;    1703      prints(hex2ch,2,PORT_KBD);
;    1704      prints(tail,3,PORT_KBD); //send tail
	LDI  R30,LOW(_tail_G8)
	LDI  R31,HIGH(_tail_G8)
	CALL SUBOPT_0x72
;    1705 }
_0x455:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,3
	RET
;    1706 static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

	.DSEG
_idle_G8:
	.BYTE 0xA
;    1707 void sendcmd(uchar len)
;    1708 {

	.CSEG
_sendcmd:
;    1709      ulong timeout;
;    1710      if(DEBUG == 1)
	SBIW R28,4
;	len -> Y+4
;	timeout -> Y+0
;    1711         return;
;    1712      while(kbd_state != STATUS_IDLE)
_0x18D:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x18F
;    1713      {
;    1714         if(kbd_state == STATUS_DRAW)
	CPI  R26,LOW(0xF2)
	BREQ _0x18D
;    1715                 continue;
;    1716         if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x191
;    1717         {
;    1718                 prints(idle,10,PORT_KBD);
	LDI  R30,LOW(_idle_G8)
	LDI  R31,HIGH(_idle_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _prints
;    1719                 kbd_state = STATUS_DRAW;
	LDI  R30,LOW(242)
	STS  _kbd_state_G8,R30
;    1720         }
;    1721      }
_0x191:
	RJMP _0x18D
_0x18F:
;    1722      onecmd(len);
	CALL SUBOPT_0x75
;    1723      timeout = 0;
;    1724      kbd_state = STATUS_DONE;
	LDI  R30,LOW(243)
	STS  _kbd_state_G8,R30
;    1725      while(kbd_state != STATUS_IDLE)
_0x192:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x194
;    1726      {
;    1727          if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x195
;    1728          {
;    1729                 onecmd(len);
	CALL SUBOPT_0x75
;    1730                 timeout = 0;
;    1731          }
;    1732          if(timeout < 655350)
_0x195:
	__GETD2S 0
	__CPD2N 0x9FFF6
	BRSH _0x196
;    1733          {
;    1734                 sleepms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x23
;    1735                 timeout++;
	__GETD1S 0
	__SUBD1N -1
	__PUTD1S 0
;    1736          }else{
	RJMP _0x197
_0x196:
;    1737                 onecmd(len);
	CALL SUBOPT_0x75
;    1738                 timeout = 0;
;    1739          }
_0x197:
;    1740      }
	RJMP _0x192
_0x194:
;    1741 }
_0x454:
	ADIW R28,5
	RET
;    1742 /*------------------初始化-----------------*/
;    1743 void LCD_Init (void)
;    1744 {
_LCD_Init:
;    1745         kbd_state = STATUS_IDLE;
	LDI  R30,LOW(240)
	STS  _kbd_state_G8,R30
;    1746         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x76
;    1747         sendcmd(1);
;    1748         //do it twice avoid the missing of first command on boot up
;    1749         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x76
;    1750         sendcmd(1);
;    1751 
;    1752 }
	RET
;    1753 
;    1754 /********************************************************/
;    1755 /* 清屏							*/
;    1756 /********************************************************/
;    1757 void LCD_Cls(void)
;    1758 {
_LCD_Cls:
;    1759         lcdcmd[0] = CMDO_LCD_CLS;
	LDI  R30,LOW(4)
	STS  _lcdcmd_G8,R30
;    1760         sendcmd(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _sendcmd
;    1761 }
	RET
;    1762 void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
;    1763 {
_LCD_ClsBlock:
;    1764         lcdcmd[0] = CMDO_LCD_CLSBLK;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(6)
	CALL SUBOPT_0x77
;    1765         lcdcmd[1] = x1;
;    1766         lcdcmd[2] = y1;
;    1767         lcdcmd[3] = x2;
;    1768         lcdcmd[4] = y2;
;    1769         sendcmd(5);
;    1770 }
	RJMP _0x453
;    1771 void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
;    1772 {
_display_buttons:
;    1773         lcdcmd[0] = CMDO_DISPLAYBTN;
;	btn_pos -> Y+1
;	btn_val -> Y+0
	LDI  R30,LOW(2)
	STS  _lcdcmd_G8,R30
;    1774         if(btn_val == 0)
	LD   R30,Y
	CPI  R30,0
	BRNE _0x198
;    1775 	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
	LDD  R30,Y+1
	SUBI R30,-LOW(4)
	STD  Y+1,R30
;    1776         lcdcmd[1] = btn_pos;
_0x198:
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,1
;    1777         sendcmd(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _sendcmd
;    1778 }
	ADIW R28,2
	RET
;    1779 
;    1780 
;    1781 /*	设定图形x,y值*/
;    1782 /*
;    1783 void LCD_GraphSetAddr(uchar x,uchar y)
;    1784 {
;    1785 	uint xy;
;    1786 	xy=y;
;    1787 	xy=xy*16+x+256;
;    1788   	//LCD_CE(0);
;    1789 	LCD_Write2(xy&0xff,xy/256,0x24);
;    1790 }
;    1791 */
;    1792 /*	设定文本x,y值	*/
;    1793 /*
;    1794 void LCD_TextSetAddr(uchar x,uchar y)
;    1795 {
;    1796   	//LCD_CE(0);
;    1797 	LCD_Write2(y*16+x,0,0x24);
;    1798 }
;    1799 */
;    1800 /*	清除一点*/
;    1801 /*
;    1802 void LCD_ClrPixel(uchar x,uchar y)
;    1803 {
;    1804 	uchar b;
;    1805 	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;
;    1806 
;    1807 	b = 7 - (x % 8);
;    1808 
;    1809 	LCD_WriteInt(addr,0x24);
;    1810 	LCD_Comm(0xf0|b);
;    1811 }
;    1812 */
;    1813 /*	点亮一点	*/
;    1814 /*
;    1815 void LCD_Putpixel(uchar x,uchar y)
;    1816 {
;    1817 	uchar b;
;    1818 	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;
;    1819 
;    1820 	b = 7 - (x % 8);
;    1821 
;    1822 	LCD_WriteInt(addr,0x24);
;    1823 	LCD_Comm(0xf8|b);
;    1824 }
;    1825 */
;    1826 /*	x,y处显示光标	*/
;    1827 /*
;    1828 void LCD_ShowCursor(uchar x,uchar y)
;    1829 {
;    1830 	return;
;    1831   	//LCD_CE(0);
;    1832 	LCD_Comm(0x97);	//光标开
;    1833 	LCD_Write2(x,y,0x21);
;    1834 }
;    1835 
;    1836 //	取消光标
;    1837 
;    1838 void LCD_HideCursor(void)
;    1839 {
;    1840 	return;
;    1841   	//LCD_CE(0);
;    1842 	LCD_Comm(0x9c);
;    1843 }
;    1844 */
;    1845 /*
;    1846 void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
;    1847 {
;    1848 	x =(x / 8) * 8;
;    1849 
;    1850 	while(*s)
;    1851 	{
;    1852 		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
;    1853 
;    1854 		x = x + 8;
;    1855 
;    1856 		s++;
;    1857 	}
;    1858 }
;    1859 
;    1860 
;    1861 void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
;    1862 {
;    1863 	x =(x / 8) * 8;
;    1864 	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
;    1865 }
;    1866 */
;    1867 
;    1868 void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
;    1869 {
_LCD_ReverseRect:
;    1870         lcdcmd[0] = CMDO_LCD_REVERSE;
;	x -> Y+3
;	y -> Y+2
;	w -> Y+1
;	h -> Y+0
	LDI  R30,LOW(5)
	CALL SUBOPT_0x77
;    1871         lcdcmd[1] = x;
;    1872         lcdcmd[2] = y;
;    1873         lcdcmd[3] = w;
;    1874         lcdcmd[4] = h;
;    1875         sendcmd(5);
;    1876 }
	RJMP _0x453
;    1877 /*--------------显示字符------------------*/
;    1878 /*
;    1879 void LCD_TextPutchar(uchar x,uchar y,uchar c)
;    1880 {
;    1881   	//LCD_CE(0);
;    1882   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    1883   	LCD_Comm(0xb0);
;    1884 	LCD_AutoWrite(c - 0x20);
;    1885   	LCD_Comm(0xb2);
;    1886 }
;    1887 
;    1888 void LCD_TextPrint(uchar x,uchar y,char *s)
;    1889 {
;    1890   	//LCD_CE(0);
;    1891   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    1892   	LCD_Comm(0xb0);
;    1893   	while(*s)
;    1894   	{
;    1895 		LCD_AutoWrite(*s - 0x20);
;    1896 		s++;
;    1897 	}
;    1898   	LCD_Comm(0xb2);
;    1899 }
;    1900 void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
;    1901 {
;    1902 	uchar buf[4];
;    1903 	int i;
;    1904 
;    1905 	if(start > 3) start = 0;
;    1906 
;    1907 	for(i = 0; i < 4; i++)
;    1908 	{
;    1909 		buf[i] = n % 10;
;    1910 		n /= 10;
;    1911 	}
;    1912 
;    1913 	for(i = 3-start; i >= 0; i--)
;    1914 	{
;    1915 		LCD_TextPutchar(x,y,'0' + buf[i]);
;    1916 		x ++;
;    1917 	}
;    1918 }
;    1919 
;    1920 void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
;    1921 {
;    1922 	uchar c1,c2;
;    1923 	//低4位
;    1924 	c1 = hex & 0x0f;	//lo
;    1925 	//高4位
;    1926 	c2 = (hex >> 4) & 0x0f; //hi
;    1927 
;    1928 	LCD_TextPutchar(x,y,HexTable[c2]);
;    1929 	LCD_TextPutchar(x+1,y,HexTable[c1]);
;    1930 }
;    1931 */
;    1932 /************************************************/
;    1933 /*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
;    1934 /************************************************/
;    1935 /*
;    1936 void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
;    1937 {
;    1938 	register uchar t;
;    1939 	int xerr=0,yerr=0,delta_x,delta_y,distance;
;    1940 	int incx,incy,uRow,uCol;
;    1941 
;    1942 	delta_x = xt-x0;				//计算坐标增量
;    1943 	delta_y = yt-y0;
;    1944 	uRow = x0;
;    1945 	uCol = y0;
;    1946 	if(delta_x>0) incx=1;				//设置单步方向
;    1947 	else if( delta_x==0 ) incx=0;			//垂直线
;    1948 	else {incx=-1;delta_x=-delta_x;}
;    1949 
;    1950 	if(delta_y>0) incy=1;
;    1951 	else if( delta_y==0 ) incy=0;			//水平线
;    1952 	else {incy=-1;delta_y=-delta_y;}
;    1953 
;    1954 	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
;    1955 	else distance=delta_y;
;    1956 
;    1957   	//LCD_CE(0);
;    1958 	for( t=0;t <= distance+1; t++ )
;    1959         {					//画线输出
;    1960 		LCD_Putpixel(uRow,uCol);			//画点
;    1961 		xerr +=	delta_x	;
;    1962 		yerr +=	delta_y	;
;    1963 
;    1964 		if( xerr > distance )
;    1965                	{
;    1966 			xerr-=distance;
;    1967 			uRow+=incx;
;    1968 		}
;    1969 		if( yerr > distance )
;    1970                	{
;    1971 			yerr-=distance;
;    1972 			uCol+=incy;
;    1973 		}
;    1974 	}
;    1975 }
;    1976 */
;    1977 /*
;    1978 void LCD_LineH(uchar y)
;    1979 {
;    1980 	char i;
;    1981 
;    1982   	//LCD_CE(0);
;    1983   	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
;    1984 	LCD_Comm(0xb0);
;    1985 	for(i=0;i<LCD_WIDTH;i++)
;    1986  		LCD_AutoWrite(0xff);
;    1987 	LCD_Comm(0xb2);
;    1988 }
;    1989 */
;    1990 /*
;    1991 void LCD_LineV(uchar x,uchar y1,uchar y2)
;    1992 {
;    1993 	int i;
;    1994   	//LCD_CE(0);
;    1995 	for(i = y1; i < y2; i++)
;    1996 	{
;    1997 		LCD_Putpixel(x,i);
;    1998 	}
;    1999 }
;    2000 */
;    2001 
;    2002 void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
;    2003 {
_LCD_Rectange:
;    2004         lcdcmd[0] = CMDO_LCD_RECT;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(7)
	CALL SUBOPT_0x77
;    2005         lcdcmd[1] = x1;
;    2006         lcdcmd[2] = y1;
;    2007         lcdcmd[3] = x2;
;    2008         lcdcmd[4] = y2;
;    2009         sendcmd(5);
;    2010 }
	RJMP _0x453
;    2011 void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
;    2012 {
_LCD_PrintChar:
;    2013         uchar pos = 0;
;    2014         lcdcmd[0] = cmd;
	ST   -Y,R17
;	cmd -> Y+5
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	pos -> R17
	LDI  R17,0
	LDD  R30,Y+5
	STS  _lcdcmd_G8,R30
;    2015         lcdcmd[1] = x;
	LDD  R30,Y+4
	__PUTB1MN _lcdcmd_G8,1
;    2016         lcdcmd[2] = y;
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,2
;    2017         pos = 1;
	LDI  R17,LOW(1)
;    2018         while(*s)
_0x199:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x19B
;    2019         {
;    2020                 lcdcmd[pos + 3] = *s++;
	CALL SUBOPT_0x78
	MOVW R0,R30
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOVW R26,R0
	ST   X,R30
;    2021                 pos = pos + 1;
	SUBI R17,-LOW(1)
;    2022         }
	RJMP _0x199
_0x19B:
;    2023         lcdcmd[pos+3] = 0x00;
	CALL SUBOPT_0x78
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2024         lcdcmd[3] = pos;
	__POINTW1MN _lcdcmd_G8,3
	ST   Z,R17
;    2025         pos = pos + 4;
	SUBI R17,-LOW(4)
;    2026         sendcmd(pos);
	ST   -Y,R17
	CALL _sendcmd
;    2027 }
	LDD  R17,Y+0
	ADIW R28,6
	RET
;    2028 void LCD_PrintHz12(uchar x,uchar y,uchar *s)
;    2029 {
_LCD_PrintHz12:
;    2030         LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(8)
	CALL SUBOPT_0x79
;    2031 }
	RJMP _0x453
;    2032 void LCD_PrintHz16(uchar x,uchar y,uchar *s)
;    2033 {
_LCD_PrintHz16:
;    2034         LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(9)
	CALL SUBOPT_0x79
;    2035 }
	RJMP _0x453
;    2036 //显示6x8的数字
;    2037 void LCD_Print6X8(uchar x, uchar y,uchar *s)
;    2038 {
_LCD_Print6X8:
;    2039         LCD_PrintChar(CMDO_LCD_68,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(16)
	CALL SUBOPT_0x79
;    2040 }
	RJMP _0x453
;    2041 
;    2042 //显示6x8的数字
;    2043 void LCD_Print8X16(uchar x, uchar y,uchar *s)
;    2044 {
_LCD_Print8X16:
;    2045         LCD_PrintChar(CMDO_LCD_816,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(17)
	CALL SUBOPT_0x79
;    2046 }
_0x453:
	ADIW R28,4
	RET
;    2047 
;    2048 //显示24x32的数字
;    2049 /*
;    2050 void LCD_Print24X32(uchar x, uchar y,uchar *s)
;    2051 {
;    2052 	//x坐标必需是8位对齐
;    2053 	x =(x / 8) * 8;
;    2054 	while(*s)
;    2055 	{
;    2056 		if( *s >= '0' && *s <= '9')	//显示
;    2057 		{
;    2058 			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
;    2059 			x += 24;
;    2060 		}
;    2061 		else if( *s == ' ')
;    2062 		{
;    2063 			x += 8;
;    2064 		}
;    2065 		else if( *s == '-')
;    2066 		{
;    2067 			LCD_PutImg(x,y,3,32,Font24X32[12]);
;    2068 			x += 24;
;    2069 		}
;    2070 		else
;    2071 		{
;    2072 			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
;    2073 			x += 8;
;    2074 		}
;    2075 		s ++;
;    2076 	}
;    2077 }*/
;    2078 #include "utili.h"
;    2079 #include "scanner.h"
;    2080 #include "stdlib.h"
;    2081 
;    2082 static uchar navlen = 0;

	.DSEG
_navlen_G9:
	.BYTE 0x1
;    2083 static double reading = -1000;
_reading_G9:
	.BYTE 0x4
;    2084 static char navread[20];
_navread_G9:
	.BYTE 0x14
;    2085 static char navread2[20];
_navread2_G9:
	.BYTE 0x14
;    2086 u8 scancmd[5];
_scancmd:
	.BYTE 0x5
;    2087 void scanner_set_mode()
;    2088 {

	.CSEG
_scanner_set_mode:
;    2089         if (IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x19D
;    2090         {
;    2091             scancmd[0] = '#';
	LDI  R30,LOW(35)
	RJMP _0x465
;    2092         }else{
_0x19D:
;    2093             scancmd[0]= '!';
	LDI  R30,LOW(33)
_0x465:
	STS  _scancmd,R30
;    2094         }
;    2095         scancmd[1] = 0x0D;
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2096         scancmd[2] = 0x0A;
	__POINTW1MN _scancmd,2
	CALL SUBOPT_0x7A
;    2097         prints(scancmd,3,PORT_SCANNER);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x7B
;    2098 }
	RET
;    2099 void scanner_set_channel(uchar ch)
;    2100 {
_scanner_set_channel:
;    2101         if(ch < 10)
;	ch -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRSH _0x19F
;    2102         {
;    2103                 scancmd[0] = (ch + '0');
	LD   R30,Y
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2104                 scancmd[1] = 'A';
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    2105                 scancmd[2] = 0x0D;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2106                 scancmd[3] = 0x0A;
	__POINTW1MN _scancmd,3
	CALL SUBOPT_0x7A
;    2107                 prints(scancmd,4,PORT_SCANNER);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x7B
;    2108                 return;
	RJMP _0x452
;    2109         }
;    2110 
;    2111         scancmd[0] = (u8)(ch / 10) + '0';
_0x19F:
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2112         ch = ch % 10;
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __MODB21U
	ST   Y,R30
;    2113         scancmd[1] = ch + '0';
	SUBI R30,-LOW(48)
	__PUTB1MN _scancmd,1
;    2114         scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(65)
	STD  Z+0,R26
	__POINTW1MN _scancmd,3
	LDI  R26,LOW(13)
	STD  Z+0,R26
	__POINTW1MN _scancmd,4
	CALL SUBOPT_0x7A
;    2115         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x7B
;    2116 }
	RJMP _0x452
;    2117 
;    2118 //incoming data handler of scanner
;    2119 void scanner_uart_push(uchar data)
;    2120 {
_scanner_uart_push:
;    2121 }
	RJMP _0x452
;    2122 //incoming data hander of PC
;    2123 void pc_uart_push(uchar data)
;    2124 {
_pc_uart_push:
;    2125 }
	RJMP _0x452
;    2126 static uchar navcmd[12];

	.DSEG
_navcmd_G9:
	.BYTE 0xC
;    2127 #define NAV_INVALID     -1000
;    2128 #define NAV_VALID       -2000
;    2129 #define RESETNAV       navlen = 0;   reading = NAV_INVALID;
;    2130 
;    2131 void nav_command(uchar cmd)
;    2132 {

	.CSEG
_nav_command:
;    2133         if(cmd == NAV_30V)
;	cmd -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1A0
;    2134                sprintf(navcmd,"%%01;12;02\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,78
	CALL SUBOPT_0x25
;    2135         if(cmd == NAV_1V)
_0x1A0:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x1A1
;    2136                sprintf(navcmd,"%%01;12;01\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,90
	CALL SUBOPT_0x25
;    2137         if(cmd == NAV_120MV)
_0x1A1:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x1A2
;    2138                sprintf(navcmd,"%%01;12;00\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,102
	CALL SUBOPT_0x25
;    2139         if(cmd == NAV_AFLTOFF)
_0x1A2:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x1A3
;    2140                sprintf(navcmd,"%%01;26\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,114
	CALL SUBOPT_0x25
;    2141         if(cmd == NAV_AFLTON)
_0x1A3:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x1A4
;    2142                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,123
	CALL SUBOPT_0x25
;    2143         if(cmd == NAV_SLOWMODE)
_0x1A4:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x1A5
;    2144                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,123
	CALL SUBOPT_0x25
;    2145         if(cmd == NAV_ZEROON)
_0x1A5:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x1A6
;    2146                sprintf(navcmd,"%%01:06\r");
	CALL SUBOPT_0x7C
	__POINTW1FN _0,132
	CALL SUBOPT_0x25
;    2147         if(cmd == NAV_INIT)
_0x1A6:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1A7
;    2148         {
;    2149                 RESETNAV;
	CALL SUBOPT_0x7D
;    2150                 sprintf(navcmd,"%%01;00\r");
	__POINTW1FN _0,141
	CALL SUBOPT_0x25
;    2151         }
;    2152         if(cmd == NAV_READ)
_0x1A7:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0x1A8
;    2153         {
;    2154                 RESETNAV;
	CALL SUBOPT_0x7D
;    2155                 sprintf(navcmd,"%%01;01\r");
	__POINTW1FN _0,150
	CALL SUBOPT_0x25
;    2156         }
;    2157 //                com1_putc('?');
;    2158         prints(navcmd,strlen(navcmd),PORT_NAV);
_0x1A8:
	CALL SUBOPT_0x7C
	CALL SUBOPT_0x7C
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _prints
;    2159 }
	RJMP _0x452
;    2160 
;    2161 
;    2162 unsigned long navtime;

	.DSEG
_navtime:
	.BYTE 0x4
;    2163 double nav_read()
;    2164 {

	.CSEG
_nav_read:
;    2165         if(DEBUG == 1)
;    2166                 return 1.0;
;    2167         nav_command(NAV_READ);
	CALL SUBOPT_0x7E
;    2168         navtime = 0;
;    2169         while(1)
_0x1AA:
;    2170         {
;    2171                 if(reading < NAV_INVALID) //make sure it is a valid reading
	LDS  R26,_reading_G9
	LDS  R27,_reading_G9+1
	LDS  R24,_reading_G9+2
	LDS  R25,_reading_G9+3
	CALL SUBOPT_0x7F
	CALL __CMPF12
	BRSH _0x1AD
;    2172                 {
;    2173                      reading = atof(navread2);
	LDI  R30,LOW(_navread2_G9)
	LDI  R31,HIGH(_navread2_G9)
	ST   -Y,R31
	ST   -Y,R30
	CALL _atof
	CALL SUBOPT_0x80
;    2174                      return reading;
	LDS  R30,_reading_G9
	LDS  R31,_reading_G9+1
	LDS  R22,_reading_G9+2
	LDS  R23,_reading_G9+3
	RET
;    2175                 }
;    2176                 if(navtime++ > 655350)
_0x1AD:
	LDS  R30,_navtime
	LDS  R31,_navtime+1
	LDS  R22,_navtime+2
	LDS  R23,_navtime+3
	__SUBD1N -1
	STS  _navtime,R30
	STS  _navtime+1,R31
	STS  _navtime+2,R22
	STS  _navtime+3,R23
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	__CPD1N 0x9FFF7
	BRLO _0x1AE
;    2177                 {
;    2178                      nav_command(NAV_READ);
	CALL SUBOPT_0x7E
;    2179                      navtime = 0;
;    2180                 }
;    2181                 sleepms(1);
_0x1AE:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x23
;    2182         }
	RJMP _0x1AA
;    2183         return 1.0;
;    2184 }
;    2185 //incoming data hander of navameter
;    2186 void nav_uart_push(uchar data)
;    2187 {
_nav_uart_push:
;    2188         if(navlen >= 19)
;	data -> Y+0
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x13)
	BRLO _0x1AF
;    2189         {
;    2190                 RESETNAV;
	CALL SUBOPT_0x82
;    2191         }
;    2192         if(((data >= '0') && (data <= '9')) ||
_0x1AF:
;    2193                 (data == '.') ||
;    2194                 (data == '-') ||
;    2195                 (data == '+') ||
;    2196                 (data == 'e') ||
;    2197                 (data == 'E') )
	LD   R26,Y
	CPI  R26,LOW(0x30)
	BRLO _0x1B1
	CPI  R26,LOW(0x3A)
	BRLO _0x1B3
_0x1B1:
	LD   R26,Y
	CPI  R26,LOW(0x2E)
	BREQ _0x1B3
	CPI  R26,LOW(0x2D)
	BREQ _0x1B3
	CPI  R26,LOW(0x2B)
	BREQ _0x1B3
	CPI  R26,LOW(0x65)
	BREQ _0x1B3
	CPI  R26,LOW(0x45)
	BRNE _0x1B0
_0x1B3:
;    2198         {
;    2199                 navread[navlen++] = data;
	LDS  R30,_navlen_G9
	SUBI R30,-LOW(1)
	STS  _navlen_G9,R30
	CALL SUBOPT_0x34
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	LD   R26,Y
	STD  Z+0,R26
;    2200                 reading = NAV_INVALID;
	CALL SUBOPT_0x7F
	CALL SUBOPT_0x80
;    2201                 return;
	RJMP _0x452
;    2202         }
;    2203 
;    2204         if(navlen < 4) //not enough digits
_0x1B0:
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x4)
	BRSH _0x1B5
;    2205         {
;    2206                 RESETNAV;
	CALL SUBOPT_0x82
;    2207                 return;
	RJMP _0x452
;    2208         }
;    2209         navread[navlen] = '\0';
_0x1B5:
	CALL SUBOPT_0x83
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2210         do
_0x1B7:
;    2211         {
;    2212                 navread2[navlen] = navread[navlen];
	LDS  R26,_navlen_G9
	LDI  R27,0
	SUBI R26,LOW(-_navread2_G9)
	SBCI R27,HIGH(-_navread2_G9)
	CALL SUBOPT_0x83
	LD   R30,Z
	ST   X,R30
;    2213         }while(navlen-- > 0);
	LDS  R26,_navlen_G9
	SUBI R26,LOW(1)
	STS  _navlen_G9,R26
	SUBI R26,-LOW(1)
	CPI  R26,LOW(0x1)
	BRSH _0x1B7
;    2214         navlen = 0;
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
;    2215         reading =  NAV_VALID; //valid data
	__GETD1N 0xC4FA0000
	CALL SUBOPT_0x80
;    2216 }
_0x452:
	ADIW R28,1
	RET
;    2217 #include <mega64.h>
;    2218 #include "lcd.h"
;    2219 #include "key.h"
;    2220 #include "utili.h"
;    2221 #include "eeprom.h"
;    2222 #include "window.h"
;    2223 #include "utili.h"
;    2224 #include "scanner.h"
;    2225 
;    2226 extern uchar nextwin;
;    2227 #define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);draw_label(&PRE##chvalue,SW_NORMAL);\
;    2228                 		if(new_page == 1){\
;    2229 		                	for(i = min_option; i <= max_option; i++){\
;    2230 				                j = ((curr_pos-1)/max_option)*max_option + i;
;    2231 
;    2232 #define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
;    2233 				        if((curr_pos-1) % max_option == (i-1))\
;    2234 					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2235         			        }\
;    2236                                 }else{\
;    2237 	                		for(i = min_option; i <= max_option; i++) {\
;    2238         		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2239 				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2240 			                }\
;    2241 		                }
;    2242 
;    2243 #define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
;    2244 			if(curr_pos > max_index) curr_pos = min_index;\
;    2245 			if(last_pos == curr_pos)	return;\
;    2246 			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}
;    2247 
;    2248 #define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
;    2249 			if(curr_pos < min_index) curr_pos = max_index;\
;    2250 			if(last_pos == curr_pos)  return;\
;    2251 			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}
;    2252 
;    2253 #define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
;    2254 					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
;    2255 
;    2256 #define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
;    2257                                         draw_label(&PRE##options[i-1], SW_NORMAL);\
;    2258 			        }
;    2259 #define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
;    2260                                         return;\
;    2261         			for(i = min_option;i <= max_option; i++){\
;    2262 	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2263         				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2264         			}
;    2265 
;    2266 #define INC_OPTION	last_sel = curr_sel;\
;    2267 			curr_sel++;\
;    2268 			if(curr_sel > max_option)\
;    2269 				curr_sel = min_option;
;    2270 
;    2271 #define DEC_OPTION	last_sel = curr_sel;\
;    2272 			curr_sel--;\
;    2273 			if(curr_sel < min_option)\
;    2274 				curr_sel = max_option;
;    2275 
;    2276 extern int   curr_ch;	//index of current channel
;    2277 extern int   curr_prb;	//index of current probe selection
;    2278 
;    2279 
;    2280 void State_Init() {
_State_Init:
;    2281 	display_buttons(KEY_BTN1,sysdata.prbmode);
	LDI  R30,LOW(97)
	ST   -Y,R30
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2282 	display_buttons(KEY_BTN2,sysdata.kttmode);
	LDI  R30,LOW(98)
	ST   -Y,R30
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2283 	display_buttons(KEY_BTN3,0); //probe type
	LDI  R30,LOW(99)
	CALL SUBOPT_0x14
;    2284 	display_buttons(KEY_BTN4,0); //zero
	LDI  R30,LOW(100)
	CALL SUBOPT_0x14
;    2285 }
	RET
;    2286 char* getprbtype(uchar);
;    2287 LABEL flash pgmain_chs[] = {
;    2288 		{LBL_HZ12, 4, 3,3,strbuf},
;    2289 		{LBL_HZ6X8,2,30,3,strbuf},
;    2290 		{LBL_HZ6X8,2,40,3,strbuf},
;    2291 		{LBL_HZ6X8,2,50,3,strbuf}
;    2292 	};
;    2293 LABEL flash pgmain_values[] = {
;    2294 		{LBL_HZ12, 40,3,  7,strbuf},
;    2295 		{LBL_HZ6X8,40,30,10,strbuf},
;    2296 		{LBL_HZ6X8,40,40,10,strbuf},
;    2297 		{LBL_HZ6X8,40,50,10,strbuf}
;    2298 	};
;    2299 LABEL flash pgmain_temps[] = {
;    2300 		{LBL_HZ12, 140,3,  7,strbuf},
;    2301 		{LBL_HZ6X8,140,30,10,strbuf},
;    2302 		{LBL_HZ6X8,140,40,10,strbuf},
;    2303 		{LBL_HZ6X8,140,50,10,strbuf}
;    2304 };
;    2305 /*      Main display      */
;    2306 void pgmain_handler(uchar msg) {
_pgmain_handler:
;    2307         uchar i,old_ch;
;    2308                 uchar *cptr;
;    2309 	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
;    2310 
;    2311         //Menu
;    2312 	if(msg == KEY_TAB) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	old_ch -> R16
;	*cptr -> R18,R19
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BRNE _0x1B9
;    2313 		if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x1BA
;    2314 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x466
;    2315 		else
_0x1BA:
;    2316 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x466:
	MOV  R4,R30
;    2317 		return;
	RJMP _0x450
;    2318 	}
;    2319 	if(msg == MSG_INIT) {
_0x1B9:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x1BC
;    2320 		LCD_Cls();
	CALL _LCD_Cls
;    2321 		old_ch = curr_ch;
	CALL SUBOPT_0x84
;    2322 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1BE:
	CPI  R17,4
	BRSH _0x1BF
;    2323 		{
;    2324 			sprintf(strbuf,"CH%02i:",curr_ch);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x85
;    2325 			draw_label(&pgmain_chs[i],SW_NORMAL);
;    2326 			INC_CH;
	BRGE _0x1C0
	CALL SUBOPT_0x29
_0x1C0:
;    2327 		}
	SUBI R17,-1
	RJMP _0x1BE
_0x1BF:
;    2328 		curr_ch = old_ch;
	CALL SUBOPT_0x86
;    2329 		msg = MSG_REFRESH;
;    2330 	}
;    2331 	if(msg == KEY_UP) {
_0x1BC:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x1C1
;    2332 		DEC_CH;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	__SUBWRR 6,7,30,31
	MOV  R0,R6
	OR   R0,R7
	BRNE _0x1C2
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	MOVW R6,R30
_0x1C2:
;    2333 		old_ch = curr_ch;
	CALL SUBOPT_0x84
;    2334 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1C4:
	CPI  R17,4
	BRSH _0x1C5
;    2335 		{
;    2336 			sprintf(strbuf,"CH%02i:",curr_ch);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x85
;    2337 			draw_label(&pgmain_chs[i],SW_NORMAL);
;    2338 			INC_CH;
	BRGE _0x1C6
	CALL SUBOPT_0x29
_0x1C6:
;    2339 		}
	SUBI R17,-1
	RJMP _0x1C4
_0x1C5:
;    2340 		curr_ch = old_ch;
	CALL SUBOPT_0x86
;    2341 		msg = MSG_REFRESH;
;    2342 	}
;    2343 	if(msg == KEY_DN) {
_0x1C1:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x1C7
;    2344 
;    2345 		INC_CH;
	CALL SUBOPT_0x87
	BRGE _0x1C8
	CALL SUBOPT_0x29
_0x1C8:
;    2346 		old_ch = curr_ch;
	CALL SUBOPT_0x84
;    2347 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1CA:
	CPI  R17,4
	BRSH _0x1CB
;    2348 		{
;    2349 			sprintf(strbuf,"CH%02i:",curr_ch);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x85
;    2350 			draw_label(&pgmain_chs[i],SW_NORMAL);
;    2351 			INC_CH;
	BRGE _0x1CC
	CALL SUBOPT_0x29
_0x1CC:
;    2352 		}
	SUBI R17,-1
	RJMP _0x1CA
_0x1CB:
;    2353 		curr_ch = old_ch;
	CALL SUBOPT_0x86
;    2354 		msg = MSG_REFRESH;
;    2355 	}
;    2356 	if(msg == MSG_REFRESH) {
_0x1C7:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x1CD
;    2357 		old_ch = curr_ch;
	CALL SUBOPT_0x84
;    2358 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1CF:
	CPI  R17,4
	BRLO PC+3
	JMP _0x1D0
;    2359 		{
;    2360 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BREQ PC+3
	JMP _0x1D1
;    2361 		        {
;    2362 			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_ch-1]]);
	CALL SUBOPT_0x88
	__POINTW2MN _tprbdata,480
	CALL SUBOPT_0x89
;    2363         			if((sysdata.tid[curr_ch - 1] == INVALID_PROBE) ||\
;    2364         				(rundata.temperature[curr_ch - 1] < -9000) ||\
;    2365 	        			(cptr[0] == 'P')){
	CALL SUBOPT_0x88
	CPI  R30,LOW(0xFF)
	BREQ _0x1D3
	CALL SUBOPT_0x8A
	CALL SUBOPT_0x1C
	CALL SUBOPT_0x8B
	BRLO _0x1D3
	MOVW R26,R18
	LD   R26,X
	CPI  R26,LOW(0x50)
	BRNE _0x1D2
_0x1D3:
;    2366         				sprintf(strbuf," -------");
	CALL SUBOPT_0x1D
	__POINTW1FN _0,167
	CALL SUBOPT_0x25
;    2367 	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8C
;    2368 		        		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8D
;    2369 			        }else{
	RJMP _0x1D5
_0x1D2:
;    2370 					sprintf(strbuf,"%8f",rundata.reading[curr_ch - 1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x8E
	CALL SUBOPT_0x8F
;    2371 					sprintf(strbuf+8," mV");
	__POINTW1FN _0,180
	CALL SUBOPT_0x25
;    2372 					draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8C
;    2373 				        sprintf(strbuf," %8f",rundata.temperature[curr_ch - 1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x90
	CALL SUBOPT_0xA
	CALL SUBOPT_0x8F
;    2374         				sprintf(strbuf+8," C %c",cptr[0]);
	CALL SUBOPT_0x91
;    2375 	        			draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
;    2376 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0x92
	BRNE _0x1D6
;    2377         				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
	SBIW R30,2
	ADIW R30,10
	RJMP _0x467
;    2378         				else                                     //Hz12
_0x1D6:
;    2379                 				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
	SBIW R30,2
	ADIW R30,16
_0x467:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2380 	                        }
_0x1D5:
;    2381 			}else{
	RJMP _0x1D8
_0x1D1:
;    2382        			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_ch-1]]);
	CALL SUBOPT_0x95
	__POINTW2MN _rprbdata,480
	CALL SUBOPT_0x89
;    2383         			if((sysdata.rid[curr_ch - 1] == INVALID_PROBE) ||\
;    2384         				(rundata.temperature[curr_ch - 1] < -9000) ||\
;    2385 	        			(cptr[0] != 'P'))
	CALL SUBOPT_0x95
	CPI  R30,LOW(0xFF)
	BREQ _0x1DA
	CALL SUBOPT_0x8A
	CALL SUBOPT_0x1C
	CALL SUBOPT_0x8B
	BRLO _0x1DA
	MOVW R26,R18
	LD   R26,X
	CPI  R26,LOW(0x50)
	BREQ _0x1D9
_0x1DA:
;    2386 	        		{
;    2387         				sprintf(strbuf," -------");
	CALL SUBOPT_0x1D
	__POINTW1FN _0,167
	CALL SUBOPT_0x25
;    2388 	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8C
;    2389 		        		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8D
;    2390 			        }else{
	RJMP _0x1DC
_0x1D9:
;    2391 					sprintf(strbuf,"%8f",rundata.reading[curr_ch - 1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x8E
	CALL SUBOPT_0x8F
;    2392 					sprintf(strbuf+8," ohm");
	__POINTW1FN _0,195
	CALL SUBOPT_0x25
;    2393 	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x8C
;    2394 		        		sprintf(strbuf," %8f",rundata.temperature[curr_ch - 1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x90
	CALL SUBOPT_0xA
	CALL SUBOPT_0x8F
;    2395 			        	sprintf(strbuf+8," C %c",cptr[0]);
	CALL SUBOPT_0x91
;    2396 				        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
;    2397 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0x92
	BRNE _0x1DD
;    2398         				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
	SBIW R30,2
	ADIW R30,10
	RJMP _0x468
;    2399         				else                                     //Hz12
_0x1DD:
;    2400                 				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
	SBIW R30,2
	ADIW R30,16
_0x468:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2401         			}
_0x1DC:
;    2402 	                }
_0x1D8:
;    2403 			INC_CH;
	CALL SUBOPT_0x87
	BRGE _0x1DF
	CALL SUBOPT_0x29
_0x1DF:
;    2404 		}
	SUBI R17,-1
	RJMP _0x1CF
_0x1D0:
;    2405 		curr_ch = old_ch;
	MOV  R6,R16
	CLR  R7
;    2406 	}
;    2407 }
_0x1CD:
	RJMP _0x450
;    2408 
;    2409 //main configuration window of R
;    2410 LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
;    2411 LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
;    2412 LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
;    2413 LABEL flash pgr_options[] = {
;    2414 	{LBL_HZ16, 10,23,6,"1.电流换向"},
;    2415 	{LBL_HZ16,130,23,7,"2.内标准校准"},
;    2416 	{LBL_HZ16, 10,45,8,"3.通道探头选择"},
;    2417 	{LBL_HZ16,130,45,7,"4.设置探头参数"}
;    2418 };
;    2419 
;    2420 //LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2421 #define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
;    2422 void pgcalibrate()
;    2423 {
_pgcalibrate:
;    2424         double oldvalue = 0;
;    2425         uint cnt;
;    2426         window_setup(10);
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x1E6*2)
	LDI  R31,HIGH(_0x1E6*2)
	CALL __INITLOCB
	ST   -Y,R17
	ST   -Y,R16
;	oldvalue -> Y+2
;	cnt -> R16,R17
	CALL SUBOPT_0x24
;    2427 	sprintf(strbuf,"请输入外标准(CH1)阻值");
	__POINTW1FN _0,285
	CALL SUBOPT_0x25
;    2428 	oldvalue = wnd_floatinput(oldvalue);
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x26
	CALL SUBOPT_0x96
;    2429 	if(oldvalue == 0)
	CALL __CPD10
	BRNE _0x1E7
;    2430                 return;
	RJMP _0x451
;    2431         wnd_msgbox(&pgr_calibrate);
_0x1E7:
	LDI  R30,LOW(_pgr_calibrate*2)
	LDI  R31,HIGH(_pgr_calibrate*2)
	CALL SUBOPT_0x22
;    2432         sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.Rx+sysdata.R0);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x1A
	CALL SUBOPT_0x97
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x18
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x1A
	__GETD2MN _rundata,192
	CALL __ADDF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    2433 }
_0x451:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    2434 //main menu of bore config
;    2435 void pgrconfig_handler(uchar msg) {
_pgrconfig_handler:
;    2436 	uchar i;
;    2437 
;    2438 	static uchar curr_sel = 1;

	.DSEG

	.CSEG
;    2439 	static uchar last_sel = 1;

	.DSEG
_last_sel_S45:
	.BYTE 0x1

	.CSEG
;    2440 	uchar min_option = 1;
;    2441 	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);
;    2442 
;    2443 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x1EB
	CPI  R26,LOW(0x43)
	BREQ _0x1EB
	CPI  R26,LOW(0x4F)
	BRNE _0x1EA
_0x1EB:
;    2444 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2445 		return;
	RJMP _0x450
;    2446 	}
;    2447 	if(msg == MSG_INIT) {
_0x1EA:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x1ED
;    2448 		LCD_Cls();
	CALL _LCD_Cls
;    2449 		draw_label(&pgr_banner, SW_NORMAL);
	LDI  R30,LOW(_pgr_banner*2)
	LDI  R31,HIGH(_pgr_banner*2)
	CALL SUBOPT_0x98
;    2450 		INIT_OPTIONS(pgr_);
	MOV  R17,R16
_0x1EF:
	CP   R19,R17
	BRLO _0x1F0
	CALL SUBOPT_0x99
	CALL SUBOPT_0x98
	SUBI R17,-1
	RJMP _0x1EF
_0x1F0:
;    2451 		sprintf(strbuf,"(%i)",sysdata.ktime);
	CALL SUBOPT_0x1D
	__POINTW1FN _0,307
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	CALL SUBOPT_0x9A
;    2452 		draw_label(&pgr_klbl, SW_NORMAL);
	LDI  R30,LOW(_pgr_klbl*2)
	LDI  R31,HIGH(_pgr_klbl*2)
	CALL SUBOPT_0x98
;    2453 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2454 	}
;    2455 	if(msg == KEY_NUM1) {
_0x1ED:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x1F1
;    2456 		window_setup(4); //4 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0x9B
;    2457 		sprintf(strbuf,"电流换向时间设置");
	__POINTW1FN _0,312
	CALL SUBOPT_0x25
;    2458 		sysdata.ktime = wnd_intinput(sysdata.ktime);
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDB
	ST   -Y,R30
	RCALL _wnd_intinput
	__POINTW2MN _sysdata,12
	LDI  R31,0
	CALL __EEPROMWRW
;    2459 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2460 		return;
	RJMP _0x450
;    2461 	}
;    2462 	if(msg == KEY_NUM2)
_0x1F1:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x1F2
;    2463 	{
;    2464                 pgcalibrate();
	CALL _pgcalibrate
;    2465 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2466 		return;
	RJMP _0x450
;    2467 	}
;    2468 	if(msg == KEY_NUM3) {
_0x1F2:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x1F3
;    2469 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2470 		return;
	RJMP _0x450
;    2471 	}
;    2472 	if(msg == KEY_NUM4) {
_0x1F3:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x1F4
;    2473 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2474 		return;
	RJMP _0x450
;    2475 	}
;    2476 	if(msg == MSG_REFRESH) {
_0x1F4:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x1F5
;    2477 		REFRESH_OPTIONS(pgr_);
	LDS  R26,_last_sel_S45
	CP   R12,R26
	BRNE _0x1F6
	RJMP _0x450
_0x1F6:
	MOV  R17,R16
_0x1F8:
	CP   R19,R17
	BRLO _0x1F9
	LDS  R26,_last_sel_S45
	CP   R17,R26
	BRNE _0x1FA
	CALL SUBOPT_0x99
	CALL SUBOPT_0x9C
_0x1FA:
	CP   R17,R12
	BRNE _0x1FB
	CALL SUBOPT_0x99
	CALL SUBOPT_0x9C
_0x1FB:
	SUBI R17,-1
	RJMP _0x1F8
_0x1F9:
;    2478 	}
;    2479 }
_0x1F5:
	RJMP _0x450
;    2480 //main configuration window of T
;    2481 LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
;    2482 LABEL flash pgt_options[] = {
;    2483 	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
;    2484 	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
;    2485 };
;    2486 
;    2487 //main menu of thermo config
;    2488 void pgtconfig_handler(uchar msg) {
_pgtconfig_handler:
;    2489 	uchar i;
;    2490 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S46:
	.BYTE 0x1

	.CSEG
;    2491 	static uchar last_sel = 1;

	.DSEG
_last_sel_S46:
	.BYTE 0x1

	.CSEG
;    2492 	uchar min_option = 1;
;    2493 	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
;    2494 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,2
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x202
	CPI  R26,LOW(0x43)
	BREQ _0x202
	CPI  R26,LOW(0x4F)
	BRNE _0x201
_0x202:
;    2495 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2496 		return;
	RJMP _0x450
;    2497 	}
;    2498 	if(msg == MSG_INIT) {
_0x201:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x204
;    2499 		LCD_Cls();
	CALL _LCD_Cls
;    2500 		draw_label(&pgt_banner, SW_NORMAL);
	LDI  R30,LOW(_pgt_banner*2)
	LDI  R31,HIGH(_pgt_banner*2)
	CALL SUBOPT_0x98
;    2501 		INIT_OPTIONS(pgt_);
	MOV  R17,R16
_0x206:
	CP   R19,R17
	BRLO _0x207
	CALL SUBOPT_0x9D
	CALL SUBOPT_0x98
	SUBI R17,-1
	RJMP _0x206
_0x207:
;    2502 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2503 	}
;    2504 	if(msg == KEY_NUM1) {
_0x204:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x208
;    2505 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2506 		return;
	RJMP _0x450
;    2507 	}
;    2508 	if(msg == KEY_NUM2) {
_0x208:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x209
;    2509 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2510 		return;
	RJMP _0x450
;    2511 	}
;    2512 	if(msg == MSG_REFRESH) {
_0x209:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x20A
;    2513 		REFRESH_OPTIONS(pgt_);
	LDS  R30,_curr_sel_S46
	LDS  R26,_last_sel_S46
	CP   R30,R26
	BRNE _0x20B
	RJMP _0x450
_0x20B:
	MOV  R17,R16
_0x20D:
	CP   R19,R17
	BRLO _0x20E
	LDS  R26,_last_sel_S46
	CP   R17,R26
	BRNE _0x20F
	CALL SUBOPT_0x9D
	CALL SUBOPT_0x9C
_0x20F:
	LDS  R26,_curr_sel_S46
	CP   R17,R26
	BRNE _0x210
	CALL SUBOPT_0x9D
	CALL SUBOPT_0x9C
_0x210:
	SUBI R17,-1
	RJMP _0x20D
_0x20E:
;    2514 	}
;    2515 	return;
_0x20A:
_0x450:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2516 }
;    2517 //channel probe setup
;    2518 LABEL flash pgch_banner =    {LBL_HZ16,3,3,4,"通道设置"};
;    2519 LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
;    2520 LABEL flash pgch_items[] = {
;    2521 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2522 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2523 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2524 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2525 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2526 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2527 };
;    2528 
;    2529 //select the channel to config
;    2530 void pgchset_handler(uchar msg) {
_pgchset_handler:
;    2531 	static uint curr_pos = 0;

	.DSEG
_curr_pos_S47:
	.BYTE 0x2

	.CSEG
;    2532 	static uint last_pos = 0;

	.DSEG
_last_pos_S47:
	.BYTE 0x2

	.CSEG
;    2533 	uchar min_option = 1;
;    2534 	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
;    2535 	uchar min_index = 1;
;    2536 	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
;    2537 	uchar new_page = 0;
;    2538 	int i,j;
;    2539 	if(msg == KEY_TAB || msg == KEY_CE) {
	SBIW R28,4
	CALL SUBOPT_0x9E
;	msg -> Y+10
;	min_option -> R17
;	max_option -> R16
;	min_index -> R19
;	max_index -> R18
;	new_page -> R21
;	i -> Y+8
;	j -> Y+6
	LDI  R21,0
	LDD  R26,Y+10
	CPI  R26,LOW(0x54)
	BREQ _0x213
	CPI  R26,LOW(0x43)
	BRNE _0x212
_0x213:
;    2540 		if(IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x215
;    2541 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	RJMP _0x469
;    2542 		else
_0x215:
;    2543 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
_0x469:
	MOV  R4,R30
;    2544 		return;
	RJMP _0x44F
;    2545 	}
;    2546 
;    2547 	if(msg == KEY_UP) {
_0x212:
	LDD  R26,Y+10
	CPI  R26,LOW(0x55)
	BRNE _0x217
;    2548 		DEC_TABLE;
	CALL SUBOPT_0x9F
	CALL SUBOPT_0xA0
	SBIW R30,1
	CALL SUBOPT_0xA1
	MOV  R30,R19
	CALL SUBOPT_0xA2
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x218
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA1
_0x218:
	CALL SUBOPT_0xA4
	BRNE _0x219
	RJMP _0x44F
_0x219:
	MOV  R30,R16
	CALL SUBOPT_0xA2
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x21A
	LDI  R21,LOW(1)
	RJMP _0x21B
_0x21A:
	LDI  R21,LOW(0)
_0x21B:
;    2549 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2550 	}
;    2551 	if(msg == KEY_DN) {
_0x217:
	LDD  R26,Y+10
	CPI  R26,LOW(0x44)
	BRNE _0x21C
;    2552 		INC_TABLE;
	CALL SUBOPT_0x9F
	CALL SUBOPT_0xA0
	ADIW R30,1
	CALL SUBOPT_0xA1
	MOV  R30,R18
	CALL SUBOPT_0xA2
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x21D
	CALL SUBOPT_0xA5
	CALL SUBOPT_0xA1
_0x21D:
	CALL SUBOPT_0xA4
	BRNE _0x21E
	RJMP _0x44F
_0x21E:
	MOV  R30,R16
	CALL SUBOPT_0xA2
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x21F
	LDI  R21,LOW(1)
	RJMP _0x220
_0x21F:
	LDI  R21,LOW(0)
_0x220:
;    2553 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2554 	}
;    2555 	if(msg == KEY_OK) {	//show window to select the probe
_0x21C:
	LDD  R26,Y+10
	CPI  R26,LOW(0x4F)
	BRNE _0x221
;    2556 		curr_ch = curr_pos;
	__GETWRMN 6,7,0,_curr_pos_S47
;    2557 		nextwin = PG_PRBLIST;
	LDI  R30,LOW(8)
	MOV  R4,R30
;    2558 		return;
	RJMP _0x44F
;    2559 	}
;    2560 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x221:
	LDD  R26,Y+10
	CPI  R26,LOW(0x30)
	BRLO _0x223
	CPI  R26,LOW(0x3A)
	BRLO _0x224
_0x223:
	RJMP _0x222
_0x224:
;    2561 		KEY_TABLE;
	LDD  R26,Y+10
	CPI  R26,LOW(0x31)
	BRLO _0x226
	CALL SUBOPT_0xA6
	BRSH _0x227
_0x226:
	RJMP _0x225
_0x227:
	LDI  R21,LOW(1)
	LDD  R26,Y+10
	CALL SUBOPT_0xA7
	CALL SUBOPT_0xA1
_0x225:
;    2562 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2563 	}
;    2564 	if(msg == KEY_DOT)
_0x222:
	LDD  R26,Y+10
	CPI  R26,LOW(0x2E)
	BRNE _0x228
;    2565 	{
;    2566 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x229
;    2567 	        {
;    2568 		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x46A
;    2569 	        }else{
_0x229:
;    2570        		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x46A:
	LDS  R30,_curr_pos_S47
	LDS  R31,_curr_pos_S47+1
	CALL SUBOPT_0xA8
;    2571 	        }
;    2572 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2573 	}
;    2574 	if(msg == MSG_INIT) {
_0x228:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFF)
	BRNE _0x22B
;    2575 		LCD_Cls();
	CALL _LCD_Cls
;    2576 		draw_label(&pgch_banner, SW_NORMAL);
	LDI  R30,LOW(_pgch_banner*2)
	LDI  R31,HIGH(_pgch_banner*2)
	CALL SUBOPT_0x98
;    2577 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xA1
;    2578 		new_page = 1;
	LDI  R21,LOW(1)
;    2579 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2580 	}
;    2581 	if(msg == MSG_REFRESH) {
_0x22B:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x22C
;    2582 
;    2583 		REFRESH_TABLES1(pgch_);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xA9
	CALL SUBOPT_0x9F
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	LDI  R30,LOW(_pgch_chvalue*2)
	LDI  R31,HIGH(_pgch_chvalue*2)
	CALL SUBOPT_0x98
	CPI  R21,1
	BREQ PC+3
	JMP _0x22D
	CALL SUBOPT_0xAC
_0x22F:
	CALL SUBOPT_0xAD
	BRGE PC+3
	JMP _0x230
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xAF
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADD  R30,R26
	ADC  R31,R27
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2584 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x231
;    2585         	        {
;    2586 				if(sysdata.tid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xB0
	BRNE _0x232
;    2587 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xB1
;    2588 				}else{
	RJMP _0x233
_0x232:
;    2589 			        	//draw line item j
;    2590 				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xB2
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xB3
	CALL _tname2b
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xB4
;    2591 				}
_0x233:
;    2592 		        }else{
	RJMP _0x234
_0x231:
;    2593 				if(sysdata.rid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xB0
	BRNE _0x235
;    2594 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xB1
;    2595 				}else{
	RJMP _0x236
_0x235:
;    2596 			        	//draw line item j
;    2597 				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xB2
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xB3
	CALL _rname2b
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xB4
;    2598 				}
_0x236:
;    2599 		        }
_0x234:
;    2600 		REFRESH_TABLES2(pgch_);
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x98
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB6
	BRNE _0x237
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x9C
_0x237:
	CALL SUBOPT_0xB7
	RJMP _0x22F
_0x230:
	RJMP _0x238
_0x22D:
	CALL SUBOPT_0xAC
_0x23A:
	CALL SUBOPT_0xAD
	BRLT _0x23B
	LDS  R26,_last_pos_S47
	LDS  R27,_last_pos_S47+1
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB6
	BRNE _0x23C
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x9C
_0x23C:
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB6
	BRNE _0x23D
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x9C
_0x23D:
	CALL SUBOPT_0xB7
	RJMP _0x23A
_0x23B:
_0x238:
;    2601 	}
;    2602 	return;
_0x22C:
_0x44F:
	CALL __LOADLOCR6
	ADIW R28,11
	RET
;    2603 }
;    2604 //select probe to edit.
;    2605 LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
;    2606 LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2607 LABEL flash pgprbset_items[] = {
;    2608 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2609 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2610 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2611 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2612 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2613 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2614 };
;    2615 //select probe to config
;    2616 void pgprbset_handler(uchar msg) {
_pgprbset_handler:
;    2617 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2618 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S48:
	.BYTE 0x2

	.CSEG
;    2619 	static uint last_pos = 1;

	.DSEG
_last_pos_S48:
	.BYTE 0x2

	.CSEG
;    2620 	uchar min_option = 1;
;    2621 	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
;    2622 	uchar min_index = 1;
;    2623 	uchar max_index = MAX_PROBE_NUM;
;    2624 	int i,j;
;    2625 	uchar new_page = 1;
;    2626 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,3
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x241*2)
	LDI  R31,HIGH(_0x241*2)
	CALL __INITLOCB
	CALL SUBOPT_0x9E
;	msg -> Y+9
;	min_option -> R17
;	max_option -> R16
;	min_index -> R19
;	max_index -> R18
;	i -> R20,R21
;	j -> Y+7
;	new_page -> Y+6
	LDD  R26,Y+9
	CPI  R26,LOW(0x30)
	BRLO _0x243
	CPI  R26,LOW(0x3A)
	BRLO _0x244
_0x243:
	RJMP _0x242
_0x244:
;    2627 		KEY_TABLE;
	LDD  R26,Y+9
	CPI  R26,LOW(0x31)
	BRLO _0x246
	CALL SUBOPT_0xA6
	BRSH _0x247
_0x246:
	RJMP _0x245
_0x247:
	LDI  R30,LOW(1)
	STD  Y+6,R30
	LDD  R26,Y+9
	CALL SUBOPT_0xA7
	CALL SUBOPT_0xB9
_0x245:
;    2628 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2629 	}
;    2630 	if(msg == KEY_CE || msg == KEY_TAB)
_0x242:
	LDD  R26,Y+9
	CPI  R26,LOW(0x43)
	BREQ _0x249
	CPI  R26,LOW(0x54)
	BRNE _0x248
_0x249:
;    2631 	{
;    2632 		if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x24B
;    2633 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x46B
;    2634 		else
_0x24B:
;    2635 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x46B:
	MOV  R4,R30
;    2636 		return;
	RJMP _0x44E
;    2637 	}
;    2638 	if(msg == KEY_OK)
_0x248:
	LDD  R26,Y+9
	CPI  R26,LOW(0x4F)
	BRNE _0x24D
;    2639 	{
;    2640 		curr_prb = curr_pos;
	__GETWRMN 8,9,0,_curr_pos_S48
;    2641 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2642 		return;
	RJMP _0x44E
;    2643 	}
;    2644 	if(msg == MSG_INIT)
_0x24D:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFF)
	BRNE _0x24E
;    2645 	{
;    2646 		LCD_Cls();
	CALL _LCD_Cls
;    2647 		draw_label(&pgprbset_banner, SW_NORMAL);
	LDI  R30,LOW(_pgprbset_banner*2)
	LDI  R31,HIGH(_pgprbset_banner*2)
	CALL SUBOPT_0x98
;    2648 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xB9
;    2649 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2650 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2651 	}
;    2652 	if(msg == KEY_UP) {
_0x24E:
	LDD  R26,Y+9
	CPI  R26,LOW(0x55)
	BRNE _0x24F
;    2653 		DEC_TABLE;
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xBB
	SBIW R30,1
	CALL SUBOPT_0xB9
	MOV  R30,R19
	CALL SUBOPT_0xBC
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x250
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xB9
_0x250:
	CALL SUBOPT_0xBD
	BRNE _0x251
	RJMP _0x44E
_0x251:
	MOV  R30,R16
	CALL SUBOPT_0xBC
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x252
	LDI  R30,LOW(1)
	RJMP _0x46C
_0x252:
	LDI  R30,LOW(0)
_0x46C:
	STD  Y+6,R30
;    2654 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2655 	}
;    2656 	if(msg == KEY_DN) {
_0x24F:
	LDD  R26,Y+9
	CPI  R26,LOW(0x44)
	BRNE _0x254
;    2657 		INC_TABLE;
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xBB
	ADIW R30,1
	CALL SUBOPT_0xB9
	MOV  R30,R18
	CALL SUBOPT_0xBC
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x255
	CALL SUBOPT_0xA5
	CALL SUBOPT_0xB9
_0x255:
	CALL SUBOPT_0xBD
	BRNE _0x256
	RJMP _0x44E
_0x256:
	MOV  R30,R16
	CALL SUBOPT_0xBC
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x257
	LDI  R30,LOW(1)
	RJMP _0x46D
_0x257:
	LDI  R30,LOW(0)
_0x46D:
	STD  Y+6,R30
;    2658 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2659 	}
;    2660 	if(msg == MSG_REFRESH) {
_0x254:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x259
;    2661 		REFRESH_TABLES1(pgprbset_);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xA9
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	LDI  R30,LOW(_pgprbset_chvalue*2)
	LDI  R31,HIGH(_pgprbset_chvalue*2)
	CALL SUBOPT_0x98
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x25A
	MOV  R20,R17
	CLR  R21
_0x25C:
	CALL SUBOPT_0xBE
	BRLT _0x25D
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xAF
	ADD  R30,R20
	ADC  R31,R21
	STD  Y+7,R30
	STD  Y+7+1,R31
;    2662 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x25E
;    2663 		        {
;    2664 			        sprintf(strbuf,"%i.%s",i,tname2b(j-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xC0
	CALL _tname2b
	RJMP _0x46E
;    2665 		        }else{
_0x25E:
;    2666         		        sprintf(strbuf,"%i.%s",i,rname2b(j-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xC0
	CALL _rname2b
_0x46E:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	CALL SUBOPT_0xB4
;    2667 		        }
;    2668         		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xC1
;    2669 		REFRESH_TABLES2(pgprbset_);
	CALL SUBOPT_0xC1
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xC2
	BRNE _0x260
	CALL SUBOPT_0xC3
_0x260:
	__ADDWRN 20,21,1
	RJMP _0x25C
_0x25D:
	RJMP _0x261
_0x25A:
	MOV  R20,R17
	CLR  R21
_0x263:
	CALL SUBOPT_0xBE
	BRLT _0x264
	LDS  R26,_last_pos_S48
	LDS  R27,_last_pos_S48+1
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xC2
	BRNE _0x265
	CALL SUBOPT_0xC3
_0x265:
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xC2
	BRNE _0x266
	CALL SUBOPT_0xC3
_0x266:
	__ADDWRN 20,21,1
	RJMP _0x263
_0x264:
_0x261:
;    2670 	}
;    2671 	return;
_0x259:
_0x44E:
	CALL __LOADLOCR6
	ADIW R28,10
	RET
;    2672 }
;    2673 //select probe for channel setup
;    2674 LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2675 LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2676 LABEL flash prblist_items[] = {
;    2677 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2678 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2679 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2680 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2681 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2682 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2683 };
;    2684 //select channel's probe
;    2685 void pgprblist_handler(uchar msg) {
_pgprblist_handler:
;    2686 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2687 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S49:
	.BYTE 0x2

	.CSEG
;    2688 	static uint last_pos = 1;

	.DSEG
_last_pos_S49:
	.BYTE 0x2

	.CSEG
;    2689 	uchar min_option = 1;
;    2690 	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
;    2691 	uchar min_index = 1;
;    2692 	uchar max_index = MAX_PROBE_NUM;
;    2693 	uchar i,j;
;    2694 	uchar new_page = 1;
;    2695 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,1
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x269*2)
	LDI  R31,HIGH(_0x269*2)
	CALL __INITLOCB
	CALL SUBOPT_0x9E
;	msg -> Y+7
;	min_option -> R17
;	max_option -> R16
;	min_index -> R19
;	max_index -> R18
;	i -> R21
;	j -> R20
;	new_page -> Y+6
	LDD  R26,Y+7
	CPI  R26,LOW(0x30)
	BRLO _0x26B
	CPI  R26,LOW(0x3A)
	BRLO _0x26C
_0x26B:
	RJMP _0x26A
_0x26C:
;    2696 		KEY_TABLE;
	LDD  R26,Y+7
	CPI  R26,LOW(0x31)
	BRLO _0x26E
	CALL SUBOPT_0xA6
	BRSH _0x26F
_0x26E:
	RJMP _0x26D
_0x26F:
	LDI  R30,LOW(1)
	STD  Y+6,R30
	LDD  R26,Y+7
	CALL SUBOPT_0xA7
	CALL SUBOPT_0xC4
_0x26D:
;    2697 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2698 	}
;    2699 	if(msg == KEY_CE)
_0x26A:
	LDD  R26,Y+7
	CPI  R26,LOW(0x43)
	BRNE _0x270
;    2700 	{
;    2701 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2702 		return;
	RJMP _0x44D
;    2703 	}
;    2704 	if(msg == KEY_TAB || msg == KEY_OK)
_0x270:
	LDD  R26,Y+7
	CPI  R26,LOW(0x54)
	BREQ _0x272
	CPI  R26,LOW(0x4F)
	BRNE _0x271
_0x272:
;    2705 	{
;    2706 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x274
;    2707 	        {
;    2708 		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,14
	RJMP _0x46F
;    2709 	        }else{
_0x274:
;    2710 		        sysdata.rid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,38
_0x46F:
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL SUBOPT_0xC5
	SBIW R30,1
	CALL __EEPROMWRB
;    2711 	        }
;    2712 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2713 		return;
	RJMP _0x44D
;    2714 	}
;    2715 	if(msg == MSG_INIT)
_0x271:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFF)
	BRNE _0x276
;    2716 	{
;    2717 		LCD_Cls();
	CALL _LCD_Cls
;    2718 		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
	CALL SUBOPT_0x1D
	__POINTW1FN _0,430
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL SUBOPT_0x9A
;    2719 		draw_label(&prblist_banner, SW_NORMAL);
	LDI  R30,LOW(_prblist_banner*2)
	LDI  R31,HIGH(_prblist_banner*2)
	CALL SUBOPT_0x98
;    2720 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xC4
;    2721 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2722 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2723 	}
;    2724 	if(msg == KEY_UP) {
_0x276:
	LDD  R26,Y+7
	CPI  R26,LOW(0x55)
	BRNE _0x277
;    2725 		DEC_TABLE;
	CALL SUBOPT_0xC6
	SBIW R30,1
	CALL SUBOPT_0xC4
	MOV  R30,R19
	CALL SUBOPT_0xC7
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x278
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xC4
_0x278:
	CALL SUBOPT_0xC8
	BRNE _0x279
	RJMP _0x44D
_0x279:
	MOV  R30,R16
	CALL SUBOPT_0xC7
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x27A
	LDI  R30,LOW(1)
	RJMP _0x470
_0x27A:
	LDI  R30,LOW(0)
_0x470:
	STD  Y+6,R30
;    2726 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2727 	}
;    2728 	if(msg == KEY_DN) {
_0x277:
	LDD  R26,Y+7
	CPI  R26,LOW(0x44)
	BRNE _0x27C
;    2729 		INC_TABLE;
	CALL SUBOPT_0xC6
	ADIW R30,1
	CALL SUBOPT_0xC4
	MOV  R30,R18
	CALL SUBOPT_0xC7
	CP   R30,R26
	CPC  R31,R27
	BRSH _0x27D
	CALL SUBOPT_0xA5
	CALL SUBOPT_0xC4
_0x27D:
	CALL SUBOPT_0xC8
	BRNE _0x27E
	RJMP _0x44D
_0x27E:
	MOV  R30,R16
	CALL SUBOPT_0xC7
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x27F
	LDI  R30,LOW(1)
	RJMP _0x471
_0x27F:
	LDI  R30,LOW(0)
_0x471:
	STD  Y+6,R30
;    2730 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2731 	}
;    2732 	if(msg == KEY_DOT){
_0x27C:
	LDD  R26,Y+7
	CPI  R26,LOW(0x2E)
	BRNE _0x281
;    2733 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x282
;    2734 	        {
;    2735 		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x472
;    2736 	        }else{
_0x282:
;    2737 		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x472:
	MOVW R30,R6
	CALL SUBOPT_0xA8
;    2738 	        }
;    2739        		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2740 	}
;    2741 	if(msg == MSG_REFRESH) {
_0x281:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x284
;    2742 		REFRESH_TABLES1(prblist_);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xA9
	CALL SUBOPT_0xC5
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	LDI  R30,LOW(_prblist_chvalue*2)
	LDI  R31,HIGH(_prblist_chvalue*2)
	CALL SUBOPT_0x98
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x285
	MOV  R21,R17
_0x287:
	CP   R16,R21
	BRLO _0x288
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xAF
	MOVW R26,R30
	MOV  R30,R21
	LDI  R31,0
	ADD  R30,R26
	MOV  R20,R30
;    2743 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x289
;    2744 		        {
;    2745 		                sprintf(strbuf,"%s",tname2b(j-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xCA
	CALL _tname2b
	RJMP _0x473
;    2746 		        }else{
_0x289:
;    2747         		        sprintf(strbuf,"%s",rname2b(j-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xCA
	CALL _rname2b
_0x473:
	CLR  R22
	CLR  R23
	CALL SUBOPT_0xCB
;    2748         		}
;    2749 			draw_label(&(prblist_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xCC
;    2750 		REFRESH_TABLES2(prblist_);
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xCD
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x28B
	CALL SUBOPT_0xCE
_0x28B:
	SUBI R21,-1
	RJMP _0x287
_0x288:
	RJMP _0x28C
_0x285:
	MOV  R21,R17
_0x28E:
	CP   R16,R21
	BRLO _0x28F
	LDS  R26,_last_pos_S49
	LDS  R27,_last_pos_S49+1
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xCD
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x290
	CALL SUBOPT_0xCE
_0x290:
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xCD
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x291
	CALL SUBOPT_0xCE
_0x291:
	SUBI R21,-1
	RJMP _0x28E
_0x28F:
_0x28C:
;    2751 	}
;    2752 	return;
_0x284:
_0x44D:
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;    2753 }
;    2754 //select probe type list
;    2755 LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2756 LABEL flash tplist_options[] = {
;    2757 	{LBL_HZ6X8,10,20,8,"1.PT100"},
;    2758 	{LBL_HZ6X8,80,20,8,"2.PT25"},
;    2759 	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
;    2760 	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
;    2761 	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
;    2762 	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
;    2763 	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
;    2764 	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
;    2765 	{LBL_HZ6X8,10,50,8,"9.R-TYPE"}
;    2766 };
;    2767 //select probe type thermo
;    2768 void pgprbtypelist_handler(uchar msg) {
_pgprbtypelist_handler:
;    2769 	static int curr_sel = 1;

	.DSEG
_curr_sel_S4A:
	.BYTE 0x2

	.CSEG
;    2770 	static int last_sel = 1;

	.DSEG
_last_sel_S4A:
	.BYTE 0x2

	.CSEG
;    2771 	uchar min_option = 1;
;    2772 	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
;    2773 	uchar i;
;    2774 	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	min_option -> R17
;	max_option -> R16
;	i -> R19
	LDI  R16,9
	LDI  R17,1
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRLO _0x29E
	CPI  R26,LOW(0x3A)
	BRLO _0x29F
_0x29E:
	RJMP _0x29D
_0x29F:
;    2775 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x2A0
;    2776 	        {
;    2777         		switch(msg)
	LDD  R30,Y+4
;    2778 	        	{
;    2779 		        	case KEY_NUM1:
	CPI  R30,LOW(0x31)
	BRNE _0x2A4
;    2780         			       return;
	RJMP _0x44C
;    2781 	        		case KEY_NUM2:
_0x2A4:
	CPI  R30,LOW(0x32)
	BRNE _0x2A6
;    2782 			               return;
	RJMP _0x44C
;    2783         			default:
_0x2A6:
;    2784 	        		       tprbdata.type[curr_prb-1] = msg-KEY_NUM3+PRBTYPE_K;
	CALL SUBOPT_0xCF
	LDD  R30,Y+4
	SUBI R30,LOW(51)
	SUBI R30,-LOW(3)
	CALL __EEPROMWRB
;    2785 		        	       break;
;    2786         		}
;    2787         	}else{
	RJMP _0x2A7
_0x2A0:
;    2788         		switch(msg)
	LDD  R30,Y+4
;    2789 	        	{
;    2790 		        	case KEY_NUM1:
	CPI  R30,LOW(0x31)
	BRNE _0x2AB
;    2791 			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
	CALL SUBOPT_0xD0
	LDI  R30,LOW(241)
	CALL __EEPROMWRB
;    2792         			       break;
	RJMP _0x2AA
;    2793 	        		case KEY_NUM2:
_0x2AB:
	CPI  R30,LOW(0x32)
	BRNE _0x2AD
;    2794 		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
	CALL SUBOPT_0xD0
	LDI  R30,LOW(242)
	CALL __EEPROMWRB
;    2795 			               break;
	RJMP _0x2AA
;    2796         			default:
_0x2AD:
;    2797                                        return;
	RJMP _0x44C
;    2798         		}
_0x2AA:
;    2799         	}
_0x2A7:
;    2800 	        msg = KEY_OK;
	LDI  R30,LOW(79)
	STD  Y+4,R30
;    2801 	}
;    2802 
;    2803 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
_0x29D:
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x2AF
	CPI  R26,LOW(0x43)
	BREQ _0x2AF
	CPI  R26,LOW(0x4F)
	BRNE _0x2AE
_0x2AF:
;    2804 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2805 		return;
	RJMP _0x44C
;    2806 	}
;    2807 	if(msg == MSG_INIT)
_0x2AE:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x2B1
;    2808 	{
;    2809 		LCD_Cls();
	CALL _LCD_Cls
;    2810 		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
	CALL SUBOPT_0x1D
	__POINTW1FN _0,531
	CALL SUBOPT_0xD1
;    2811 		draw_label(&tplist_banner, SW_NORMAL);
	LDI  R30,LOW(_tplist_banner*2)
	LDI  R31,HIGH(_tplist_banner*2)
	CALL SUBOPT_0x98
;    2812 		INIT_OPTIONS(tplist_);
	MOV  R19,R17
_0x2B3:
	CP   R16,R19
	BRLO _0x2B4
	CALL SUBOPT_0xD2
	CALL SUBOPT_0x98
	SUBI R19,-1
	RJMP _0x2B3
_0x2B4:
;    2813 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2814 	}
;    2815 	if(msg == KEY_UP) {
_0x2B1:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x2B5
;    2816 		DEC_OPTION;
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xD4
	SBIW R30,1
	CALL SUBOPT_0xD5
	MOV  R30,R17
	CALL SUBOPT_0xD6
	CP   R26,R30
	CPC  R27,R31
	BRGE _0x2B6
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xD5
;    2817 		msg = MSG_REFRESH;
_0x2B6:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2818 	}
;    2819 	if(msg == KEY_DN) {
_0x2B5:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x2B7
;    2820 		INC_OPTION;
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xD4
	ADIW R30,1
	CALL SUBOPT_0xD5
	MOV  R30,R16
	CALL SUBOPT_0xD6
	CP   R30,R26
	CPC  R31,R27
	BRGE _0x2B8
	CALL SUBOPT_0xD8
	CALL SUBOPT_0xD5
;    2821 		msg = MSG_REFRESH;
_0x2B8:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2822 	}
;    2823 	if(msg == MSG_REFRESH) {
_0x2B7:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x2B9
;    2824 		REFRESH_OPTIONS(tplist_);
	CALL SUBOPT_0xD3
	LDS  R26,_last_sel_S4A
	LDS  R27,_last_sel_S4A+1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2BA
	RJMP _0x44C
_0x2BA:
	MOV  R19,R17
_0x2BC:
	CP   R16,R19
	BRLO _0x2BD
	MOV  R30,R19
	LDS  R26,_last_sel_S4A
	LDS  R27,_last_sel_S4A+1
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2BE
	CALL SUBOPT_0xD2
	CALL SUBOPT_0x9C
_0x2BE:
	MOV  R30,R19
	CALL SUBOPT_0xD6
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2BF
	CALL SUBOPT_0xD2
	CALL SUBOPT_0x9C
_0x2BF:
	SUBI R19,-1
	RJMP _0x2BC
_0x2BD:
;    2825 	}
;    2826 	return;
_0x2B9:
_0x44C:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2827 }
;    2828 /*
;    2829  *       Convert the probe type const to string
;    2830  */
;    2831 char PRBSTR[7];

	.DSEG
_PRBSTR:
	.BYTE 0x7
;    2832 char* getprbtype(uchar prbtype)
;    2833 {

	.CSEG
_getprbtype:
;    2834 	switch(prbtype)
;	prbtype -> Y+0
	LD   R30,Y
;    2835 	{
;    2836 		case PRBTYPE_PT25:
	CPI  R30,LOW(0xF2)
	BRNE _0x2C3
;    2837 		        sprintf(PRBSTR,"PT  25");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,549
	CALL SUBOPT_0x25
;    2838 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2839 		case PRBTYPE_PT100:
_0x2C3:
	CPI  R30,LOW(0xF1)
	BRNE _0x2C4
;    2840 		        sprintf(PRBSTR,"PT 100");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,556
	CALL SUBOPT_0x25
;    2841 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2842 		case PRBTYPE_K:
_0x2C4:
	CPI  R30,LOW(0x3)
	BRNE _0x2C5
;    2843 		        sprintf(PRBSTR,"K TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,563
	CALL SUBOPT_0x25
;    2844 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2845 		case PRBTYPE_N:
_0x2C5:
	CPI  R30,LOW(0x4)
	BRNE _0x2C6
;    2846 		        sprintf(PRBSTR,"N TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,570
	CALL SUBOPT_0x25
;    2847 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2848 		case PRBTYPE_E:
_0x2C6:
	CPI  R30,LOW(0x5)
	BRNE _0x2C7
;    2849 		        sprintf(PRBSTR,"E TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,577
	CALL SUBOPT_0x25
;    2850 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2851 		case PRBTYPE_B:
_0x2C7:
	CPI  R30,LOW(0x6)
	BRNE _0x2C8
;    2852 		        sprintf(PRBSTR,"B TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,584
	CALL SUBOPT_0x25
;    2853 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2854 		case PRBTYPE_J:
_0x2C8:
	CPI  R30,LOW(0x7)
	BRNE _0x2C9
;    2855 		        sprintf(PRBSTR,"J TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,591
	CALL SUBOPT_0x25
;    2856 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2857 		case PRBTYPE_S:
_0x2C9:
	CPI  R30,LOW(0x8)
	BRNE _0x2CA
;    2858 		        sprintf(PRBSTR,"S TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,598
	CALL SUBOPT_0x25
;    2859 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2860 		case PRBTYPE_R:
_0x2CA:
	CPI  R30,LOW(0x9)
	BRNE _0x2C2
;    2861 		        sprintf(PRBSTR,"R TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,605
	CALL SUBOPT_0x25
;    2862 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2863 
;    2864 	}
_0x2C2:
;    2865 
;    2866 		        sprintf(PRBSTR,"? TYPE");
	CALL SUBOPT_0xD9
	__POINTW1FN _0,612
	CALL SUBOPT_0x25
;    2867 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x44B
;    2868 
;    2869 }
;    2870 //config r probe parameter
;    2871 LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
;    2872 LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
;    2873 LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
;    2874 LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
;    2875 LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
;    2876 LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3.a"};
;    2877 LABEL flash paramval1 = {LBL_HZ6X8,165,36,10,strbuf};
;    2878 LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4.b"};
;    2879 LABEL flash paramval2 = {LBL_HZ6X8,45,50,10,strbuf};
;    2880 LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5.c"};
;    2881 LABEL flash paramval3 = {LBL_HZ6X8,165,50,10,strbuf};
;    2882 //configuration of BORE probe parameter
;    2883 void pgprbconfig_handler(uchar msg) {
_pgprbconfig_handler:
;    2884 	if(msg == KEY_TAB || msg == KEY_OK) {
;	msg -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x54)
	BREQ _0x2D2
	CPI  R26,LOW(0x4F)
	BRNE _0x2D1
_0x2D2:
;    2885 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2886 		return;
	RJMP _0x44B
;    2887 	}
;    2888 	if(msg == KEY_CE) {
_0x2D1:
	LD   R26,Y
	CPI  R26,LOW(0x43)
	BRNE _0x2D4
;    2889 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2890 		return;
	RJMP _0x44B
;    2891 	}
;    2892 	if(msg == MSG_INIT)
_0x2D4:
	LD   R26,Y
	CPI  R26,LOW(0xFF)
	BREQ PC+3
	JMP _0x2D5
;    2893 	{
;    2894 		LCD_Cls();
	CALL _LCD_Cls
;    2895 		if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x2D6
;    2896 		{
;    2897 		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
	CALL SUBOPT_0x1D
	__POINTW1FN _0,647
	CALL SUBOPT_0xD1
;    2898         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x98
;    2899 	        	//name
;    2900 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    2901         		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDA
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	CALL _tname2b
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2902 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x98
;    2903         		//type
;    2904 	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDA
	CALL SUBOPT_0xCF
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xAB
;    2905 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	RJMP _0x474
;    2906 		}else{
_0x2D6:
;    2907 		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
	CALL SUBOPT_0x1D
	__POINTW1FN _0,664
	CALL SUBOPT_0xD1
;    2908         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x98
;    2909 	        	//name
;    2910 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    2911         		sprintf(strbuf,":%s",rname2b(curr_prb-1));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDA
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	CALL _rname2b
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2912 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x98
;    2913         		//type
;    2914 	        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDA
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xAB
;    2915 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	CALL SUBOPT_0x98
;    2916         		//param1
;    2917         		sprintf(strbuf,":%7f",rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDC
	CALL SUBOPT_0xDD
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xDE
;    2918 		        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
	LDI  R30,LOW(_paramlbl1*2)
	LDI  R31,HIGH(_paramlbl1*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_paramval1*2)
	LDI  R31,HIGH(_paramval1*2)
	CALL SUBOPT_0x98
;    2919         		//param2
;    2920 	        	sprintf(strbuf,":%7f",rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDC
	CALL SUBOPT_0xDF
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xDE
;    2921         		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	LDI  R30,LOW(_paramlbl2*2)
	LDI  R31,HIGH(_paramlbl2*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_paramval2*2)
	LDI  R31,HIGH(_paramval2*2)
	CALL SUBOPT_0x98
;    2922 	        	//param3
;    2923 		        sprintf(strbuf,":%7f",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x1D
	CALL SUBOPT_0xDC
	CALL SUBOPT_0xE0
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xDE
;    2924         		draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3*2)
	LDI  R31,HIGH(_paramlbl3*2)
	CALL SUBOPT_0x98
	LDI  R30,LOW(_paramval3*2)
	LDI  R31,HIGH(_paramval3*2)
_0x474:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _draw_label
;    2925                 }
;    2926 		return;
	RJMP _0x44B
;    2927 	}
;    2928 	if(msg == KEY_NUM1) {
_0x2D5:
	LD   R26,Y
	CPI  R26,LOW(0x31)
	BRNE _0x2D8
;    2929 		window_setup(7);
	LDI  R30,LOW(7)
	CALL SUBOPT_0x9B
;    2930 		sprintf(strbuf,"请输入探头序列号");
	__POINTW1FN _0,686
	CALL SUBOPT_0x25
;    2931 		prbsninput();
	RCALL _prbsninput
;    2932 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2933 		return;
	RJMP _0x44B
;    2934 	}
;    2935 	if(msg == KEY_NUM2){
_0x2D8:
	LD   R26,Y
	CPI  R26,LOW(0x32)
	BRNE _0x2D9
;    2936 		nextwin = PG_PRBTYPELIST;
	LDI  R30,LOW(9)
	MOV  R4,R30
;    2937 		return;
	RJMP _0x44B
;    2938 	}
;    2939 	if(msg == KEY_NUM3 ){
_0x2D9:
	LD   R26,Y
	CPI  R26,LOW(0x33)
	BRNE _0x2DA
;    2940 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x2DB
;    2941 	        {
;    2942         		window_setup(8);
	LDI  R30,LOW(8)
	CALL SUBOPT_0x9B
;    2943 	        	sprintf(strbuf,"输入系数a");
	__POINTW1FN _0,703
	CALL SUBOPT_0x25
;    2944 		        rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0xDD
	CALL SUBOPT_0x6C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xDD
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xE1
	POP  R26
	POP  R27
	CALL SUBOPT_0xE2
;    2945         		nextwin = PG_PRBCONFIG;
;    2946         	}
;    2947 		return;
_0x2DB:
	RJMP _0x44B
;    2948 	}
;    2949 	if(msg == KEY_NUM4) {
_0x2DA:
	LD   R26,Y
	CPI  R26,LOW(0x34)
	BRNE _0x2DC
;    2950         	if(IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x2DD
;    2951         	{
;    2952         		window_setup(8);
	LDI  R30,LOW(8)
	CALL SUBOPT_0x9B
;    2953 	        	sprintf(strbuf,"输入系数b");
	__POINTW1FN _0,713
	CALL SUBOPT_0x25
;    2954 		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0xDF
	CALL SUBOPT_0x6C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xDF
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xE1
	POP  R26
	POP  R27
	CALL SUBOPT_0xE2
;    2955         		nextwin = PG_PRBCONFIG;
;    2956                 }
;    2957 		return;
_0x2DD:
	RJMP _0x44B
;    2958 	}
;    2959 	if(msg == KEY_NUM5) {
_0x2DC:
	LD   R26,Y
	CPI  R26,LOW(0x35)
	BRNE _0x2DE
;    2960 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x2B
	BRNE _0x2DF
;    2961 	        {
;    2962         		window_setup(8);
	LDI  R30,LOW(8)
	CALL SUBOPT_0x9B
;    2963 		        sprintf(strbuf,"输入系数c");
	__POINTW1FN _0,723
	CALL SUBOPT_0x25
;    2964         		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0xE0
	CALL SUBOPT_0x6C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xE0
	CALL SUBOPT_0x1F
	CALL SUBOPT_0xE1
	POP  R26
	POP  R27
	CALL SUBOPT_0xE2
;    2965 		        nextwin = PG_PRBCONFIG;
;    2966 	        }
;    2967 		return;
_0x2DF:
;    2968 	}
;    2969 	return;
_0x2DE:
_0x44B:
	ADIW R28,1
	RET
;    2970 }
;    2971 
;    2972 LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
;    2973 LABEL flash boot_options[] = {
;    2974  	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
;    2975  	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
;    2976  	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
;    2977  	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
;    2978 };
;    2979 //boot up menu
;    2980 void pgboottype_handler(uchar msg) {
_pgboottype_handler:
;    2981 	uchar i;
;    2982 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2983 	static uchar last_sel = 1;

	.DSEG
_last_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2984 	uchar min_option = 1;
;    2985 	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
;    2986 
;    2987 	if(msg == MSG_INIT) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x2E7
;    2988 		LCD_Cls();
	CALL _LCD_Cls
;    2989 		draw_label(&boot_banner, SW_NORMAL);
	LDI  R30,LOW(_boot_banner*2)
	LDI  R31,HIGH(_boot_banner*2)
	CALL SUBOPT_0x98
;    2990 		INIT_OPTIONS(boot_);
	MOV  R17,R16
_0x2E9:
	CP   R19,R17
	BRLO _0x2EA
	CALL SUBOPT_0xE3
	CALL SUBOPT_0x98
	SUBI R17,-1
	RJMP _0x2E9
_0x2EA:
;    2991 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2992 	}
;    2993 	if(msg == KEY_NUM1) {
_0x2E7:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x2EB
;    2994 		SET_BORE_MODE;
	CALL SUBOPT_0x27
;    2995 		SET_TORX;
	LDI  R30,LOW(106)
	CALL SUBOPT_0x14
;    2996 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2997 		return;
	CALL __LOADLOCR4
	RJMP _0x449
;    2998 	}
;    2999 	if(msg == KEY_NUM2) {
_0x2EB:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x2EC
;    3000 		SET_BORE_MODE;
	CALL SUBOPT_0x27
;    3001 		SET_TORX;
	LDI  R30,LOW(106)
	CALL SUBOPT_0x14
;    3002 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    3003 		return;
	CALL __LOADLOCR4
	RJMP _0x449
;    3004 	}
;    3005 	if(msg == KEY_NUM3) {
_0x2EC:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x2ED
;    3006 		SET_THERM_MODE;
	CALL SUBOPT_0x2C
;    3007 		SET_TORS;
;    3008 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3009 		return;
	CALL __LOADLOCR4
	RJMP _0x449
;    3010 	}
;    3011 	if(msg == KEY_NUM4) {
_0x2ED:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x2EE
;    3012 		SET_THERM_MODE;
	CALL SUBOPT_0x2C
;    3013 		SET_TORS;
;    3014 		nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	MOV  R4,R30
;    3015 		return;
	CALL __LOADLOCR4
	RJMP _0x449
;    3016 	}
;    3017 	if(msg == MSG_REFRESH) {
_0x2EE:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x2EF
;    3018 		REFRESH_OPTIONS(boot_);
	LDS  R30,_curr_sel_S4D
	LDS  R26,_last_sel_S4D
	CP   R30,R26
	BRNE _0x2F0
	CALL __LOADLOCR4
	RJMP _0x449
_0x2F0:
	MOV  R17,R16
_0x2F2:
	CP   R19,R17
	BRLO _0x2F3
	LDS  R26,_last_sel_S4D
	CP   R17,R26
	BRNE _0x2F4
	CALL SUBOPT_0xE3
	CALL SUBOPT_0x9C
_0x2F4:
	LDS  R26,_curr_sel_S4D
	CP   R17,R26
	BRNE _0x2F5
	CALL SUBOPT_0xE3
	CALL SUBOPT_0x9C
_0x2F5:
	SUBI R17,-1
	RJMP _0x2F2
_0x2F3:
;    3019 	}
;    3020 }
_0x2EF:
	CALL __LOADLOCR4
	RJMP _0x449
;    3021 #include "utili.h"
;    3022 #include "window.h"
;    3023 /*---------------延时子程序----------------*/
;    3024 /*
;    3025 void delay1 (uint ms)
;    3026 {
;    3027   	int i,j;
;    3028   	for(i=0;i<ms;i++)
;    3029   	for(j=0;j<1000;j++)
;    3030   		;
;    3031 
;    3032 }
;    3033 */
;    3034 /*
;    3035 char lowc(uchar x)
;    3036 {
;    3037 	x = x&0x0f;
;    3038 	if(x > 0x09)
;    3039 		return 'A'+x-0x0a;
;    3040 	else
;    3041 		return '0'+x;
;    3042 }
;    3043 char highc(uchar x)
;    3044 {
;    3045 
;    3046 	x = x>>4;
;    3047 	if(x > 0x09)
;    3048 		return 'A'+x-0x0a;
;    3049 	else
;    3050 		return '0'+x;
;    3051 }
;    3052 */
;    3053 /*计算字符串长度*/
;    3054 
;    3055 extern uchar pos_databuf;
;    3056 extern uchar data_sign;
;    3057 extern u8 databuf[12];
;    3058 double buf2double()
;    3059 {
_buf2double:
;    3060 	double tmp = 0.0;
;    3061 	uchar i = 0;
;    3062 	uchar pos = 0;
;    3063 	for(i=1;i<pos_databuf;i++) {
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x2F6*2)
	LDI  R31,HIGH(_0x2F6*2)
	CALL __INITLOCB
	ST   -Y,R17
	ST   -Y,R16
;	tmp -> Y+2
;	i -> R17
;	pos -> R16
	LDI  R16,0
	LDI  R17,0
	LDI  R17,LOW(1)
_0x2F8:
	LDS  R30,_pos_databuf
	CP   R17,R30
	BRSH _0x2F9
;    3064 		if(databuf[i] != KEY_DOT)
	CALL SUBOPT_0xD8
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x2E)
	BREQ _0x2FA
;    3065 			tmp = tmp * 10.0+(databuf[i] - '0');
	CALL SUBOPT_0xE4
	CALL SUBOPT_0x19
	CALL SUBOPT_0xD8
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	SUBI R30,LOW(48)
	CALL SUBOPT_0xE5
	CALL __ADDF12
	CALL SUBOPT_0x59
;    3066 		else
	RJMP _0x2FB
_0x2FA:
;    3067 			pos = pos_databuf - i - 2;
	LDS  R30,_pos_databuf
	SUB  R30,R17
	SUBI R30,LOW(2)
	MOV  R16,R30
;    3068 	}
_0x2FB:
	SUBI R17,-1
	RJMP _0x2F8
_0x2F9:
;    3069 	while(pos > 0) {
_0x2FC:
	CPI  R16,1
	BRLO _0x2FE
;    3070 		tmp = tmp / 10.0;
	CALL SUBOPT_0xE4
	CALL SUBOPT_0x52
;    3071 		pos--;
	SUBI R16,1
;    3072 	}
	RJMP _0x2FC
_0x2FE:
;    3073 	if(data_sign == 1)
	LDS  R26,_data_sign
	CPI  R26,LOW(0x1)
	BRNE _0x2FF
;    3074 	        tmp = -tmp;
	CALL SUBOPT_0x4A
	CALL __ANEGF1
	CALL SUBOPT_0x59
;    3075        	return tmp;
_0x2FF:
	CALL SUBOPT_0x4A
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    3076 }
;    3077 int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
;    3078 {
_buf2byte:
;    3079 	int tmp = 0;
;    3080 	uchar i;
;    3081 	for(i=0;i<pos_databuf;i++) {
	CALL __SAVELOCR4
;	tmp -> R16,R17
;	i -> R19
	LDI  R16,0
	LDI  R17,0
	LDI  R19,LOW(0)
_0x301:
	LDS  R30,_pos_databuf
	CP   R19,R30
	BRSH _0x302
;    3082 		tmp = tmp * 10+(databuf[i] - '0');
	MOVW R26,R16
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12
	MOVW R26,R30
	CALL SUBOPT_0xA5
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	SUBI R30,LOW(48)
	CALL SUBOPT_0xE6
	MOVW R16,R30
;    3083 	}
	SUBI R19,-1
	RJMP _0x301
_0x302:
;    3084 	return tmp;
	MOVW R30,R16
	CALL __LOADLOCR4
	RJMP _0x447
;    3085 }
;    3086 
;    3087 
;    3088 #include "window.h"
;    3089 #include "utili.h"
;    3090 #include "lcd.h"
;    3091 
;    3092 MSG_HANDLER curr_window = 0;

	.DSEG
_curr_window:
	.BYTE 0x2
;    3093 
;    3094 extern uchar key;
;    3095 
;    3096 u8 databuf[12];
_databuf:
	.BYTE 0xC
;    3097 u8 pos_databuf; //position in data buffer
_pos_databuf:
	.BYTE 0x1
;    3098 u8 max_databuf;
_max_databuf:
	.BYTE 0x1
;    3099 u8 data_sign;   // sign of the data
_data_sign:
	.BYTE 0x1
;    3100 LABEL flash datalbl = {LBL_HZ16,30,10,8,strbuf};

	.CSEG
;    3101 LABEL flash databox = {LBL_HZ16,30,30,8,databuf};
;    3102 
;    3103 void prbsninput()
;    3104 {
_prbsninput:
;    3105 	uchar msg,len;
;    3106 	databuf[0] = '\0';
	ST   -Y,R17
	ST   -Y,R16
;	msg -> R17
;	len -> R16
	CALL SUBOPT_0xE7
;    3107 	pos_databuf = 0;
;    3108 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3109 	while(1)
_0x304:
;    3110 	{
;    3111 		if(key == KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x304
;    3112 		        continue;
;    3113 
;    3114 		msg = key;
	MOV  R17,R5
;    3115 
;    3116 
;    3117 		if(msg == MSG_INIT) {
	CPI  R17,255
	BRNE _0x308
;    3118 			LCD_Cls();
	CALL SUBOPT_0xE8
;    3119 			draw_label(&datalbl,SW_NORMAL);
;    3120 			sprintf(databuf,"");
	LDI  R30,LOW(_databuf)
	LDI  R31,HIGH(_databuf)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,11
	CALL SUBOPT_0x25
;    3121 			draw_inputbox(&databox);
	CALL SUBOPT_0xE9
;    3122 //			LCD_ShowCursor(databox.x,databox.y);
;    3123 		}
;    3124 		if(msg == KEY_TAB)
_0x308:
	CPI  R17,84
	BRNE _0x309
;    3125 		{
;    3126 		/*
;    3127         		if(pos_databuf < max_databuf)
;    3128         		{
;    3129 			        databuf[pos_databuf++] = '0';
;    3130 			        databuf[pos_databuf] = '\0';
;    3131         			msg = MSG_REFRESH;
;    3132                         }
;    3133                 */
;    3134 		}
;    3135 		if(msg == KEY_CE) {
_0x309:
	CPI  R17,67
	BRNE _0x30A
;    3136 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3137 			return;
	RJMP _0x44A
;    3138 		}
;    3139 
;    3140 		if(msg == KEY_DN) {
_0x30A:
	CPI  R17,68
	BRNE _0x30B
;    3141 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x30C
;    3142 			{
;    3143 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3144 				databuf[pos_databuf] = '\0';
;    3145 			}
;    3146 
;    3147 			if(databuf[pos_databuf-1] == '0')
_0x30C:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xED
	BRNE _0x30D
;    3148 				databuf[pos_databuf-1] = 'Z';
	CALL SUBOPT_0xEC
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(90)
	STD  Z+0,R26
;    3149 			else if (databuf[pos_databuf-1] == 'A')
	RJMP _0x30E
_0x30D:
	CALL SUBOPT_0xEC
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRNE _0x30F
;    3150 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEE
;    3151 			else
	RJMP _0x310
_0x30F:
;    3152 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,LOW(1)
	ST   X,R30
;    3153 			msg = MSG_REFRESH;
_0x310:
_0x30E:
	LDI  R17,LOW(254)
;    3154 		}
;    3155 		if(msg == KEY_UP) {
_0x30B:
	CPI  R17,85
	BRNE _0x311
;    3156 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x312
;    3157 			{
;    3158 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3159 				databuf[pos_databuf] = '\0';
;    3160 			}
;    3161 
;    3162 			if(databuf[pos_databuf-1] == '9')
_0x312:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF0
	BRNE _0x313
;    3163 				databuf[pos_databuf-1] = 'A';
	CALL SUBOPT_0xEC
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    3164 			else if (databuf[pos_databuf-1] == 'Z')
	RJMP _0x314
_0x313:
	CALL SUBOPT_0xEC
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x5A)
	BRNE _0x315
;    3165 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF1
;    3166 			else
	RJMP _0x316
_0x315:
;    3167 				databuf[pos_databuf-1] +=1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,-LOW(1)
	ST   X,R30
;    3168 			msg = MSG_REFRESH;
_0x316:
_0x314:
	LDI  R17,LOW(254)
;    3169 		}
;    3170 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x311:
	CPI  R17,48
	BRLO _0x318
	CPI  R17,58
	BRLO _0x319
_0x318:
	RJMP _0x317
_0x319:
;    3171         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xF2
	BRSH _0x31A
;    3172         		{
;    3173 			        databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xF3
;    3174         			databuf[pos_databuf] = '\0';
;    3175         		}
;    3176 			msg = MSG_REFRESH;
_0x31A:
	LDI  R17,LOW(254)
;    3177 		}
;    3178 		if(msg == KEY_OK) {
_0x317:
	CPI  R17,79
	BRNE _0x31B
;    3179 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x2E
	BRNE _0x31C
;    3180 		        {
;    3181 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x31E:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x31F
;    3182         		                tprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _tprbdata,288
	CALL SUBOPT_0xF4
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3183         			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x31E
_0x31F:
	__POINTW2MN _tprbdata,288
	RJMP _0x475
;    3184         		}else{
_0x31C:
;    3185 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x322:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x323
;    3186         		                rprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _rprbdata,288
	CALL SUBOPT_0xF4
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3187         			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x322
_0x323:
	__POINTW2MN _rprbdata,288
_0x475:
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	LDS  R30,_pos_databuf
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
;    3188         		}
;    3189 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3190 			break;
	RJMP _0x306
;    3191 		}
;    3192 		if(msg == MSG_REFRESH) {
_0x31B:
	CPI  R17,254
	BRNE _0x324
;    3193 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xF5
;    3194 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3195 		}
;    3196 				key = KEY_INVALID;
_0x324:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3197 	}
	RJMP _0x304
_0x306:
;    3198 //	LCD_HideCursor();
;    3199 }
_0x44A:
	LD   R16,Y+
	LD   R17,Y+
	RET
;    3200 
;    3201 uchar wnd_intinput(uchar lastval)
;    3202 {
_wnd_intinput:
;    3203 	uchar msg;
;    3204 
;    3205 	key = MSG_INIT;
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3206 	databuf[0] = '\0';
	CALL SUBOPT_0xE7
;    3207 	pos_databuf = 0;
;    3208         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3209 	while(1)
_0x325:
;    3210 	{
;    3211 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x328
;    3212 		{
;    3213 			msg = key;
	MOV  R17,R5
;    3214 
;    3215 		}else{
	RJMP _0x329
_0x328:
;    3216 			continue;
	RJMP _0x325
;    3217 		}
_0x329:
;    3218 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x32A
;    3219 		{
;    3220 			LCD_Cls();
	CALL SUBOPT_0xE8
;    3221 			draw_label(&datalbl,SW_NORMAL);
;    3222 			draw_inputbox(&databox);
	CALL SUBOPT_0xE9
;    3223 //			LCD_ShowCursor(databox.x,databox.y);
;    3224 
;    3225 		}
;    3226 
;    3227 		if(msg == KEY_TAB)
_0x32A:
	CPI  R17,84
	BRNE _0x32B
;    3228 		{
;    3229 		/*
;    3230         		if(pos_databuf < max_databuf)
;    3231         		{
;    3232 			        databuf[pos_databuf++] = '0';
;    3233 			        databuf[pos_databuf] = '\0';
;    3234         			msg = MSG_REFRESH;
;    3235                         }
;    3236                 */
;    3237 		}
;    3238 		if(msg == KEY_DN) {
_0x32B:
	CPI  R17,68
	BRNE _0x32C
;    3239 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x32D
;    3240 			{
;    3241 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3242 				databuf[pos_databuf] = '\0';
;    3243 			}
;    3244 
;    3245 			if(databuf[pos_databuf-1] == '0')
_0x32D:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xED
	BRNE _0x32E
;    3246 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEE
;    3247 			else
	RJMP _0x32F
_0x32E:
;    3248 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,LOW(1)
	ST   X,R30
;    3249 			msg = MSG_REFRESH;
_0x32F:
	LDI  R17,LOW(254)
;    3250 		}
;    3251 		if(msg == KEY_UP) {
_0x32C:
	CPI  R17,85
	BRNE _0x330
;    3252 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x331
;    3253 			{
;    3254 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3255 				databuf[pos_databuf] = '\0';
;    3256 			}
;    3257 
;    3258 			if(databuf[pos_databuf-1] == '9')
_0x331:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF0
	BRNE _0x332
;    3259 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF1
;    3260 			else
	RJMP _0x333
_0x332:
;    3261 				databuf[pos_databuf-1] += 1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,-LOW(1)
	ST   X,R30
;    3262 			msg = MSG_REFRESH;
_0x333:
	LDI  R17,LOW(254)
;    3263 		}
;    3264 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x330:
	CPI  R17,48
	BRLO _0x335
	CPI  R17,58
	BRLO _0x336
_0x335:
	RJMP _0x334
_0x336:
;    3265         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xF2
	BRSH _0x337
;    3266         		{
;    3267         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xF3
;    3268 	        		databuf[pos_databuf] = '\0';
;    3269 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3270 		        }
;    3271 		}
_0x337:
;    3272 		if(msg == KEY_CE) {
_0x334:
	CPI  R17,67
	BRNE _0x338
;    3273 //			LCD_HideCursor();
;    3274                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3275 			return lastval;
	LDD  R30,Y+1
	LDD  R17,Y+0
	RJMP _0x448
;    3276 		}
;    3277 		if(msg == KEY_OK){
_0x338:
	CPI  R17,79
	BRNE _0x339
;    3278 //			LCD_HideCursor();
;    3279                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3280 			return buf2byte();
	CALL _buf2byte
	LDD  R17,Y+0
	RJMP _0x448
;    3281 		}
;    3282 		if(msg == MSG_REFRESH) {
_0x339:
	CPI  R17,254
	BRNE _0x33A
;    3283 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xF5
;    3284 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3285 		}
;    3286 		key = KEY_INVALID;
_0x33A:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3287 	}
	RJMP _0x325
;    3288 }
;    3289 double wnd_floatinput(double lastval)
;    3290 {
_wnd_floatinput:
;    3291 	uchar msg;
;    3292 
;    3293 	databuf[0] = ' ';
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(32)
	STS  _databuf,R30
;    3294         databuf[1] = '\0';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3295         pos_databuf = 1;
	LDI  R30,LOW(1)
	STS  _pos_databuf,R30
;    3296         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3297 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3298 	while(1)
_0x33B:
;    3299 	{
;    3300 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x33E
;    3301 		{
;    3302 			msg = key;
	MOV  R17,R5
;    3303 
;    3304 		}else{
	RJMP _0x33F
_0x33E:
;    3305 			continue;
	RJMP _0x33B
;    3306 		}
_0x33F:
;    3307 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x340
;    3308 		{
;    3309 			LCD_Cls();
	CALL SUBOPT_0xE8
;    3310 			draw_label(&datalbl,SW_NORMAL);
;    3311 			draw_inputbox(&databox);
	CALL SUBOPT_0xE9
;    3312 //			LCD_ShowCursor(databox.x,databox.y);
;    3313 		}
;    3314 
;    3315 		if(msg == KEY_TAB)
_0x340:
	CPI  R17,84
	BRNE _0x341
;    3316 		{
;    3317 		        if(data_sign == 0)
	LDS  R30,_data_sign
	CPI  R30,0
	BRNE _0x342
;    3318 		        {
;    3319 		                data_sign  = 1;
	LDI  R30,LOW(1)
	STS  _data_sign,R30
;    3320         			databuf[0] = '-';
	LDI  R30,LOW(45)
	RJMP _0x476
;    3321 		        }else{
_0x342:
;    3322 		                data_sign  = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3323         			databuf[0] = '+';
	LDI  R30,LOW(43)
_0x476:
	STS  _databuf,R30
;    3324 		        }
;    3325                         if(pos_databuf == 1){
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x344
;    3326 	        	        databuf[pos_databuf] = '\0';
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3327 	        	}
;    3328 		        msg = MSG_REFRESH;
_0x344:
	LDI  R17,LOW(254)
;    3329 
;    3330 		}
;    3331 		if(msg == KEY_DN) {
_0x341:
	CPI  R17,68
	BRNE _0x345
;    3332 			if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x346
;    3333 			{
;    3334 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3335 				databuf[pos_databuf] = '\0';
;    3336 			}
;    3337 			if(databuf[pos_databuf-1] == '0')
_0x346:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xED
	BRNE _0x347
;    3338 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEE
;    3339 			else
	RJMP _0x348
_0x347:
;    3340 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,LOW(1)
	ST   X,R30
;    3341 			msg = MSG_REFRESH;
_0x348:
	LDI  R17,LOW(254)
;    3342 		}
;    3343 		if(msg == KEY_UP) {
_0x345:
	CPI  R17,85
	BRNE _0x349
;    3344 			if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x34A
;    3345 			{
;    3346 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xEB
;    3347 				databuf[pos_databuf] = '\0';
;    3348 			}
;    3349 			if(databuf[pos_databuf-1] == '9')
_0x34A:
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF0
	BRNE _0x34B
;    3350 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF1
;    3351 			else
	RJMP _0x34C
_0x34B:
;    3352 				databuf[pos_databuf-1] += 1;
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xEF
	SUBI R30,-LOW(1)
	ST   X,R30
;    3353 			msg = MSG_REFRESH;
_0x34C:
	LDI  R17,LOW(254)
;    3354 		}
;    3355 		if((msg >= KEY_NUM0 && msg <= KEY_NUM9) || msg == KEY_DOT) {
_0x349:
	CPI  R17,48
	BRLO _0x34E
	CPI  R17,58
	BRLO _0x350
_0x34E:
	CPI  R17,46
	BRNE _0x34D
_0x350:
;    3356                		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xF2
	BRSH _0x352
;    3357         		{
;    3358         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xEA
	CALL SUBOPT_0xF3
;    3359 	        		databuf[pos_databuf] = '\0';
;    3360 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3361 		        }
;    3362 		}
_0x352:
;    3363 		if(msg == KEY_CE) {
_0x34D:
	CPI  R17,67
	BRNE _0x353
;    3364 //			LCD_HideCursor();
;    3365 		key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3366 			return lastval;
	__GETD1S 1
	LDD  R17,Y+0
	RJMP _0x449
;    3367 		}
;    3368 		if(msg == KEY_OK){
_0x353:
	CPI  R17,79
	BRNE _0x354
;    3369 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3370 //			LCD_HideCursor();
;    3371 			return buf2double();
	CALL _buf2double
	LDD  R17,Y+0
	RJMP _0x449
;    3372 		}
;    3373 		if(msg == MSG_REFRESH) {
_0x354:
	CPI  R17,254
	BRNE _0x355
;    3374 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xF5
;    3375 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3376 		}
;    3377 				key = KEY_INVALID;
_0x355:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3378 	}
	RJMP _0x33B
;    3379 }
_0x449:
	ADIW R28,5
	RET
;    3380 //display a message box
;    3381 void wnd_msgbox(flash LABEL *lbl)
;    3382 {
_wnd_msgbox:
;    3383 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0xF6
	CALL _LCD_ClsBlock
;    3384 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x98
;    3385 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	CALL SUBOPT_0xF6
	CALL _LCD_Rectange
;    3386 	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
	CALL SUBOPT_0xF7
	SBIW R30,8
	CALL SUBOPT_0xF8
	SBIW R30,1
	CALL SUBOPT_0xF9
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(16)
	LDI  R31,HIGH(16)
	CALL __SWAPW12
	SUB  R30,R26
	SBC  R31,R27
	CALL SUBOPT_0xFA
	ADIW R30,17
	ST   -Y,R30
	CALL _LCD_Rectange
;    3387 
;    3388 }
	RJMP _0x448
;    3389 
;    3390 void draw_inputbox(flash LABEL *lbl)
;    3391 {
_draw_inputbox:
;    3392 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0xF7
	SBIW R30,10
	CALL SUBOPT_0xF8
	SBIW R30,3
	CALL SUBOPT_0xF9
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0xFA
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3393 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	CALL SUBOPT_0xF7
	SBIW R30,10
	CALL SUBOPT_0xF8
	SBIW R30,3
	CALL SUBOPT_0xF9
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0xFA
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_Rectange
;    3394 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x98
;    3395 }
_0x448:
	ADIW R28,2
	RET
;    3396 
;    3397 void draw_label(flash LABEL *lbl,uchar reverse) {
_draw_label:
;    3398 	uchar len = lbl->width;
;    3399 	if(lbl->type == LBL_HZ6X8) {
	ST   -Y,R17
;	*lbl -> Y+2
;	reverse -> Y+1
;	len -> R17
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	MOV  R17,R30
	CALL SUBOPT_0xFB
	SBIW R30,1
	BRNE _0x356
;    3400 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x357
;    3401 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x358
;    3402 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFD
	CALL SUBOPT_0xFE
	ADIW R30,8
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3403 			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
_0x358:
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFF
	CALL _LCD_Print6X8
;    3404 			return;
	LDD  R17,Y+0
	RJMP _0x447
;    3405 		}
;    3406 		if(reverse & SW_REVERSE)
_0x357:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x359
;    3407 			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
	CALL SUBOPT_0x100
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3408 		return;
_0x359:
	LDD  R17,Y+0
	RJMP _0x447
;    3409 	}
;    3410 	if(lbl->type == LBL_HZ8X16) {
_0x356:
	CALL SUBOPT_0xFB
	SBIW R30,2
	BRNE _0x35A
;    3411 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x35B
;    3412 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x35C
;    3413 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFD
	CALL SUBOPT_0xFE
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3414 			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
_0x35C:
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFF
	CALL _LCD_Print8X16
;    3415 			return;
	LDD  R17,Y+0
	RJMP _0x447
;    3416 		}
;    3417 		if(reverse & SW_REVERSE)
_0x35B:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x35D
;    3418 			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
	CALL SUBOPT_0x100
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3419 		return;
_0x35D:
	LDD  R17,Y+0
	RJMP _0x447
;    3420 	}
;    3421 	/*
;    3422 	if(lbl->type == LBL_HZ24X32) {
;    3423 		if(reverse & SW_NORMAL) {
;    3424 			if((reverse & SW_OVERLAP) == 0)
;    3425 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
;    3426 			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
;    3427 		}
;    3428 		if(reverse & SW_REVERSE)
;    3429 			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
;    3430 		return;
;    3431 	}
;    3432 	*/
;    3433 	if(lbl->type == LBL_HZ12) {
_0x35A:
	CALL SUBOPT_0xFB
	SBIW R30,4
	BRNE _0x35E
;    3434 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x35F
;    3435 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x360
;    3436 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
	CALL SUBOPT_0xFC
	CALL SUBOPT_0x101
	CALL SUBOPT_0xFE
	ADIW R30,12
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3437 			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
_0x360:
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFF
	CALL _LCD_PrintHz12
;    3438 		}
;    3439 		if(reverse & SW_REVERSE)
_0x35F:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x361
;    3440 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
	CALL SUBOPT_0x102
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3441 		return;
_0x361:
	LDD  R17,Y+0
	RJMP _0x447
;    3442 	}
;    3443 	if(lbl->type == LBL_HZ16) {
_0x35E:
	CALL SUBOPT_0xFB
	SBIW R30,5
	BRNE _0x362
;    3444 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x363
;    3445 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x364
;    3446 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
	CALL SUBOPT_0xFC
	CALL SUBOPT_0x101
	CALL SUBOPT_0xFE
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3447 			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
_0x364:
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xFF
	CALL _LCD_PrintHz16
;    3448 		}
;    3449 		if(reverse & SW_REVERSE)
_0x363:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x365
;    3450 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
	CALL SUBOPT_0x102
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3451 		return;
_0x365:
	LDD  R17,Y+0
	RJMP _0x447
;    3452 	}
;    3453 }
_0x362:
	LDD  R17,Y+0
_0x447:
	ADIW R28,4
	RET
;    3454 
;    3455 

_getchar:
     sbis usr,rxc
     rjmp _getchar
     in   r30,udr
	RET
_putchar:
     sbis usr,udre
     rjmp _putchar
     ld   r30,y
     out  udr,r30
	ADIW R28,1
	RET
__put_GD:
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL __GETW1P
	SBIW R30,0
	BREQ _0x366
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x368
	__CPWRN 16,17,2
	BRLO _0x369
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x368:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL SUBOPT_0x103
	LDD  R26,Y+6
	STD  Z+0,R26
_0x369:
	RJMP _0x36A
_0x366:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x36A:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,7
	RET
__ftoa_GD:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	LDD  R26,Y+8
	CPI  R26,LOW(0x7)
	BRLO _0x36B
	LDI  R30,LOW(6)
	STD  Y+8,R30
_0x36B:
	LDD  R30,Y+8
	LDI  R26,LOW(__fround_GD*2)
	LDI  R27,HIGH(__fround_GD*2)
	LDI  R31,0
	CALL SUBOPT_0x6C
	CALL __GETD1PF
	CALL SUBOPT_0x104
	CALL __ADDF12
	CALL SUBOPT_0x105
	LDI  R17,LOW(0)
	CALL SUBOPT_0x81
	CALL SUBOPT_0x59
_0x36C:
	CALL SUBOPT_0x4A
	CALL SUBOPT_0x104
	CALL __CMPF12
	BRLO _0x36E
	CALL SUBOPT_0xE4
	CALL __MULF12
	CALL SUBOPT_0x59
	SUBI R17,-LOW(1)
	RJMP _0x36C
_0x36E:
	CPI  R17,0
	BRNE _0x36F
	CALL SUBOPT_0x106
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x370
_0x36F:
_0x371:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x373
	CALL SUBOPT_0xE4
	CALL SUBOPT_0x107
	CALL SUBOPT_0x96
	CALL SUBOPT_0x104
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL SUBOPT_0x97
	CALL SUBOPT_0xE5
	CALL __MULF12
	CALL SUBOPT_0x104
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x105
	RJMP _0x371
_0x373:
_0x370:
	LDD  R30,Y+8
	CPI  R30,0
	BRNE _0x374
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x446
_0x374:
	CALL SUBOPT_0x106
	LDI  R30,LOW(46)
	ST   X,R30
_0x375:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x377
	CALL SUBOPT_0x104
	CALL SUBOPT_0x109
	CALL SUBOPT_0x105
	__GETD1S 9
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL SUBOPT_0x104
	CALL SUBOPT_0xE5
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x105
	RJMP _0x375
_0x377:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x446:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
__ftoe_GD:
	SBIW R28,4
	CALL __SAVELOCR4
	CALL SUBOPT_0x81
	CALL SUBOPT_0x6B
	LDD  R26,Y+11
	CPI  R26,LOW(0x7)
	BRLO _0x378
	LDI  R30,LOW(6)
	STD  Y+11,R30
_0x378:
	LDD  R17,Y+11
_0x379:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x37B
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x6B
	RJMP _0x379
_0x37B:
	CALL SUBOPT_0x10B
	CALL __CPD10
	BRNE _0x37C
	LDI  R19,LOW(0)
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x6B
	RJMP _0x37D
_0x37C:
	LDD  R19,Y+11
	CALL SUBOPT_0x10C
	BREQ PC+2
	BRCC PC+3
	JMP  _0x37E
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x6B
_0x37F:
	CALL SUBOPT_0x10C
	BRLO _0x381
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10E
	RJMP _0x37F
_0x381:
	RJMP _0x382
_0x37E:
_0x383:
	CALL SUBOPT_0x10C
	BRSH _0x385
	CALL SUBOPT_0x10F
	CALL SUBOPT_0x110
	SUBI R19,LOW(1)
	RJMP _0x383
_0x385:
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x6B
_0x382:
	CALL SUBOPT_0x10B
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL SUBOPT_0x110
	CALL SUBOPT_0x10C
	BRLO _0x386
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10E
_0x386:
_0x37D:
	LDI  R17,LOW(0)
_0x387:
	LDD  R30,Y+11
	CP   R30,R17
	BRLO _0x389
	CALL SUBOPT_0x111
	__GETD1N 0x41200000
	CALL SUBOPT_0x107
	CALL SUBOPT_0x6B
	CALL SUBOPT_0x6D
	CALL SUBOPT_0x10D
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x112
	CALL SUBOPT_0x108
	CALL SUBOPT_0xE5
	CALL SUBOPT_0x111
	CALL __MULF12
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x1B
	CALL SUBOPT_0x110
	MOV  R30,R17
	SUBI R17,-1
	CPI  R30,0
	BRNE _0x387
	CALL SUBOPT_0x112
	LDI  R30,LOW(46)
	ST   X,R30
	RJMP _0x387
_0x389:
	CALL SUBOPT_0xB7
	SBIW R30,1
	LDD  R26,Y+10
	STD  Z+0,R26
	CPI  R19,0
	BRGE _0x38B
	CALL SUBOPT_0x112
	LDI  R30,LOW(45)
	ST   X,R30
	NEG  R19
_0x38B:
	CPI  R19,10
	BRLT _0x38C
	CALL SUBOPT_0xB7
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __DIVB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
_0x38C:
	CALL SUBOPT_0xB7
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __MODB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDI  R30,LOW(0)
	ST   X,R30
	CALL __LOADLOCR4
	ADIW R28,16
	RET
__print_GD:
	SBIW R28,63
	SBIW R28,15
	CALL __SAVELOCR6
	LDI  R17,0
	__GETW1SX 84
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x38D:
	MOVW R26,R28
	SUBI R26,LOW(-(90))
	SBCI R27,HIGH(-(90))
	CALL SUBOPT_0x103
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x38F
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x393
	CPI  R18,37
	BRNE _0x394
	LDI  R17,LOW(1)
	RJMP _0x395
_0x394:
	CALL SUBOPT_0x113
_0x395:
	RJMP _0x392
_0x393:
	CPI  R30,LOW(0x1)
	BRNE _0x396
	CPI  R18,37
	BRNE _0x397
	CALL SUBOPT_0x113
	RJMP _0x477
_0x397:
	LDI  R17,LOW(2)
	LDI  R30,LOW(0)
	STD  Y+19,R30
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x398
	LDI  R16,LOW(1)
	RJMP _0x392
_0x398:
	CPI  R18,43
	BRNE _0x399
	LDI  R30,LOW(43)
	STD  Y+19,R30
	RJMP _0x392
_0x399:
	CPI  R18,32
	BRNE _0x39A
	LDI  R30,LOW(32)
	STD  Y+19,R30
	RJMP _0x392
_0x39A:
	RJMP _0x39B
_0x396:
	CPI  R30,LOW(0x2)
	BRNE _0x39C
_0x39B:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x39D
	ORI  R16,LOW(128)
	RJMP _0x392
_0x39D:
	RJMP _0x39E
_0x39C:
	CPI  R30,LOW(0x3)
	BRNE _0x39F
_0x39E:
	CPI  R18,48
	BRLO _0x3A1
	CPI  R18,58
	BRLO _0x3A2
_0x3A1:
	RJMP _0x3A0
_0x3A2:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x392
_0x3A0:
	LDI  R20,LOW(0)
	CPI  R18,46
	BRNE _0x3A3
	LDI  R17,LOW(4)
	RJMP _0x392
_0x3A3:
	RJMP _0x3A4
_0x39F:
	CPI  R30,LOW(0x4)
	BRNE _0x3A6
	CPI  R18,48
	BRLO _0x3A8
	CPI  R18,58
	BRLO _0x3A9
_0x3A8:
	RJMP _0x3A7
_0x3A9:
	ORI  R16,LOW(32)
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R20,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R20,R30
	RJMP _0x392
_0x3A7:
_0x3A4:
	CPI  R18,108
	BRNE _0x3AA
	ORI  R16,LOW(2)
	LDI  R17,LOW(5)
	RJMP _0x392
_0x3AA:
	RJMP _0x3AB
_0x3A6:
	CPI  R30,LOW(0x5)
	BREQ PC+3
	JMP _0x392
_0x3AB:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x3B0
	CALL SUBOPT_0x114
	LDD  R30,Z+4
	CALL SUBOPT_0x115
	RJMP _0x3B1
_0x3B0:
	CPI  R30,LOW(0x45)
	BREQ _0x3B4
	CPI  R30,LOW(0x65)
	BRNE _0x3B5
_0x3B4:
	RJMP _0x3B6
_0x3B5:
	CPI  R30,LOW(0x66)
	BREQ PC+3
	JMP _0x3B7
_0x3B6:
	MOVW R30,R28
	ADIW R30,20
	STD  Y+10,R30
	STD  Y+10+1,R31
	CALL SUBOPT_0x114
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	CALL SUBOPT_0x44
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD20
	BRLT _0x3B8
	LDD  R26,Y+19
	CPI  R26,LOW(0x2B)
	BREQ _0x3BA
	RJMP _0x3BB
_0x3B8:
	CALL SUBOPT_0x57
	CALL __ANEGF1
	CALL SUBOPT_0x44
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x3BA:
	SBRS R16,7
	RJMP _0x3BC
	LDD  R30,Y+19
	CALL SUBOPT_0x115
	RJMP _0x3BD
_0x3BC:
	CALL SUBOPT_0x116
	LDD  R26,Y+19
	STD  Z+0,R26
_0x3BD:
_0x3BB:
	SBRS R16,5
	LDI  R20,LOW(6)
	CPI  R18,102
	BRNE _0x3BF
	CALL SUBOPT_0x57
	CALL __PUTPARD1
	ST   -Y,R20
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoa_GD
	RJMP _0x3C0
_0x3BF:
	CALL SUBOPT_0x57
	CALL __PUTPARD1
	ST   -Y,R20
	ST   -Y,R18
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoe_GD
_0x3C0:
	MOVW R30,R28
	ADIW R30,20
	CALL SUBOPT_0x117
	RJMP _0x3C1
_0x3B7:
	CPI  R30,LOW(0x73)
	BRNE _0x3C3
	CALL SUBOPT_0x114
	CALL SUBOPT_0x118
	CALL SUBOPT_0x117
	RJMP _0x3C4
_0x3C3:
	CPI  R30,LOW(0x70)
	BRNE _0x3C6
	CALL SUBOPT_0x114
	CALL SUBOPT_0x118
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x3C4:
	ANDI R16,LOW(127)
	CPI  R20,0
	BREQ _0x3C8
	CP   R20,R17
	BRLO _0x3C9
_0x3C8:
	RJMP _0x3C7
_0x3C9:
	MOV  R17,R20
_0x3C7:
_0x3C1:
	LDI  R20,LOW(0)
	LDI  R30,LOW(0)
	STD  Y+18,R30
	LDI  R19,LOW(0)
	RJMP _0x3CA
_0x3C6:
	CPI  R30,LOW(0x64)
	BREQ _0x3CD
	CPI  R30,LOW(0x69)
	BRNE _0x3CE
_0x3CD:
	ORI  R16,LOW(4)
	RJMP _0x3CF
_0x3CE:
	CPI  R30,LOW(0x75)
	BRNE _0x3D0
_0x3CF:
	LDI  R30,LOW(10)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x3D1
	__GETD1N 0x3B9ACA00
	CALL SUBOPT_0x110
	LDI  R17,LOW(10)
	RJMP _0x3D2
_0x3D1:
	__GETD1N 0x2710
	CALL SUBOPT_0x110
	LDI  R17,LOW(5)
	RJMP _0x3D2
_0x3D0:
	CPI  R30,LOW(0x58)
	BRNE _0x3D4
	ORI  R16,LOW(8)
	RJMP _0x3D5
_0x3D4:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x413
_0x3D5:
	LDI  R30,LOW(16)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x3D7
	__GETD1N 0x10000000
	CALL SUBOPT_0x110
	LDI  R17,LOW(8)
	RJMP _0x3D2
_0x3D7:
	__GETD1N 0x1000
	CALL SUBOPT_0x110
	LDI  R17,LOW(4)
_0x3D2:
	CPI  R20,0
	BREQ _0x3D8
	ANDI R16,LOW(127)
	RJMP _0x3D9
_0x3D8:
	LDI  R20,LOW(1)
_0x3D9:
	SBRS R16,1
	RJMP _0x3DA
	CALL SUBOPT_0x114
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	RJMP _0x478
_0x3DA:
	SBRS R16,2
	RJMP _0x3DC
	CALL SUBOPT_0x114
	CALL SUBOPT_0x118
	CALL __CWD1
	RJMP _0x478
_0x3DC:
	CALL SUBOPT_0x114
	CALL SUBOPT_0x118
	CLR  R22
	CLR  R23
_0x478:
	__PUTD1S 6
	SBRS R16,2
	RJMP _0x3DE
	CALL SUBOPT_0x45
	CALL __CPD20
	BRGE _0x3DF
	CALL SUBOPT_0x57
	CALL __ANEGD1
	CALL SUBOPT_0x44
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x3DF:
	LDD  R30,Y+19
	CPI  R30,0
	BREQ _0x3E0
	SUBI R17,-LOW(1)
	SUBI R20,-LOW(1)
	RJMP _0x3E1
_0x3E0:
	ANDI R16,LOW(251)
_0x3E1:
_0x3DE:
	MOV  R19,R20
_0x3CA:
	SBRC R16,0
	RJMP _0x3E2
_0x3E3:
	CP   R17,R21
	BRSH _0x3E6
	CP   R19,R21
	BRLO _0x3E7
_0x3E6:
	RJMP _0x3E5
_0x3E7:
	SBRS R16,7
	RJMP _0x3E8
	SBRS R16,2
	RJMP _0x3E9
	ANDI R16,LOW(251)
	LDD  R18,Y+19
	SUBI R17,LOW(1)
	RJMP _0x3EA
_0x3E9:
	LDI  R18,LOW(48)
_0x3EA:
	RJMP _0x3EB
_0x3E8:
	LDI  R18,LOW(32)
_0x3EB:
	CALL SUBOPT_0x113
	SUBI R21,LOW(1)
	RJMP _0x3E3
_0x3E5:
_0x3E2:
_0x3EC:
	CP   R17,R20
	BRSH _0x3EE
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x3EF
	CALL SUBOPT_0x119
	BREQ _0x3F0
	SUBI R21,LOW(1)
_0x3F0:
	SUBI R17,LOW(1)
	SUBI R20,LOW(1)
_0x3EF:
	LDI  R30,LOW(48)
	CALL SUBOPT_0x115
	CPI  R21,0
	BREQ _0x3F1
	SUBI R21,LOW(1)
_0x3F1:
	SUBI R20,LOW(1)
	RJMP _0x3EC
_0x3EE:
	MOV  R19,R17
	LDD  R30,Y+18
	CPI  R30,0
	BRNE _0x3F2
_0x3F3:
	CPI  R19,0
	BREQ _0x3F5
	SBRS R16,3
	RJMP _0x3F6
	CALL SUBOPT_0x116
	LPM  R30,Z
	RJMP _0x479
_0x3F6:
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X+
	STD  Y+10,R26
	STD  Y+10+1,R27
_0x479:
	ST   -Y,R30
	__GETW1SX 87
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,19
	ST   -Y,R31
	ST   -Y,R30
	CALL __put_GD
	CPI  R21,0
	BREQ _0x3F8
	SUBI R21,LOW(1)
_0x3F8:
	SUBI R19,LOW(1)
	RJMP _0x3F3
_0x3F5:
	RJMP _0x3F9
_0x3F2:
_0x3FB:
	CALL SUBOPT_0x10B
	CALL SUBOPT_0x45
	CALL __DIVD21U
	MOV  R18,R30
	CPI  R18,10
	BRLO _0x3FD
	SBRS R16,3
	RJMP _0x3FE
	SUBI R18,-LOW(55)
	RJMP _0x3FF
_0x3FE:
	SUBI R18,-LOW(87)
_0x3FF:
	RJMP _0x400
_0x3FD:
	SUBI R18,-LOW(48)
_0x400:
	SBRC R16,4
	RJMP _0x402
	CPI  R18,49
	BRSH _0x404
	CALL SUBOPT_0x10D
	__CPD2N 0x1
	BRNE _0x403
_0x404:
	RJMP _0x406
_0x403:
	CP   R20,R19
	BRLO _0x407
	RJMP _0x47A
_0x407:
	CP   R21,R19
	BRLO _0x409
	SBRS R16,0
	RJMP _0x40A
_0x409:
	RJMP _0x408
_0x40A:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x40B
_0x47A:
	LDI  R18,LOW(48)
_0x406:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x40C
	CALL SUBOPT_0x119
	BREQ _0x40D
	SUBI R21,LOW(1)
_0x40D:
_0x40C:
_0x40B:
_0x402:
	CALL SUBOPT_0x113
	CPI  R21,0
	BREQ _0x40E
	SUBI R21,LOW(1)
_0x40E:
_0x408:
	SUBI R19,LOW(1)
	CALL SUBOPT_0x10B
	CALL SUBOPT_0x45
	CALL __MODD21U
	CALL SUBOPT_0x44
	LDD  R30,Y+18
	CALL SUBOPT_0x10D
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __DIVD21U
	CALL SUBOPT_0x110
	CALL SUBOPT_0x10B
	CALL __CPD10
	BREQ _0x3FC
	RJMP _0x3FB
_0x3FC:
_0x3F9:
	SBRS R16,0
	RJMP _0x40F
_0x410:
	CPI  R21,0
	BREQ _0x412
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	CALL SUBOPT_0x115
	RJMP _0x410
_0x412:
_0x40F:
_0x413:
_0x3B1:
_0x477:
	LDI  R17,LOW(0)
_0x392:
	RJMP _0x38D
_0x38F:
	CALL __LOADLOCR6
	ADIW R28,63
	ADIW R28,29
	RET
_sprintf:
	PUSH R15
	MOV  R15,R24
	SBIW R28,2
	ST   -Y,R17
	ST   -Y,R16
	MOVW R26,R28
	CALL __ADDW2R15
	MOVW R16,R26
	MOVW R26,R28
	ADIW R26,6
	CALL __ADDW2R15
	CALL __GETW1P
	STD  Y+2,R30
	STD  Y+2+1,R31
	MOVW R26,R28
	ADIW R26,4
	CALL __ADDW2R15
	CALL __GETW1P
	ST   -Y,R31
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	MOVW R30,R28
	ADIW R30,6
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(0)
	LDI  R31,HIGH(0)
	ST   -Y,R31
	ST   -Y,R30
	CALL __print_GD
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	LDI  R30,LOW(0)
	ST   X,R30
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,4
	POP  R15
	RET
_atof:
	SBIW R28,10
	CALL __SAVELOCR6
	__CLRD1S 8
_0x414:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isspace
	CPI  R30,0
	BREQ _0x416
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x414
_0x416:
	LDI  R30,LOW(0)
	STD  Y+7,R30
	CPI  R21,43
	BRNE _0x417
	RJMP _0x47B
_0x417:
	CPI  R21,45
	BRNE _0x419
	LDI  R30,LOW(1)
	STD  Y+7,R30
_0x47B:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x419:
	LDI  R30,LOW(0)
	MOV  R20,R30
	MOV  R21,R30
	__GETWRS 16,17,16
_0x41A:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BRNE _0x41D
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	LDI  R30,LOW(46)
	CALL __EQB12
	MOV  R21,R30
	CPI  R30,0
	BREQ _0x41C
_0x41D:
	OR   R20,R21
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x41A
_0x41C:
	__GETWRS 18,19,16
	CPI  R20,0
	BREQ _0x41F
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x420:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	CPI  R26,LOW(0x2E)
	BREQ _0x422
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x11A
	CALL SUBOPT_0xE5
	CALL SUBOPT_0x4E
	__GETD1N 0x41200000
	CALL __DIVF21
	CALL SUBOPT_0x67
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x420
_0x422:
_0x41F:
	CALL SUBOPT_0x81
	CALL SUBOPT_0x110
_0x423:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,1
	STD  Y+16,R26
	STD  Y+16+1,R27
	CP   R26,R16
	CPC  R27,R17
	BRLO _0x425
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x10D
	CALL SUBOPT_0xE5
	CALL SUBOPT_0x6A
	CALL SUBOPT_0x10F
	CALL SUBOPT_0x110
	RJMP _0x423
_0x425:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R21,X
	CPI  R21,101
	BREQ _0x427
	CPI  R21,69
	BREQ _0x427
	RJMP _0x426
_0x427:
	LDI  R30,LOW(0)
	MOV  R20,R30
	STD  Y+6,R30
	MOVW R26,R18
	LD   R21,X
	CPI  R21,43
	BRNE _0x429
	RJMP _0x47C
_0x429:
	CPI  R21,45
	BRNE _0x42B
	LDI  R30,LOW(1)
	STD  Y+6,R30
_0x47C:
	__ADDWRN 18,19,1
_0x42B:
_0x42C:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BREQ _0x42E
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R30,R21
	SUBI R30,LOW(48)
	MOV  R20,R30
	RJMP _0x42C
_0x42E:
	CPI  R20,39
	BRLO _0x42F
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x430
	__GETD1N 0xFF7FFFFF
	RJMP _0x445
_0x430:
	__GETD1N 0x7F7FFFFF
	RJMP _0x445
_0x42F:
	LDI  R21,LOW(32)
	CALL SUBOPT_0x81
	CALL SUBOPT_0x110
_0x431:
	CPI  R21,0
	BREQ _0x433
	CALL SUBOPT_0x10B
	CALL SUBOPT_0x10D
	CALL __MULF12
	CALL SUBOPT_0x110
	MOV  R30,R20
	AND  R30,R21
	BREQ _0x434
	CALL SUBOPT_0x10F
	CALL SUBOPT_0x110
_0x434:
	LSR  R21
	RJMP _0x431
_0x433:
	LDD  R30,Y+6
	CPI  R30,0
	BREQ _0x435
	CALL SUBOPT_0x10B
	CALL SUBOPT_0x11A
	CALL __DIVF21
	RJMP _0x47D
_0x435:
	CALL SUBOPT_0x10B
	CALL SUBOPT_0x11A
	CALL __MULF12
_0x47D:
	__PUTD1S 8
_0x426:
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x437
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x67
_0x437:
	CALL SUBOPT_0x6E
_0x445:
	CALL __LOADLOCR6
	ADIW R28,18
	RET
_log:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x45
	CALL __CPD02
	BRLT _0x438
	__GETD1N 0xFF7FFFFF
	RJMP _0x444
_0x438:
	RCALL SUBOPT_0x57
	CALL __PUTPARD1
	IN   R30,SPL
	IN   R31,SPH
	SBIW R30,1
	ST   -Y,R31
	ST   -Y,R30
	PUSH R17
	PUSH R16
	CALL _frexp
	POP  R16
	POP  R17
	RCALL SUBOPT_0x44
	RCALL SUBOPT_0x45
	__GETD1N 0x3F3504F3
	CALL __CMPF12
	BRSH _0x439
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x45
	CALL __ADDF12
	RCALL SUBOPT_0x44
	__SUBWRN 16,17,1
_0x439:
	RCALL SUBOPT_0x48
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x6F
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RCALL SUBOPT_0x5A
	CALL SUBOPT_0x11C
	RCALL SUBOPT_0x96
	__GETD2N 0x3F654226
	RCALL SUBOPT_0x19
	__GETD1N 0x4054114E
	RCALL SUBOPT_0x1B
	CALL SUBOPT_0x11C
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x97
	__GETD1N 0x3FD4114D
	RCALL SUBOPT_0x1B
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	MOVW R30,R16
	__GETD2N 0x3F317218
	CALL __CWD1
	CALL __CDF1
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDF12
_0x444:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,10
	RET
_exp:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x4C
	__GETD1N 0xC2AEAC50
	CALL __CMPF12
	BRSH _0x43A
	RCALL SUBOPT_0x66
	RJMP _0x443
_0x43A:
	RCALL SUBOPT_0x40
	CALL __CPD10
	BRNE _0x43B
	RCALL SUBOPT_0x81
	RJMP _0x443
_0x43B:
	RCALL SUBOPT_0x4C
	__GETD1N 0x42B17218
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x43C
	__GETD1N 0x7F7FFFFF
	RJMP _0x443
_0x43C:
	RCALL SUBOPT_0x4C
	__GETD1N 0x3FB8AA3B
	CALL __MULF12
	RCALL SUBOPT_0x3E
	RCALL SUBOPT_0x40
	CALL __PUTPARD1
	CALL _floor
	CALL __CFD1
	MOVW R16,R30
	MOVW R30,R16
	RCALL SUBOPT_0x4C
	CALL __CWD1
	CALL __CDF1
	RCALL SUBOPT_0x1B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F000000
	RCALL SUBOPT_0x1B
	RCALL SUBOPT_0x49
	CALL SUBOPT_0x11C
	RCALL SUBOPT_0x96
	__GETD2N 0x3D6C4C6D
	CALL __MULF12
	__GETD2N 0x40E6E3A6
	CALL __ADDF12
	CALL SUBOPT_0x11C
	RCALL SUBOPT_0x44
	RCALL SUBOPT_0x4A
	__GETD2N 0x41A68D28
	CALL __ADDF12
	RCALL SUBOPT_0x59
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x97
	CALL __ADDF12
	__GETD2N 0x3FB504F3
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x45
	RCALL SUBOPT_0x4A
	CALL __SUBF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	CALL __PUTPARD1
	ST   -Y,R17
	ST   -Y,R16
	CALL _ldexp
_0x443:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
_pow:
	SBIW R28,4
	RCALL SUBOPT_0x6E
	CALL __CPD10
	BRNE _0x43D
	RCALL SUBOPT_0x66
	RJMP _0x442
_0x43D:
	CALL SUBOPT_0x11A
	CALL __CPD02
	BRGE _0x43E
	RCALL SUBOPT_0x6D
	CALL __CPD10
	BRNE _0x43F
	RCALL SUBOPT_0x81
	RJMP _0x442
_0x43F:
	RCALL SUBOPT_0x6E
	RCALL SUBOPT_0x51
	CALL SUBOPT_0x111
	RCALL SUBOPT_0x69
	RJMP _0x442
_0x43E:
	RCALL SUBOPT_0x6D
	MOVW R26,R28
	CALL __CFD1
	CALL __PUTDP1
	__GETD1S 0
	CALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x6D
	CALL __CPD12
	BREQ _0x440
	RCALL SUBOPT_0x66
	RJMP _0x442
_0x440:
	CALL SUBOPT_0x11B
	RCALL SUBOPT_0x51
	CALL SUBOPT_0x111
	RCALL SUBOPT_0x69
	RCALL SUBOPT_0x67
	LD   R30,Y
	ANDI R30,LOW(0x1)
	BRNE _0x441
	RCALL SUBOPT_0x6E
	RJMP _0x442
_0x441:
	CALL SUBOPT_0x11B
_0x442:
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x0:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_namebuf)
	SBCI R31,HIGH(-_namebuf)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1:
	LDD  R30,Y+1
	LDI  R31,0
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	MOVW R26,R0
	ST   X,R30
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x2:
	__POINTW1MN _sysdata,14
	ADD  R30,R10
	ADC  R31,R11
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x3:
	__POINTW2MN _rundata,96
	MOVW R30,R10
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(2)
	MOV  R13,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x4:
	__POINTW2MN _tprbdata,480
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5:
	MOVW R30,R10
	ADIW R30,1
	ST   -Y,R30
	JMP  _scanner_set_channel

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x6:
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R31
	STS  _dlg_cnt+2,R22
	STS  _dlg_cnt+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x7:
	MOVW R30,R10
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x8:
	CALL __PUTDP1
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x9:
	__POINTW1FN _0,28
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	ADIW R30,1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0xA:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0xB:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL _prints
	__POINTW2MN _rundata,96
	MOVW R30,R10
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC:
	__POINTW1FN _0,36
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rundata,96
	MOVW R30,R10
	CALL __LSLW2
	RJMP SUBOPT_0xA

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0xD:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xE:
	__POINTW2MN _rundata,96
	MOVW R30,R10
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(5)
	MOV  R13,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF:
	__POINTW2MN _rprbdata,480
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x10:
	LDI  R30,LOW(105)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x11:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	CALL _display_buttons
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	__GETD2N 0x493E0
	CALL __CWD1
	CALL __MULD12
	RJMP SUBOPT_0x6

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x12:
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x13:
	CALL _nav_read
	__PUTD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x14:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x15:
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	__GETD2N 0x493E0
	CALL __CWD1
	CALL __MULD12
	RJMP SUBOPT_0x6

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x16:
	LDS  R26,_valuep
	LDS  R27,_valuep+1
	LDS  R24,_valuep+2
	LDS  R25,_valuep+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x17:
	__GETD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x18:
	__POINTW2MN _sysdata,8
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x19:
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1A:
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0x1B:
	CALL __SWAPD12
	CALL __SUBF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1C:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 42 TIMES, CODE SIZE REDUCTION:79 WORDS
SUBOPT_0x1D:
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1E:
	__GETD1MN _rundata,192
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x1F:
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x20:
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	LDI  R30,LOW(7)
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	LDI  R30,LOW(6)
	ST   -Y,R30
	JMP  _nav_command

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x21:
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x22:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _wnd_msgbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x23:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x24:
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
	RJMP SUBOPT_0x1D

;OPTIMIZER ADDED SUBROUTINE, CALLED 32 TIMES, CODE SIZE REDUCTION:121 WORDS
SUBOPT_0x25:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x26:
	CALL __PUTPARD1
	JMP  _wnd_floatinput

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x27:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	RJMP SUBOPT_0x14

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x28:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x29:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R6,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x2A:
	LDI  R30,LOW(_pgmain_handler)
	LDI  R31,HIGH(_pgmain_handler)
	LDS  R26,_curr_window
	LDS  R27,_curr_window+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0x2B:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:23 WORDS
SUBOPT_0x2C:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
	RJMP SUBOPT_0x28

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2D:
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:42 WORDS
SUBOPT_0x2E:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x2F:
	__POINTW1MN _tprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x30:
	__POINTW1MN _rprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x31:
	CALL __MULW12U
	ADD  R30,R22
	ADC  R31,R23
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x32:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
	ST   -Y,R17
	ST   -Y,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x33:
	LD   R16,Y+
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 39 TIMES, CODE SIZE REDUCTION:73 WORDS
SUBOPT_0x34:
	SUBI R30,LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x35:
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R15
	ST   -Y,R22
	ST   -Y,R23
	ST   -Y,R24
	ST   -Y,R25
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
	ST   -Y,R17
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:81 WORDS
SUBOPT_0x36:
	LD   R17,Y+
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R25,Y+
	LD   R24,Y+
	LD   R23,Y+
	LD   R22,Y+
	LD   R15,Y+
	LD   R1,Y+
	LD   R0,Y+
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x37:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	STD  Y+4,R26
	STD  Y+4+1,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x38:
	__POINTW2MN _rprbdata,480
	LDD  R30,Y+30
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x39:
	__GETD2S 31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3A:
	CALL __DIVF21
	__PUTD1S 31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3B:
	LDI  R31,0
	RJMP SUBOPT_0x1F

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3C:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F800000
	RJMP SUBOPT_0x1B

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x3D:
	__PUTD1S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 19 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x3E:
	__PUTD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x3F:
	__GETD2S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x40:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x41:
	CALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3851B717
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x42:
	MOVW R26,R16
	__ADDWRN 16,17,1
	CPI  R26,LOW(0x64)
	LDI  R30,HIGH(0x64)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x43:
	RCALL SUBOPT_0x40
	RCALL SUBOPT_0x3F
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x40000000
	CALL __DIVF21
	__PUTD1S 2
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 19 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x44:
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 21 TIMES, CODE SIZE REDUCTION:37 WORDS
SUBOPT_0x45:
	__GETD2S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x46:
	__GETD1N 0x3F800000
	RJMP SUBOPT_0x1B

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x47:
	CALL __MULF12
	RCALL SUBOPT_0x45
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x48:
	RCALL SUBOPT_0x45
	RJMP SUBOPT_0x46

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x49:
	RCALL SUBOPT_0x44
	__GETD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x4A:
	__GETD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4B:
	RCALL SUBOPT_0x3F
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 18 TIMES, CODE SIZE REDUCTION:31 WORDS
SUBOPT_0x4C:
	__GETD2S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4D:
	__GETD1N 0x461C4000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x4E:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x4F:
	__GETD1N 0x40000000
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	CALL _floor
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x4D
	CALL __DIVF21
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x50:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x4C
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x51:
	CALL __PUTPARD1
	RJMP _log

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x52:
	CALL __DIVF21
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x53:
	RCALL SUBOPT_0x4A
	RCALL SUBOPT_0x3E
	__GETWRN 16,17,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x54:
	MOVW R26,R16
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x55:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RCALL SUBOPT_0x4C
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x56:
	RCALL SUBOPT_0x44
	RCALL SUBOPT_0x4A
	RCALL SUBOPT_0x4C
	CALL __MULF12
	RJMP SUBOPT_0x3E

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x57:
	__GETD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x58:
	CALL __PUTPARD1
	RJMP _exp

;OPTIMIZER ADDED SUBROUTINE, CALLED 15 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x59:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5A:
	CALL __DIVF21
	RJMP SUBOPT_0x49

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5B:
	CALL __GETD1PF
	RCALL SUBOPT_0x59
	__GETWRN 16,17,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x5C:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RCALL SUBOPT_0x4C
	CALL __MULF12
	__GETD2S 2
	CALL __ADDF12
	RCALL SUBOPT_0x59
	RCALL SUBOPT_0x57
	RCALL SUBOPT_0x4C
	CALL __MULF12
	RJMP SUBOPT_0x3E

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x5D:
	__GETD2S 13
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x5E:
	STS  _coef,R30
	STS  _coef+1,R31
	RJMP SUBOPT_0x5D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x5F:
	__GETD2S 13
	__GETD1N 0x441DA75C
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x60:
	STS  _coef,R30
	STS  _coef+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x61:
	__GETD2S 13
	__GETD1N 0x443E0000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x62:
	__GETD2S 13
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x63:
	__GETD1N 0x448505C3
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x64:
	RCALL SUBOPT_0x62
	__GETD1N 0x44D01000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x65:
	RCALL SUBOPT_0x60
	RCALL SUBOPT_0x62
	RJMP SUBOPT_0x63

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x66:
	__GETD1N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x67:
	__PUTD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x68:
	RCALL SUBOPT_0x62
	__GETD1N 0x42FDEFEC
	RJMP SUBOPT_0x1B

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x69:
	CALL __MULF12
	RJMP SUBOPT_0x58

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x6A:
	CALL __MULF12
	__GETD2S 8
	CALL __ADDF12
	RJMP SUBOPT_0x67

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x6B:
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6C:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x6D:
	__GETD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x6E:
	__GETD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6F:
	__GETD2N 0x3F800000
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x70:
	__GETD1N 0xC3870000
	RJMP SUBOPT_0x3D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x71:
	__GETD1N 0xC2480000
	RCALL SUBOPT_0x3D
	__GETD1N 0x44DD0000
	RJMP SUBOPT_0x3E

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x72:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x73:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_lcdcmd_G8)
	SBCI R31,HIGH(-_lcdcmd_G8)
	LD   R30,Z
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x74:
	ST   -Y,R30
	CALL _hex2char
	LDI  R30,LOW(_hex2ch_G8)
	LDI  R31,HIGH(_hex2ch_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x75:
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _onecmd
	__CLRD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x76:
	LDI  R30,LOW(3)
	STS  _lcdcmd_G8,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _sendcmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x77:
	STS  _lcdcmd_G8,R30
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,1
	LDD  R30,Y+2
	__PUTB1MN _lcdcmd_G8,2
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,3
	LD   R30,Y
	__PUTB1MN _lcdcmd_G8,4
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _sendcmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x78:
	MOV  R30,R17
	LDI  R31,0
	__ADDW1MN _lcdcmd_G8,3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x79:
	ST   -Y,R30
	LDD  R30,Y+4
	ST   -Y,R30
	LDD  R30,Y+4
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ST   -Y,R31
	ST   -Y,R30
	JMP  _LCD_PrintChar

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x7A:
	LDI  R26,LOW(10)
	STD  Z+0,R26
	LDI  R30,LOW(_scancmd)
	LDI  R31,HIGH(_scancmd)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x7B:
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x7C:
	LDI  R30,LOW(_navcmd_G9)
	LDI  R31,HIGH(_navcmd_G9)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x7D:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	__GETD1N 0xC47A0000
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RJMP SUBOPT_0x7C

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x7E:
	LDI  R30,LOW(9)
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,0
	STS  _navtime,R30
	STS  _navtime+1,R30
	STS  _navtime+2,R30
	STS  _navtime+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x7F:
	__GETD1N 0xC47A0000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x80:
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x81:
	__GETD1N 0x3F800000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x82:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	RCALL SUBOPT_0x7F
	RJMP SUBOPT_0x80

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x83:
	LDS  R30,_navlen_G9
	LDI  R31,0
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x84:
	MOVW R30,R6
	MOV  R16,R30
	LDI  R17,LOW(0)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0x85:
	__POINTW1FN _0,159
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(10)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _draw_label
	MOVW R30,R6
	ADIW R30,1
	MOVW R6,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R30,R6
	CPC  R31,R7
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x86:
	MOV  R6,R16
	CLR  R7
	LDI  R30,LOW(254)
	STD  Y+4,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x87:
	MOVW R30,R6
	ADIW R30,1
	MOVW R6,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R30,R6
	CPC  R31,R7
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x88:
	__POINTW2MN _sysdata,14
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x89:
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	MOVW R18,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x8A:
	__POINTW2MN _rundata,96
	MOVW R30,R6
	SBIW R30,1
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x8B:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0xC60CA000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x8C:
	LDI  R30,LOW(10)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_values*2)
	SBCI R31,HIGH(-_pgmain_values*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x8D:
	LDI  R30,LOW(10)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x8E:
	__POINTW1FN _0,176
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	SBIW R30,1
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RJMP SUBOPT_0xA

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x8F:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	__POINTW1MN _strbuf,8
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x90:
	__POINTW1FN _0,184
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0x8A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x91:
	__POINTW1FN _0,189
	ST   -Y,R31
	ST   -Y,R30
	MOVW R26,R18
	LD   R30,X
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RJMP SUBOPT_0x8D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x92:
	LDI  R30,LOW(10)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	CALL __GETW1PF
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x93:
	LDI  R30,LOW(221)
	ST   -Y,R30
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x94:
	SBIW R30,2
	ST   -Y,R30
	LDI  R30,LOW(233)
	ST   -Y,R30
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x95:
	__POINTW2MN _sysdata,38
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x96:
	RCALL SUBOPT_0x59
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x97:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 42 TIMES, CODE SIZE REDUCTION:161 WORDS
SUBOPT_0x98:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x99:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgr_options*2)
	SBCI R31,HIGH(-_pgr_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x9A:
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x9B:
	STS  _max_databuf,R30
	RJMP SUBOPT_0x1D

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0x9C:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x9D:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgt_options*2)
	SBCI R31,HIGH(-_pgt_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x9E:
	CALL __SAVELOCR6
	LDI  R16,6
	LDI  R17,1
	LDI  R18,24
	LDI  R19,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x9F:
	LDS  R30,_curr_pos_S47
	LDS  R31,_curr_pos_S47+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA0:
	STS  _last_pos_S47,R30
	STS  _last_pos_S47+1,R31
	RJMP SUBOPT_0x9F

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xA1:
	STS  _curr_pos_S47,R30
	STS  _curr_pos_S47+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xA2:
	LDS  R26,_curr_pos_S47
	LDS  R27,_curr_pos_S47+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA3:
	MOV  R30,R18
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xA4:
	RCALL SUBOPT_0x9F
	LDS  R26,_last_pos_S47
	LDS  R27,_last_pos_S47+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xA5:
	MOV  R30,R19
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA6:
	SUBI R26,LOW(48)
	MUL  R16,R26
	MOVW R30,R0
	CP   R18,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xA7:
	SUBI R26,LOW(49)
	MUL  R16,R26
	MOVW R30,R0
	SUBI R30,-LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA8:
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(255)
	CALL __EEPROMWRB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA9:
	__POINTW1FN _0,387
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0xAA:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0xAB:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xAC:
	MOV  R30,R17
	LDI  R31,0
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xAD:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xAE:
	LDS  R26,_curr_pos_S47
	LDS  R27,_curr_pos_S47+1
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xAF:
	CALL __DIVW21U
	MOVW R26,R30
	MOV  R30,R16
	LDI  R31,0
	CALL __MULW12U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB0:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,LOW(0xFF)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB1:
	__POINTW1FN _0,390
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0x9A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xB2:
	__POINTW1FN _0,397
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB3:
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB4:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xB5:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgch_items*2)
	SBCI R31,HIGH(-_pgch_items*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB6:
	CALL __MODW21U
	MOVW R26,R30
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB7:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xB8:
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB9:
	STS  _curr_pos_S48,R30
	STS  _curr_pos_S48+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xBA:
	LDS  R30,_curr_pos_S48
	LDS  R31,_curr_pos_S48+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBB:
	STS  _last_pos_S48,R30
	STS  _last_pos_S48+1,R31
	RJMP SUBOPT_0xBA

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xBC:
	LDS  R26,_curr_pos_S48
	LDS  R27,_curr_pos_S48+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xBD:
	RCALL SUBOPT_0xBA
	LDS  R26,_last_pos_S48
	LDS  R27,_last_pos_S48+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xBE:
	MOV  R30,R16
	MOVW R26,R20
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xBF:
	LDS  R26,_curr_pos_S48
	LDS  R27,_curr_pos_S48+1
	RJMP SUBOPT_0xB8

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xC0:
	__POINTW1FN _0,424
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R20
	CALL __CWD1
	CALL __PUTPARD1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,1
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xC1:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x98

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xC2:
	CALL __MODW21U
	MOVW R26,R30
	MOVW R30,R20
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xC3:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x9C

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC4:
	STS  _curr_pos_S49,R30
	STS  _curr_pos_S49+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xC5:
	LDS  R30,_curr_pos_S49
	LDS  R31,_curr_pos_S49+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC6:
	RCALL SUBOPT_0xC5
	STS  _last_pos_S49,R30
	STS  _last_pos_S49+1,R31
	RJMP SUBOPT_0xC5

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xC7:
	LDS  R26,_curr_pos_S49
	LDS  R27,_curr_pos_S49+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC8:
	RCALL SUBOPT_0xC5
	LDS  R26,_last_pos_S49
	LDS  R27,_last_pos_S49+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xC9:
	LDS  R26,_curr_pos_S49
	LDS  R27,_curr_pos_S49+1
	RJMP SUBOPT_0xB8

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xCA:
	__POINTW1FN _0,402
	ST   -Y,R31
	ST   -Y,R30
	MOV  R30,R20
	SUBI R30,LOW(1)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCB:
	CALL __PUTPARD1
	RJMP SUBOPT_0xAB

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xCC:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0x98

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xCD:
	CALL __MODW21U
	MOVW R26,R30
	MOV  R30,R21
	RJMP SUBOPT_0x34

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xCE:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0x9C

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xCF:
	__POINTW2MN _tprbdata,480
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD0:
	__POINTW2MN _rprbdata,480
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xD1:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R8
	RJMP SUBOPT_0x9A

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD2:
	MOV  R30,R19
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_tplist_options*2)
	SBCI R31,HIGH(-_tplist_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xD3:
	LDS  R30,_curr_sel_S4A
	LDS  R31,_curr_sel_S4A+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD4:
	STS  _last_sel_S4A,R30
	STS  _last_sel_S4A+1,R31
	RJMP SUBOPT_0xD3

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD5:
	STS  _curr_sel_S4A,R30
	STS  _curr_sel_S4A+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD6:
	LDS  R26,_curr_sel_S4A
	LDS  R27,_curr_sel_S4A+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD7:
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD8:
	MOV  R30,R17
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xD9:
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xDA:
	__POINTW1FN _0,401
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xDB:
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	RJMP SUBOPT_0xAA

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xDC:
	__POINTW1FN _0,681
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xDD:
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xDE:
	CALL __EEPROMRDD
	RJMP SUBOPT_0xCB

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xDF:
	__POINTW2MN _rprbdata,96
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE0:
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE1:
	CALL __EEPROMRDD
	RJMP SUBOPT_0x26

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE2:
	CALL __EEPROMWRD
	LDI  R30,LOW(11)
	MOV  R4,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xE3:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_boot_options*2)
	SBCI R31,HIGH(-_boot_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE4:
	RCALL SUBOPT_0x97
	__GETD1N 0x41200000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xE5:
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE6:
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE7:
	LDI  R30,LOW(0)
	STS  _databuf,R30
	STS  _pos_databuf,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE8:
	CALL _LCD_Cls
	LDI  R30,LOW(_datalbl*2)
	LDI  R31,HIGH(_datalbl*2)
	RJMP SUBOPT_0x98

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE9:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _draw_inputbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:37 WORDS
SUBOPT_0xEA:
	LDS  R30,_pos_databuf
	SUBI R30,-LOW(1)
	STS  _pos_databuf,R30
	RJMP SUBOPT_0x34

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:52 WORDS
SUBOPT_0xEB:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0xEC:
	LDS  R30,_pos_databuf
	RJMP SUBOPT_0x34

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xED:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x30)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xEE:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(57)
	STD  Z+0,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xEF:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF0:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x39)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF1:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF2:
	LDS  R30,_max_databuf
	LDS  R26,_pos_databuf
	CP   R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0xF3:
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	ST   Z,R17
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xF4:
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CLR  R30
	ADD  R26,R16
	ADC  R27,R30
	RJMP SUBOPT_0xD7

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF5:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	RJMP SUBOPT_0x98

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:38 WORDS
SUBOPT_0xF6:
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,2
	CALL __GETW1PF
	SBIW R30,10
	ST   -Y,R30
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,4
	CALL __GETW1PF
	SBIW R30,3
	ST   -Y,R30
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	CALL __LSLW4
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(14)
	LDI  R31,HIGH(14)
	CALL __SWAPW12
	SUB  R30,R26
	SBC  R31,R27
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	CALL __GETW1PF
	ADIW R30,19
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF7:
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,2
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF8:
	ST   -Y,R30
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xF9:
	ST   -Y,R30
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	CALL __LSLW4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFA:
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xFB:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:67 WORDS
SUBOPT_0xFC:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,2
	LPM  R30,Z
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	LPM  R30,Z
	ST   -Y,R30
	LDD  R30,Y+4
	LDD  R31,Y+4+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFD:
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	MOV  R30,R17
	LSL  R30
	LSL  R30
	LSL  R30
	RJMP SUBOPT_0xE6

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xFE:
	ST   -Y,R30
	LDD  R30,Y+5
	LDD  R31,Y+5+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xFF:
	ADIW R30,8
	CALL __GETW1PF
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x100:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,2
	LPM  R30,Z
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	LPM  R30,Z
	ST   -Y,R30
	ST   -Y,R17
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x101:
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	MOV  R30,R17
	SWAP R30
	ANDI R30,0xF0
	RJMP SUBOPT_0xE6

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x102:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,2
	LPM  R30,Z
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	LPM  R30,Z
	ST   -Y,R30
	MOV  R30,R17
	LSL  R30
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x103:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x104:
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x105:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x106:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x107:
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	JMP  _floor

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x108:
	MOV  R30,R16
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x109:
	__GETD1N 0x41200000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x10A:
	__GETD2S 4
	RJMP SUBOPT_0x109

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x10B:
	__GETD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x10C:
	RCALL SUBOPT_0x6D
	__GETD2S 12
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x10D:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x10E:
	__GETD1N 0x41200000
	CALL __DIVF21
	__PUTD1S 12
	SUBI R19,-LOW(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10F:
	RCALL SUBOPT_0x10D
	RJMP SUBOPT_0x109

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x110:
	__PUTD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x111:
	__GETD2S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x112:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADIW R26,1
	STD  Y+8,R26
	STD  Y+8+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x113:
	ST   -Y,R18
	__GETW1SX 87
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,19
	ST   -Y,R31
	ST   -Y,R30
	JMP  __put_GD

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x114:
	MOVW R26,R28
	SUBI R26,LOW(-(88))
	SBCI R27,HIGH(-(88))
	LD   R30,X+
	LD   R31,X+
	SBIW R30,4
	ST   -X,R31
	ST   -X,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x115:
	ST   -Y,R30
	__GETW1SX 87
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	ADIW R30,19
	ST   -Y,R31
	ST   -Y,R30
	JMP  __put_GD

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x116:
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ADIW R30,1
	STD  Y+10,R30
	STD  Y+10+1,R31
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x117:
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x118:
	ADIW R30,4
	MOVW R26,R30
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x119:
	ANDI R16,LOW(251)
	LDD  R30,Y+19
	ST   -Y,R30
	__GETW1SX 87
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R28
	SUBI R30,LOW(-(87))
	SBCI R31,HIGH(-(87))
	ST   -Y,R31
	ST   -Y,R30
	CALL __put_GD
	CPI  R21,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x11A:
	__GETD2S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x11B:
	RCALL SUBOPT_0x6E
	CALL __ANEGF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x11C:
	RCALL SUBOPT_0x45
	CALL __MULF12
	RET


	.DSEG
_3:
	.BYTE 0x0C
_4:
	.BYTE 0x10
_480:
	.BYTE 0x0F
_481:
	.BYTE 0x10
_482:
	.BYTE 0x0B
_483:
	.BYTE 0x0D
_484:
	.BYTE 0x0F
_485:
	.BYTE 0x0F
_508:
	.BYTE 0x0F
_509:
	.BYTE 0x11
_510:
	.BYTE 0x11
_529:
	.BYTE 0x09
_574:
	.BYTE 0x13
_658:
	.BYTE 0x08
_659:
	.BYTE 0x07
_660:
	.BYTE 0x09
_661:
	.BYTE 0x09
_662:
	.BYTE 0x09
_663:
	.BYTE 0x09
_664:
	.BYTE 0x09
_665:
	.BYTE 0x09
_666:
	.BYTE 0x09
_716:
	.BYTE 0x09
_717:
	.BYTE 0x07
_718:
	.BYTE 0x04
_719:
	.BYTE 0x04
_720:
	.BYTE 0x04
_736:
	.BYTE 0x0F
_737:
	.BYTE 0x0D
_738:
	.BYTE 0x0D
_739:
	.BYTE 0x0D
_740:
	.BYTE 0x0D

	.CSEG
_isdigit:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,'0'
	brlo __isdigit0
	cpi  r31,'9'+1
	brlo __isdigit1
__isdigit0:
	clr  r30
__isdigit1:
	ret

_isspace:
	ldi  r30,1
	ld   r31,y+
	cpi  r31,' '
	breq __isspace1
	cpi  r31,9
	brlo __isspace0
	cpi  r31,14
	brlo __isspace1
__isspace0:
	clr  r30
__isspace1:
	ret

_strlen:
	ld   r26,y+
	ld   r27,y+
	clr  r30
	clr  r31
__strlen0:
	ld   r22,x+
	tst  r22
	breq __strlen1
	adiw r30,1
	rjmp __strlen0
__strlen1:
	ret

_strlenf:
	clr  r26
	clr  r27
	ld   r30,y+
	ld   r31,y+
__strlenf0:
	lpm  r0,z+
	tst  r0
	breq __strlenf1
	adiw r26,1
	rjmp __strlenf0
__strlenf1:
	movw r30,r26
	ret

__ftrunc:
	ldd  r23,y+3
	ldd  r22,y+2
	ldd  r31,y+1
	ld   r30,y
	bst  r23,7
	lsl  r23
	sbrc r22,7
	sbr  r23,1
	mov  r25,r23
	subi r25,0x7e
	breq __ftrunc0
	brcs __ftrunc0
	cpi  r25,24
	brsh __ftrunc1
	clr  r26
	clr  r27
	clr  r24
__ftrunc2:
	sec
	ror  r24
	ror  r27
	ror  r26
	dec  r25
	brne __ftrunc2
	and  r30,r26
	and  r31,r27
	and  r22,r24
	rjmp __ftrunc1
__ftrunc0:
	clt
	clr  r23
	clr  r30
	clr  r31
	clr  r22
__ftrunc1:
	cbr  r22,0x80
	lsr  r23
	brcc __ftrunc3
	sbr  r22,0x80
__ftrunc3:
	bld  r23,7
	ld   r26,y+
	ld   r27,y+
	ld   r24,y+
	ld   r25,y+
	cp   r30,r26
	cpc  r31,r27
	cpc  r22,r24
	cpc  r23,r25
	bst  r25,7
	ret

_floor:
	rcall __ftrunc
	brne __floor1
__floor0:
	ret
__floor1:
	brtc __floor0
	ldi  r25,0xbf

__addfc:
	clr  r26
	clr  r27
	ldi  r24,0x80
	rjmp __addf12

__ADDW2R15:
	CLR  R0
	ADD  R26,R15
	ADC  R27,R0
	RET

__ANEGW1:
	COM  R30
	COM  R31
	ADIW R30,1
	RET

__ANEGD1:
	COM  R30
	COM  R31
	COM  R22
	COM  R23
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__LSLW4:
	LSL  R30
	ROL  R31
__LSLW3:
	LSL  R30
	ROL  R31
__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__CBD1:
	MOV  R31,R30
	ADD  R31,R31
	SBC  R31,R31
	MOV  R22,R31
	MOV  R23,R31
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__EQB12:
	CP   R30,R26
	LDI  R30,1
	BREQ __EQB12T
	CLR  R30
__EQB12T:
	RET

__MULW12U:
	MUL  R31,R26
	MOV  R31,R0
	MUL  R30,R27
	ADD  R31,R0
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	RET

__MULD12U:
	MUL  R23,R26
	MOV  R23,R0
	MUL  R22,R27
	ADD  R23,R0
	MUL  R31,R24
	ADD  R23,R0
	MUL  R30,R25
	ADD  R23,R0
	MUL  R22,R26
	MOV  R22,R0
	ADD  R23,R1
	MUL  R31,R27
	ADD  R22,R0
	ADC  R23,R1
	MUL  R30,R24
	ADD  R22,R0
	ADC  R23,R1
	CLR  R24
	MUL  R31,R26
	MOV  R31,R0
	ADD  R22,R1
	ADC  R23,R24
	MUL  R30,R27
	ADD  R31,R0
	ADC  R22,R1
	ADC  R23,R24
	MUL  R30,R26
	MOV  R30,R0
	ADD  R31,R1
	ADC  R22,R24
	ADC  R23,R24
	RET

__MULW12:
	RCALL __CHKSIGNW
	RCALL __MULW12U
	BRTC __MULW121
	RCALL __ANEGW1
__MULW121:
	RET

__MULD12:
	RCALL __CHKSIGND
	RCALL __MULD12U
	BRTC __MULD121
	RCALL __ANEGD1
__MULD121:
	RET

__DIVB21U:
	CLR  R0
	LDI  R25,8
__DIVB21U1:
	LSL  R26
	ROL  R0
	SUB  R0,R30
	BRCC __DIVB21U2
	ADD  R0,R30
	RJMP __DIVB21U3
__DIVB21U2:
	SBR  R26,1
__DIVB21U3:
	DEC  R25
	BRNE __DIVB21U1
	MOV  R30,R26
	MOV  R26,R0
	RET

__DIVB21:
	RCALL __CHKSIGNB
	RCALL __DIVB21U
	BRTC __DIVB211
	NEG  R30
__DIVB211:
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOVW R30,R26
	MOVW R26,R0
	RET

__DIVD21U:
	PUSH R19
	PUSH R20
	PUSH R21
	CLR  R0
	CLR  R1
	CLR  R20
	CLR  R21
	LDI  R19,32
__DIVD21U1:
	LSL  R26
	ROL  R27
	ROL  R24
	ROL  R25
	ROL  R0
	ROL  R1
	ROL  R20
	ROL  R21
	SUB  R0,R30
	SBC  R1,R31
	SBC  R20,R22
	SBC  R21,R23
	BRCC __DIVD21U2
	ADD  R0,R30
	ADC  R1,R31
	ADC  R20,R22
	ADC  R21,R23
	RJMP __DIVD21U3
__DIVD21U2:
	SBR  R26,1
__DIVD21U3:
	DEC  R19
	BRNE __DIVD21U1
	MOVW R30,R26
	MOVW R22,R24
	MOVW R26,R0
	MOVW R24,R20
	POP  R21
	POP  R20
	POP  R19
	RET

__MODB21:
	CLT
	SBRS R26,7
	RJMP __MODB211
	NEG  R26
	SET
__MODB211:
	SBRC R30,7
	NEG  R30
	RCALL __DIVB21U
	MOV  R30,R26
	BRTC __MODB212
	NEG  R30
__MODB212:
	RET

__MODB21U:
	RCALL __DIVB21U
	MOV  R30,R26
	RET

__MODW21U:
	RCALL __DIVW21U
	MOVW R30,R26
	RET

__MODD21U:
	RCALL __DIVD21U
	MOVW R30,R26
	MOVW R22,R24
	RET

__CHKSIGNB:
	CLT
	SBRS R30,7
	RJMP __CHKSB1
	NEG  R30
	SET
__CHKSB1:
	SBRS R26,7
	RJMP __CHKSB2
	NEG  R26
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSB2:
	RET

__CHKSIGNW:
	CLT
	SBRS R31,7
	RJMP __CHKSW1
	RCALL __ANEGW1
	SET
__CHKSW1:
	SBRS R27,7
	RJMP __CHKSW2
	COM  R26
	COM  R27
	ADIW R26,1
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSW2:
	RET

__CHKSIGND:
	CLT
	SBRS R23,7
	RJMP __CHKSD1
	RCALL __ANEGD1
	SET
__CHKSD1:
	SBRS R25,7
	RJMP __CHKSD2
	CLR  R0
	COM  R26
	COM  R27
	COM  R24
	COM  R25
	ADIW R26,1
	ADC  R24,R0
	ADC  R25,R0
	BLD  R0,0
	INC  R0
	BST  R0,0
__CHKSD2:
	RET

__GETW1P:
	LD   R30,X+
	LD   R31,X
	SBIW R26,1
	RET

__GETD1P:
	LD   R30,X+
	LD   R31,X+
	LD   R22,X+
	LD   R23,X
	SBIW R26,3
	RET

__PUTDP1:
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	RET

__GETW1PF:
	LPM  R0,Z+
	LPM  R31,Z
	MOV  R30,R0
	RET

__GETD1PF:
	LPM  R0,Z+
	LPM  R1,Z+
	LPM  R22,Z+
	LPM  R23,Z
	MOVW R30,R0
	RET

__PUTPARD1:
	ST   -Y,R23
	ST   -Y,R22
	ST   -Y,R31
	ST   -Y,R30
	RET

__SWAPD12:
	MOV  R1,R24
	MOV  R24,R22
	MOV  R22,R1
	MOV  R1,R25
	MOV  R25,R23
	MOV  R23,R1

__SWAPW12:
	MOV  R1,R27
	MOV  R27,R31
	MOV  R31,R1

__SWAPB12:
	MOV  R1,R26
	MOV  R26,R30
	MOV  R30,R1
	RET

__EEPROMRDD:
	ADIW R26,2
	RCALL __EEPROMRDW
	MOV  R23,R31
	MOV  R22,R30
	SBIW R26,2

__EEPROMRDW:
	ADIW R26,1
	RCALL __EEPROMRDB
	MOV  R31,R30
	SBIW R26,1

__EEPROMRDB:
	SBIC EECR,EEWE
	RJMP __EEPROMRDB
	PUSH R31
	IN   R31,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R30,EEDR
	OUT  SREG,R31
	POP  R31
	RET

__EEPROMWRD:
	RCALL __EEPROMWRW
	ADIW R26,2
	MOV  R0,R30
	MOV  R1,R31
	MOV  R30,R22
	MOV  R31,R23
	RCALL __EEPROMWRW
	MOV  R30,R0
	MOV  R31,R1
	SBIW R26,2
	RET

__EEPROMWRW:
	RCALL __EEPROMWRB
	ADIW R26,1
	PUSH R30
	MOV  R30,R31
	RCALL __EEPROMWRB
	POP  R30
	SBIW R26,1
	RET

__EEPROMWRB:
	SBIC EECR,EEWE
	RJMP __EEPROMWRB
	IN   R25,SREG
	CLI
	OUT  EEARL,R26
	OUT  EEARH,R27
	SBI  EECR,EERE
	IN   R24,EEDR
	CP   R30,R24
	BREQ __EEPROMWRB0
	OUT  EEDR,R30
	SBI  EECR,EEMWE
	SBI  EECR,EEWE
__EEPROMWRB0:
	OUT  SREG,R25
	RET

_frexp:
	LD   R26,Y+
	LD   R27,Y+
	LD   R30,Y+
	LD   R31,Y+
	LD   R22,Y+
	LD   R23,Y+
	BST  R23,7
	LSL  R22
	ROL  R23
	CLR  R24
	SUBI R23,0x7E
	SBC  R24,R24
	ST   X+,R23
	ST   X,R24
	LDI  R23,0x7E
	LSR  R23
	ROR  R22
	BRTS __ANEGF1
	RET

_ldexp:
	LD   R26,Y+
	LD   R27,Y+
	LD   R30,Y+
	LD   R31,Y+
	LD   R22,Y+
	LD   R23,Y+
	BST  R23,7
	LSL  R22
	ROL  R23
	ADD  R23,R26
	LSR  R23
	ROR  R22
	BRTS __ANEGF1
	RET

__ANEGF1:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __ANEGF10
	SUBI R23,0x80
__ANEGF10:
	RET

__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1U:
	SET
	RJMP __CFD1U0
__CFD1:
	CLT
__CFD1U0:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRTC __CFD19
	CPI  R21,9
__CFD19:
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	BLD  R23,7
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__SWAPACC:
	PUSH R20
	MOVW R20,R30
	MOVW R30,R26
	MOVW R26,R20
	MOVW R20,R22
	MOVW R22,R24
	MOVW R24,R20
	MOV  R20,R0
	MOV  R0,R1
	MOV  R1,R20
	POP  R20
	RET

__UADD12:
	ADD  R30,R26
	ADC  R31,R27
	ADC  R22,R24
	RET

__NEGMAN1:
	COM  R30
	COM  R31
	COM  R22
	SUBI R30,-1
	SBCI R31,-1
	SBCI R22,-1
	RET

__SUBF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129
	LDI  R21,0x80
	EOR  R1,R21

	RJMP __ADDF120

__ADDF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R25,0x80
	BREQ __ADDF129

__ADDF120:
	CPI  R23,0x80
	BREQ __ADDF128
__ADDF121:
	MOV  R21,R23
	SUB  R21,R25
	BRVS __ADDF129
	BRPL __ADDF122
	RCALL __SWAPACC
	RJMP __ADDF121
__ADDF122:
	CPI  R21,24
	BRLO __ADDF123
	CLR  R26
	CLR  R27
	CLR  R24
__ADDF123:
	CPI  R21,8
	BRLO __ADDF124
	MOV  R26,R27
	MOV  R27,R24
	CLR  R24
	SUBI R21,8
	RJMP __ADDF123
__ADDF124:
	TST  R21
	BREQ __ADDF126
__ADDF125:
	LSR  R24
	ROR  R27
	ROR  R26
	DEC  R21
	BRNE __ADDF125
__ADDF126:
	MOV  R21,R0
	EOR  R21,R1
	BRMI __ADDF127
	RCALL __UADD12
	BRCC __ADDF129
	ROR  R22
	ROR  R31
	ROR  R30
	INC  R23
	BRVC __ADDF129
	RJMP __MAXRES
__ADDF128:
	RCALL __SWAPACC
__ADDF129:
	RCALL __REPACK
	POP  R21
	RET
__ADDF127:
	SUB  R30,R26
	SBC  R31,R27
	SBC  R22,R24
	BREQ __ZERORES
	BRCC __ADDF1210
	COM  R0
	RCALL __NEGMAN1
__ADDF1210:
	TST  R22
	BRMI __ADDF129
	LSL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVC __ADDF1210

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R0
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R17
	CLR  R18
	CLR  R25
	MUL  R22,R24
	MOVW R20,R0
	MUL  R24,R31
	MOV  R19,R0
	ADD  R20,R1
	ADC  R21,R25
	MUL  R22,R27
	ADD  R19,R0
	ADC  R20,R1
	ADC  R21,R25
	MUL  R24,R30
	RCALL __MULF126
	MUL  R27,R31
	RCALL __MULF126
	MUL  R22,R26
	RCALL __MULF126
	MUL  R27,R30
	RCALL __MULF127
	MUL  R26,R31
	RCALL __MULF127
	MUL  R26,R30
	ADD  R17,R1
	ADC  R18,R25
	ADC  R19,R25
	ADC  R20,R25
	ADC  R21,R25
	MOV  R30,R19
	MOV  R31,R20
	MOV  R22,R21
	MOV  R21,R18
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	POP  R0
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__MULF127:
	ADD  R17,R0
	ADC  R18,R1
	ADC  R19,R25
	RJMP __MULF128
__MULF126:
	ADD  R18,R0
	ADC  R19,R1
__MULF128:
	ADC  R20,R25
	ADC  R21,R25
	RET

__DIVF21:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BRNE __DIVF210
	TST  R1
__DIVF211:
	BRPL __DIVF219
	RJMP __MINRES
__DIVF219:
	RJMP __MAXRES
__DIVF210:
	CPI  R25,0x80
	BRNE __DIVF218
__DIVF217:
	RJMP __ZERORES
__DIVF218:
	EOR  R0,R1
	SEC
	SBC  R25,R23
	BRVC __DIVF216
	BRLT __DIVF217
	TST  R0
	RJMP __DIVF211
__DIVF216:
	MOV  R23,R25
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R1
	CLR  R17
	CLR  R18
	CLR  R19
	CLR  R20
	CLR  R21
	LDI  R25,32
__DIVF212:
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	CPC  R20,R17
	BRLO __DIVF213
	SUB  R26,R30
	SBC  R27,R31
	SBC  R24,R22
	SBC  R20,R17
	SEC
	RJMP __DIVF214
__DIVF213:
	CLC
__DIVF214:
	ROL  R21
	ROL  R18
	ROL  R19
	ROL  R1
	ROL  R26
	ROL  R27
	ROL  R24
	ROL  R20
	DEC  R25
	BRNE __DIVF212
	MOVW R30,R18
	MOV  R22,R1
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	TST  R22
	BRMI __DIVF215
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	DEC  R23
	BRVS __DIVF217
__DIVF215:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__CMPF12:
	TST  R25
	BRMI __CMPF120
	TST  R23
	BRMI __CMPF121
	CP   R25,R23
	BRLO __CMPF122
	BRNE __CMPF121
	CP   R26,R30
	CPC  R27,R31
	CPC  R24,R22
	BRLO __CMPF122
	BREQ __CMPF123
__CMPF121:
	CLZ
	CLC
	RET
__CMPF122:
	CLZ
	SEC
	RET
__CMPF123:
	SEZ
	CLC
	RET
__CMPF120:
	TST  R23
	BRPL __CMPF122
	CP   R25,R23
	BRLO __CMPF121
	BRNE __CMPF122
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	BRLO __CMPF122
	BREQ __CMPF123
	RJMP __CMPF121

__CPD10:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	RET

__CPW02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	RET

__CPD20:
	SBIW R26,0
	SBCI R24,0
	SBCI R25,0
	RET

__CPD02:
	CLR  R0
	CP   R0,R26
	CPC  R0,R27
	CPC  R0,R24
	CPC  R0,R25
	RET

__CPD12:
	CP   R30,R26
	CPC  R31,R27
	CPC  R22,R24
	CPC  R23,R25
	RET

__SAVELOCR6:
	ST   -Y,R21
__SAVELOCR5:
	ST   -Y,R20
__SAVELOCR4:
	ST   -Y,R19
__SAVELOCR3:
	ST   -Y,R18
__SAVELOCR2:
	ST   -Y,R17
	ST   -Y,R16
	RET

__LOADLOCR6:
	LDD  R21,Y+5
__LOADLOCR5:
	LDD  R20,Y+4
__LOADLOCR4:
	LDD  R19,Y+3
__LOADLOCR3:
	LDD  R18,Y+2
__LOADLOCR2:
	LDD  R17,Y+1
	LD   R16,Y
	RET

__INITLOCB:
__INITLOCW:
	ADD R26,R28
	ADC R27,R29
__INITLOC0:
	LPM  R0,Z+
	ST   X+,R0
	DEC  R24
	BRNE __INITLOC0
	RET

;END OF CODE MARKER
__END_OF_CODE:
