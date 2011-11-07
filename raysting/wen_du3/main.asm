
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
;      38 //#include "math.h"
;      39 extern void pgmain_handler(uchar);	//running menu
;      40 extern void pgrconfig_handler(uchar);	//main menu of R config
;      41 extern void pgtconfig_handler(uchar);	//main menu of T config
;      42 extern void pgchset_handler(uchar);	//menu of channel probe setup
;      43 extern void pgprblist_handler(uchar);	//list of probe selection
;      44 extern void pgprbtypelist_handler(uchar); //select probe type
;      45 extern void pgprbset_handler(uchar);	//menu of probe setup
;      46 extern void pgprbconfig_handler(uchar);  //config of probe
;      47 extern void pgboottype_handler(uchar msg) ; //boot up config
;      48 extern void com1_putstr(char *s);
;      49 extern unsigned int strlen(char *s);
;      50 RUNDATA rundata;
_rundata:
	.BYTE 0xC8
;      51 SYSDATA eeprom sysdata = {

	.ESEG
_sysdata:
;      52 	0,0,100,3,
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x42C8
	.DW  0x3
;      53 	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      54 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      55 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
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
;      56 	{0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      57 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      58 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},
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
;      59 	1,1};
	.DB  0x1
	.DB  0x1
;      60 PRBDATA	eeprom tprbdata = {
_tprbdata:
;      61        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      62 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      63 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      64 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      65 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      66 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      67 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      68 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      69 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      70 	{25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
;      71 	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
;      72 	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0},
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
;      73 	{"11","","","","","","","",
;      74 	"","","","","","","","",
;      75 	"","","","","","","",""},
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
;      76        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      77 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      78 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
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
;      79 };
;      80 PRBDATA	eeprom rprbdata = {
_rprbdata:
;      81        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      82 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      83 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      84 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      85 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      86 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      87 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      88 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      89 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
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
;      90 	{25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
;      91 	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0,
;      92 	25.0,25.0,25.0,25.0,25.0,25.0,25.0,25.0},
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
	.DW  0x0
	.DW  0x41C8
;      93 	{"11","","","","","","","",
;      94 	"","","","","","","","",
;      95 	"","","","","","","",""},
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
;      96        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      97 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      98 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
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
;      99 };
;     100 
;     101 char  strbuf[50];        //buffer for menu display

	.DSEG
_strbuf:
	.BYTE 0x32
;     102 uchar key;
;     103 
;     104 
;     105 
;     106 LABEL flash bootup = {LBL_HZ16,30,30,7,"正在启动..."};

	.CSEG
;     107 LABEL flash modify = {LBL_HZ16,30,30,8,"正在更新数据..."};
;     108 
;     109 LABEL flash lbldbg = {LBL_HZ6X8,10,30,8,strbuf};
;     110 
;     111 uchar nextwin = 0;
;     112 
;     113 int   curr_ch = 1;	//index of current channel in config menu window, 1 based

	.DSEG
;     114 int   curr_prb = 1;	//index of current probe selection in menu window 1 based
;     115 int   curr_dispch = 1;  //index of start channel in main running window, 1 based
;     116 int ch_to_search = 0;   //current channel , 0 based
;     117 
;     118 unsigned long dlg_cnt = 0;        //delay count time display
_dlg_cnt:
	.BYTE 0x4
;     119 unsigned long onesec_cnt = 0;
_onesec_cnt:
	.BYTE 0x4
;     120 uchar phase = 0;       //state machine index
_phase:
	.BYTE 0x1
;     121 double valuep;
_valuep:
	.BYTE 0x4
;     122 
;     123 /**************************************************************************************/
;     124 //                      Timer 1 overflow interrupt service routine
;     125 /**************************************************************************************/
;     126 interrupt [TIM1_OVF] void timer1_ovf_isr(void)
;     127 {

	.CSEG
_timer1_ovf_isr:
;     128 // Place your flash here
;     129 }
	RETI
;     130 void dbgout(double val)
;     131 {
_dbgout:
;     132         sprintf(strbuf,"%f\r\n",val);
;	val -> Y+0
	CALL SUBOPT_0x0
	__POINTW1FN _0,28
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x1
	CALL SUBOPT_0x2
;     133         prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     134 }
	RJMP _0x49A
;     135 void InitLED()
;     136 {
_InitLED:
;     137 /*
;     138     PORTB.7=1;
;     139     PORTB.6=1;
;     140     PORTB.5=1;
;     141     PORTB.4=1;
;     142     PORTD.7=1;
;     143     PORTD.6=0;
;     144 */
;     145 }
	RET
;     146 uchar nav1v = 1;

	.DSEG
_nav1v:
	.BYTE 0x1
;     147 void navto120mv()
;     148 {

	.CSEG
_navto120mv:
;     149    nav1v = 0;
	LDI  R30,LOW(0)
	STS  _nav1v,R30
;     150    dbgout(-0.12);
	__GETD1N 0xBDF5C28F
	CALL SUBOPT_0x4
;     151    nav_command(NAV_120MV);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x5
;     152    sleepms(20000);
;     153 }
	RET
;     154 
;     155 void navto1v()
;     156 {
_navto1v:
;     157     nav1v = 1;
	LDI  R30,LOW(1)
	STS  _nav1v,R30
;     158     dbgout(-1.0);
	__GETD1N 0xBF800000
	CALL SUBOPT_0x4
;     159     nav_command(NAV_INIT);
	LDI  R30,LOW(1)
	CALL SUBOPT_0x5
;     160     sleepms(20000);
;     161     nav_command(NAV_1V);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x5
;     162     sleepms(20000);
;     163     nav_command(NAV_SLOWMODE);
	LDI  R30,LOW(7)
	CALL SUBOPT_0x5
;     164     sleepms(20000);
;     165     nav_command(NAV_AFLTON);
	LDI  R30,LOW(6)
	CALL SUBOPT_0x5
;     166     sleepms(20000);
;     167 }
	RET
;     168 double mabs(double val)
;     169 {
_mabs:
;     170         if(val < 0)
;	val -> Y+0
	CALL SUBOPT_0x6
	CALL __CPD20
	BRGE _0x9
;     171                 return -val;
	CALL SUBOPT_0x7
	CALL __ANEGF1
	RJMP _0x49A
;     172         else
_0x9:
;     173                 return val;
	CALL SUBOPT_0x7
;     174 }
_0x49A:
	ADIW R28,4
	RET
;     175 u8 ToByte(char l, char h)
;     176 {
;     177 	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
;	l -> Y+1
;	h -> Y+0
;     178 	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
;     179 	return (h<<4)|l;
;     180 }
;     181 
;     182 /**************************************************************************************/
;     183 //                              Global Variable
;     184 /**************************************************************************************/
;     185 
;     186 /**************************************************************************************/
;     187 //                               Watchdog
;     188 // Enable watchdog timer.
;     189 // Timeout limit WDTCR[2:0]:
;     190 // 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
;     191 // 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
;     192 /**************************************************************************************/
;     193 #define WDCE 4
;     194 #define WDE  3
;     195 
;     196 
;     197 
;     198 void Turn_on_Watchdog()
;     199 {
;     200   #asm("cli")
;     201   #asm("wdr")                    // reset watchdog timer to avoid timeout reset
;     202   WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
;     203   WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
;     204   #asm("sei")                    // Enable global interrupt.
;     205 }
;     206 
;     207 char namebuf[8];

	.DSEG
_namebuf:
	.BYTE 0x8
;     208 char* tname2b(u8 i)
;     209 {

	.CSEG
_tname2b:
;     210         u8 j=0;
;     211         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x12:
	CPI  R17,8
	BRSH _0x13
;     212         {
;     213             namebuf[j] = tprbdata.name[i][j];
	CALL SUBOPT_0x8
	MOVW R0,R30
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0x9
;     214             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x13
;     215                 break;
;     216 
;     217             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x16
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x18
_0x16:
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x19
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x18
_0x19:
	RJMP _0x15
_0x18:
;     218                 continue;
	RJMP _0x11
;     219             namebuf[0] = '\0';
_0x15:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     220             break;
	RJMP _0x13
;     221         }
_0x11:
	SUBI R17,-1
	RJMP _0x12
_0x13:
;     222         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x499
;     223 }
;     224 
;     225 char* rname2b(u8 i)
;     226 {
_rname2b:
;     227         u8 j=0;
;     228         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x1D:
	CPI  R17,8
	BRSH _0x1E
;     229         {
;     230             namebuf[j] = rprbdata.name[i][j];
	CALL SUBOPT_0x8
	MOVW R0,R30
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0x9
;     231             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x1E
;     232                 break;
;     233 
;     234             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x21
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x23
_0x21:
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x24
	CALL SUBOPT_0x8
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x23
_0x24:
	RJMP _0x20
_0x23:
;     235                 continue;
	RJMP _0x1C
;     236             namebuf[0] = '\0';
_0x20:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     237             break;
	RJMP _0x1E
;     238         }
_0x1C:
	SUBI R17,-1
	RJMP _0x1D
_0x1E:
;     239         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x499
;     240 }
;     241 void sleepms(u16 ms)
;     242 {
_sleepms:
;     243     while(ms-- > 0)
;	ms -> Y+0
_0x27:
	LD   R26,Y
	LDD  R27,Y+1
	SBIW R26,1
	ST   Y,R26
	STD  Y+1,R27
	ADIW R26,1
	CALL __CPW02
	BRLO _0x27
;     244         ;
;     245 }
_0x499:
	ADIW R28,2
	RET
;     246 
;     247 
;     248 /**************************************************************************************/
;     249 //      board init related function.
;     250 /**************************************************************************************/
;     251 void init_var()
;     252 {
_init_var:
;     253 }
	RET
;     254 //state machine of therm type
;     255 //phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
;     256 //phase 1:get the reading and update the display
;     257 //phase 2: move to next channel
;     258 uchar therm_state()
;     259 {
_therm_state:
;     260         uchar i;
;     261 	i = sysdata.tid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	__POINTW1MN _sysdata,14
	CALL SUBOPT_0xA
;     262 	if(i == INVALID_PROBE)
	BRNE _0x2A
;     263 	{
;     264         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0xB
;     265         	phase = 2;
;     266 	}
;     267 	if(phase == 0)
_0x2A:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x2B
;     268 	{
;     269 		if( (tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0xC
	CPI  R30,LOW(0x3)
	BRLO _0x2D
	CALL SUBOPT_0xC
	CPI  R30,LOW(0xA)
	BRLO _0x2E
_0x2D:
	RJMP _0x2C
_0x2E:
;     270 		{
;     271 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0xD
;     272 			dlg_cnt = ONESEC;
	CALL SUBOPT_0xE
	CALL SUBOPT_0xF
;     273 			onesec_cnt = 0;
;     274 			phase = 1;
	LDI  R30,LOW(1)
	STS  _phase,R30
;     275 			return 0;
	LDI  R30,LOW(0)
	RJMP _0x498
;     276 		}else{
_0x2C:
;     277 			rundata.temperature[ch_to_search] = -9999; //invalid data
	CALL SUBOPT_0xB
;     278 			phase = 2;
;     279 		}
;     280 	}
;     281 	if(phase == 1)
_0x2B:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x30
;     282 	{
;     283 		rundata.reading[ch_to_search] = nav_read();//-sysdata.V0;
	CALL SUBOPT_0x10
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL _nav_read
	POP  R26
	POP  R27
	CALL __PUTDP1
;     284                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x11
	CALL SUBOPT_0x10
	CALL SUBOPT_0x12
	CALL SUBOPT_0x13
;     285                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     286 
;     287 		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
	CALL SUBOPT_0x14
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x10
	CALL SUBOPT_0x12
	CALL SUBOPT_0xC
	ST   -Y,R30
	CALL _MValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     288 //		if(ch_to_search==1) 		     rundata.temperature[ch_to_search] = 10;
;     289                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x15
	CALL SUBOPT_0x16
	CALL SUBOPT_0x2
;     290                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     291 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     292 	}
;     293 	if(phase == 2)
_0x30:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x31
;     294 	{
;     295 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x17
;     296 		phase = 0;
;     297 	}
;     298 	return 1;       //one reading is updated
_0x31:
	LDI  R30,LOW(1)
	RJMP _0x498
;     299 }
;     300 //state machine of bore measure
;     301 //phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
;     302 //phase1 get reading of rs+, set to nktt, add delay
;     303 //phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
;     304 //phase3 get reading of rx-, set to pktt, add delay
;     305 //phase4 get reading of rx+, switch to rx, caculate
;     306 //phase5 update display and move to next channel
;     307 uchar bore_state()
;     308 {
_bore_state:
;     309         uchar i;
;     310 	i = sysdata.rid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	__POINTW1MN _sysdata,38
	CALL SUBOPT_0xA
;     311 	if(i == INVALID_PROBE)
	BRNE _0x32
;     312 	{
;     313         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x14
	CALL SUBOPT_0x18
;     314         	phase = 5;
;     315 	}
;     316 	if(phase == 0)
_0x32:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x33
;     317 	{
;     318 		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0x19
	CPI  R30,LOW(0xF3)
	BRSH _0x35
	CALL SUBOPT_0x19
	CPI  R30,LOW(0xF1)
	BRSH _0x36
_0x35:
	RJMP _0x34
_0x36:
;     319 		{
;     320 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0xD
;     321 			SET_PKTT;
	CALL SUBOPT_0x1A
;     322 			SET_TORS;
	CALL SUBOPT_0x1B
;     323 			dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1C
;     324 			onesec_cnt = 0;
;     325 			if(IS_MODE_KTT)
	CALL SUBOPT_0x1D
	BRNE _0x37
;     326 				phase = 1;
	LDI  R30,LOW(1)
	RJMP _0x49B
;     327 			else
_0x37:
;     328 				phase = 2;
	LDI  R30,LOW(2)
_0x49B:
	STS  _phase,R30
;     329        			return 0;
	LDI  R30,LOW(0)
	RJMP _0x498
;     330 		}else{
_0x34:
;     331 			rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x14
	CALL SUBOPT_0x18
;     332 			phase = 5;
;     333 		}
;     334 
;     335 	}
;     336 	if(phase == 1)  //-stdV in ktt mode
_0x33:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x3A
;     337 	{
;     338 		rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x1F
;     339 		SET_NKTT;
	LDI  R30,LOW(105)
	CALL SUBOPT_0x20
;     340 		dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1C
;     341 		onesec_cnt = 0;
;     342 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     343 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x498
;     344 	}
;     345 	if(phase == 2) //final stdV
_0x3A:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x3B
;     346 	{
;     347 		if(IS_MODE_KTT)
	CALL SUBOPT_0x1D
	BRNE _0x3C
;     348 		{
;     349 			rundata.stdV = (rundata.stdV + mabs(nav_read()));
	CALL SUBOPT_0x1E
	__GETD2MN _rundata,196
	CALL __ADDF12
	CALL SUBOPT_0x1F
;     350 			phase = 3;
	LDI  R30,LOW(3)
	RJMP _0x49C
;     351 		}else{
_0x3C:
;     352 			rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x1F
;     353 			phase = 4;
	LDI  R30,LOW(4)
_0x49C:
	STS  _phase,R30
;     354 		}
;     355 		SET_TORX;
	CALL SUBOPT_0x21
;     356 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1C
;     357 		onesec_cnt = 0;
;     358 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x498
;     359 	}
;     360 	if(phase == 3)  //-V on rx in ktt mode
_0x3B:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x3E
;     361 	{
;     362 		valuep = mabs(nav_read());
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x22
;     363 		SET_PKTT;
	CALL SUBOPT_0x1A
;     364 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1C
;     365 		onesec_cnt = 0;
;     366 		phase = 4;
	LDI  R30,LOW(4)
	STS  _phase,R30
;     367 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x498
;     368 	}
;     369 	if(phase == 4) // final V on rx
_0x3E:
	LDS  R26,_phase
	CPI  R26,LOW(0x4)
	BREQ PC+3
	JMP _0x3F
;     370 	{
;     371 		if(IS_MODE_KTT){
	CALL SUBOPT_0x1D
	BRNE _0x40
;     372 			valuep = (valuep + mabs(nav_read()));
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x23
	CALL __ADDF12
	CALL SUBOPT_0x22
;     373 			dbgout(valuep);
	LDS  R30,_valuep
	LDS  R31,_valuep+1
	LDS  R22,_valuep+2
	LDS  R23,_valuep+3
	CALL SUBOPT_0x4
;     374 			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
	CALL SUBOPT_0x24
	BREQ PC+2
	BRCC PC+3
	JMP  _0x42
	CALL SUBOPT_0x23
	CALL SUBOPT_0x25
	CALL __CMPF12
	BRSH _0x42
	LDS  R26,_nav1v
	CPI  R26,LOW(0x0)
	BREQ _0x43
_0x42:
	RJMP _0x41
_0x43:
;     375 			{
;     376         		        navto1v();
	CALL _navto1v
;     377 			}
;     378 			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
_0x41:
	CALL SUBOPT_0x24
	BRSH _0x45
	CALL SUBOPT_0x23
	__GETD1N 0x3951B717
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x45
	LDS  R26,_nav1v
	CPI  R26,LOW(0x1)
	BREQ _0x46
_0x45:
	RJMP _0x44
_0x46:
;     379 			{
;     380 			        navto120mv();
	CALL _navto120mv
;     381 			}
;     382 		}else{
_0x44:
	RJMP _0x47
_0x40:
;     383 			valuep = mabs(nav_read());
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x22
;     384 			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
	CALL SUBOPT_0x26
	BREQ PC+2
	BRCC PC+3
	JMP  _0x49
	CALL SUBOPT_0x23
	CALL SUBOPT_0x27
	CALL __CMPF12
	BRSH _0x49
	LDS  R26,_nav1v
	CPI  R26,LOW(0x0)
	BREQ _0x4A
_0x49:
	RJMP _0x48
_0x4A:
;     385 			{
;     386         		        navto1v();
	CALL _navto1v
;     387 			}
;     388 			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
_0x48:
	CALL SUBOPT_0x26
	BRSH _0x4C
	CALL SUBOPT_0x23
	__GETD1N 0x38D1B717
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x4C
	LDS  R26,_nav1v
	CPI  R26,LOW(0x1)
	BREQ _0x4D
_0x4C:
	RJMP _0x4B
_0x4D:
;     389 			{
;     390 			        navto120mv();
	CALL _navto120mv
;     391 			}
;     392 		}
_0x4B:
_0x47:
;     393 		if(rundata.stdV != 0)
	CALL SUBOPT_0x28
	CALL __CPD10
	BRNE PC+3
	JMP _0x4E
;     394 		{
;     395 			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
	CALL SUBOPT_0x10
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x29
	CALL SUBOPT_0x23
	CALL SUBOPT_0x2A
	CALL SUBOPT_0x28
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2B
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x2C
	POP  R26
	POP  R27
	CALL __PUTDP1
;     396 			if(rundata.reading[ch_to_search] > 0)
	CALL SUBOPT_0x10
	CALL SUBOPT_0x16
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD02
	BRGE _0x4F
;     397 			{
;     398                                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x11
	CALL SUBOPT_0x2D
	CALL __PUTPARD1
	CALL SUBOPT_0x13
;     399                                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     400 
;     401 			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
	CALL SUBOPT_0x14
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x10
	CALL SUBOPT_0x12
	ST   -Y,R17
	RCALL _RValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     402                                 rundata.Rx = rundata.reading[ch_to_search];
	CALL SUBOPT_0x10
	CALL SUBOPT_0x16
	__PUTD1MN _rundata,192
;     403                                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x15
	CALL SUBOPT_0x16
	CALL SUBOPT_0x2
;     404                                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     405 			}
;     406 		}else{
_0x4F:
	RJMP _0x50
_0x4E:
;     407 		        rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x14
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
;     408 		}
_0x50:
;     409 		phase = 5;
	LDI  R30,LOW(5)
	STS  _phase,R30
;     410 	}
;     411 	if(phase == 5) //got one reading
_0x3F:
	LDS  R26,_phase
	CPI  R26,LOW(0x5)
	BRNE _0x51
;     412 	{
;     413 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x17
;     414 		phase = 0;
;     415 	}
;     416 	return 1;
_0x51:
	LDI  R30,LOW(1)
_0x498:
	LD   R17,Y+
	RET
;     417 }
;     418 LABEL flash statelbl = {LBL_HZ6X8,100,55,16,strbuf};
;     419 
;     420 void updatestate()
;     421 {
_updatestate:
;     422         char star[6];
;     423         sprintf(star,"    ");
	SBIW R28,6
;	star -> Y+0
	CALL SUBOPT_0x2E
	__POINTW1FN _0,47
	CALL SUBOPT_0x2F
;     424         if(phase == 0)                sprintf(star,"*   ");
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x52
	CALL SUBOPT_0x2E
	__POINTW1FN _0,52
	CALL SUBOPT_0x2F
;     425         if(phase == 1)                sprintf(star,"**  ");
_0x52:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x53
	CALL SUBOPT_0x2E
	__POINTW1FN _0,57
	CALL SUBOPT_0x2F
;     426         if(phase == 2)                sprintf(star,"*** ");
_0x53:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x54
	CALL SUBOPT_0x2E
	__POINTW1FN _0,62
	CALL SUBOPT_0x2F
;     427         if(phase == 3)                sprintf(star,"****");
_0x54:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x55
	CALL SUBOPT_0x2E
	__POINTW1FN _0,67
	CALL SUBOPT_0x2F
;     428 
;     429         if(IS_BORE_MODE){
_0x55:
	CALL SUBOPT_0x30
	BRNE _0x56
;     430                 if(IS_MODE_KTT)
	CALL SUBOPT_0x1D
	BRNE _0x57
;     431                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
;     432                 else
_0x57:
;     433                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
_0x49D:
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,72
	CALL SUBOPT_0x31
;     434         }else{
	RJMP _0x59
_0x56:
;     435                 sprintf(strbuf,"(ch:%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
	CALL SUBOPT_0x0
	__POINTW1FN _0,87
	CALL SUBOPT_0x31
;     436         }
_0x59:
;     437         draw_label(&statelbl,SW_NORMAL);
	LDI  R30,LOW(_statelbl*2)
	LDI  R31,HIGH(_statelbl*2)
	CALL SUBOPT_0x32
;     438 }
	ADIW R28,6
	RET
;     439 static uchar tA = 0xff;

	.DSEG
_tA_G1:
	.BYTE 0x1
;     440 static uchar tB = 0xff;
_tB_G1:
	.BYTE 0x1
;     441 void testA(uchar data)
;     442 {

	.CSEG
_testA:
;     443         tA = data;
;	data -> Y+0
	LD   R30,Y
	STS  _tA_G1,R30
;     444 }
	ADIW R28,1
	RET
;     445 void testB(uchar data)
;     446 {
;     447         tB = data;
;	data -> Y+0
;     448 }
;     449 /**************************************************************************************/
;     450 //                              Main Function Loop
;     451 /**************************************************************************************/
;     452 extern double GetThmoVolt(double t,char type);
;     453 extern u8 databuf[12];
;     454 extern u8 pos_databuf; //position in data buffer
;     455 void main(void)
;     456 {
_main:
;     457     u16 i;
;     458     /*  just test algrithom
;     459     sprintf(databuf,"9.99");
;     460     pos_databuf = 4;
;     461     dt = buf2double();
;     462     rprbdata.type[0] = PRBTYPE_PT100;
;     463     rprbdata.param1[0] = 3.9083e-3;
;     464     rprbdata.param2[0] = -5.775e-7;
;     465     rprbdata.param3[0] = 100;//-4.183e-12;
;     466     dt = RValueToTValue(139.26, 0);//102
;     467     */
;     468     // RS485 Node
;     469     init_var();	//init data structure
;	i -> R16,R17
	CALL _init_var
;     470     // System Initialization
;     471     Init_Port();
	RCALL _Init_Port
;     472 //    Init_Timers();
;     473 //    Init_Ex_Interrupt();
;     474     Init_UART();
	RCALL _Init_UART
;     475     Enable_XMEM();
	RCALL _Enable_XMEM
;     476     Init_554();
	RCALL _Init_554
;     477     InitLED();
	CALL _InitLED
;     478     Key_Init();
	CALL _Key_Init
;     479     // Global enable interrupts
;     480     WDTCR = 0x00; //disable dog watch
	LDI  R30,LOW(0)
	OUT  0x21,R30
;     481     #asm("sei")
	sei
;     482 
;     483     databuf[0] = 'A';
	LDI  R30,LOW(65)
	STS  _databuf,R30
;     484     databuf[1] = '5';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(53)
	STD  Z+0,R26
;     485     databuf[2] = '?';
	__POINTW1MN _databuf,2
	LDI  R26,LOW(63)
	STD  Z+0,R26
;     486     while(1)
_0x5C:
;     487     {
;     488             prints(databuf,3,PORT_SCANNER);
	CALL SUBOPT_0x33
	CALL SUBOPT_0x34
;     489             prints(databuf,3,PORT_PC);
	CALL SUBOPT_0x33
	LDI  R30,LOW(3)
	ST   -Y,R30
	RCALL _prints
;     490             sleepms(20000);
	LDI  R30,LOW(20000)
	LDI  R31,HIGH(20000)
	CALL SUBOPT_0x35
;     491     }
	RJMP _0x5C
;     492     /*********************************************************************/
;     493     // System hardware dection
;     494     /*********************************************************************/
;     495     // intialize LED.
;     496     nextwin = 0;
;     497 
;     498     sleepms(2000);
;     499     LCD_Init();
;     500     wnd_msgbox(&bootup);
;     501     //init the DMM
;     502     nav_command(NAV_INIT);
;     503     sleepms(20000);
;     504     nav_command(NAV_1V);
;     505     sleepms(20000);
;     506     nav_command(NAV_SLOWMODE);
;     507     sleepms(20000);
;     508     nav_command(NAV_AFLTON);
;     509     sleepms(20000);
;     510 
;     511     sleepms(2*ONESEC); //wait until all the node is ready after power up
;     512     State_Init();
;     513 
;     514     SET_BORE_MODE;
;     515 
;     516     SET_PKTT;
;     517     SET_TORS;
;     518 
;     519 	 nextwin = PG_BOOTTYPE;
;     520 	 key = KEY_INVALID;
;     521 
;     522 	 curr_ch = 1; //channel for display
;     523 	 curr_dispch = 1;
;     524 	 while(1)
_0x5F:
;     525 	 {
;     526  	 	if(nextwin != 0)
	TST  R4
	BREQ _0x62
;     527 		{
;     528 			SwitchWindow(nextwin);
	ST   -Y,R4
	RCALL _SwitchWindow
;     529 			(*curr_window)(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	__CALL1MN _curr_window,0
;     530 			nextwin = 0;
	CLR  R4
;     531 		}
;     532 		if(key != KEY_INVALID)
_0x62:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE PC+3
	JMP _0x63
;     533 		{
;     534 			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
	LDI  R30,LOW(97)
	CP   R30,R5
	BREQ _0x65
	LDI  R30,LOW(98)
	CP   R30,R5
	BREQ _0x65
	LDI  R30,LOW(99)
	CP   R30,R5
	BREQ _0x65
	LDI  R30,LOW(100)
	CP   R30,R5
	BREQ _0x65
	RJMP _0x64
_0x65:
;     535 			{
;     536 				if(curr_window == pgmain_handler)
	CALL SUBOPT_0x39
	BRNE _0x67
;     537 				{
;     538 					LCD_Cls();
	CALL _LCD_Cls
;     539 					wnd_msgbox(&modify);
	LDI  R30,LOW(_modify*2)
	LDI  R31,HIGH(_modify*2)
	CALL SUBOPT_0x36
;     540 				}
;     541 				if(key == KEY_BTN1) //mode switch
_0x67:
	LDI  R30,LOW(97)
	CP   R30,R5
	BRNE _0x68
;     542 				{
;     543 					if(IS_BORE_MODE){
	CALL SUBOPT_0x30
	BRNE _0x69
;     544 						SET_THERM_MODE;
	CALL SUBOPT_0x3A
;     545                                                 SET_TORS;
	CALL SUBOPT_0x1B
;     546                 				SET_PKTT;
	CALL SUBOPT_0x1A
;     547                                                 navto120mv();
	CALL _navto120mv
;     548 					}else{
	RJMP _0x6A
_0x69:
;     549 						SET_BORE_MODE;
	CALL SUBOPT_0x37
;     550                                                 SET_TORX;
	CALL SUBOPT_0x21
;     551                 				SET_PKTT;
	CALL SUBOPT_0x1A
;     552                                                 navto1v();
	CALL _navto1v
;     553 					}
_0x6A:
;     554 					dlg_cnt = 0;
	LDI  R30,0
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R30
	STS  _dlg_cnt+2,R30
	STS  _dlg_cnt+3,R30
;     555 					onesec_cnt = 0;
	CALL SUBOPT_0x3B
;     556 					phase = 0;      //reset the state machine
	LDI  R30,LOW(0)
	STS  _phase,R30
;     557 
;     558 					SET_PKTT;
	CALL SUBOPT_0x1A
;     559 				}
;     560 				if(key == KEY_BTN2) //auto ktt or not
_0x68:
	LDI  R30,LOW(98)
	CP   R30,R5
	BRNE _0x6B
;     561 				{
;     562 					if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x6C
;     563 					{
;     564 						if((IS_MODE_KTT)){
	CALL SUBOPT_0x1D
	BRNE _0x6D
;     565 							CLR_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(1)
	RJMP _0x49E
;     566 							SET_PKTT;
;     567 						}else{
_0x6D:
;     568 							SET_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(0)
_0x49E:
	ST   -Y,R30
	CALL _display_buttons
;     569 							SET_PKTT;
	CALL SUBOPT_0x1A
;     570 						}
;     571 					}
;     572 				}
_0x6C:
;     573 				if(key == KEY_BTN3) //thermal probe type
_0x6B:
	LDI  R30,LOW(99)
	CP   R30,R5
	BRNE _0x6F
;     574 				{
;     575 
;     576 					display_buttons(KEY_BTN3,1);
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
;     577 					if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x70
;     578 					{
;     579 					        i = sysdata.tid[curr_dispch-1];
	CALL SUBOPT_0x3D
	MOV  R16,R30
	CLR  R17
;     580 					        if(i != INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x71
;     581 					        {
;     582                 					if((tprbdata.type[i] >= PRBTYPE_K) &&\
;     583 	                				   (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0x3)
	BRLO _0x73
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0xA)
	BRLO _0x74
_0x73:
	RJMP _0x72
_0x74:
;     584 		                			{
;     585 			                			if(tprbdata.type[i] == PRBTYPE_R)
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0x9)
	BRNE _0x75
;     586 				                			tprbdata.type[i] = PRBTYPE_K;
	__POINTW1MN _tprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	LDI  R30,LOW(3)
	RJMP _0x49F
;     587 					                	else
_0x75:
;     588 						                	tprbdata.type[i] +=1;
	CALL SUBOPT_0x3E
	SUBI R30,-LOW(1)
_0x49F:
	CALL __EEPROMWRB
;     589         					        }
;     590         					}
_0x72:
;     591                                         }
_0x71:
;     592 					display_buttons(KEY_BTN3,0);
_0x70:
	LDI  R30,LOW(99)
	CALL SUBOPT_0x20
;     593 
;     594 				}
;     595 				if(key == KEY_BTN4) //remove zero
_0x6F:
	LDI  R30,LOW(100)
	CP   R30,R5
	BRNE _0x77
;     596 				{
;     597 					display_buttons(KEY_BTN4,1);
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
;     598 					if(IS_BORE_MODE){
	CALL SUBOPT_0x30
	BRNE _0x78
;     599 					        sysdata.R0 = rundata.Rx;
	CALL SUBOPT_0x2D
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMWRD
;     600 					}else{
	RJMP _0x79
_0x78:
;     601 					        //sysdata.V0 = nav_read();
;     602 					        nav_command(NAV_ZEROON);
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _nav_command
;     603 					        sleepms(ONESEC);
	LDI  R30,LOW(100000)
	LDI  R31,HIGH(100000)
	CALL SUBOPT_0x35
;     604 					}
_0x79:
;     605 					display_buttons(KEY_BTN4,0);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x20
;     606 				}
;     607 				if(curr_window == pgmain_handler)       //redraw the running window
_0x77:
	CALL SUBOPT_0x39
	BRNE _0x7A
;     608 				{
;     609 					pgmain_handler(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _pgmain_handler
;     610 				}
;     611 			}else{
_0x7A:
	RJMP _0x7B
_0x64:
;     612 				(*curr_window)(key);
	ST   -Y,R5
	__CALL1MN _curr_window,0
;     613 			}
_0x7B:
;     614 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     615 	  	}else{
	RJMP _0x7C
_0x63:
;     616 			if(curr_window != pgmain_handler)
	CALL SUBOPT_0x39
	BREQ _0x7D
;     617 				continue;
	RJMP _0x5F
;     618 
;     619 
;     620 			if(dlg_cnt > 1)
_0x7D:
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	__CPD2N 0x2
	BRLO _0x7E
;     621 			{
;     622 			        onesec_cnt++;
	LDS  R30,_onesec_cnt
	LDS  R31,_onesec_cnt+1
	LDS  R22,_onesec_cnt+2
	LDS  R23,_onesec_cnt+3
	CALL SUBOPT_0x3F
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R31
	STS  _onesec_cnt+2,R22
	STS  _onesec_cnt+3,R23
;     623 			        if(onesec_cnt == (ONESEC-10))
	CALL SUBOPT_0x40
	__CPD2N 0x18696
	BRNE _0x7F
;     624 			        {
;     625         			        updatestate();
	CALL _updatestate
;     626 
;     627         			}
;     628         			if(onesec_cnt == ONESEC)
_0x7F:
	CALL SUBOPT_0x40
	__CPD2N 0x186A0
	BRNE _0x80
;     629         			        onesec_cnt = 0 ;
	CALL SUBOPT_0x3B
;     630 				dlg_cnt--;
_0x80:
	LDS  R30,_dlg_cnt
	LDS  R31,_dlg_cnt+1
	LDS  R22,_dlg_cnt+2
	LDS  R23,_dlg_cnt+3
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R31
	STS  _dlg_cnt+2,R22
	STS  _dlg_cnt+3,R23
;     631 				continue;
	RJMP _0x5F
;     632 			}
;     633 			updatestate();
_0x7E:
	CALL _updatestate
;     634 			if((IS_THERM_MODE))
	CALL SUBOPT_0x3C
	BRNE _0x81
;     635 			{
;     636 				if(therm_state() == 0)
	CALL _therm_state
	CPI  R30,0
	BRNE _0x82
;     637 				        continue;
	RJMP _0x5F
;     638 			}else{
_0x82:
	RJMP _0x83
_0x81:
;     639 				if(bore_state() == 0)
	CALL _bore_state
	CPI  R30,0
	BRNE _0x84
;     640 				        continue;
	RJMP _0x5F
;     641 			}
_0x84:
_0x83:
;     642                         //shift to next channel
;     643                         while(true)
_0x85:
;     644                         {
;     645                                 ch_to_search += 1;
	MOVW R30,R12
	ADIW R30,1
	MOVW R12,R30
;     646         			if(ch_to_search >= MAX_CH_NUM)
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R12,R30
	CPC  R13,R31
	BRLT _0x88
;     647         			{
;     648 	        			ch_to_search = 0;
	CLR  R12
	CLR  R13
;     649 	        			break;
	RJMP _0x87
;     650 	        		}
;     651 	        		if(IS_THERM_MODE)
_0x88:
	CALL SUBOPT_0x3C
	BRNE _0x89
;     652 	        		{
;     653 	        		        i = sysdata.tid[ch_to_search];
	__POINTW1MN _sysdata,14
	RJMP _0x4A0
;     654 	        		}else{
_0x89:
;     655 	        		        i = sysdata.rid[ch_to_search];
	__POINTW1MN _sysdata,38
_0x4A0:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R16,R30
	CLR  R17
;     656 	        		}
;     657 	        		if(i == INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x85
;     658 	        		        continue;
;     659 	        		if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x8C
;     660 	        		{
;     661        					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0x3)
	BRLO _0x8E
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0xA)
	BRLO _0x8F
_0x8E:
	RJMP _0x8D
_0x8F:
;     662        					        break;
	RJMP _0x87
;     663 	        		}else{
_0x8D:
	RJMP _0x90
_0x8C:
;     664        		        		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0x41
	CPI  R30,LOW(0xF3)
	BRSH _0x92
	CALL SUBOPT_0x41
	CPI  R30,LOW(0xF1)
	BRSH _0x93
_0x92:
	RJMP _0x91
_0x93:
;     665        		        		        break;
	RJMP _0x87
;     666 	        		}
_0x91:
_0x90:
;     667 	                }
	RJMP _0x85
_0x87:
;     668 		}
_0x7C:
;     669 	}
	RJMP _0x5F
;     670 }
_0x94:
	RJMP _0x94
;     671 
;     672 WINDOW flash wins[]={
;     673 	{PG_MAIN,	pgmain_handler},	//running menu
;     674 	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
;     675 	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
;     676 	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
;     677 	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
;     678 	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
;     679 	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
;     680 	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
;     681 	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
;     682 };
;     683 
;     684 
;     685 void SwitchWindow(uchar page)
;     686 {
_SwitchWindow:
;     687 	u8 i;
;     688 	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	ST   -Y,R17
;	page -> Y+1
;	i -> R17
	LDI  R17,LOW(0)
_0x96:
	CPI  R17,9
	BRSH _0x97
;     689 	{
;     690 		if(wins[i].page_id == page)
	LDI  R30,LOW(3)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_wins*2)
	SBCI R31,HIGH(-_wins*2)
	LPM  R30,Z
	MOV  R26,R30
	LDD  R30,Y+1
	CP   R30,R26
	BRNE _0x98
;     691 		{
;     692 			curr_window = wins[i].handler;
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_wins,1
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	CALL SUBOPT_0x42
	STS  _curr_window,R30
	STS  _curr_window+1,R31
;     693 			return;
	LDD  R17,Y+0
	RJMP _0x497
;     694 		}
;     695 	}
_0x98:
	SUBI R17,-1
	RJMP _0x96
_0x97:
;     696 }
	LDD  R17,Y+0
_0x497:
	ADIW R28,2
	RET
;     697 /*********************************************************************************/
;     698 //                         UART0 and UART1 Subroutines
;     699 /*********************************************************************************/
;     700 #include <mega64.h>
;     701 #include "uart.h"
;     702 #include "16c554.h"
;     703 #include "global.h"
;     704 
;     705 //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
;     706 //extern u8 x_modem_databuf[];                       /* store data sent from PC */
;     707 //extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
;     708 //extern u8 x_modem_file_being_transferred;
;     709 //extern u8 x_modem_file_transfer_complete;
;     710 
;     711 /*********************************************************************************/
;     712 //                     UART0  Variables & Buffer Definition
;     713 /*********************************************************************************/
;     714 u8 volatile tx_buffer0[TX_BUFFER_SIZE0];

	.DSEG
_tx_buffer0:
	.BYTE 0x80
;     715 u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
;     716 
;     717 /*********************************************************************************/
;     718 //                     UART1  Variables & Buffer Definition
;     719 /*********************************************************************************/
;     720 u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
_tx_buffer1:
	.BYTE 0x80
;     721 u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;
_tx_wr_index1:
	.BYTE 0x1
_tx_rd_index1:
	.BYTE 0x1
_tx_counter1:
	.BYTE 0x1
;     722 
;     723 /*
;     724  * 0-9,a-f,A-F char to hex conversion
;     725  * otherwise 0xff will be returned.
;     726  */
;     727 u8 ishexchar(u8 dat){

	.CSEG
;     728         if( (dat >= '0') && (dat <= '9'))
;	dat -> Y+0
;     729                 return dat-'0';
;     730         if( (dat >= 'a') && (dat <= 'f'))
;     731                 return dat-'a'+0x0a;
;     732         if( (dat >= 'A') && (dat <= 'F'))
;     733                 return dat-'A'+0x0a;
;     734         return 0xff;
;     735 }
;     736 
;     737 
;     738 
;     739 interrupt [USART0_RXC] void usart0_rx_isr(void)
;     740 {
_usart0_rx_isr:
	CALL SUBOPT_0x43
;     741    u8 status,data;
;     742    //read status and data
;     743    status=UCSR0A;
;	status -> R17
;	data -> R16
	IN   R17,11
;     744    data=UDR0;
	IN   R16,12
;     745    cm0_push(data);
	ST   -Y,R16
	RCALL _kbd_uart_push
;     746 }
	CALL SUBOPT_0x44
	RETI
;     747 
;     748 /*********************************************************************************/
;     749 //                 USART0 Transmitter interrupt service routine
;     750 /*********************************************************************************/
;     751 interrupt [USART0_TXC] void usart0_tx_isr(void)
;     752 {
_usart0_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     753 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0xA2
;     754    {
;     755    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
;     756    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	CALL SUBOPT_0x45
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	OUT  0xC,R30
;     757    tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_rd_index0
	ANDI R30,0x7F
	STS  _tx_rd_index0,R30
;     758    };
_0xA2:
;     759 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     760 
;     761 /*********************************************************************************/
;     762 //                      UART1 Receiver interrupt service routine
;     763 /*********************************************************************************/
;     764 interrupt [USART1_RXC] void usart1_rx_isr(void)
;     765 {
_usart1_rx_isr:
	CALL SUBOPT_0x43
;     766    u8 status,data;
;     767 
;     768    //read status and data
;     769    status=UCSR1A;
;	status -> R17
;	data -> R16
	LDS  R17,155
;     770    data=UDR1;
	LDS  R16,156
;     771    cm1_push(data);
	ST   -Y,R16
	CALL _nav_uart_push
;     772 }
	CALL SUBOPT_0x44
	RETI
;     773 
;     774 /*********************************************************************************/
;     775 //                   USART1 Transmitter interrupt service routine
;     776 /*********************************************************************************/
;     777 interrupt [USART1_TXC] void usart1_tx_isr(void)
;     778 {
_usart1_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     779 if (tx_counter1)
	LDS  R30,_tx_counter1
	CPI  R30,0
	BREQ _0xA3
;     780    {
;     781    --tx_counter1;
	SUBI R30,LOW(1)
	STS  _tx_counter1,R30
;     782    UDR1=tx_buffer1[tx_rd_index1++];
	LDS  R30,_tx_rd_index1
	SUBI R30,-LOW(1)
	STS  _tx_rd_index1,R30
	CALL SUBOPT_0x45
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R30,Z
	STS  156,R30
;     783    tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_rd_index1
	ANDI R30,0x7F
	STS  _tx_rd_index1,R30
;     784    };
_0xA3:
;     785 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     786 
;     787 void com0_putc(char c)
;     788 {
_com0_putc:
;     789 if(DEBUG == 1)
;	c -> Y+0
;     790         return;
;     791 while (tx_counter0 == TX_BUFFER_SIZE0);
_0xA5:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x80)
	BREQ _0xA5
;     792 #asm("cli")
	cli
;     793 if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0xA9
	SBIC 0xB,5
	RJMP _0xA8
_0xA9:
;     794    {
;     795    tx_buffer0[tx_wr_index0++]=c;
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	CALL SUBOPT_0x45
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
;     796    tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_wr_index0
	ANDI R30,0x7F
	STS  _tx_wr_index0,R30
;     797    ++tx_counter0;
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
;     798    }
;     799 else
	RJMP _0xAB
_0xA8:
;     800    UDR0=c;
	LD   R30,Y
	OUT  0xC,R30
;     801 #asm("sei")
_0xAB:
	sei
;     802 }
	ADIW R28,1
	RET
;     803 
;     804 
;     805 void com1_putc(char c)
;     806 {
_com1_putc:
;     807 while (tx_counter1 == TX_BUFFER_SIZE1);
;	c -> Y+0
_0xAC:
	LDS  R26,_tx_counter1
	CPI  R26,LOW(0x80)
	BREQ _0xAC
;     808 #asm("cli")
	cli
;     809 if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter1
	CPI  R30,0
	BRNE _0xB0
	LDS  R30,155
	ANDI R30,LOW(0x20)
	BRNE _0xAF
_0xB0:
;     810    {
;     811    tx_buffer1[tx_wr_index1++]=c;
	LDS  R30,_tx_wr_index1
	SUBI R30,-LOW(1)
	STS  _tx_wr_index1,R30
	CALL SUBOPT_0x45
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R26,Y
	STD  Z+0,R26
;     812    tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_wr_index1
	ANDI R30,0x7F
	STS  _tx_wr_index1,R30
;     813    ++tx_counter1;
	LDS  R30,_tx_counter1
	SUBI R30,-LOW(1)
	STS  _tx_counter1,R30
;     814    }
;     815 else
	RJMP _0xB2
_0xAF:
;     816    UDR1=c;
	LD   R30,Y
	STS  156,R30
;     817 #asm("sei")
_0xB2:
	sei
;     818 }
	ADIW R28,1
	RET
;     819 
;     820 /*
;     821 void com0_puthex(u8 a)
;     822 {
;     823 	unsigned char h,l;
;     824 	h = (a & 0xf0) >> 4;
;     825 	l = (a & 0x0f);
;     826 	if(h <= 9)
;     827 		com0_putc(h+'0');
;     828 	else
;     829 		com0_putc(h+'A'-0x0a);
;     830 	if(l <= 9)
;     831 		com0_putc(l+'0');
;     832 	else
;     833 		com0_putc(l+'A'-0x0a);
;     834 }
;     835 */
;     836 
;     837 void com0_putstr(u8 *ptr){
;     838         while(*ptr != 0x00){
;	*ptr -> Y+0
;     839                 com0_putc(*ptr++);
;     840         }
;     841 }
;     842 
;     843 void com1_puthex(u8 a)
;     844 {
;     845 	unsigned char h,l;
;     846 	h = (a & 0xf0) >> 4;
;	a -> Y+2
;	h -> R17
;	l -> R16
;     847 	l = (a & 0x0f);
;     848 	if(h <= 9)
;     849 		com1_putc(h+'0');
;     850 	else
;     851 		com1_putc(h+'A'-0x0a);
;     852 	if(l <= 9)
;     853 		com1_putc(l+'0');
;     854 	else
;     855 		com1_putc(l+'A'-0x0a);
;     856 }
;     857 void com1_putstr(u8 *ptr){
;     858         while(*ptr != 0x00){
;	*ptr -> Y+0
;     859                 com1_putc(*ptr++);
;     860         }
;     861 }
;     862 /*
;     863 void com0_mputs(u8 *buf, u8 size){
;     864         while(size-- > 0){
;     865                 com0_puthex(*buf++);
;     866         }
;     867 }
;     868 */
;     869 void com1_mputs(u8 *buf, u8 size){
;     870         while(size-- > 0){
;	*buf -> Y+1
;	size -> Y+0
;     871                 com1_puthex(*buf++);
;     872         }
;     873 }
;     874 
;     875 /*********************************************************************************/
;     876 //                              Intialize UART
;     877 /*********************************************************************************/
;     878 void Init_UART()
;     879 {
_Init_UART:
;     880    // USART0 initialization
;     881    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     882    // USART0 Receiver: On
;     883    // USART0 Transmitter: On
;     884    // USART0 Mode: Asynchronous
;     885    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     886       UCSR0A=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;     887       UCSR0B=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     888       UCSR0C=0x06;
	LDI  R30,LOW(6)
	STS  149,R30
;     889       UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  144,R30
;     890       UBRR0L=0x0F;
	LDI  R30,LOW(15)
	OUT  0x9,R30
;     891 
;     892    // USART1 initialization
;     893    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     894    // USART1 Receiver: On
;     895    // USART1 Transmitter: On
;     896    // USART1 Mode: Asynchronous
;     897    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     898       UCSR1A=0x00;
	LDI  R30,LOW(0)
	STS  155,R30
;     899       UCSR1B=0xD8;
	LDI  R30,LOW(216)
	STS  154,R30
;     900       UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
;     901       UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
;     902       UBRR1L=0x0F;
	LDI  R30,LOW(15)
	STS  153,R30
;     903 
;     904    // Initialize buffer variables
;     905       tx_wr_index0 = 0;
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
;     906       tx_rd_index0 = 0;
	STS  _tx_rd_index0,R30
;     907       tx_counter0  = 0;
	STS  _tx_counter0,R30
;     908 
;     909       tx_wr_index1 = 0;
	STS  _tx_wr_index1,R30
;     910       tx_rd_index1 = 0;
	STS  _tx_rd_index1,R30
;     911       tx_counter1  = 0;
	STS  _tx_counter1,R30
;     912 }
	RET
;     913 /********************************************************************************/
;     914 //                             TL16C554 Driver
;     915 // Controller:  MEGA64
;     916 // 16C554 clk:  7.3728MHZ
;     917 //
;     918 // Schematic Description:
;     919 // 16C554 <---> MEGA64
;     920 // INTA   <---> INT4
;     921 // INTB   <---> INT5
;     922 // INTC   <---> INT6
;     923 // INTD   <---> INT7
;     924 // CSA#   <---> A15
;     925 // CSB#   <---> A14
;     926 // CSC#   <---> A13
;     927 // CSD#   <---> A12
;     928 //   A0   <---> A8
;     929 //   A1   <---> A9
;     930 //   A2   <---> A10
;     931 // RESET  <---> PB0
;     932 // INTN#  <---> PE2
;     933 
;     934 // Coded by: fd_lxh@yahoo.com.cn
;     935 /********************************************************************************/
;     936 #include "mega64.h"
;     937 #include "global.h"
;     938 #include "16c554.h"
;     939 #include "uart.h"
;     940 
;     941 #define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
;     942 #define IER_DATA 0b00000001
;     943 // LCR.7 must be cleared to 0 when accessing RBR/THR/IER
;     944 // LCR.7 must be set to 1 when accessing divisor latch
;     945 #define LCR_DATA 0b00011011    // Even Parity 8 data bit, 1 stop bit
;     946 #define DLL_DATA 0x04          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
;     947 #define DLM_DATA 0x0
;     948 #define MCR_DATA 0b00001000
;     949 
;     950 #define min(A,B)        ((A<B)?(A):(B))
;     951 /********************************************************************************/
;     952 //               TLC16C554 UART PORT A Interrupt Subroutine
;     953 /********************************************************************************/
;     954 interrupt [EXT_INT4] void ext_int4_isr(void)
;     955 {
_ext_int4_isr:
	CALL SUBOPT_0x46
;     956    u8 rundata;
;     957    switch(UC_554A_IIR & 0xF)
;	rundata -> R17
	LDS  R30,29184
	ANDI R30,LOW(0xF)
;     958    {
;     959       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC3
;     960           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     961           break;
	RJMP _0xC2
;     962       case 0x4:                         // Receiver data available or trigger level reached
_0xC3:
	CPI  R30,LOW(0x4)
	BRNE _0xC5
;     963           rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,28672
;     964           cm_pushA(rundata);
	ST   -Y,R17
	RCALL _pc_uart_push
;     965           return;
	CALL SUBOPT_0x47
	RETI
;     966       default:
_0xC5:
;     967           break;
;     968    }
_0xC2:
;     969 
;     970    rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,29952
;     971 }
	CALL SUBOPT_0x47
	RETI
;     972 /********************************************************************************/
;     973 //               TLC16C554 UART PORT B Interrupt Subroutine
;     974 /********************************************************************************/
;     975 // External Interrupt 5 service routine
;     976 interrupt [EXT_INT5] void ext_int5_isr(void)
;     977 {
_ext_int5_isr:
	CALL SUBOPT_0x46
;     978    u8 rundata;
;     979    switch(UC_554B_IIR & 0xF)
;	rundata -> R17
	LDS  R30,45568
	ANDI R30,LOW(0xF)
;     980    {
;     981       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC9
;     982           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     983           break;
	RJMP _0xC8
;     984       case 0x4:                         // Receiver data available or trigger level reached
_0xC9:
	CPI  R30,LOW(0x4)
	BRNE _0xCB
;     985           rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,45056
;     986           cm_pushB(rundata);
	ST   -Y,R17
	RCALL _scanner_uart_push
;     987           return;
	CALL SUBOPT_0x47
	RETI
;     988       default:
_0xCB:
;     989           break;
;     990    }
_0xC8:
;     991    rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,46336
;     992 }
	CALL SUBOPT_0x47
	RETI
;     993 /********************************************************************************/
;     994 //               TLC16C554 UART PORT C Interrupt Subroutine
;     995 /********************************************************************************/
;     996 // External Interrupt 6 service routine
;     997 interrupt [EXT_INT6] void ext_int6_isr(void)
;     998 {
_ext_int6_isr:
	CALL SUBOPT_0x46
;     999    u8 rundata;
;    1000    switch(UC_554C_IIR & 0xF)
;	rundata -> R17
	LDS  R30,53760
	ANDI R30,LOW(0xF)
;    1001    {
;    1002       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xCF
;    1003           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1004           break;
	RJMP _0xCE
;    1005       case 0x4:                         // Receiver data available or trigger level reached
_0xCF:
	CPI  R30,LOW(0x4)
	BRNE _0xD1
;    1006           rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,53248
;    1007           cm_pushC(rundata);
	ST   -Y,R17
	CALL _testA
;    1008           return;
	CALL SUBOPT_0x47
	RETI
;    1009       default:
_0xD1:
;    1010           break;
;    1011    }
_0xCE:
;    1012 
;    1013    rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,54528
;    1014 }
	CALL SUBOPT_0x47
	RETI
;    1015 
;    1016 /********************************************************************************/
;    1017 //               TLC16C554 UART PORT D Interrupt Subroutine
;    1018 /********************************************************************************/
;    1019 // External Interrupt 7 service routine
;    1020 interrupt [EXT_INT7] void ext_int7_isr(void)
;    1021 {
_ext_int7_isr:
	CALL SUBOPT_0x46
;    1022    u8 rundata;
;    1023    switch(UC_554D_IIR & 0xF)
;	rundata -> R17
	LDS  R30,57856
	ANDI R30,LOW(0xF)
;    1024    {
;    1025       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xD5
;    1026           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1027           break;
	RJMP _0xD4
;    1028       case 0x4:                         // Receiver data available or trigger level reached
_0xD5:
	CPI  R30,LOW(0x4)
	BRNE _0xD7
;    1029           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1030           cm_pushD(rundata);
	ST   -Y,R17
	CALL _testA
;    1031           return;
	CALL SUBOPT_0x47
	RETI
;    1032       default:
_0xD7:
;    1033           break;
;    1034    }
_0xD4:
;    1035    rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,58624
;    1036 }
	CALL SUBOPT_0x47
	RETI
;    1037 
;    1038 /********************************************************************************/
;    1039 //                             Reset 16C554
;    1040 /********************************************************************************/
;    1041 void Reset_554(void)
;    1042 {
_Reset_554:
;    1043    // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
;    1044       PORTE.3 = 1;
	SBI  0x3,3
;    1045       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x35
;    1046       PORTE.3 = 0;
	CBI  0x3,3
;    1047       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x35
;    1048 }
	RET
;    1049 /********************************************************************************/
;    1050 //                             Initialize 16C554
;    1051 /********************************************************************************/
;    1052 void Init_554(void)
;    1053 {
_Init_554:
;    1054       Reset_554();  // Reset 554
	CALL _Reset_554
;    1055 
;    1056    /********************************************************************/
;    1057    //                      Initialize UART A
;    1058    /********************************************************************/
;    1059    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1060       UC_554A_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  29440,R30
;    1061       UC_554A_DLL = DLL_DATA;
	LDI  R30,LOW(4)
	STS  28672,R30
;    1062       UC_554A_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  28928,R30
;    1063 
;    1064    // Set Line Control Register:
;    1065    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1066       UC_554A_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  29440,R30
;    1067 
;    1068    // Set FIFO Control Register:
;    1069    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1070       UC_554A_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  29184,R30
;    1071 
;    1072    // Set Modem Control Register:
;    1073    // Enable enternal interrupt
;    1074       UC_554A_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  29696,R30
;    1075 
;    1076    // Set Interrupt Enable Register:
;    1077    // Enable Received data available / timeout / transmitter holding register empty &
;    1078    // Receiver line status interrupt
;    1079       UC_554A_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  28928,R30
;    1080 
;    1081    /********************************************************************/
;    1082    //                      Initialize UART B
;    1083    /********************************************************************/
;    1084    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1085    // LCR.7 must be set to "1" before setting baud rate
;    1086       UC_554B_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  45824,R30
;    1087       UC_554B_DLL = DLL_DATA;
	LDI  R30,LOW(4)
	STS  45056,R30
;    1088       UC_554B_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  45312,R30
;    1089 
;    1090    // Set Line Control Register:
;    1091    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1092       UC_554B_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  45824,R30
;    1093 
;    1094    // Set FIFO Control Register:
;    1095    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1096       UC_554B_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  45568,R30
;    1097 
;    1098    // Set Modem Control Register:
;    1099    // Enable enternal interrupt
;    1100       UC_554B_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  46080,R30
;    1101 
;    1102    // Set Interrupt Enable Register:
;    1103    // Enable Received data available / timeout / transmitter holding register empty &
;    1104    // Receiver line status interrupt
;    1105       UC_554B_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  45312,R30
;    1106 
;    1107    /********************************************************************/
;    1108    //                      Initialize UART C
;    1109    /********************************************************************/
;    1110    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1111       UC_554C_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  54016,R30
;    1112       UC_554C_DLL = DLL_DATA;
	LDI  R30,LOW(4)
	STS  53248,R30
;    1113       UC_554C_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  53504,R30
;    1114 
;    1115    // Set Line Control Register:
;    1116    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1117       UC_554C_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  54016,R30
;    1118 
;    1119    // Set FIFO Control Register:
;    1120    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1121       UC_554C_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  53760,R30
;    1122 
;    1123    // Set Modem Control Register:
;    1124    // Enable enternal interrupt
;    1125       UC_554C_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  54272,R30
;    1126 
;    1127    // Set Interrupt Enable Register:
;    1128    // Enable Received data available / timeout / transmitter holding register empty &
;    1129    // Receiver line status interrupt
;    1130       UC_554C_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  53504,R30
;    1131 
;    1132    /********************************************************************/
;    1133    //                      Initialize UART D
;    1134    /********************************************************************/
;    1135    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1136       UC_554D_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  58112,R30
;    1137       UC_554D_DLL = DLL_DATA;
	LDI  R30,LOW(4)
	STS  57344,R30
;    1138       UC_554D_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  57600,R30
;    1139 
;    1140    // Set Line Control Register:
;    1141    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1142       UC_554D_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  58112,R30
;    1143 
;    1144    // Set FIFO Control Register:
;    1145    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1146       UC_554D_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  57856,R30
;    1147 
;    1148    // Set Modem Control Register:
;    1149    // Enable enternal interrupt
;    1150       UC_554D_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  58368,R30
;    1151 
;    1152    // Set Interrupt Enable Register:
;    1153    // Enable Received data available / timeout / transmitter holding register empty &
;    1154    // Receiver line status interrupt
;    1155       UC_554D_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  57600,R30
;    1156 }
	RET
;    1157 
;    1158 
;    1159 /********************************************************************************/
;    1160 //                 Send multi-bytes (less than 16 bytes) via UART
;    1161 // Function:
;    1162 // Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
;    1163 // format:
;    1164 // print_554(u8 *str, char uart_port)
;    1165 // arg1: string to be sent. arg2: port (A B,C,D)
;    1166 /********************************************************************************/
;    1167 void prints(u8 *str, u8 length, char uart_port)
;    1168 {
_prints:
;    1169     u8 len, i;
;    1170     //len = length & 0x0F;        //15bytes at most
;    1171     len = length;
	ST   -Y,R17
	ST   -Y,R16
;	*str -> Y+4
;	length -> Y+3
;	uart_port -> Y+2
;	len -> R17
;	i -> R16
	LDD  R17,Y+3
;    1172     switch(uart_port)
	LDD  R30,Y+2
;    1173     {
;    1174        case SPORTA:
	CPI  R30,0
	BRNE _0xDB
;    1175            // wait if data still being transmitted in UART
;    1176            // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
;    1177            // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
;    1178            // check if FIFO is empty by LSR5.
;    1179            // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
;    1180            // UC_554A_LSR is not a register name,
;    1181            // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
;    1182            // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
;    1183            // here in 16c554.h, a bracket is added
;    1184            while(len)
_0xDC:
	CPI  R17,0
	BREQ _0xDE
;    1185            {
;    1186               while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xDF:
	LDS  R30,29952
	ANDI R30,LOW(0x20)
	BREQ _0xDF
;    1187               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xE3:
	CPI  R17,16
	BRSH _0xE5
	MOV  R30,R17
	RJMP _0xE6
_0xE5:
	LDI  R30,LOW(16)
_0xE6:
	CP   R16,R30
	BRSH _0xE4
;    1188                   UC_554A_THR = *str++;
	CALL SUBOPT_0x48
	STS  28672,R30
;    1189               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xE3
_0xE4:
	CPI  R17,16
	BRSH _0xE8
	MOV  R30,R17
	RJMP _0xE9
_0xE8:
	LDI  R30,LOW(16)
_0xE9:
	SUB  R17,R30
;    1190            }
	RJMP _0xDC
_0xDE:
;    1191            break;
	RJMP _0xDA
;    1192        case SPORTB:
_0xDB:
	CPI  R30,LOW(0x1)
	BRNE _0xEB
;    1193            while(len)
_0xEC:
	CPI  R17,0
	BREQ _0xEE
;    1194            {
;    1195               while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xEF:
	LDS  R30,46336
	ANDI R30,LOW(0x20)
	BREQ _0xEF
;    1196               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xF3:
	CPI  R17,16
	BRSH _0xF5
	MOV  R30,R17
	RJMP _0xF6
_0xF5:
	LDI  R30,LOW(16)
_0xF6:
	CP   R16,R30
	BRSH _0xF4
;    1197                   UC_554B_THR = *str++;
	CALL SUBOPT_0x48
	STS  45056,R30
;    1198               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xF3
_0xF4:
	CPI  R17,16
	BRSH _0xF8
	MOV  R30,R17
	RJMP _0xF9
_0xF8:
	LDI  R30,LOW(16)
_0xF9:
	SUB  R17,R30
;    1199            }
	RJMP _0xEC
_0xEE:
;    1200            break;
	RJMP _0xDA
;    1201        case SPORTC:
_0xEB:
	CPI  R30,LOW(0x2)
	BRNE _0xFB
;    1202            while(len)
_0xFC:
	CPI  R17,0
	BREQ _0xFE
;    1203            {
;    1204               while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xFF:
	LDS  R30,54528
	ANDI R30,LOW(0x20)
	BREQ _0xFF
;    1205               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0x103:
	CPI  R17,16
	BRSH _0x105
	MOV  R30,R17
	RJMP _0x106
_0x105:
	LDI  R30,LOW(16)
_0x106:
	CP   R16,R30
	BRSH _0x104
;    1206                   UC_554C_THR = *str++;
	CALL SUBOPT_0x48
	STS  53248,R30
;    1207               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0x103
_0x104:
	CPI  R17,16
	BRSH _0x108
	MOV  R30,R17
	RJMP _0x109
_0x108:
	LDI  R30,LOW(16)
_0x109:
	SUB  R17,R30
;    1208            }
	RJMP _0xFC
_0xFE:
;    1209 
;    1210            break;
	RJMP _0xDA
;    1211        case SPORTD:
_0xFB:
	CPI  R30,LOW(0x3)
	BRNE _0x10B
;    1212            while(len)
_0x10C:
	CPI  R17,0
	BREQ _0x10E
;    1213            {
;    1214               while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
_0x10F:
	LDS  R30,58624
	ANDI R30,LOW(0x20)
	BREQ _0x10F
;    1215               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0x113:
	CPI  R17,16
	BRSH _0x115
	MOV  R30,R17
	RJMP _0x116
_0x115:
	LDI  R30,LOW(16)
_0x116:
	CP   R16,R30
	BRSH _0x114
;    1216                   UC_554D_THR = *str++;
	CALL SUBOPT_0x48
	STS  57344,R30
;    1217               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0x113
_0x114:
	CPI  R17,16
	BRSH _0x118
	MOV  R30,R17
	RJMP _0x119
_0x118:
	LDI  R30,LOW(16)
_0x119:
	SUB  R17,R30
;    1218            }
	RJMP _0x10C
_0x10E:
;    1219            break;
	RJMP _0xDA
;    1220        case SPORT0:
_0x10B:
	CPI  R30,LOW(0x4)
	BRNE _0x11B
;    1221            while(len-- > 0){
_0x11C:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x11E
;    1222                 com0_putc(*str++);
	CALL SUBOPT_0x48
	ST   -Y,R30
	CALL _com0_putc
;    1223            }
	RJMP _0x11C
_0x11E:
;    1224            break;
	RJMP _0xDA
;    1225        case SPORT1:
_0x11B:
	CPI  R30,LOW(0x5)
	BRNE _0x123
;    1226            while(len-- > 0){
_0x120:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x122
;    1227                 com1_putc(*str++);
	CALL SUBOPT_0x48
	ST   -Y,R30
	CALL _com1_putc
;    1228            }
	RJMP _0x120
_0x122:
;    1229            break;
	RJMP _0xDA
;    1230        default:
_0x123:
;    1231            PORTC = 0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1232            break;
;    1233     }
_0xDA:
;    1234 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    1235 
;    1236 
;    1237 // System Initialization file
;    1238 
;    1239 #include <mega64.h>
;    1240 #include "global.h"
;    1241 /**************************************************************************************/
;    1242 //                              Initialize Port A-G
;    1243 /**************************************************************************************/
;    1244 void Init_Port()
;    1245 {
_Init_Port:
;    1246  // Input/Output Ports initialization
;    1247  // Port A initialization
;    1248  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;    1249  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1250     PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;    1251     DDRA=0x00;
	OUT  0x1A,R30
;    1252 
;    1253  // Port B initialization
;    1254  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
;    1255  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
;    1256     PORTB=0x00;
	OUT  0x18,R30
;    1257     DDRB=0x00;
	OUT  0x17,R30
;    1258 
;    1259  // Port C initialization
;    1260  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
;    1261  // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
;    1262  // PORTC[7:4]: chip select of 16C554 channels.
;    1263     PORTC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1264     DDRC=0xFF;
	LDI  R30,LOW(255)
	OUT  0x14,R30
;    1265 
;    1266  // Port D initialization
;    1267  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
;    1268  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1269     PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1270     DDRD=0x00;
	OUT  0x11,R30
;    1271 
;    1272  // Port E initialization
;    1273  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
;    1274  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
;    1275  // PORTE.0: Input (M_RXD0)
;    1276  // PORTE.1: output (M_TXD0)
;    1277  // PORTE.2: output (16C554 INTC#)
;    1278  // PORTE.3: output (16C554 reset)
;    1279     PORTE=0x06;
	LDI  R30,LOW(6)
	OUT  0x3,R30
;    1280     DDRE=0x0E;
	LDI  R30,LOW(14)
	OUT  0x2,R30
;    1281 
;    1282  // Port F initialization
;    1283  // Input
;    1284  // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
;    1285  // PORTF[3:0]: LED[4:1]
;    1286     PORTF=0x0F;
	LDI  R30,LOW(15)
	STS  98,R30
;    1287     DDRF=0x0F;
	STS  97,R30
;    1288 
;    1289  // Port G initialization
;    1290  // Input
;    1291     PORTG=0x00;
	LDI  R30,LOW(0)
	STS  101,R30
;    1292     DDRG=0x00;
	STS  100,R30
;    1293 }
	RET
;    1294 
;    1295 /**************************************************************************************/
;    1296 //                              Initialize Timers
;    1297 /**************************************************************************************/
;    1298 void Init_Timers()
;    1299 {
;    1300  // Timer/Counter 0 initialization
;    1301  // Clock source: System Clock
;    1302  // Clock value: 14.400 kHz
;    1303  // Mode: Normal top=FFh
;    1304  // OC0 output: Disconnected
;    1305     ASSR=0x00;
;    1306     TCCR0=0x07;
;    1307     TCNT0=0x00;
;    1308     OCR0=0x00;
;    1309 
;    1310  // Timer/Counter 1 initialization
;    1311  // Clock source: System Clock
;    1312  // Clock value: 14.400 kHz
;    1313  // Mode: Normal top=FFFFh
;    1314  // OC1A output: Discon.
;    1315  // OC1B output: Discon.
;    1316  // OC1C output: Discon.
;    1317  // Noise Canceler: Off
;    1318  // Input Capture on Falling Edge
;    1319  // Timer 1 Overflow Interrupt: On
;    1320  // Input Capture Interrupt: Off
;    1321  // Compare A Match Interrupt: Off
;    1322  // Compare B Match Interrupt: Off
;    1323  // Compare C Match Interrupt: Off
;    1324     TCCR1A=0x00;
;    1325     TCCR1B=0x05;
;    1326     TCNT1H=0x00;
;    1327     TCNT1L=0x00;
;    1328     ICR1H=0x00;
;    1329     ICR1L=0x00;
;    1330     OCR1AH=0x00;
;    1331     OCR1AL=0x00;
;    1332     OCR1BH=0x00;
;    1333     OCR1BL=0x00;
;    1334     OCR1CH=0x00;
;    1335     OCR1CL=0x00;
;    1336 
;    1337  // Timer/Counter 2 initialization
;    1338  // Clock source: System Clock
;    1339  // Clock value: Timer 2 Stopped
;    1340  // Mode: Normal top=FFh
;    1341  // OC2 output: Disconnected
;    1342     TCCR2=0x00;
;    1343     TCNT2=0x00;
;    1344     OCR2=0x00;
;    1345 
;    1346  // Timer/Counter 3 initialization
;    1347  // Clock source: System Clock
;    1348  // Clock value: Timer 3 Stopped
;    1349  // Mode: Normal top=FFFFh
;    1350  // Noise Canceler: Off
;    1351  // Input Capture on Falling Edge
;    1352  // OC3A output: Discon.
;    1353  // OC3B output: Discon.
;    1354  // OC3C output: Discon.
;    1355  // Timer 3 Overflow Interrupt: Off
;    1356  // Input Capture Interrupt: Off
;    1357  // Compare A Match Interrupt: Off
;    1358  // Compare B Match Interrupt: Off
;    1359  // Compare C Match Interrupt: Off
;    1360     TCCR3A=0x00;
;    1361     TCCR3B=0x00;
;    1362     TCNT3H=0x00;
;    1363     TCNT3L=0x00;
;    1364     ICR3H=0x00;
;    1365     ICR3L=0x00;
;    1366     OCR3AH=0x00;
;    1367     OCR3AL=0x00;
;    1368     OCR3BH=0x00;
;    1369     OCR3BL=0x00;
;    1370     OCR3CH=0x00;
;    1371     OCR3CL=0x00;
;    1372 
;    1373  // Timer(s)/Counter(s) Interrupt(s) initialization
;    1374     TIMSK=0x05;
;    1375     ETIMSK=0x00;
;    1376 }
;    1377 
;    1378 /**************************************************************************************/
;    1379 //                              Enable External Memory
;    1380 /**************************************************************************************/
;    1381 void Enable_XMEM()
;    1382 {
_Enable_XMEM:
;    1383  // External SRAM page configuration:
;    1384  // 1100h - 7FFFh / 8000h - FFFFh
;    1385  // Lower page wait state(s): 2r/w +1 addr
;    1386  // Upper page wait state(s): 2r/w + 1 addr
;    1387     MCUCR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x35,R30
;    1388     //MCUCR = 0x80;
;    1389 
;    1390     XMCRA=0x4E;
	LDI  R30,LOW(78)
	STS  109,R30
;    1391     //XMCRA=0x0A;
;    1392  // PC7:0 can not be released
;    1393     XMCRB=0x80;
	LDI  R30,LOW(128)
	STS  108,R30
;    1394     // XMCRB = 0x07;
;    1395 }
	RET
;    1396 
;    1397 /**************************************************************************************/
;    1398 //                              Initialize External Interrupt
;    1399 /**************************************************************************************/
;    1400 void Init_Ex_Interrupt()
;    1401 {
;    1402  // External Interrupt(s) initialization
;    1403  // INT0: Off
;    1404  // INT1: Off
;    1405  // INT2: Off
;    1406  // INT3: Off
;    1407  // INT4: On
;    1408  // INT4 Mode: Rising Edge
;    1409  // INT5: On
;    1410  // INT5 Mode: Rising Edge
;    1411  // INT6: On
;    1412  // INT6 Mode: Rising Edge
;    1413  // INT7: On
;    1414  // INT7 Mode: Rising Edge
;    1415     EICRA=0x00;
;    1416     EICRB=0xFF;
;    1417     EIMSK=0xF0;
;    1418     EIFR=0xF0;
;    1419 }
;    1420 
;    1421 /**************************************************************************************/
;    1422 //                              Analog Comparator initialization
;    1423 /**************************************************************************************/
;    1424 void Init_ADC()
;    1425 {
;    1426  // Analog Comparator initialization
;    1427  // Analog Comparator: Off
;    1428  // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1429     ACSR=0x80;
;    1430     SFIOR=0x00;
;    1431 }
;    1432 #include "global.h"
;    1433 #include "wendu.h"
;    1434 #include "utili.h"
;    1435 #include "math.h"
;    1436 #define logl log
;    1437 #define powl pow
;    1438 double PT100RToTValue(double r,double r0)
;    1439 {
_PT100RToTValue:
;    1440         double ac,bc,cc,tlow,tup,rnew,tnew;
;    1441         int count;
;    1442         ac = 3.908e-3;
	SBIW R28,28
	ST   -Y,R17
	ST   -Y,R16
;	r -> Y+34
;	r0 -> Y+30
;	ac -> Y+26
;	bc -> Y+22
;	cc -> Y+18
;	tlow -> Y+14
;	tup -> Y+10
;	rnew -> Y+6
;	tnew -> Y+2
;	count -> R16,R17
	__GETD1N 0x3B800EAE
	__PUTD1S 26
;    1443         bc = -5.775E-7;
	__GETD1N 0xB51B057F
	__PUTD1S 22
;    1444         cc = -4.183E-12;
	__GETD1N 0xAC932D1D
	__PUTD1S 18
;    1445         tlow = -200;
	__GETD1N 0xC3480000
	CALL SUBOPT_0x49
;    1446         tup = 850;
	__GETD1N 0x44548000
	CALL SUBOPT_0x4A
;    1447 	count = 0;
	__GETWRN 16,17,0
;    1448 
;    1449 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x124:
	CALL SUBOPT_0x4B
	CALL SUBOPT_0x4C
	CALL SUBOPT_0x4D
	BREQ PC+2
	BRCC PC+3
	JMP  _0x127
	CALL SUBOPT_0x4E
	BRLT _0x128
_0x127:
	RJMP _0x126
_0x128:
;    1450 	{
;    1451 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4F
;    1452 		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
	__GETD1S 26
	CALL SUBOPT_0x50
	CALL SUBOPT_0x51
	CALL SUBOPT_0x50
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	__GETD1S 22
	CALL SUBOPT_0x50
	CALL SUBOPT_0x51
	CALL SUBOPT_0x52
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x53
;    1453 		if(tnew < 0)
	CALL SUBOPT_0x52
	CALL __CPD20
	BRGE _0x129
;    1454 		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
	__GETD1S 18
	CALL SUBOPT_0x50
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x52
	__GETD1N 0x42C80000
	CALL SUBOPT_0x2C
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x51
	CALL SUBOPT_0x52
	CALL SUBOPT_0x51
	CALL SUBOPT_0x54
	CALL SUBOPT_0x53
;    1455 
;    1456 		if(r < rnew)
_0x129:
	CALL SUBOPT_0x55
	__GETD2S 34
	CALL __CMPF12
	BRSH _0x12A
;    1457 			tup = tnew;
	CALL SUBOPT_0x56
	CALL SUBOPT_0x4A
;    1458 		else
	RJMP _0x12B
_0x12A:
;    1459 			tlow = tnew;
	CALL SUBOPT_0x57
;    1460 	}
_0x12B:
	RJMP _0x124
_0x126:
;    1461 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x58
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x59
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x5A
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,38
	RET
;    1462 
;    1463 }
;    1464 double RValueToTValue(double r, u8 prbid)
;    1465 {
_RValueToTValue:
;    1466 	double ac,bc,cc,tlow,tup,rnew,tnew;
;    1467 	int count;
;    1468 
;    1469 	ac = rprbdata.param1[prbid];
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
	LDD  R30,Y+30
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL SUBOPT_0x5B
	__PUTD1S 26
;    1470 	bc = rprbdata.param2[prbid];
	__POINTW2MN _rprbdata,96
	LDD  R30,Y+30
	CALL SUBOPT_0x5B
	__PUTD1S 22
;    1471 	cc = rprbdata.param3[prbid];
	__POINTW2MN _rprbdata,192
	LDD  R30,Y+30
	CALL SUBOPT_0x5B
	__PUTD1S 18
;    1472 
;    1473 	if(rprbdata.type[prbid] == PRBTYPE_PT100)
	CALL SUBOPT_0x5C
	CPI  R30,LOW(0xF1)
	BRNE _0x12C
;    1474 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x5D
	__GETD1S 22
	CALL __PUTPARD1
	CALL _PT100RToTValue
	RJMP _0x496
;    1475 	if(rprbdata.type[prbid] != PRBTYPE_PT25)
_0x12C:
	CALL SUBOPT_0x5C
	CPI  R30,LOW(0xF2)
	BREQ _0x12D
;    1476 		return -9999.999;
	__GETD1N 0xC61C3FFF
	RJMP _0x496
;    1477         if(rprbdata.rtp[prbid] > 0.1)
_0x12D:
	__POINTW2MN _rprbdata,288
	LDD  R30,Y+30
	CALL SUBOPT_0x5B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3DCCCCCD
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x12E
;    1478                 r = r/rprbdata.rtp[prbid];
	__POINTW2MN _rprbdata,288
	LDD  R30,Y+30
	CALL SUBOPT_0x5B
	CALL SUBOPT_0x5E
	RJMP _0x4A1
;    1479         else
_0x12E:
;    1480 		r = r/25.0;
	CALL SUBOPT_0x5E
	__GETD1N 0x41C80000
_0x4A1:
	CALL __DIVF21
	__PUTD1S 31
;    1481 
;    1482 
;    1483 
;    1484 
;    1485 
;    1486 	//set the search range of T between GetT(r) +/- 1 degree
;    1487 	tlow = GetT(r) - 1;
	CALL SUBOPT_0x5D
	RCALL _GetT
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x49
;    1488 	tup = tlow + 2;
	__GETD1S 14
	__GETD2N 0x40000000
	CALL __ADDF12
	CALL SUBOPT_0x4A
;    1489 
;    1490 	count = 0;
	__GETWRN 16,17,0
;    1491 
;    1492 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x130:
	CALL SUBOPT_0x60
	BREQ PC+2
	BRCC PC+3
	JMP  _0x133
	CALL SUBOPT_0x4E
	BRLT _0x134
_0x133:
	RJMP _0x132
_0x134:
;    1493 	{
;    1494 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4F
;    1495 		rnew = GetWr(tnew);
	CALL SUBOPT_0x56
	CALL __PUTPARD1
	RCALL _GetWr
	CALL SUBOPT_0x61
;    1496 		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
	CALL SUBOPT_0x52
	__GETD1N 0xC33D581D
	CALL __CMPF12
	BRLO _0x136
	CALL SUBOPT_0x52
	__GETD1N 0x3C23D70A
	CALL __CMPF12
	BREQ PC+4
	BRCS PC+3
	JMP  _0x136
	RJMP _0x137
_0x136:
	RJMP _0x135
_0x137:
;    1497         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
	CALL SUBOPT_0x62
	CALL SUBOPT_0x63
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
	CALL SUBOPT_0x64
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x65
	CALL _log
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	RJMP _0x4A2
;    1498                 else
_0x135:
;    1499         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
	CALL SUBOPT_0x62
	CALL SUBOPT_0x63
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
	CALL SUBOPT_0x64
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
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
	CALL SUBOPT_0x62
	__GETD2S 18
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
_0x4A2:
	CALL __ADDF12
	CALL SUBOPT_0x61
;    1500 		if(r < rnew)
	CALL SUBOPT_0x55
	CALL SUBOPT_0x5E
	CALL __CMPF12
	BRSH _0x139
;    1501 			tup = tnew;
	CALL SUBOPT_0x66
;    1502 		else
	RJMP _0x13A
_0x139:
;    1503 			tlow = tnew;
	CALL SUBOPT_0x57
;    1504 	}
_0x13A:
	RJMP _0x130
_0x132:
;    1505 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x58
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x59
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x5A
_0x496:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,35
	RET
;    1506 
;    1507 
;    1508 //	return r+1;
;    1509 }
;    1510 
;    1511 flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
;    1512 flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
;    1513 flash u8 sizeAi = sizeof(Ai)/sizeof(double);
;    1514 flash u8 sizeCi = sizeof(Ci)/sizeof(double);
;    1515 
;    1516 double GetWr(double t)
;    1517 {
_GetWr:
;    1518 	double result;
;    1519 	double pert;
;    1520 	int i;
;    1521 	if(t < 0)
	CALL SUBOPT_0x67
;	t -> Y+10
;	result -> Y+6
;	pert -> Y+2
;	i -> R16,R17
	BRGE _0x13B
;    1522 	{       //equals to Wr189(t) in mfile
;    1523 		result = Ai[0];
	LDI  R30,LOW(_Ai*2)
	LDI  R31,HIGH(_Ai*2)
	CALL __GETD1PF
	CALL SUBOPT_0x61
;    1524 		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
	CALL SUBOPT_0x4C
	__GETD2N 0x43889333
	CALL SUBOPT_0x68
	__GETD1N 0x4388947B
	CALL __DIVF21
	CALL SUBOPT_0x69
	__GETD2N 0x3FC00000
	CALL SUBOPT_0x68
	__GETD1N 0x3FC00000
	CALL __DIVF21
	CALL SUBOPT_0x6A
;    1525 		t = pert;
;    1526 		for(i=1;i<sizeAi;i++)
	__GETWRN 16,17,1
_0x13D:
	LDI  R30,LOW(_sizeAi*2)
	LDI  R31,HIGH(_sizeAi*2)
	LPM  R30,Z
	CALL SUBOPT_0x6B
	BRGE _0x13E
;    1527 		{
;    1528 			result = result + Ai[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ai*2)
	LDI  R27,HIGH(_Ai*2)
	CALL SUBOPT_0x6C
;    1529 			t = t*pert;
	CALL SUBOPT_0x6D
;    1530 		}
	__ADDWRN 16,17,1
	RJMP _0x13D
_0x13E:
;    1531 		result = exp(result);
	CALL SUBOPT_0x65
	CALL _exp
	CALL SUBOPT_0x61
;    1532 	}else{
	RJMP _0x13F
_0x13B:
;    1533 			 result = Ci[0];
	LDI  R30,LOW(_Ci*2)
	LDI  R31,HIGH(_Ci*2)
	CALL __GETD1PF
	CALL SUBOPT_0x61
;    1534 			 pert = t/481.0 - 1;
	CALL SUBOPT_0x6E
	__GETD1N 0x43F08000
	CALL __DIVF21
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x6A
;    1535 			 t = pert;
;    1536 			 for(i=1;i<sizeCi;i++)
	__GETWRN 16,17,1
_0x141:
	LDI  R30,LOW(_sizeCi*2)
	LDI  R31,HIGH(_sizeCi*2)
	LPM  R30,Z
	CALL SUBOPT_0x6B
	BRGE _0x142
;    1537 			 {
;    1538 				 result = result + Ci[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ci*2)
	LDI  R27,HIGH(_Ci*2)
	CALL SUBOPT_0x6C
;    1539 				 t = t*pert;
	CALL SUBOPT_0x6D
;    1540 			 }
	__ADDWRN 16,17,1
	RJMP _0x141
_0x142:
;    1541 	}
_0x13F:
;    1542 	return result;
	CALL SUBOPT_0x55
	RJMP _0x495
;    1543 }
;    1544 flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
;    1545 flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
;    1546 flash u8 sizeDi = sizeof(Di)/sizeof(double);
;    1547 flash u8 sizeBi = sizeof(Bi)/sizeof(double);
;    1548 
;    1549 double GetT(double w)
;    1550 {
_GetT:
;    1551 	double perw;
;    1552 	double result;
;    1553 	int i;
;    1554 
;    1555 	if(w >= 0)
	CALL SUBOPT_0x67
;	w -> Y+10
;	perw -> Y+6
;	result -> Y+2
;	i -> R16,R17
	BRLT _0x143
;    1556 	{       //t660r
;    1557 		perw = (w-2.64)/1.64;
	CALL SUBOPT_0x6E
	__GETD1N 0x4028F5C3
	CALL SUBOPT_0x2C
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3FD1EB85
	CALL SUBOPT_0x6F
;    1558 		w = perw;
;    1559 		result = Di[0];
	LDI  R30,LOW(_Di*2)
	LDI  R31,HIGH(_Di*2)
	CALL SUBOPT_0x70
;    1560 		for(i=1;i<sizeDi;i++)
_0x145:
	LDI  R30,LOW(_sizeDi*2)
	LDI  R31,HIGH(_sizeDi*2)
	LPM  R30,Z
	CALL SUBOPT_0x6B
	BRGE _0x146
;    1561 		{
;    1562 			result = result + Di[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Di*2)
	LDI  R27,HIGH(_Di*2)
	CALL SUBOPT_0x71
;    1563 			w = w*perw;
;    1564 		}
	__ADDWRN 16,17,1
	RJMP _0x145
_0x146:
;    1565 	}else{   //t189(r)
	RJMP _0x147
_0x143:
;    1566 		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
	CALL SUBOPT_0x4C
	CALL __PUTPARD1
	__GETD1N 0x3E2AAAAB
	CALL __PUTPARD1
	CALL _pow
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F266666
	CALL SUBOPT_0x2C
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3EB33333
	CALL SUBOPT_0x6F
;    1567 		w = perw;
;    1568 		result = Bi[0];
	LDI  R30,LOW(_Bi*2)
	LDI  R31,HIGH(_Bi*2)
	CALL SUBOPT_0x70
;    1569 		for(i=1;i<sizeBi;i++)
_0x149:
	LDI  R30,LOW(_sizeBi*2)
	LDI  R31,HIGH(_sizeBi*2)
	LPM  R30,Z
	CALL SUBOPT_0x6B
	BRGE _0x14A
;    1570 		{
;    1571 			result = result + Bi[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Bi*2)
	LDI  R27,HIGH(_Bi*2)
	CALL SUBOPT_0x71
;    1572 			w = w*perw;
;    1573 		}
	__ADDWRN 16,17,1
	RJMP _0x149
_0x14A:
;    1574 		result = 273.15*result - 273.15;
	CALL SUBOPT_0x56
	__GETD2N 0x43889333
	CALL SUBOPT_0x2A
	__GETD1N 0x43889333
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x72
;    1575 	}
_0x147:
;    1576 	return result;
	CALL SUBOPT_0x56
_0x495:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
;    1577 }
;    1578 
;    1579 flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
;    1580 flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};
;    1581 
;    1582 flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
;    1583 flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
;    1584 flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};
;    1585 
;    1586 flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};
;    1587 
;    1588 flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
;    1589 flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};
;    1590 
;    1591 flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
;    1592 flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};
;    1593 
;    1594 flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
;    1595 flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};
;    1596 
;    1597 flash double BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1598 flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};
;    1599 
;    1600 flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1601 flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};
;    1602 
;    1603 flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
;    1604 flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};
;    1605 
;    1606 flash int TLowLen = sizeof(TLow)/sizeof(double);
;    1607 flash int THighLen = sizeof(THigh)/sizeof(double);
;    1608 
;    1609 flash int SLowLen = sizeof(SLow)/sizeof(double);
;    1610 flash int SMedLen = sizeof(SMed)/sizeof(double);
;    1611 flash int SHighLen = sizeof(SHigh)/sizeof(double);
;    1612 
;    1613 flash int RLowLen = sizeof(RLow)/sizeof(double);
;    1614 flash int RMedLen = sizeof(RMed)/sizeof(double);
;    1615 flash int RHighLen = sizeof(RHigh)/sizeof(double);
;    1616 
;    1617 flash int NLowLen = sizeof(NLow)/sizeof(double);
;    1618 flash int NHighLen = sizeof(NHigh)/sizeof(double);
;    1619 
;    1620 flash int JLowLen = sizeof(JLow)/sizeof(double);
;    1621 flash int JHighLen = sizeof(JHigh)/sizeof(double);
;    1622 
;    1623 flash int BLowLen = sizeof(BLow)/sizeof(double);
;    1624 flash int BHighLen = sizeof(BHigh)/sizeof(double);
;    1625 
;    1626 flash int ELowLen = sizeof(ELow)/sizeof(double);
;    1627 flash int EHighLen = sizeof(EHigh)/sizeof(double);
;    1628 
;    1629 flash int KLowLen = sizeof(KLow)/sizeof(double);
;    1630 flash int KHighLen = sizeof(KHigh)/sizeof(double);
;    1631 
;    1632 flash double *coef;

	.DSEG
_coef:
	.BYTE 0x2
;    1633 
;    1634 double GetThmoVolt(double t,char type)
;    1635 {

	.CSEG
_GetThmoVolt:
;    1636 	double result,pert;
;    1637 	int i;
;    1638 	int len = 0;
;    1639 	switch(type)
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
;    1640 	{
;    1641 		case 'T':
	CPI  R30,LOW(0x54)
	BRNE _0x14E
;    1642 			coef = (t < 0) ? TLow : THigh;
	CALL SUBOPT_0x73
	BRGE _0x14F
	LDI  R30,LOW(_TLow*2)
	LDI  R31,HIGH(_TLow*2)
	RJMP _0x150
_0x14F:
	LDI  R30,LOW(_THigh*2)
	LDI  R31,HIGH(_THigh*2)
_0x150:
	CALL SUBOPT_0x74
;    1643 			len = (t < 0) ? TLowLen : THighLen;
	BRGE _0x152
	LDI  R30,LOW(_TLowLen*2)
	LDI  R31,HIGH(_TLowLen*2)
	CALL __GETW1PF
	RJMP _0x153
_0x152:
	LDI  R30,LOW(_THighLen*2)
	LDI  R31,HIGH(_THighLen*2)
	CALL __GETW1PF
_0x153:
	MOVW R18,R30
;    1644 			break;
	RJMP _0x14D
;    1645 		case 'K':
_0x14E:
	CPI  R30,LOW(0x4B)
	BRNE _0x155
;    1646 			coef = (t < 0) ? KLow : KHigh;
	CALL SUBOPT_0x73
	BRGE _0x156
	LDI  R30,LOW(_KLow*2)
	LDI  R31,HIGH(_KLow*2)
	RJMP _0x157
_0x156:
	LDI  R30,LOW(_KHigh*2)
	LDI  R31,HIGH(_KHigh*2)
_0x157:
	CALL SUBOPT_0x74
;    1647 			len = (t < 0) ? KLowLen : KHighLen;
	BRGE _0x159
	LDI  R30,LOW(_KLowLen*2)
	LDI  R31,HIGH(_KLowLen*2)
	CALL __GETW1PF
	RJMP _0x15A
_0x159:
	LDI  R30,LOW(_KHighLen*2)
	LDI  R31,HIGH(_KHighLen*2)
	CALL __GETW1PF
_0x15A:
	MOVW R18,R30
;    1648 			break;
	RJMP _0x14D
;    1649 		case 'N':
_0x155:
	CPI  R30,LOW(0x4E)
	BRNE _0x15C
;    1650 			coef = (t < 0) ? NLow : NHigh;
	CALL SUBOPT_0x73
	BRGE _0x15D
	LDI  R30,LOW(_NLow*2)
	LDI  R31,HIGH(_NLow*2)
	RJMP _0x15E
_0x15D:
	LDI  R30,LOW(_NHigh*2)
	LDI  R31,HIGH(_NHigh*2)
_0x15E:
	CALL SUBOPT_0x74
;    1651 			len = (t < 0) ? NLowLen : NHighLen;
	BRGE _0x160
	LDI  R30,LOW(_NLowLen*2)
	LDI  R31,HIGH(_NLowLen*2)
	CALL __GETW1PF
	RJMP _0x161
_0x160:
	LDI  R30,LOW(_NHighLen*2)
	LDI  R31,HIGH(_NHighLen*2)
	CALL __GETW1PF
_0x161:
	MOVW R18,R30
;    1652 			break;
	RJMP _0x14D
;    1653 		case 'E':
_0x15C:
	CPI  R30,LOW(0x45)
	BRNE _0x163
;    1654 			coef = (t < 0) ? ELow : EHigh;
	CALL SUBOPT_0x73
	BRGE _0x164
	LDI  R30,LOW(_ELow*2)
	LDI  R31,HIGH(_ELow*2)
	RJMP _0x165
_0x164:
	LDI  R30,LOW(_EHigh*2)
	LDI  R31,HIGH(_EHigh*2)
_0x165:
	CALL SUBOPT_0x74
;    1655 			len = (t < 0) ? ELowLen : EHighLen;
	BRGE _0x167
	LDI  R30,LOW(_ELowLen*2)
	LDI  R31,HIGH(_ELowLen*2)
	CALL __GETW1PF
	RJMP _0x168
_0x167:
	LDI  R30,LOW(_EHighLen*2)
	LDI  R31,HIGH(_EHighLen*2)
	CALL __GETW1PF
_0x168:
	MOVW R18,R30
;    1656 			break;
	RJMP _0x14D
;    1657 		case 'B':
_0x163:
	CPI  R30,LOW(0x42)
	BRNE _0x16A
;    1658 			coef = (t < 630.615) ? BLow : BHigh;
	CALL SUBOPT_0x75
	BRSH _0x16B
	LDI  R30,LOW(_BLow*2)
	LDI  R31,HIGH(_BLow*2)
	RJMP _0x16C
_0x16B:
	LDI  R30,LOW(_BHigh*2)
	LDI  R31,HIGH(_BHigh*2)
_0x16C:
	CALL SUBOPT_0x76
;    1659 			len = (t < 630.615) ? BLowLen : BHighLen;
	CALL SUBOPT_0x75
	BRSH _0x16E
	LDI  R30,LOW(_BLowLen*2)
	LDI  R31,HIGH(_BLowLen*2)
	CALL __GETW1PF
	RJMP _0x16F
_0x16E:
	LDI  R30,LOW(_BHighLen*2)
	LDI  R31,HIGH(_BHighLen*2)
	CALL __GETW1PF
_0x16F:
	MOVW R18,R30
;    1660 			break;
	RJMP _0x14D
;    1661 		case 'J':
_0x16A:
	CPI  R30,LOW(0x4A)
	BRNE _0x171
;    1662 			coef = (t < 760) ? JLow : JHigh;
	CALL SUBOPT_0x77
	BRSH _0x172
	LDI  R30,LOW(_JLow*2)
	LDI  R31,HIGH(_JLow*2)
	RJMP _0x173
_0x172:
	LDI  R30,LOW(_JHigh*2)
	LDI  R31,HIGH(_JHigh*2)
_0x173:
	CALL SUBOPT_0x76
;    1663 			len = (t < 760) ? JLowLen : JHighLen;
	CALL SUBOPT_0x77
	BRSH _0x175
	LDI  R30,LOW(_JLowLen*2)
	LDI  R31,HIGH(_JLowLen*2)
	CALL __GETW1PF
	RJMP _0x176
_0x175:
	LDI  R30,LOW(_JHighLen*2)
	LDI  R31,HIGH(_JHighLen*2)
	CALL __GETW1PF
_0x176:
	MOVW R18,R30
;    1664 			break;
	RJMP _0x14D
;    1665 
;    1666 		case 'S':
_0x171:
	CPI  R30,LOW(0x53)
	BRNE _0x178
;    1667 			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
	CALL SUBOPT_0x78
	CALL SUBOPT_0x79
	BRSH _0x179
	LDI  R30,LOW(_SLow*2)
	LDI  R31,HIGH(_SLow*2)
	RJMP _0x17A
_0x179:
	CALL SUBOPT_0x7A
	BRSH _0x17C
	LDI  R30,LOW(_SMed*2)
	LDI  R31,HIGH(_SMed*2)
	RJMP _0x17D
_0x17C:
	LDI  R30,LOW(_SHigh*2)
	LDI  R31,HIGH(_SHigh*2)
_0x17D:
_0x17A:
	CALL SUBOPT_0x7B
;    1668 			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
	BRSH _0x17F
	LDI  R30,LOW(_SLowLen*2)
	LDI  R31,HIGH(_SLowLen*2)
	CALL __GETW1PF
	RJMP _0x180
_0x17F:
	CALL SUBOPT_0x7A
	BRSH _0x182
	LDI  R30,LOW(_SMedLen*2)
	LDI  R31,HIGH(_SMedLen*2)
	CALL __GETW1PF
	RJMP _0x183
_0x182:
	LDI  R30,LOW(_SHighLen*2)
	LDI  R31,HIGH(_SHighLen*2)
	CALL __GETW1PF
_0x183:
_0x180:
	MOVW R18,R30
;    1669 			break;
	RJMP _0x14D
;    1670 		case 'R':
_0x178:
	CPI  R30,LOW(0x52)
	BRNE _0x192
;    1671 			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
	CALL SUBOPT_0x78
	CALL SUBOPT_0x79
	BRSH _0x186
	LDI  R30,LOW(_RLow*2)
	LDI  R31,HIGH(_RLow*2)
	RJMP _0x187
_0x186:
	CALL SUBOPT_0x7A
	BRSH _0x189
	LDI  R30,LOW(_RMed*2)
	LDI  R31,HIGH(_RMed*2)
	RJMP _0x18A
_0x189:
	LDI  R30,LOW(_RHigh*2)
	LDI  R31,HIGH(_RHigh*2)
_0x18A:
_0x187:
	CALL SUBOPT_0x7B
;    1672 			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
	BRSH _0x18C
	LDI  R30,LOW(_RLowLen*2)
	LDI  R31,HIGH(_RLowLen*2)
	CALL __GETW1PF
	RJMP _0x18D
_0x18C:
	CALL SUBOPT_0x7A
	BRSH _0x18F
	LDI  R30,LOW(_RMedLen*2)
	LDI  R31,HIGH(_RMedLen*2)
	CALL __GETW1PF
	RJMP _0x190
_0x18F:
	LDI  R30,LOW(_RHighLen*2)
	LDI  R31,HIGH(_RHighLen*2)
	CALL __GETW1PF
_0x190:
_0x18D:
	MOVW R18,R30
;    1673 			break;
	RJMP _0x14D
;    1674 		default:
_0x192:
;    1675 			return 0.0;
	CALL SUBOPT_0x7C
	RJMP _0x494
;    1676 	}
_0x14D:
;    1677 	if (len == 0)
	MOV  R0,R18
	OR   R0,R19
	BRNE _0x193
;    1678 		return 0.0;
	CALL SUBOPT_0x7C
	RJMP _0x494
;    1679 
;    1680 	result = coef[0];
_0x193:
	LDS  R30,_coef
	LDS  R31,_coef+1
	CALL __GETD1PF
	CALL SUBOPT_0x7D
;    1681 	if(type == 'K')
	LDD  R26,Y+12
	CPI  R26,LOW(0x4B)
	BRNE _0x194
;    1682 	{
;    1683 		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	CALL SUBOPT_0x7E
	__GETD2N 0xB8F82F06
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x7E
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x7F
	__GETD2N 0x3DF2E34C
	CALL SUBOPT_0x80
;    1684 	}
;    1685 
;    1686 	pert = t;
_0x194:
	__GETD1S 13
	CALL SUBOPT_0x81
;    1687 	for(i=1;i< len ;i++)
	__GETWRN 16,17,1
_0x196:
	__CPWRR 16,17,18,19
	BRGE _0x197
;    1688 	{
;    1689 		result = result + coef[i] * t;
	MOVW R30,R16
	LDS  R26,_coef
	LDS  R27,_coef+1
	CALL SUBOPT_0x82
	CALL __GETD1PF
	CALL SUBOPT_0x78
	CALL SUBOPT_0x80
;    1690 		t = t*pert;
	CALL SUBOPT_0x1
	CALL SUBOPT_0x78
	CALL __MULF12
	__PUTD1S 13
;    1691 	}
	__ADDWRN 16,17,1
	RJMP _0x196
_0x197:
;    1692 	return result;
	CALL SUBOPT_0x83
_0x494:
	CALL __LOADLOCR4
	ADIW R28,17
	RET
;    1693 }
;    1694 
;    1695 double MValueToTValue(double r,char type)
;    1696 {
_MValueToTValue:
;    1697 	double tlow;
;    1698 	double tup;
;    1699 	double rnew;
;    1700 	double tnew;
;    1701 	int count = 0;
;    1702 
;    1703 	switch(type)
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
	LDD  R30,Y+18
;    1704 	{
;    1705 		case 'T':
	CPI  R30,LOW(0x54)
	BRNE _0x19B
;    1706 			tlow =  -270;	tup = 400;
	CALL SUBOPT_0x84
	__GETD1N 0x43C80000
	CALL SUBOPT_0x4A
;    1707 			break;
	RJMP _0x19A
;    1708 		case 'K':
_0x19B:
	CPI  R30,LOW(0x4B)
	BRNE _0x19C
;    1709 			tlow =  -270;	tup = 1372;
	CALL SUBOPT_0x84
	__GETD1N 0x44AB8000
	CALL SUBOPT_0x4A
;    1710 			break;
	RJMP _0x19A
;    1711 		case 'N':
_0x19C:
	CPI  R30,LOW(0x4E)
	BRNE _0x19D
;    1712 			tlow =  -270;	tup = 1300;
	CALL SUBOPT_0x84
	__GETD1N 0x44A28000
	CALL SUBOPT_0x4A
;    1713 			break;
	RJMP _0x19A
;    1714 		case 'E':
_0x19D:
	CPI  R30,LOW(0x45)
	BRNE _0x19E
;    1715 			tlow =  -270;	tup = 1000;
	CALL SUBOPT_0x84
	__GETD1N 0x447A0000
	CALL SUBOPT_0x4A
;    1716 			break;
	RJMP _0x19A
;    1717 		case 'B':
_0x19E:
	CPI  R30,LOW(0x42)
	BRNE _0x19F
;    1718 			tlow =  0;	tup = 1280;
	__CLRD1S 14
	__GETD1N 0x44A00000
	CALL SUBOPT_0x4A
;    1719 			break;
	RJMP _0x19A
;    1720 		case 'J':
_0x19F:
	CPI  R30,LOW(0x4A)
	BRNE _0x1A0
;    1721 			tlow =  -210;	tup = 1200;
	__GETD1N 0xC3520000
	CALL SUBOPT_0x49
	__GETD1N 0x44960000
	CALL SUBOPT_0x4A
;    1722 			break;
	RJMP _0x19A
;    1723 		case 'S':
_0x1A0:
	CPI  R30,LOW(0x53)
	BRNE _0x1A1
;    1724 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x85
;    1725 			break;
	RJMP _0x19A
;    1726 		case 'R':
_0x1A1:
	CPI  R30,LOW(0x52)
	BRNE _0x1A3
;    1727 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x85
;    1728 			break;
	RJMP _0x19A
;    1729 		default:
_0x1A3:
;    1730 			return -9999.9999;
	__GETD1N 0xC61C4000
	RJMP _0x493
;    1731 	}
_0x19A:
;    1732 
;    1733 	while((tup - tlow > 0.00005) || (count++ < 100))
_0x1A4:
	CALL SUBOPT_0x60
	BREQ PC+4
	BRCS PC+3
	JMP  _0x1A7
	CALL SUBOPT_0x4E
	BRGE _0x1A6
_0x1A7:
;    1734 	{
;    1735 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4F
;    1736 		rnew = GetThmoVolt(tnew,type);
	CALL SUBOPT_0x56
	CALL __PUTPARD1
	LDD  R30,Y+22
	ST   -Y,R30
	CALL _GetThmoVolt
	CALL SUBOPT_0x86
;    1737 
;    1738 		if(r < rnew)
	__GETD2S 19
	CALL __CMPF12
	BRSH _0x1A9
;    1739 			tup = tnew;
	CALL SUBOPT_0x66
;    1740 		else
	RJMP _0x1AA
_0x1A9:
;    1741 			tlow = tnew;
	CALL SUBOPT_0x57
;    1742 	}
_0x1AA:
	RJMP _0x1A4
_0x1A6:
;    1743 
;    1744 	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x58
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x59
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x5A
_0x493:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,23
	RET
;    1745 }
;    1746 
;    1747 #include "font.h"
;    1748 
;    1749 #include <MATH.H>
;    1750 #include "typedef.h"
;    1751 #include "global.h"
;    1752 #include "utili.h"
;    1753 #include "key.h"
;    1754 extern uchar key;
;    1755 extern u8 lcd_buffer;
;    1756 /*
;    1757         Communication with LCD/KBD board,
;    1758         Incoming command format:
;    1759                 CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
;    1760                 key:            key pressed
;    1761         Out command format:
;    1762                 CMDO_DISPLAYBTN,LED_STATUS      update the led status
;    1763 */
;    1764 /**************************主程序********************************/
;    1765 void Key_Init()
;    1766 {
_Key_Init:
;    1767 	key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    1768 }
	RET
;    1769 
;    1770 #include <string.h>
;    1771 #include "lcd.h"
;    1772 #include "font.h"
;    1773 #include "global.h"
;    1774 #include "utili.h"
;    1775 
;    1776 
;    1777 
;    1778 extern uchar key;
;    1779 static uchar kbd_state;

	.DSEG
_kbd_state_G8:
	.BYTE 0x1
;    1780 void kbd_uart_push(uchar data)
;    1781 {

	.CSEG
_kbd_uart_push:
;    1782         if(data == STATUS_IDLE||
;	data -> Y+0
;    1783            data == STATUS_ERR||
;    1784            data == STATUS_DRAW||
;    1785            data == STATUS_DONE) //lcd acknowledge
	LD   R26,Y
	CPI  R26,LOW(0xF0)
	BREQ _0x1AC
	CPI  R26,LOW(0xF1)
	BREQ _0x1AC
	CPI  R26,LOW(0xF2)
	BREQ _0x1AC
	CPI  R26,LOW(0xF3)
	BRNE _0x1AB
_0x1AC:
;    1786         {
;    1787                 kbd_state = data;
	LD   R30,Y
	STS  _kbd_state_G8,R30
;    1788                 return;
	RJMP _0x48F
;    1789         }
;    1790         if(key == KEY_INVALID)
_0x1AB:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE _0x1AE
;    1791                 key = data;
	LDD  R5,Y+0
;    1792 }
_0x1AE:
	RJMP _0x48F
;    1793 static u8 lcdcmd[40];

	.DSEG
_lcdcmd_G8:
	.BYTE 0x28
;    1794 
;    1795 static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
_head_G8:
	.BYTE 0x3
;    1796 static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};
_tail_G8:
	.BYTE 0x3
;    1797 
;    1798 static u8 hex2ch[2];
_hex2ch_G8:
	.BYTE 0x2
;    1799 void hex2char(uchar a)
;    1800 {

	.CSEG
_hex2char:
;    1801         uchar h,l;
;    1802 	h = (a & 0xf0) >> 4;
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
;    1803 	l = (a & 0x0f);
	LDD  R30,Y+2
	ANDI R30,LOW(0xF)
	MOV  R16,R30
;    1804 	if(h <= 9)
	CPI  R17,10
	BRSH _0x1B1
;    1805 		hex2ch[0] = (h+'0');
	MOV  R30,R17
	SUBI R30,-LOW(48)
	RJMP _0x4A3
;    1806 	else
_0x1B1:
;    1807 		hex2ch[0] = (h+'A'-0x0a);
	MOV  R30,R17
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4A3:
	STS  _hex2ch_G8,R30
;    1808 	if(l <= 9)
	CPI  R16,10
	BRSH _0x1B3
;    1809 		hex2ch[1] = (l+'0');
	MOV  R30,R16
	SUBI R30,-LOW(48)
	RJMP _0x4A4
;    1810 	else
_0x1B3:
;    1811 		hex2ch[1] = (l+'A'-0x0a);
	MOV  R30,R16
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4A4:
	__PUTB1MN _hex2ch_G8,1
;    1812 }
	RJMP _0x492
;    1813 void onecmd(uchar len)
;    1814 {
_onecmd:
;    1815      uchar i = 0;
;    1816      uchar sum;
;    1817      sum = 0;
	ST   -Y,R17
	ST   -Y,R16
;	len -> Y+2
;	i -> R17
;	sum -> R16
	LDI  R17,0
	LDI  R16,LOW(0)
;    1818      prints(head,3,PORT_KBD); //send head
	LDI  R30,LOW(_head_G8)
	LDI  R31,HIGH(_head_G8)
	CALL SUBOPT_0x87
;    1819      for(i = 0;i<len;i++)     //caculate cksum
	LDI  R17,LOW(0)
_0x1B6:
	LDD  R30,Y+2
	CP   R17,R30
	BRSH _0x1B7
;    1820      {
;    1821         hex2char(lcdcmd[i]);
	CALL SUBOPT_0x88
	CALL SUBOPT_0x89
;    1822 	prints(hex2ch,2,PORT_KBD);
;    1823         sum = sum + lcdcmd[i];
	CALL SUBOPT_0x88
	ADD  R16,R30
;    1824      }
	SUBI R17,-1
	RJMP _0x1B6
_0x1B7:
;    1825      lcdcmd[i] = 0xff - sum;
	MOV  R26,R17
	LDI  R27,0
	SUBI R26,LOW(-_lcdcmd_G8)
	SBCI R27,HIGH(-_lcdcmd_G8)
	LDI  R30,LOW(255)
	SUB  R30,R16
	ST   X,R30
;    1826      hex2char(lcdcmd[i]);
	CALL SUBOPT_0x88
	CALL SUBOPT_0x89
;    1827      prints(hex2ch,2,PORT_KBD);
;    1828      prints(tail,3,PORT_KBD); //send tail
	LDI  R30,LOW(_tail_G8)
	LDI  R31,HIGH(_tail_G8)
	CALL SUBOPT_0x87
;    1829 }
_0x492:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,3
	RET
;    1830 static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

	.DSEG
_idle_G8:
	.BYTE 0xA
;    1831 void sendcmd(uchar len)
;    1832 {

	.CSEG
_sendcmd:
;    1833      ulong timeout;
;    1834      if(DEBUG == 1)
	SBIW R28,4
;	len -> Y+4
;	timeout -> Y+0
;    1835         return;
;    1836      while(kbd_state != STATUS_IDLE)
_0x1BA:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1BC
;    1837      {
;    1838         if(kbd_state == STATUS_DRAW)
	CPI  R26,LOW(0xF2)
	BREQ _0x1BA
;    1839                 continue;
;    1840         if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1BE
;    1841         {
;    1842                 prints(idle,10,PORT_KBD);
	LDI  R30,LOW(_idle_G8)
	LDI  R31,HIGH(_idle_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _prints
;    1843                 kbd_state = STATUS_DRAW;
	LDI  R30,LOW(242)
	STS  _kbd_state_G8,R30
;    1844         }
;    1845      }
_0x1BE:
	RJMP _0x1BA
_0x1BC:
;    1846      onecmd(len);
	CALL SUBOPT_0x8A
;    1847      timeout = 0;
;    1848      kbd_state = STATUS_DONE;
	LDI  R30,LOW(243)
	STS  _kbd_state_G8,R30
;    1849      while(kbd_state != STATUS_IDLE)
_0x1BF:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1C1
;    1850      {
;    1851          if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1C2
;    1852          {
;    1853                 onecmd(len);
	CALL SUBOPT_0x8A
;    1854                 timeout = 0;
;    1855          }
;    1856          if(timeout < 655350)
_0x1C2:
	CALL SUBOPT_0x6
	__CPD2N 0x9FFF6
	BRSH _0x1C3
;    1857          {
;    1858                 sleepms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x35
;    1859                 timeout++;
	CALL SUBOPT_0x7
	CALL SUBOPT_0x3F
	__PUTD1S 0
;    1860          }else{
	RJMP _0x1C4
_0x1C3:
;    1861                 onecmd(len);
	CALL SUBOPT_0x8A
;    1862                 timeout = 0;
;    1863          }
_0x1C4:
;    1864      }
	RJMP _0x1BF
_0x1C1:
;    1865 }
_0x491:
	ADIW R28,5
	RET
;    1866 /*------------------初始化-----------------*/
;    1867 void LCD_Init (void)
;    1868 {
_LCD_Init:
;    1869         kbd_state = STATUS_IDLE;
	LDI  R30,LOW(240)
	STS  _kbd_state_G8,R30
;    1870         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x8B
;    1871         sendcmd(1);
;    1872         //do it twice avoid the missing of first command on boot up
;    1873         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x8B
;    1874         sendcmd(1);
;    1875 
;    1876 }
	RET
;    1877 
;    1878 /********************************************************/
;    1879 /* 清屏							*/
;    1880 /********************************************************/
;    1881 void LCD_Cls(void)
;    1882 {
_LCD_Cls:
;    1883         lcdcmd[0] = CMDO_LCD_CLS;
	LDI  R30,LOW(4)
	STS  _lcdcmd_G8,R30
;    1884         sendcmd(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _sendcmd
;    1885 }
	RET
;    1886 void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
;    1887 {
_LCD_ClsBlock:
;    1888         lcdcmd[0] = CMDO_LCD_CLSBLK;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(6)
	CALL SUBOPT_0x8C
;    1889         lcdcmd[1] = x1;
;    1890         lcdcmd[2] = y1;
;    1891         lcdcmd[3] = x2;
;    1892         lcdcmd[4] = y2;
;    1893         sendcmd(5);
;    1894 }
	RJMP _0x490
;    1895 void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
;    1896 {
_display_buttons:
;    1897         lcdcmd[0] = CMDO_DISPLAYBTN;
;	btn_pos -> Y+1
;	btn_val -> Y+0
	LDI  R30,LOW(2)
	STS  _lcdcmd_G8,R30
;    1898         if(btn_val == 0)
	LD   R30,Y
	CPI  R30,0
	BRNE _0x1C5
;    1899 	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
	LDD  R30,Y+1
	SUBI R30,-LOW(4)
	STD  Y+1,R30
;    1900         lcdcmd[1] = btn_pos;
_0x1C5:
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,1
;    1901         sendcmd(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _sendcmd
;    1902 }
	ADIW R28,2
	RET
;    1903 
;    1904 
;    1905 /*	设定图形x,y值*/
;    1906 /*
;    1907 void LCD_GraphSetAddr(uchar x,uchar y)
;    1908 {
;    1909 	uint xy;
;    1910 	xy=y;
;    1911 	xy=xy*16+x+256;
;    1912   	//LCD_CE(0);
;    1913 	LCD_Write2(xy&0xff,xy/256,0x24);
;    1914 }
;    1915 */
;    1916 /*	设定文本x,y值	*/
;    1917 /*
;    1918 void LCD_TextSetAddr(uchar x,uchar y)
;    1919 {
;    1920   	//LCD_CE(0);
;    1921 	LCD_Write2(y*16+x,0,0x24);
;    1922 }
;    1923 */
;    1924 /*	清除一点*/
;    1925 /*
;    1926 void LCD_ClrPixel(uchar x,uchar y)
;    1927 {
;    1928 	uchar b;
;    1929 	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;
;    1930 
;    1931 	b = 7 - (x % 8);
;    1932 
;    1933 	LCD_WriteInt(addr,0x24);
;    1934 	LCD_Comm(0xf0|b);
;    1935 }
;    1936 */
;    1937 /*	点亮一点	*/
;    1938 /*
;    1939 void LCD_Putpixel(uchar x,uchar y)
;    1940 {
;    1941 	uchar b;
;    1942 	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;
;    1943 
;    1944 	b = 7 - (x % 8);
;    1945 
;    1946 	LCD_WriteInt(addr,0x24);
;    1947 	LCD_Comm(0xf8|b);
;    1948 }
;    1949 */
;    1950 /*	x,y处显示光标	*/
;    1951 /*
;    1952 void LCD_ShowCursor(uchar x,uchar y)
;    1953 {
;    1954 	return;
;    1955   	//LCD_CE(0);
;    1956 	LCD_Comm(0x97);	//光标开
;    1957 	LCD_Write2(x,y,0x21);
;    1958 }
;    1959 
;    1960 //	取消光标
;    1961 
;    1962 void LCD_HideCursor(void)
;    1963 {
;    1964 	return;
;    1965   	//LCD_CE(0);
;    1966 	LCD_Comm(0x9c);
;    1967 }
;    1968 */
;    1969 /*
;    1970 void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
;    1971 {
;    1972 	x =(x / 8) * 8;
;    1973 
;    1974 	while(*s)
;    1975 	{
;    1976 		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
;    1977 
;    1978 		x = x + 8;
;    1979 
;    1980 		s++;
;    1981 	}
;    1982 }
;    1983 
;    1984 
;    1985 void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
;    1986 {
;    1987 	x =(x / 8) * 8;
;    1988 	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
;    1989 }
;    1990 */
;    1991 
;    1992 void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
;    1993 {
_LCD_ReverseRect:
;    1994         lcdcmd[0] = CMDO_LCD_REVERSE;
;	x -> Y+3
;	y -> Y+2
;	w -> Y+1
;	h -> Y+0
	LDI  R30,LOW(5)
	CALL SUBOPT_0x8C
;    1995         lcdcmd[1] = x;
;    1996         lcdcmd[2] = y;
;    1997         lcdcmd[3] = w;
;    1998         lcdcmd[4] = h;
;    1999         sendcmd(5);
;    2000 }
	RJMP _0x490
;    2001 /*--------------显示字符------------------*/
;    2002 /*
;    2003 void LCD_TextPutchar(uchar x,uchar y,uchar c)
;    2004 {
;    2005   	//LCD_CE(0);
;    2006   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2007   	LCD_Comm(0xb0);
;    2008 	LCD_AutoWrite(c - 0x20);
;    2009   	LCD_Comm(0xb2);
;    2010 }
;    2011 
;    2012 void LCD_TextPrint(uchar x,uchar y,char *s)
;    2013 {
;    2014   	//LCD_CE(0);
;    2015   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2016   	LCD_Comm(0xb0);
;    2017   	while(*s)
;    2018   	{
;    2019 		LCD_AutoWrite(*s - 0x20);
;    2020 		s++;
;    2021 	}
;    2022   	LCD_Comm(0xb2);
;    2023 }
;    2024 void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
;    2025 {
;    2026 	uchar buf[4];
;    2027 	int i;
;    2028 
;    2029 	if(start > 3) start = 0;
;    2030 
;    2031 	for(i = 0; i < 4; i++)
;    2032 	{
;    2033 		buf[i] = n % 10;
;    2034 		n /= 10;
;    2035 	}
;    2036 
;    2037 	for(i = 3-start; i >= 0; i--)
;    2038 	{
;    2039 		LCD_TextPutchar(x,y,'0' + buf[i]);
;    2040 		x ++;
;    2041 	}
;    2042 }
;    2043 
;    2044 void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
;    2045 {
;    2046 	uchar c1,c2;
;    2047 	//低4位
;    2048 	c1 = hex & 0x0f;	//lo
;    2049 	//高4位
;    2050 	c2 = (hex >> 4) & 0x0f; //hi
;    2051 
;    2052 	LCD_TextPutchar(x,y,HexTable[c2]);
;    2053 	LCD_TextPutchar(x+1,y,HexTable[c1]);
;    2054 }
;    2055 */
;    2056 /************************************************/
;    2057 /*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
;    2058 /************************************************/
;    2059 /*
;    2060 void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
;    2061 {
;    2062 	register uchar t;
;    2063 	int xerr=0,yerr=0,delta_x,delta_y,distance;
;    2064 	int incx,incy,uRow,uCol;
;    2065 
;    2066 	delta_x = xt-x0;				//计算坐标增量
;    2067 	delta_y = yt-y0;
;    2068 	uRow = x0;
;    2069 	uCol = y0;
;    2070 	if(delta_x>0) incx=1;				//设置单步方向
;    2071 	else if( delta_x==0 ) incx=0;			//垂直线
;    2072 	else {incx=-1;delta_x=-delta_x;}
;    2073 
;    2074 	if(delta_y>0) incy=1;
;    2075 	else if( delta_y==0 ) incy=0;			//水平线
;    2076 	else {incy=-1;delta_y=-delta_y;}
;    2077 
;    2078 	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
;    2079 	else distance=delta_y;
;    2080 
;    2081   	//LCD_CE(0);
;    2082 	for( t=0;t <= distance+1; t++ )
;    2083         {					//画线输出
;    2084 		LCD_Putpixel(uRow,uCol);			//画点
;    2085 		xerr +=	delta_x	;
;    2086 		yerr +=	delta_y	;
;    2087 
;    2088 		if( xerr > distance )
;    2089                	{
;    2090 			xerr-=distance;
;    2091 			uRow+=incx;
;    2092 		}
;    2093 		if( yerr > distance )
;    2094                	{
;    2095 			yerr-=distance;
;    2096 			uCol+=incy;
;    2097 		}
;    2098 	}
;    2099 }
;    2100 */
;    2101 /*
;    2102 void LCD_LineH(uchar y)
;    2103 {
;    2104 	char i;
;    2105 
;    2106   	//LCD_CE(0);
;    2107   	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
;    2108 	LCD_Comm(0xb0);
;    2109 	for(i=0;i<LCD_WIDTH;i++)
;    2110  		LCD_AutoWrite(0xff);
;    2111 	LCD_Comm(0xb2);
;    2112 }
;    2113 */
;    2114 /*
;    2115 void LCD_LineV(uchar x,uchar y1,uchar y2)
;    2116 {
;    2117 	int i;
;    2118   	//LCD_CE(0);
;    2119 	for(i = y1; i < y2; i++)
;    2120 	{
;    2121 		LCD_Putpixel(x,i);
;    2122 	}
;    2123 }
;    2124 */
;    2125 
;    2126 void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
;    2127 {
_LCD_Rectange:
;    2128         lcdcmd[0] = CMDO_LCD_RECT;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(7)
	CALL SUBOPT_0x8C
;    2129         lcdcmd[1] = x1;
;    2130         lcdcmd[2] = y1;
;    2131         lcdcmd[3] = x2;
;    2132         lcdcmd[4] = y2;
;    2133         sendcmd(5);
;    2134 }
	RJMP _0x490
;    2135 void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
;    2136 {
_LCD_PrintChar:
;    2137         uchar pos = 0;
;    2138         lcdcmd[0] = cmd;
	ST   -Y,R17
;	cmd -> Y+5
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	pos -> R17
	LDI  R17,0
	LDD  R30,Y+5
	STS  _lcdcmd_G8,R30
;    2139         lcdcmd[1] = x;
	LDD  R30,Y+4
	__PUTB1MN _lcdcmd_G8,1
;    2140         lcdcmd[2] = y;
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,2
;    2141         pos = 1;
	LDI  R17,LOW(1)
;    2142         while(*s)
_0x1C6:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x1C8
;    2143         {
;    2144                 lcdcmd[(uchar)(pos + 3)] = *s++;
	CALL SUBOPT_0x8D
	MOVW R0,R30
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOVW R26,R0
	ST   X,R30
;    2145                 pos = pos + 1;
	SUBI R17,-LOW(1)
;    2146         }
	RJMP _0x1C6
_0x1C8:
;    2147         lcdcmd[(uchar)(pos+3)] = 0x00;
	CALL SUBOPT_0x8D
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2148         lcdcmd[3] = pos;
	__POINTW1MN _lcdcmd_G8,3
	ST   Z,R17
;    2149         pos = pos + 4;
	SUBI R17,-LOW(4)
;    2150         sendcmd(pos);
	ST   -Y,R17
	CALL _sendcmd
;    2151 }
	LDD  R17,Y+0
	ADIW R28,6
	RET
;    2152 void LCD_PrintHz12(uchar x,uchar y,uchar *s)
;    2153 {
_LCD_PrintHz12:
;    2154         LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(8)
	CALL SUBOPT_0x8E
;    2155 }
	RJMP _0x490
;    2156 void LCD_PrintHz16(uchar x,uchar y,uchar *s)
;    2157 {
_LCD_PrintHz16:
;    2158         LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(9)
	CALL SUBOPT_0x8E
;    2159 }
	RJMP _0x490
;    2160 //显示6x8的数字
;    2161 void LCD_Print6X8(uchar x, uchar y,uchar *s)
;    2162 {
_LCD_Print6X8:
;    2163         LCD_PrintChar(CMDO_LCD_68,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(16)
	CALL SUBOPT_0x8E
;    2164 }
	RJMP _0x490
;    2165 
;    2166 //显示6x8的数字
;    2167 void LCD_Print8X16(uchar x, uchar y,uchar *s)
;    2168 {
_LCD_Print8X16:
;    2169         LCD_PrintChar(CMDO_LCD_816,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(17)
	CALL SUBOPT_0x8E
;    2170 }
_0x490:
	ADIW R28,4
	RET
;    2171 
;    2172 //显示24x32的数字
;    2173 /*
;    2174 void LCD_Print24X32(uchar x, uchar y,uchar *s)
;    2175 {
;    2176 	//x坐标必需是8位对齐
;    2177 	x =(x / 8) * 8;
;    2178 	while(*s)
;    2179 	{
;    2180 		if( *s >= '0' && *s <= '9')	//显示
;    2181 		{
;    2182 			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
;    2183 			x += 24;
;    2184 		}
;    2185 		else if( *s == ' ')
;    2186 		{
;    2187 			x += 8;
;    2188 		}
;    2189 		else if( *s == '-')
;    2190 		{
;    2191 			LCD_PutImg(x,y,3,32,Font24X32[12]);
;    2192 			x += 24;
;    2193 		}
;    2194 		else
;    2195 		{
;    2196 			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
;    2197 			x += 8;
;    2198 		}
;    2199 		s ++;
;    2200 	}
;    2201 }*/
;    2202 #include "utili.h"
;    2203 #include "scanner.h"
;    2204 #include "stdlib.h"
;    2205 
;    2206 static uchar navlen = 0;

	.DSEG
_navlen_G9:
	.BYTE 0x1
;    2207 static double reading = -1000;
_reading_G9:
	.BYTE 0x4
;    2208 static char navread[20];
_navread_G9:
	.BYTE 0x14
;    2209 static char navread2[20];
_navread2_G9:
	.BYTE 0x14
;    2210 u8 scancmd[5];
_scancmd:
	.BYTE 0x5
;    2211 u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE

	.ESEG
_scanner_type:
	.DB  0x1
;    2212 void scanner_set_mode()
;    2213 {

	.CSEG
_scanner_set_mode:
;    2214         if (IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x1CA
;    2215         {
;    2216             scancmd[0] = '#';
	LDI  R30,LOW(35)
	RJMP _0x4A5
;    2217         }else{
_0x1CA:
;    2218             scancmd[0]= '!';
	LDI  R30,LOW(33)
_0x4A5:
	STS  _scancmd,R30
;    2219         }
;    2220         scancmd[1] = 0x0D;
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2221         scancmd[2] = 0x0A;
	__POINTW1MN _scancmd,2
	CALL SUBOPT_0x8F
;    2222         prints(scancmd,3,PORT_SCANNER);
	LDI  R30,LOW(3)
	ST   -Y,R30
	CALL SUBOPT_0x34
;    2223 }
	RET
;    2224 void scanner_set_channel(uchar ch)
;    2225 {
_scanner_set_channel:
;    2226         if(scanner_type == 1) //MI
;	ch -> Y+0
	CALL SUBOPT_0x90
	CPI  R30,LOW(0x1)
	BRNE _0x1CC
;    2227         {
;    2228                 if(ch < 10)
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRSH _0x1CD
;    2229                 {
;    2230                         scancmd[0] = (ch + '0');
	LD   R30,Y
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2231                         scancmd[1] = 'A';
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    2232                         scancmd[2] = 0x0D;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2233                         scancmd[3] = 0x0A;
	__POINTW1MN _scancmd,3
	CALL SUBOPT_0x8F
;    2234                         prints(scancmd,4,PORT_SCANNER);
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL SUBOPT_0x34
;    2235                         return;
	RJMP _0x48F
;    2236                 }else{
_0x1CD:
;    2237                         scancmd[0] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2238                         ch = ch % 10;
	CALL SUBOPT_0x91
;    2239                         scancmd[1] = ch + '0';
	__PUTB1MN _scancmd,1
;    2240                         scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(65)
	STD  Z+0,R26
	CALL SUBOPT_0x92
;    2241                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL SUBOPT_0x34
;    2242                 }
;    2243         }
;    2244         if(scanner_type == 2) //guidline
_0x1CC:
	CALL SUBOPT_0x90
	CPI  R30,LOW(0x2)
	BRNE _0x1CF
;    2245         {
;    2246                         scancmd[0] = 'A';
	LDI  R30,LOW(65)
	STS  _scancmd,R30
;    2247                         scancmd[1] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	__PUTB1MN _scancmd,1
;    2248                         ch = ch % 10;
	CALL SUBOPT_0x91
;    2249                         scancmd[2] = ch + '0';
	__PUTB1MN _scancmd,2
;    2250                         scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	CALL SUBOPT_0x92
;    2251                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL SUBOPT_0x34
;    2252         }
;    2253 }
_0x1CF:
	RJMP _0x48F
;    2254 
;    2255 
;    2256 //incoming data handler of scanner
;    2257 void scanner_uart_push(uchar data)
;    2258 {
_scanner_uart_push:
;    2259 }
	RJMP _0x48F
;    2260 //incoming data hander of PC
;    2261 void pc_uart_push(uchar data)
;    2262 {
_pc_uart_push:
;    2263 }
	RJMP _0x48F
;    2264 static uchar navcmd[12];

	.DSEG
_navcmd_G9:
	.BYTE 0xC
;    2265 #define NAV_INVALID     -1000
;    2266 #define NAV_VALID       -2000
;    2267 #define RESETNAV       navlen = 0;   reading = NAV_INVALID;
;    2268 
;    2269 void nav_command(uchar cmd)
;    2270 {

	.CSEG
_nav_command:
;    2271         if(cmd == NAV_30V)
;	cmd -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1D0
;    2272                sprintf(navcmd,"%%01;12;02\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,103
	CALL SUBOPT_0x2F
;    2273         if(cmd == NAV_1V)
_0x1D0:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x1D1
;    2274                sprintf(navcmd,"%%01;12;01\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,115
	CALL SUBOPT_0x2F
;    2275         if(cmd == NAV_120MV)
_0x1D1:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x1D2
;    2276                sprintf(navcmd,"%%01;12;00\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,127
	CALL SUBOPT_0x2F
;    2277         if(cmd == NAV_AFLTOFF)
_0x1D2:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x1D3
;    2278                sprintf(navcmd,"%%01;26\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,139
	CALL SUBOPT_0x2F
;    2279         if(cmd == NAV_AFLTON)
_0x1D3:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x1D4
;    2280                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,148
	CALL SUBOPT_0x2F
;    2281         if(cmd == NAV_SLOWMODE)
_0x1D4:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x1D5
;    2282                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,148
	CALL SUBOPT_0x2F
;    2283         if(cmd == NAV_ZEROON)
_0x1D5:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x1D6
;    2284         {
;    2285                sprintf(navcmd,"%%01;06\r");
	CALL SUBOPT_0x93
	__POINTW1FN _0,157
	CALL SUBOPT_0x2F
;    2286         }
;    2287         if(cmd == NAV_INIT)
_0x1D6:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1D7
;    2288         {
;    2289                 RESETNAV;
	CALL SUBOPT_0x94
;    2290                 sprintf(navcmd,"%%01;00\r");
	__POINTW1FN _0,166
	CALL SUBOPT_0x2F
;    2291         }
;    2292         if(cmd == NAV_READ)
_0x1D7:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0x1D8
;    2293         {
;    2294                 RESETNAV;
	CALL SUBOPT_0x94
;    2295                 sprintf(navcmd,"%%01;01\r");
	__POINTW1FN _0,175
	CALL SUBOPT_0x2F
;    2296         }
;    2297 //                com1_putc('?');
;    2298         prints(navcmd,strlen(navcmd),PORT_NAV);
_0x1D8:
	CALL SUBOPT_0x93
	CALL SUBOPT_0x93
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _prints
;    2299 }
	RJMP _0x48F
;    2300 
;    2301 
;    2302 unsigned long navtime;

	.DSEG
_navtime:
	.BYTE 0x4
;    2303 double nav_read()
;    2304 {

	.CSEG
_nav_read:
;    2305         if(DEBUG == 1)
;    2306                 return 1.0;
;    2307         nav_command(NAV_READ);
	CALL SUBOPT_0x95
;    2308         navtime = 0;
;    2309         while(1)
_0x1DA:
;    2310         {
;    2311                 if(reading < NAV_INVALID) //make sure it is a valid reading
	LDS  R26,_reading_G9
	LDS  R27,_reading_G9+1
	LDS  R24,_reading_G9+2
	LDS  R25,_reading_G9+3
	CALL SUBOPT_0x96
	CALL __CMPF12
	BRSH _0x1DD
;    2312                 {
;    2313                      reading = atof(navread2);
	LDI  R30,LOW(_navread2_G9)
	LDI  R31,HIGH(_navread2_G9)
	ST   -Y,R31
	ST   -Y,R30
	CALL _atof
	CALL SUBOPT_0x97
;    2314                      return reading;
	LDS  R30,_reading_G9
	LDS  R31,_reading_G9+1
	LDS  R22,_reading_G9+2
	LDS  R23,_reading_G9+3
	RET
;    2315                 }
;    2316                 if(navtime++ > 655350)
_0x1DD:
	LDS  R30,_navtime
	LDS  R31,_navtime+1
	LDS  R22,_navtime+2
	LDS  R23,_navtime+3
	CALL SUBOPT_0x3F
	STS  _navtime,R30
	STS  _navtime+1,R31
	STS  _navtime+2,R22
	STS  _navtime+3,R23
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	__CPD1N 0x9FFF7
	BRLO _0x1DE
;    2317                 {
;    2318                      nav_command(NAV_READ);
	CALL SUBOPT_0x95
;    2319                      navtime = 0;
;    2320                 }
;    2321                 sleepms(1);
_0x1DE:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x35
;    2322         }
	RJMP _0x1DA
;    2323         return 1.0;
;    2324 }
;    2325 //incoming data hander of navameter
;    2326 void nav_uart_push(uchar data)
;    2327 {
_nav_uart_push:
;    2328         if(navlen >= 19)
;	data -> Y+0
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x13)
	BRLO _0x1DF
;    2329         {
;    2330                 RESETNAV;
	CALL SUBOPT_0x98
;    2331         }
;    2332         if(((data >= '0') && (data <= '9')) ||
_0x1DF:
;    2333                 (data == '.') ||
;    2334                 (data == '-') ||
;    2335                 (data == '+') ||
;    2336                 (data == 'e') ||
;    2337                 (data == 'E') )
	LD   R26,Y
	CPI  R26,LOW(0x30)
	BRLO _0x1E1
	CPI  R26,LOW(0x3A)
	BRLO _0x1E3
_0x1E1:
	LD   R26,Y
	CPI  R26,LOW(0x2E)
	BREQ _0x1E3
	CPI  R26,LOW(0x2D)
	BREQ _0x1E3
	CPI  R26,LOW(0x2B)
	BREQ _0x1E3
	CPI  R26,LOW(0x65)
	BREQ _0x1E3
	CPI  R26,LOW(0x45)
	BRNE _0x1E0
_0x1E3:
;    2338         {
;    2339                 navread[navlen++] = data;
	LDS  R30,_navlen_G9
	SUBI R30,-LOW(1)
	STS  _navlen_G9,R30
	CALL SUBOPT_0x45
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	LD   R26,Y
	STD  Z+0,R26
;    2340                 reading = NAV_INVALID;
	CALL SUBOPT_0x96
	CALL SUBOPT_0x97
;    2341                 return;
	RJMP _0x48F
;    2342         }
;    2343 
;    2344         if(navlen < 4) //not enough digits
_0x1E0:
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x4)
	BRSH _0x1E5
;    2345         {
;    2346                 RESETNAV;
	CALL SUBOPT_0x98
;    2347                 return;
	RJMP _0x48F
;    2348         }
;    2349         navread[navlen] = '\0';
_0x1E5:
	CALL SUBOPT_0x99
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2350         do
_0x1E7:
;    2351         {
;    2352                 navread2[navlen] = navread[navlen];
	LDS  R26,_navlen_G9
	LDI  R27,0
	SUBI R26,LOW(-_navread2_G9)
	SBCI R27,HIGH(-_navread2_G9)
	CALL SUBOPT_0x99
	LD   R30,Z
	ST   X,R30
;    2353         }while(navlen-- > 0);
	LDS  R26,_navlen_G9
	SUBI R26,LOW(1)
	STS  _navlen_G9,R26
	SUBI R26,-LOW(1)
	CPI  R26,LOW(0x1)
	BRSH _0x1E7
;    2354         navlen = 0;
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
;    2355         reading =  NAV_VALID; //valid data
	__GETD1N 0xC4FA0000
	CALL SUBOPT_0x97
;    2356 }
_0x48F:
	ADIW R28,1
	RET
;    2357 #include <mega64.h>
;    2358 #include "lcd.h"
;    2359 #include "key.h"
;    2360 #include "utili.h"
;    2361 #include "eeprom.h"
;    2362 #include "window.h"
;    2363 #include "utili.h"
;    2364 #include "scanner.h"
;    2365 
;    2366 extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
;    2367 #define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
;    2368                 		if(new_page == 1){\
;    2369 		                	for(i = min_option; i <= max_option; i++){\
;    2370 				                j = ((curr_pos-1)/max_option)*max_option + i;
;    2371 
;    2372 #define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
;    2373 				        if((curr_pos-1) % max_option == (i-1))\
;    2374 					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2375         			        }\
;    2376                                 }else{\
;    2377 	                		for(i = min_option; i <= max_option; i++) {\
;    2378         		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2379 				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2380 			                }\
;    2381 		                }
;    2382 
;    2383 #define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
;    2384 			if(curr_pos > max_index) curr_pos = min_index;\
;    2385 			if(last_pos == curr_pos)	return;\
;    2386 			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}
;    2387 
;    2388 #define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
;    2389 			if(curr_pos < min_index) curr_pos = max_index;\
;    2390 			if(last_pos == curr_pos)  return;\
;    2391 			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}
;    2392 
;    2393 #define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
;    2394 					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
;    2395 
;    2396 #define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
;    2397                                         draw_label(&PRE##options[i-1], SW_NORMAL);\
;    2398 			        }
;    2399 #define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
;    2400                                         return;\
;    2401         			for(i = min_option;i <= max_option; i++){\
;    2402 	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2403         				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2404         			}
;    2405 
;    2406 #define INC_OPTION	last_sel = curr_sel;\
;    2407 			curr_sel++;\
;    2408 			if(curr_sel > max_option)\
;    2409 				curr_sel = min_option;
;    2410 
;    2411 #define DEC_OPTION	last_sel = curr_sel;\
;    2412 			curr_sel--;\
;    2413 			if(curr_sel < min_option)\
;    2414 				curr_sel = max_option;
;    2415 
;    2416 extern int   curr_ch;	//index of current channel in configuation   menu
;    2417 extern int   curr_prb;	//index of current probe selection
;    2418 extern int   curr_dispch; //start of current channel for display in main menu
;    2419 
;    2420 void State_Init() {
_State_Init:
;    2421 	display_buttons(KEY_BTN1,sysdata.prbmode);
	LDI  R30,LOW(97)
	ST   -Y,R30
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2422 	display_buttons(KEY_BTN2,sysdata.kttmode);
	LDI  R30,LOW(98)
	ST   -Y,R30
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2423 	display_buttons(KEY_BTN3,0); //probe type
	LDI  R30,LOW(99)
	CALL SUBOPT_0x20
;    2424 	display_buttons(KEY_BTN4,0); //zero
	LDI  R30,LOW(100)
	CALL SUBOPT_0x20
;    2425 }
	RET
;    2426 char* getprbtype(uchar);
;    2427 LABEL flash pgmain_chs[] = {
;    2428 		{LBL_HZ6X8,3, 3,5,strbuf},
;    2429 		{LBL_HZ6X8,3,16,5,strbuf},
;    2430 		{LBL_HZ6X8,3,29,5,strbuf},
;    2431 		{LBL_HZ6X8,3,42,5,strbuf}
;    2432 	};
;    2433 LABEL flash pgmain_values[] = {
;    2434 		{LBL_HZ6X8,40,3,10,strbuf},
;    2435 		{LBL_HZ6X8,40,16,10,strbuf},
;    2436 		{LBL_HZ6X8,40,29,10,strbuf},
;    2437 		{LBL_HZ6X8,40,42,10,strbuf}
;    2438 	};
;    2439 LABEL flash pgmain_temps[] = {
;    2440 		{LBL_HZ6X8,140,3, 10,strbuf},
;    2441 		{LBL_HZ6X8,140,16,10,strbuf},
;    2442 		{LBL_HZ6X8,140,29,10,strbuf},
;    2443 		{LBL_HZ6X8,140,42,10,strbuf}
;    2444 };
;    2445 /*      Main display      */
;    2446 
;    2447 void pgmain_handler(uchar msg) {
_pgmain_handler:
;    2448         int max_option = 4;
;    2449         uchar i,old_ch,isinit;
;    2450                 uchar *cptr;
;    2451 	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
;    2452 
;    2453 	isinit = 0;
	SBIW R28,2
	CALL __SAVELOCR6
;	msg -> Y+8
;	max_option -> R16,R17
;	i -> R19
;	old_ch -> R18
;	isinit -> R21
;	*cptr -> Y+6
	LDI  R16,4
	LDI  R17,0
	LDI  R21,LOW(0)
;    2454         //Menu
;    2455         if(msg == KEY_CE) {
	LDD  R26,Y+8
	CPI  R26,LOW(0x43)
	BRNE _0x1E9
;    2456                 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2457                 return;
	RJMP _0x48E
;    2458         }
;    2459 	if(msg == KEY_TAB) {
_0x1E9:
	LDD  R26,Y+8
	CPI  R26,LOW(0x54)
	BRNE _0x1EA
;    2460 		if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x1EB
;    2461 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4A6
;    2462 		else
_0x1EB:
;    2463 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4A6:
	MOV  R4,R30
;    2464 		return;
	RJMP _0x48E
;    2465 	}
;    2466         if(key == KEY_NUM1) //R0
_0x1EA:
	LDI  R30,LOW(49)
	CP   R30,R5
	BRNE _0x1ED
;    2467         {
;    2468                 window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    2469                 if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x1EE
;    2470                 {
;    2471                         sprintf(strbuf,"请输入铂电阻R0阻值");
	CALL SUBOPT_0x0
	__POINTW1FN _0,184
	CALL SUBOPT_0x2F
;    2472 	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x9A
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	RJMP _0x4A7
;    2473 		        msg = MSG_INIT;
;    2474 		}else{
_0x1EE:
;    2475 		        return;
	RJMP _0x48E
;    2476                         sprintf(strbuf,"请输入V0值");
;    2477 	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
_0x4A7:
	CALL __EEPROMWRD
;    2478                         msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2479 		}
;    2480         }
;    2481         if(key == KEY_NUM2) //Rs1
_0x1ED:
	LDI  R30,LOW(50)
	CP   R30,R5
	BRNE _0x1F0
;    2482         {
;    2483                 if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x1F1
;    2484                 {
;    2485                         window_setup(10);
	CALL SUBOPT_0x9B
;    2486                         sprintf(strbuf,"请输入内标准阻值");
	__POINTW1FN _0,214
	CALL SUBOPT_0x2F
;    2487 		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x29
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    2488         		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2489                 }
;    2490         }
_0x1F1:
;    2491 	if(msg == KEY_UP) {
_0x1F0:
	LDD  R26,Y+8
	CPI  R26,LOW(0x55)
	BRNE _0x1F2
;    2492 		DEC_DISPCH;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	__SUBWRR 10,11,30,31
	MOV  R0,R10
	OR   R0,R11
	BRNE _0x1F3
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	MOVW R10,R30
_0x1F3:
;    2493 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2494 	}
;    2495 	if(msg == KEY_DN) {
_0x1F2:
	LDD  R26,Y+8
	CPI  R26,LOW(0x44)
	BRNE _0x1F4
;    2496 		INC_DISPCH;
	CALL SUBOPT_0x9C
	BRGE _0x1F5
	CALL SUBOPT_0x38
_0x1F5:
;    2497 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2498 	}
;    2499 
;    2500 	if(msg == MSG_INIT) {
_0x1F4:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFF)
	BRNE _0x1F6
;    2501 	        isinit = 1;
	LDI  R21,LOW(1)
;    2502 		LCD_Cls();
	CALL _LCD_Cls
;    2503 		old_ch = curr_dispch;
	CALL SUBOPT_0x9D
;    2504 
;    2505 	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2506 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1F8:
	CPI  R19,4
	BRSH _0x1F9
;    2507 		{
;    2508 		        if(curr_dispch == old_ch)
	MOV  R30,R18
	MOVW R26,R10
	CALL SUBOPT_0x9E
	BRNE _0x1FA
;    2509 		        {
;    2510         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9F
;    2511                                 draw_label(&pgmain_chs[i],SW_NORMAL);
	CALL SUBOPT_0x32
;    2512                                 draw_label(&pgmain_chs[i],SW_REVERSE);
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	RJMP _0x4A8
;    2513         		}
;    2514         		else{
_0x1FA:
;    2515         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9F
;    2516         			draw_label(&pgmain_chs[i],SW_NORMAL);
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
_0x4A8:
	ST   -Y,R30
	CALL _draw_label
;    2517         		}
;    2518 			INC_DISPCH;
	CALL SUBOPT_0x9C
	BRGE _0x1FC
	CALL SUBOPT_0x38
_0x1FC:
;    2519 		}
	SUBI R19,-1
	RJMP _0x1F8
_0x1F9:
;    2520 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2521 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+8,R30
;    2522 	}
;    2523 
;    2524 	if(msg == MSG_REFRESH) {
_0x1F6:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x1FD
;    2525 		old_ch = curr_dispch;
	CALL SUBOPT_0x9D
;    2526 		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2527 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1FF:
	CPI  R19,4
	BRLO PC+3
	JMP _0x200
;    2528 		{
;    2529 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BREQ PC+3
	JMP _0x201
;    2530 		        {
;    2531 			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
	CALL SUBOPT_0x3D
	__POINTW2MN _tprbdata,576
	CALL SUBOPT_0xA0
;    2532         			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2533 	        			(cptr[0] == 'P')){
	CALL SUBOPT_0x3D
	CPI  R30,LOW(0xFF)
	BREQ _0x203
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BRNE _0x202
_0x203:
;    2534                				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA1
;    2535                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xA2
	BREQ _0x206
	CPI  R21,1
	BRNE _0x205
_0x206:
;    2536                                         {
;    2537                         			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0x32
;    2538 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(1)
	RJMP _0x4A9
;    2539 		                        }else{
_0x205:
;    2540                         			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA5
;    2541 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(5)
_0x4A9:
	ST   -Y,R30
	CALL _draw_label
;    2542 		                        }
;    2543 			        }else{
	RJMP _0x209
_0x202:
;    2544 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA6
	CALL SUBOPT_0xA7
;    2545 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xA2
	BRNE _0x20A
;    2546         					sprintf(strbuf+8,"*mV");
	CALL SUBOPT_0xA8
	__POINTW1FN _0,254
	RJMP _0x4AA
;    2547                                         else
_0x20A:
;    2548                                                 sprintf(strbuf+8," mV");
	CALL SUBOPT_0xA8
	__POINTW1FN _0,258
_0x4AA:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA9
;    2549                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xA2
	BREQ _0x20D
	CPI  R21,1
	BRNE _0x20C
_0x20D:
;    2550                                         {
;    2551         					draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0x32
;    2552         					if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0xAA
	CALL SUBOPT_0x16
	CALL SUBOPT_0xAB
	BRSH _0x20F
;    2553         					        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,262
	CALL SUBOPT_0x2F
;    2554         					else
	RJMP _0x210
_0x20F:
;    2555                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xAC
	CALL SUBOPT_0x12
	CALL SUBOPT_0xA7
;    2556 
;    2557                 				sprintf(strbuf+8," ! %c",cptr[0]);
_0x210:
	CALL SUBOPT_0xA8
	CALL SUBOPT_0xAD
;    2558                 			        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(1)
	RJMP _0x4AB
;    2559                                         }else{
_0x20C:
;    2560                                                 draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA5
;    2561                 				if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0xAA
	CALL SUBOPT_0x16
	CALL SUBOPT_0xAB
	BRSH _0x212
;    2562                 				{
;    2563                 				        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,262
	CALL SUBOPT_0x2F
;    2564                 				}else{
	RJMP _0x213
_0x212:
;    2565                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xAC
	CALL SUBOPT_0x12
	CALL SUBOPT_0xA7
;    2566                 				}
_0x213:
;    2567         				        sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA8
	CALL SUBOPT_0xAD
;    2568 	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(5)
_0x4AB:
	ST   -Y,R30
	CALL _draw_label
;    2569                                         }
;    2570 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xAE
	BRNE _0x214
;    2571         				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xAF
	CALL SUBOPT_0xB0
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4AC
;    2572         				else                                     //Hz12
_0x214:
;    2573                 				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xAF
	CALL SUBOPT_0xB0
	SBIW R30,2
	ADIW R30,16
_0x4AC:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2574 	                        }
_0x209:
;    2575 			}else{
	RJMP _0x216
_0x201:
;    2576        			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
	CALL SUBOPT_0xB1
	__POINTW2MN _rprbdata,576
	CALL SUBOPT_0xA0
;    2577         			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2578         				(rundata.temperature[curr_dispch - 1] < -9000) ||\
;    2579 	        			(cptr[0] != 'P'))
	CALL SUBOPT_0xB1
	CPI  R30,LOW(0xFF)
	BREQ _0x218
	CALL SUBOPT_0xAA
	CALL SUBOPT_0x16
	CALL SUBOPT_0xAB
	BRLO _0x218
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BREQ _0x217
_0x218:
;    2580 	        		{
;    2581 	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
	CALL SUBOPT_0xB1
	CPI  R30,LOW(0xFF)
	BRNE _0x21A
;    2582                 				sprintf(strbuf," ------- ohm");
	CALL SUBOPT_0x0
	__POINTW1FN _0,283
	CALL SUBOPT_0x2F
;    2583                 			if(rundata.temperature[curr_dispch - 1] < -9000)
_0x21A:
	CALL SUBOPT_0xAA
	CALL SUBOPT_0x16
	CALL SUBOPT_0xAB
	BRSH _0x21B
;    2584                         			sprintf(strbuf," NO READ    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,296
	RJMP _0x4AD
;    2585                                         else
_0x21B:
;    2586                                         	sprintf(strbuf," -------    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,309
_0x4AD:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA9
;    2587                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xA2
	BREQ _0x21E
	CPI  R21,1
	BRNE _0x21D
_0x21E:
;    2588                                         {
;    2589         	        			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0x32
;    2590                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA1
;    2591 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(1)
	RJMP _0x4AE
;    2592 		                	}else{
_0x21D:
;    2593         	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA5
;    2594                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA1
;    2595 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(5)
_0x4AE:
	ST   -Y,R30
	CALL _draw_label
;    2596 		                	}
;    2597 			        }else{
	RJMP _0x221
_0x217:
;    2598 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA6
	CALL SUBOPT_0xA7
;    2599 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xA2
	BRNE _0x222
;    2600         					sprintf(strbuf+8,"*ohm");
	CALL SUBOPT_0xA8
	__POINTW1FN _0,322
	RJMP _0x4AF
;    2601                                         else
_0x222:
;    2602         					sprintf(strbuf+8," ohm");
	CALL SUBOPT_0xA8
	__POINTW1FN _0,291
_0x4AF:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA9
;    2603                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xA2
	BREQ _0x225
	CPI  R21,1
	BRNE _0x224
_0x225:
;    2604                                         {
;    2605 	        			        draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0x32
;    2606         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xAC
	CALL SUBOPT_0x12
	CALL SUBOPT_0xA7
;    2607 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA8
	CALL SUBOPT_0xAD
;    2608 		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(1)
	RJMP _0x4B0
;    2609 		        		 }else{
_0x224:
;    2610 	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA5
;    2611         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xAC
	CALL SUBOPT_0x12
	CALL SUBOPT_0xA7
;    2612 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA8
	CALL SUBOPT_0xAD
;    2613 		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xA4
	LDI  R30,LOW(5)
_0x4B0:
	ST   -Y,R30
	CALL _draw_label
;    2614 		        		 }
;    2615 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xAE
	BRNE _0x228
;    2616         				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xAF
	CALL SUBOPT_0xB0
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4B1
;    2617         				else                                     //Hz12
_0x228:
;    2618                 				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xAF
	CALL SUBOPT_0xB0
	SBIW R30,2
	ADIW R30,16
_0x4B1:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2619         			}
_0x221:
;    2620 	                }
_0x216:
;    2621 			INC_DISPCH;
	CALL SUBOPT_0x9C
	BRGE _0x22A
	CALL SUBOPT_0x38
_0x22A:
;    2622 		}
	SUBI R19,-1
	RJMP _0x1FF
_0x200:
;    2623 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2624 	}
;    2625 }
_0x1FD:
_0x48E:
	CALL __LOADLOCR6
	ADIW R28,9
	RET
;    2626 
;    2627 //main configuration window of R
;    2628 LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
;    2629 LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
;    2630 LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
;    2631 LABEL flash pgr_options[] = {
;    2632 	{LBL_HZ16, 10,23,6,"1.电流换向"},
;    2633 	{LBL_HZ16,130,23,7,"2.内标准校准"},
;    2634 	{LBL_HZ16, 10,45,8,"3.通道探头选择"},
;    2635 	{LBL_HZ16,130,45,7,"4.设置探头参数"}
;    2636 };
;    2637 
;    2638 //LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2639 #define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
;    2640 void pgcalibrate()
;    2641 {
_pgcalibrate:
;    2642         double oldvalue = 0;
;    2643         window_setup(10);
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x231*2)
	LDI  R31,HIGH(_0x231*2)
	CALL __INITLOCB
;	oldvalue -> Y+0
	CALL SUBOPT_0x9B
;    2644 	sprintf(strbuf,"请输入外标准(CH1)阻值");
	__POINTW1FN _0,412
	CALL SUBOPT_0x2F
;    2645 	oldvalue = wnd_floatinput(oldvalue);
	CALL SUBOPT_0x7
	CALL SUBOPT_0x9A
	__PUTD1S 0
;    2646 	if(oldvalue == 0)
	CALL SUBOPT_0x7
	CALL __CPD10
	BRNE _0x232
;    2647                 return;
	RJMP _0x48D
;    2648         wnd_msgbox(&pgr_calibrate);
_0x232:
	LDI  R30,LOW(_pgr_calibrate*2)
	LDI  R31,HIGH(_pgr_calibrate*2)
	CALL SUBOPT_0x36
;    2649         sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x6
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x29
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	MOVW R30,R10
	SBIW R30,1
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	CALL SUBOPT_0x16
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2B
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __ADDF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    2650 }
_0x48D:
	ADIW R28,4
	RET
;    2651 //main menu of bore config
;    2652 void pgrconfig_handler(uchar msg) {
_pgrconfig_handler:
;    2653 	uchar i;
;    2654 
;    2655 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4B:
	.BYTE 0x1

	.CSEG
;    2656 	static uchar last_sel = 1;

	.DSEG
_last_sel_S4B:
	.BYTE 0x1

	.CSEG
;    2657 	uchar min_option = 1;
;    2658 	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);
;    2659 
;    2660 	if(msg == KEY_TAB) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BRNE _0x235
;    2661 	        SET_BORE_MODE;
	CALL SUBOPT_0x37
;    2662 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2663 		return;
	RJMP _0x48C
;    2664 	}
;    2665 	if(msg == KEY_CE)
_0x235:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x236
;    2666 	{
;    2667 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2668 	        return;
	RJMP _0x48C
;    2669 	}
;    2670 	if(msg == MSG_INIT) {
_0x236:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x237
;    2671 		LCD_Cls();
	CALL _LCD_Cls
;    2672 		draw_label(&pgr_banner, SW_NORMAL);
	LDI  R30,LOW(_pgr_banner*2)
	LDI  R31,HIGH(_pgr_banner*2)
	CALL SUBOPT_0x32
;    2673 		INIT_OPTIONS(pgr_);
	MOV  R17,R16
_0x239:
	CP   R19,R17
	BRLO _0x23A
	CALL SUBOPT_0xB2
	CALL SUBOPT_0x32
	SUBI R17,-1
	RJMP _0x239
_0x23A:
;    2674 		sprintf(strbuf,"(%i)",sysdata.ktime);
	CALL SUBOPT_0x0
	__POINTW1FN _0,434
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	CALL SUBOPT_0xB3
;    2675 		draw_label(&pgr_klbl, SW_NORMAL);
	LDI  R30,LOW(_pgr_klbl*2)
	LDI  R31,HIGH(_pgr_klbl*2)
	CALL SUBOPT_0x32
;    2676 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2677 	}
;    2678 	if(msg == KEY_NUM1) {
_0x237:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x23B
;    2679 		window_setup(4); //4 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xB4
;    2680 		sprintf(strbuf,"电流换向时间设置");
	__POINTW1FN _0,439
	CALL SUBOPT_0x2F
;    2681 		sysdata.ktime = wnd_intinput(sysdata.ktime);
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDB
	ST   -Y,R30
	RCALL _wnd_intinput
	__POINTW2MN _sysdata,12
	LDI  R31,0
	CALL __EEPROMWRW
;    2682 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2683 		return;
	RJMP _0x48C
;    2684 	}
;    2685 	if(msg == KEY_NUM2)
_0x23B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x23C
;    2686 	{
;    2687                 pgcalibrate();
	CALL _pgcalibrate
;    2688 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2689 		return;
	RJMP _0x48C
;    2690 	}
;    2691 	if(msg == KEY_NUM3) {
_0x23C:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x23D
;    2692 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2693 		return;
	RJMP _0x48C
;    2694 	}
;    2695 	if(msg == KEY_NUM4) {
_0x23D:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x23E
;    2696 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2697 		return;
	RJMP _0x48C
;    2698 	}
;    2699 	if(msg == MSG_REFRESH) {
_0x23E:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x23F
;    2700 		REFRESH_OPTIONS(pgr_);
	LDS  R30,_curr_sel_S4B
	LDS  R26,_last_sel_S4B
	CP   R30,R26
	BRNE _0x240
	RJMP _0x48C
_0x240:
	MOV  R17,R16
_0x242:
	CP   R19,R17
	BRLO _0x243
	LDS  R26,_last_sel_S4B
	CP   R17,R26
	BRNE _0x244
	CALL SUBOPT_0xB2
	CALL SUBOPT_0xB5
_0x244:
	LDS  R26,_curr_sel_S4B
	CP   R17,R26
	BRNE _0x245
	CALL SUBOPT_0xB2
	CALL SUBOPT_0xB5
_0x245:
	SUBI R17,-1
	RJMP _0x242
_0x243:
;    2701 	}
;    2702 }
_0x23F:
	RJMP _0x48C
;    2703 //main configuration window of T
;    2704 LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
;    2705 LABEL flash pgt_options[] = {
;    2706 	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
;    2707 	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
;    2708 };
;    2709 
;    2710 //main menu of thermo config
;    2711 void pgtconfig_handler(uchar msg) {
_pgtconfig_handler:
;    2712 	uchar i;
;    2713 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2714 	static uchar last_sel = 1;

	.DSEG
_last_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2715 	uchar min_option = 1;
;    2716 	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
;    2717 	if(msg == KEY_TAB) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,2
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BRNE _0x24B
;    2718 	        SET_THERM_MODE;
	CALL SUBOPT_0x3A
;    2719 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2720 		return;
	RJMP _0x48C
;    2721 	}
;    2722 	if(msg == KEY_CE)
_0x24B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x24C
;    2723 	{
;    2724 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2725 	        return;
	RJMP _0x48C
;    2726 	}
;    2727 	if(msg == MSG_INIT) {
_0x24C:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x24D
;    2728 		LCD_Cls();
	CALL _LCD_Cls
;    2729 		draw_label(&pgt_banner, SW_NORMAL);
	LDI  R30,LOW(_pgt_banner*2)
	LDI  R31,HIGH(_pgt_banner*2)
	CALL SUBOPT_0x32
;    2730 		INIT_OPTIONS(pgt_);
	MOV  R17,R16
_0x24F:
	CP   R19,R17
	BRLO _0x250
	CALL SUBOPT_0xB6
	CALL SUBOPT_0x32
	SUBI R17,-1
	RJMP _0x24F
_0x250:
;    2731 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2732 	}
;    2733 	if(msg == KEY_NUM1) {
_0x24D:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x251
;    2734 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2735 		return;
	RJMP _0x48C
;    2736 	}
;    2737 	if(msg == KEY_NUM2) {
_0x251:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x252
;    2738 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2739 		return;
	RJMP _0x48C
;    2740 	}
;    2741 	if(msg == MSG_REFRESH) {
_0x252:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x253
;    2742 		REFRESH_OPTIONS(pgt_);
	LDS  R30,_curr_sel_S4C
	LDS  R26,_last_sel_S4C
	CP   R30,R26
	BRNE _0x254
	RJMP _0x48C
_0x254:
	MOV  R17,R16
_0x256:
	CP   R19,R17
	BRLO _0x257
	LDS  R26,_last_sel_S4C
	CP   R17,R26
	BRNE _0x258
	CALL SUBOPT_0xB6
	CALL SUBOPT_0xB5
_0x258:
	LDS  R26,_curr_sel_S4C
	CP   R17,R26
	BRNE _0x259
	CALL SUBOPT_0xB6
	CALL SUBOPT_0xB5
_0x259:
	SUBI R17,-1
	RJMP _0x256
_0x257:
;    2743 	}
;    2744 	return;
_0x253:
_0x48C:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2745 }
;    2746 //channel probe setup
;    2747 LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
;    2748 LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
;    2749 LABEL flash pgch_items[] = {
;    2750 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2751 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2752 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2753 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2754 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2755 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2756 };
;    2757 
;    2758 //select the channel to config
;    2759 void pgchset_handler(uchar msg) {
_pgchset_handler:
;    2760 	static uint curr_pos = 1; //absolute postion in options 1-24

	.DSEG
_curr_pos_S4D:
	.BYTE 0x2

	.CSEG
;    2761 	static uint last_pos = 1;

	.DSEG
_last_pos_S4D:
	.BYTE 0x2

	.CSEG
;    2762 	uchar min_option = 1;
;    2763 	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
;    2764 	uchar min_index = 1;
;    2765 	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
;    2766 	uchar new_page = 0;
;    2767 	int i,j;
;    2768 	if(msg == KEY_CE) {
	SBIW R28,4
	CALL SUBOPT_0xB7
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
	CPI  R26,LOW(0x43)
	BRNE _0x25D
;    2769 		if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x25E
;    2770 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	RJMP _0x4B2
;    2771 		else
_0x25E:
;    2772 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
_0x4B2:
	MOV  R4,R30
;    2773 		return;
	RJMP _0x48B
;    2774 	}
;    2775 
;    2776 	if(msg == KEY_UP) {
_0x25D:
	LDD  R26,Y+10
	CPI  R26,LOW(0x55)
	BRNE _0x260
;    2777 		DEC_TABLE;
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,1
	CALL SUBOPT_0xBA
	MOV  R30,R19
	CALL SUBOPT_0xBB
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x261
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xBA
_0x261:
	CALL SUBOPT_0xBD
	BRNE _0x262
	RJMP _0x48B
_0x262:
	MOV  R30,R16
	CALL SUBOPT_0xBB
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x263
	LDI  R21,LOW(1)
	RJMP _0x264
_0x263:
	LDI  R21,LOW(0)
_0x264:
;    2778 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2779 	}
;    2780 	if(msg == KEY_DN) {
_0x260:
	LDD  R26,Y+10
	CPI  R26,LOW(0x44)
	BRNE _0x265
;    2781 		INC_TABLE;
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	ADIW R30,1
	CALL SUBOPT_0xBA
	MOV  R30,R18
	CALL SUBOPT_0xBE
	CALL SUBOPT_0x9E
	BRSH _0x266
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xBA
_0x266:
	CALL SUBOPT_0xBD
	BRNE _0x267
	RJMP _0x48B
_0x267:
	MOV  R30,R16
	CALL SUBOPT_0xBB
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x268
	LDI  R21,LOW(1)
	RJMP _0x269
_0x268:
	LDI  R21,LOW(0)
_0x269:
;    2782 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2783 	}
;    2784 	if(msg == KEY_OK) {	//show window to select the probe
_0x265:
	LDD  R26,Y+10
	CPI  R26,LOW(0x4F)
	BRNE _0x26A
;    2785 		curr_ch = curr_pos;
	__GETWRMN 6,7,0,_curr_pos_S4D
;    2786 		nextwin = PG_PRBLIST;
	LDI  R30,LOW(8)
	MOV  R4,R30
;    2787 		return;
	RJMP _0x48B
;    2788 	}
;    2789 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x26A:
	LDD  R26,Y+10
	CPI  R26,LOW(0x30)
	BRLO _0x26C
	CPI  R26,LOW(0x3A)
	BRLO _0x26D
_0x26C:
_0x26D:
;    2790 	/*
;    2791 		KEY_TABLE;
;    2792 		msg = MSG_REFRESH;
;    2793 	*/
;    2794 	}
;    2795 	if(msg == KEY_DOT)
	LDD  R26,Y+10
	CPI  R26,LOW(0x2E)
	BRNE _0x26E
;    2796 	{
;    2797 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x26F
;    2798 	        {
;    2799 		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4B3
;    2800 	        }else{
_0x26F:
;    2801        		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4B3:
	LDS  R30,_curr_pos_S4D
	LDS  R31,_curr_pos_S4D+1
	CALL SUBOPT_0xC0
;    2802 	        }
;    2803 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2804 		new_page = 1; //refresh the whole page
	LDI  R21,LOW(1)
;    2805 	}
;    2806 	if(msg == MSG_INIT) {
_0x26E:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFF)
	BRNE _0x271
;    2807 		LCD_Cls();
	CALL _LCD_Cls
;    2808 		draw_label(&pgch_banner, SW_NORMAL);
	LDI  R30,LOW(_pgch_banner*2)
	LDI  R31,HIGH(_pgch_banner*2)
	CALL SUBOPT_0x32
;    2809 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xBA
;    2810 		new_page = 1;
	LDI  R21,LOW(1)
;    2811 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2812 	}
;    2813 	if(msg == MSG_REFRESH) {
_0x271:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x272
;    2814 
;    2815 		REFRESH_TABLES1(pgch_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC1
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xA7
	CPI  R21,1
	BREQ PC+3
	JMP _0x273
	CALL SUBOPT_0xC3
_0x275:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0x9E
	BRGE PC+3
	JMP _0x276
	CALL SUBOPT_0xC4
	CALL SUBOPT_0xC5
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADD  R30,R26
	ADC  R31,R27
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2816 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x277
;    2817         	        {
;    2818 				if(sysdata.tid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xC6
	BRNE _0x278
;    2819 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC7
;    2820 				}else{
	RJMP _0x279
_0x278:
;    2821 			        	//draw line item j
;    2822 				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC8
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xC9
	CALL _tname2b
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xCA
;    2823 				}
_0x279:
;    2824 		        }else{
	RJMP _0x27A
_0x277:
;    2825 				if(sysdata.rid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xC6
	BRNE _0x27B
;    2826 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC7
;    2827 				}else{
	RJMP _0x27C
_0x27B:
;    2828 			        	//draw line item j
;    2829 				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC8
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xCA
;    2830 				}
_0x27C:
;    2831 		        }
_0x27A:
;    2832 		REFRESH_TABLES2(pgch_);
	CALL SUBOPT_0xCC
	CALL SUBOPT_0x32
	CALL SUBOPT_0xC4
	CALL SUBOPT_0xCD
	BRNE _0x27D
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xB5
_0x27D:
	CALL SUBOPT_0xCE
	RJMP _0x275
_0x276:
	RJMP _0x27E
_0x273:
	CALL SUBOPT_0xC3
_0x280:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0x9E
	BRLT _0x281
	LDS  R26,_last_pos_S4D
	LDS  R27,_last_pos_S4D+1
	CALL SUBOPT_0xCF
	CALL SUBOPT_0xCD
	BRNE _0x282
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xB5
_0x282:
	CALL SUBOPT_0xC4
	CALL SUBOPT_0xCD
	BRNE _0x283
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xB5
_0x283:
	CALL SUBOPT_0xCE
	RJMP _0x280
_0x281:
_0x27E:
;    2833 	}
;    2834 	return;
_0x272:
_0x48B:
	CALL __LOADLOCR6
	ADIW R28,11
	RET
;    2835 }
;    2836 //select probe to edit.
;    2837 LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
;    2838 LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2839 LABEL flash pgprbset_items[] = {
;    2840 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2841 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2842 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2843 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2844 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2845 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2846 };
;    2847 //select probe to config
;    2848 void pgprbset_handler(uchar msg) {
_pgprbset_handler:
;    2849 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2850 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2851 	static uint last_pos = 1;

	.DSEG
_last_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2852 	uchar min_option = 1;
;    2853 	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
;    2854 	uchar min_index = 1;
;    2855 	uchar max_index = MAX_PROBE_NUM;
;    2856 	int i,j;
;    2857 	uchar new_page = 1;
;    2858 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,3
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x287*2)
	LDI  R31,HIGH(_0x287*2)
	CALL __INITLOCB
	CALL SUBOPT_0xB7
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
	BRLO _0x289
	CPI  R26,LOW(0x3A)
	BRLO _0x28A
_0x289:
_0x28A:
;    2859 	/*
;    2860 		KEY_TABLE;
;    2861 		msg = MSG_REFRESH;
;    2862 	*/
;    2863 	}
;    2864 
;    2865 	if(msg == KEY_CE)
	LDD  R26,Y+9
	CPI  R26,LOW(0x43)
	BRNE _0x28B
;    2866 	{
;    2867 		if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x28C
;    2868 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4B4
;    2869 		else
_0x28C:
;    2870 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4B4:
	MOV  R4,R30
;    2871 		return;
	RJMP _0x48A
;    2872 	}
;    2873 	if(msg == KEY_OK)
_0x28B:
	LDD  R26,Y+9
	CPI  R26,LOW(0x4F)
	BRNE _0x28E
;    2874 	{
;    2875 		curr_prb = curr_pos;
	__GETWRMN 8,9,0,_curr_pos_S4E
;    2876 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2877 		return;
	RJMP _0x48A
;    2878 	}
;    2879 	if(msg == MSG_INIT)
_0x28E:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFF)
	BRNE _0x28F
;    2880 	{
;    2881 		LCD_Cls();
	CALL _LCD_Cls
;    2882 		draw_label(&pgprbset_banner, SW_NORMAL);
	LDI  R30,LOW(_pgprbset_banner*2)
	LDI  R31,HIGH(_pgprbset_banner*2)
	CALL SUBOPT_0x32
;    2883 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xD0
;    2884 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2885 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2886 	}
;    2887 	if(msg == KEY_UP) {
_0x28F:
	LDD  R26,Y+9
	CPI  R26,LOW(0x55)
	BRNE _0x290
;    2888 		DEC_TABLE;
	CALL SUBOPT_0xD1
	CALL SUBOPT_0xD2
	SBIW R30,1
	CALL SUBOPT_0xD0
	MOV  R30,R19
	CALL SUBOPT_0xD3
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x291
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xD0
_0x291:
	CALL SUBOPT_0xD4
	BRNE _0x292
	RJMP _0x48A
_0x292:
	MOV  R30,R16
	CALL SUBOPT_0xD3
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x293
	LDI  R30,LOW(1)
	RJMP _0x4B5
_0x293:
	LDI  R30,LOW(0)
_0x4B5:
	STD  Y+6,R30
;    2889 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2890 	}
;    2891 	if(msg == KEY_DN) {
_0x290:
	LDD  R26,Y+9
	CPI  R26,LOW(0x44)
	BRNE _0x295
;    2892 		INC_TABLE;
	CALL SUBOPT_0xD1
	CALL SUBOPT_0xD2
	ADIW R30,1
	CALL SUBOPT_0xD0
	MOV  R30,R18
	CALL SUBOPT_0xD5
	CALL SUBOPT_0x9E
	BRSH _0x296
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xD0
_0x296:
	CALL SUBOPT_0xD4
	BRNE _0x297
	RJMP _0x48A
_0x297:
	MOV  R30,R16
	CALL SUBOPT_0xD3
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x298
	LDI  R30,LOW(1)
	RJMP _0x4B6
_0x298:
	LDI  R30,LOW(0)
_0x4B6:
	STD  Y+6,R30
;    2893 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2894 	}
;    2895 	if(msg == MSG_REFRESH) {
_0x295:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x29A
;    2896 		REFRESH_TABLES1(pgprbset_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC1
	CALL SUBOPT_0xD1
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xA7
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x29B
	MOV  R20,R17
	CLR  R21
_0x29D:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0x9E
	BRLT _0x29E
	CALL SUBOPT_0xD6
	CALL SUBOPT_0xC5
	ADD  R30,R20
	ADC  R31,R21
	STD  Y+7,R30
	STD  Y+7+1,R31
;    2897 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x29F
;    2898 		        {
;    2899 			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xD7
	CALL _tname2b
	RJMP _0x4B7
;    2900 		        }else{
_0x29F:
;    2901         		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xD7
	CALL _rname2b
_0x4B7:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	CALL SUBOPT_0xCA
;    2902 		        }
;    2903         		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xD8
;    2904 		REFRESH_TABLES2(pgprbset_);
	CALL SUBOPT_0xD8
	CALL SUBOPT_0xD6
	CALL SUBOPT_0xD9
	BRNE _0x2A1
	CALL SUBOPT_0xDA
_0x2A1:
	__ADDWRN 20,21,1
	RJMP _0x29D
_0x29E:
	RJMP _0x2A2
_0x29B:
	MOV  R20,R17
	CLR  R21
_0x2A4:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0x9E
	BRLT _0x2A5
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CALL SUBOPT_0xCF
	CALL SUBOPT_0xD9
	BRNE _0x2A6
	CALL SUBOPT_0xDA
_0x2A6:
	CALL SUBOPT_0xD6
	CALL SUBOPT_0xD9
	BRNE _0x2A7
	CALL SUBOPT_0xDA
_0x2A7:
	__ADDWRN 20,21,1
	RJMP _0x2A4
_0x2A5:
_0x2A2:
;    2905 	}
;    2906 	return;
_0x29A:
_0x48A:
	CALL __LOADLOCR6
	ADIW R28,10
	RET
;    2907 }
;    2908 //select probe for channel setup
;    2909 LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2910 LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2911 LABEL flash prblist_items[] = {
;    2912 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2913 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2914 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2915 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2916 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2917 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2918 };
;    2919 //select channel's probe
;    2920 void pgprblist_handler(uchar msg) {
_pgprblist_handler:
;    2921 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2922 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2923 	static uint last_pos = 1;

	.DSEG
_last_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2924 	uchar min_option = 1;
;    2925 	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
;    2926 	uchar min_index = 1;
;    2927 	uchar max_index = MAX_PROBE_NUM;
;    2928 	uchar i,j;
;    2929 	uchar new_page = 1;
;    2930 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,1
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x2AA*2)
	LDI  R31,HIGH(_0x2AA*2)
	CALL __INITLOCB
	CALL SUBOPT_0xB7
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
	BRLO _0x2AC
	CPI  R26,LOW(0x3A)
	BRLO _0x2AD
_0x2AC:
_0x2AD:
;    2931 	/*
;    2932 		KEY_TABLE;
;    2933 		msg = MSG_REFRESH;
;    2934 	*/
;    2935 	}
;    2936 	if(msg == KEY_CE)
	LDD  R26,Y+7
	CPI  R26,LOW(0x43)
	BRNE _0x2AE
;    2937 	{
;    2938 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2939 		return;
	RJMP _0x489
;    2940 	}
;    2941 	if(msg == KEY_OK)
_0x2AE:
	LDD  R26,Y+7
	CPI  R26,LOW(0x4F)
	BRNE _0x2AF
;    2942 	{
;    2943 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x2B0
;    2944 	        {
;    2945        	                if(tprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xDC
	BREQ _0x2B1
;    2946         		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xDD
	CALL __EEPROMWRB
;    2947 	        }else{
_0x2B1:
	RJMP _0x2B2
_0x2B0:
;    2948                         if(rprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xDC
	BREQ _0x2B3
;    2949 		                sysdata.rid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xDD
	CALL __EEPROMWRB
;    2950 	        }
_0x2B3:
_0x2B2:
;    2951 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2952 		return;
	RJMP _0x489
;    2953 	}
;    2954 	if(msg == MSG_INIT)
_0x2AF:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFF)
	BRNE _0x2B4
;    2955 	{
;    2956 		LCD_Cls();
	CALL _LCD_Cls
;    2957 		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
	CALL SUBOPT_0x0
	__POINTW1FN _0,565
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL SUBOPT_0xB3
;    2958 		draw_label(&prblist_banner, SW_NORMAL);
	LDI  R30,LOW(_prblist_banner*2)
	LDI  R31,HIGH(_prblist_banner*2)
	CALL SUBOPT_0x32
;    2959 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xDE
;    2960 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2961 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2962 	}
;    2963 	if(msg == KEY_UP) {
_0x2B4:
	LDD  R26,Y+7
	CPI  R26,LOW(0x55)
	BRNE _0x2B5
;    2964 		DEC_TABLE;
	CALL SUBOPT_0xDF
	STS  _last_pos_S4F,R30
	STS  _last_pos_S4F+1,R31
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xDE
	MOV  R30,R19
	CALL SUBOPT_0xE0
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x2B6
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xDE
_0x2B6:
	CALL SUBOPT_0xE1
	BRNE _0x2B7
	RJMP _0x489
_0x2B7:
	MOV  R30,R16
	CALL SUBOPT_0xE0
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x2B8
	LDI  R30,LOW(1)
	RJMP _0x4B8
_0x2B8:
	LDI  R30,LOW(0)
_0x4B8:
	STD  Y+6,R30
;    2965 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2966 	}
;    2967 	if(msg == KEY_DN) {
_0x2B5:
	LDD  R26,Y+7
	CPI  R26,LOW(0x44)
	BRNE _0x2BA
;    2968 		INC_TABLE;
	CALL SUBOPT_0xDF
	STS  _last_pos_S4F,R30
	STS  _last_pos_S4F+1,R31
	CALL SUBOPT_0xDF
	ADIW R30,1
	CALL SUBOPT_0xDE
	MOV  R30,R18
	CALL SUBOPT_0xE2
	CALL SUBOPT_0x9E
	BRSH _0x2BB
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xDE
_0x2BB:
	CALL SUBOPT_0xE1
	BRNE _0x2BC
	RJMP _0x489
_0x2BC:
	MOV  R30,R16
	CALL SUBOPT_0xE0
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x2BD
	LDI  R30,LOW(1)
	RJMP _0x4B9
_0x2BD:
	LDI  R30,LOW(0)
_0x4B9:
	STD  Y+6,R30
;    2969 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2970 	}
;    2971 	if(msg == KEY_DOT){
_0x2BA:
	LDD  R26,Y+7
	CPI  R26,LOW(0x2E)
	BRNE _0x2BF
;    2972 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x2C0
;    2973 	        {
;    2974 		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4BA
;    2975 	        }else{
_0x2C0:
;    2976 		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4BA:
	MOVW R30,R6
	CALL SUBOPT_0xC0
;    2977 	        }
;    2978        		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2979        		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2980 	}
;    2981 	if(msg == MSG_REFRESH) {
_0x2BF:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2C2
;    2982 		REFRESH_TABLES1(prblist_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC1
	CALL SUBOPT_0xDF
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xA7
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x2C3
	MOV  R21,R17
_0x2C5:
	CP   R16,R21
	BRLO _0x2C6
	CALL SUBOPT_0xE3
	CALL SUBOPT_0xC5
	MOVW R26,R30
	MOV  R30,R21
	LDI  R31,0
	ADD  R30,R26
	MOV  R20,R30
;    2983 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x2C7
;    2984 		        {
;    2985 		                sprintf(strbuf,"%s",tname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xE4
	CALL _tname2b
	RJMP _0x4BB
;    2986 		        }else{
_0x2C7:
;    2987         		        sprintf(strbuf,"%s",rname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xE4
	CALL _rname2b
_0x4BB:
	CLR  R22
	CLR  R23
	CALL SUBOPT_0xE5
;    2988         		}
;    2989 			draw_label(&(prblist_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xE6
;    2990 		REFRESH_TABLES2(prblist_);
	CALL SUBOPT_0xE6
	CALL SUBOPT_0xE3
	CALL SUBOPT_0xE7
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2C9
	CALL SUBOPT_0xE8
_0x2C9:
	SUBI R21,-1
	RJMP _0x2C5
_0x2C6:
	RJMP _0x2CA
_0x2C3:
	MOV  R21,R17
_0x2CC:
	CP   R16,R21
	BRLO _0x2CD
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CALL SUBOPT_0xCF
	CALL SUBOPT_0xE7
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2CE
	CALL SUBOPT_0xE8
_0x2CE:
	CALL SUBOPT_0xE3
	CALL SUBOPT_0xE7
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2CF
	CALL SUBOPT_0xE8
_0x2CF:
	SUBI R21,-1
	RJMP _0x2CC
_0x2CD:
_0x2CA:
;    2991 	}
;    2992 	return;
_0x2C2:
_0x489:
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;    2993 }
;    2994 //select probe type list
;    2995 LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2996 LABEL flash tplist_options[] = {
;    2997 	{LBL_HZ6X8,10,20,8,"1.PT100"},
;    2998 	{LBL_HZ6X8,80,20,8,"2.PT25"},
;    2999 	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
;    3000 	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
;    3001 	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
;    3002 	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
;    3003 	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
;    3004 	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
;    3005 	{LBL_HZ6X8,10,50,8,"9.R-TYPE"}
;    3006 };
;    3007 //select probe type thermo
;    3008 void pgprbtypelist_handler(uchar msg) {
_pgprbtypelist_handler:
;    3009 	static int curr_sel = 1;

	.DSEG
_curr_sel_S50:
	.BYTE 0x2

	.CSEG
;    3010 	static int last_sel = 1;

	.DSEG
_last_sel_S50:
	.BYTE 0x2

	.CSEG
;    3011 	uchar min_option = 1;
;    3012 	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
;    3013 	uchar i;
;    3014 	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	min_option -> R17
;	max_option -> R16
;	i -> R19
	LDI  R16,9
	LDI  R17,1
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRLO _0x2DC
	CPI  R26,LOW(0x3A)
	BRLO _0x2DD
_0x2DC:
	RJMP _0x2DB
_0x2DD:
;    3015 	        curr_sel = msg - KEY_NUM1 + 1;
	LDD  R30,Y+4
	SUBI R30,LOW(49)
	SUBI R30,-LOW(1)
	CALL SUBOPT_0xE9
;    3016 	        msg = KEY_OK;
	LDI  R30,LOW(79)
	STD  Y+4,R30
;    3017 	}
;    3018 
;    3019 	if(msg == KEY_CE ) {
_0x2DB:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x2DE
;    3020 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3021 		return;
	RJMP _0x488
;    3022 	}
;    3023 	if(msg == KEY_OK)
_0x2DE:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BREQ PC+3
	JMP _0x2DF
;    3024 	{
;    3025 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x2E0
;    3026 	        {
;    3027         		switch(curr_sel)
	CALL SUBOPT_0xEA
;    3028 	        	{
;    3029 		        	case 1:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x2E4
;    3030         			       return;
	RJMP _0x488
;    3031 	        		case 2:
_0x2E4:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x2E6
;    3032 			               return;
	RJMP _0x488
;    3033         			default:
_0x2E6:
;    3034 	        		       tprbdata.type[curr_prb-1] = curr_sel - 3+PRBTYPE_K;
	CALL SUBOPT_0xEB
	CALL SUBOPT_0xEA
	SBIW R30,3
	ADIW R30,3
	CALL __EEPROMWRB
;    3035 		        	       break;
;    3036         		}
;    3037         	}else{
	RJMP _0x2E7
_0x2E0:
;    3038         		switch(curr_sel)
	CALL SUBOPT_0xEA
;    3039 	        	{
;    3040 		        	case 1:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x2EB
;    3041 			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
	CALL SUBOPT_0xEC
	LDI  R30,LOW(241)
	CALL __EEPROMWRB
;    3042         			       break;
	RJMP _0x2EA
;    3043 	        		case 2:
_0x2EB:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x2ED
;    3044                        	               sprintf(strbuf,"输入Rtp:");
	CALL SUBOPT_0x0
	__POINTW1FN _0,666
	CALL SUBOPT_0x2F
;    3045         	                       rprbdata.rtp[curr_prb-1] = wnd_floatinput(rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0xED
	CALL SUBOPT_0x82
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xED
	CALL SUBOPT_0xEE
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3046 		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
	CALL SUBOPT_0xEC
	LDI  R30,LOW(242)
	CALL __EEPROMWRB
;    3047 			               break;
	RJMP _0x2EA
;    3048         			default:
_0x2ED:
;    3049                                        return;
	RJMP _0x488
;    3050         		}
_0x2EA:
;    3051         	}
_0x2E7:
;    3052 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3053 		return;
	RJMP _0x488
;    3054 	}
;    3055 	if(msg == MSG_INIT)
_0x2DF:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x2EE
;    3056 	{
;    3057 		LCD_Cls();
	CALL _LCD_Cls
;    3058 		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,675
	CALL SUBOPT_0xEF
;    3059 		draw_label(&tplist_banner, SW_NORMAL);
	LDI  R30,LOW(_tplist_banner*2)
	LDI  R31,HIGH(_tplist_banner*2)
	CALL SUBOPT_0x32
;    3060 		INIT_OPTIONS(tplist_);
	MOV  R19,R17
_0x2F0:
	CP   R16,R19
	BRLO _0x2F1
	CALL SUBOPT_0xF0
	CALL SUBOPT_0x32
	SUBI R19,-1
	RJMP _0x2F0
_0x2F1:
;    3061 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3062 	}
;    3063 	if(msg == KEY_UP) {
_0x2EE:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x2F2
;    3064 		DEC_OPTION;
	CALL SUBOPT_0xF1
	SBIW R30,1
	STS  _curr_sel_S50,R30
	STS  _curr_sel_S50+1,R31
	MOV  R30,R17
	CALL SUBOPT_0xF2
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	BRGE _0x2F3
	MOV  R30,R16
	CALL SUBOPT_0xE9
;    3065 		msg = MSG_REFRESH;
_0x2F3:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3066 	}
;    3067 	if(msg == KEY_DN) {
_0x2F2:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x2F4
;    3068 		INC_OPTION;
	CALL SUBOPT_0xF1
	ADIW R30,1
	STS  _curr_sel_S50,R30
	STS  _curr_sel_S50+1,R31
	MOV  R30,R16
	CALL SUBOPT_0xF2
	CALL SUBOPT_0x9E
	BRGE _0x2F5
	MOV  R30,R17
	CALL SUBOPT_0xE9
;    3069 		msg = MSG_REFRESH;
_0x2F5:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3070 	}
;    3071 	if(msg == MSG_REFRESH) {
_0x2F4:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x2F6
;    3072 		REFRESH_OPTIONS(tplist_);
	CALL SUBOPT_0xEA
	LDS  R26,_last_sel_S50
	LDS  R27,_last_sel_S50+1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2F7
	RJMP _0x488
_0x2F7:
	MOV  R19,R17
_0x2F9:
	CP   R16,R19
	BRLO _0x2FA
	MOV  R30,R19
	LDS  R26,_last_sel_S50
	LDS  R27,_last_sel_S50+1
	CALL SUBOPT_0x9E
	BRNE _0x2FB
	CALL SUBOPT_0xF0
	CALL SUBOPT_0xB5
_0x2FB:
	MOV  R30,R19
	CALL SUBOPT_0xF2
	CALL SUBOPT_0x9E
	BRNE _0x2FC
	CALL SUBOPT_0xF0
	CALL SUBOPT_0xB5
_0x2FC:
	SUBI R19,-1
	RJMP _0x2F9
_0x2FA:
;    3073 	}
;    3074 	return;
_0x2F6:
_0x488:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    3075 }
;    3076 /*
;    3077  *       Convert the probe type const to string
;    3078  */
;    3079 char PRBSTR[7];

	.DSEG
_PRBSTR:
	.BYTE 0x7
;    3080 char* getprbtype(uchar prbtype)
;    3081 {

	.CSEG
_getprbtype:
;    3082 	switch(prbtype)
;	prbtype -> Y+0
	LD   R30,Y
;    3083 	{
;    3084 		case PRBTYPE_PT25:
	CPI  R30,LOW(0xF2)
	BRNE _0x300
;    3085 		        sprintf(PRBSTR,"PT  25");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,693
	CALL SUBOPT_0x2F
;    3086 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3087 		case PRBTYPE_PT100:
_0x300:
	CPI  R30,LOW(0xF1)
	BRNE _0x301
;    3088 		        sprintf(PRBSTR,"PT 100");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,700
	CALL SUBOPT_0x2F
;    3089 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3090 		case PRBTYPE_K:
_0x301:
	CPI  R30,LOW(0x3)
	BRNE _0x302
;    3091 		        sprintf(PRBSTR,"K TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,707
	CALL SUBOPT_0x2F
;    3092 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3093 		case PRBTYPE_N:
_0x302:
	CPI  R30,LOW(0x4)
	BRNE _0x303
;    3094 		        sprintf(PRBSTR,"N TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,714
	CALL SUBOPT_0x2F
;    3095 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3096 		case PRBTYPE_E:
_0x303:
	CPI  R30,LOW(0x5)
	BRNE _0x304
;    3097 		        sprintf(PRBSTR,"E TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,721
	CALL SUBOPT_0x2F
;    3098 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3099 		case PRBTYPE_B:
_0x304:
	CPI  R30,LOW(0x6)
	BRNE _0x305
;    3100 		        sprintf(PRBSTR,"B TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,728
	CALL SUBOPT_0x2F
;    3101 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3102 		case PRBTYPE_J:
_0x305:
	CPI  R30,LOW(0x7)
	BRNE _0x306
;    3103 		        sprintf(PRBSTR,"J TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,735
	CALL SUBOPT_0x2F
;    3104 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3105 		case PRBTYPE_S:
_0x306:
	CPI  R30,LOW(0x8)
	BRNE _0x307
;    3106 		        sprintf(PRBSTR,"S TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,742
	CALL SUBOPT_0x2F
;    3107 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3108 		case PRBTYPE_R:
_0x307:
	CPI  R30,LOW(0x9)
	BRNE _0x2FF
;    3109 		        sprintf(PRBSTR,"R TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,749
	CALL SUBOPT_0x2F
;    3110 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3111 
;    3112 	}
_0x2FF:
;    3113 
;    3114 		        sprintf(PRBSTR,"? TYPE");
	CALL SUBOPT_0xF3
	__POINTW1FN _0,756
	CALL SUBOPT_0x2F
;    3115 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x487
;    3116 
;    3117 }
;    3118 //config r probe parameter
;    3119 LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
;    3120 LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
;    3121 LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
;    3122 LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
;    3123 LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
;    3124 LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
;    3125 LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
;    3126 LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
;    3127 LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
;    3128 LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
;    3129 LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
;    3130 LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
;    3131 LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};
;    3132 
;    3133 //configuration of BORE probe parameter
;    3134 void pgprbconfig_handler(uchar msg) {
_pgprbconfig_handler:
;    3135 	if( msg == KEY_CE) {
;	msg -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x43)
	BRNE _0x30F
;    3136 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    3137 		return;
	RJMP _0x487
;    3138 	}
;    3139 	if(msg == MSG_INIT)
_0x30F:
	LD   R26,Y
	CPI  R26,LOW(0xFF)
	BREQ PC+3
	JMP _0x310
;    3140 	{
;    3141 		LCD_Cls();
	CALL _LCD_Cls
;    3142 		if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x311
;    3143 		{
;    3144 		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,788
	CALL SUBOPT_0xEF
;    3145         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x32
;    3146 	        	//name
;    3147 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3148         		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xF5
	CALL _tname2b
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xA7
;    3149 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x32
;    3150         		//type
;    3151 	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xEB
	CALL SUBOPT_0xF6
	CALL SUBOPT_0xA7
;    3152 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	RJMP _0x4BC
;    3153 		}else{
_0x311:
;    3154 		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,805
	CALL SUBOPT_0xEF
;    3155         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x32
;    3156 	        	//name
;    3157 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3158 		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xEC
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x313
;    3159 		        {
;    3160         		        sprintf(strbuf,":%s",rname2b(curr_prb-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xF5
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xA7
;    3161         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	RJMP _0x4BD
;    3162         	        }else{
_0x313:
;    3163                		        sprintf(strbuf,":%s Rtp:%7f",rname2b(curr_prb-1),rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,822
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xF5
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xED
	CALL SUBOPT_0xEE
	CALL __PUTPARD1
	CALL SUBOPT_0xCA
;    3164         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
_0x4BD:
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x32
;    3165         	        }
;    3166                		//type
;    3167                        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xEC
	CALL SUBOPT_0xF6
	CALL SUBOPT_0xA7
;    3168 	                draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	CALL SUBOPT_0x32
;    3169 
;    3170 		        if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xEC
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x315
;    3171 		        {
;    3172 		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,834
	CALL SUBOPT_0xF7
	CALL SUBOPT_0xE5
;    3173         		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3b*2)
	LDI  R31,HIGH(_paramlbl3b*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_paramval3b*2)
	LDI  R31,HIGH(_paramval3b*2)
	RJMP _0x4BC
;    3174         		}else{
_0x315:
;    3175         		        //param1
;    3176                 		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,844
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xF8
	CALL SUBOPT_0xE5
;    3177 	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
	LDI  R30,LOW(_paramlbl1*2)
	LDI  R31,HIGH(_paramlbl1*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_paramval1*2)
	LDI  R31,HIGH(_paramval1*2)
	CALL SUBOPT_0x32
;    3178         	        	//param2
;    3179 	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,852
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xEE
	CALL SUBOPT_0xE5
;    3180                 		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	LDI  R30,LOW(_paramlbl2*2)
	LDI  R31,HIGH(_paramlbl2*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_paramval2*2)
	LDI  R31,HIGH(_paramval2*2)
	CALL SUBOPT_0x32
;    3181 	                	//param3
;    3182 		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,860
	CALL SUBOPT_0xF7
	CALL SUBOPT_0xE5
;    3183         		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3*2)
	LDI  R31,HIGH(_paramlbl3*2)
	CALL SUBOPT_0x32
	LDI  R30,LOW(_paramval3*2)
	LDI  R31,HIGH(_paramval3*2)
_0x4BC:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _draw_label
;    3184         		}
;    3185                 }
;    3186 		return;
	RJMP _0x487
;    3187 	}
;    3188 	if(msg == KEY_NUM1) {
_0x310:
	LD   R26,Y
	CPI  R26,LOW(0x31)
	BRNE _0x317
;    3189 		window_setup(7);
	LDI  R30,LOW(7)
	CALL SUBOPT_0xB4
;    3190 		sprintf(strbuf,"请输入探头序列号");
	__POINTW1FN _0,868
	CALL SUBOPT_0x2F
;    3191 		prbsninput();
	RCALL _prbsninput
;    3192 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3193 		return;
	RJMP _0x487
;    3194 	}
;    3195 	if(msg == KEY_NUM2){
_0x317:
	LD   R26,Y
	CPI  R26,LOW(0x32)
	BRNE _0x318
;    3196 		nextwin = PG_PRBTYPELIST;
	LDI  R30,LOW(9)
	MOV  R4,R30
;    3197 		return;
	RJMP _0x487
;    3198 	}
;    3199 	if(msg == KEY_NUM3 ){
_0x318:
	LD   R26,Y
	CPI  R26,LOW(0x33)
	BRNE _0x319
;    3200 
;    3201 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x31A
;    3202 	        {
;    3203                		window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    3204 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xEC
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x31B
;    3205 	                {
;    3206         	                sprintf(strbuf,"输入R(0!)");
	CALL SUBOPT_0x0
	__POINTW1FN _0,885
	CALL SUBOPT_0x2F
;    3207         	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0xFA
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF8
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
	RJMP _0x4BE
;    3208                         }else{
_0x31B:
;    3209         	        	sprintf(strbuf,"输入系数a");
	CALL SUBOPT_0x0
	__POINTW1FN _0,895
	CALL SUBOPT_0x2F
;    3210         	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL SUBOPT_0x82
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF8
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
_0x4BE:
	CALL __EEPROMWRD
;    3211         	        }
;    3212         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3213         	}
;    3214 		return;
_0x31A:
	RJMP _0x487
;    3215 	}
;    3216 	if(msg == KEY_NUM4) {
_0x319:
	LD   R26,Y
	CPI  R26,LOW(0x34)
	BRNE _0x31D
;    3217         	if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x31E
;    3218         	{
;    3219 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xEC
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x31F
;    3220 	                        return;
	RJMP _0x487
;    3221 
;    3222         		window_setup(10);
_0x31F:
	CALL SUBOPT_0x9B
;    3223 	        	sprintf(strbuf,"输入系数b");
	__POINTW1FN _0,905
	CALL SUBOPT_0x2F
;    3224 		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x82
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xEE
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3225         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3226                 }
;    3227 		return;
_0x31E:
	RJMP _0x487
;    3228 	}
;    3229 	if(msg == KEY_NUM5) {
_0x31D:
	LD   R26,Y
	CPI  R26,LOW(0x35)
	BRNE _0x320
;    3230 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x30
	BRNE _0x321
;    3231 	        {
;    3232        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xEC
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x322
;    3233        	                        return;
	RJMP _0x487
;    3234 	                window_setup(10);
_0x322:
	CALL SUBOPT_0x9B
;    3235        		        sprintf(strbuf,"输入系数c");
	__POINTW1FN _0,915
	CALL SUBOPT_0x2F
;    3236         		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0xFA
	PUSH R31
	PUSH R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	CALL SUBOPT_0xEE
	CALL SUBOPT_0x9A
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3237 		        nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3238 	        }
;    3239 		return;
_0x321:
;    3240 	}
;    3241 	return;
_0x320:
_0x487:
	ADIW R28,1
	RET
;    3242 }
;    3243 
;    3244 LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
;    3245 LABEL flash boot_options[] = {
;    3246  	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
;    3247  	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
;    3248  	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
;    3249  	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
;    3250 };
;    3251 //boot up menu
;    3252 extern u8 eeprom scanner_type;
;    3253 void pgboottype_handler(uchar msg) {
_pgboottype_handler:
;    3254 	uchar i;
;    3255 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S53:
	.BYTE 0x1

	.CSEG
;    3256 	static uchar last_sel = 1;

	.DSEG
_last_sel_S53:
	.BYTE 0x1

	.CSEG
;    3257 	uchar min_option = 1;
;    3258 	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
;    3259 	if(msg == KEY_NUM5) { //select scanner type
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x35)
	BRNE _0x32A
;    3260        		window_setup(4); //2 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xB4
;    3261 		sprintf(strbuf,"Scanner(1:MI,2:GUIDLINE)");
	__POINTW1FN _0,992
	CALL SUBOPT_0x2F
;    3262 		i = scanner_type;
	CALL SUBOPT_0x90
	MOV  R17,R30
;    3263 		i = wnd_intinput(i);
	ST   -Y,R17
	RCALL _wnd_intinput
	MOV  R17,R30
;    3264 		if(i == 1 || i == 2)
	CPI  R17,1
	BREQ _0x32C
	CPI  R17,2
	BRNE _0x32B
_0x32C:
;    3265 		        scanner_type = i;
	MOV  R30,R17
	RJMP _0x4BF
;    3266 		else
_0x32B:
;    3267 		        scanner_type = 1;
	LDI  R30,LOW(1)
_0x4BF:
	LDI  R26,LOW(_scanner_type)
	LDI  R27,HIGH(_scanner_type)
	CALL __EEPROMWRB
;    3268 	        msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+4,R30
;    3269 	}
;    3270 
;    3271 	if(msg == MSG_INIT) {
_0x32A:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x32F
;    3272 		LCD_Cls();
	CALL _LCD_Cls
;    3273 		draw_label(&boot_banner, SW_NORMAL);
	LDI  R30,LOW(_boot_banner*2)
	LDI  R31,HIGH(_boot_banner*2)
	CALL SUBOPT_0x32
;    3274 		INIT_OPTIONS(boot_);
	MOV  R17,R16
_0x331:
	CP   R19,R17
	BRLO _0x332
	CALL SUBOPT_0xFB
	CALL SUBOPT_0x32
	SUBI R17,-1
	RJMP _0x331
_0x332:
;    3275 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3276 	}
;    3277 	if(msg == KEY_NUM1) {
_0x32F:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x333
;    3278 		SET_BORE_MODE;
	CALL SUBOPT_0x37
;    3279 		SET_TORX;
	CALL SUBOPT_0x21
;    3280 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3281 		return;
	CALL __LOADLOCR4
	RJMP _0x485
;    3282 	}
;    3283 	if(msg == KEY_NUM2) {
_0x333:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x334
;    3284 		SET_BORE_MODE;
	CALL SUBOPT_0x37
;    3285 		SET_TORX;
	CALL SUBOPT_0x21
;    3286 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    3287 		return;
	CALL __LOADLOCR4
	RJMP _0x485
;    3288 	}
;    3289 	if(msg == KEY_NUM3) {
_0x334:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x335
;    3290 		SET_THERM_MODE;
	CALL SUBOPT_0x3A
;    3291 		SET_TORS;
	CALL SUBOPT_0x1B
;    3292 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3293 		return;
	CALL __LOADLOCR4
	RJMP _0x485
;    3294 	}
;    3295 	if(msg == KEY_NUM4) {
_0x335:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x336
;    3296 		SET_THERM_MODE;
	CALL SUBOPT_0x3A
;    3297 		SET_TORS;
	CALL SUBOPT_0x1B
;    3298 		nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	MOV  R4,R30
;    3299 		return;
	CALL __LOADLOCR4
	RJMP _0x485
;    3300 	}
;    3301 	if(msg == MSG_REFRESH) {
_0x336:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x337
;    3302 		REFRESH_OPTIONS(boot_);
	LDS  R30,_curr_sel_S53
	LDS  R26,_last_sel_S53
	CP   R30,R26
	BRNE _0x338
	CALL __LOADLOCR4
	RJMP _0x485
_0x338:
	MOV  R17,R16
_0x33A:
	CP   R19,R17
	BRLO _0x33B
	LDS  R26,_last_sel_S53
	CP   R17,R26
	BRNE _0x33C
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB5
_0x33C:
	LDS  R26,_curr_sel_S53
	CP   R17,R26
	BRNE _0x33D
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB5
_0x33D:
	SUBI R17,-1
	RJMP _0x33A
_0x33B:
;    3303 	}
;    3304 }
_0x337:
	CALL __LOADLOCR4
	RJMP _0x485
;    3305 #include "utili.h"
;    3306 #include "window.h"
;    3307 #include "stdlib.h"
;    3308 /*---------------延时子程序----------------*/
;    3309 /*
;    3310 void delay1 (uint ms)
;    3311 {
;    3312   	int i,j;
;    3313   	for(i=0;i<ms;i++)
;    3314   	for(j=0;j<1000;j++)
;    3315   		;
;    3316 
;    3317 }
;    3318 */
;    3319 /*
;    3320 char lowc(uchar x)
;    3321 {
;    3322 	x = x&0x0f;
;    3323 	if(x > 0x09)
;    3324 		return 'A'+x-0x0a;
;    3325 	else
;    3326 		return '0'+x;
;    3327 }
;    3328 char highc(uchar x)
;    3329 {
;    3330 
;    3331 	x = x>>4;
;    3332 	if(x > 0x09)
;    3333 		return 'A'+x-0x0a;
;    3334 	else
;    3335 		return '0'+x;
;    3336 }
;    3337 */
;    3338 /*计算字符串长度*/
;    3339 
;    3340 extern uchar pos_databuf;
;    3341 extern uchar data_sign;
;    3342 extern u8 databuf[12];
;    3343 double buf2double()
;    3344 {
_buf2double:
;    3345         return atof(databuf);
	CALL SUBOPT_0xFC
	CALL _atof
	RET
;    3346         /*
;    3347 	double tmp = 0.0;
;    3348 	uchar i = 0;
;    3349 	uchar pos = 0;
;    3350 	for(i=1;i<pos_databuf;i++) {
;    3351 		if(databuf[i] != KEY_DOT)
;    3352 			tmp = tmp * 10.0+(databuf[i] - '0');
;    3353 		else
;    3354 			pos = pos_databuf - i - 2;
;    3355 	}
;    3356 	while(pos > 0) {
;    3357 		tmp = tmp / 10.0;
;    3358 		pos--;
;    3359 	}
;    3360 	if(data_sign == 1)
;    3361 	        tmp = -tmp;
;    3362        	return tmp;
;    3363        	*/
;    3364 }
;    3365 int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
;    3366 {
_buf2byte:
;    3367         return atoi(databuf);
	CALL SUBOPT_0xFC
	CALL _atoi
	RET
;    3368         /*
;    3369 	int tmp = 0;
;    3370 	uchar i;
;    3371 	for(i=0;i<pos_databuf;i++) {
;    3372 		tmp = tmp * 10+(databuf[i] - '0');
;    3373 	}
;    3374 	return tmp;
;    3375 	*/
;    3376 }
;    3377 
;    3378 
;    3379 #include "window.h"
;    3380 #include "utili.h"
;    3381 #include "lcd.h"
;    3382 
;    3383 MSG_HANDLER curr_window = 0;

	.DSEG
_curr_window:
	.BYTE 0x2
;    3384 
;    3385 extern uchar key;
;    3386 
;    3387 u8 databuf[12];
_databuf:
	.BYTE 0xC
;    3388 u8 pos_databuf; //position in data buffer
_pos_databuf:
	.BYTE 0x1
;    3389 u8 max_databuf;
_max_databuf:
	.BYTE 0x1
;    3390 u8 data_sign;   // sign of the data
_data_sign:
	.BYTE 0x1
;    3391 LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};

	.CSEG
;    3392 LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
;    3393 LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
;    3394 LABEL flash databox = {LBL_HZ16,20,30,9,databuf};
;    3395 
;    3396 
;    3397 
;    3398 void prbsninput()
;    3399 {
_prbsninput:
;    3400 	uchar msg,len;
;    3401 	databuf[0] = '\0';
	ST   -Y,R17
	ST   -Y,R16
;	msg -> R17
;	len -> R16
	CALL SUBOPT_0xFD
;    3402 	pos_databuf = 0;
;    3403 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3404 	while(1)
_0x341:
;    3405 	{
;    3406 		if(key == KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x341
;    3407 		        continue;
;    3408 
;    3409 		msg = key;
	MOV  R17,R5
;    3410 
;    3411 
;    3412 		if(msg == MSG_INIT) {
	CPI  R17,255
	BRNE _0x345
;    3413 			LCD_Cls();
	CALL SUBOPT_0xFE
;    3414 			draw_label(&datalbl,SW_NORMAL);
;    3415 			draw_label(&datalbl3,SW_NORMAL);
	LDI  R30,LOW(_datalbl3*2)
	LDI  R31,HIGH(_datalbl3*2)
	CALL SUBOPT_0x32
;    3416 			sprintf(databuf,"");
	CALL SUBOPT_0xFC
	__POINTW1FN _0,11
	CALL SUBOPT_0x2F
;    3417 			draw_inputbox(&databox);
	CALL SUBOPT_0xFF
;    3418 //			LCD_ShowCursor(databox.x,databox.y);
;    3419 		}
;    3420 		if(msg == KEY_TAB)
_0x345:
	CPI  R17,84
	BRNE _0x346
;    3421 		{
;    3422 		/*
;    3423         		if(pos_databuf < max_databuf)
;    3424         		{
;    3425 			        databuf[pos_databuf++] = '0';
;    3426 			        databuf[pos_databuf] = '\0';
;    3427         			msg = MSG_REFRESH;
;    3428                         }
;    3429                 */
;    3430                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3431                         return;
	RJMP _0x486
;    3432 		}
;    3433 		if(msg == KEY_CE) {
_0x346:
	CPI  R17,67
	BRNE _0x347
;    3434 		        key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3435 			return;
	RJMP _0x486
;    3436 		}
;    3437 
;    3438 		if(msg == KEY_DN) {
_0x347:
	CPI  R17,68
	BRNE _0x348
;    3439 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x349
;    3440 			{
;    3441 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x100
	CALL SUBOPT_0x101
;    3442 				databuf[pos_databuf] = '\0';
;    3443 			}
;    3444 
;    3445 			if(databuf[pos_databuf-1] == '0')
_0x349:
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRNE _0x34A
;    3446 				databuf[pos_databuf-1] = 'Z';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(90)
	STD  Z+0,R26
;    3447 			else if (databuf[pos_databuf-1] == 'A')
	RJMP _0x34B
_0x34A:
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRNE _0x34C
;    3448 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(57)
	STD  Z+0,R26
;    3449 			else
	RJMP _0x34D
_0x34C:
;    3450 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,LOW(1)
	ST   X,R30
;    3451 			msg = MSG_REFRESH;
_0x34D:
_0x34B:
	LDI  R17,LOW(254)
;    3452 		}
;    3453 		if(msg == KEY_UP) {
_0x348:
	CPI  R17,85
	BRNE _0x34E
;    3454 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x34F
;    3455 			{
;    3456 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x100
	CALL SUBOPT_0x101
;    3457 				databuf[pos_databuf] = '\0';
;    3458 			}
;    3459 
;    3460 			if(databuf[pos_databuf-1] == '9')
_0x34F:
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x39)
	BRNE _0x350
;    3461 				databuf[pos_databuf-1] = 'A';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    3462 			else if (databuf[pos_databuf-1] == 'Z')
	RJMP _0x351
_0x350:
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x5A)
	BRNE _0x352
;    3463 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
;    3464 			else
	RJMP _0x353
_0x352:
;    3465 				databuf[pos_databuf-1] +=1;
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
;    3466 			msg = MSG_REFRESH;
_0x353:
_0x351:
	LDI  R17,LOW(254)
;    3467 		}
;    3468 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x34E:
	CPI  R17,48
	BRLO _0x355
	CPI  R17,58
	BRLO _0x356
_0x355:
	RJMP _0x354
_0x356:
;    3469         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x103
	BRSH _0x357
;    3470         		{
;    3471 			        databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x100
	CALL SUBOPT_0x104
;    3472         			databuf[pos_databuf] = '\0';
;    3473         		}
;    3474 			msg = MSG_REFRESH;
_0x357:
	LDI  R17,LOW(254)
;    3475 		}
;    3476 		if(msg == KEY_OK) {
_0x354:
	CPI  R17,79
	BRNE _0x358
;    3477 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x3C
	BRNE _0x359
;    3478 		        {
;    3479 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x35B:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x35C
;    3480         		                tprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0x105
;    3481         			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x35B
_0x35C:
	__POINTW2MN _tprbdata,384
	RJMP _0x4C0
;    3482         		}else{
_0x359:
;    3483 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x35F:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x360
;    3484         		                rprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0x105
;    3485         			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x35F
_0x360:
	__POINTW2MN _rprbdata,384
_0x4C0:
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
;    3486         		}
;    3487 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3488 			break;
	RJMP _0x343
;    3489 		}
;    3490 		if(msg == MSG_REFRESH) {
_0x358:
	CPI  R17,254
	BRNE _0x361
;    3491 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x106
;    3492 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3493 		}
;    3494 		key = KEY_INVALID;
_0x361:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3495 	}
	RJMP _0x341
_0x343:
;    3496 //	LCD_HideCursor();
;    3497 }
_0x486:
	LD   R16,Y+
	LD   R17,Y+
	RET
;    3498 
;    3499 uchar wnd_intinput(uchar lastval)
;    3500 {
_wnd_intinput:
;    3501 	uchar msg;
;    3502 
;    3503 	key = MSG_INIT;
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3504 	databuf[0] = '\0';
	CALL SUBOPT_0xFD
;    3505 	pos_databuf = 0;
;    3506         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3507 	while(1)
_0x362:
;    3508 	{
;    3509 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x365
;    3510 		{
;    3511 			msg = key;
	MOV  R17,R5
;    3512 
;    3513 		}else{
	RJMP _0x366
_0x365:
;    3514 			continue;
	RJMP _0x362
;    3515 		}
_0x366:
;    3516 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x367
;    3517 		{
;    3518 			LCD_Cls();
	CALL SUBOPT_0xFE
;    3519 			draw_label(&datalbl,SW_NORMAL);
;    3520 			draw_inputbox(&databox);
	CALL SUBOPT_0xFF
;    3521 //			LCD_ShowCursor(databox.x,databox.y);
;    3522 
;    3523 		}
;    3524 
;    3525 		if(msg == KEY_TAB)
_0x367:
	CPI  R17,84
	BRNE _0x368
;    3526 		{
;    3527 		/*
;    3528         		if(pos_databuf < max_databuf)
;    3529         		{
;    3530 			        databuf[pos_databuf++] = '0';
;    3531 			        databuf[pos_databuf] = '\0';
;    3532         			msg = MSG_REFRESH;
;    3533                         }
;    3534                 */
;    3535 		}
;    3536 
;    3537 		if(msg == KEY_DN) {
_0x368:
	CPI  R17,68
	BRNE _0x369
;    3538 		/*
;    3539 			if(pos_databuf == 0)
;    3540 			{
;    3541 				databuf[pos_databuf++] = '0';
;    3542 				databuf[pos_databuf] = '\0';
;    3543 			}
;    3544 
;    3545 			if(databuf[pos_databuf-1] == '0')
;    3546 				databuf[pos_databuf-1] = '9';
;    3547 			else
;    3548 				databuf[pos_databuf-1] -= 1;
;    3549 			msg = MSG_REFRESH;
;    3550                 */
;    3551 		}
;    3552 		if(msg == KEY_UP) {
_0x369:
	CPI  R17,85
	BRNE _0x36A
;    3553 		/*
;    3554 			if(pos_databuf == 0)
;    3555 			{
;    3556 				databuf[pos_databuf++] = '0';
;    3557 				databuf[pos_databuf] = '\0';
;    3558 			}
;    3559 
;    3560 			if(databuf[pos_databuf-1] == '9')
;    3561 				databuf[pos_databuf-1] = '0';
;    3562 			else
;    3563 				databuf[pos_databuf-1] += 1;
;    3564 			msg = MSG_REFRESH;
;    3565                 */
;    3566 		}
;    3567 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x36A:
	CPI  R17,48
	BRLO _0x36C
	CPI  R17,58
	BRLO _0x36D
_0x36C:
	RJMP _0x36B
_0x36D:
;    3568         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x103
	BRSH _0x36E
;    3569         		{
;    3570         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x100
	CALL SUBOPT_0x104
;    3571 	        		databuf[pos_databuf] = '\0';
;    3572 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3573 		        }
;    3574 		}
_0x36E:
;    3575 		if(msg == KEY_CE) {
_0x36B:
	CPI  R17,67
	BRNE _0x36F
;    3576 //			LCD_HideCursor();
;    3577                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3578 			return lastval;
	LDD  R30,Y+1
	LDD  R17,Y+0
	RJMP _0x484
;    3579 		}
;    3580 		if(msg == KEY_OK){
_0x36F:
	CPI  R17,79
	BRNE _0x370
;    3581 //			LCD_HideCursor();
;    3582                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3583 			return buf2byte();
	CALL _buf2byte
	LDD  R17,Y+0
	RJMP _0x484
;    3584 		}
;    3585 		if(msg == MSG_REFRESH) {
_0x370:
	CPI  R17,254
	BRNE _0x371
;    3586 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x106
;    3587 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3588 		}
;    3589 		key = KEY_INVALID;
_0x371:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3590 	}
	RJMP _0x362
;    3591 }
;    3592 double wnd_floatinput(double lastval)
;    3593 {
_wnd_floatinput:
;    3594 	uchar msg;
;    3595 
;    3596 	databuf[0] = '+';
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3597         databuf[1] = '\0';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3598         pos_databuf = 1;
	LDI  R30,LOW(1)
	STS  _pos_databuf,R30
;    3599         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3600 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3601 	while(1)
_0x372:
;    3602 	{
;    3603 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x375
;    3604 		{
;    3605 			msg = key;
	MOV  R17,R5
;    3606 
;    3607 		}else{
	RJMP _0x376
_0x375:
;    3608 			continue;
	RJMP _0x372
;    3609 		}
_0x376:
;    3610 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x377
;    3611 		{
;    3612 			LCD_Cls();
	CALL SUBOPT_0xFE
;    3613 			draw_label(&datalbl,SW_NORMAL);
;    3614 			draw_label(&datalbl2,SW_NORMAL);
	LDI  R30,LOW(_datalbl2*2)
	LDI  R31,HIGH(_datalbl2*2)
	CALL SUBOPT_0x32
;    3615 			draw_inputbox(&databox);
	CALL SUBOPT_0xFF
;    3616 //			LCD_ShowCursor(databox.x,databox.y);
;    3617 		}
;    3618 
;    3619 		if(msg == KEY_TAB)
_0x377:
	CPI  R17,84
	BRNE _0x378
;    3620 		{
;    3621 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3622 		}
;    3623 		if(msg == KEY_UP) {
_0x378:
	CPI  R17,85
	BREQ PC+3
	JMP _0x379
;    3624 		        if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x37A
;    3625 		        {
;    3626 		                if((data_sign & 0x01) == 0)      //no sign
	LDS  R30,_data_sign
	ANDI R30,LOW(0x1)
	BRNE _0x37B
;    3627 		                {
;    3628 		                        databuf[0] = '-';
	LDI  R30,LOW(45)
	STS  _databuf,R30
;    3629 		                        data_sign |= 0x01;
	LDS  R30,_data_sign
	ORI  R30,1
	RJMP _0x4C1
;    3630 		                }else{
_0x37B:
;    3631 		                        databuf[0] = '+';       //
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3632 		                        data_sign ^= 0x01;
	LDS  R26,_data_sign
	LDI  R30,LOW(1)
	EOR  R30,R26
_0x4C1:
	STS  _data_sign,R30
;    3633 		                }
;    3634 		        }else{
	RJMP _0x37D
_0x37A:
;    3635                        		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
	CALL SUBOPT_0x103
	BRSH _0x37F
	LDS  R30,_pos_databuf
	SUBI R30,LOW(2)
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x45)
	BREQ _0x380
_0x37F:
	RJMP _0x37E
_0x380:
;    3636                 		{
;    3637 			                if((data_sign & 0x08) == 0)
	LDS  R30,_data_sign
	ANDI R30,LOW(0x8)
	BRNE _0x381
;    3638 			                {
;    3639                		        	        databuf[pos_databuf-1] = '-';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(45)
	STD  Z+0,R26
;    3640                		        	        data_sign |= 0x08;
	LDS  R30,_data_sign
	ORI  R30,8
	RJMP _0x4C2
;    3641                		                }else{
_0x381:
;    3642                		        	        databuf[pos_databuf-1] = '+';
	CALL SUBOPT_0x102
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3643                		        	        data_sign ^= 0x08;
	LDS  R26,_data_sign
	LDI  R30,LOW(8)
	EOR  R30,R26
_0x4C2:
	STS  _data_sign,R30
;    3644                		                }
;    3645 	        		}
;    3646 		        	msg = MSG_REFRESH;
_0x37E:
	LDI  R17,LOW(254)
;    3647 		        }
_0x37D:
;    3648 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3649 		}
;    3650 		if(msg == KEY_DN) {
_0x379:
	CPI  R17,68
	BRNE _0x383
;    3651                		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
	CALL SUBOPT_0x103
	BRSH _0x385
	LDS  R30,_data_sign
	ANDI R30,LOW(0x4)
	BREQ _0x386
_0x385:
	RJMP _0x384
_0x386:
;    3652         		{
;    3653         			databuf[pos_databuf++] = 'E';
	CALL SUBOPT_0x100
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(69)
	STD  Z+0,R26
;    3654         			databuf[pos_databuf++] = '+';
	CALL SUBOPT_0x100
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3655 	        		databuf[pos_databuf] = '\0';
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3656 	        		data_sign |= 0x04;
	LDS  R30,_data_sign
	ORI  R30,4
	STS  _data_sign,R30
;    3657 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3658 		        }
;    3659 
;    3660 			msg = MSG_REFRESH;
_0x384:
	LDI  R17,LOW(254)
;    3661 		}
;    3662 		if( msg == KEY_DOT)
_0x383:
	CPI  R17,46
	BRNE _0x387
;    3663 		{
;    3664                		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
	CALL SUBOPT_0x103
	BRSH _0x389
	LDS  R30,_data_sign
	ANDI R30,LOW(0x2)
	BREQ _0x38A
_0x389:
	RJMP _0x388
_0x38A:
;    3665         		{
;    3666         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x100
	CALL SUBOPT_0x104
;    3667 	        		databuf[pos_databuf] = '\0';
;    3668 	        		data_sign |= 0x02;
	LDS  R30,_data_sign
	ORI  R30,2
	STS  _data_sign,R30
;    3669 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3670 		        }
;    3671 
;    3672 		}
_0x388:
;    3673 		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
_0x387:
	CPI  R17,48
	BRLO _0x38C
	CPI  R17,58
	BRLO _0x38D
_0x38C:
	RJMP _0x38B
_0x38D:
;    3674                		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x103
	BRSH _0x38E
;    3675         		{
;    3676         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x100
	CALL SUBOPT_0x104
;    3677 	        		databuf[pos_databuf] = '\0';
;    3678 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3679 		        }
;    3680 		}
_0x38E:
;    3681 		if(msg == KEY_CE) {
_0x38B:
	CPI  R17,67
	BRNE _0x38F
;    3682 //			LCD_HideCursor();
;    3683 		key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3684 			return lastval;
	__GETD1S 1
	LDD  R17,Y+0
	RJMP _0x485
;    3685 		}
;    3686 		if(msg == KEY_OK){
_0x38F:
	CPI  R17,79
	BRNE _0x390
;    3687 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3688 //			LCD_HideCursor();
;    3689 			return buf2double();
	CALL _buf2double
	LDD  R17,Y+0
	RJMP _0x485
;    3690 		}
;    3691 		if(msg == MSG_REFRESH) {
_0x390:
	CPI  R17,254
	BRNE _0x391
;    3692 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x106
;    3693 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3694 		}
;    3695 				key = KEY_INVALID;
_0x391:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3696 	}
	RJMP _0x372
;    3697 }
_0x485:
	ADIW R28,5
	RET
;    3698 //display a message box
;    3699 void wnd_msgbox(flash LABEL *lbl)
;    3700 {
_wnd_msgbox:
;    3701 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x107
	CALL _LCD_ClsBlock
;    3702 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x32
;    3703 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	CALL SUBOPT_0x107
	CALL _LCD_Rectange
;    3704 	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
	CALL SUBOPT_0x108
	SBIW R30,8
	CALL SUBOPT_0x109
	SBIW R30,1
	CALL SUBOPT_0x10A
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(16)
	LDI  R31,HIGH(16)
	CALL __SWAPW12
	SUB  R30,R26
	SBC  R31,R27
	CALL SUBOPT_0x10B
	ADIW R30,17
	ST   -Y,R30
	CALL _LCD_Rectange
;    3705 
;    3706 }
	RJMP _0x484
;    3707 
;    3708 void draw_inputbox(flash LABEL *lbl)
;    3709 {
_draw_inputbox:
;    3710 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x108
	SBIW R30,10
	CALL SUBOPT_0x109
	SBIW R30,3
	CALL SUBOPT_0x10A
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x10B
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3711 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	CALL SUBOPT_0x108
	SBIW R30,10
	CALL SUBOPT_0x109
	SBIW R30,3
	CALL SUBOPT_0x10A
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x10B
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_Rectange
;    3712 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x32
;    3713 }
_0x484:
	ADIW R28,2
	RET
;    3714 
;    3715 void draw_label(flash LABEL *lbl,uchar reverse) {
_draw_label:
;    3716 	uchar len = lbl->width;
;    3717 	if(lbl->type == LBL_HZ6X8) {
	ST   -Y,R17
;	*lbl -> Y+2
;	reverse -> Y+1
;	len -> R17
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	MOV  R17,R30
	CALL SUBOPT_0x10C
	SBIW R30,1
	BRNE _0x392
;    3718 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x393
;    3719 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x394
;    3720 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10E
	ADIW R30,8
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3721 			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
_0x394:
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10F
	CALL _LCD_Print6X8
;    3722 			return;
	LDD  R17,Y+0
	RJMP _0x483
;    3723 		}
;    3724 		if(reverse & SW_REVERSE)
_0x393:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x395
;    3725 			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
	CALL SUBOPT_0x110
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3726 		return;
_0x395:
	LDD  R17,Y+0
	RJMP _0x483
;    3727 	}
;    3728 	if(lbl->type == LBL_HZ8X16) {
_0x392:
	CALL SUBOPT_0x10C
	SBIW R30,2
	BRNE _0x396
;    3729 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x397
;    3730 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x398
;    3731 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10E
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3732 			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
_0x398:
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10F
	CALL _LCD_Print8X16
;    3733 			return;
	LDD  R17,Y+0
	RJMP _0x483
;    3734 		}
;    3735 		if(reverse & SW_REVERSE)
_0x397:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x399
;    3736 			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
	CALL SUBOPT_0x110
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3737 		return;
_0x399:
	LDD  R17,Y+0
	RJMP _0x483
;    3738 	}
;    3739 	/*
;    3740 	if(lbl->type == LBL_HZ24X32) {
;    3741 		if(reverse & SW_NORMAL) {
;    3742 			if((reverse & SW_OVERLAP) == 0)
;    3743 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
;    3744 			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
;    3745 		}
;    3746 		if(reverse & SW_REVERSE)
;    3747 			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
;    3748 		return;
;    3749 	}
;    3750 	*/
;    3751 	if(lbl->type == LBL_HZ12) {
_0x396:
	CALL SUBOPT_0x10C
	SBIW R30,4
	BRNE _0x39A
;    3752 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x39B
;    3753 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x39C
;    3754 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x111
	ADIW R30,12
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3755 			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
_0x39C:
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10F
	CALL _LCD_PrintHz12
;    3756 		}
;    3757 		if(reverse & SW_REVERSE)
_0x39B:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x39D
;    3758 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
	CALL SUBOPT_0x112
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3759 		return;
_0x39D:
	LDD  R17,Y+0
	RJMP _0x483
;    3760 	}
;    3761 	if(lbl->type == LBL_HZ16) {
_0x39A:
	CALL SUBOPT_0x10C
	SBIW R30,5
	BRNE _0x39E
;    3762 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x39F
;    3763 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3A0
;    3764 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x111
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3765 			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
_0x3A0:
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x10F
	CALL _LCD_PrintHz16
;    3766 		}
;    3767 		if(reverse & SW_REVERSE)
_0x39F:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3A1
;    3768 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
	CALL SUBOPT_0x112
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3769 		return;
_0x3A1:
	LDD  R17,Y+0
	RJMP _0x483
;    3770 	}
;    3771 }
_0x39E:
	LDD  R17,Y+0
_0x483:
	ADIW R28,4
	RET
;    3772 
;    3773 

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
	BREQ _0x3A2
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x3A4
	__CPWRN 16,17,2
	BRLO _0x3A5
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x3A4:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL SUBOPT_0x113
	LDD  R26,Y+6
	STD  Z+0,R26
_0x3A5:
	RJMP _0x3A6
_0x3A2:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x3A6:
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
	BRLO _0x3A7
	LDI  R30,LOW(6)
	STD  Y+8,R30
_0x3A7:
	LDD  R30,Y+8
	LDI  R26,LOW(__fround_GD*2)
	LDI  R27,HIGH(__fround_GD*2)
	LDI  R31,0
	CALL SUBOPT_0x82
	CALL __GETD1PF
	CALL SUBOPT_0x114
	CALL __ADDF12
	CALL SUBOPT_0x115
	LDI  R17,LOW(0)
	CALL SUBOPT_0x27
	CALL SUBOPT_0x72
_0x3A8:
	CALL SUBOPT_0x56
	CALL SUBOPT_0x114
	CALL __CMPF12
	BRLO _0x3AA
	CALL SUBOPT_0x116
	CALL SUBOPT_0x117
	SUBI R17,-LOW(1)
	RJMP _0x3A8
_0x3AA:
	CPI  R17,0
	BRNE _0x3AB
	CALL SUBOPT_0x118
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x3AC
_0x3AB:
_0x3AD:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3AF
	CALL SUBOPT_0x116
	CALL SUBOPT_0x119
	CALL SUBOPT_0x72
	CALL SUBOPT_0x56
	CALL SUBOPT_0x114
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x118
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x52
	CALL SUBOPT_0x11B
	CALL __MULF12
	CALL SUBOPT_0x114
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x115
	RJMP _0x3AD
_0x3AF:
_0x3AC:
	LDD  R30,Y+8
	CPI  R30,0
	BRNE _0x3B0
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x482
_0x3B0:
	CALL SUBOPT_0x118
	LDI  R30,LOW(46)
	ST   X,R30
_0x3B1:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x3B3
	CALL SUBOPT_0x114
	CALL SUBOPT_0x11C
	CALL SUBOPT_0x115
	__GETD1S 9
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x118
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x114
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x115
	RJMP _0x3B1
_0x3B3:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x482:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
__ftoe_GD:
	SBIW R28,4
	CALL __SAVELOCR4
	CALL SUBOPT_0x27
	CALL SUBOPT_0x81
	LDD  R26,Y+11
	CPI  R26,LOW(0x7)
	BRLO _0x3B4
	LDI  R30,LOW(6)
	STD  Y+11,R30
_0x3B4:
	LDD  R17,Y+11
_0x3B5:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3B7
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x81
	RJMP _0x3B5
_0x3B7:
	CALL SUBOPT_0x11E
	CALL __CPD10
	BRNE _0x3B8
	LDI  R19,LOW(0)
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x81
	RJMP _0x3B9
_0x3B8:
	LDD  R19,Y+11
	CALL SUBOPT_0x11F
	BREQ PC+2
	BRCC PC+3
	JMP  _0x3BA
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x81
_0x3BB:
	CALL SUBOPT_0x11F
	BRLO _0x3BD
	CALL SUBOPT_0x120
	CALL SUBOPT_0x121
	RJMP _0x3BB
_0x3BD:
	RJMP _0x3BE
_0x3BA:
_0x3BF:
	CALL SUBOPT_0x11F
	BRSH _0x3C1
	CALL SUBOPT_0x122
	CALL SUBOPT_0x123
	SUBI R19,LOW(1)
	RJMP _0x3BF
_0x3C1:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x81
_0x3BE:
	CALL SUBOPT_0x11E
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL SUBOPT_0x123
	CALL SUBOPT_0x11F
	BRLO _0x3C2
	CALL SUBOPT_0x120
	CALL SUBOPT_0x121
_0x3C2:
_0x3B9:
	LDI  R17,LOW(0)
_0x3C3:
	LDD  R30,Y+11
	CP   R30,R17
	BRLO _0x3C5
	CALL SUBOPT_0x124
	__GETD1N 0x41200000
	CALL SUBOPT_0x119
	CALL SUBOPT_0x81
	CALL SUBOPT_0x1
	CALL SUBOPT_0x120
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x125
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x124
	CALL __MULF12
	CALL SUBOPT_0x120
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x123
	MOV  R30,R17
	SUBI R17,-1
	CPI  R30,0
	BRNE _0x3C3
	CALL SUBOPT_0x125
	LDI  R30,LOW(46)
	ST   X,R30
	RJMP _0x3C3
_0x3C5:
	CALL SUBOPT_0xCE
	SBIW R30,1
	LDD  R26,Y+10
	STD  Z+0,R26
	CPI  R19,0
	BRGE _0x3C7
	CALL SUBOPT_0x125
	LDI  R30,LOW(45)
	ST   X,R30
	NEG  R19
_0x3C7:
	CPI  R19,10
	BRLT _0x3C8
	CALL SUBOPT_0xCE
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __DIVB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
_0x3C8:
	CALL SUBOPT_0xCE
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
_0x3C9:
	MOVW R26,R28
	SUBI R26,LOW(-(90))
	SBCI R27,HIGH(-(90))
	CALL SUBOPT_0x113
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x3CB
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x3CF
	CPI  R18,37
	BRNE _0x3D0
	LDI  R17,LOW(1)
	RJMP _0x3D1
_0x3D0:
	CALL SUBOPT_0x126
_0x3D1:
	RJMP _0x3CE
_0x3CF:
	CPI  R30,LOW(0x1)
	BRNE _0x3D2
	CPI  R18,37
	BRNE _0x3D3
	CALL SUBOPT_0x126
	RJMP _0x4C3
_0x3D3:
	LDI  R17,LOW(2)
	LDI  R30,LOW(0)
	STD  Y+19,R30
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x3D4
	LDI  R16,LOW(1)
	RJMP _0x3CE
_0x3D4:
	CPI  R18,43
	BRNE _0x3D5
	LDI  R30,LOW(43)
	STD  Y+19,R30
	RJMP _0x3CE
_0x3D5:
	CPI  R18,32
	BRNE _0x3D6
	LDI  R30,LOW(32)
	STD  Y+19,R30
	RJMP _0x3CE
_0x3D6:
	RJMP _0x3D7
_0x3D2:
	CPI  R30,LOW(0x2)
	BRNE _0x3D8
_0x3D7:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x3D9
	ORI  R16,LOW(128)
	RJMP _0x3CE
_0x3D9:
	RJMP _0x3DA
_0x3D8:
	CPI  R30,LOW(0x3)
	BRNE _0x3DB
_0x3DA:
	CPI  R18,48
	BRLO _0x3DD
	CPI  R18,58
	BRLO _0x3DE
_0x3DD:
	RJMP _0x3DC
_0x3DE:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x3CE
_0x3DC:
	LDI  R20,LOW(0)
	CPI  R18,46
	BRNE _0x3DF
	LDI  R17,LOW(4)
	RJMP _0x3CE
_0x3DF:
	RJMP _0x3E0
_0x3DB:
	CPI  R30,LOW(0x4)
	BRNE _0x3E2
	CPI  R18,48
	BRLO _0x3E4
	CPI  R18,58
	BRLO _0x3E5
_0x3E4:
	RJMP _0x3E3
_0x3E5:
	ORI  R16,LOW(32)
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R20,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R20,R30
	RJMP _0x3CE
_0x3E3:
_0x3E0:
	CPI  R18,108
	BRNE _0x3E6
	ORI  R16,LOW(2)
	LDI  R17,LOW(5)
	RJMP _0x3CE
_0x3E6:
	RJMP _0x3E7
_0x3E2:
	CPI  R30,LOW(0x5)
	BREQ PC+3
	JMP _0x3CE
_0x3E7:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x3EC
	CALL SUBOPT_0x127
	LDD  R30,Z+4
	CALL SUBOPT_0x128
	RJMP _0x3ED
_0x3EC:
	CPI  R30,LOW(0x45)
	BREQ _0x3F0
	CPI  R30,LOW(0x65)
	BRNE _0x3F1
_0x3F0:
	RJMP _0x3F2
_0x3F1:
	CPI  R30,LOW(0x66)
	BREQ PC+3
	JMP _0x3F3
_0x3F2:
	MOVW R30,R28
	ADIW R30,20
	STD  Y+10,R30
	STD  Y+10+1,R31
	CALL SUBOPT_0x127
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	CALL SUBOPT_0x61
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD20
	BRLT _0x3F4
	LDD  R26,Y+19
	CPI  R26,LOW(0x2B)
	BREQ _0x3F6
	RJMP _0x3F7
_0x3F4:
	CALL SUBOPT_0x55
	CALL __ANEGF1
	CALL SUBOPT_0x61
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x3F6:
	SBRS R16,7
	RJMP _0x3F8
	LDD  R30,Y+19
	CALL SUBOPT_0x128
	RJMP _0x3F9
_0x3F8:
	CALL SUBOPT_0x129
	LDD  R26,Y+19
	STD  Z+0,R26
_0x3F9:
_0x3F7:
	SBRS R16,5
	LDI  R20,LOW(6)
	CPI  R18,102
	BRNE _0x3FB
	CALL SUBOPT_0x65
	ST   -Y,R20
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoa_GD
	RJMP _0x3FC
_0x3FB:
	CALL SUBOPT_0x65
	ST   -Y,R20
	ST   -Y,R18
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoe_GD
_0x3FC:
	MOVW R30,R28
	ADIW R30,20
	CALL SUBOPT_0x12A
	RJMP _0x3FD
_0x3F3:
	CPI  R30,LOW(0x73)
	BRNE _0x3FF
	CALL SUBOPT_0x127
	CALL SUBOPT_0x12B
	CALL SUBOPT_0x12A
	RJMP _0x400
_0x3FF:
	CPI  R30,LOW(0x70)
	BRNE _0x402
	CALL SUBOPT_0x127
	CALL SUBOPT_0x12B
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x400:
	ANDI R16,LOW(127)
	CPI  R20,0
	BREQ _0x404
	CP   R20,R17
	BRLO _0x405
_0x404:
	RJMP _0x403
_0x405:
	MOV  R17,R20
_0x403:
_0x3FD:
	LDI  R20,LOW(0)
	LDI  R30,LOW(0)
	STD  Y+18,R30
	LDI  R19,LOW(0)
	RJMP _0x406
_0x402:
	CPI  R30,LOW(0x64)
	BREQ _0x409
	CPI  R30,LOW(0x69)
	BRNE _0x40A
_0x409:
	ORI  R16,LOW(4)
	RJMP _0x40B
_0x40A:
	CPI  R30,LOW(0x75)
	BRNE _0x40C
_0x40B:
	LDI  R30,LOW(10)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x40D
	__GETD1N 0x3B9ACA00
	CALL SUBOPT_0x123
	LDI  R17,LOW(10)
	RJMP _0x40E
_0x40D:
	__GETD1N 0x2710
	CALL SUBOPT_0x123
	LDI  R17,LOW(5)
	RJMP _0x40E
_0x40C:
	CPI  R30,LOW(0x58)
	BRNE _0x410
	ORI  R16,LOW(8)
	RJMP _0x411
_0x410:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x44F
_0x411:
	LDI  R30,LOW(16)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x413
	__GETD1N 0x10000000
	CALL SUBOPT_0x123
	LDI  R17,LOW(8)
	RJMP _0x40E
_0x413:
	__GETD1N 0x1000
	CALL SUBOPT_0x123
	LDI  R17,LOW(4)
_0x40E:
	CPI  R20,0
	BREQ _0x414
	ANDI R16,LOW(127)
	RJMP _0x415
_0x414:
	LDI  R20,LOW(1)
_0x415:
	SBRS R16,1
	RJMP _0x416
	CALL SUBOPT_0x127
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	RJMP _0x4C4
_0x416:
	SBRS R16,2
	RJMP _0x418
	CALL SUBOPT_0x127
	CALL SUBOPT_0x12B
	CALL __CWD1
	RJMP _0x4C4
_0x418:
	CALL SUBOPT_0x127
	CALL SUBOPT_0x12B
	CLR  R22
	CLR  R23
_0x4C4:
	__PUTD1S 6
	SBRS R16,2
	RJMP _0x41A
	CALL SUBOPT_0x54
	CALL __CPD20
	BRGE _0x41B
	CALL SUBOPT_0x55
	CALL __ANEGD1
	CALL SUBOPT_0x61
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x41B:
	LDD  R30,Y+19
	CPI  R30,0
	BREQ _0x41C
	SUBI R17,-LOW(1)
	SUBI R20,-LOW(1)
	RJMP _0x41D
_0x41C:
	ANDI R16,LOW(251)
_0x41D:
_0x41A:
	MOV  R19,R20
_0x406:
	SBRC R16,0
	RJMP _0x41E
_0x41F:
	CP   R17,R21
	BRSH _0x422
	CP   R19,R21
	BRLO _0x423
_0x422:
	RJMP _0x421
_0x423:
	SBRS R16,7
	RJMP _0x424
	SBRS R16,2
	RJMP _0x425
	ANDI R16,LOW(251)
	LDD  R18,Y+19
	SUBI R17,LOW(1)
	RJMP _0x426
_0x425:
	LDI  R18,LOW(48)
_0x426:
	RJMP _0x427
_0x424:
	LDI  R18,LOW(32)
_0x427:
	CALL SUBOPT_0x126
	SUBI R21,LOW(1)
	RJMP _0x41F
_0x421:
_0x41E:
_0x428:
	CP   R17,R20
	BRSH _0x42A
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x42B
	CALL SUBOPT_0x12C
	BREQ _0x42C
	SUBI R21,LOW(1)
_0x42C:
	SUBI R17,LOW(1)
	SUBI R20,LOW(1)
_0x42B:
	LDI  R30,LOW(48)
	CALL SUBOPT_0x128
	CPI  R21,0
	BREQ _0x42D
	SUBI R21,LOW(1)
_0x42D:
	SUBI R20,LOW(1)
	RJMP _0x428
_0x42A:
	MOV  R19,R17
	LDD  R30,Y+18
	CPI  R30,0
	BRNE _0x42E
_0x42F:
	CPI  R19,0
	BREQ _0x431
	SBRS R16,3
	RJMP _0x432
	CALL SUBOPT_0x129
	LPM  R30,Z
	RJMP _0x4C5
_0x432:
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X+
	STD  Y+10,R26
	STD  Y+10+1,R27
_0x4C5:
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
	BREQ _0x434
	SUBI R21,LOW(1)
_0x434:
	SUBI R19,LOW(1)
	RJMP _0x42F
_0x431:
	RJMP _0x435
_0x42E:
_0x437:
	CALL SUBOPT_0x11E
	CALL SUBOPT_0x54
	CALL __DIVD21U
	MOV  R18,R30
	CPI  R18,10
	BRLO _0x439
	SBRS R16,3
	RJMP _0x43A
	SUBI R18,-LOW(55)
	RJMP _0x43B
_0x43A:
	SUBI R18,-LOW(87)
_0x43B:
	RJMP _0x43C
_0x439:
	SUBI R18,-LOW(48)
_0x43C:
	SBRC R16,4
	RJMP _0x43E
	CPI  R18,49
	BRSH _0x440
	CALL SUBOPT_0x120
	__CPD2N 0x1
	BRNE _0x43F
_0x440:
	RJMP _0x442
_0x43F:
	CP   R20,R19
	BRLO _0x443
	RJMP _0x4C6
_0x443:
	CP   R21,R19
	BRLO _0x445
	SBRS R16,0
	RJMP _0x446
_0x445:
	RJMP _0x444
_0x446:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x447
_0x4C6:
	LDI  R18,LOW(48)
_0x442:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x448
	CALL SUBOPT_0x12C
	BREQ _0x449
	SUBI R21,LOW(1)
_0x449:
_0x448:
_0x447:
_0x43E:
	CALL SUBOPT_0x126
	CPI  R21,0
	BREQ _0x44A
	SUBI R21,LOW(1)
_0x44A:
_0x444:
	SUBI R19,LOW(1)
	CALL SUBOPT_0x11E
	CALL SUBOPT_0x54
	CALL __MODD21U
	CALL SUBOPT_0x61
	LDD  R30,Y+18
	CALL SUBOPT_0x120
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __DIVD21U
	CALL SUBOPT_0x123
	CALL SUBOPT_0x11E
	CALL __CPD10
	BREQ _0x438
	RJMP _0x437
_0x438:
_0x435:
	SBRS R16,0
	RJMP _0x44B
_0x44C:
	CPI  R21,0
	BREQ _0x44E
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	CALL SUBOPT_0x128
	RJMP _0x44C
_0x44E:
_0x44B:
_0x44F:
_0x3ED:
_0x4C3:
	LDI  R17,LOW(0)
_0x3CE:
	RJMP _0x3C9
_0x3CB:
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
_atoi:
   	ldd  r27,y+1
   	ld   r26,y
__atoi0:
   	ld   r30,x
	ST   -Y,R30
	CALL _isspace
   	tst  r30
   	breq __atoi1
   	adiw r26,1
   	rjmp __atoi0
__atoi1:
   	clt
   	ld   r30,x
   	cpi  r30,'-'
   	brne __atoi2
   	set
   	rjmp __atoi3
__atoi2:
   	cpi  r30,'+'
   	brne __atoi4
__atoi3:
   	adiw r26,1
__atoi4:
   	clr  r22
   	clr  r23
__atoi5:
   	ld   r30,x
	ST   -Y,R30
	CALL _isdigit
   	tst  r30
   	breq __atoi6
   	mov  r30,r22
   	mov  r31,r23
   	lsl  r22
   	rol  r23
   	lsl  r22
   	rol  r23
   	add  r22,r30
   	adc  r23,r31
   	lsl  r22
   	rol  r23
   	ld   r30,x+
   	clr  r31
   	subi r30,'0'
   	add  r22,r30
   	adc  r23,r31
   	rjmp __atoi5
__atoi6:
   	mov  r30,r22
   	mov  r31,r23
   	brtc __atoi7
   	com  r30
   	com  r31
   	adiw r30,1
__atoi7:
   	adiw r28,2
   	ret
_atof:
	SBIW R28,10
	CALL __SAVELOCR6
	__CLRD1S 8
_0x450:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isspace
	CPI  R30,0
	BREQ _0x452
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x450
_0x452:
	LDI  R30,LOW(0)
	STD  Y+7,R30
	CPI  R21,43
	BRNE _0x453
	RJMP _0x4C7
_0x453:
	CPI  R21,45
	BRNE _0x455
	LDI  R30,LOW(1)
	STD  Y+7,R30
_0x4C7:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x455:
	LDI  R30,LOW(0)
	MOV  R20,R30
	MOV  R21,R30
	__GETWRS 16,17,16
_0x456:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BRNE _0x459
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	LDI  R30,LOW(46)
	CALL __EQB12
	MOV  R21,R30
	CPI  R30,0
	BREQ _0x458
_0x459:
	OR   R20,R21
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x456
_0x458:
	__GETWRS 18,19,16
	CPI  R20,0
	BREQ _0x45B
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x45C:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	CPI  R26,LOW(0x2E)
	BREQ _0x45E
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x12D
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x68
	__GETD1N 0x41200000
	CALL __DIVF21
	CALL SUBOPT_0x7D
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x45C
_0x45E:
_0x45B:
	CALL SUBOPT_0x27
	CALL SUBOPT_0x123
_0x45F:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,1
	STD  Y+16,R26
	STD  Y+16+1,R27
	CP   R26,R16
	CPC  R27,R17
	BRLO _0x461
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x120
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x80
	CALL SUBOPT_0x122
	CALL SUBOPT_0x123
	RJMP _0x45F
_0x461:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R21,X
	CPI  R21,101
	BREQ _0x463
	CPI  R21,69
	BREQ _0x463
	RJMP _0x462
_0x463:
	LDI  R30,LOW(0)
	MOV  R20,R30
	STD  Y+6,R30
	MOVW R26,R18
	LD   R21,X
	CPI  R21,43
	BRNE _0x465
	RJMP _0x4C8
_0x465:
	CPI  R21,45
	BRNE _0x467
	LDI  R30,LOW(1)
	STD  Y+6,R30
_0x4C8:
	__ADDWRN 18,19,1
_0x467:
_0x468:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BREQ _0x46A
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R30,R21
	SUBI R30,LOW(48)
	MOV  R20,R30
	RJMP _0x468
_0x46A:
	CPI  R20,39
	BRLO _0x46B
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x46C
	__GETD1N 0xFF7FFFFF
	RJMP _0x481
_0x46C:
	__GETD1N 0x7F7FFFFF
	RJMP _0x481
_0x46B:
	LDI  R21,LOW(32)
	CALL SUBOPT_0x27
	CALL SUBOPT_0x123
_0x46D:
	CPI  R21,0
	BREQ _0x46F
	CALL SUBOPT_0x11E
	CALL SUBOPT_0x120
	CALL __MULF12
	CALL SUBOPT_0x123
	MOV  R30,R20
	AND  R30,R21
	BREQ _0x470
	CALL SUBOPT_0x122
	CALL SUBOPT_0x123
_0x470:
	LSR  R21
	RJMP _0x46D
_0x46F:
	LDD  R30,Y+6
	CPI  R30,0
	BREQ _0x471
	CALL SUBOPT_0x11E
	CALL SUBOPT_0x12D
	CALL __DIVF21
	RJMP _0x4C9
_0x471:
	CALL SUBOPT_0x11E
	CALL SUBOPT_0x12D
	CALL __MULF12
_0x4C9:
	__PUTD1S 8
_0x462:
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x473
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x7D
_0x473:
	CALL SUBOPT_0x83
_0x481:
	CALL __LOADLOCR6
	ADIW R28,18
	RET
_log:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x54
	CALL __CPD02
	BRLT _0x474
	__GETD1N 0xFF7FFFFF
	RJMP _0x480
_0x474:
	RCALL SUBOPT_0x65
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
	RCALL SUBOPT_0x61
	RCALL SUBOPT_0x54
	__GETD1N 0x3F3504F3
	CALL __CMPF12
	BRSH _0x475
	RCALL SUBOPT_0x55
	RCALL SUBOPT_0x54
	RCALL SUBOPT_0x53
	__SUBWRN 16,17,1
_0x475:
	RCALL SUBOPT_0x62
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x55
	__GETD2N 0x3F800000
	CALL __ADDF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	RCALL SUBOPT_0x86
	RCALL SUBOPT_0x54
	CALL SUBOPT_0x117
	RCALL SUBOPT_0x56
	__GETD2N 0x3F654226
	RCALL SUBOPT_0x2A
	__GETD1N 0x4054114E
	RCALL SUBOPT_0x2C
	RCALL SUBOPT_0x54
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x52
	__GETD1N 0x3FD4114D
	RCALL SUBOPT_0x2C
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
_0x480:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,10
	RET
_exp:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x6E
	__GETD1N 0xC2AEAC50
	CALL __CMPF12
	BRSH _0x476
	RCALL SUBOPT_0x7C
	RJMP _0x47F
_0x476:
	RCALL SUBOPT_0x4C
	CALL __CPD10
	BRNE _0x477
	RCALL SUBOPT_0x27
	RJMP _0x47F
_0x477:
	RCALL SUBOPT_0x6E
	__GETD1N 0x42B17218
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x478
	__GETD1N 0x7F7FFFFF
	RJMP _0x47F
_0x478:
	RCALL SUBOPT_0x6E
	__GETD1N 0x3FB8AA3B
	CALL __MULF12
	RCALL SUBOPT_0x4A
	RCALL SUBOPT_0x4C
	CALL __PUTPARD1
	CALL _floor
	CALL __CFD1
	MOVW R16,R30
	MOVW R30,R16
	RCALL SUBOPT_0x6E
	CALL __CWD1
	CALL __CDF1
	RCALL SUBOPT_0x2C
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F000000
	RCALL SUBOPT_0x2C
	RCALL SUBOPT_0x86
	RCALL SUBOPT_0x54
	CALL SUBOPT_0x117
	RCALL SUBOPT_0x56
	__GETD2N 0x3D6C4C6D
	CALL __MULF12
	__GETD2N 0x40E6E3A6
	CALL __ADDF12
	RCALL SUBOPT_0x54
	CALL __MULF12
	RCALL SUBOPT_0x61
	RCALL SUBOPT_0x56
	__GETD2N 0x41A68D28
	CALL __ADDF12
	RCALL SUBOPT_0x72
	RCALL SUBOPT_0x55
	RCALL SUBOPT_0x52
	CALL __ADDF12
	__GETD2N 0x3FB504F3
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x54
	RCALL SUBOPT_0x56
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
_0x47F:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
_pow:
	SBIW R28,4
	RCALL SUBOPT_0x83
	CALL __CPD10
	BRNE _0x479
	RCALL SUBOPT_0x7C
	RJMP _0x47E
_0x479:
	CALL SUBOPT_0x12D
	CALL __CPD02
	BRGE _0x47A
	RCALL SUBOPT_0x1
	CALL __CPD10
	BRNE _0x47B
	RCALL SUBOPT_0x27
	RJMP _0x47E
_0x47B:
	RCALL SUBOPT_0x83
	RCALL SUBOPT_0x69
	CALL SUBOPT_0x124
	RCALL SUBOPT_0x7F
	RJMP _0x47E
_0x47A:
	RCALL SUBOPT_0x1
	MOVW R26,R28
	CALL __CFD1
	CALL __PUTDP1
	RCALL SUBOPT_0x7
	CALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x1
	CALL __CPD12
	BREQ _0x47C
	RCALL SUBOPT_0x7C
	RJMP _0x47E
_0x47C:
	CALL SUBOPT_0x12E
	RCALL SUBOPT_0x69
	CALL SUBOPT_0x124
	RCALL SUBOPT_0x7F
	RCALL SUBOPT_0x7D
	LD   R30,Y
	ANDI R30,LOW(0x1)
	BRNE _0x47D
	RCALL SUBOPT_0x83
	RJMP _0x47E
_0x47D:
	CALL SUBOPT_0x12E
_0x47E:
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 68 TIMES, CODE SIZE REDUCTION:131 WORDS
SUBOPT_0x0:
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x1:
	__GETD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x2:
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x3:
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x4:
	CALL __PUTPARD1
	JMP  _dbgout

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:53 WORDS
SUBOPT_0x5:
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,LOW(20000)
	LDI  R31,HIGH(20000)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6:
	__GETD2S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x7:
	__GETD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:41 WORDS
SUBOPT_0x8:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_namebuf)
	SBCI R31,HIGH(-_namebuf)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x9:
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
	RJMP SUBOPT_0x8

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xA:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R17,R30
	CPI  R17,255
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xB:
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(2)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC:
	__POINTW2MN _tprbdata,576
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD:
	MOVW R30,R12
	ADIW R30,1
	ST   -Y,R30
	JMP  _scanner_set_channel

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE:
	__GETD1N 0x186A0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:57 WORDS
SUBOPT_0xF:
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R31
	STS  _dlg_cnt+2,R22
	STS  _dlg_cnt+3,R23
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x10:
	MOVW R30,R12
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x11:
	__POINTW1FN _0,33
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R12
	ADIW R30,1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x12:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x13:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x14:
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x15:
	__POINTW1FN _0,41
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0x14

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x16:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x17:
	LDI  R30,LOW(254)
	ST   -Y,R30
	CALL _pgmain_handler
	LDI  R30,LOW(0)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x18:
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(5)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x19:
	__POINTW2MN _rprbdata,576
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x1A:
	LDI  R30,LOW(105)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1B:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x1C:
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	__GETD2N 0x186A0
	CALL __CWD1
	CALL __MULD12
	RJMP SUBOPT_0xF

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1D:
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x1E:
	CALL _nav_read
	CALL __PUTPARD1
	JMP  _mabs

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1F:
	__PUTD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x20:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x21:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x22:
	STS  _valuep,R30
	STS  _valuep+1,R31
	STS  _valuep+2,R22
	STS  _valuep+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:51 WORDS
SUBOPT_0x23:
	LDS  R26,_valuep
	LDS  R27,_valuep+1
	LDS  R24,_valuep+2
	LDS  R25,_valuep+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x24:
	RCALL SUBOPT_0x23
	__GETD1N 0x3E570A3D
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x25:
	__GETD1N 0x40000000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x26:
	RCALL SUBOPT_0x23
	__GETD1N 0x3DD70A3D
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 19 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x27:
	__GETD1N 0x3F800000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x28:
	__GETD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x29:
	__POINTW2MN _sysdata,8
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x2A:
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x2B:
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 25 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x2C:
	CALL __SWAPD12
	CALL __SUBF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x2D:
	__GETD1MN _rundata,192
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2E:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 42 TIMES, CODE SIZE REDUCTION:161 WORDS
SUBOPT_0x2F:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x30:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:28 WORDS
SUBOPT_0x31:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R12
	ADIW R30,1
	CALL __CWD1
	CALL __PUTPARD1
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	RCALL SUBOPT_0xE
	CALL __DIVD21U
	CALL __PUTPARD1
	MOVW R30,R28
	ADIW R30,12
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	LDI  R24,12
	CALL _sprintf
	ADIW R28,16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 48 TIMES, CODE SIZE REDUCTION:185 WORDS
SUBOPT_0x32:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x33:
	LDI  R30,LOW(_databuf)
	LDI  R31,HIGH(_databuf)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x34:
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x35:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x36:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _wnd_msgbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x37:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	RJMP SUBOPT_0x20

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x38:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R10,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x39:
	LDI  R30,LOW(_pgmain_handler)
	LDI  R31,HIGH(_pgmain_handler)
	LDS  R26,_curr_window
	LDS  R27,_curr_window+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:30 WORDS
SUBOPT_0x3A:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x3B:
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:42 WORDS
SUBOPT_0x3C:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x3D:
	__POINTW2MN _sysdata,14
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x3E:
	__POINTW1MN _tprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3F:
	__SUBD1N -1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x40:
	LDS  R26,_onesec_cnt
	LDS  R27,_onesec_cnt+1
	LDS  R24,_onesec_cnt+2
	LDS  R25,_onesec_cnt+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x41:
	__POINTW1MN _rprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x42:
	CALL __MULW12U
	ADD  R30,R22
	ADC  R31,R23
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x43:
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
SUBOPT_0x44:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 28 TIMES, CODE SIZE REDUCTION:51 WORDS
SUBOPT_0x45:
	SUBI R30,LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x46:
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
SUBOPT_0x47:
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
SUBOPT_0x48:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	STD  Y+4,R26
	STD  Y+4+1,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x49:
	__PUTD1S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0x4A:
	__PUTD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x4B:
	__GETD2S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x4C:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x4D:
	CALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3851B717
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x4E:
	MOVW R26,R16
	__ADDWRN 16,17,1
	CPI  R26,LOW(0x64)
	LDI  R30,HIGH(0x64)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x4F:
	RCALL SUBOPT_0x4C
	RCALL SUBOPT_0x4B
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x25
	CALL __DIVF21
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x50:
	__GETD2S 30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x51:
	CALL __MULF12
	__GETD2S 2
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x52:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x53:
	CALL __ADDF12
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 25 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x54:
	__GETD2S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x55:
	__GETD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x56:
	__GETD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x57:
	RCALL SUBOPT_0x56
	RJMP SUBOPT_0x49

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x58:
	RCALL SUBOPT_0x4B
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x59:
	__GETD2S 10
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x5A:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x25
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	CALL _floor
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x461C4000
	CALL __DIVF21
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x5B:
	LDI  R31,0
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x5C:
	__POINTW2MN _rprbdata,576
	LDD  R30,Y+30
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5D:
	__GETD1S 31
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5E:
	__GETD2S 31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5F:
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x27
	RJMP SUBOPT_0x2C

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x60:
	RCALL SUBOPT_0x4B
	RCALL SUBOPT_0x4C
	RJMP SUBOPT_0x4D

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x61:
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x62:
	RCALL SUBOPT_0x54
	RCALL SUBOPT_0x27
	RJMP SUBOPT_0x2C

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x63:
	__GETD2S 26
	CALL __MULF12
	RCALL SUBOPT_0x54
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x64:
	__GETD2S 22
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x65:
	RCALL SUBOPT_0x55
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x66:
	RCALL SUBOPT_0x56
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x67:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	__GETD2S 10
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x68:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x69:
	CALL __PUTPARD1
	RJMP _log

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6A:
	__PUTD1S 2
	RJMP SUBOPT_0x66

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x6B:
	MOVW R26,R16
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x6C:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	__GETD2S 10
	CALL __MULF12
	RCALL SUBOPT_0x54
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6D:
	RCALL SUBOPT_0x56
	__GETD2S 10
	CALL __MULF12
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x6E:
	__GETD2S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6F:
	CALL __DIVF21
	RCALL SUBOPT_0x61
	RCALL SUBOPT_0x55
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x70:
	CALL __GETD1PF
	__PUTD1S 2
	__GETWRN 16,17,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x71:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RCALL SUBOPT_0x6E
	CALL __MULF12
	RCALL SUBOPT_0x52
	CALL __ADDF12
	__PUTD1S 2
	RCALL SUBOPT_0x55
	RCALL SUBOPT_0x6E
	CALL __MULF12
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x72:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x73:
	__GETD2S 13
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x74:
	STS  _coef,R30
	STS  _coef+1,R31
	RJMP SUBOPT_0x73

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x75:
	__GETD2S 13
	__GETD1N 0x441DA75C
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x76:
	STS  _coef,R30
	STS  _coef+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x77:
	__GETD2S 13
	__GETD1N 0x443E0000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x78:
	__GETD2S 13
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x79:
	__GETD1N 0x448505C3
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x7A:
	RCALL SUBOPT_0x78
	__GETD1N 0x44D01000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x7B:
	RCALL SUBOPT_0x76
	RCALL SUBOPT_0x78
	RJMP SUBOPT_0x79

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x7C:
	__GETD1N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x7D:
	__PUTD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x7E:
	RCALL SUBOPT_0x78
	__GETD1N 0x42FDEFEC
	RJMP SUBOPT_0x2C

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x7F:
	CALL __MULF12
	CALL __PUTPARD1
	RJMP _exp

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x80:
	CALL __MULF12
	__GETD2S 8
	CALL __ADDF12
	RJMP SUBOPT_0x7D

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x81:
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x82:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x83:
	__GETD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x84:
	__GETD1N 0xC3870000
	RJMP SUBOPT_0x49

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x85:
	__GETD1N 0xC2480000
	RCALL SUBOPT_0x49
	__GETD1N 0x44DD0000
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x86:
	RCALL SUBOPT_0x61
	RJMP SUBOPT_0x55

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x87:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x88:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_lcdcmd_G8)
	SBCI R31,HIGH(-_lcdcmd_G8)
	LD   R30,Z
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x89:
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
SUBOPT_0x8A:
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _onecmd
	__CLRD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x8B:
	LDI  R30,LOW(3)
	STS  _lcdcmd_G8,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _sendcmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x8C:
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
SUBOPT_0x8D:
	MOV  R30,R17
	LDI  R31,0
	__ADDW1MN _lcdcmd_G8,3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x8E:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x8F:
	LDI  R26,LOW(10)
	STD  Z+0,R26
	LDI  R30,LOW(_scancmd)
	LDI  R31,HIGH(_scancmd)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x90:
	LDI  R26,LOW(_scanner_type)
	LDI  R27,HIGH(_scanner_type)
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x91:
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __MODB21U
	ST   Y,R30
	SUBI R30,-LOW(48)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x92:
	__POINTW1MN _scancmd,3
	LDI  R26,LOW(13)
	STD  Z+0,R26
	__POINTW1MN _scancmd,4
	RJMP SUBOPT_0x8F

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x93:
	LDI  R30,LOW(_navcmd_G9)
	LDI  R31,HIGH(_navcmd_G9)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x94:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	__GETD1N 0xC47A0000
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RJMP SUBOPT_0x93

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x95:
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
SUBOPT_0x96:
	__GETD1N 0xC47A0000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x97:
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x98:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	RCALL SUBOPT_0x96
	RJMP SUBOPT_0x97

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x99:
	LDS  R30,_navlen_G9
	LDI  R31,0
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x9A:
	CALL __PUTPARD1
	JMP  _wnd_floatinput

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x9B:
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x9C:
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R30,R10
	CPC  R31,R11
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x9D:
	MOVW R30,R10
	MOV  R18,R30
	MOVW R26,R10
	SBIW R26,1
	MOVW R30,R16
	CALL __MODW21
	__SUBWRR 10,11,30,31
	LDI  R19,LOW(0)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x9E:
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x9F:
	__POINTW1FN _0,231
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	CALL __CWD1
	CALL __PUTPARD1
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xA0:
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	STD  Y+6,R30
	STD  Y+6+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA1:
	__POINTW1FN _0,239
	RJMP SUBOPT_0x2F

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xA2:
	MOVW R26,R10
	SBIW R26,1
	CP   R12,R26
	CPC  R13,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xA3:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_values*2)
	SBCI R31,HIGH(-_pgmain_values*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xA4:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xA5:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xA6:
	__POINTW1FN _0,250
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	SBIW R30,1
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RJMP SUBOPT_0x12

;OPTIMIZER ADDED SUBROUTINE, CALLED 29 TIMES, CODE SIZE REDUCTION:53 WORDS
SUBOPT_0xA7:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xA8:
	__POINTW1MN _strbuf,8
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA9:
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xAA:
	__POINTW2MN _rundata,96
	MOVW R30,R10
	SBIW R30,1
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xAB:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0xC60CA000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xAC:
	__POINTW1FN _0,272
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0xAA

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0xAD:
	__POINTW1FN _0,277
	ST   -Y,R31
	ST   -Y,R30
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0xA7

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xAE:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	CALL __GETW1PF
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xAF:
	LDI  R30,LOW(221)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x42

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0xB0:
	SBIW R30,2
	ST   -Y,R30
	LDI  R30,LOW(233)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x42

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xB1:
	__POINTW2MN _sysdata,38
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB2:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgr_options*2)
	SBCI R31,HIGH(-_pgr_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xB3:
	CALL __CWD1
	CALL __PUTPARD1
	RJMP SUBOPT_0xA7

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB4:
	STS  _max_databuf,R30
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0xB5:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB6:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgt_options*2)
	SBCI R31,HIGH(-_pgt_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xB7:
	CALL __SAVELOCR6
	LDI  R16,6
	LDI  R17,1
	LDI  R18,24
	LDI  R19,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB8:
	LDS  R30,_curr_pos_S4D
	LDS  R31,_curr_pos_S4D+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB9:
	STS  _last_pos_S4D,R30
	STS  _last_pos_S4D+1,R31
	RJMP SUBOPT_0xB8

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xBA:
	STS  _curr_pos_S4D,R30
	STS  _curr_pos_S4D+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xBB:
	LDS  R26,_curr_pos_S4D
	LDS  R27,_curr_pos_S4D+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBC:
	MOV  R30,R18
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xBD:
	RCALL SUBOPT_0xB8
	LDS  R26,_last_pos_S4D
	LDS  R27,_last_pos_S4D+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xBE:
	LDS  R26,_curr_pos_S4D
	LDS  R27,_curr_pos_S4D+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBF:
	MOV  R30,R19
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC0:
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(255)
	CALL __EEPROMWRB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC1:
	__POINTW1FN _0,522
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xC2:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC3:
	MOV  R30,R17
	LDI  R31,0
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC4:
	RCALL SUBOPT_0xBE
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xC5:
	CALL __DIVW21U
	MOVW R26,R30
	MOV  R30,R16
	LDI  R31,0
	CALL __MULW12U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC6:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,LOW(0xFF)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC7:
	__POINTW1FN _0,525
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0xB3

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xC8:
	__POINTW1FN _0,532
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC9:
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCA:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCB:
	CALL _rname2b
	RJMP SUBOPT_0xC2

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xCC:
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
SUBOPT_0xCD:
	CALL __MODW21U
	MOVW R26,R30
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xCE:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xCF:
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xD0:
	STS  _curr_pos_S4E,R30
	STS  _curr_pos_S4E+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD1:
	LDS  R30,_curr_pos_S4E
	LDS  R31,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD2:
	STS  _last_pos_S4E,R30
	STS  _last_pos_S4E+1,R31
	RJMP SUBOPT_0xD1

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD3:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD4:
	RCALL SUBOPT_0xD1
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD5:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD6:
	RCALL SUBOPT_0xD5
	RJMP SUBOPT_0xCF

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD7:
	__POINTW1FN _0,559
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+11
	LDD  R31,Y+11+1
	CALL __CWD1
	CALL __PUTPARD1
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,1
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xD8:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x32

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD9:
	CALL __MODW21U
	MOVW R26,R30
	MOVW R30,R20
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xDA:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0xB5

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xDB:
	LDS  R30,_curr_pos_S4F
	LDS  R31,_curr_pos_S4F+1
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xDC:
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xDD:
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RJMP SUBOPT_0xDB

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xDE:
	STS  _curr_pos_S4F,R30
	STS  _curr_pos_S4F+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xDF:
	LDS  R30,_curr_pos_S4F
	LDS  R31,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xE0:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE1:
	RCALL SUBOPT_0xDF
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE2:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE3:
	RCALL SUBOPT_0xE2
	RJMP SUBOPT_0xCF

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xE4:
	__POINTW1FN _0,537
	ST   -Y,R31
	ST   -Y,R30
	MOV  R30,R20
	SUBI R30,LOW(1)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE5:
	CALL __PUTPARD1
	RJMP SUBOPT_0xA7

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xE6:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0x32

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE7:
	CALL __MODW21U
	MOVW R26,R30
	MOV  R30,R21
	RJMP SUBOPT_0x45

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xE8:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0xB5

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xE9:
	LDI  R31,0
	STS  _curr_sel_S50,R30
	STS  _curr_sel_S50+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xEA:
	LDS  R30,_curr_sel_S50
	LDS  R31,_curr_sel_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xEB:
	__POINTW2MN _tprbdata,576
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xEC:
	__POINTW2MN _rprbdata,576
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xED:
	__POINTW2MN _rprbdata,288
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0xEE:
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xEF:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R8
	RJMP SUBOPT_0xB3

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xF0:
	MOV  R30,R19
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_tplist_options*2)
	SBCI R31,HIGH(-_tplist_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF1:
	RCALL SUBOPT_0xEA
	STS  _last_sel_S50,R30
	STS  _last_sel_S50+1,R31
	RJMP SUBOPT_0xEA

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF2:
	LDS  R26,_curr_sel_S50
	LDS  R27,_curr_sel_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xF3:
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF4:
	__POINTW1FN _0,536
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF5:
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF6:
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	RJMP SUBOPT_0xC2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xF7:
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0xEE

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xF8:
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	RJMP SUBOPT_0xEE

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF9:
	__POINTW2MN _rprbdata,96
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xFA:
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0x82

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xFB:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_boot_options*2)
	SBCI R31,HIGH(-_boot_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xFC:
	LDI  R30,LOW(_databuf)
	LDI  R31,HIGH(_databuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xFD:
	LDI  R30,LOW(0)
	STS  _databuf,R30
	STS  _pos_databuf,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFE:
	CALL _LCD_Cls
	LDI  R30,LOW(_datalbl*2)
	LDI  R31,HIGH(_datalbl*2)
	RJMP SUBOPT_0x32

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFF:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _draw_inputbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0x100:
	LDS  R30,_pos_databuf
	SUBI R30,-LOW(1)
	STS  _pos_databuf,R30
	RJMP SUBOPT_0x45

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x101:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x102:
	LDS  R30,_pos_databuf
	RJMP SUBOPT_0x45

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x103:
	LDS  R30,_max_databuf
	LDS  R26,_pos_databuf
	CP   R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x104:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x105:
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CLR  R30
	ADD  R26,R16
	ADC  R27,R30
	MOV  R30,R16
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x106:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	RJMP SUBOPT_0x32

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:38 WORDS
SUBOPT_0x107:
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
SUBOPT_0x108:
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,2
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x109:
	ST   -Y,R30
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x10A:
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
SUBOPT_0x10B:
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x10C:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:67 WORDS
SUBOPT_0x10D:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x10E:
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	MOV  R30,R17
	LSL  R30
	LSL  R30
	LSL  R30
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	ST   -Y,R30
	LDD  R30,Y+5
	LDD  R31,Y+5+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x10F:
	ADIW R30,8
	CALL __GETW1PF
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x110:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x111:
	ADIW R30,2
	CALL __GETW1PF
	MOVW R26,R30
	MOV  R30,R17
	SWAP R30
	ANDI R30,0xF0
	LDI  R31,0
	ADD  R30,R26
	ADC  R31,R27
	ST   -Y,R30
	LDD  R30,Y+5
	LDD  R31,Y+5+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x112:
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
SUBOPT_0x113:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x114:
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x115:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x116:
	RCALL SUBOPT_0x52
	__GETD1N 0x41200000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x117:
	CALL __MULF12
	RJMP SUBOPT_0x72

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x118:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x119:
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	JMP  _floor

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x11A:
	MOV  R30,R16
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x11B:
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x11C:
	__GETD1N 0x41200000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x11D:
	__GETD2S 4
	RJMP SUBOPT_0x11C

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x11E:
	__GETD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x11F:
	CALL SUBOPT_0x1
	__GETD2S 12
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x120:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x121:
	__GETD1N 0x41200000
	CALL __DIVF21
	__PUTD1S 12
	SUBI R19,-LOW(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x122:
	RCALL SUBOPT_0x120
	RJMP SUBOPT_0x11C

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x123:
	__PUTD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x124:
	__GETD2S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x125:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADIW R26,1
	STD  Y+8,R26
	STD  Y+8+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x126:
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
SUBOPT_0x127:
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
SUBOPT_0x128:
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
SUBOPT_0x129:
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ADIW R30,1
	STD  Y+10,R30
	STD  Y+10+1,R31
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x12A:
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x12B:
	ADIW R30,4
	MOVW R26,R30
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x12C:
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
SUBOPT_0x12D:
	__GETD2S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x12E:
	RCALL SUBOPT_0x83
	CALL __ANEGF1
	RET


	.DSEG
_3:
	.BYTE 0x0C
_4:
	.BYTE 0x10
_555:
	.BYTE 0x0F
_556:
	.BYTE 0x10
_557:
	.BYTE 0x0B
_558:
	.BYTE 0x0D
_559:
	.BYTE 0x0F
_560:
	.BYTE 0x0F
_582:
	.BYTE 0x0F
_583:
	.BYTE 0x11
_584:
	.BYTE 0x11
_602:
	.BYTE 0x11
_644:
	.BYTE 0x13
_720:
	.BYTE 0x08
_721:
	.BYTE 0x07
_722:
	.BYTE 0x09
_723:
	.BYTE 0x09
_724:
	.BYTE 0x09
_725:
	.BYTE 0x09
_726:
	.BYTE 0x09
_727:
	.BYTE 0x09
_728:
	.BYTE 0x09
_777:
	.BYTE 0x09
_778:
	.BYTE 0x07
_779:
	.BYTE 0x03
_780:
	.BYTE 0x03
_781:
	.BYTE 0x03
_782:
	.BYTE 0x03
_803:
	.BYTE 0x0F
_804:
	.BYTE 0x0D
_805:
	.BYTE 0x0D
_806:
	.BYTE 0x0D
_807:
	.BYTE 0x0D
_831:
	.BYTE 0x0E
_832:
	.BYTE 0x0E

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

__MODW21:
	CLT
	SBRS R27,7
	RJMP __MODW211
	COM  R26
	COM  R27
	ADIW R26,1
	SET
__MODW211:
	SBRC R31,7
	RCALL __ANEGW1
	RCALL __DIVW21U
	MOVW R30,R26
	BRTC __MODW212
	RCALL __ANEGW1
__MODW212:
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
