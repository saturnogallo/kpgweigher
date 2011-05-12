
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
;      70 	{"11","","","","","","","",
;      71 	"","","","","","","","",
;      72 	"","","","","","","",""},
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
;      73        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      74 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      75 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
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
;      76 };
;      77 PRBDATA	eeprom rprbdata = {
_rprbdata:
;      78        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      79 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
;      80 	0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
	.DW  0x0
;      81 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,
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
;      87 	{"11","","","","","","","",
;      88 	"","","","","","","","",
;      89 	"","","","","","","",""},
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
;      90        {0x03,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      91 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
;      92 	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}
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
;      93 };
;      94 
;      95 char  strbuf[50];        //buffer for menu display

	.DSEG
_strbuf:
	.BYTE 0x32
;      96 uchar key;
;      97 
;      98 
;      99 
;     100 LABEL flash bootup = {LBL_HZ16,30,30,7,"正在启动..."};

	.CSEG
;     101 LABEL flash modify = {LBL_HZ16,30,30,8,"正在更新数据..."};
;     102 
;     103 LABEL flash lbldbg = {LBL_HZ6X8,10,30,8,strbuf};
;     104 
;     105 uchar nextwin = 0;
;     106 
;     107 int   curr_ch = 1;	//index of current channel in config menu window, 1 based

	.DSEG
;     108 int   curr_prb = 1;	//index of current probe selection in menu window 1 based
;     109 int   curr_dispch = 1;  //index of start channel in main running window, 1 based
;     110 int ch_to_search = 0;   //current channel , 0 based
;     111 
;     112 unsigned long dlg_cnt = 0;        //delay count time display
_dlg_cnt:
	.BYTE 0x4
;     113 unsigned long onesec_cnt = 0;
_onesec_cnt:
	.BYTE 0x4
;     114 uchar phase = 0;       //state machine index
_phase:
	.BYTE 0x1
;     115 double valuep;
_valuep:
	.BYTE 0x4
;     116 
;     117 /**************************************************************************************/
;     118 //                      Timer 1 overflow interrupt service routine
;     119 /**************************************************************************************/
;     120 interrupt [TIM1_OVF] void timer1_ovf_isr(void)
;     121 {

	.CSEG
_timer1_ovf_isr:
;     122 // Place your flash here
;     123 }
	RETI
;     124 void dbgout(double val)
;     125 {
_dbgout:
;     126         sprintf(strbuf,"%f\r\n",val);
;	val -> Y+0
	CALL SUBOPT_0x0
	__POINTW1FN _0,28
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x1
	CALL SUBOPT_0x2
;     127         prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     128 }
	RJMP _0x498
;     129 void InitLED()
;     130 {
_InitLED:
;     131 /*
;     132     PORTB.7=1;
;     133     PORTB.6=1;
;     134     PORTB.5=1;
;     135     PORTB.4=1;
;     136     PORTD.7=1;
;     137     PORTD.6=0;
;     138 */
;     139 }
	RET
;     140 uchar nav1v = 1;

	.DSEG
_nav1v:
	.BYTE 0x1
;     141 void navto120mv()
;     142 {

	.CSEG
_navto120mv:
;     143    nav1v = 0;
	LDI  R30,LOW(0)
	STS  _nav1v,R30
;     144    dbgout(-0.12);
	__GETD1N 0xBDF5C28F
	CALL SUBOPT_0x4
;     145    nav_command(NAV_120MV);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x5
;     146    sleepms(20000);
;     147 }
	RET
;     148 
;     149 void navto1v()
;     150 {
_navto1v:
;     151     nav1v = 1;
	LDI  R30,LOW(1)
	STS  _nav1v,R30
;     152     dbgout(-1.0);
	__GETD1N 0xBF800000
	CALL SUBOPT_0x4
;     153     nav_command(NAV_INIT);
	LDI  R30,LOW(1)
	CALL SUBOPT_0x5
;     154     sleepms(20000);
;     155     nav_command(NAV_1V);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x5
;     156     sleepms(20000);
;     157     nav_command(NAV_SLOWMODE);
	LDI  R30,LOW(7)
	CALL SUBOPT_0x5
;     158     sleepms(20000);
;     159     nav_command(NAV_AFLTON);
	LDI  R30,LOW(6)
	CALL SUBOPT_0x5
;     160     sleepms(20000);
;     161 }
	RET
;     162 double mabs(double val)
;     163 {
_mabs:
;     164         if(val < 0)
;	val -> Y+0
	__GETD2S 0
	CALL __CPD20
	BRGE _0x9
;     165                 return -val;
	CALL SUBOPT_0x6
	CALL __ANEGF1
	RJMP _0x498
;     166         else
_0x9:
;     167                 return val;
	CALL SUBOPT_0x6
;     168 }
_0x498:
	ADIW R28,4
	RET
;     169 u8 ToByte(char l, char h)
;     170 {
;     171 	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
;	l -> Y+1
;	h -> Y+0
;     172 	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
;     173 	return (h<<4)|l;
;     174 }
;     175 
;     176 /**************************************************************************************/
;     177 //                              Global Variable
;     178 /**************************************************************************************/
;     179 
;     180 /**************************************************************************************/
;     181 //                               Watchdog
;     182 // Enable watchdog timer.
;     183 // Timeout limit WDTCR[2:0]:
;     184 // 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
;     185 // 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
;     186 /**************************************************************************************/
;     187 #define WDCE 4
;     188 #define WDE  3
;     189 
;     190 
;     191 
;     192 void Turn_on_Watchdog()
;     193 {
;     194   #asm("cli")
;     195   #asm("wdr")                    // reset watchdog timer to avoid timeout reset
;     196   WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
;     197   WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
;     198   #asm("sei")                    // Enable global interrupt.
;     199 }
;     200 
;     201 char namebuf[8];

	.DSEG
_namebuf:
	.BYTE 0x8
;     202 char* tname2b(u8 i)
;     203 {

	.CSEG
_tname2b:
;     204         u8 j=0;
;     205         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x12:
	CPI  R17,8
	BRSH _0x13
;     206         {
;     207             namebuf[j] = tprbdata.name[i][j];
	CALL SUBOPT_0x7
	MOVW R0,R30
	__POINTW2MN _tprbdata,288
	CALL SUBOPT_0x8
;     208             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x13
;     209                 break;
;     210 
;     211             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x16
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x18
_0x16:
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x19
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x18
_0x19:
	RJMP _0x15
_0x18:
;     212                 continue;
	RJMP _0x11
;     213             namebuf[0] = '\0';
_0x15:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     214             break;
	RJMP _0x13
;     215         }
_0x11:
	SUBI R17,-1
	RJMP _0x12
_0x13:
;     216         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x497
;     217 }
;     218 
;     219 char* rname2b(u8 i)
;     220 {
_rname2b:
;     221         u8 j=0;
;     222         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x1D:
	CPI  R17,8
	BRSH _0x1E
;     223         {
;     224             namebuf[j] = rprbdata.name[i][j];
	CALL SUBOPT_0x7
	MOVW R0,R30
	__POINTW2MN _rprbdata,288
	CALL SUBOPT_0x8
;     225             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x1E
;     226                 break;
;     227 
;     228             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x21
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x23
_0x21:
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x24
	CALL SUBOPT_0x7
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x23
_0x24:
	RJMP _0x20
_0x23:
;     229                 continue;
	RJMP _0x1C
;     230             namebuf[0] = '\0';
_0x20:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     231             break;
	RJMP _0x1E
;     232         }
_0x1C:
	SUBI R17,-1
	RJMP _0x1D
_0x1E:
;     233         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x497
;     234 }
;     235 void sleepms(u16 ms)
;     236 {
_sleepms:
;     237     while(ms-- > 0)
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
;     238         ;
;     239 }
_0x497:
	ADIW R28,2
	RET
;     240 
;     241 
;     242 /**************************************************************************************/
;     243 //      board init related function.
;     244 /**************************************************************************************/
;     245 void init_var()
;     246 {
_init_var:
;     247 }
	RET
;     248 //state machine of therm type
;     249 //phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
;     250 //phase 1:get the reading and update the display
;     251 //phase 2: move to next channel
;     252 uchar therm_state()
;     253 {
_therm_state:
;     254         uchar i;
;     255 	i = sysdata.tid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	__POINTW1MN _sysdata,14
	CALL SUBOPT_0x9
;     256 	if(i == INVALID_PROBE)
	BRNE _0x2A
;     257 	{
;     258         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0xA
;     259         	phase = 2;
;     260 	}
;     261 	if(phase == 0)
_0x2A:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x2B
;     262 	{
;     263 		if( (tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0xB
	CPI  R30,LOW(0x3)
	BRLO _0x2D
	CALL SUBOPT_0xB
	CPI  R30,LOW(0xA)
	BRLO _0x2E
_0x2D:
	RJMP _0x2C
_0x2E:
;     264 		{
;     265 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0xC
;     266 			dlg_cnt = ONESEC;
	CALL SUBOPT_0xD
	CALL SUBOPT_0xE
;     267 			onesec_cnt = 0;
;     268 			phase = 1;
	LDI  R30,LOW(1)
	STS  _phase,R30
;     269 			return 0;
	LDI  R30,LOW(0)
	RJMP _0x496
;     270 		}else{
_0x2C:
;     271 			rundata.temperature[ch_to_search] = -9999; //invalid data
	CALL SUBOPT_0xA
;     272 			phase = 2;
;     273 		}
;     274 	}
;     275 	if(phase == 1)
_0x2B:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x30
;     276 	{
;     277 		rundata.reading[ch_to_search] = nav_read();//-sysdata.V0;
	CALL SUBOPT_0xF
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL _nav_read
	POP  R26
	POP  R27
	CALL __PUTDP1
;     278                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x10
	CALL SUBOPT_0xF
	CALL SUBOPT_0x11
	CALL SUBOPT_0x12
;     279                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     280 
;     281 		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
	CALL SUBOPT_0x13
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF
	CALL SUBOPT_0x11
	CALL SUBOPT_0xB
	ST   -Y,R30
	CALL _MValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     282 //		if(ch_to_search==1) 		     rundata.temperature[ch_to_search] = 10;
;     283                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x14
	CALL SUBOPT_0x15
	CALL SUBOPT_0x2
;     284                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     285 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     286 	}
;     287 	if(phase == 2)
_0x30:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x31
;     288 	{
;     289 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x16
;     290 		phase = 0;
;     291 	}
;     292 	return 1;       //one reading is updated
_0x31:
	LDI  R30,LOW(1)
	RJMP _0x496
;     293 }
;     294 //state machine of bore measure
;     295 //phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
;     296 //phase1 get reading of rs+, set to nktt, add delay
;     297 //phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
;     298 //phase3 get reading of rx-, set to pktt, add delay
;     299 //phase4 get reading of rx+, switch to rx, caculate
;     300 //phase5 update display and move to next channel
;     301 uchar bore_state()
;     302 {
_bore_state:
;     303         uchar i;
;     304 	i = sysdata.rid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	__POINTW1MN _sysdata,38
	CALL SUBOPT_0x9
;     305 	if(i == INVALID_PROBE)
	BRNE _0x32
;     306 	{
;     307         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x13
	CALL SUBOPT_0x17
;     308         	phase = 5;
;     309 	}
;     310 	if(phase == 0)
_0x32:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x33
;     311 	{
;     312 		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0x18
	CPI  R30,LOW(0xF3)
	BRSH _0x35
	CALL SUBOPT_0x18
	CPI  R30,LOW(0xF1)
	BRSH _0x36
_0x35:
	RJMP _0x34
_0x36:
;     313 		{
;     314 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0xC
;     315 			SET_PKTT;
	CALL SUBOPT_0x19
;     316 			SET_TORS;
	CALL SUBOPT_0x1A
;     317 			dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1B
;     318 			onesec_cnt = 0;
;     319 			if(IS_MODE_KTT)
	CALL SUBOPT_0x1C
	BRNE _0x37
;     320 				phase = 1;
	LDI  R30,LOW(1)
	RJMP _0x499
;     321 			else
_0x37:
;     322 				phase = 2;
	LDI  R30,LOW(2)
_0x499:
	STS  _phase,R30
;     323        			return 0;
	LDI  R30,LOW(0)
	RJMP _0x496
;     324 		}else{
_0x34:
;     325 			rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x13
	CALL SUBOPT_0x17
;     326 			phase = 5;
;     327 		}
;     328 
;     329 	}
;     330 	if(phase == 1)  //-stdV in ktt mode
_0x33:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x3A
;     331 	{
;     332 		rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x1E
;     333 		SET_NKTT;
	LDI  R30,LOW(105)
	CALL SUBOPT_0x1F
;     334 		dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1B
;     335 		onesec_cnt = 0;
;     336 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     337 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x496
;     338 	}
;     339 	if(phase == 2) //final stdV
_0x3A:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x3B
;     340 	{
;     341 		if(IS_MODE_KTT)
	CALL SUBOPT_0x1C
	BRNE _0x3C
;     342 		{
;     343 			rundata.stdV = (rundata.stdV + mabs(nav_read()));
	CALL SUBOPT_0x1D
	__GETD2MN _rundata,196
	CALL __ADDF12
	CALL SUBOPT_0x1E
;     344 			phase = 3;
	LDI  R30,LOW(3)
	RJMP _0x49A
;     345 		}else{
_0x3C:
;     346 			rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x1E
;     347 			phase = 4;
	LDI  R30,LOW(4)
_0x49A:
	STS  _phase,R30
;     348 		}
;     349 		SET_TORX;
	CALL SUBOPT_0x20
;     350 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1B
;     351 		onesec_cnt = 0;
;     352 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x496
;     353 	}
;     354 	if(phase == 3)  //-V on rx in ktt mode
_0x3B:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x3E
;     355 	{
;     356 		valuep = mabs(nav_read());
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x21
;     357 		SET_PKTT;
	CALL SUBOPT_0x19
;     358 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x1B
;     359 		onesec_cnt = 0;
;     360 		phase = 4;
	LDI  R30,LOW(4)
	STS  _phase,R30
;     361 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x496
;     362 	}
;     363 	if(phase == 4) // final V on rx
_0x3E:
	LDS  R26,_phase
	CPI  R26,LOW(0x4)
	BREQ PC+3
	JMP _0x3F
;     364 	{
;     365 		if(IS_MODE_KTT){
	CALL SUBOPT_0x1C
	BRNE _0x40
;     366 			valuep = (valuep + mabs(nav_read()));
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x22
	CALL __ADDF12
	CALL SUBOPT_0x21
;     367 			dbgout(valuep);
	LDS  R30,_valuep
	LDS  R31,_valuep+1
	LDS  R22,_valuep+2
	LDS  R23,_valuep+3
	CALL SUBOPT_0x4
;     368 			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
	CALL SUBOPT_0x23
	BREQ PC+2
	BRCC PC+3
	JMP  _0x42
	CALL SUBOPT_0x22
	CALL SUBOPT_0x24
	CALL __CMPF12
	BRSH _0x42
	LDS  R26,_nav1v
	CPI  R26,LOW(0x0)
	BREQ _0x43
_0x42:
	RJMP _0x41
_0x43:
;     369 			{
;     370         		        navto1v();
	CALL _navto1v
;     371 			}
;     372 			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
_0x41:
	CALL SUBOPT_0x23
	BRSH _0x45
	CALL SUBOPT_0x22
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
;     373 			{
;     374 			        navto120mv();
	CALL _navto120mv
;     375 			}
;     376 		}else{
_0x44:
	RJMP _0x47
_0x40:
;     377 			valuep = mabs(nav_read());
	CALL SUBOPT_0x1D
	CALL SUBOPT_0x21
;     378 			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
	CALL SUBOPT_0x25
	BREQ PC+2
	BRCC PC+3
	JMP  _0x49
	CALL SUBOPT_0x22
	CALL SUBOPT_0x26
	CALL __CMPF12
	BRSH _0x49
	LDS  R26,_nav1v
	CPI  R26,LOW(0x0)
	BREQ _0x4A
_0x49:
	RJMP _0x48
_0x4A:
;     379 			{
;     380         		        navto1v();
	CALL _navto1v
;     381 			}
;     382 			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
_0x48:
	CALL SUBOPT_0x25
	BRSH _0x4C
	CALL SUBOPT_0x22
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
;     383 			{
;     384 			        navto120mv();
	CALL _navto120mv
;     385 			}
;     386 		}
_0x4B:
_0x47:
;     387 		if(rundata.stdV != 0)
	CALL SUBOPT_0x27
	CALL __CPD10
	BRNE PC+3
	JMP _0x4E
;     388 		{
;     389 			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
	CALL SUBOPT_0xF
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x28
	CALL SUBOPT_0x22
	CALL SUBOPT_0x29
	CALL SUBOPT_0x27
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2A
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x2B
	POP  R26
	POP  R27
	CALL __PUTDP1
;     390 			if(rundata.reading[ch_to_search] > 0)
	CALL SUBOPT_0xF
	CALL SUBOPT_0x15
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD02
	BRGE _0x4F
;     391 			{
;     392                                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x10
	CALL SUBOPT_0x2C
	CALL __PUTPARD1
	CALL SUBOPT_0x12
;     393                                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     394 
;     395 			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
	CALL SUBOPT_0x13
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF
	CALL SUBOPT_0x11
	ST   -Y,R17
	RCALL _RValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     396                                 rundata.Rx = rundata.reading[ch_to_search];
	CALL SUBOPT_0xF
	CALL SUBOPT_0x15
	__PUTD1MN _rundata,192
;     397                                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x14
	CALL SUBOPT_0x15
	CALL SUBOPT_0x2
;     398                                 prints(strbuf,strlen(strbuf),PORT_PC);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x3
;     399 			}
;     400 		}else{
_0x4F:
	RJMP _0x50
_0x4E:
;     401 		        rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x13
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
;     402 		}
_0x50:
;     403 		phase = 5;
	LDI  R30,LOW(5)
	STS  _phase,R30
;     404 	}
;     405 	if(phase == 5) //got one reading
_0x3F:
	LDS  R26,_phase
	CPI  R26,LOW(0x5)
	BRNE _0x51
;     406 	{
;     407 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x16
;     408 		phase = 0;
;     409 	}
;     410 	return 1;
_0x51:
	LDI  R30,LOW(1)
_0x496:
	LD   R17,Y+
	RET
;     411 }
;     412 LABEL flash statelbl = {LBL_HZ6X8,100,55,16,strbuf};
;     413 
;     414 void updatestate()
;     415 {
_updatestate:
;     416         char star[6];
;     417         sprintf(star,"    ");
	SBIW R28,6
;	star -> Y+0
	CALL SUBOPT_0x2D
	__POINTW1FN _0,47
	CALL SUBOPT_0x2E
;     418         if(phase == 0)                sprintf(star,"*   ");
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x52
	CALL SUBOPT_0x2D
	__POINTW1FN _0,52
	CALL SUBOPT_0x2E
;     419         if(phase == 1)                sprintf(star,"**  ");
_0x52:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x53
	CALL SUBOPT_0x2D
	__POINTW1FN _0,57
	CALL SUBOPT_0x2E
;     420         if(phase == 2)                sprintf(star,"*** ");
_0x53:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x54
	CALL SUBOPT_0x2D
	__POINTW1FN _0,62
	CALL SUBOPT_0x2E
;     421         if(phase == 3)                sprintf(star,"****");
_0x54:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x55
	CALL SUBOPT_0x2D
	__POINTW1FN _0,67
	CALL SUBOPT_0x2E
;     422 
;     423         if(IS_BORE_MODE){
_0x55:
	CALL SUBOPT_0x2F
	BRNE _0x56
;     424                 if(IS_MODE_KTT)
	CALL SUBOPT_0x1C
	BRNE _0x57
;     425                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
;     426                 else
_0x57:
;     427                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
_0x49B:
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	__POINTW1FN _0,72
	CALL SUBOPT_0x30
;     428         }else{
	RJMP _0x59
_0x56:
;     429                 sprintf(strbuf,"(ch:%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
	CALL SUBOPT_0x0
	__POINTW1FN _0,87
	CALL SUBOPT_0x30
;     430         }
_0x59:
;     431         draw_label(&statelbl,SW_NORMAL);
	LDI  R30,LOW(_statelbl*2)
	LDI  R31,HIGH(_statelbl*2)
	CALL SUBOPT_0x31
;     432 }
	ADIW R28,6
	RET
;     433 static uchar tA = 0xff;

	.DSEG
_tA_G1:
	.BYTE 0x1
;     434 static uchar tB = 0xff;
_tB_G1:
	.BYTE 0x1
;     435 void testA(uchar data)
;     436 {

	.CSEG
_testA:
;     437         tA = data;
;	data -> Y+0
	LD   R30,Y
	STS  _tA_G1,R30
;     438 }
	ADIW R28,1
	RET
;     439 void testB(uchar data)
;     440 {
;     441         tB = data;
;	data -> Y+0
;     442 }
;     443 /**************************************************************************************/
;     444 //                              Main Function Loop
;     445 /**************************************************************************************/
;     446 extern double GetThmoVolt(double t,char type);
;     447 extern u8 databuf[12];
;     448 extern u8 pos_databuf; //position in data buffer
;     449 void main(void)
;     450 {
_main:
;     451     u16 i;
;     452     double dt;
;     453 
;     454     /*  just test algrithom
;     455     sprintf(databuf,"9.99");
;     456     pos_databuf = 4;
;     457     dt = buf2double();
;     458     rprbdata.type[0] = PRBTYPE_PT100;
;     459     rprbdata.param1[0] = 3.9083e-3;
;     460     rprbdata.param2[0] = -5.775e-7;
;     461     rprbdata.param3[0] = 100;//-4.183e-12;
;     462     dt = RValueToTValue(139.26, 0);//102
;     463     */
;     464     // RS485 Node
;     465     init_var();	//init data structure
	SBIW R28,4
;	i -> R16,R17
;	dt -> Y+0
	CALL _init_var
;     466     // System Initialization
;     467     Init_Port();
	RCALL _Init_Port
;     468 //    Init_Timers();
;     469 //    Init_Ex_Interrupt();
;     470     Init_UART();
	RCALL _Init_UART
;     471     Enable_XMEM();
	RCALL _Enable_XMEM
;     472     Init_554();
	RCALL _Init_554
;     473     InitLED();
	CALL _InitLED
;     474     Key_Init();
	CALL _Key_Init
;     475     // Global enable interrupts
;     476     WDTCR = 0x00; //disable dog watch
	LDI  R30,LOW(0)
	OUT  0x21,R30
;     477     #asm("sei")
	sei
;     478     /*********************************************************************/
;     479     // System hardware dection
;     480     /*********************************************************************/
;     481     // intialize LED.
;     482     nextwin = 0;
	CLR  R4
;     483 
;     484     sleepms(2000);
	LDI  R30,LOW(2000)
	LDI  R31,HIGH(2000)
	CALL SUBOPT_0x32
;     485     LCD_Init();
	CALL _LCD_Init
;     486     wnd_msgbox(&bootup);
	LDI  R30,LOW(_bootup*2)
	LDI  R31,HIGH(_bootup*2)
	CALL SUBOPT_0x33
;     487     //init the DMM
;     488     nav_command(NAV_INIT);
	LDI  R30,LOW(1)
	CALL SUBOPT_0x5
;     489     sleepms(20000);
;     490     nav_command(NAV_1V);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x5
;     491     sleepms(20000);
;     492     nav_command(NAV_SLOWMODE);
	LDI  R30,LOW(7)
	CALL SUBOPT_0x5
;     493     sleepms(20000);
;     494     nav_command(NAV_AFLTON);
	LDI  R30,LOW(6)
	CALL SUBOPT_0x5
;     495     sleepms(20000);
;     496 
;     497     sleepms(2*ONESEC); //wait until all the node is ready after power up
	LDI  R30,LOW(200000)
	LDI  R31,HIGH(200000)
	CALL SUBOPT_0x32
;     498     State_Init();
	CALL _State_Init
;     499 
;     500     SET_BORE_MODE;
	CALL SUBOPT_0x34
;     501 
;     502     SET_PKTT;
	CALL SUBOPT_0x19
;     503     SET_TORS;
	CALL SUBOPT_0x1A
;     504 
;     505 	 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;     506 	 key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     507 
;     508 	 curr_ch = 1; //channel for display
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R6,R30
;     509 	 curr_dispch = 1;
	CALL SUBOPT_0x35
;     510 	 while(1)
_0x5C:
;     511 	 {
;     512  	 	if(nextwin != 0)
	TST  R4
	BREQ _0x5F
;     513 		{
;     514 			SwitchWindow(nextwin);
	ST   -Y,R4
	RCALL _SwitchWindow
;     515 			(*curr_window)(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	__CALL1MN _curr_window,0
;     516 			nextwin = 0;
	CLR  R4
;     517 		}
;     518 		if(key != KEY_INVALID)
_0x5F:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE PC+3
	JMP _0x60
;     519 		{
;     520 			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
	LDI  R30,LOW(97)
	CP   R30,R5
	BREQ _0x62
	LDI  R30,LOW(98)
	CP   R30,R5
	BREQ _0x62
	LDI  R30,LOW(99)
	CP   R30,R5
	BREQ _0x62
	LDI  R30,LOW(100)
	CP   R30,R5
	BREQ _0x62
	RJMP _0x61
_0x62:
;     521 			{
;     522 				if(curr_window == pgmain_handler)
	CALL SUBOPT_0x36
	BRNE _0x64
;     523 				{
;     524 					LCD_Cls();
	CALL _LCD_Cls
;     525 					wnd_msgbox(&modify);
	LDI  R30,LOW(_modify*2)
	LDI  R31,HIGH(_modify*2)
	CALL SUBOPT_0x33
;     526 				}
;     527 				if(key == KEY_BTN1) //mode switch
_0x64:
	LDI  R30,LOW(97)
	CP   R30,R5
	BRNE _0x65
;     528 				{
;     529 					if(IS_BORE_MODE){
	CALL SUBOPT_0x2F
	BRNE _0x66
;     530 						SET_THERM_MODE;
	CALL SUBOPT_0x37
;     531                                                 SET_TORS;
	CALL SUBOPT_0x1A
;     532                 				SET_PKTT;
	CALL SUBOPT_0x19
;     533                                                 navto120mv();
	CALL _navto120mv
;     534 					}else{
	RJMP _0x67
_0x66:
;     535 						SET_BORE_MODE;
	CALL SUBOPT_0x34
;     536                                                 SET_TORX;
	CALL SUBOPT_0x20
;     537                 				SET_PKTT;
	CALL SUBOPT_0x19
;     538                                                 navto1v();
	CALL _navto1v
;     539 					}
_0x67:
;     540 					dlg_cnt = 0;
	LDI  R30,0
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R30
	STS  _dlg_cnt+2,R30
	STS  _dlg_cnt+3,R30
;     541 					onesec_cnt = 0;
	CALL SUBOPT_0x38
;     542 					phase = 0;      //reset the state machine
	LDI  R30,LOW(0)
	STS  _phase,R30
;     543 
;     544 					SET_PKTT;
	CALL SUBOPT_0x19
;     545 				}
;     546 				if(key == KEY_BTN2) //auto ktt or not
_0x65:
	LDI  R30,LOW(98)
	CP   R30,R5
	BRNE _0x68
;     547 				{
;     548 					if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x69
;     549 					{
;     550 						if((IS_MODE_KTT)){
	CALL SUBOPT_0x1C
	BRNE _0x6A
;     551 							CLR_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(1)
	RJMP _0x49C
;     552 							SET_PKTT;
;     553 						}else{
_0x6A:
;     554 							SET_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(0)
_0x49C:
	ST   -Y,R30
	CALL _display_buttons
;     555 							SET_PKTT;
	CALL SUBOPT_0x19
;     556 						}
;     557 					}
;     558 				}
_0x69:
;     559 				if(key == KEY_BTN3) //thermal probe type
_0x68:
	LDI  R30,LOW(99)
	CP   R30,R5
	BRNE _0x6C
;     560 				{
;     561 
;     562 					display_buttons(KEY_BTN3,1);
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
;     563 					if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x6D
;     564 					{
;     565 					        i = sysdata.tid[curr_dispch-1];
	CALL SUBOPT_0x3A
	MOV  R16,R30
	CLR  R17
;     566 					        if(i != INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x6E
;     567 					        {
;     568                 					if((tprbdata.type[i] >= PRBTYPE_K) &&\
;     569 	                				   (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x3B
	CPI  R30,LOW(0x3)
	BRLO _0x70
	CALL SUBOPT_0x3B
	CPI  R30,LOW(0xA)
	BRLO _0x71
_0x70:
	RJMP _0x6F
_0x71:
;     570 		                			{
;     571 			                			if(tprbdata.type[i] == PRBTYPE_R)
	CALL SUBOPT_0x3B
	CPI  R30,LOW(0x9)
	BRNE _0x72
;     572 				                			tprbdata.type[i] = PRBTYPE_K;
	__POINTW1MN _tprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	LDI  R30,LOW(3)
	RJMP _0x49D
;     573 					                	else
_0x72:
;     574 						                	tprbdata.type[i] +=1;
	CALL SUBOPT_0x3B
	SUBI R30,-LOW(1)
_0x49D:
	CALL __EEPROMWRB
;     575         					        }
;     576         					}
_0x6F:
;     577                                         }
_0x6E:
;     578 					display_buttons(KEY_BTN3,0);
_0x6D:
	LDI  R30,LOW(99)
	CALL SUBOPT_0x1F
;     579 
;     580 				}
;     581 				if(key == KEY_BTN4) //remove zero
_0x6C:
	LDI  R30,LOW(100)
	CP   R30,R5
	BRNE _0x74
;     582 				{
;     583 					display_buttons(KEY_BTN4,1);
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
;     584 					if(IS_BORE_MODE){
	CALL SUBOPT_0x2F
	BRNE _0x75
;     585 					        sysdata.R0 = rundata.Rx;
	CALL SUBOPT_0x2C
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMWRD
;     586 					}else{
	RJMP _0x76
_0x75:
;     587 					        //sysdata.V0 = nav_read();
;     588 					        nav_command(NAV_ZEROON);
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _nav_command
;     589 					        sleepms(ONESEC);
	LDI  R30,LOW(100000)
	LDI  R31,HIGH(100000)
	CALL SUBOPT_0x32
;     590 					}
_0x76:
;     591 					display_buttons(KEY_BTN4,0);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x1F
;     592 				}
;     593 				if(curr_window == pgmain_handler)       //redraw the running window
_0x74:
	CALL SUBOPT_0x36
	BRNE _0x77
;     594 				{
;     595 					pgmain_handler(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _pgmain_handler
;     596 				}
;     597 			}else{
_0x77:
	RJMP _0x78
_0x61:
;     598 				(*curr_window)(key);
	ST   -Y,R5
	__CALL1MN _curr_window,0
;     599 			}
_0x78:
;     600 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     601 	  	}else{
	RJMP _0x79
_0x60:
;     602 			if(curr_window != pgmain_handler)
	CALL SUBOPT_0x36
	BREQ _0x7A
;     603 				continue;
	RJMP _0x5C
;     604 
;     605 
;     606 			if(dlg_cnt > 1)
_0x7A:
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	__CPD2N 0x2
	BRLO _0x7B
;     607 			{
;     608 			        onesec_cnt++;
	LDS  R30,_onesec_cnt
	LDS  R31,_onesec_cnt+1
	LDS  R22,_onesec_cnt+2
	LDS  R23,_onesec_cnt+3
	CALL SUBOPT_0x3C
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R31
	STS  _onesec_cnt+2,R22
	STS  _onesec_cnt+3,R23
;     609 			        if(onesec_cnt == (ONESEC-10))
	CALL SUBOPT_0x3D
	__CPD2N 0x18696
	BRNE _0x7C
;     610 			        {
;     611         			        updatestate();
	CALL _updatestate
;     612 
;     613         			}
;     614         			if(onesec_cnt == ONESEC)
_0x7C:
	CALL SUBOPT_0x3D
	__CPD2N 0x186A0
	BRNE _0x7D
;     615         			        onesec_cnt = 0 ;
	CALL SUBOPT_0x38
;     616 				dlg_cnt--;
_0x7D:
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
;     617 				continue;
	RJMP _0x5C
;     618 			}
;     619 			updatestate();
_0x7B:
	CALL _updatestate
;     620 			if((IS_THERM_MODE))
	CALL SUBOPT_0x39
	BRNE _0x7E
;     621 			{
;     622 				if(therm_state() == 0)
	CALL _therm_state
	CPI  R30,0
	BRNE _0x7F
;     623 				        continue;
	RJMP _0x5C
;     624 			}else{
_0x7F:
	RJMP _0x80
_0x7E:
;     625 				if(bore_state() == 0)
	CALL _bore_state
	CPI  R30,0
	BRNE _0x81
;     626 				        continue;
	RJMP _0x5C
;     627 			}
_0x81:
_0x80:
;     628                         //shift to next channel
;     629                         while(true)
_0x82:
;     630                         {
;     631                                 ch_to_search += 1;
	MOVW R30,R12
	ADIW R30,1
	MOVW R12,R30
;     632         			if(ch_to_search >= MAX_CH_NUM)
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R12,R30
	CPC  R13,R31
	BRLT _0x85
;     633         			{
;     634 	        			ch_to_search = 0;
	CLR  R12
	CLR  R13
;     635 	        			break;
	RJMP _0x84
;     636 	        		}
;     637 	        		if(IS_THERM_MODE)
_0x85:
	CALL SUBOPT_0x39
	BRNE _0x86
;     638 	        		{
;     639 	        		        i = sysdata.tid[ch_to_search];
	__POINTW1MN _sysdata,14
	RJMP _0x49E
;     640 	        		}else{
_0x86:
;     641 	        		        i = sysdata.rid[ch_to_search];
	__POINTW1MN _sysdata,38
_0x49E:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R16,R30
	CLR  R17
;     642 	        		}
;     643 	        		if(i == INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x82
;     644 	        		        continue;
;     645 	        		if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x89
;     646 	        		{
;     647        					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x3B
	CPI  R30,LOW(0x3)
	BRLO _0x8B
	CALL SUBOPT_0x3B
	CPI  R30,LOW(0xA)
	BRLO _0x8C
_0x8B:
	RJMP _0x8A
_0x8C:
;     648        					        break;
	RJMP _0x84
;     649 	        		}else{
_0x8A:
	RJMP _0x8D
_0x89:
;     650        		        		if((rprbdata.type[i] <= PRBTYPE_PT25) && (rprbdata.type[i] >= PRBTYPE_PT100))
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0xF3)
	BRSH _0x8F
	CALL SUBOPT_0x3E
	CPI  R30,LOW(0xF1)
	BRSH _0x90
_0x8F:
	RJMP _0x8E
_0x90:
;     651        		        		        break;
	RJMP _0x84
;     652 	        		}
_0x8E:
_0x8D:
;     653 	                }
	RJMP _0x82
_0x84:
;     654 		}
_0x79:
;     655 	}
	RJMP _0x5C
;     656 }
_0x91:
	RJMP _0x91
;     657 
;     658 WINDOW flash wins[]={
;     659 	{PG_MAIN,	pgmain_handler},	//running menu
;     660 	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
;     661 	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
;     662 	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
;     663 	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
;     664 	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
;     665 	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
;     666 	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
;     667 	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
;     668 };
;     669 
;     670 
;     671 void SwitchWindow(uchar page)
;     672 {
_SwitchWindow:
;     673 	u8 i;
;     674 	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	ST   -Y,R17
;	page -> Y+1
;	i -> R17
	LDI  R17,LOW(0)
_0x93:
	CPI  R17,9
	BRSH _0x94
;     675 	{
;     676 		if(wins[i].page_id == page)
	LDI  R30,LOW(3)
	MUL  R30,R17
	MOVW R30,R0
	SUBI R30,LOW(-_wins*2)
	SBCI R31,HIGH(-_wins*2)
	LPM  R30,Z
	MOV  R26,R30
	LDD  R30,Y+1
	CP   R30,R26
	BRNE _0x95
;     677 		{
;     678 			curr_window = wins[i].handler;
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_wins,1
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	CALL SUBOPT_0x3F
	STS  _curr_window,R30
	STS  _curr_window+1,R31
;     679 			return;
	LDD  R17,Y+0
	RJMP _0x495
;     680 		}
;     681 	}
_0x95:
	SUBI R17,-1
	RJMP _0x93
_0x94:
;     682 }
	LDD  R17,Y+0
_0x495:
	ADIW R28,2
	RET
;     683 /*********************************************************************************/
;     684 //                         UART0 and UART1 Subroutines
;     685 /*********************************************************************************/
;     686 #include <mega64.h>
;     687 #include "uart.h"
;     688 #include "16c554.h"
;     689 #include "global.h"
;     690 
;     691 //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
;     692 //extern u8 x_modem_databuf[];                       /* store data sent from PC */
;     693 //extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
;     694 //extern u8 x_modem_file_being_transferred;
;     695 //extern u8 x_modem_file_transfer_complete;
;     696 
;     697 /*********************************************************************************/
;     698 //                     UART0  Variables & Buffer Definition
;     699 /*********************************************************************************/
;     700 u8 volatile tx_buffer0[TX_BUFFER_SIZE0];

	.DSEG
_tx_buffer0:
	.BYTE 0x80
;     701 u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
;     702 
;     703 /*********************************************************************************/
;     704 //                     UART1  Variables & Buffer Definition
;     705 /*********************************************************************************/
;     706 u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
_tx_buffer1:
	.BYTE 0x80
;     707 u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;
_tx_wr_index1:
	.BYTE 0x1
_tx_rd_index1:
	.BYTE 0x1
_tx_counter1:
	.BYTE 0x1
;     708 
;     709 /*
;     710  * 0-9,a-f,A-F char to hex conversion
;     711  * otherwise 0xff will be returned.
;     712  */
;     713 u8 ishexchar(u8 dat){

	.CSEG
;     714         if( (dat >= '0') && (dat <= '9'))
;	dat -> Y+0
;     715                 return dat-'0';
;     716         if( (dat >= 'a') && (dat <= 'f'))
;     717                 return dat-'a'+0x0a;
;     718         if( (dat >= 'A') && (dat <= 'F'))
;     719                 return dat-'A'+0x0a;
;     720         return 0xff;
;     721 }
;     722 
;     723 
;     724 
;     725 interrupt [USART0_RXC] void usart0_rx_isr(void)
;     726 {
_usart0_rx_isr:
	CALL SUBOPT_0x40
;     727    u8 status,data;
;     728    //read status and data
;     729    status=UCSR0A;
;	status -> R17
;	data -> R16
	IN   R17,11
;     730    data=UDR0;
	IN   R16,12
;     731    cm0_push(data);
	ST   -Y,R16
	RCALL _kbd_uart_push
;     732 }
	CALL SUBOPT_0x41
	RETI
;     733 
;     734 /*********************************************************************************/
;     735 //                 USART0 Transmitter interrupt service routine
;     736 /*********************************************************************************/
;     737 interrupt [USART0_TXC] void usart0_tx_isr(void)
;     738 {
_usart0_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     739 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0x9F
;     740    {
;     741    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
;     742    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	CALL SUBOPT_0x42
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	OUT  0xC,R30
;     743    tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_rd_index0
	ANDI R30,0x7F
	STS  _tx_rd_index0,R30
;     744    };
_0x9F:
;     745 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     746 
;     747 /*********************************************************************************/
;     748 //                      UART1 Receiver interrupt service routine
;     749 /*********************************************************************************/
;     750 interrupt [USART1_RXC] void usart1_rx_isr(void)
;     751 {
_usart1_rx_isr:
	CALL SUBOPT_0x40
;     752    u8 status,data;
;     753 
;     754    //read status and data
;     755    status=UCSR1A;
;	status -> R17
;	data -> R16
	LDS  R17,155
;     756    data=UDR1;
	LDS  R16,156
;     757    cm1_push(data);
	ST   -Y,R16
	CALL _nav_uart_push
;     758 }
	CALL SUBOPT_0x41
	RETI
;     759 
;     760 /*********************************************************************************/
;     761 //                   USART1 Transmitter interrupt service routine
;     762 /*********************************************************************************/
;     763 interrupt [USART1_TXC] void usart1_tx_isr(void)
;     764 {
_usart1_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     765 if (tx_counter1)
	LDS  R30,_tx_counter1
	CPI  R30,0
	BREQ _0xA0
;     766    {
;     767    --tx_counter1;
	SUBI R30,LOW(1)
	STS  _tx_counter1,R30
;     768    UDR1=tx_buffer1[tx_rd_index1++];
	LDS  R30,_tx_rd_index1
	SUBI R30,-LOW(1)
	STS  _tx_rd_index1,R30
	CALL SUBOPT_0x42
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R30,Z
	STS  156,R30
;     769    tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_rd_index1
	ANDI R30,0x7F
	STS  _tx_rd_index1,R30
;     770    };
_0xA0:
;     771 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     772 
;     773 void com0_putc(char c)
;     774 {
_com0_putc:
;     775 if(DEBUG == 1)
;	c -> Y+0
;     776         return;
;     777 while (tx_counter0 == TX_BUFFER_SIZE0);
_0xA2:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x80)
	BREQ _0xA2
;     778 #asm("cli")
	cli
;     779 if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0xA6
	SBIC 0xB,5
	RJMP _0xA5
_0xA6:
;     780    {
;     781    tx_buffer0[tx_wr_index0++]=c;
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	CALL SUBOPT_0x42
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
;     782    tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_wr_index0
	ANDI R30,0x7F
	STS  _tx_wr_index0,R30
;     783    ++tx_counter0;
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
;     784    }
;     785 else
	RJMP _0xA8
_0xA5:
;     786    UDR0=c;
	LD   R30,Y
	OUT  0xC,R30
;     787 #asm("sei")
_0xA8:
	sei
;     788 }
	ADIW R28,1
	RET
;     789 
;     790 
;     791 void com1_putc(char c)
;     792 {
_com1_putc:
;     793 while (tx_counter1 == TX_BUFFER_SIZE1);
;	c -> Y+0
_0xA9:
	LDS  R26,_tx_counter1
	CPI  R26,LOW(0x80)
	BREQ _0xA9
;     794 #asm("cli")
	cli
;     795 if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter1
	CPI  R30,0
	BRNE _0xAD
	LDS  R30,155
	ANDI R30,LOW(0x20)
	BRNE _0xAC
_0xAD:
;     796    {
;     797    tx_buffer1[tx_wr_index1++]=c;
	LDS  R30,_tx_wr_index1
	SUBI R30,-LOW(1)
	STS  _tx_wr_index1,R30
	CALL SUBOPT_0x42
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R26,Y
	STD  Z+0,R26
;     798    tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_wr_index1
	ANDI R30,0x7F
	STS  _tx_wr_index1,R30
;     799    ++tx_counter1;
	LDS  R30,_tx_counter1
	SUBI R30,-LOW(1)
	STS  _tx_counter1,R30
;     800    }
;     801 else
	RJMP _0xAF
_0xAC:
;     802    UDR1=c;
	LD   R30,Y
	STS  156,R30
;     803 #asm("sei")
_0xAF:
	sei
;     804 }
	ADIW R28,1
	RET
;     805 
;     806 /*
;     807 void com0_puthex(u8 a)
;     808 {
;     809 	unsigned char h,l;
;     810 	h = (a & 0xf0) >> 4;
;     811 	l = (a & 0x0f);
;     812 	if(h <= 9)
;     813 		com0_putc(h+'0');
;     814 	else
;     815 		com0_putc(h+'A'-0x0a);
;     816 	if(l <= 9)
;     817 		com0_putc(l+'0');
;     818 	else
;     819 		com0_putc(l+'A'-0x0a);
;     820 }
;     821 */
;     822 
;     823 void com0_putstr(u8 *ptr){
;     824         while(*ptr != 0x00){
;	*ptr -> Y+0
;     825                 com0_putc(*ptr++);
;     826         }
;     827 }
;     828 
;     829 void com1_puthex(u8 a)
;     830 {
;     831 	unsigned char h,l;
;     832 	h = (a & 0xf0) >> 4;
;	a -> Y+2
;	h -> R17
;	l -> R16
;     833 	l = (a & 0x0f);
;     834 	if(h <= 9)
;     835 		com1_putc(h+'0');
;     836 	else
;     837 		com1_putc(h+'A'-0x0a);
;     838 	if(l <= 9)
;     839 		com1_putc(l+'0');
;     840 	else
;     841 		com1_putc(l+'A'-0x0a);
;     842 }
;     843 void com1_putstr(u8 *ptr){
;     844         while(*ptr != 0x00){
;	*ptr -> Y+0
;     845                 com1_putc(*ptr++);
;     846         }
;     847 }
;     848 /*
;     849 void com0_mputs(u8 *buf, u8 size){
;     850         while(size-- > 0){
;     851                 com0_puthex(*buf++);
;     852         }
;     853 }
;     854 */
;     855 void com1_mputs(u8 *buf, u8 size){
;     856         while(size-- > 0){
;	*buf -> Y+1
;	size -> Y+0
;     857                 com1_puthex(*buf++);
;     858         }
;     859 }
;     860 
;     861 /*********************************************************************************/
;     862 //                              Intialize UART
;     863 /*********************************************************************************/
;     864 void Init_UART()
;     865 {
_Init_UART:
;     866    // USART0 initialization
;     867    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     868    // USART0 Receiver: On
;     869    // USART0 Transmitter: On
;     870    // USART0 Mode: Asynchronous
;     871    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     872       UCSR0A=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;     873       UCSR0B=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     874       UCSR0C=0x06;
	LDI  R30,LOW(6)
	STS  149,R30
;     875       UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  144,R30
;     876       UBRR0L=0x0F;
	LDI  R30,LOW(15)
	OUT  0x9,R30
;     877 
;     878    // USART1 initialization
;     879    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     880    // USART1 Receiver: On
;     881    // USART1 Transmitter: On
;     882    // USART1 Mode: Asynchronous
;     883    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     884       UCSR1A=0x00;
	LDI  R30,LOW(0)
	STS  155,R30
;     885       UCSR1B=0xD8;
	LDI  R30,LOW(216)
	STS  154,R30
;     886       UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
;     887       UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
;     888       UBRR1L=0x0F;
	LDI  R30,LOW(15)
	STS  153,R30
;     889 
;     890    // Initialize buffer variables
;     891       tx_wr_index0 = 0;
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
;     892       tx_rd_index0 = 0;
	STS  _tx_rd_index0,R30
;     893       tx_counter0  = 0;
	STS  _tx_counter0,R30
;     894 
;     895       tx_wr_index1 = 0;
	STS  _tx_wr_index1,R30
;     896       tx_rd_index1 = 0;
	STS  _tx_rd_index1,R30
;     897       tx_counter1  = 0;
	STS  _tx_counter1,R30
;     898 }
	RET
;     899 /********************************************************************************/
;     900 //                             TL16C554 Driver
;     901 // Controller:  MEGA64
;     902 // 16C554 clk:  7.3728MHZ
;     903 //
;     904 // Schematic Description:
;     905 // 16C554 <---> MEGA64
;     906 // INTA   <---> INT4
;     907 // INTB   <---> INT5
;     908 // INTC   <---> INT6
;     909 // INTD   <---> INT7
;     910 // CSA#   <---> A15
;     911 // CSB#   <---> A14
;     912 // CSC#   <---> A13
;     913 // CSD#   <---> A12
;     914 //   A0   <---> A8
;     915 //   A1   <---> A9
;     916 //   A2   <---> A10
;     917 // RESET  <---> PB0
;     918 // INTN#  <---> PE2
;     919 
;     920 // Coded by: fd_lxh@yahoo.com.cn
;     921 /********************************************************************************/
;     922 #include "mega64.h"
;     923 #include "global.h"
;     924 #include "16c554.h"
;     925 #include "uart.h"
;     926 
;     927 #define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
;     928 #define IER_DATA 0b00000001
;     929 // LCR.7 must be cleared to 0 when accessing RBR/THR/IER
;     930 // LCR.7 must be set to 1 when accessing divisor latch
;     931 #define LCR_DATA 0b00011011   // Even Parity 8 data bit, 1 stop bit
;     932 #define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
;     933 #define DLM_DATA 0x0
;     934 #define MCR_DATA 0b00001000
;     935 
;     936 #define min(A,B)        ((A<B)?(A):(B))
;     937 /********************************************************************************/
;     938 //               TLC16C554 UART PORT A Interrupt Subroutine
;     939 /********************************************************************************/
;     940 interrupt [EXT_INT4] void ext_int4_isr(void)
;     941 {
_ext_int4_isr:
	CALL SUBOPT_0x43
;     942    u8 rundata;
;     943    switch(UC_554A_IIR & 0xF)
;	rundata -> R17
	LDS  R30,29184
	ANDI R30,LOW(0xF)
;     944    {
;     945       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC0
;     946           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     947           break;
	RJMP _0xBF
;     948       case 0x4:                         // Receiver data available or trigger level reached
_0xC0:
	CPI  R30,LOW(0x4)
	BRNE _0xC2
;     949           rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,28672
;     950           cm_pushA(rundata);
	ST   -Y,R17
	RCALL _pc_uart_push
;     951           return;
	CALL SUBOPT_0x44
	RETI
;     952       default:
_0xC2:
;     953           break;
;     954    }
_0xBF:
;     955 
;     956    rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,29952
;     957 }
	CALL SUBOPT_0x44
	RETI
;     958 /********************************************************************************/
;     959 //               TLC16C554 UART PORT B Interrupt Subroutine
;     960 /********************************************************************************/
;     961 // External Interrupt 5 service routine
;     962 interrupt [EXT_INT5] void ext_int5_isr(void)
;     963 {
_ext_int5_isr:
	CALL SUBOPT_0x43
;     964    u8 rundata;
;     965    switch(UC_554B_IIR & 0xF)
;	rundata -> R17
	LDS  R30,45568
	ANDI R30,LOW(0xF)
;     966    {
;     967       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC6
;     968           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     969           break;
	RJMP _0xC5
;     970       case 0x4:                         // Receiver data available or trigger level reached
_0xC6:
	CPI  R30,LOW(0x4)
	BRNE _0xC8
;     971           rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,45056
;     972           cm_pushB(rundata);
	ST   -Y,R17
	RCALL _scanner_uart_push
;     973           return;
	CALL SUBOPT_0x44
	RETI
;     974       default:
_0xC8:
;     975           break;
;     976    }
_0xC5:
;     977    rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,46336
;     978 }
	CALL SUBOPT_0x44
	RETI
;     979 /********************************************************************************/
;     980 //               TLC16C554 UART PORT C Interrupt Subroutine
;     981 /********************************************************************************/
;     982 // External Interrupt 6 service routine
;     983 interrupt [EXT_INT6] void ext_int6_isr(void)
;     984 {
_ext_int6_isr:
	CALL SUBOPT_0x43
;     985    u8 rundata;
;     986    switch(UC_554C_IIR & 0xF)
;	rundata -> R17
	LDS  R30,53760
	ANDI R30,LOW(0xF)
;     987    {
;     988       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xCC
;     989           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     990           break;
	RJMP _0xCB
;     991       case 0x4:                         // Receiver data available or trigger level reached
_0xCC:
	CPI  R30,LOW(0x4)
	BRNE _0xCE
;     992           rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,53248
;     993           cm_pushC(rundata);
	ST   -Y,R17
	CALL _testA
;     994           return;
	CALL SUBOPT_0x44
	RETI
;     995       default:
_0xCE:
;     996           break;
;     997    }
_0xCB:
;     998 
;     999    rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,54528
;    1000 }
	CALL SUBOPT_0x44
	RETI
;    1001 
;    1002 /********************************************************************************/
;    1003 //               TLC16C554 UART PORT D Interrupt Subroutine
;    1004 /********************************************************************************/
;    1005 // External Interrupt 7 service routine
;    1006 interrupt [EXT_INT7] void ext_int7_isr(void)
;    1007 {
_ext_int7_isr:
	CALL SUBOPT_0x43
;    1008    u8 rundata;
;    1009    switch(UC_554D_IIR & 0xF)
;	rundata -> R17
	LDS  R30,57856
	ANDI R30,LOW(0xF)
;    1010    {
;    1011       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xD2
;    1012           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1013           break;
	RJMP _0xD1
;    1014       case 0x4:                         // Receiver data available or trigger level reached
_0xD2:
	CPI  R30,LOW(0x4)
	BRNE _0xD4
;    1015           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1016           cm_pushD(rundata);
	ST   -Y,R17
	CALL _testA
;    1017           return;
	CALL SUBOPT_0x44
	RETI
;    1018       default:
_0xD4:
;    1019           break;
;    1020    }
_0xD1:
;    1021    rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,58624
;    1022 }
	CALL SUBOPT_0x44
	RETI
;    1023 
;    1024 /********************************************************************************/
;    1025 //                             Reset 16C554
;    1026 /********************************************************************************/
;    1027 void Reset_554(void)
;    1028 {
_Reset_554:
;    1029    // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
;    1030       PORTB.0 = 1;
	SBI  0x18,0
;    1031       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x32
;    1032       PORTB.0 = 0;     // Recover from reset
	CBI  0x18,0
;    1033       sleepms(500);    // Delay
	LDI  R30,LOW(500)
	LDI  R31,HIGH(500)
	CALL SUBOPT_0x32
;    1034 }
	RET
;    1035 /********************************************************************************/
;    1036 //                             Initialize 16C554
;    1037 /********************************************************************************/
;    1038 void Init_554(void)
;    1039 {
_Init_554:
;    1040       Reset_554();  // Reset 554
	CALL _Reset_554
;    1041 
;    1042    /********************************************************************/
;    1043    //                      Initialize UART A
;    1044    /********************************************************************/
;    1045    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1046       UC_554A_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  29440,R30
;    1047       UC_554A_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  28672,R30
;    1048       UC_554A_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  28928,R30
;    1049 
;    1050    // Set Line Control Register:
;    1051    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1052       UC_554A_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  29440,R30
;    1053 
;    1054    // Set FIFO Control Register:
;    1055    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1056       UC_554A_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  29184,R30
;    1057 
;    1058    // Set Modem Control Register:
;    1059    // Enable enternal interrupt
;    1060       UC_554A_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  29696,R30
;    1061 
;    1062    // Set Interrupt Enable Register:
;    1063    // Enable Received data available / timeout / transmitter holding register empty &
;    1064    // Receiver line status interrupt
;    1065       UC_554A_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  28928,R30
;    1066 
;    1067    /********************************************************************/
;    1068    //                      Initialize UART B
;    1069    /********************************************************************/
;    1070    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1071    // LCR.7 must be set to "1" before setting baud rate
;    1072       UC_554B_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  45824,R30
;    1073       UC_554B_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  45056,R30
;    1074       UC_554B_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  45312,R30
;    1075 
;    1076    // Set Line Control Register:
;    1077    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1078       UC_554B_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  45824,R30
;    1079 
;    1080    // Set FIFO Control Register:
;    1081    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1082       UC_554B_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  45568,R30
;    1083 
;    1084    // Set Modem Control Register:
;    1085    // Enable enternal interrupt
;    1086       UC_554B_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  46080,R30
;    1087 
;    1088    // Set Interrupt Enable Register:
;    1089    // Enable Received data available / timeout / transmitter holding register empty &
;    1090    // Receiver line status interrupt
;    1091       UC_554B_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  45312,R30
;    1092 
;    1093    /********************************************************************/
;    1094    //                      Initialize UART C
;    1095    /********************************************************************/
;    1096    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1097       UC_554C_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  54016,R30
;    1098       UC_554C_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  53248,R30
;    1099       UC_554C_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  53504,R30
;    1100 
;    1101    // Set Line Control Register:
;    1102    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1103       UC_554C_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  54016,R30
;    1104 
;    1105    // Set FIFO Control Register:
;    1106    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1107       UC_554C_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  53760,R30
;    1108 
;    1109    // Set Modem Control Register:
;    1110    // Enable enternal interrupt
;    1111       UC_554C_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  54272,R30
;    1112 
;    1113    // Set Interrupt Enable Register:
;    1114    // Enable Received data available / timeout / transmitter holding register empty &
;    1115    // Receiver line status interrupt
;    1116       UC_554C_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  53504,R30
;    1117 
;    1118    /********************************************************************/
;    1119    //                      Initialize UART D
;    1120    /********************************************************************/
;    1121    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1122       UC_554D_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  58112,R30
;    1123       UC_554D_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  57344,R30
;    1124       UC_554D_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  57600,R30
;    1125 
;    1126    // Set Line Control Register:
;    1127    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1128       UC_554D_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  58112,R30
;    1129 
;    1130    // Set FIFO Control Register:
;    1131    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1132       UC_554D_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  57856,R30
;    1133 
;    1134    // Set Modem Control Register:
;    1135    // Enable enternal interrupt
;    1136       UC_554D_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  58368,R30
;    1137 
;    1138    // Set Interrupt Enable Register:
;    1139    // Enable Received data available / timeout / transmitter holding register empty &
;    1140    // Receiver line status interrupt
;    1141       UC_554D_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  57600,R30
;    1142 }
	RET
;    1143 
;    1144 
;    1145 /********************************************************************************/
;    1146 //                 Send multi-bytes (less than 16 bytes) via UART
;    1147 // Function:
;    1148 // Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
;    1149 // format:
;    1150 // print_554(u8 *str, char uart_port)
;    1151 // arg1: string to be sent. arg2: port (A B,C,D)
;    1152 /********************************************************************************/
;    1153 void prints(u8 *str, u8 length, char uart_port)
;    1154 {
_prints:
;    1155     u8 len, i;
;    1156     len = length & 0x0F;        //15bytes at most
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
;    1157     switch(uart_port)
	LDD  R30,Y+2
;    1158     {
;    1159        case SPORTA:
	CPI  R30,0
	BRNE _0xD8
;    1160            // wait if data still being transmitted in UART
;    1161            // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
;    1162            // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
;    1163            // check if FIFO is empty by LSR5.
;    1164            // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
;    1165            // UC_554A_LSR is not a register name,
;    1166            // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
;    1167            // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
;    1168            // here in 16c554.h, a bracket is added
;    1169            while(len)
_0xD9:
	CPI  R17,0
	BREQ _0xDB
;    1170            {
;    1171               while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xDC:
	LDS  R30,29952
	ANDI R30,LOW(0x20)
	BREQ _0xDC
;    1172               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xE0:
	CPI  R17,16
	BRSH _0xE2
	MOV  R30,R17
	RJMP _0xE3
_0xE2:
	LDI  R30,LOW(16)
_0xE3:
	CP   R16,R30
	BRSH _0xE1
;    1173                   UC_554A_THR = *str++;
	CALL SUBOPT_0x45
	STS  28672,R30
;    1174               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xE0
_0xE1:
	CPI  R17,16
	BRSH _0xE5
	MOV  R30,R17
	RJMP _0xE6
_0xE5:
	LDI  R30,LOW(16)
_0xE6:
	SUB  R17,R30
;    1175            }
	RJMP _0xD9
_0xDB:
;    1176            break;
	RJMP _0xD7
;    1177        case SPORTB:
_0xD8:
	CPI  R30,LOW(0x1)
	BRNE _0xE8
;    1178            while(len)
_0xE9:
	CPI  R17,0
	BREQ _0xEB
;    1179            {
;    1180               while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xEC:
	LDS  R30,46336
	ANDI R30,LOW(0x20)
	BREQ _0xEC
;    1181               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0xF0:
	CPI  R17,16
	BRSH _0xF2
	MOV  R30,R17
	RJMP _0xF3
_0xF2:
	LDI  R30,LOW(16)
_0xF3:
	CP   R16,R30
	BRSH _0xF1
;    1182                   UC_554B_THR = *str++;
	CALL SUBOPT_0x45
	STS  45056,R30
;    1183               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0xF0
_0xF1:
	CPI  R17,16
	BRSH _0xF5
	MOV  R30,R17
	RJMP _0xF6
_0xF5:
	LDI  R30,LOW(16)
_0xF6:
	SUB  R17,R30
;    1184            }
	RJMP _0xE9
_0xEB:
;    1185            break;
	RJMP _0xD7
;    1186        case SPORTC:
_0xE8:
	CPI  R30,LOW(0x2)
	BRNE _0xF8
;    1187            while(len)
_0xF9:
	CPI  R17,0
	BREQ _0xFB
;    1188            {
;    1189               while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xFC:
	LDS  R30,54528
	ANDI R30,LOW(0x20)
	BREQ _0xFC
;    1190               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0x100:
	CPI  R17,16
	BRSH _0x102
	MOV  R30,R17
	RJMP _0x103
_0x102:
	LDI  R30,LOW(16)
_0x103:
	CP   R16,R30
	BRSH _0x101
;    1191                   UC_554C_THR = *str++;
	CALL SUBOPT_0x45
	STS  53248,R30
;    1192               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0x100
_0x101:
	CPI  R17,16
	BRSH _0x105
	MOV  R30,R17
	RJMP _0x106
_0x105:
	LDI  R30,LOW(16)
_0x106:
	SUB  R17,R30
;    1193            }
	RJMP _0xF9
_0xFB:
;    1194 
;    1195            break;
	RJMP _0xD7
;    1196        case SPORTD:
_0xF8:
	CPI  R30,LOW(0x3)
	BRNE _0x108
;    1197            while(len)
_0x109:
	CPI  R17,0
	BREQ _0x10B
;    1198            {
;    1199               while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
_0x10C:
	LDS  R30,58624
	ANDI R30,LOW(0x20)
	BREQ _0x10C
;    1200               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
	LDI  R16,LOW(0)
_0x110:
	CPI  R17,16
	BRSH _0x112
	MOV  R30,R17
	RJMP _0x113
_0x112:
	LDI  R30,LOW(16)
_0x113:
	CP   R16,R30
	BRSH _0x111
;    1201                   UC_554D_THR = *str++;
	CALL SUBOPT_0x45
	STS  57344,R30
;    1202               len = len - min(len,16);
	SUBI R16,-1
	RJMP _0x110
_0x111:
	CPI  R17,16
	BRSH _0x115
	MOV  R30,R17
	RJMP _0x116
_0x115:
	LDI  R30,LOW(16)
_0x116:
	SUB  R17,R30
;    1203            }
	RJMP _0x109
_0x10B:
;    1204            break;
	RJMP _0xD7
;    1205        case SPORT0:
_0x108:
	CPI  R30,LOW(0x4)
	BRNE _0x118
;    1206            while(len-- > 0){
_0x119:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x11B
;    1207                 com0_putc(*str++);
	CALL SUBOPT_0x45
	ST   -Y,R30
	CALL _com0_putc
;    1208            }
	RJMP _0x119
_0x11B:
;    1209            break;
	RJMP _0xD7
;    1210        case SPORT1:
_0x118:
	CPI  R30,LOW(0x5)
	BRNE _0x120
;    1211            while(len-- > 0){
_0x11D:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x11F
;    1212                 com1_putc(*str++);
	CALL SUBOPT_0x45
	ST   -Y,R30
	CALL _com1_putc
;    1213            }
	RJMP _0x11D
_0x11F:
;    1214            break;
	RJMP _0xD7
;    1215        default:
_0x120:
;    1216            PORTC = 0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1217            break;
;    1218     }
_0xD7:
;    1219 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    1220 
;    1221 
;    1222 // System Initialization file
;    1223 
;    1224 #include <mega64.h>
;    1225 #include "global.h"
;    1226 /**************************************************************************************/
;    1227 //                              Initialize Port A-G
;    1228 /**************************************************************************************/
;    1229 void Init_Port()
;    1230 {
_Init_Port:
;    1231  // Input/Output Ports initialization
;    1232  // Port A initialization
;    1233  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;    1234  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1235     PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;    1236     DDRA=0x00;
	OUT  0x1A,R30
;    1237 
;    1238  // Port B initialization
;    1239  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
;    1240  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
;    1241  // Use PORTB[7:4] as LED output, 0 -> LED ON
;    1242     PORTB=0x01;
	LDI  R30,LOW(1)
	OUT  0x18,R30
;    1243     DDRB=0xF1;
	LDI  R30,LOW(241)
	OUT  0x17,R30
;    1244 
;    1245  // Port C initialization
;    1246  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
;    1247  // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
;    1248     PORTC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1249     DDRC=0xFF;
	LDI  R30,LOW(255)
	OUT  0x14,R30
;    1250 
;    1251  // Port D initialization
;    1252  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
;    1253  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1254  // Use PORTD[7:6] as LED output, 0 -> LED ON
;    1255  // PORTD.0-1 are set as input of external interrupt 0/1 (see interrupt.c)
;    1256     PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1257     DDRD=0xF0;
	LDI  R30,LOW(240)
	OUT  0x11,R30
;    1258 
;    1259  // Port E initialization
;    1260  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
;    1261  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
;    1262     PORTE=0x00;
	LDI  R30,LOW(0)
	OUT  0x3,R30
;    1263     DDRE=0x04;
	LDI  R30,LOW(4)
	OUT  0x2,R30
;    1264 
;    1265  // Port F initialization
;    1266  // Input
;    1267  // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
;    1268     PORTF=0x00;
	LDI  R30,LOW(0)
	STS  98,R30
;    1269     DDRF=0x00;
	STS  97,R30
;    1270 
;    1271  // Port G initialization
;    1272  // Func4=out Func3=out Func2=out Func1=out Func0=out
;    1273  // State4=0 State3=0 State2=0 State1=0 State0=0
;    1274     PORTG=0xFF;
	LDI  R30,LOW(255)
	STS  101,R30
;    1275     DDRG=0x1F;
	LDI  R30,LOW(31)
	STS  100,R30
;    1276 }
	RET
;    1277 
;    1278 /**************************************************************************************/
;    1279 //                              Initialize Timers
;    1280 /**************************************************************************************/
;    1281 void Init_Timers()
;    1282 {
;    1283  // Timer/Counter 0 initialization
;    1284  // Clock source: System Clock
;    1285  // Clock value: 14.400 kHz
;    1286  // Mode: Normal top=FFh
;    1287  // OC0 output: Disconnected
;    1288     ASSR=0x00;
;    1289     TCCR0=0x07;
;    1290     TCNT0=0x00;
;    1291     OCR0=0x00;
;    1292 
;    1293  // Timer/Counter 1 initialization
;    1294  // Clock source: System Clock
;    1295  // Clock value: 14.400 kHz
;    1296  // Mode: Normal top=FFFFh
;    1297  // OC1A output: Discon.
;    1298  // OC1B output: Discon.
;    1299  // OC1C output: Discon.
;    1300  // Noise Canceler: Off
;    1301  // Input Capture on Falling Edge
;    1302  // Timer 1 Overflow Interrupt: On
;    1303  // Input Capture Interrupt: Off
;    1304  // Compare A Match Interrupt: Off
;    1305  // Compare B Match Interrupt: Off
;    1306  // Compare C Match Interrupt: Off
;    1307     TCCR1A=0x00;
;    1308     TCCR1B=0x05;
;    1309     TCNT1H=0x00;
;    1310     TCNT1L=0x00;
;    1311     ICR1H=0x00;
;    1312     ICR1L=0x00;
;    1313     OCR1AH=0x00;
;    1314     OCR1AL=0x00;
;    1315     OCR1BH=0x00;
;    1316     OCR1BL=0x00;
;    1317     OCR1CH=0x00;
;    1318     OCR1CL=0x00;
;    1319 
;    1320  // Timer/Counter 2 initialization
;    1321  // Clock source: System Clock
;    1322  // Clock value: Timer 2 Stopped
;    1323  // Mode: Normal top=FFh
;    1324  // OC2 output: Disconnected
;    1325     TCCR2=0x00;
;    1326     TCNT2=0x00;
;    1327     OCR2=0x00;
;    1328 
;    1329  // Timer/Counter 3 initialization
;    1330  // Clock source: System Clock
;    1331  // Clock value: Timer 3 Stopped
;    1332  // Mode: Normal top=FFFFh
;    1333  // Noise Canceler: Off
;    1334  // Input Capture on Falling Edge
;    1335  // OC3A output: Discon.
;    1336  // OC3B output: Discon.
;    1337  // OC3C output: Discon.
;    1338  // Timer 3 Overflow Interrupt: Off
;    1339  // Input Capture Interrupt: Off
;    1340  // Compare A Match Interrupt: Off
;    1341  // Compare B Match Interrupt: Off
;    1342  // Compare C Match Interrupt: Off
;    1343     TCCR3A=0x00;
;    1344     TCCR3B=0x00;
;    1345     TCNT3H=0x00;
;    1346     TCNT3L=0x00;
;    1347     ICR3H=0x00;
;    1348     ICR3L=0x00;
;    1349     OCR3AH=0x00;
;    1350     OCR3AL=0x00;
;    1351     OCR3BH=0x00;
;    1352     OCR3BL=0x00;
;    1353     OCR3CH=0x00;
;    1354     OCR3CL=0x00;
;    1355 
;    1356  // Timer(s)/Counter(s) Interrupt(s) initialization
;    1357     TIMSK=0x05;
;    1358     ETIMSK=0x00;
;    1359 }
;    1360 
;    1361 /**************************************************************************************/
;    1362 //                              Enable External Memory
;    1363 /**************************************************************************************/
;    1364 void Enable_XMEM()
;    1365 {
_Enable_XMEM:
;    1366  // External SRAM page configuration:
;    1367  // 1100h - 7FFFh / 8000h - FFFFh
;    1368  // Lower page wait state(s): 2r/w +1 addr
;    1369  // Upper page wait state(s): 2r/w + 1 addr
;    1370     MCUCR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x35,R30
;    1371     //MCUCR = 0x80;
;    1372 
;    1373     XMCRA=0x4E;
	LDI  R30,LOW(78)
	STS  109,R30
;    1374     //XMCRA=0x0A;
;    1375  // PC7:0 can not be released
;    1376     XMCRB=0x80;
	LDI  R30,LOW(128)
	STS  108,R30
;    1377     // XMCRB = 0x07;
;    1378 }
	RET
;    1379 
;    1380 /**************************************************************************************/
;    1381 //                              Initialize External Interrupt
;    1382 /**************************************************************************************/
;    1383 void Init_Ex_Interrupt()
;    1384 {
;    1385  // External Interrupt(s) initialization
;    1386  // INT0: Off
;    1387  // INT1: Off
;    1388  // INT2: Off
;    1389  // INT3: Off
;    1390  // INT4: On
;    1391  // INT4 Mode: Rising Edge
;    1392  // INT5: On
;    1393  // INT5 Mode: Rising Edge
;    1394  // INT6: On
;    1395  // INT6 Mode: Rising Edge
;    1396  // INT7: On
;    1397  // INT7 Mode: Rising Edge
;    1398     EICRA=0x00;
;    1399     EICRB=0xFF;
;    1400     EIMSK=0xF0;
;    1401     EIFR=0xF0;
;    1402 }
;    1403 
;    1404 /**************************************************************************************/
;    1405 //                              Analog Comparator initialization
;    1406 /**************************************************************************************/
;    1407 void Init_ADC()
;    1408 {
;    1409  // Analog Comparator initialization
;    1410  // Analog Comparator: Off
;    1411  // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1412     ACSR=0x80;
;    1413     SFIOR=0x00;
;    1414 }
;    1415 #include "global.h"
;    1416 #include "wendu.h"
;    1417 #include "utili.h"
;    1418 #include "math.h"
;    1419 #define logl log
;    1420 #define powl pow
;    1421 double PT100RToTValue(double r,double r0)
;    1422 {
_PT100RToTValue:
;    1423         double ac,bc,cc,tlow,tup,rnew,tnew;
;    1424         int count;
;    1425         ac = 3.908e-3;
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
;    1426         bc = -5.775E-7;
	__GETD1N 0xB51B057F
	__PUTD1S 22
;    1427         cc = -4.183E-12;
	__GETD1N 0xAC932D1D
	__PUTD1S 18
;    1428         tlow = -200;
	__GETD1N 0xC3480000
	CALL SUBOPT_0x46
;    1429         tup = 850;
	__GETD1N 0x44548000
	CALL SUBOPT_0x47
;    1430 	count = 0;
	__GETWRN 16,17,0
;    1431 
;    1432 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x121:
	CALL SUBOPT_0x48
	CALL SUBOPT_0x49
	CALL SUBOPT_0x4A
	BREQ PC+2
	BRCC PC+3
	JMP  _0x124
	CALL SUBOPT_0x4B
	BRLT _0x125
_0x124:
	RJMP _0x123
_0x125:
;    1433 	{
;    1434 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4C
;    1435 		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
	__GETD1S 26
	CALL SUBOPT_0x4D
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x4D
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	__GETD1S 22
	CALL SUBOPT_0x4D
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x4F
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x50
;    1436 		if(tnew < 0)
	CALL SUBOPT_0x4F
	CALL __CPD20
	BRGE _0x126
;    1437 		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
	__GETD1S 18
	CALL SUBOPT_0x4D
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x4F
	__GETD1N 0x42C80000
	CALL SUBOPT_0x2B
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x4F
	CALL SUBOPT_0x4E
	CALL SUBOPT_0x51
	CALL SUBOPT_0x50
;    1438 
;    1439 		if(r < rnew)
_0x126:
	CALL SUBOPT_0x52
	__GETD2S 34
	CALL __CMPF12
	BRSH _0x127
;    1440 			tup = tnew;
	CALL SUBOPT_0x53
	CALL SUBOPT_0x47
;    1441 		else
	RJMP _0x128
_0x127:
;    1442 			tlow = tnew;
	CALL SUBOPT_0x54
;    1443 	}
_0x128:
	RJMP _0x121
_0x123:
;    1444 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x55
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x56
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x57
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,38
	RET
;    1445 
;    1446 }
;    1447 double RValueToTValue(double r, u8 prbid)
;    1448 {
_RValueToTValue:
;    1449 	double ac,bc,cc,tlow,tup,rnew,tnew;
;    1450 	int count;
;    1451 
;    1452 	ac = rprbdata.param1[prbid];
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
	CALL SUBOPT_0x58
	__PUTD1S 26
;    1453 	bc = rprbdata.param2[prbid];
	__POINTW2MN _rprbdata,96
	LDD  R30,Y+30
	CALL SUBOPT_0x58
	__PUTD1S 22
;    1454 	cc = rprbdata.param3[prbid];
	__POINTW2MN _rprbdata,192
	LDD  R30,Y+30
	CALL SUBOPT_0x58
	__PUTD1S 18
;    1455 
;    1456 	if(rprbdata.type[prbid] == PRBTYPE_PT100)
	CALL SUBOPT_0x59
	CPI  R30,LOW(0xF1)
	BRNE _0x129
;    1457 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x5A
	__GETD1S 22
	CALL __PUTPARD1
	CALL _PT100RToTValue
	RJMP _0x494
;    1458 	else if(rprbdata.type[prbid] == PRBTYPE_PT25)
_0x129:
	CALL SUBOPT_0x59
	CPI  R30,LOW(0xF2)
	BRNE _0x12B
;    1459 		r = r/25.0;
	__GETD2S 31
	__GETD1N 0x41C80000
	CALL __DIVF21
	__PUTD1S 31
;    1460 	else
	RJMP _0x12C
_0x12B:
;    1461 		return -9999.999;
	__GETD1N 0xC61C3FFF
	RJMP _0x494
;    1462 
;    1463 
;    1464 
;    1465 	//set the search range of T between GetT(r) +/- 1 degree
;    1466 	tlow = GetT(r) - 1;
_0x12C:
	CALL SUBOPT_0x5A
	RCALL _GetT
	CALL SUBOPT_0x5B
	CALL SUBOPT_0x46
;    1467 	tup = tlow + 2;
	__GETD1S 14
	__GETD2N 0x40000000
	CALL __ADDF12
	CALL SUBOPT_0x47
;    1468 
;    1469 	count = 0;
	__GETWRN 16,17,0
;    1470 
;    1471 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x12D:
	CALL SUBOPT_0x5C
	BREQ PC+2
	BRCC PC+3
	JMP  _0x130
	CALL SUBOPT_0x4B
	BRLT _0x131
_0x130:
	RJMP _0x12F
_0x131:
;    1472 	{
;    1473 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4C
;    1474 		rnew = GetWr(tnew);
	CALL SUBOPT_0x5D
	RCALL _GetWr
	CALL SUBOPT_0x5E
;    1475 		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
	CALL SUBOPT_0x4F
	__GETD1N 0xC33D581D
	CALL __CMPF12
	BRLO _0x133
	CALL SUBOPT_0x4F
	__GETD1N 0x3C23D70A
	CALL __CMPF12
	BREQ PC+4
	BRCS PC+3
	JMP  _0x133
	RJMP _0x134
_0x133:
	RJMP _0x132
_0x134:
;    1476         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x60
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x61
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x62
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
	RJMP _0x49F
;    1477                 else
_0x132:
;    1478         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x60
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x61
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5F
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
	CALL SUBOPT_0x5F
	__GETD2S 18
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5F
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5F
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
_0x49F:
	CALL __ADDF12
	CALL SUBOPT_0x5E
;    1479 		if(r < rnew)
	CALL SUBOPT_0x52
	__GETD2S 31
	CALL __CMPF12
	BRSH _0x136
;    1480 			tup = tnew;
	CALL SUBOPT_0x63
;    1481 		else
	RJMP _0x137
_0x136:
;    1482 			tlow = tnew;
	CALL SUBOPT_0x54
;    1483 	}
_0x137:
	RJMP _0x12D
_0x12F:
;    1484 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x55
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x56
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x57
_0x494:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,35
	RET
;    1485 
;    1486 
;    1487 //	return r+1;
;    1488 }
;    1489 
;    1490 flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
;    1491 flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
;    1492 flash u8 sizeAi = sizeof(Ai)/sizeof(double);
;    1493 flash u8 sizeCi = sizeof(Ci)/sizeof(double);
;    1494 
;    1495 double GetWr(double t)
;    1496 {
_GetWr:
;    1497 	double result;
;    1498 	double pert;
;    1499 	int i;
;    1500 	if(t < 0)
	CALL SUBOPT_0x64
;	t -> Y+10
;	result -> Y+6
;	pert -> Y+2
;	i -> R16,R17
	BRGE _0x138
;    1501 	{       //equals to Wr189(t) in mfile
;    1502 		result = Ai[0];
	LDI  R30,LOW(_Ai*2)
	LDI  R31,HIGH(_Ai*2)
	CALL __GETD1PF
	CALL SUBOPT_0x5E
;    1503 		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
	CALL SUBOPT_0x49
	__GETD2N 0x43889333
	CALL SUBOPT_0x65
	__GETD1N 0x4388947B
	CALL __DIVF21
	CALL SUBOPT_0x66
	__GETD2N 0x3FC00000
	CALL SUBOPT_0x65
	__GETD1N 0x3FC00000
	CALL __DIVF21
	CALL SUBOPT_0x67
;    1504 		t = pert;
;    1505 		for(i=1;i<sizeAi;i++)
	__GETWRN 16,17,1
_0x13A:
	LDI  R30,LOW(_sizeAi*2)
	LDI  R31,HIGH(_sizeAi*2)
	LPM  R30,Z
	CALL SUBOPT_0x68
	BRGE _0x13B
;    1506 		{
;    1507 			result = result + Ai[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ai*2)
	LDI  R27,HIGH(_Ai*2)
	CALL SUBOPT_0x69
;    1508 			t = t*pert;
	CALL SUBOPT_0x6A
;    1509 		}
	__ADDWRN 16,17,1
	RJMP _0x13A
_0x13B:
;    1510 		result = exp(result);
	CALL SUBOPT_0x62
	CALL _exp
	CALL SUBOPT_0x5E
;    1511 	}else{
	RJMP _0x13C
_0x138:
;    1512 			 result = Ci[0];
	LDI  R30,LOW(_Ci*2)
	LDI  R31,HIGH(_Ci*2)
	CALL __GETD1PF
	CALL SUBOPT_0x5E
;    1513 			 pert = t/481.0 - 1;
	CALL SUBOPT_0x6B
	__GETD1N 0x43F08000
	CALL __DIVF21
	CALL SUBOPT_0x5B
	CALL SUBOPT_0x67
;    1514 			 t = pert;
;    1515 			 for(i=1;i<sizeCi;i++)
	__GETWRN 16,17,1
_0x13E:
	LDI  R30,LOW(_sizeCi*2)
	LDI  R31,HIGH(_sizeCi*2)
	LPM  R30,Z
	CALL SUBOPT_0x68
	BRGE _0x13F
;    1516 			 {
;    1517 				 result = result + Ci[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ci*2)
	LDI  R27,HIGH(_Ci*2)
	CALL SUBOPT_0x69
;    1518 				 t = t*pert;
	CALL SUBOPT_0x6A
;    1519 			 }
	__ADDWRN 16,17,1
	RJMP _0x13E
_0x13F:
;    1520 	}
_0x13C:
;    1521 	return result;
	CALL SUBOPT_0x52
	RJMP _0x493
;    1522 }
;    1523 flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
;    1524 flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
;    1525 flash u8 sizeDi = sizeof(Di)/sizeof(double);
;    1526 flash u8 sizeBi = sizeof(Bi)/sizeof(double);
;    1527 
;    1528 double GetT(double w)
;    1529 {
_GetT:
;    1530 	double perw;
;    1531 	double result;
;    1532 	int i;
;    1533 
;    1534 	if(w >= 0)
	CALL SUBOPT_0x64
;	w -> Y+10
;	perw -> Y+6
;	result -> Y+2
;	i -> R16,R17
	BRLT _0x140
;    1535 	{       //t660r
;    1536 		perw = (w-2.64)/1.64;
	CALL SUBOPT_0x6B
	__GETD1N 0x4028F5C3
	CALL SUBOPT_0x2B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3FD1EB85
	CALL SUBOPT_0x6C
;    1537 		w = perw;
;    1538 		result = Di[0];
	LDI  R30,LOW(_Di*2)
	LDI  R31,HIGH(_Di*2)
	CALL SUBOPT_0x6D
;    1539 		for(i=1;i<sizeDi;i++)
_0x142:
	LDI  R30,LOW(_sizeDi*2)
	LDI  R31,HIGH(_sizeDi*2)
	LPM  R30,Z
	CALL SUBOPT_0x68
	BRGE _0x143
;    1540 		{
;    1541 			result = result + Di[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Di*2)
	LDI  R27,HIGH(_Di*2)
	CALL SUBOPT_0x6E
;    1542 			w = w*perw;
;    1543 		}
	__ADDWRN 16,17,1
	RJMP _0x142
_0x143:
;    1544 	}else{   //t189(r)
	RJMP _0x144
_0x140:
;    1545 		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
	CALL SUBOPT_0x49
	CALL __PUTPARD1
	__GETD1N 0x3E2AAAAB
	CALL __PUTPARD1
	CALL _pow
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F266666
	CALL SUBOPT_0x2B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3EB33333
	CALL SUBOPT_0x6C
;    1546 		w = perw;
;    1547 		result = Bi[0];
	LDI  R30,LOW(_Bi*2)
	LDI  R31,HIGH(_Bi*2)
	CALL SUBOPT_0x6D
;    1548 		for(i=1;i<sizeBi;i++)
_0x146:
	LDI  R30,LOW(_sizeBi*2)
	LDI  R31,HIGH(_sizeBi*2)
	LPM  R30,Z
	CALL SUBOPT_0x68
	BRGE _0x147
;    1549 		{
;    1550 			result = result + Bi[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Bi*2)
	LDI  R27,HIGH(_Bi*2)
	CALL SUBOPT_0x6E
;    1551 			w = w*perw;
;    1552 		}
	__ADDWRN 16,17,1
	RJMP _0x146
_0x147:
;    1553 		result = 273.15*result - 273.15;
	CALL SUBOPT_0x53
	__GETD2N 0x43889333
	CALL SUBOPT_0x29
	__GETD1N 0x43889333
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x6F
;    1554 	}
_0x144:
;    1555 	return result;
	CALL SUBOPT_0x53
_0x493:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
;    1556 }
;    1557 
;    1558 flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
;    1559 flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};
;    1560 
;    1561 flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
;    1562 flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
;    1563 flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};
;    1564 
;    1565 flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};
;    1566 
;    1567 flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
;    1568 flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};
;    1569 
;    1570 flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
;    1571 flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};
;    1572 
;    1573 flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
;    1574 flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};
;    1575 
;    1576 flash double BLow[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1577 flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};
;    1578 
;    1579 flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1580 flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};
;    1581 
;    1582 flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
;    1583 flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};
;    1584 
;    1585 flash int TLowLen = sizeof(TLow)/sizeof(double);
;    1586 flash int THighLen = sizeof(THigh)/sizeof(double);
;    1587 
;    1588 flash int SLowLen = sizeof(SLow)/sizeof(double);
;    1589 flash int SMedLen = sizeof(SMed)/sizeof(double);
;    1590 flash int SHighLen = sizeof(SHigh)/sizeof(double);
;    1591 
;    1592 flash int RLowLen = sizeof(RLow)/sizeof(double);
;    1593 flash int RMedLen = sizeof(RMed)/sizeof(double);
;    1594 flash int RHighLen = sizeof(RHigh)/sizeof(double);
;    1595 
;    1596 flash int NLowLen = sizeof(NLow)/sizeof(double);
;    1597 flash int NHighLen = sizeof(NHigh)/sizeof(double);
;    1598 
;    1599 flash int JLowLen = sizeof(JLow)/sizeof(double);
;    1600 flash int JHighLen = sizeof(JHigh)/sizeof(double);
;    1601 
;    1602 flash int BLowLen = sizeof(BLow)/sizeof(double);
;    1603 flash int BHighLen = sizeof(BHigh)/sizeof(double);
;    1604 
;    1605 flash int ELowLen = sizeof(ELow)/sizeof(double);
;    1606 flash int EHighLen = sizeof(EHigh)/sizeof(double);
;    1607 
;    1608 flash int KLowLen = sizeof(KLow)/sizeof(double);
;    1609 flash int KHighLen = sizeof(KHigh)/sizeof(double);
;    1610 
;    1611 flash double *coef;

	.DSEG
_coef:
	.BYTE 0x2
;    1612 
;    1613 double GetThmoVolt(double t,char type)
;    1614 {

	.CSEG
_GetThmoVolt:
;    1615 	double result,pert;
;    1616 	int i;
;    1617 	int len = 0;
;    1618 	switch(type)
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
;    1619 	{
;    1620 		case 'T':
	CPI  R30,LOW(0x54)
	BRNE _0x14B
;    1621 			coef = (t < 0) ? TLow : THigh;
	CALL SUBOPT_0x70
	BRGE _0x14C
	LDI  R30,LOW(_TLow*2)
	LDI  R31,HIGH(_TLow*2)
	RJMP _0x14D
_0x14C:
	LDI  R30,LOW(_THigh*2)
	LDI  R31,HIGH(_THigh*2)
_0x14D:
	CALL SUBOPT_0x71
;    1622 			len = (t < 0) ? TLowLen : THighLen;
	BRGE _0x14F
	LDI  R30,LOW(_TLowLen*2)
	LDI  R31,HIGH(_TLowLen*2)
	CALL __GETW1PF
	RJMP _0x150
_0x14F:
	LDI  R30,LOW(_THighLen*2)
	LDI  R31,HIGH(_THighLen*2)
	CALL __GETW1PF
_0x150:
	MOVW R18,R30
;    1623 			break;
	RJMP _0x14A
;    1624 		case 'K':
_0x14B:
	CPI  R30,LOW(0x4B)
	BRNE _0x152
;    1625 			coef = (t < 0) ? KLow : KHigh;
	CALL SUBOPT_0x70
	BRGE _0x153
	LDI  R30,LOW(_KLow*2)
	LDI  R31,HIGH(_KLow*2)
	RJMP _0x154
_0x153:
	LDI  R30,LOW(_KHigh*2)
	LDI  R31,HIGH(_KHigh*2)
_0x154:
	CALL SUBOPT_0x71
;    1626 			len = (t < 0) ? KLowLen : KHighLen;
	BRGE _0x156
	LDI  R30,LOW(_KLowLen*2)
	LDI  R31,HIGH(_KLowLen*2)
	CALL __GETW1PF
	RJMP _0x157
_0x156:
	LDI  R30,LOW(_KHighLen*2)
	LDI  R31,HIGH(_KHighLen*2)
	CALL __GETW1PF
_0x157:
	MOVW R18,R30
;    1627 			break;
	RJMP _0x14A
;    1628 		case 'N':
_0x152:
	CPI  R30,LOW(0x4E)
	BRNE _0x159
;    1629 			coef = (t < 0) ? NLow : NHigh;
	CALL SUBOPT_0x70
	BRGE _0x15A
	LDI  R30,LOW(_NLow*2)
	LDI  R31,HIGH(_NLow*2)
	RJMP _0x15B
_0x15A:
	LDI  R30,LOW(_NHigh*2)
	LDI  R31,HIGH(_NHigh*2)
_0x15B:
	CALL SUBOPT_0x71
;    1630 			len = (t < 0) ? NLowLen : NHighLen;
	BRGE _0x15D
	LDI  R30,LOW(_NLowLen*2)
	LDI  R31,HIGH(_NLowLen*2)
	CALL __GETW1PF
	RJMP _0x15E
_0x15D:
	LDI  R30,LOW(_NHighLen*2)
	LDI  R31,HIGH(_NHighLen*2)
	CALL __GETW1PF
_0x15E:
	MOVW R18,R30
;    1631 			break;
	RJMP _0x14A
;    1632 		case 'E':
_0x159:
	CPI  R30,LOW(0x45)
	BRNE _0x160
;    1633 			coef = (t < 0) ? ELow : EHigh;
	CALL SUBOPT_0x70
	BRGE _0x161
	LDI  R30,LOW(_ELow*2)
	LDI  R31,HIGH(_ELow*2)
	RJMP _0x162
_0x161:
	LDI  R30,LOW(_EHigh*2)
	LDI  R31,HIGH(_EHigh*2)
_0x162:
	CALL SUBOPT_0x71
;    1634 			len = (t < 0) ? ELowLen : EHighLen;
	BRGE _0x164
	LDI  R30,LOW(_ELowLen*2)
	LDI  R31,HIGH(_ELowLen*2)
	CALL __GETW1PF
	RJMP _0x165
_0x164:
	LDI  R30,LOW(_EHighLen*2)
	LDI  R31,HIGH(_EHighLen*2)
	CALL __GETW1PF
_0x165:
	MOVW R18,R30
;    1635 			break;
	RJMP _0x14A
;    1636 		case 'B':
_0x160:
	CPI  R30,LOW(0x42)
	BRNE _0x167
;    1637 			coef = (t < 630.615) ? BLow : BHigh;
	CALL SUBOPT_0x72
	BRSH _0x168
	LDI  R30,LOW(_BLow*2)
	LDI  R31,HIGH(_BLow*2)
	RJMP _0x169
_0x168:
	LDI  R30,LOW(_BHigh*2)
	LDI  R31,HIGH(_BHigh*2)
_0x169:
	CALL SUBOPT_0x73
;    1638 			len = (t < 630.615) ? BLowLen : BHighLen;
	CALL SUBOPT_0x72
	BRSH _0x16B
	LDI  R30,LOW(_BLowLen*2)
	LDI  R31,HIGH(_BLowLen*2)
	CALL __GETW1PF
	RJMP _0x16C
_0x16B:
	LDI  R30,LOW(_BHighLen*2)
	LDI  R31,HIGH(_BHighLen*2)
	CALL __GETW1PF
_0x16C:
	MOVW R18,R30
;    1639 			break;
	RJMP _0x14A
;    1640 		case 'J':
_0x167:
	CPI  R30,LOW(0x4A)
	BRNE _0x16E
;    1641 			coef = (t < 760) ? JLow : JHigh;
	CALL SUBOPT_0x74
	BRSH _0x16F
	LDI  R30,LOW(_JLow*2)
	LDI  R31,HIGH(_JLow*2)
	RJMP _0x170
_0x16F:
	LDI  R30,LOW(_JHigh*2)
	LDI  R31,HIGH(_JHigh*2)
_0x170:
	CALL SUBOPT_0x73
;    1642 			len = (t < 760) ? JLowLen : JHighLen;
	CALL SUBOPT_0x74
	BRSH _0x172
	LDI  R30,LOW(_JLowLen*2)
	LDI  R31,HIGH(_JLowLen*2)
	CALL __GETW1PF
	RJMP _0x173
_0x172:
	LDI  R30,LOW(_JHighLen*2)
	LDI  R31,HIGH(_JHighLen*2)
	CALL __GETW1PF
_0x173:
	MOVW R18,R30
;    1643 			break;
	RJMP _0x14A
;    1644 
;    1645 		case 'S':
_0x16E:
	CPI  R30,LOW(0x53)
	BRNE _0x175
;    1646 			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
	CALL SUBOPT_0x75
	CALL SUBOPT_0x76
	BRSH _0x176
	LDI  R30,LOW(_SLow*2)
	LDI  R31,HIGH(_SLow*2)
	RJMP _0x177
_0x176:
	CALL SUBOPT_0x77
	BRSH _0x179
	LDI  R30,LOW(_SMed*2)
	LDI  R31,HIGH(_SMed*2)
	RJMP _0x17A
_0x179:
	LDI  R30,LOW(_SHigh*2)
	LDI  R31,HIGH(_SHigh*2)
_0x17A:
_0x177:
	CALL SUBOPT_0x78
;    1647 			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
	BRSH _0x17C
	LDI  R30,LOW(_SLowLen*2)
	LDI  R31,HIGH(_SLowLen*2)
	CALL __GETW1PF
	RJMP _0x17D
_0x17C:
	CALL SUBOPT_0x77
	BRSH _0x17F
	LDI  R30,LOW(_SMedLen*2)
	LDI  R31,HIGH(_SMedLen*2)
	CALL __GETW1PF
	RJMP _0x180
_0x17F:
	LDI  R30,LOW(_SHighLen*2)
	LDI  R31,HIGH(_SHighLen*2)
	CALL __GETW1PF
_0x180:
_0x17D:
	MOVW R18,R30
;    1648 			break;
	RJMP _0x14A
;    1649 		case 'R':
_0x175:
	CPI  R30,LOW(0x52)
	BRNE _0x18F
;    1650 			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
	CALL SUBOPT_0x75
	CALL SUBOPT_0x76
	BRSH _0x183
	LDI  R30,LOW(_RLow*2)
	LDI  R31,HIGH(_RLow*2)
	RJMP _0x184
_0x183:
	CALL SUBOPT_0x77
	BRSH _0x186
	LDI  R30,LOW(_RMed*2)
	LDI  R31,HIGH(_RMed*2)
	RJMP _0x187
_0x186:
	LDI  R30,LOW(_RHigh*2)
	LDI  R31,HIGH(_RHigh*2)
_0x187:
_0x184:
	CALL SUBOPT_0x78
;    1651 			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
	BRSH _0x189
	LDI  R30,LOW(_RLowLen*2)
	LDI  R31,HIGH(_RLowLen*2)
	CALL __GETW1PF
	RJMP _0x18A
_0x189:
	CALL SUBOPT_0x77
	BRSH _0x18C
	LDI  R30,LOW(_RMedLen*2)
	LDI  R31,HIGH(_RMedLen*2)
	CALL __GETW1PF
	RJMP _0x18D
_0x18C:
	LDI  R30,LOW(_RHighLen*2)
	LDI  R31,HIGH(_RHighLen*2)
	CALL __GETW1PF
_0x18D:
_0x18A:
	MOVW R18,R30
;    1652 			break;
	RJMP _0x14A
;    1653 		default:
_0x18F:
;    1654 			return 0.0;
	CALL SUBOPT_0x79
	RJMP _0x492
;    1655 	}
_0x14A:
;    1656 	if (len == 0)
	MOV  R0,R18
	OR   R0,R19
	BRNE _0x190
;    1657 		return 0.0;
	CALL SUBOPT_0x79
	RJMP _0x492
;    1658 
;    1659 	result = coef[0];
_0x190:
	LDS  R30,_coef
	LDS  R31,_coef+1
	CALL __GETD1PF
	CALL SUBOPT_0x7A
;    1660 	if(type == 'K')
	LDD  R26,Y+12
	CPI  R26,LOW(0x4B)
	BRNE _0x191
;    1661 	{
;    1662 		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	CALL SUBOPT_0x7B
	__GETD2N 0xB8F82F06
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x7B
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x7C
	__GETD2N 0x3DF2E34C
	CALL SUBOPT_0x7D
;    1663 	}
;    1664 
;    1665 	pert = t;
_0x191:
	__GETD1S 13
	CALL SUBOPT_0x7E
;    1666 	for(i=1;i< len ;i++)
	__GETWRN 16,17,1
_0x193:
	__CPWRR 16,17,18,19
	BRGE _0x194
;    1667 	{
;    1668 		result = result + coef[i] * t;
	MOVW R30,R16
	LDS  R26,_coef
	LDS  R27,_coef+1
	CALL SUBOPT_0x7F
	CALL __GETD1PF
	CALL SUBOPT_0x75
	CALL SUBOPT_0x7D
;    1669 		t = t*pert;
	CALL SUBOPT_0x1
	CALL SUBOPT_0x75
	CALL __MULF12
	__PUTD1S 13
;    1670 	}
	__ADDWRN 16,17,1
	RJMP _0x193
_0x194:
;    1671 	return result;
	CALL SUBOPT_0x80
_0x492:
	CALL __LOADLOCR4
	ADIW R28,17
	RET
;    1672 }
;    1673 
;    1674 double MValueToTValue(double r,char type)
;    1675 {
_MValueToTValue:
;    1676 	double tlow;
;    1677 	double tup;
;    1678 	double rnew;
;    1679 	double tnew;
;    1680 	int count = 0;
;    1681 
;    1682 	switch(type)
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
;    1683 	{
;    1684 		case 'T':
	CPI  R30,LOW(0x54)
	BRNE _0x198
;    1685 			tlow =  -270;	tup = 400;
	CALL SUBOPT_0x81
	__GETD1N 0x43C80000
	CALL SUBOPT_0x47
;    1686 			break;
	RJMP _0x197
;    1687 		case 'K':
_0x198:
	CPI  R30,LOW(0x4B)
	BRNE _0x199
;    1688 			tlow =  -270;	tup = 1372;
	CALL SUBOPT_0x81
	__GETD1N 0x44AB8000
	CALL SUBOPT_0x47
;    1689 			break;
	RJMP _0x197
;    1690 		case 'N':
_0x199:
	CPI  R30,LOW(0x4E)
	BRNE _0x19A
;    1691 			tlow =  -270;	tup = 1300;
	CALL SUBOPT_0x81
	__GETD1N 0x44A28000
	CALL SUBOPT_0x47
;    1692 			break;
	RJMP _0x197
;    1693 		case 'E':
_0x19A:
	CPI  R30,LOW(0x45)
	BRNE _0x19B
;    1694 			tlow =  -270;	tup = 1000;
	CALL SUBOPT_0x81
	__GETD1N 0x447A0000
	CALL SUBOPT_0x47
;    1695 			break;
	RJMP _0x197
;    1696 		case 'B':
_0x19B:
	CPI  R30,LOW(0x42)
	BRNE _0x19C
;    1697 			tlow =  0;	tup = 1280;
	__CLRD1S 14
	__GETD1N 0x44A00000
	CALL SUBOPT_0x47
;    1698 			break;
	RJMP _0x197
;    1699 		case 'J':
_0x19C:
	CPI  R30,LOW(0x4A)
	BRNE _0x19D
;    1700 			tlow =  -210;	tup = 1200;
	__GETD1N 0xC3520000
	CALL SUBOPT_0x46
	__GETD1N 0x44960000
	CALL SUBOPT_0x47
;    1701 			break;
	RJMP _0x197
;    1702 		case 'S':
_0x19D:
	CPI  R30,LOW(0x53)
	BRNE _0x19E
;    1703 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x82
;    1704 			break;
	RJMP _0x197
;    1705 		case 'R':
_0x19E:
	CPI  R30,LOW(0x52)
	BRNE _0x1A0
;    1706 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x82
;    1707 			break;
	RJMP _0x197
;    1708 		default:
_0x1A0:
;    1709 			return -9999.9999;
	__GETD1N 0xC61C4000
	RJMP _0x491
;    1710 	}
_0x197:
;    1711 
;    1712 	while((tup - tlow > 0.00005) || (count++ < 100))
_0x1A1:
	CALL SUBOPT_0x5C
	BREQ PC+4
	BRCS PC+3
	JMP  _0x1A4
	CALL SUBOPT_0x4B
	BRGE _0x1A3
_0x1A4:
;    1713 	{
;    1714 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x4C
;    1715 		rnew = GetThmoVolt(tnew,type);
	CALL SUBOPT_0x5D
	LDD  R30,Y+22
	ST   -Y,R30
	CALL _GetThmoVolt
	CALL SUBOPT_0x83
;    1716 
;    1717 		if(r < rnew)
	__GETD2S 19
	CALL __CMPF12
	BRSH _0x1A6
;    1718 			tup = tnew;
	CALL SUBOPT_0x63
;    1719 		else
	RJMP _0x1A7
_0x1A6:
;    1720 			tlow = tnew;
	CALL SUBOPT_0x54
;    1721 	}
_0x1A7:
	RJMP _0x1A1
_0x1A3:
;    1722 
;    1723 	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x55
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x56
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x57
_0x491:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,23
	RET
;    1724 }
;    1725 
;    1726 #include "font.h"
;    1727 
;    1728 #include <MATH.H>
;    1729 #include "typedef.h"
;    1730 #include "global.h"
;    1731 #include "utili.h"
;    1732 #include "key.h"
;    1733 extern uchar key;
;    1734 extern u8 lcd_buffer;
;    1735 /*
;    1736         Communication with LCD/KBD board,
;    1737         Incoming command format:
;    1738                 CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
;    1739                 key:            key pressed
;    1740         Out command format:
;    1741                 CMDO_DISPLAYBTN,LED_STATUS      update the led status
;    1742 */
;    1743 /**************************主程序********************************/
;    1744 void Key_Init()
;    1745 {
_Key_Init:
;    1746 	key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    1747 }
	RET
;    1748 
;    1749 #include <string.h>
;    1750 #include "lcd.h"
;    1751 #include "font.h"
;    1752 #include "global.h"
;    1753 #include "utili.h"
;    1754 
;    1755 
;    1756 
;    1757 extern uchar key;
;    1758 static uchar kbd_state;

	.DSEG
_kbd_state_G8:
	.BYTE 0x1
;    1759 void kbd_uart_push(uchar data)
;    1760 {

	.CSEG
_kbd_uart_push:
;    1761         if(data == STATUS_IDLE||
;	data -> Y+0
;    1762            data == STATUS_ERR||
;    1763            data == STATUS_DRAW||
;    1764            data == STATUS_DONE) //lcd acknowledge
	LD   R26,Y
	CPI  R26,LOW(0xF0)
	BREQ _0x1A9
	CPI  R26,LOW(0xF1)
	BREQ _0x1A9
	CPI  R26,LOW(0xF2)
	BREQ _0x1A9
	CPI  R26,LOW(0xF3)
	BRNE _0x1A8
_0x1A9:
;    1765         {
;    1766                 kbd_state = data;
	LD   R30,Y
	STS  _kbd_state_G8,R30
;    1767                 return;
	RJMP _0x48D
;    1768         }
;    1769         if(key == KEY_INVALID)
_0x1A8:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE _0x1AB
;    1770                 key = data;
	LDD  R5,Y+0
;    1771 }
_0x1AB:
	RJMP _0x48D
;    1772 static u8 lcdcmd[40];

	.DSEG
_lcdcmd_G8:
	.BYTE 0x28
;    1773 
;    1774 static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
_head_G8:
	.BYTE 0x3
;    1775 static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};
_tail_G8:
	.BYTE 0x3
;    1776 
;    1777 static u8 hex2ch[2];
_hex2ch_G8:
	.BYTE 0x2
;    1778 void hex2char(uchar a)
;    1779 {

	.CSEG
_hex2char:
;    1780         uchar h,l;
;    1781 	h = (a & 0xf0) >> 4;
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
;    1782 	l = (a & 0x0f);
	LDD  R30,Y+2
	ANDI R30,LOW(0xF)
	MOV  R16,R30
;    1783 	if(h <= 9)
	CPI  R17,10
	BRSH _0x1AE
;    1784 		hex2ch[0] = (h+'0');
	MOV  R30,R17
	SUBI R30,-LOW(48)
	RJMP _0x4A0
;    1785 	else
_0x1AE:
;    1786 		hex2ch[0] = (h+'A'-0x0a);
	MOV  R30,R17
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4A0:
	STS  _hex2ch_G8,R30
;    1787 	if(l <= 9)
	CPI  R16,10
	BRSH _0x1B0
;    1788 		hex2ch[1] = (l+'0');
	MOV  R30,R16
	SUBI R30,-LOW(48)
	RJMP _0x4A1
;    1789 	else
_0x1B0:
;    1790 		hex2ch[1] = (l+'A'-0x0a);
	MOV  R30,R16
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4A1:
	__PUTB1MN _hex2ch_G8,1
;    1791 }
	RJMP _0x490
;    1792 void onecmd(uchar len)
;    1793 {
_onecmd:
;    1794      uchar i = 0;
;    1795      uchar sum;
;    1796      sum = 0;
	ST   -Y,R17
	ST   -Y,R16
;	len -> Y+2
;	i -> R17
;	sum -> R16
	LDI  R17,0
	LDI  R16,LOW(0)
;    1797      prints(head,3,PORT_KBD); //send head
	LDI  R30,LOW(_head_G8)
	LDI  R31,HIGH(_head_G8)
	CALL SUBOPT_0x84
;    1798      for(i = 0;i<len;i++)     //caculate cksum
	LDI  R17,LOW(0)
_0x1B3:
	LDD  R30,Y+2
	CP   R17,R30
	BRSH _0x1B4
;    1799      {
;    1800         hex2char(lcdcmd[i]);
	CALL SUBOPT_0x85
	CALL SUBOPT_0x86
;    1801 	prints(hex2ch,2,PORT_KBD);
;    1802         sum = sum + lcdcmd[i];
	CALL SUBOPT_0x85
	ADD  R16,R30
;    1803      }
	SUBI R17,-1
	RJMP _0x1B3
_0x1B4:
;    1804      lcdcmd[i] = 0xff - sum;
	MOV  R26,R17
	LDI  R27,0
	SUBI R26,LOW(-_lcdcmd_G8)
	SBCI R27,HIGH(-_lcdcmd_G8)
	LDI  R30,LOW(255)
	SUB  R30,R16
	ST   X,R30
;    1805      hex2char(lcdcmd[i]);
	CALL SUBOPT_0x85
	CALL SUBOPT_0x86
;    1806      prints(hex2ch,2,PORT_KBD);
;    1807      prints(tail,3,PORT_KBD); //send tail
	LDI  R30,LOW(_tail_G8)
	LDI  R31,HIGH(_tail_G8)
	CALL SUBOPT_0x84
;    1808 }
_0x490:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,3
	RET
;    1809 static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

	.DSEG
_idle_G8:
	.BYTE 0xA
;    1810 void sendcmd(uchar len)
;    1811 {

	.CSEG
_sendcmd:
;    1812      ulong timeout;
;    1813      if(DEBUG == 1)
	SBIW R28,4
;	len -> Y+4
;	timeout -> Y+0
;    1814         return;
;    1815      while(kbd_state != STATUS_IDLE)
_0x1B7:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1B9
;    1816      {
;    1817         if(kbd_state == STATUS_DRAW)
	CPI  R26,LOW(0xF2)
	BREQ _0x1B7
;    1818                 continue;
;    1819         if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1BB
;    1820         {
;    1821                 prints(idle,10,PORT_KBD);
	LDI  R30,LOW(_idle_G8)
	LDI  R31,HIGH(_idle_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _prints
;    1822                 kbd_state = STATUS_DRAW;
	LDI  R30,LOW(242)
	STS  _kbd_state_G8,R30
;    1823         }
;    1824      }
_0x1BB:
	RJMP _0x1B7
_0x1B9:
;    1825      onecmd(len);
	CALL SUBOPT_0x87
;    1826      timeout = 0;
;    1827      kbd_state = STATUS_DONE;
	LDI  R30,LOW(243)
	STS  _kbd_state_G8,R30
;    1828      while(kbd_state != STATUS_IDLE)
_0x1BC:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1BE
;    1829      {
;    1830          if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1BF
;    1831          {
;    1832                 onecmd(len);
	CALL SUBOPT_0x87
;    1833                 timeout = 0;
;    1834          }
;    1835          if(timeout < 655350)
_0x1BF:
	__GETD2S 0
	__CPD2N 0x9FFF6
	BRSH _0x1C0
;    1836          {
;    1837                 sleepms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x32
;    1838                 timeout++;
	CALL SUBOPT_0x6
	CALL SUBOPT_0x3C
	__PUTD1S 0
;    1839          }else{
	RJMP _0x1C1
_0x1C0:
;    1840                 onecmd(len);
	CALL SUBOPT_0x87
;    1841                 timeout = 0;
;    1842          }
_0x1C1:
;    1843      }
	RJMP _0x1BC
_0x1BE:
;    1844 }
_0x48F:
	ADIW R28,5
	RET
;    1845 /*------------------初始化-----------------*/
;    1846 void LCD_Init (void)
;    1847 {
_LCD_Init:
;    1848         kbd_state = STATUS_IDLE;
	LDI  R30,LOW(240)
	STS  _kbd_state_G8,R30
;    1849         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x88
;    1850         sendcmd(1);
;    1851         //do it twice avoid the missing of first command on boot up
;    1852         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x88
;    1853         sendcmd(1);
;    1854 
;    1855 }
	RET
;    1856 
;    1857 /********************************************************/
;    1858 /* 清屏							*/
;    1859 /********************************************************/
;    1860 void LCD_Cls(void)
;    1861 {
_LCD_Cls:
;    1862         lcdcmd[0] = CMDO_LCD_CLS;
	LDI  R30,LOW(4)
	STS  _lcdcmd_G8,R30
;    1863         sendcmd(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _sendcmd
;    1864 }
	RET
;    1865 void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
;    1866 {
_LCD_ClsBlock:
;    1867         lcdcmd[0] = CMDO_LCD_CLSBLK;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(6)
	CALL SUBOPT_0x89
;    1868         lcdcmd[1] = x1;
;    1869         lcdcmd[2] = y1;
;    1870         lcdcmd[3] = x2;
;    1871         lcdcmd[4] = y2;
;    1872         sendcmd(5);
;    1873 }
	RJMP _0x48E
;    1874 void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
;    1875 {
_display_buttons:
;    1876         lcdcmd[0] = CMDO_DISPLAYBTN;
;	btn_pos -> Y+1
;	btn_val -> Y+0
	LDI  R30,LOW(2)
	STS  _lcdcmd_G8,R30
;    1877         if(btn_val == 0)
	LD   R30,Y
	CPI  R30,0
	BRNE _0x1C2
;    1878 	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
	LDD  R30,Y+1
	SUBI R30,-LOW(4)
	STD  Y+1,R30
;    1879         lcdcmd[1] = btn_pos;
_0x1C2:
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,1
;    1880         sendcmd(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _sendcmd
;    1881 }
	ADIW R28,2
	RET
;    1882 
;    1883 
;    1884 /*	设定图形x,y值*/
;    1885 /*
;    1886 void LCD_GraphSetAddr(uchar x,uchar y)
;    1887 {
;    1888 	uint xy;
;    1889 	xy=y;
;    1890 	xy=xy*16+x+256;
;    1891   	//LCD_CE(0);
;    1892 	LCD_Write2(xy&0xff,xy/256,0x24);
;    1893 }
;    1894 */
;    1895 /*	设定文本x,y值	*/
;    1896 /*
;    1897 void LCD_TextSetAddr(uchar x,uchar y)
;    1898 {
;    1899   	//LCD_CE(0);
;    1900 	LCD_Write2(y*16+x,0,0x24);
;    1901 }
;    1902 */
;    1903 /*	清除一点*/
;    1904 /*
;    1905 void LCD_ClrPixel(uchar x,uchar y)
;    1906 {
;    1907 	uchar b;
;    1908 	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;
;    1909 
;    1910 	b = 7 - (x % 8);
;    1911 
;    1912 	LCD_WriteInt(addr,0x24);
;    1913 	LCD_Comm(0xf0|b);
;    1914 }
;    1915 */
;    1916 /*	点亮一点	*/
;    1917 /*
;    1918 void LCD_Putpixel(uchar x,uchar y)
;    1919 {
;    1920 	uchar b;
;    1921 	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;
;    1922 
;    1923 	b = 7 - (x % 8);
;    1924 
;    1925 	LCD_WriteInt(addr,0x24);
;    1926 	LCD_Comm(0xf8|b);
;    1927 }
;    1928 */
;    1929 /*	x,y处显示光标	*/
;    1930 /*
;    1931 void LCD_ShowCursor(uchar x,uchar y)
;    1932 {
;    1933 	return;
;    1934   	//LCD_CE(0);
;    1935 	LCD_Comm(0x97);	//光标开
;    1936 	LCD_Write2(x,y,0x21);
;    1937 }
;    1938 
;    1939 //	取消光标
;    1940 
;    1941 void LCD_HideCursor(void)
;    1942 {
;    1943 	return;
;    1944   	//LCD_CE(0);
;    1945 	LCD_Comm(0x9c);
;    1946 }
;    1947 */
;    1948 /*
;    1949 void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
;    1950 {
;    1951 	x =(x / 8) * 8;
;    1952 
;    1953 	while(*s)
;    1954 	{
;    1955 		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
;    1956 
;    1957 		x = x + 8;
;    1958 
;    1959 		s++;
;    1960 	}
;    1961 }
;    1962 
;    1963 
;    1964 void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
;    1965 {
;    1966 	x =(x / 8) * 8;
;    1967 	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
;    1968 }
;    1969 */
;    1970 
;    1971 void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
;    1972 {
_LCD_ReverseRect:
;    1973         lcdcmd[0] = CMDO_LCD_REVERSE;
;	x -> Y+3
;	y -> Y+2
;	w -> Y+1
;	h -> Y+0
	LDI  R30,LOW(5)
	CALL SUBOPT_0x89
;    1974         lcdcmd[1] = x;
;    1975         lcdcmd[2] = y;
;    1976         lcdcmd[3] = w;
;    1977         lcdcmd[4] = h;
;    1978         sendcmd(5);
;    1979 }
	RJMP _0x48E
;    1980 /*--------------显示字符------------------*/
;    1981 /*
;    1982 void LCD_TextPutchar(uchar x,uchar y,uchar c)
;    1983 {
;    1984   	//LCD_CE(0);
;    1985   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    1986   	LCD_Comm(0xb0);
;    1987 	LCD_AutoWrite(c - 0x20);
;    1988   	LCD_Comm(0xb2);
;    1989 }
;    1990 
;    1991 void LCD_TextPrint(uchar x,uchar y,char *s)
;    1992 {
;    1993   	//LCD_CE(0);
;    1994   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    1995   	LCD_Comm(0xb0);
;    1996   	while(*s)
;    1997   	{
;    1998 		LCD_AutoWrite(*s - 0x20);
;    1999 		s++;
;    2000 	}
;    2001   	LCD_Comm(0xb2);
;    2002 }
;    2003 void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
;    2004 {
;    2005 	uchar buf[4];
;    2006 	int i;
;    2007 
;    2008 	if(start > 3) start = 0;
;    2009 
;    2010 	for(i = 0; i < 4; i++)
;    2011 	{
;    2012 		buf[i] = n % 10;
;    2013 		n /= 10;
;    2014 	}
;    2015 
;    2016 	for(i = 3-start; i >= 0; i--)
;    2017 	{
;    2018 		LCD_TextPutchar(x,y,'0' + buf[i]);
;    2019 		x ++;
;    2020 	}
;    2021 }
;    2022 
;    2023 void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
;    2024 {
;    2025 	uchar c1,c2;
;    2026 	//低4位
;    2027 	c1 = hex & 0x0f;	//lo
;    2028 	//高4位
;    2029 	c2 = (hex >> 4) & 0x0f; //hi
;    2030 
;    2031 	LCD_TextPutchar(x,y,HexTable[c2]);
;    2032 	LCD_TextPutchar(x+1,y,HexTable[c1]);
;    2033 }
;    2034 */
;    2035 /************************************************/
;    2036 /*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
;    2037 /************************************************/
;    2038 /*
;    2039 void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
;    2040 {
;    2041 	register uchar t;
;    2042 	int xerr=0,yerr=0,delta_x,delta_y,distance;
;    2043 	int incx,incy,uRow,uCol;
;    2044 
;    2045 	delta_x = xt-x0;				//计算坐标增量
;    2046 	delta_y = yt-y0;
;    2047 	uRow = x0;
;    2048 	uCol = y0;
;    2049 	if(delta_x>0) incx=1;				//设置单步方向
;    2050 	else if( delta_x==0 ) incx=0;			//垂直线
;    2051 	else {incx=-1;delta_x=-delta_x;}
;    2052 
;    2053 	if(delta_y>0) incy=1;
;    2054 	else if( delta_y==0 ) incy=0;			//水平线
;    2055 	else {incy=-1;delta_y=-delta_y;}
;    2056 
;    2057 	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
;    2058 	else distance=delta_y;
;    2059 
;    2060   	//LCD_CE(0);
;    2061 	for( t=0;t <= distance+1; t++ )
;    2062         {					//画线输出
;    2063 		LCD_Putpixel(uRow,uCol);			//画点
;    2064 		xerr +=	delta_x	;
;    2065 		yerr +=	delta_y	;
;    2066 
;    2067 		if( xerr > distance )
;    2068                	{
;    2069 			xerr-=distance;
;    2070 			uRow+=incx;
;    2071 		}
;    2072 		if( yerr > distance )
;    2073                	{
;    2074 			yerr-=distance;
;    2075 			uCol+=incy;
;    2076 		}
;    2077 	}
;    2078 }
;    2079 */
;    2080 /*
;    2081 void LCD_LineH(uchar y)
;    2082 {
;    2083 	char i;
;    2084 
;    2085   	//LCD_CE(0);
;    2086   	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
;    2087 	LCD_Comm(0xb0);
;    2088 	for(i=0;i<LCD_WIDTH;i++)
;    2089  		LCD_AutoWrite(0xff);
;    2090 	LCD_Comm(0xb2);
;    2091 }
;    2092 */
;    2093 /*
;    2094 void LCD_LineV(uchar x,uchar y1,uchar y2)
;    2095 {
;    2096 	int i;
;    2097   	//LCD_CE(0);
;    2098 	for(i = y1; i < y2; i++)
;    2099 	{
;    2100 		LCD_Putpixel(x,i);
;    2101 	}
;    2102 }
;    2103 */
;    2104 
;    2105 void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
;    2106 {
_LCD_Rectange:
;    2107         lcdcmd[0] = CMDO_LCD_RECT;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(7)
	CALL SUBOPT_0x89
;    2108         lcdcmd[1] = x1;
;    2109         lcdcmd[2] = y1;
;    2110         lcdcmd[3] = x2;
;    2111         lcdcmd[4] = y2;
;    2112         sendcmd(5);
;    2113 }
	RJMP _0x48E
;    2114 void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
;    2115 {
_LCD_PrintChar:
;    2116         uchar pos = 0;
;    2117         lcdcmd[0] = cmd;
	ST   -Y,R17
;	cmd -> Y+5
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	pos -> R17
	LDI  R17,0
	LDD  R30,Y+5
	STS  _lcdcmd_G8,R30
;    2118         lcdcmd[1] = x;
	LDD  R30,Y+4
	__PUTB1MN _lcdcmd_G8,1
;    2119         lcdcmd[2] = y;
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,2
;    2120         pos = 1;
	LDI  R17,LOW(1)
;    2121         while(*s)
_0x1C3:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x1C5
;    2122         {
;    2123                 lcdcmd[pos + 3] = *s++;
	CALL SUBOPT_0x8A
	MOVW R0,R30
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOVW R26,R0
	ST   X,R30
;    2124                 pos = pos + 1;
	SUBI R17,-LOW(1)
;    2125         }
	RJMP _0x1C3
_0x1C5:
;    2126         lcdcmd[pos+3] = 0x00;
	CALL SUBOPT_0x8A
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2127         lcdcmd[3] = pos;
	__POINTW1MN _lcdcmd_G8,3
	ST   Z,R17
;    2128         pos = pos + 4;
	SUBI R17,-LOW(4)
;    2129         sendcmd(pos);
	ST   -Y,R17
	CALL _sendcmd
;    2130 }
	LDD  R17,Y+0
	ADIW R28,6
	RET
;    2131 void LCD_PrintHz12(uchar x,uchar y,uchar *s)
;    2132 {
_LCD_PrintHz12:
;    2133         LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(8)
	CALL SUBOPT_0x8B
;    2134 }
	RJMP _0x48E
;    2135 void LCD_PrintHz16(uchar x,uchar y,uchar *s)
;    2136 {
_LCD_PrintHz16:
;    2137         LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(9)
	CALL SUBOPT_0x8B
;    2138 }
	RJMP _0x48E
;    2139 //显示6x8的数字
;    2140 void LCD_Print6X8(uchar x, uchar y,uchar *s)
;    2141 {
_LCD_Print6X8:
;    2142         LCD_PrintChar(CMDO_LCD_68,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(16)
	CALL SUBOPT_0x8B
;    2143 }
	RJMP _0x48E
;    2144 
;    2145 //显示6x8的数字
;    2146 void LCD_Print8X16(uchar x, uchar y,uchar *s)
;    2147 {
_LCD_Print8X16:
;    2148         LCD_PrintChar(CMDO_LCD_816,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(17)
	CALL SUBOPT_0x8B
;    2149 }
_0x48E:
	ADIW R28,4
	RET
;    2150 
;    2151 //显示24x32的数字
;    2152 /*
;    2153 void LCD_Print24X32(uchar x, uchar y,uchar *s)
;    2154 {
;    2155 	//x坐标必需是8位对齐
;    2156 	x =(x / 8) * 8;
;    2157 	while(*s)
;    2158 	{
;    2159 		if( *s >= '0' && *s <= '9')	//显示
;    2160 		{
;    2161 			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
;    2162 			x += 24;
;    2163 		}
;    2164 		else if( *s == ' ')
;    2165 		{
;    2166 			x += 8;
;    2167 		}
;    2168 		else if( *s == '-')
;    2169 		{
;    2170 			LCD_PutImg(x,y,3,32,Font24X32[12]);
;    2171 			x += 24;
;    2172 		}
;    2173 		else
;    2174 		{
;    2175 			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
;    2176 			x += 8;
;    2177 		}
;    2178 		s ++;
;    2179 	}
;    2180 }*/
;    2181 #include "utili.h"
;    2182 #include "scanner.h"
;    2183 #include "stdlib.h"
;    2184 
;    2185 static uchar navlen = 0;

	.DSEG
_navlen_G9:
	.BYTE 0x1
;    2186 static double reading = -1000;
_reading_G9:
	.BYTE 0x4
;    2187 static char navread[20];
_navread_G9:
	.BYTE 0x14
;    2188 static char navread2[20];
_navread2_G9:
	.BYTE 0x14
;    2189 u8 scancmd[5];
_scancmd:
	.BYTE 0x5
;    2190 void scanner_set_mode()
;    2191 {

	.CSEG
_scanner_set_mode:
;    2192         if (IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x1C7
;    2193         {
;    2194             scancmd[0] = '#';
	LDI  R30,LOW(35)
	RJMP _0x4A2
;    2195         }else{
_0x1C7:
;    2196             scancmd[0]= '!';
	LDI  R30,LOW(33)
_0x4A2:
	STS  _scancmd,R30
;    2197         }
;    2198         scancmd[1] = 0x0D;
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2199         scancmd[2] = 0x0A;
	__POINTW1MN _scancmd,2
	CALL SUBOPT_0x8C
;    2200         prints(scancmd,3,PORT_SCANNER);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x8D
;    2201 }
	RET
;    2202 void scanner_set_channel(uchar ch)
;    2203 {
_scanner_set_channel:
;    2204         if(ch < 10)
;	ch -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRSH _0x1C9
;    2205         {
;    2206                 scancmd[0] = (ch + '0');
	LD   R30,Y
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2207                 scancmd[1] = 'A';
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    2208                 scancmd[2] = 0x0D;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2209                 scancmd[3] = 0x0A;
	__POINTW1MN _scancmd,3
	CALL SUBOPT_0x8C
;    2210                 prints(scancmd,4,PORT_SCANNER);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x8D
;    2211                 return;
	RJMP _0x48D
;    2212         }
;    2213 
;    2214         scancmd[0] = (u8)(ch / 10) + '0';
_0x1C9:
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2215         ch = ch % 10;
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __MODB21U
	ST   Y,R30
;    2216         scancmd[1] = ch + '0';
	SUBI R30,-LOW(48)
	__PUTB1MN _scancmd,1
;    2217         scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(65)
	STD  Z+0,R26
	__POINTW1MN _scancmd,3
	LDI  R26,LOW(13)
	STD  Z+0,R26
	__POINTW1MN _scancmd,4
	CALL SUBOPT_0x8C
;    2218         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x8D
;    2219 }
	RJMP _0x48D
;    2220 
;    2221 //incoming data handler of scanner
;    2222 void scanner_uart_push(uchar data)
;    2223 {
_scanner_uart_push:
;    2224 }
	RJMP _0x48D
;    2225 //incoming data hander of PC
;    2226 void pc_uart_push(uchar data)
;    2227 {
_pc_uart_push:
;    2228 }
	RJMP _0x48D
;    2229 static uchar navcmd[12];

	.DSEG
_navcmd_G9:
	.BYTE 0xC
;    2230 #define NAV_INVALID     -1000
;    2231 #define NAV_VALID       -2000
;    2232 #define RESETNAV       navlen = 0;   reading = NAV_INVALID;
;    2233 
;    2234 void nav_command(uchar cmd)
;    2235 {

	.CSEG
_nav_command:
;    2236         if(cmd == NAV_30V)
;	cmd -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1CA
;    2237                sprintf(navcmd,"%%01;12;02\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,103
	CALL SUBOPT_0x2E
;    2238         if(cmd == NAV_1V)
_0x1CA:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x1CB
;    2239                sprintf(navcmd,"%%01;12;01\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,115
	CALL SUBOPT_0x2E
;    2240         if(cmd == NAV_120MV)
_0x1CB:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x1CC
;    2241                sprintf(navcmd,"%%01;12;00\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,127
	CALL SUBOPT_0x2E
;    2242         if(cmd == NAV_AFLTOFF)
_0x1CC:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x1CD
;    2243                sprintf(navcmd,"%%01;26\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,139
	CALL SUBOPT_0x2E
;    2244         if(cmd == NAV_AFLTON)
_0x1CD:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x1CE
;    2245                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,148
	CALL SUBOPT_0x2E
;    2246         if(cmd == NAV_SLOWMODE)
_0x1CE:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x1CF
;    2247                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,148
	CALL SUBOPT_0x2E
;    2248         if(cmd == NAV_ZEROON)
_0x1CF:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x1D0
;    2249         {
;    2250                sprintf(navcmd,"%%01;06\r");
	CALL SUBOPT_0x8E
	__POINTW1FN _0,157
	CALL SUBOPT_0x2E
;    2251         }
;    2252         if(cmd == NAV_INIT)
_0x1D0:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1D1
;    2253         {
;    2254                 RESETNAV;
	CALL SUBOPT_0x8F
;    2255                 sprintf(navcmd,"%%01;00\r");
	__POINTW1FN _0,166
	CALL SUBOPT_0x2E
;    2256         }
;    2257         if(cmd == NAV_READ)
_0x1D1:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0x1D2
;    2258         {
;    2259                 RESETNAV;
	CALL SUBOPT_0x8F
;    2260                 sprintf(navcmd,"%%01;01\r");
	__POINTW1FN _0,175
	CALL SUBOPT_0x2E
;    2261         }
;    2262 //                com1_putc('?');
;    2263         prints(navcmd,strlen(navcmd),PORT_NAV);
_0x1D2:
	CALL SUBOPT_0x8E
	CALL SUBOPT_0x8E
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _prints
;    2264 }
	RJMP _0x48D
;    2265 
;    2266 
;    2267 unsigned long navtime;

	.DSEG
_navtime:
	.BYTE 0x4
;    2268 double nav_read()
;    2269 {

	.CSEG
_nav_read:
;    2270         if(DEBUG == 1)
;    2271                 return 1.0;
;    2272         nav_command(NAV_READ);
	CALL SUBOPT_0x90
;    2273         navtime = 0;
;    2274         while(1)
_0x1D4:
;    2275         {
;    2276                 if(reading < NAV_INVALID) //make sure it is a valid reading
	LDS  R26,_reading_G9
	LDS  R27,_reading_G9+1
	LDS  R24,_reading_G9+2
	LDS  R25,_reading_G9+3
	CALL SUBOPT_0x91
	CALL __CMPF12
	BRSH _0x1D7
;    2277                 {
;    2278                      reading = atof(navread2);
	LDI  R30,LOW(_navread2_G9)
	LDI  R31,HIGH(_navread2_G9)
	ST   -Y,R31
	ST   -Y,R30
	CALL _atof
	CALL SUBOPT_0x92
;    2279                      return reading;
	LDS  R30,_reading_G9
	LDS  R31,_reading_G9+1
	LDS  R22,_reading_G9+2
	LDS  R23,_reading_G9+3
	RET
;    2280                 }
;    2281                 if(navtime++ > 655350)
_0x1D7:
	LDS  R30,_navtime
	LDS  R31,_navtime+1
	LDS  R22,_navtime+2
	LDS  R23,_navtime+3
	CALL SUBOPT_0x3C
	STS  _navtime,R30
	STS  _navtime+1,R31
	STS  _navtime+2,R22
	STS  _navtime+3,R23
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	__CPD1N 0x9FFF7
	BRLO _0x1D8
;    2282                 {
;    2283                      nav_command(NAV_READ);
	CALL SUBOPT_0x90
;    2284                      navtime = 0;
;    2285                 }
;    2286                 sleepms(1);
_0x1D8:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x32
;    2287         }
	RJMP _0x1D4
;    2288         return 1.0;
;    2289 }
;    2290 //incoming data hander of navameter
;    2291 void nav_uart_push(uchar data)
;    2292 {
_nav_uart_push:
;    2293         if(navlen >= 19)
;	data -> Y+0
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x13)
	BRLO _0x1D9
;    2294         {
;    2295                 RESETNAV;
	CALL SUBOPT_0x93
;    2296         }
;    2297         if(((data >= '0') && (data <= '9')) ||
_0x1D9:
;    2298                 (data == '.') ||
;    2299                 (data == '-') ||
;    2300                 (data == '+') ||
;    2301                 (data == 'e') ||
;    2302                 (data == 'E') )
	LD   R26,Y
	CPI  R26,LOW(0x30)
	BRLO _0x1DB
	CPI  R26,LOW(0x3A)
	BRLO _0x1DD
_0x1DB:
	LD   R26,Y
	CPI  R26,LOW(0x2E)
	BREQ _0x1DD
	CPI  R26,LOW(0x2D)
	BREQ _0x1DD
	CPI  R26,LOW(0x2B)
	BREQ _0x1DD
	CPI  R26,LOW(0x65)
	BREQ _0x1DD
	CPI  R26,LOW(0x45)
	BRNE _0x1DA
_0x1DD:
;    2303         {
;    2304                 navread[navlen++] = data;
	LDS  R30,_navlen_G9
	SUBI R30,-LOW(1)
	STS  _navlen_G9,R30
	CALL SUBOPT_0x42
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	LD   R26,Y
	STD  Z+0,R26
;    2305                 reading = NAV_INVALID;
	CALL SUBOPT_0x91
	CALL SUBOPT_0x92
;    2306                 return;
	RJMP _0x48D
;    2307         }
;    2308 
;    2309         if(navlen < 4) //not enough digits
_0x1DA:
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x4)
	BRSH _0x1DF
;    2310         {
;    2311                 RESETNAV;
	CALL SUBOPT_0x93
;    2312                 return;
	RJMP _0x48D
;    2313         }
;    2314         navread[navlen] = '\0';
_0x1DF:
	CALL SUBOPT_0x94
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2315         do
_0x1E1:
;    2316         {
;    2317                 navread2[navlen] = navread[navlen];
	LDS  R26,_navlen_G9
	LDI  R27,0
	SUBI R26,LOW(-_navread2_G9)
	SBCI R27,HIGH(-_navread2_G9)
	CALL SUBOPT_0x94
	LD   R30,Z
	ST   X,R30
;    2318         }while(navlen-- > 0);
	LDS  R26,_navlen_G9
	SUBI R26,LOW(1)
	STS  _navlen_G9,R26
	SUBI R26,-LOW(1)
	CPI  R26,LOW(0x1)
	BRSH _0x1E1
;    2319         navlen = 0;
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
;    2320         reading =  NAV_VALID; //valid data
	__GETD1N 0xC4FA0000
	CALL SUBOPT_0x92
;    2321 }
_0x48D:
	ADIW R28,1
	RET
;    2322 #include <mega64.h>
;    2323 #include "lcd.h"
;    2324 #include "key.h"
;    2325 #include "utili.h"
;    2326 #include "eeprom.h"
;    2327 #include "window.h"
;    2328 #include "utili.h"
;    2329 #include "scanner.h"
;    2330 
;    2331 extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
;    2332 #define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
;    2333                 		if(new_page == 1){\
;    2334 		                	for(i = min_option; i <= max_option; i++){\
;    2335 				                j = ((curr_pos-1)/max_option)*max_option + i;
;    2336 
;    2337 #define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
;    2338 				        if((curr_pos-1) % max_option == (i-1))\
;    2339 					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2340         			        }\
;    2341                                 }else{\
;    2342 	                		for(i = min_option; i <= max_option; i++) {\
;    2343         		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2344 				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2345 			                }\
;    2346 		                }
;    2347 
;    2348 #define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
;    2349 			if(curr_pos > max_index) curr_pos = min_index;\
;    2350 			if(last_pos == curr_pos)	return;\
;    2351 			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}
;    2352 
;    2353 #define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
;    2354 			if(curr_pos < min_index) curr_pos = max_index;\
;    2355 			if(last_pos == curr_pos)  return;\
;    2356 			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}
;    2357 
;    2358 #define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
;    2359 					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
;    2360 
;    2361 #define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
;    2362                                         draw_label(&PRE##options[i-1], SW_NORMAL);\
;    2363 			        }
;    2364 #define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
;    2365                                         return;\
;    2366         			for(i = min_option;i <= max_option; i++){\
;    2367 	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2368         				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2369         			}
;    2370 
;    2371 #define INC_OPTION	last_sel = curr_sel;\
;    2372 			curr_sel++;\
;    2373 			if(curr_sel > max_option)\
;    2374 				curr_sel = min_option;
;    2375 
;    2376 #define DEC_OPTION	last_sel = curr_sel;\
;    2377 			curr_sel--;\
;    2378 			if(curr_sel < min_option)\
;    2379 				curr_sel = max_option;
;    2380 
;    2381 extern int   curr_ch;	//index of current channel in configuation   menu
;    2382 extern int   curr_prb;	//index of current probe selection
;    2383 extern int   curr_dispch; //start of current channel for display in main menu
;    2384 
;    2385 void State_Init() {
_State_Init:
;    2386 	display_buttons(KEY_BTN1,sysdata.prbmode);
	LDI  R30,LOW(97)
	ST   -Y,R30
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2387 	display_buttons(KEY_BTN2,sysdata.kttmode);
	LDI  R30,LOW(98)
	ST   -Y,R30
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2388 	display_buttons(KEY_BTN3,0); //probe type
	LDI  R30,LOW(99)
	CALL SUBOPT_0x1F
;    2389 	display_buttons(KEY_BTN4,0); //zero
	LDI  R30,LOW(100)
	CALL SUBOPT_0x1F
;    2390 }
	RET
;    2391 char* getprbtype(uchar);
;    2392 LABEL flash pgmain_chs[] = {
;    2393 		{LBL_HZ6X8,3, 3,5,strbuf},
;    2394 		{LBL_HZ6X8,3,16,5,strbuf},
;    2395 		{LBL_HZ6X8,3,29,5,strbuf},
;    2396 		{LBL_HZ6X8,3,42,5,strbuf}
;    2397 	};
;    2398 LABEL flash pgmain_values[] = {
;    2399 		{LBL_HZ6X8,40,3,10,strbuf},
;    2400 		{LBL_HZ6X8,40,16,10,strbuf},
;    2401 		{LBL_HZ6X8,40,29,10,strbuf},
;    2402 		{LBL_HZ6X8,40,42,10,strbuf}
;    2403 	};
;    2404 LABEL flash pgmain_temps[] = {
;    2405 		{LBL_HZ6X8,140,3, 10,strbuf},
;    2406 		{LBL_HZ6X8,140,16,10,strbuf},
;    2407 		{LBL_HZ6X8,140,29,10,strbuf},
;    2408 		{LBL_HZ6X8,140,42,10,strbuf}
;    2409 };
;    2410 /*      Main display      */
;    2411 
;    2412 void pgmain_handler(uchar msg) {
_pgmain_handler:
;    2413         int max_option = 4;
;    2414         uchar i,old_ch,isinit;
;    2415                 uchar *cptr;
;    2416 	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
;    2417 
;    2418 	isinit = 0;
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
;    2419         //Menu
;    2420 	if(msg == KEY_TAB) {
	LDD  R26,Y+8
	CPI  R26,LOW(0x54)
	BRNE _0x1E3
;    2421 		if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x1E4
;    2422 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4A3
;    2423 		else
_0x1E4:
;    2424 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4A3:
	MOV  R4,R30
;    2425 		return;
	RJMP _0x48C
;    2426 	}
;    2427         if(key == KEY_NUM1) //R0
_0x1E3:
	LDI  R30,LOW(49)
	CP   R30,R5
	BRNE _0x1E6
;    2428         {
;    2429                 window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    2430                 if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x1E7
;    2431                 {
;    2432                         sprintf(strbuf,"请输入铂电阻R0阻值");
	CALL SUBOPT_0x0
	__POINTW1FN _0,184
	CALL SUBOPT_0x2E
;    2433 	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
	CALL SUBOPT_0x2A
	CALL SUBOPT_0x95
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	RJMP _0x4A4
;    2434 		        msg = MSG_INIT;
;    2435 		}else{
_0x1E7:
;    2436 		        return;
	RJMP _0x48C
;    2437                         sprintf(strbuf,"请输入V0值");
;    2438 	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
_0x4A4:
	CALL __EEPROMWRD
;    2439                         msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2440 		}
;    2441         }
;    2442         if(key == KEY_NUM2) //Rs1
_0x1E6:
	LDI  R30,LOW(50)
	CP   R30,R5
	BRNE _0x1E9
;    2443         {
;    2444                 if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x1EA
;    2445                 {
;    2446                         window_setup(10);
	CALL SUBOPT_0x96
;    2447                         sprintf(strbuf,"请输入内标准阻值");
	__POINTW1FN _0,214
	CALL SUBOPT_0x2E
;    2448 		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x28
	CALL SUBOPT_0x95
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    2449         		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2450                 }
;    2451         }
_0x1EA:
;    2452 	if(msg == KEY_UP) {
_0x1E9:
	LDD  R26,Y+8
	CPI  R26,LOW(0x55)
	BRNE _0x1EB
;    2453 		DEC_DISPCH;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	__SUBWRR 10,11,30,31
	MOV  R0,R10
	OR   R0,R11
	BRNE _0x1EC
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	MOVW R10,R30
_0x1EC:
;    2454 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2455 	}
;    2456 	if(msg == KEY_DN) {
_0x1EB:
	LDD  R26,Y+8
	CPI  R26,LOW(0x44)
	BRNE _0x1ED
;    2457 		INC_DISPCH;
	CALL SUBOPT_0x97
	BRGE _0x1EE
	CALL SUBOPT_0x35
_0x1EE:
;    2458 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2459 	}
;    2460 
;    2461 	if(msg == MSG_INIT) {
_0x1ED:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFF)
	BRNE _0x1EF
;    2462 	        isinit = 1;
	LDI  R21,LOW(1)
;    2463 		LCD_Cls();
	CALL _LCD_Cls
;    2464 		old_ch = curr_dispch;
	CALL SUBOPT_0x98
;    2465 
;    2466 	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2467 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1F1:
	CPI  R19,4
	BRSH _0x1F2
;    2468 		{
;    2469 		        if(curr_dispch == old_ch)
	MOV  R30,R18
	MOVW R26,R10
	CALL SUBOPT_0x99
	BRNE _0x1F3
;    2470 		        {
;    2471         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9A
;    2472                                 draw_label(&pgmain_chs[i],SW_NORMAL);
	CALL SUBOPT_0x31
;    2473                                 draw_label(&pgmain_chs[i],SW_REVERSE);
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	RJMP _0x4A5
;    2474         		}
;    2475         		else{
_0x1F3:
;    2476         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9A
;    2477         			draw_label(&pgmain_chs[i],SW_NORMAL);
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
_0x4A5:
	ST   -Y,R30
	CALL _draw_label
;    2478         		}
;    2479 			INC_DISPCH;
	CALL SUBOPT_0x97
	BRGE _0x1F5
	CALL SUBOPT_0x35
_0x1F5:
;    2480 		}
	SUBI R19,-1
	RJMP _0x1F1
_0x1F2:
;    2481 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2482 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+8,R30
;    2483 	}
;    2484 
;    2485 	if(msg == MSG_REFRESH) {
_0x1EF:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x1F6
;    2486 		old_ch = curr_dispch;
	CALL SUBOPT_0x98
;    2487 		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2488 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1F8:
	CPI  R19,4
	BRLO PC+3
	JMP _0x1F9
;    2489 		{
;    2490 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BREQ PC+3
	JMP _0x1FA
;    2491 		        {
;    2492 			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
	CALL SUBOPT_0x3A
	__POINTW2MN _tprbdata,480
	CALL SUBOPT_0x9B
;    2493         			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2494 	        			(cptr[0] == 'P')){
	CALL SUBOPT_0x3A
	CPI  R30,LOW(0xFF)
	BREQ _0x1FC
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BRNE _0x1FB
_0x1FC:
;    2495                				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9C
;    2496                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0x9D
	BREQ _0x1FF
	CPI  R21,1
	BRNE _0x1FE
_0x1FF:
;    2497                                         {
;    2498                         			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0x31
;    2499 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(1)
	RJMP _0x4A6
;    2500 		                        }else{
_0x1FE:
;    2501                         			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0xA0
;    2502 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(5)
_0x4A6:
	ST   -Y,R30
	CALL _draw_label
;    2503 		                        }
;    2504 			        }else{
	RJMP _0x202
_0x1FB:
;    2505 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA1
	CALL SUBOPT_0xA2
;    2506 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0x9D
	BRNE _0x203
;    2507         					sprintf(strbuf+8,"*mV");
	CALL SUBOPT_0xA3
	__POINTW1FN _0,254
	RJMP _0x4A7
;    2508                                         else
_0x203:
;    2509                                                 sprintf(strbuf+8," mV");
	CALL SUBOPT_0xA3
	__POINTW1FN _0,258
_0x4A7:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA4
;    2510                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0x9D
	BREQ _0x206
	CPI  R21,1
	BRNE _0x205
_0x206:
;    2511                                         {
;    2512         					draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0x31
;    2513         					if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0xA5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xA6
	BRSH _0x208
;    2514         					        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,262
	CALL SUBOPT_0x2E
;    2515         					else
	RJMP _0x209
_0x208:
;    2516                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA7
	CALL SUBOPT_0x11
	CALL SUBOPT_0xA2
;    2517 
;    2518                 				sprintf(strbuf+8," ! %c",cptr[0]);
_0x209:
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA8
;    2519                 			        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(1)
	RJMP _0x4A8
;    2520                                         }else{
_0x205:
;    2521                                                 draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0xA0
;    2522                 				if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0xA5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xA6
	BRSH _0x20B
;    2523                 				{
;    2524                 				        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,262
	CALL SUBOPT_0x2E
;    2525                 				}else{
	RJMP _0x20C
_0x20B:
;    2526                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA7
	CALL SUBOPT_0x11
	CALL SUBOPT_0xA2
;    2527                 				}
_0x20C:
;    2528         				        sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA8
;    2529 	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(5)
_0x4A8:
	ST   -Y,R30
	CALL _draw_label
;    2530                                         }
;    2531 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xA9
	BRNE _0x20D
;    2532         				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4A9
;    2533         				else                                     //Hz12
_0x20D:
;    2534                 				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	SBIW R30,2
	ADIW R30,16
_0x4A9:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2535 	                        }
_0x202:
;    2536 			}else{
	RJMP _0x20F
_0x1FA:
;    2537        			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
	CALL SUBOPT_0xAC
	__POINTW2MN _rprbdata,480
	CALL SUBOPT_0x9B
;    2538         			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2539         				(rundata.temperature[curr_dispch - 1] < -9000) ||\
;    2540 	        			(cptr[0] != 'P'))
	CALL SUBOPT_0xAC
	CPI  R30,LOW(0xFF)
	BREQ _0x211
	CALL SUBOPT_0xA5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xA6
	BRLO _0x211
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BREQ _0x210
_0x211:
;    2541 	        		{
;    2542 	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
	CALL SUBOPT_0xAC
	CPI  R30,LOW(0xFF)
	BRNE _0x213
;    2543                 				sprintf(strbuf," ------- ohm");
	CALL SUBOPT_0x0
	__POINTW1FN _0,283
	CALL SUBOPT_0x2E
;    2544                 			if(rundata.temperature[curr_dispch - 1] < -9000)
_0x213:
	CALL SUBOPT_0xA5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xA6
	BRSH _0x214
;    2545                         			sprintf(strbuf," NO READ    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,296
	RJMP _0x4AA
;    2546                                         else
_0x214:
;    2547                                         	sprintf(strbuf," -------    ");
	CALL SUBOPT_0x0
	__POINTW1FN _0,309
_0x4AA:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA4
;    2548                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0x9D
	BREQ _0x217
	CPI  R21,1
	BRNE _0x216
_0x217:
;    2549                                         {
;    2550         	        			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0x31
;    2551                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9C
;    2552 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(1)
	RJMP _0x4AB
;    2553 		                	}else{
_0x216:
;    2554         	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0xA0
;    2555                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x0
	CALL SUBOPT_0x9C
;    2556 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(5)
_0x4AB:
	ST   -Y,R30
	CALL _draw_label
;    2557 		                	}
;    2558 			        }else{
	RJMP _0x21A
_0x210:
;    2559 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA1
	CALL SUBOPT_0xA2
;    2560 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0x9D
	BRNE _0x21B
;    2561         					sprintf(strbuf+8,"*ohm");
	CALL SUBOPT_0xA3
	__POINTW1FN _0,322
	RJMP _0x4AC
;    2562                                         else
_0x21B:
;    2563         					sprintf(strbuf+8," ohm");
	CALL SUBOPT_0xA3
	__POINTW1FN _0,291
_0x4AC:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xA4
;    2564                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0x9D
	BREQ _0x21E
	CPI  R21,1
	BRNE _0x21D
_0x21E:
;    2565                                         {
;    2566 	        			        draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0x31
;    2567         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA7
	CALL SUBOPT_0x11
	CALL SUBOPT_0xA2
;    2568 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA8
;    2569 		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(1)
	RJMP _0x4AD
;    2570 		        		 }else{
_0x21D:
;    2571 	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9E
	CALL SUBOPT_0xA0
;    2572         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xA7
	CALL SUBOPT_0x11
	CALL SUBOPT_0xA2
;    2573 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xA3
	CALL SUBOPT_0xA8
;    2574 		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0x9F
	LDI  R30,LOW(5)
_0x4AD:
	ST   -Y,R30
	CALL _draw_label
;    2575 		        		 }
;    2576 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xA9
	BRNE _0x221
;    2577         				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4AE
;    2578         				else                                     //Hz12
_0x221:
;    2579                 				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
	SBIW R30,2
	ADIW R30,16
_0x4AE:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2580         			}
_0x21A:
;    2581 	                }
_0x20F:
;    2582 			INC_DISPCH;
	CALL SUBOPT_0x97
	BRGE _0x223
	CALL SUBOPT_0x35
_0x223:
;    2583 		}
	SUBI R19,-1
	RJMP _0x1F8
_0x1F9:
;    2584 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2585 	}
;    2586 }
_0x1F6:
_0x48C:
	CALL __LOADLOCR6
	ADIW R28,9
	RET
;    2587 
;    2588 //main configuration window of R
;    2589 LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
;    2590 LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
;    2591 LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
;    2592 LABEL flash pgr_options[] = {
;    2593 	{LBL_HZ16, 10,23,6,"1.电流换向"},
;    2594 	{LBL_HZ16,130,23,7,"2.内标准校准"},
;    2595 	{LBL_HZ16, 10,45,8,"3.通道探头选择"},
;    2596 	{LBL_HZ16,130,45,7,"4.设置探头参数"}
;    2597 };
;    2598 
;    2599 //LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2600 #define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
;    2601 void pgcalibrate()
;    2602 {
_pgcalibrate:
;    2603         double oldvalue = 0;
;    2604         uint cnt;
;    2605         window_setup(10);
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x22A*2)
	LDI  R31,HIGH(_0x22A*2)
	CALL __INITLOCB
	ST   -Y,R17
	ST   -Y,R16
;	oldvalue -> Y+2
;	cnt -> R16,R17
	CALL SUBOPT_0x96
;    2606 	sprintf(strbuf,"请输入外标准(CH1)阻值");
	__POINTW1FN _0,412
	CALL SUBOPT_0x2E
;    2607 	oldvalue = wnd_floatinput(oldvalue);
	CALL SUBOPT_0x5D
	CALL _wnd_floatinput
	CALL SUBOPT_0xAD
;    2608 	if(oldvalue == 0)
	CALL __CPD10
	BRNE _0x22B
;    2609                 return;
	RJMP _0x48B
;    2610         wnd_msgbox(&pgr_calibrate);
_0x22B:
	LDI  R30,LOW(_pgr_calibrate*2)
	LDI  R31,HIGH(_pgr_calibrate*2)
	CALL SUBOPT_0x33
;    2611         sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2A
	CALL SUBOPT_0x4F
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x28
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
	CALL SUBOPT_0x15
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2A
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
;    2612 }
_0x48B:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    2613 //main menu of bore config
;    2614 void pgrconfig_handler(uchar msg) {
_pgrconfig_handler:
;    2615 	uchar i;
;    2616 
;    2617 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4B:
	.BYTE 0x1

	.CSEG
;    2618 	static uchar last_sel = 1;

	.DSEG
_last_sel_S4B:
	.BYTE 0x1

	.CSEG
;    2619 	uchar min_option = 1;
;    2620 	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);
;    2621 
;    2622 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x22F
	CPI  R26,LOW(0x43)
	BREQ _0x22F
	CPI  R26,LOW(0x4F)
	BRNE _0x22E
_0x22F:
;    2623 	        SET_BORE_MODE;
	CALL SUBOPT_0x34
;    2624 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2625 		return;
	RJMP _0x48A
;    2626 	}
;    2627 	if(msg == MSG_INIT) {
_0x22E:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x231
;    2628 		LCD_Cls();
	CALL _LCD_Cls
;    2629 		draw_label(&pgr_banner, SW_NORMAL);
	LDI  R30,LOW(_pgr_banner*2)
	LDI  R31,HIGH(_pgr_banner*2)
	CALL SUBOPT_0x31
;    2630 		INIT_OPTIONS(pgr_);
	MOV  R17,R16
_0x233:
	CP   R19,R17
	BRLO _0x234
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x31
	SUBI R17,-1
	RJMP _0x233
_0x234:
;    2631 		sprintf(strbuf,"(%i)",sysdata.ktime);
	CALL SUBOPT_0x0
	__POINTW1FN _0,434
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	CALL SUBOPT_0xAF
;    2632 		draw_label(&pgr_klbl, SW_NORMAL);
	LDI  R30,LOW(_pgr_klbl*2)
	LDI  R31,HIGH(_pgr_klbl*2)
	CALL SUBOPT_0x31
;    2633 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2634 	}
;    2635 	if(msg == KEY_NUM1) {
_0x231:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x235
;    2636 		window_setup(4); //4 char at max
	LDI  R30,LOW(4)
	STS  _max_databuf,R30
;    2637 		sprintf(strbuf,"电流换向时间设置");
	CALL SUBOPT_0x0
	__POINTW1FN _0,439
	CALL SUBOPT_0x2E
;    2638 		sysdata.ktime = wnd_intinput(sysdata.ktime);
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDB
	ST   -Y,R30
	RCALL _wnd_intinput
	__POINTW2MN _sysdata,12
	LDI  R31,0
	CALL __EEPROMWRW
;    2639 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2640 		return;
	RJMP _0x48A
;    2641 	}
;    2642 	if(msg == KEY_NUM2)
_0x235:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x236
;    2643 	{
;    2644                 pgcalibrate();
	CALL _pgcalibrate
;    2645 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2646 		return;
	RJMP _0x48A
;    2647 	}
;    2648 	if(msg == KEY_NUM3) {
_0x236:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x237
;    2649 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2650 		return;
	RJMP _0x48A
;    2651 	}
;    2652 	if(msg == KEY_NUM4) {
_0x237:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x238
;    2653 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2654 		return;
	RJMP _0x48A
;    2655 	}
;    2656 	if(msg == MSG_REFRESH) {
_0x238:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x239
;    2657 		REFRESH_OPTIONS(pgr_);
	LDS  R30,_curr_sel_S4B
	LDS  R26,_last_sel_S4B
	CP   R30,R26
	BRNE _0x23A
	RJMP _0x48A
_0x23A:
	MOV  R17,R16
_0x23C:
	CP   R19,R17
	BRLO _0x23D
	LDS  R26,_last_sel_S4B
	CP   R17,R26
	BRNE _0x23E
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
_0x23E:
	LDS  R26,_curr_sel_S4B
	CP   R17,R26
	BRNE _0x23F
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
_0x23F:
	SUBI R17,-1
	RJMP _0x23C
_0x23D:
;    2658 	}
;    2659 }
_0x239:
	RJMP _0x48A
;    2660 //main configuration window of T
;    2661 LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
;    2662 LABEL flash pgt_options[] = {
;    2663 	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
;    2664 	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
;    2665 };
;    2666 
;    2667 //main menu of thermo config
;    2668 void pgtconfig_handler(uchar msg) {
_pgtconfig_handler:
;    2669 	uchar i;
;    2670 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2671 	static uchar last_sel = 1;

	.DSEG
_last_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2672 	uchar min_option = 1;
;    2673 	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
;    2674 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,2
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x246
	CPI  R26,LOW(0x43)
	BREQ _0x246
	CPI  R26,LOW(0x4F)
	BRNE _0x245
_0x246:
;    2675 	        SET_THERM_MODE;
	CALL SUBOPT_0x37
;    2676 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2677 		return;
	RJMP _0x48A
;    2678 	}
;    2679 	if(msg == MSG_INIT) {
_0x245:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x248
;    2680 		LCD_Cls();
	CALL _LCD_Cls
;    2681 		draw_label(&pgt_banner, SW_NORMAL);
	LDI  R30,LOW(_pgt_banner*2)
	LDI  R31,HIGH(_pgt_banner*2)
	CALL SUBOPT_0x31
;    2682 		INIT_OPTIONS(pgt_);
	MOV  R17,R16
_0x24A:
	CP   R19,R17
	BRLO _0x24B
	CALL SUBOPT_0xB1
	CALL SUBOPT_0x31
	SUBI R17,-1
	RJMP _0x24A
_0x24B:
;    2683 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2684 	}
;    2685 	if(msg == KEY_NUM1) {
_0x248:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x24C
;    2686 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2687 		return;
	RJMP _0x48A
;    2688 	}
;    2689 	if(msg == KEY_NUM2) {
_0x24C:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x24D
;    2690 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2691 		return;
	RJMP _0x48A
;    2692 	}
;    2693 	if(msg == MSG_REFRESH) {
_0x24D:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x24E
;    2694 		REFRESH_OPTIONS(pgt_);
	LDS  R30,_curr_sel_S4C
	LDS  R26,_last_sel_S4C
	CP   R30,R26
	BRNE _0x24F
	RJMP _0x48A
_0x24F:
	MOV  R17,R16
_0x251:
	CP   R19,R17
	BRLO _0x252
	LDS  R26,_last_sel_S4C
	CP   R17,R26
	BRNE _0x253
	CALL SUBOPT_0xB1
	CALL SUBOPT_0xB0
_0x253:
	LDS  R26,_curr_sel_S4C
	CP   R17,R26
	BRNE _0x254
	CALL SUBOPT_0xB1
	CALL SUBOPT_0xB0
_0x254:
	SUBI R17,-1
	RJMP _0x251
_0x252:
;    2695 	}
;    2696 	return;
_0x24E:
_0x48A:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2697 }
;    2698 //channel probe setup
;    2699 LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
;    2700 LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
;    2701 LABEL flash pgch_items[] = {
;    2702 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2703 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2704 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2705 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2706 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2707 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2708 };
;    2709 
;    2710 //select the channel to config
;    2711 void pgchset_handler(uchar msg) {
_pgchset_handler:
;    2712 	static uint curr_pos = 1; //absolute postion in options 1-24

	.DSEG
_curr_pos_S4D:
	.BYTE 0x2

	.CSEG
;    2713 	static uint last_pos = 1;

	.DSEG
_last_pos_S4D:
	.BYTE 0x2

	.CSEG
;    2714 	uchar min_option = 1;
;    2715 	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
;    2716 	uchar min_index = 1;
;    2717 	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
;    2718 	uchar new_page = 0;
;    2719 	int i,j;
;    2720 	if(msg == KEY_TAB || msg == KEY_CE) {
	SBIW R28,4
	CALL SUBOPT_0xB2
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
	BREQ _0x259
	CPI  R26,LOW(0x43)
	BRNE _0x258
_0x259:
;    2721 		if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x25B
;    2722 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	RJMP _0x4AF
;    2723 		else
_0x25B:
;    2724 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
_0x4AF:
	MOV  R4,R30
;    2725 		return;
	RJMP _0x489
;    2726 	}
;    2727 
;    2728 	if(msg == KEY_UP) {
_0x258:
	LDD  R26,Y+10
	CPI  R26,LOW(0x55)
	BRNE _0x25D
;    2729 		DEC_TABLE;
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB4
	SBIW R30,1
	CALL SUBOPT_0xB5
	MOV  R30,R19
	CALL SUBOPT_0xB6
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x25E
	CALL SUBOPT_0xB7
	CALL SUBOPT_0xB5
_0x25E:
	CALL SUBOPT_0xB8
	BRNE _0x25F
	RJMP _0x489
_0x25F:
	MOV  R30,R16
	CALL SUBOPT_0xB6
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x260
	LDI  R21,LOW(1)
	RJMP _0x261
_0x260:
	LDI  R21,LOW(0)
_0x261:
;    2730 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2731 	}
;    2732 	if(msg == KEY_DN) {
_0x25D:
	LDD  R26,Y+10
	CPI  R26,LOW(0x44)
	BRNE _0x262
;    2733 		INC_TABLE;
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB4
	ADIW R30,1
	CALL SUBOPT_0xB5
	MOV  R30,R18
	CALL SUBOPT_0xB9
	CALL SUBOPT_0x99
	BRSH _0x263
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xB5
_0x263:
	CALL SUBOPT_0xB8
	BRNE _0x264
	RJMP _0x489
_0x264:
	MOV  R30,R16
	CALL SUBOPT_0xB6
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x265
	LDI  R21,LOW(1)
	RJMP _0x266
_0x265:
	LDI  R21,LOW(0)
_0x266:
;    2734 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2735 	}
;    2736 	if(msg == KEY_OK) {	//show window to select the probe
_0x262:
	LDD  R26,Y+10
	CPI  R26,LOW(0x4F)
	BRNE _0x267
;    2737 		curr_ch = curr_pos;
	__GETWRMN 6,7,0,_curr_pos_S4D
;    2738 		nextwin = PG_PRBLIST;
	LDI  R30,LOW(8)
	MOV  R4,R30
;    2739 		return;
	RJMP _0x489
;    2740 	}
;    2741 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x267:
	LDD  R26,Y+10
	CPI  R26,LOW(0x30)
	BRLO _0x269
	CPI  R26,LOW(0x3A)
	BRLO _0x26A
_0x269:
_0x26A:
;    2742 	/*
;    2743 		KEY_TABLE;
;    2744 		msg = MSG_REFRESH;
;    2745 	*/
;    2746 	}
;    2747 	if(msg == KEY_DOT)
	LDD  R26,Y+10
	CPI  R26,LOW(0x2E)
	BRNE _0x26B
;    2748 	{
;    2749 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x26C
;    2750 	        {
;    2751 		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4B0
;    2752 	        }else{
_0x26C:
;    2753        		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4B0:
	LDS  R30,_curr_pos_S4D
	LDS  R31,_curr_pos_S4D+1
	CALL SUBOPT_0xBB
;    2754 	        }
;    2755 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2756 		new_page = 1; //refresh the whole page
	LDI  R21,LOW(1)
;    2757 	}
;    2758 	if(msg == MSG_INIT) {
_0x26B:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFF)
	BRNE _0x26E
;    2759 		LCD_Cls();
	CALL _LCD_Cls
;    2760 		draw_label(&pgch_banner, SW_NORMAL);
	LDI  R30,LOW(_pgch_banner*2)
	LDI  R31,HIGH(_pgch_banner*2)
	CALL SUBOPT_0x31
;    2761 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xB5
;    2762 		new_page = 1;
	LDI  R21,LOW(1)
;    2763 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2764 	}
;    2765 	if(msg == MSG_REFRESH) {
_0x26E:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x26F
;    2766 
;    2767 		REFRESH_TABLES1(pgch_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xA2
	CPI  R21,1
	BREQ PC+3
	JMP _0x270
	CALL SUBOPT_0xBE
_0x272:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0x99
	BRGE PC+3
	JMP _0x273
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xC0
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADD  R30,R26
	ADC  R31,R27
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2768 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x274
;    2769         	        {
;    2770 				if(sysdata.tid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xC1
	BRNE _0x275
;    2771 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC2
;    2772 				}else{
	RJMP _0x276
_0x275:
;    2773 			        	//draw line item j
;    2774 				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC3
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xC4
	CALL _tname2b
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xC5
;    2775 				}
_0x276:
;    2776 		        }else{
	RJMP _0x277
_0x274:
;    2777 				if(sysdata.rid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xC1
	BRNE _0x278
;    2778 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC2
;    2779 				}else{
	RJMP _0x279
_0x278:
;    2780 			        	//draw line item j
;    2781 				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xC3
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xC4
	CALL _rname2b
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xC5
;    2782 				}
_0x279:
;    2783 		        }
_0x277:
;    2784 		REFRESH_TABLES2(pgch_);
	CALL SUBOPT_0xC6
	CALL SUBOPT_0x31
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xC7
	BRNE _0x27A
	CALL SUBOPT_0xC6
	CALL SUBOPT_0xB0
_0x27A:
	CALL SUBOPT_0xC8
	RJMP _0x272
_0x273:
	RJMP _0x27B
_0x270:
	CALL SUBOPT_0xBE
_0x27D:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0x99
	BRLT _0x27E
	LDS  R26,_last_pos_S4D
	LDS  R27,_last_pos_S4D+1
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xC7
	BRNE _0x27F
	CALL SUBOPT_0xC6
	CALL SUBOPT_0xB0
_0x27F:
	CALL SUBOPT_0xBF
	CALL SUBOPT_0xC7
	BRNE _0x280
	CALL SUBOPT_0xC6
	CALL SUBOPT_0xB0
_0x280:
	CALL SUBOPT_0xC8
	RJMP _0x27D
_0x27E:
_0x27B:
;    2785 	}
;    2786 	return;
_0x26F:
_0x489:
	CALL __LOADLOCR6
	ADIW R28,11
	RET
;    2787 }
;    2788 //select probe to edit.
;    2789 LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
;    2790 LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2791 LABEL flash pgprbset_items[] = {
;    2792 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2793 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2794 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2795 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2796 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2797 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2798 };
;    2799 //select probe to config
;    2800 void pgprbset_handler(uchar msg) {
_pgprbset_handler:
;    2801 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2802 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2803 	static uint last_pos = 1;

	.DSEG
_last_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2804 	uchar min_option = 1;
;    2805 	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
;    2806 	uchar min_index = 1;
;    2807 	uchar max_index = MAX_PROBE_NUM;
;    2808 	int i,j;
;    2809 	uchar new_page = 1;
;    2810 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,3
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x284*2)
	LDI  R31,HIGH(_0x284*2)
	CALL __INITLOCB
	CALL SUBOPT_0xB2
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
	BRLO _0x286
	CPI  R26,LOW(0x3A)
	BRLO _0x287
_0x286:
_0x287:
;    2811 	/*
;    2812 		KEY_TABLE;
;    2813 		msg = MSG_REFRESH;
;    2814 	*/
;    2815 	}
;    2816 	if(msg == KEY_CE || msg == KEY_TAB)
	LDD  R26,Y+9
	CPI  R26,LOW(0x43)
	BREQ _0x289
	CPI  R26,LOW(0x54)
	BRNE _0x288
_0x289:
;    2817 	{
;    2818 		if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x28B
;    2819 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4B1
;    2820 		else
_0x28B:
;    2821 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4B1:
	MOV  R4,R30
;    2822 		return;
	RJMP _0x488
;    2823 	}
;    2824 	if(msg == KEY_OK)
_0x288:
	LDD  R26,Y+9
	CPI  R26,LOW(0x4F)
	BRNE _0x28D
;    2825 	{
;    2826 		curr_prb = curr_pos;
	__GETWRMN 8,9,0,_curr_pos_S4E
;    2827 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2828 		return;
	RJMP _0x488
;    2829 	}
;    2830 	if(msg == MSG_INIT)
_0x28D:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFF)
	BRNE _0x28E
;    2831 	{
;    2832 		LCD_Cls();
	CALL _LCD_Cls
;    2833 		draw_label(&pgprbset_banner, SW_NORMAL);
	LDI  R30,LOW(_pgprbset_banner*2)
	LDI  R31,HIGH(_pgprbset_banner*2)
	CALL SUBOPT_0x31
;    2834 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xCA
;    2835 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2836 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2837 	}
;    2838 	if(msg == KEY_UP) {
_0x28E:
	LDD  R26,Y+9
	CPI  R26,LOW(0x55)
	BRNE _0x28F
;    2839 		DEC_TABLE;
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xCC
	SBIW R30,1
	CALL SUBOPT_0xCA
	MOV  R30,R19
	CALL SUBOPT_0xCD
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x290
	CALL SUBOPT_0xB7
	CALL SUBOPT_0xCA
_0x290:
	CALL SUBOPT_0xCE
	BRNE _0x291
	RJMP _0x488
_0x291:
	MOV  R30,R16
	CALL SUBOPT_0xCD
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x292
	LDI  R30,LOW(1)
	RJMP _0x4B2
_0x292:
	LDI  R30,LOW(0)
_0x4B2:
	STD  Y+6,R30
;    2840 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2841 	}
;    2842 	if(msg == KEY_DN) {
_0x28F:
	LDD  R26,Y+9
	CPI  R26,LOW(0x44)
	BRNE _0x294
;    2843 		INC_TABLE;
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xCC
	ADIW R30,1
	CALL SUBOPT_0xCA
	MOV  R30,R18
	CALL SUBOPT_0xCF
	CALL SUBOPT_0x99
	BRSH _0x295
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xCA
_0x295:
	CALL SUBOPT_0xCE
	BRNE _0x296
	RJMP _0x488
_0x296:
	MOV  R30,R16
	CALL SUBOPT_0xCD
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x297
	LDI  R30,LOW(1)
	RJMP _0x4B3
_0x297:
	LDI  R30,LOW(0)
_0x4B3:
	STD  Y+6,R30
;    2844 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2845 	}
;    2846 	if(msg == MSG_REFRESH) {
_0x294:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x299
;    2847 		REFRESH_TABLES1(pgprbset_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xA2
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x29A
	MOV  R20,R17
	CLR  R21
_0x29C:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0x99
	BRLT _0x29D
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xC0
	ADD  R30,R20
	ADC  R31,R21
	STD  Y+7,R30
	STD  Y+7+1,R31
;    2848 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x29E
;    2849 		        {
;    2850 			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xD1
	CALL _tname2b
	RJMP _0x4B4
;    2851 		        }else{
_0x29E:
;    2852         		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xD1
	CALL _rname2b
_0x4B4:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	CALL SUBOPT_0xC5
;    2853 		        }
;    2854         		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xD2
;    2855 		REFRESH_TABLES2(pgprbset_);
	CALL SUBOPT_0xD2
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xD3
	BRNE _0x2A0
	CALL SUBOPT_0xD4
_0x2A0:
	__ADDWRN 20,21,1
	RJMP _0x29C
_0x29D:
	RJMP _0x2A1
_0x29A:
	MOV  R20,R17
	CLR  R21
_0x2A3:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0x99
	BRLT _0x2A4
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xD3
	BRNE _0x2A5
	CALL SUBOPT_0xD4
_0x2A5:
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xD3
	BRNE _0x2A6
	CALL SUBOPT_0xD4
_0x2A6:
	__ADDWRN 20,21,1
	RJMP _0x2A3
_0x2A4:
_0x2A1:
;    2856 	}
;    2857 	return;
_0x299:
_0x488:
	CALL __LOADLOCR6
	ADIW R28,10
	RET
;    2858 }
;    2859 //select probe for channel setup
;    2860 LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2861 LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2862 LABEL flash prblist_items[] = {
;    2863 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2864 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2865 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2866 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2867 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2868 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2869 };
;    2870 //select channel's probe
;    2871 void pgprblist_handler(uchar msg) {
_pgprblist_handler:
;    2872 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2873 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2874 	static uint last_pos = 1;

	.DSEG
_last_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2875 	uchar min_option = 1;
;    2876 	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
;    2877 	uchar min_index = 1;
;    2878 	uchar max_index = MAX_PROBE_NUM;
;    2879 	uchar i,j;
;    2880 	uchar new_page = 1;
;    2881 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,1
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x2A9*2)
	LDI  R31,HIGH(_0x2A9*2)
	CALL __INITLOCB
	CALL SUBOPT_0xB2
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
	BRLO _0x2AB
	CPI  R26,LOW(0x3A)
	BRLO _0x2AC
_0x2AB:
_0x2AC:
;    2882 	/*
;    2883 		KEY_TABLE;
;    2884 		msg = MSG_REFRESH;
;    2885 	*/
;    2886 	}
;    2887 	if(msg == KEY_TAB || msg == KEY_CE)
	LDD  R26,Y+7
	CPI  R26,LOW(0x54)
	BREQ _0x2AE
	CPI  R26,LOW(0x43)
	BRNE _0x2AD
_0x2AE:
;    2888 	{
;    2889 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2890 		return;
	RJMP _0x487
;    2891 	}
;    2892 	if(msg == KEY_OK)
_0x2AD:
	LDD  R26,Y+7
	CPI  R26,LOW(0x4F)
	BRNE _0x2B0
;    2893 	{
;    2894 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x2B1
;    2895 	        {
;    2896        	                if(tprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _tprbdata,288
	CALL SUBOPT_0xD5
	CALL SUBOPT_0xD6
	BREQ _0x2B2
;    2897         		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xD7
	CALL __EEPROMWRB
;    2898 	        }else{
_0x2B2:
	RJMP _0x2B3
_0x2B1:
;    2899                         if(rprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _rprbdata,288
	CALL SUBOPT_0xD5
	CALL SUBOPT_0xD6
	BREQ _0x2B4
;    2900 		                sysdata.rid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xD7
	CALL __EEPROMWRB
;    2901 	        }
_0x2B4:
_0x2B3:
;    2902 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2903 		return;
	RJMP _0x487
;    2904 	}
;    2905 	if(msg == MSG_INIT)
_0x2B0:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFF)
	BRNE _0x2B5
;    2906 	{
;    2907 		LCD_Cls();
	CALL _LCD_Cls
;    2908 		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
	CALL SUBOPT_0x0
	__POINTW1FN _0,565
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL SUBOPT_0xAF
;    2909 		draw_label(&prblist_banner, SW_NORMAL);
	LDI  R30,LOW(_prblist_banner*2)
	LDI  R31,HIGH(_prblist_banner*2)
	CALL SUBOPT_0x31
;    2910 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xD8
;    2911 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2912 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2913 	}
;    2914 	if(msg == KEY_UP) {
_0x2B5:
	LDD  R26,Y+7
	CPI  R26,LOW(0x55)
	BRNE _0x2B6
;    2915 		DEC_TABLE;
	CALL SUBOPT_0xD9
	STS  _last_pos_S4F,R30
	STS  _last_pos_S4F+1,R31
	CALL SUBOPT_0xD5
	CALL SUBOPT_0xD8
	MOV  R30,R19
	CALL SUBOPT_0xDA
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x2B7
	CALL SUBOPT_0xB7
	CALL SUBOPT_0xD8
_0x2B7:
	CALL SUBOPT_0xDB
	BRNE _0x2B8
	RJMP _0x487
_0x2B8:
	MOV  R30,R16
	CALL SUBOPT_0xDA
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x2B9
	LDI  R30,LOW(1)
	RJMP _0x4B5
_0x2B9:
	LDI  R30,LOW(0)
_0x4B5:
	STD  Y+6,R30
;    2916 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2917 	}
;    2918 	if(msg == KEY_DN) {
_0x2B6:
	LDD  R26,Y+7
	CPI  R26,LOW(0x44)
	BRNE _0x2BB
;    2919 		INC_TABLE;
	CALL SUBOPT_0xD9
	STS  _last_pos_S4F,R30
	STS  _last_pos_S4F+1,R31
	CALL SUBOPT_0xD9
	ADIW R30,1
	CALL SUBOPT_0xD8
	MOV  R30,R18
	CALL SUBOPT_0xDC
	CALL SUBOPT_0x99
	BRSH _0x2BC
	CALL SUBOPT_0xBA
	CALL SUBOPT_0xD8
_0x2BC:
	CALL SUBOPT_0xDB
	BRNE _0x2BD
	RJMP _0x487
_0x2BD:
	MOV  R30,R16
	CALL SUBOPT_0xDA
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x2BE
	LDI  R30,LOW(1)
	RJMP _0x4B6
_0x2BE:
	LDI  R30,LOW(0)
_0x4B6:
	STD  Y+6,R30
;    2920 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2921 	}
;    2922 	if(msg == KEY_DOT){
_0x2BB:
	LDD  R26,Y+7
	CPI  R26,LOW(0x2E)
	BRNE _0x2C0
;    2923 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x2C1
;    2924 	        {
;    2925 		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4B7
;    2926 	        }else{
_0x2C1:
;    2927 		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4B7:
	MOVW R30,R6
	CALL SUBOPT_0xBB
;    2928 	        }
;    2929        		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    2930        		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2931 	}
;    2932 	if(msg == MSG_REFRESH) {
_0x2C0:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2C3
;    2933 		REFRESH_TABLES1(prblist_);
	CALL SUBOPT_0x0
	CALL SUBOPT_0xBC
	CALL SUBOPT_0xD9
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xA2
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x2C4
	MOV  R21,R17
_0x2C6:
	CP   R16,R21
	BRLO _0x2C7
	CALL SUBOPT_0xDD
	CALL SUBOPT_0xC0
	MOVW R26,R30
	MOV  R30,R21
	LDI  R31,0
	ADD  R30,R26
	MOV  R20,R30
;    2934 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x2C8
;    2935 		        {
;    2936 		                sprintf(strbuf,"%s",tname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xDE
	CALL _tname2b
	RJMP _0x4B8
;    2937 		        }else{
_0x2C8:
;    2938         		        sprintf(strbuf,"%s",rname2b(j-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xDE
	CALL _rname2b
_0x4B8:
	CLR  R22
	CLR  R23
	CALL SUBOPT_0xDF
;    2939         		}
;    2940 			draw_label(&(prblist_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xE0
;    2941 		REFRESH_TABLES2(prblist_);
	CALL SUBOPT_0xE0
	CALL SUBOPT_0xDD
	CALL SUBOPT_0xE1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2CA
	CALL SUBOPT_0xE2
_0x2CA:
	SUBI R21,-1
	RJMP _0x2C6
_0x2C7:
	RJMP _0x2CB
_0x2C4:
	MOV  R21,R17
_0x2CD:
	CP   R16,R21
	BRLO _0x2CE
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xE1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2CF
	CALL SUBOPT_0xE2
_0x2CF:
	CALL SUBOPT_0xDD
	CALL SUBOPT_0xE1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2D0
	CALL SUBOPT_0xE2
_0x2D0:
	SUBI R21,-1
	RJMP _0x2CD
_0x2CE:
_0x2CB:
;    2942 	}
;    2943 	return;
_0x2C3:
_0x487:
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;    2944 }
;    2945 //select probe type list
;    2946 LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
;    2947 LABEL flash tplist_options[] = {
;    2948 	{LBL_HZ6X8,10,20,8,"1.PT100"},
;    2949 	{LBL_HZ6X8,80,20,8,"2.PT25"},
;    2950 	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
;    2951 	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
;    2952 	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
;    2953 	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
;    2954 	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
;    2955 	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
;    2956 	{LBL_HZ6X8,10,50,8,"9.R-TYPE"}
;    2957 };
;    2958 //select probe type thermo
;    2959 void pgprbtypelist_handler(uchar msg) {
_pgprbtypelist_handler:
;    2960 	static int curr_sel = 1;

	.DSEG
_curr_sel_S50:
	.BYTE 0x2

	.CSEG
;    2961 	static int last_sel = 1;

	.DSEG
_last_sel_S50:
	.BYTE 0x2

	.CSEG
;    2962 	uchar min_option = 1;
;    2963 	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
;    2964 	uchar i;
;    2965 	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	min_option -> R17
;	max_option -> R16
;	i -> R19
	LDI  R16,9
	LDI  R17,1
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRLO _0x2DD
	CPI  R26,LOW(0x3A)
	BRLO _0x2DE
_0x2DD:
	RJMP _0x2DC
_0x2DE:
;    2966 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x2DF
;    2967 	        {
;    2968         		switch(msg)
	LDD  R30,Y+4
;    2969 	        	{
;    2970 		        	case KEY_NUM1:
	CPI  R30,LOW(0x31)
	BRNE _0x2E3
;    2971         			       return;
	RJMP _0x486
;    2972 	        		case KEY_NUM2:
_0x2E3:
	CPI  R30,LOW(0x32)
	BRNE _0x2E5
;    2973 			               return;
	RJMP _0x486
;    2974         			default:
_0x2E5:
;    2975 	        		       tprbdata.type[curr_prb-1] = msg-KEY_NUM3+PRBTYPE_K;
	CALL SUBOPT_0xE3
	LDD  R30,Y+4
	SUBI R30,LOW(51)
	SUBI R30,-LOW(3)
	CALL __EEPROMWRB
;    2976 		        	       break;
;    2977         		}
;    2978         	}else{
	RJMP _0x2E6
_0x2DF:
;    2979         		switch(msg)
	LDD  R30,Y+4
;    2980 	        	{
;    2981 		        	case KEY_NUM1:
	CPI  R30,LOW(0x31)
	BRNE _0x2EA
;    2982 			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
	CALL SUBOPT_0xE4
	LDI  R30,LOW(241)
	CALL __EEPROMWRB
;    2983         			       break;
	RJMP _0x2E9
;    2984 	        		case KEY_NUM2:
_0x2EA:
	CPI  R30,LOW(0x32)
	BRNE _0x2EC
;    2985 		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
	CALL SUBOPT_0xE4
	LDI  R30,LOW(242)
	CALL __EEPROMWRB
;    2986 			               break;
	RJMP _0x2E9
;    2987         			default:
_0x2EC:
;    2988                                        return;
	RJMP _0x486
;    2989         		}
_0x2E9:
;    2990         	}
_0x2E6:
;    2991 	        msg = KEY_OK;
	LDI  R30,LOW(79)
	STD  Y+4,R30
;    2992 	}
;    2993 
;    2994 	if(msg == KEY_TAB || msg == KEY_CE || msg == KEY_OK) {
_0x2DC:
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BREQ _0x2EE
	CPI  R26,LOW(0x43)
	BREQ _0x2EE
	CPI  R26,LOW(0x4F)
	BRNE _0x2ED
_0x2EE:
;    2995 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2996 		return;
	RJMP _0x486
;    2997 	}
;    2998 	if(msg == MSG_INIT)
_0x2ED:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x2F0
;    2999 	{
;    3000 		LCD_Cls();
	CALL _LCD_Cls
;    3001 		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,666
	CALL SUBOPT_0xE5
;    3002 		draw_label(&tplist_banner, SW_NORMAL);
	LDI  R30,LOW(_tplist_banner*2)
	LDI  R31,HIGH(_tplist_banner*2)
	CALL SUBOPT_0x31
;    3003 		INIT_OPTIONS(tplist_);
	MOV  R19,R17
_0x2F2:
	CP   R16,R19
	BRLO _0x2F3
	CALL SUBOPT_0xE6
	CALL SUBOPT_0x31
	SUBI R19,-1
	RJMP _0x2F2
_0x2F3:
;    3004 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3005 	}
;    3006 	if(msg == KEY_UP) {
_0x2F0:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x2F4
;    3007 		DEC_OPTION;
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	SBIW R30,1
	CALL SUBOPT_0xE9
	MOV  R30,R17
	CALL SUBOPT_0xEA
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	BRGE _0x2F5
	CALL SUBOPT_0xEB
	CALL SUBOPT_0xE9
;    3008 		msg = MSG_REFRESH;
_0x2F5:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3009 	}
;    3010 	if(msg == KEY_DN) {
_0x2F4:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x2F6
;    3011 		INC_OPTION;
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	ADIW R30,1
	CALL SUBOPT_0xE9
	MOV  R30,R16
	CALL SUBOPT_0xEA
	CALL SUBOPT_0x99
	BRGE _0x2F7
	MOV  R30,R17
	LDI  R31,0
	CALL SUBOPT_0xE9
;    3012 		msg = MSG_REFRESH;
_0x2F7:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3013 	}
;    3014 	if(msg == MSG_REFRESH) {
_0x2F6:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x2F8
;    3015 		REFRESH_OPTIONS(tplist_);
	CALL SUBOPT_0xE7
	LDS  R26,_last_sel_S50
	LDS  R27,_last_sel_S50+1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2F9
	RJMP _0x486
_0x2F9:
	MOV  R19,R17
_0x2FB:
	CP   R16,R19
	BRLO _0x2FC
	MOV  R30,R19
	LDS  R26,_last_sel_S50
	LDS  R27,_last_sel_S50+1
	CALL SUBOPT_0x99
	BRNE _0x2FD
	CALL SUBOPT_0xE6
	CALL SUBOPT_0xB0
_0x2FD:
	MOV  R30,R19
	CALL SUBOPT_0xEA
	CALL SUBOPT_0x99
	BRNE _0x2FE
	CALL SUBOPT_0xE6
	CALL SUBOPT_0xB0
_0x2FE:
	SUBI R19,-1
	RJMP _0x2FB
_0x2FC:
;    3016 	}
;    3017 	return;
_0x2F8:
_0x486:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    3018 }
;    3019 /*
;    3020  *       Convert the probe type const to string
;    3021  */
;    3022 char PRBSTR[7];

	.DSEG
_PRBSTR:
	.BYTE 0x7
;    3023 char* getprbtype(uchar prbtype)
;    3024 {

	.CSEG
_getprbtype:
;    3025 	switch(prbtype)
;	prbtype -> Y+0
	LD   R30,Y
;    3026 	{
;    3027 		case PRBTYPE_PT25:
	CPI  R30,LOW(0xF2)
	BRNE _0x302
;    3028 		        sprintf(PRBSTR,"PT  25");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,684
	CALL SUBOPT_0x2E
;    3029 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3030 		case PRBTYPE_PT100:
_0x302:
	CPI  R30,LOW(0xF1)
	BRNE _0x303
;    3031 		        sprintf(PRBSTR,"PT 100");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,691
	CALL SUBOPT_0x2E
;    3032 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3033 		case PRBTYPE_K:
_0x303:
	CPI  R30,LOW(0x3)
	BRNE _0x304
;    3034 		        sprintf(PRBSTR,"K TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,698
	CALL SUBOPT_0x2E
;    3035 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3036 		case PRBTYPE_N:
_0x304:
	CPI  R30,LOW(0x4)
	BRNE _0x305
;    3037 		        sprintf(PRBSTR,"N TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,705
	CALL SUBOPT_0x2E
;    3038 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3039 		case PRBTYPE_E:
_0x305:
	CPI  R30,LOW(0x5)
	BRNE _0x306
;    3040 		        sprintf(PRBSTR,"E TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,712
	CALL SUBOPT_0x2E
;    3041 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3042 		case PRBTYPE_B:
_0x306:
	CPI  R30,LOW(0x6)
	BRNE _0x307
;    3043 		        sprintf(PRBSTR,"B TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,719
	CALL SUBOPT_0x2E
;    3044 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3045 		case PRBTYPE_J:
_0x307:
	CPI  R30,LOW(0x7)
	BRNE _0x308
;    3046 		        sprintf(PRBSTR,"J TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,726
	CALL SUBOPT_0x2E
;    3047 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3048 		case PRBTYPE_S:
_0x308:
	CPI  R30,LOW(0x8)
	BRNE _0x309
;    3049 		        sprintf(PRBSTR,"S TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,733
	CALL SUBOPT_0x2E
;    3050 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3051 		case PRBTYPE_R:
_0x309:
	CPI  R30,LOW(0x9)
	BRNE _0x301
;    3052 		        sprintf(PRBSTR,"R TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,740
	CALL SUBOPT_0x2E
;    3053 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3054 
;    3055 	}
_0x301:
;    3056 
;    3057 		        sprintf(PRBSTR,"? TYPE");
	CALL SUBOPT_0xEC
	__POINTW1FN _0,747
	CALL SUBOPT_0x2E
;    3058 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x485
;    3059 
;    3060 }
;    3061 //config r probe parameter
;    3062 LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
;    3063 LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
;    3064 LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
;    3065 LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
;    3066 LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
;    3067 LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
;    3068 LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
;    3069 LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
;    3070 LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
;    3071 LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
;    3072 LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
;    3073 LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
;    3074 LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};
;    3075 
;    3076 //configuration of BORE probe parameter
;    3077 void pgprbconfig_handler(uchar msg) {
_pgprbconfig_handler:
;    3078 	if( msg == KEY_OK) {
;	msg -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x4F)
	BRNE _0x311
;    3079 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    3080 		return;
	RJMP _0x485
;    3081 	}
;    3082 	if(msg == KEY_TAB || msg == KEY_CE) {
_0x311:
	LD   R26,Y
	CPI  R26,LOW(0x54)
	BREQ _0x313
	CPI  R26,LOW(0x43)
	BRNE _0x312
_0x313:
;    3083 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    3084 		return;
	RJMP _0x485
;    3085 	}
;    3086 	if(msg == MSG_INIT)
_0x312:
	LD   R26,Y
	CPI  R26,LOW(0xFF)
	BREQ PC+3
	JMP _0x315
;    3087 	{
;    3088 		LCD_Cls();
	CALL _LCD_Cls
;    3089 		if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x316
;    3090 		{
;    3091 		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,779
	CALL SUBOPT_0xE5
;    3092         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x31
;    3093 	        	//name
;    3094 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3095         		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xED
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	CALL _tname2b
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xA2
;    3096 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x31
;    3097         		//type
;    3098 	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xED
	CALL SUBOPT_0xE3
	CALL SUBOPT_0xEE
	CALL SUBOPT_0xA2
;    3099 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	RJMP _0x4B9
;    3100 		}else{
_0x316:
;    3101 		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
	CALL SUBOPT_0x0
	__POINTW1FN _0,796
	CALL SUBOPT_0xE5
;    3102         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x31
;    3103 	        	//name
;    3104 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3105         		sprintf(strbuf,":%s",rname2b(curr_prb-1));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xED
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	CALL _rname2b
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xA2
;    3106 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x31
;    3107         		//type
;    3108 	        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x0
	CALL SUBOPT_0xED
	CALL SUBOPT_0xE4
	CALL SUBOPT_0xEE
	CALL SUBOPT_0xA2
;    3109 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	CALL SUBOPT_0x31
;    3110 		        if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xE4
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x318
;    3111 		        {
;    3112 		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,813
	CALL SUBOPT_0xEF
;    3113         		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3b*2)
	LDI  R31,HIGH(_paramlbl3b*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_paramval3b*2)
	LDI  R31,HIGH(_paramval3b*2)
	RJMP _0x4B9
;    3114         		}else{
_0x318:
;    3115         		        //param1
;    3116                 		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,823
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xF0
	CALL SUBOPT_0xDF
;    3117 	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
	LDI  R30,LOW(_paramlbl1*2)
	LDI  R31,HIGH(_paramlbl1*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_paramval1*2)
	LDI  R31,HIGH(_paramval1*2)
	CALL SUBOPT_0x31
;    3118         	        	//param2
;    3119 	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,831
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xF1
	CALL SUBOPT_0xF2
	CALL SUBOPT_0xDF
;    3120                 		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	LDI  R30,LOW(_paramlbl2*2)
	LDI  R31,HIGH(_paramlbl2*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_paramval2*2)
	LDI  R31,HIGH(_paramval2*2)
	CALL SUBOPT_0x31
;    3121 	                	//param3
;    3122 		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x0
	__POINTW1FN _0,839
	CALL SUBOPT_0xEF
;    3123         		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3*2)
	LDI  R31,HIGH(_paramlbl3*2)
	CALL SUBOPT_0x31
	LDI  R30,LOW(_paramval3*2)
	LDI  R31,HIGH(_paramval3*2)
_0x4B9:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _draw_label
;    3124 
;    3125         		}
;    3126                 }
;    3127 		return;
	RJMP _0x485
;    3128 	}
;    3129 	if(msg == KEY_NUM1) {
_0x315:
	LD   R26,Y
	CPI  R26,LOW(0x31)
	BRNE _0x31A
;    3130 		window_setup(7);
	LDI  R30,LOW(7)
	STS  _max_databuf,R30
;    3131 		sprintf(strbuf,"请输入探头序列号");
	CALL SUBOPT_0x0
	__POINTW1FN _0,847
	CALL SUBOPT_0x2E
;    3132 		prbsninput();
	RCALL _prbsninput
;    3133 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3134 		return;
	RJMP _0x485
;    3135 	}
;    3136 	if(msg == KEY_NUM2){
_0x31A:
	LD   R26,Y
	CPI  R26,LOW(0x32)
	BRNE _0x31B
;    3137 		nextwin = PG_PRBTYPELIST;
	LDI  R30,LOW(9)
	MOV  R4,R30
;    3138 		return;
	RJMP _0x485
;    3139 	}
;    3140 	if(msg == KEY_NUM3 ){
_0x31B:
	LD   R26,Y
	CPI  R26,LOW(0x33)
	BRNE _0x31C
;    3141 
;    3142 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x31D
;    3143 	        {
;    3144                		window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    3145 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xE4
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x31E
;    3146 	                {
;    3147         	                sprintf(strbuf,"输入R(0!)");
	CALL SUBOPT_0x0
	__POINTW1FN _0,864
	CALL SUBOPT_0x2E
;    3148         	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0xF3
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF0
	CALL SUBOPT_0x95
	POP  R26
	POP  R27
	RJMP _0x4BA
;    3149                         }else{
_0x31E:
;    3150         	        	sprintf(strbuf,"输入系数a");
	CALL SUBOPT_0x0
	__POINTW1FN _0,874
	CALL SUBOPT_0x2E
;    3151         	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL SUBOPT_0x7F
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF0
	CALL SUBOPT_0x95
	POP  R26
	POP  R27
_0x4BA:
	CALL __EEPROMWRD
;    3152         	        }
;    3153         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3154         	}
;    3155 		return;
_0x31D:
	RJMP _0x485
;    3156 	}
;    3157 	if(msg == KEY_NUM4) {
_0x31C:
	LD   R26,Y
	CPI  R26,LOW(0x34)
	BRNE _0x320
;    3158         	if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x321
;    3159         	{
;    3160 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xE4
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x322
;    3161 	                        return;
	RJMP _0x485
;    3162 
;    3163         		window_setup(10);
_0x322:
	CALL SUBOPT_0x96
;    3164 	        	sprintf(strbuf,"输入系数b");
	__POINTW1FN _0,884
	CALL SUBOPT_0x2E
;    3165 		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0xF1
	CALL SUBOPT_0x7F
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF1
	CALL SUBOPT_0xF2
	CALL SUBOPT_0x95
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3166         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3167                 }
;    3168 		return;
_0x321:
	RJMP _0x485
;    3169 	}
;    3170 	if(msg == KEY_NUM5) {
_0x320:
	LD   R26,Y
	CPI  R26,LOW(0x35)
	BRNE _0x323
;    3171 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x2F
	BRNE _0x324
;    3172 	        {
;    3173        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xE4
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x325
;    3174        	                        return;
	RJMP _0x485
;    3175 	                window_setup(10);
_0x325:
	CALL SUBOPT_0x96
;    3176        		        sprintf(strbuf,"输入系数c");
	__POINTW1FN _0,894
	CALL SUBOPT_0x2E
;    3177         		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0xF3
	PUSH R31
	PUSH R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	CALL SUBOPT_0xF2
	CALL SUBOPT_0x95
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3178 		        nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3179 	        }
;    3180 		return;
_0x324:
;    3181 	}
;    3182 	return;
_0x323:
_0x485:
	ADIW R28,1
	RET
;    3183 }
;    3184 
;    3185 LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
;    3186 LABEL flash boot_options[] = {
;    3187  	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
;    3188  	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
;    3189  	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
;    3190  	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
;    3191 };
;    3192 //boot up menu
;    3193 void pgboottype_handler(uchar msg) {
_pgboottype_handler:
;    3194 	uchar i;
;    3195 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S53:
	.BYTE 0x1

	.CSEG
;    3196 	static uchar last_sel = 1;

	.DSEG
_last_sel_S53:
	.BYTE 0x1

	.CSEG
;    3197 	uchar min_option = 1;
;    3198 	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
;    3199 
;    3200 	if(msg == MSG_INIT) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x32D
;    3201 		LCD_Cls();
	CALL _LCD_Cls
;    3202 		draw_label(&boot_banner, SW_NORMAL);
	LDI  R30,LOW(_boot_banner*2)
	LDI  R31,HIGH(_boot_banner*2)
	CALL SUBOPT_0x31
;    3203 		INIT_OPTIONS(boot_);
	MOV  R17,R16
_0x32F:
	CP   R19,R17
	BRLO _0x330
	CALL SUBOPT_0xF4
	CALL SUBOPT_0x31
	SUBI R17,-1
	RJMP _0x32F
_0x330:
;    3204 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3205 	}
;    3206 	if(msg == KEY_NUM1) {
_0x32D:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x331
;    3207 		SET_BORE_MODE;
	CALL SUBOPT_0x34
;    3208 		SET_TORX;
	CALL SUBOPT_0x20
;    3209 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3210 		return;
	CALL __LOADLOCR4
	RJMP _0x483
;    3211 	}
;    3212 	if(msg == KEY_NUM2) {
_0x331:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x332
;    3213 		SET_BORE_MODE;
	CALL SUBOPT_0x34
;    3214 		SET_TORX;
	CALL SUBOPT_0x20
;    3215 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    3216 		return;
	CALL __LOADLOCR4
	RJMP _0x483
;    3217 	}
;    3218 	if(msg == KEY_NUM3) {
_0x332:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x333
;    3219 		SET_THERM_MODE;
	CALL SUBOPT_0x37
;    3220 		SET_TORS;
	CALL SUBOPT_0x1A
;    3221 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3222 		return;
	CALL __LOADLOCR4
	RJMP _0x483
;    3223 	}
;    3224 	if(msg == KEY_NUM4) {
_0x333:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x334
;    3225 		SET_THERM_MODE;
	CALL SUBOPT_0x37
;    3226 		SET_TORS;
	CALL SUBOPT_0x1A
;    3227 		nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	MOV  R4,R30
;    3228 		return;
	CALL __LOADLOCR4
	RJMP _0x483
;    3229 	}
;    3230 	if(msg == MSG_REFRESH) {
_0x334:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x335
;    3231 		REFRESH_OPTIONS(boot_);
	LDS  R30,_curr_sel_S53
	LDS  R26,_last_sel_S53
	CP   R30,R26
	BRNE _0x336
	CALL __LOADLOCR4
	RJMP _0x483
_0x336:
	MOV  R17,R16
_0x338:
	CP   R19,R17
	BRLO _0x339
	LDS  R26,_last_sel_S53
	CP   R17,R26
	BRNE _0x33A
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xB0
_0x33A:
	LDS  R26,_curr_sel_S53
	CP   R17,R26
	BRNE _0x33B
	CALL SUBOPT_0xF4
	CALL SUBOPT_0xB0
_0x33B:
	SUBI R17,-1
	RJMP _0x338
_0x339:
;    3232 	}
;    3233 }
_0x335:
	CALL __LOADLOCR4
	RJMP _0x483
;    3234 #include "utili.h"
;    3235 #include "window.h"
;    3236 #include "stdlib.h"
;    3237 /*---------------延时子程序----------------*/
;    3238 /*
;    3239 void delay1 (uint ms)
;    3240 {
;    3241   	int i,j;
;    3242   	for(i=0;i<ms;i++)
;    3243   	for(j=0;j<1000;j++)
;    3244   		;
;    3245 
;    3246 }
;    3247 */
;    3248 /*
;    3249 char lowc(uchar x)
;    3250 {
;    3251 	x = x&0x0f;
;    3252 	if(x > 0x09)
;    3253 		return 'A'+x-0x0a;
;    3254 	else
;    3255 		return '0'+x;
;    3256 }
;    3257 char highc(uchar x)
;    3258 {
;    3259 
;    3260 	x = x>>4;
;    3261 	if(x > 0x09)
;    3262 		return 'A'+x-0x0a;
;    3263 	else
;    3264 		return '0'+x;
;    3265 }
;    3266 */
;    3267 /*计算字符串长度*/
;    3268 
;    3269 extern uchar pos_databuf;
;    3270 extern uchar data_sign;
;    3271 extern u8 databuf[12];
;    3272 double buf2double()
;    3273 {
_buf2double:
;    3274         return atof(databuf);
	CALL SUBOPT_0xF5
	CALL _atof
	RET
;    3275         /*
;    3276 	double tmp = 0.0;
;    3277 	uchar i = 0;
;    3278 	uchar pos = 0;
;    3279 	for(i=1;i<pos_databuf;i++) {
;    3280 		if(databuf[i] != KEY_DOT)
;    3281 			tmp = tmp * 10.0+(databuf[i] - '0');
;    3282 		else
;    3283 			pos = pos_databuf - i - 2;
;    3284 	}
;    3285 	while(pos > 0) {
;    3286 		tmp = tmp / 10.0;
;    3287 		pos--;
;    3288 	}
;    3289 	if(data_sign == 1)
;    3290 	        tmp = -tmp;
;    3291        	return tmp;
;    3292        	*/
;    3293 }
;    3294 int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
;    3295 {
_buf2byte:
;    3296         return atoi(databuf);
	CALL SUBOPT_0xF5
	CALL _atoi
	RET
;    3297         /*
;    3298 	int tmp = 0;
;    3299 	uchar i;
;    3300 	for(i=0;i<pos_databuf;i++) {
;    3301 		tmp = tmp * 10+(databuf[i] - '0');
;    3302 	}
;    3303 	return tmp;
;    3304 	*/
;    3305 }
;    3306 
;    3307 
;    3308 #include "window.h"
;    3309 #include "utili.h"
;    3310 #include "lcd.h"
;    3311 
;    3312 MSG_HANDLER curr_window = 0;

	.DSEG
_curr_window:
	.BYTE 0x2
;    3313 
;    3314 extern uchar key;
;    3315 
;    3316 u8 databuf[12];
_databuf:
	.BYTE 0xC
;    3317 u8 pos_databuf; //position in data buffer
_pos_databuf:
	.BYTE 0x1
;    3318 u8 max_databuf;
_max_databuf:
	.BYTE 0x1
;    3319 u8 data_sign;   // sign of the data
_data_sign:
	.BYTE 0x1
;    3320 LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};

	.CSEG
;    3321 LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
;    3322 LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
;    3323 LABEL flash databox = {LBL_HZ16,20,30,9,databuf};
;    3324 
;    3325 
;    3326 void prbsninput()
;    3327 {
_prbsninput:
;    3328 	uchar msg,len;
;    3329 	databuf[0] = '\0';
	ST   -Y,R17
	ST   -Y,R16
;	msg -> R17
;	len -> R16
	CALL SUBOPT_0xF6
;    3330 	pos_databuf = 0;
;    3331 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3332 	while(1)
_0x33F:
;    3333 	{
;    3334 		if(key == KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x33F
;    3335 		        continue;
;    3336 
;    3337 		msg = key;
	MOV  R17,R5
;    3338 
;    3339 
;    3340 		if(msg == MSG_INIT) {
	CPI  R17,255
	BRNE _0x343
;    3341 			LCD_Cls();
	CALL SUBOPT_0xF7
;    3342 			draw_label(&datalbl,SW_NORMAL);
;    3343 			draw_label(&datalbl3,SW_NORMAL);
	LDI  R30,LOW(_datalbl3*2)
	LDI  R31,HIGH(_datalbl3*2)
	CALL SUBOPT_0x31
;    3344 			sprintf(databuf,"");
	CALL SUBOPT_0xF5
	__POINTW1FN _0,11
	CALL SUBOPT_0x2E
;    3345 			draw_inputbox(&databox);
	CALL SUBOPT_0xF8
;    3346 //			LCD_ShowCursor(databox.x,databox.y);
;    3347 		}
;    3348 		if(msg == KEY_TAB)
_0x343:
	CPI  R17,84
	BRNE _0x344
;    3349 		{
;    3350 		/*
;    3351         		if(pos_databuf < max_databuf)
;    3352         		{
;    3353 			        databuf[pos_databuf++] = '0';
;    3354 			        databuf[pos_databuf] = '\0';
;    3355         			msg = MSG_REFRESH;
;    3356                         }
;    3357                 */
;    3358                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3359                         return;
	RJMP _0x484
;    3360 		}
;    3361 		if(msg == KEY_CE) {
_0x344:
	CPI  R17,67
	BRNE _0x345
;    3362 		        key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3363 			return;
	RJMP _0x484
;    3364 		}
;    3365 
;    3366 		if(msg == KEY_DN) {
_0x345:
	CPI  R17,68
	BRNE _0x346
;    3367 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x347
;    3368 			{
;    3369 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFA
;    3370 				databuf[pos_databuf] = '\0';
;    3371 			}
;    3372 
;    3373 			if(databuf[pos_databuf-1] == '0')
_0x347:
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRNE _0x348
;    3374 				databuf[pos_databuf-1] = 'Z';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(90)
	STD  Z+0,R26
;    3375 			else if (databuf[pos_databuf-1] == 'A')
	RJMP _0x349
_0x348:
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRNE _0x34A
;    3376 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(57)
	STD  Z+0,R26
;    3377 			else
	RJMP _0x34B
_0x34A:
;    3378 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,LOW(1)
	ST   X,R30
;    3379 			msg = MSG_REFRESH;
_0x34B:
_0x349:
	LDI  R17,LOW(254)
;    3380 		}
;    3381 		if(msg == KEY_UP) {
_0x346:
	CPI  R17,85
	BRNE _0x34C
;    3382 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x34D
;    3383 			{
;    3384 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFA
;    3385 				databuf[pos_databuf] = '\0';
;    3386 			}
;    3387 
;    3388 			if(databuf[pos_databuf-1] == '9')
_0x34D:
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x39)
	BRNE _0x34E
;    3389 				databuf[pos_databuf-1] = 'A';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    3390 			else if (databuf[pos_databuf-1] == 'Z')
	RJMP _0x34F
_0x34E:
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x5A)
	BRNE _0x350
;    3391 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
;    3392 			else
	RJMP _0x351
_0x350:
;    3393 				databuf[pos_databuf-1] +=1;
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
;    3394 			msg = MSG_REFRESH;
_0x351:
_0x34F:
	LDI  R17,LOW(254)
;    3395 		}
;    3396 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x34C:
	CPI  R17,48
	BRLO _0x353
	CPI  R17,58
	BRLO _0x354
_0x353:
	RJMP _0x352
_0x354:
;    3397         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xFC
	BRSH _0x355
;    3398         		{
;    3399 			        databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFD
;    3400         			databuf[pos_databuf] = '\0';
;    3401         		}
;    3402 			msg = MSG_REFRESH;
_0x355:
	LDI  R17,LOW(254)
;    3403 		}
;    3404 		if(msg == KEY_OK) {
_0x352:
	CPI  R17,79
	BRNE _0x356
;    3405 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x39
	BRNE _0x357
;    3406 		        {
;    3407 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x359:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x35A
;    3408         		                tprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _tprbdata,288
	CALL SUBOPT_0xFE
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3409         			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x359
_0x35A:
	__POINTW2MN _tprbdata,288
	RJMP _0x4BB
;    3410         		}else{
_0x357:
;    3411 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x35D:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x35E
;    3412         		                rprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _rprbdata,288
	CALL SUBOPT_0xFE
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3413         			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x35D
_0x35E:
	__POINTW2MN _rprbdata,288
_0x4BB:
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
;    3414         		}
;    3415 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3416 			break;
	RJMP _0x341
;    3417 		}
;    3418 		if(msg == MSG_REFRESH) {
_0x356:
	CPI  R17,254
	BRNE _0x35F
;    3419 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xFF
;    3420 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3421 		}
;    3422 		key = KEY_INVALID;
_0x35F:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3423 	}
	RJMP _0x33F
_0x341:
;    3424 //	LCD_HideCursor();
;    3425 }
_0x484:
	LD   R16,Y+
	LD   R17,Y+
	RET
;    3426 
;    3427 uchar wnd_intinput(uchar lastval)
;    3428 {
_wnd_intinput:
;    3429 	uchar msg;
;    3430 
;    3431 	key = MSG_INIT;
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3432 	databuf[0] = '\0';
	CALL SUBOPT_0xF6
;    3433 	pos_databuf = 0;
;    3434         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3435 	while(1)
_0x360:
;    3436 	{
;    3437 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x363
;    3438 		{
;    3439 			msg = key;
	MOV  R17,R5
;    3440 
;    3441 		}else{
	RJMP _0x364
_0x363:
;    3442 			continue;
	RJMP _0x360
;    3443 		}
_0x364:
;    3444 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x365
;    3445 		{
;    3446 			LCD_Cls();
	CALL SUBOPT_0xF7
;    3447 			draw_label(&datalbl,SW_NORMAL);
;    3448 			draw_inputbox(&databox);
	CALL SUBOPT_0xF8
;    3449 //			LCD_ShowCursor(databox.x,databox.y);
;    3450 
;    3451 		}
;    3452 
;    3453 		if(msg == KEY_TAB)
_0x365:
	CPI  R17,84
	BRNE _0x366
;    3454 		{
;    3455 		/*
;    3456         		if(pos_databuf < max_databuf)
;    3457         		{
;    3458 			        databuf[pos_databuf++] = '0';
;    3459 			        databuf[pos_databuf] = '\0';
;    3460         			msg = MSG_REFRESH;
;    3461                         }
;    3462                 */
;    3463 		}
;    3464 
;    3465 		if(msg == KEY_DN) {
_0x366:
	CPI  R17,68
	BRNE _0x367
;    3466 		/*
;    3467 			if(pos_databuf == 0)
;    3468 			{
;    3469 				databuf[pos_databuf++] = '0';
;    3470 				databuf[pos_databuf] = '\0';
;    3471 			}
;    3472 
;    3473 			if(databuf[pos_databuf-1] == '0')
;    3474 				databuf[pos_databuf-1] = '9';
;    3475 			else
;    3476 				databuf[pos_databuf-1] -= 1;
;    3477 			msg = MSG_REFRESH;
;    3478                 */
;    3479 		}
;    3480 		if(msg == KEY_UP) {
_0x367:
	CPI  R17,85
	BRNE _0x368
;    3481 		/*
;    3482 			if(pos_databuf == 0)
;    3483 			{
;    3484 				databuf[pos_databuf++] = '0';
;    3485 				databuf[pos_databuf] = '\0';
;    3486 			}
;    3487 
;    3488 			if(databuf[pos_databuf-1] == '9')
;    3489 				databuf[pos_databuf-1] = '0';
;    3490 			else
;    3491 				databuf[pos_databuf-1] += 1;
;    3492 			msg = MSG_REFRESH;
;    3493                 */
;    3494 		}
;    3495 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x368:
	CPI  R17,48
	BRLO _0x36A
	CPI  R17,58
	BRLO _0x36B
_0x36A:
	RJMP _0x369
_0x36B:
;    3496         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xFC
	BRSH _0x36C
;    3497         		{
;    3498         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFD
;    3499 	        		databuf[pos_databuf] = '\0';
;    3500 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3501 		        }
;    3502 		}
_0x36C:
;    3503 		if(msg == KEY_CE) {
_0x369:
	CPI  R17,67
	BRNE _0x36D
;    3504 //			LCD_HideCursor();
;    3505                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3506 			return lastval;
	LDD  R30,Y+1
	LDD  R17,Y+0
	RJMP _0x482
;    3507 		}
;    3508 		if(msg == KEY_OK){
_0x36D:
	CPI  R17,79
	BRNE _0x36E
;    3509 //			LCD_HideCursor();
;    3510                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3511 			return buf2byte();
	CALL _buf2byte
	LDD  R17,Y+0
	RJMP _0x482
;    3512 		}
;    3513 		if(msg == MSG_REFRESH) {
_0x36E:
	CPI  R17,254
	BRNE _0x36F
;    3514 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xFF
;    3515 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3516 		}
;    3517 		key = KEY_INVALID;
_0x36F:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3518 	}
	RJMP _0x360
;    3519 }
;    3520 double wnd_floatinput(double lastval)
;    3521 {
_wnd_floatinput:
;    3522 	uchar msg;
;    3523 
;    3524 	databuf[0] = '+';
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3525         databuf[1] = '\0';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3526         pos_databuf = 1;
	LDI  R30,LOW(1)
	STS  _pos_databuf,R30
;    3527         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3528 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3529 	while(1)
_0x370:
;    3530 	{
;    3531 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x373
;    3532 		{
;    3533 			msg = key;
	MOV  R17,R5
;    3534 
;    3535 		}else{
	RJMP _0x374
_0x373:
;    3536 			continue;
	RJMP _0x370
;    3537 		}
_0x374:
;    3538 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x375
;    3539 		{
;    3540 			LCD_Cls();
	CALL SUBOPT_0xF7
;    3541 			draw_label(&datalbl,SW_NORMAL);
;    3542 			draw_label(&datalbl2,SW_NORMAL);
	LDI  R30,LOW(_datalbl2*2)
	LDI  R31,HIGH(_datalbl2*2)
	CALL SUBOPT_0x31
;    3543 			draw_inputbox(&databox);
	CALL SUBOPT_0xF8
;    3544 //			LCD_ShowCursor(databox.x,databox.y);
;    3545 		}
;    3546 
;    3547 		if(msg == KEY_TAB)
_0x375:
	CPI  R17,84
	BRNE _0x376
;    3548 		{
;    3549 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3550 		}
;    3551 		if(msg == KEY_UP) {
_0x376:
	CPI  R17,85
	BREQ PC+3
	JMP _0x377
;    3552 		        if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x378
;    3553 		        {
;    3554 		                if((data_sign & 0x01) == 0)      //no sign
	LDS  R30,_data_sign
	ANDI R30,LOW(0x1)
	BRNE _0x379
;    3555 		                {
;    3556 		                        databuf[0] = '-';
	LDI  R30,LOW(45)
	STS  _databuf,R30
;    3557 		                        data_sign |= 0x01;
	LDS  R30,_data_sign
	ORI  R30,1
	RJMP _0x4BC
;    3558 		                }else{
_0x379:
;    3559 		                        databuf[0] = '+';       //
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3560 		                        data_sign ^= 0x01;
	LDS  R26,_data_sign
	LDI  R30,LOW(1)
	EOR  R30,R26
_0x4BC:
	STS  _data_sign,R30
;    3561 		                }
;    3562 		        }else{
	RJMP _0x37B
_0x378:
;    3563                        		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
	CALL SUBOPT_0xFC
	BRSH _0x37D
	LDS  R30,_pos_databuf
	SUBI R30,LOW(2)
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x45)
	BREQ _0x37E
_0x37D:
	RJMP _0x37C
_0x37E:
;    3564                 		{
;    3565 			                if((data_sign & 0x08) == 0)
	LDS  R30,_data_sign
	ANDI R30,LOW(0x8)
	BRNE _0x37F
;    3566 			                {
;    3567                		        	        databuf[pos_databuf-1] = '-';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(45)
	STD  Z+0,R26
;    3568                		        	        data_sign |= 0x08;
	LDS  R30,_data_sign
	ORI  R30,8
	RJMP _0x4BD
;    3569                		                }else{
_0x37F:
;    3570                		        	        databuf[pos_databuf-1] = '+';
	CALL SUBOPT_0xFB
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3571                		        	        data_sign ^= 0x08;
	LDS  R26,_data_sign
	LDI  R30,LOW(8)
	EOR  R30,R26
_0x4BD:
	STS  _data_sign,R30
;    3572                		                }
;    3573 	        		}
;    3574 		        	msg = MSG_REFRESH;
_0x37C:
	LDI  R17,LOW(254)
;    3575 		        }
_0x37B:
;    3576 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3577 		}
;    3578 		if(msg == KEY_DN) {
_0x377:
	CPI  R17,68
	BRNE _0x381
;    3579                		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
	CALL SUBOPT_0xFC
	BRSH _0x383
	LDS  R30,_data_sign
	ANDI R30,LOW(0x4)
	BREQ _0x384
_0x383:
	RJMP _0x382
_0x384:
;    3580         		{
;    3581         			databuf[pos_databuf++] = 'E';
	CALL SUBOPT_0xF9
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(69)
	STD  Z+0,R26
;    3582         			databuf[pos_databuf++] = '+';
	CALL SUBOPT_0xF9
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3583 	        		databuf[pos_databuf] = '\0';
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3584 	        		data_sign |= 0x04;
	LDS  R30,_data_sign
	ORI  R30,4
	STS  _data_sign,R30
;    3585 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3586 		        }
;    3587 
;    3588 			msg = MSG_REFRESH;
_0x382:
	LDI  R17,LOW(254)
;    3589 		}
;    3590 		if( msg == KEY_DOT)
_0x381:
	CPI  R17,46
	BRNE _0x385
;    3591 		{
;    3592                		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
	CALL SUBOPT_0xFC
	BRSH _0x387
	LDS  R30,_data_sign
	ANDI R30,LOW(0x2)
	BREQ _0x388
_0x387:
	RJMP _0x386
_0x388:
;    3593         		{
;    3594         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFD
;    3595 	        		databuf[pos_databuf] = '\0';
;    3596 	        		data_sign |= 0x02;
	LDS  R30,_data_sign
	ORI  R30,2
	STS  _data_sign,R30
;    3597 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3598 		        }
;    3599 
;    3600 		}
_0x386:
;    3601 		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
_0x385:
	CPI  R17,48
	BRLO _0x38A
	CPI  R17,58
	BRLO _0x38B
_0x38A:
	RJMP _0x389
_0x38B:
;    3602                		if(pos_databuf < max_databuf)
	CALL SUBOPT_0xFC
	BRSH _0x38C
;    3603         		{
;    3604         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0xF9
	CALL SUBOPT_0xFD
;    3605 	        		databuf[pos_databuf] = '\0';
;    3606 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3607 		        }
;    3608 		}
_0x38C:
;    3609 		if(msg == KEY_CE) {
_0x389:
	CPI  R17,67
	BRNE _0x38D
;    3610 //			LCD_HideCursor();
;    3611 		key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3612 			return lastval;
	__GETD1S 1
	LDD  R17,Y+0
	RJMP _0x483
;    3613 		}
;    3614 		if(msg == KEY_OK){
_0x38D:
	CPI  R17,79
	BRNE _0x38E
;    3615 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3616 //			LCD_HideCursor();
;    3617 			return buf2double();
	CALL _buf2double
	LDD  R17,Y+0
	RJMP _0x483
;    3618 		}
;    3619 		if(msg == MSG_REFRESH) {
_0x38E:
	CPI  R17,254
	BRNE _0x38F
;    3620 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0xFF
;    3621 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3622 		}
;    3623 				key = KEY_INVALID;
_0x38F:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3624 	}
	RJMP _0x370
;    3625 }
_0x483:
	ADIW R28,5
	RET
;    3626 //display a message box
;    3627 void wnd_msgbox(flash LABEL *lbl)
;    3628 {
_wnd_msgbox:
;    3629 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x100
	CALL _LCD_ClsBlock
;    3630 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x31
;    3631 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	CALL SUBOPT_0x100
	CALL _LCD_Rectange
;    3632 	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
	CALL SUBOPT_0x101
	SBIW R30,8
	CALL SUBOPT_0x102
	SBIW R30,1
	CALL SUBOPT_0x103
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(16)
	LDI  R31,HIGH(16)
	CALL __SWAPW12
	SUB  R30,R26
	SBC  R31,R27
	CALL SUBOPT_0x104
	ADIW R30,17
	ST   -Y,R30
	CALL _LCD_Rectange
;    3633 
;    3634 }
	RJMP _0x482
;    3635 
;    3636 void draw_inputbox(flash LABEL *lbl)
;    3637 {
_draw_inputbox:
;    3638 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x101
	SBIW R30,10
	CALL SUBOPT_0x102
	SBIW R30,3
	CALL SUBOPT_0x103
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x104
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3639 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	CALL SUBOPT_0x101
	SBIW R30,10
	CALL SUBOPT_0x102
	SBIW R30,3
	CALL SUBOPT_0x103
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x104
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_Rectange
;    3640 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x31
;    3641 }
_0x482:
	ADIW R28,2
	RET
;    3642 
;    3643 void draw_label(flash LABEL *lbl,uchar reverse) {
_draw_label:
;    3644 	uchar len = lbl->width;
;    3645 	if(lbl->type == LBL_HZ6X8) {
	ST   -Y,R17
;	*lbl -> Y+2
;	reverse -> Y+1
;	len -> R17
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	MOV  R17,R30
	CALL SUBOPT_0x105
	SBIW R30,1
	BRNE _0x390
;    3646 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x391
;    3647 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x392
;    3648 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
	CALL SUBOPT_0x106
	CALL SUBOPT_0x107
	ADIW R30,8
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3649 			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
_0x392:
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL _LCD_Print6X8
;    3650 			return;
	LDD  R17,Y+0
	RJMP _0x481
;    3651 		}
;    3652 		if(reverse & SW_REVERSE)
_0x391:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x393
;    3653 			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
	CALL SUBOPT_0x109
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3654 		return;
_0x393:
	LDD  R17,Y+0
	RJMP _0x481
;    3655 	}
;    3656 	if(lbl->type == LBL_HZ8X16) {
_0x390:
	CALL SUBOPT_0x105
	SBIW R30,2
	BRNE _0x394
;    3657 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x395
;    3658 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x396
;    3659 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
	CALL SUBOPT_0x106
	CALL SUBOPT_0x107
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3660 			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
_0x396:
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL _LCD_Print8X16
;    3661 			return;
	LDD  R17,Y+0
	RJMP _0x481
;    3662 		}
;    3663 		if(reverse & SW_REVERSE)
_0x395:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x397
;    3664 			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
	CALL SUBOPT_0x109
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3665 		return;
_0x397:
	LDD  R17,Y+0
	RJMP _0x481
;    3666 	}
;    3667 	/*
;    3668 	if(lbl->type == LBL_HZ24X32) {
;    3669 		if(reverse & SW_NORMAL) {
;    3670 			if((reverse & SW_OVERLAP) == 0)
;    3671 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
;    3672 			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
;    3673 		}
;    3674 		if(reverse & SW_REVERSE)
;    3675 			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
;    3676 		return;
;    3677 	}
;    3678 	*/
;    3679 	if(lbl->type == LBL_HZ12) {
_0x394:
	CALL SUBOPT_0x105
	SBIW R30,4
	BRNE _0x398
;    3680 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x399
;    3681 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x39A
;    3682 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
	CALL SUBOPT_0x106
	CALL SUBOPT_0x10A
	ADIW R30,12
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3683 			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
_0x39A:
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL _LCD_PrintHz12
;    3684 		}
;    3685 		if(reverse & SW_REVERSE)
_0x399:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x39B
;    3686 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
	CALL SUBOPT_0x10B
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3687 		return;
_0x39B:
	LDD  R17,Y+0
	RJMP _0x481
;    3688 	}
;    3689 	if(lbl->type == LBL_HZ16) {
_0x398:
	CALL SUBOPT_0x105
	SBIW R30,5
	BRNE _0x39C
;    3690 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x39D
;    3691 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x39E
;    3692 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
	CALL SUBOPT_0x106
	CALL SUBOPT_0x10A
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3693 			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
_0x39E:
	CALL SUBOPT_0x106
	CALL SUBOPT_0x108
	CALL _LCD_PrintHz16
;    3694 		}
;    3695 		if(reverse & SW_REVERSE)
_0x39D:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x39F
;    3696 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
	CALL SUBOPT_0x10B
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3697 		return;
_0x39F:
	LDD  R17,Y+0
	RJMP _0x481
;    3698 	}
;    3699 }
_0x39C:
	LDD  R17,Y+0
_0x481:
	ADIW R28,4
	RET
;    3700 
;    3701 

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
	BREQ _0x3A0
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x3A2
	__CPWRN 16,17,2
	BRLO _0x3A3
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x3A2:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL SUBOPT_0x10C
	LDD  R26,Y+6
	STD  Z+0,R26
_0x3A3:
	RJMP _0x3A4
_0x3A0:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x3A4:
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
	BRLO _0x3A5
	LDI  R30,LOW(6)
	STD  Y+8,R30
_0x3A5:
	LDD  R30,Y+8
	LDI  R26,LOW(__fround_GD*2)
	LDI  R27,HIGH(__fround_GD*2)
	LDI  R31,0
	CALL SUBOPT_0x7F
	CALL __GETD1PF
	CALL SUBOPT_0x10D
	CALL __ADDF12
	CALL SUBOPT_0x10E
	LDI  R17,LOW(0)
	CALL SUBOPT_0x26
	CALL SUBOPT_0x6F
_0x3A6:
	CALL SUBOPT_0x53
	CALL SUBOPT_0x10D
	CALL __CMPF12
	BRLO _0x3A8
	CALL SUBOPT_0x10F
	CALL __MULF12
	CALL SUBOPT_0x6F
	SUBI R17,-LOW(1)
	RJMP _0x3A6
_0x3A8:
	CPI  R17,0
	BRNE _0x3A9
	CALL SUBOPT_0x110
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x3AA
_0x3A9:
_0x3AB:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3AD
	CALL SUBOPT_0x10F
	CALL SUBOPT_0x111
	CALL SUBOPT_0xAD
	CALL SUBOPT_0x10D
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x110
	CALL SUBOPT_0x112
	CALL SUBOPT_0x4F
	CALL SUBOPT_0x113
	CALL __MULF12
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x10E
	RJMP _0x3AB
_0x3AD:
_0x3AA:
	LDD  R30,Y+8
	CPI  R30,0
	BRNE _0x3AE
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x480
_0x3AE:
	CALL SUBOPT_0x110
	LDI  R30,LOW(46)
	ST   X,R30
_0x3AF:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x3B1
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x114
	CALL SUBOPT_0x10E
	__GETD1S 9
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x110
	CALL SUBOPT_0x112
	CALL SUBOPT_0x10D
	CALL SUBOPT_0x113
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x10E
	RJMP _0x3AF
_0x3B1:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x480:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
__ftoe_GD:
	SBIW R28,4
	CALL __SAVELOCR4
	CALL SUBOPT_0x26
	CALL SUBOPT_0x7E
	LDD  R26,Y+11
	CPI  R26,LOW(0x7)
	BRLO _0x3B2
	LDI  R30,LOW(6)
	STD  Y+11,R30
_0x3B2:
	LDD  R17,Y+11
_0x3B3:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3B5
	CALL SUBOPT_0x115
	CALL SUBOPT_0x7E
	RJMP _0x3B3
_0x3B5:
	CALL SUBOPT_0x116
	CALL __CPD10
	BRNE _0x3B6
	LDI  R19,LOW(0)
	CALL SUBOPT_0x115
	CALL SUBOPT_0x7E
	RJMP _0x3B7
_0x3B6:
	LDD  R19,Y+11
	CALL SUBOPT_0x117
	BREQ PC+2
	BRCC PC+3
	JMP  _0x3B8
	CALL SUBOPT_0x115
	CALL SUBOPT_0x7E
_0x3B9:
	CALL SUBOPT_0x117
	BRLO _0x3BB
	CALL SUBOPT_0x118
	CALL SUBOPT_0x119
	RJMP _0x3B9
_0x3BB:
	RJMP _0x3BC
_0x3B8:
_0x3BD:
	CALL SUBOPT_0x117
	BRSH _0x3BF
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x11B
	SUBI R19,LOW(1)
	RJMP _0x3BD
_0x3BF:
	CALL SUBOPT_0x115
	CALL SUBOPT_0x7E
_0x3BC:
	CALL SUBOPT_0x116
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x117
	BRLO _0x3C0
	CALL SUBOPT_0x118
	CALL SUBOPT_0x119
_0x3C0:
_0x3B7:
	LDI  R17,LOW(0)
_0x3C1:
	LDD  R30,Y+11
	CP   R30,R17
	BRLO _0x3C3
	CALL SUBOPT_0x11C
	__GETD1N 0x41200000
	CALL SUBOPT_0x111
	CALL SUBOPT_0x7E
	CALL SUBOPT_0x1
	CALL SUBOPT_0x118
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x112
	CALL SUBOPT_0x113
	CALL SUBOPT_0x11C
	CALL __MULF12
	CALL SUBOPT_0x118
	CALL SUBOPT_0x2B
	CALL SUBOPT_0x11B
	MOV  R30,R17
	SUBI R17,-1
	CPI  R30,0
	BRNE _0x3C1
	CALL SUBOPT_0x11D
	LDI  R30,LOW(46)
	ST   X,R30
	RJMP _0x3C1
_0x3C3:
	CALL SUBOPT_0xC8
	SBIW R30,1
	LDD  R26,Y+10
	STD  Z+0,R26
	CPI  R19,0
	BRGE _0x3C5
	CALL SUBOPT_0x11D
	LDI  R30,LOW(45)
	ST   X,R30
	NEG  R19
_0x3C5:
	CPI  R19,10
	BRLT _0x3C6
	CALL SUBOPT_0xC8
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __DIVB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
_0x3C6:
	CALL SUBOPT_0xC8
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
_0x3C7:
	MOVW R26,R28
	SUBI R26,LOW(-(90))
	SBCI R27,HIGH(-(90))
	CALL SUBOPT_0x10C
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x3C9
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x3CD
	CPI  R18,37
	BRNE _0x3CE
	LDI  R17,LOW(1)
	RJMP _0x3CF
_0x3CE:
	CALL SUBOPT_0x11E
_0x3CF:
	RJMP _0x3CC
_0x3CD:
	CPI  R30,LOW(0x1)
	BRNE _0x3D0
	CPI  R18,37
	BRNE _0x3D1
	CALL SUBOPT_0x11E
	RJMP _0x4BE
_0x3D1:
	LDI  R17,LOW(2)
	LDI  R30,LOW(0)
	STD  Y+19,R30
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x3D2
	LDI  R16,LOW(1)
	RJMP _0x3CC
_0x3D2:
	CPI  R18,43
	BRNE _0x3D3
	LDI  R30,LOW(43)
	STD  Y+19,R30
	RJMP _0x3CC
_0x3D3:
	CPI  R18,32
	BRNE _0x3D4
	LDI  R30,LOW(32)
	STD  Y+19,R30
	RJMP _0x3CC
_0x3D4:
	RJMP _0x3D5
_0x3D0:
	CPI  R30,LOW(0x2)
	BRNE _0x3D6
_0x3D5:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x3D7
	ORI  R16,LOW(128)
	RJMP _0x3CC
_0x3D7:
	RJMP _0x3D8
_0x3D6:
	CPI  R30,LOW(0x3)
	BRNE _0x3D9
_0x3D8:
	CPI  R18,48
	BRLO _0x3DB
	CPI  R18,58
	BRLO _0x3DC
_0x3DB:
	RJMP _0x3DA
_0x3DC:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x3CC
_0x3DA:
	LDI  R20,LOW(0)
	CPI  R18,46
	BRNE _0x3DD
	LDI  R17,LOW(4)
	RJMP _0x3CC
_0x3DD:
	RJMP _0x3DE
_0x3D9:
	CPI  R30,LOW(0x4)
	BRNE _0x3E0
	CPI  R18,48
	BRLO _0x3E2
	CPI  R18,58
	BRLO _0x3E3
_0x3E2:
	RJMP _0x3E1
_0x3E3:
	ORI  R16,LOW(32)
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R20,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R20,R30
	RJMP _0x3CC
_0x3E1:
_0x3DE:
	CPI  R18,108
	BRNE _0x3E4
	ORI  R16,LOW(2)
	LDI  R17,LOW(5)
	RJMP _0x3CC
_0x3E4:
	RJMP _0x3E5
_0x3E0:
	CPI  R30,LOW(0x5)
	BREQ PC+3
	JMP _0x3CC
_0x3E5:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x3EA
	CALL SUBOPT_0x11F
	LDD  R30,Z+4
	CALL SUBOPT_0x120
	RJMP _0x3EB
_0x3EA:
	CPI  R30,LOW(0x45)
	BREQ _0x3EE
	CPI  R30,LOW(0x65)
	BRNE _0x3EF
_0x3EE:
	RJMP _0x3F0
_0x3EF:
	CPI  R30,LOW(0x66)
	BREQ PC+3
	JMP _0x3F1
_0x3F0:
	MOVW R30,R28
	ADIW R30,20
	STD  Y+10,R30
	STD  Y+10+1,R31
	CALL SUBOPT_0x11F
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	CALL SUBOPT_0x5E
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD20
	BRLT _0x3F2
	LDD  R26,Y+19
	CPI  R26,LOW(0x2B)
	BREQ _0x3F4
	RJMP _0x3F5
_0x3F2:
	CALL SUBOPT_0x52
	CALL __ANEGF1
	CALL SUBOPT_0x5E
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x3F4:
	SBRS R16,7
	RJMP _0x3F6
	LDD  R30,Y+19
	CALL SUBOPT_0x120
	RJMP _0x3F7
_0x3F6:
	CALL SUBOPT_0x121
	LDD  R26,Y+19
	STD  Z+0,R26
_0x3F7:
_0x3F5:
	SBRS R16,5
	LDI  R20,LOW(6)
	CPI  R18,102
	BRNE _0x3F9
	CALL SUBOPT_0x62
	ST   -Y,R20
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoa_GD
	RJMP _0x3FA
_0x3F9:
	CALL SUBOPT_0x62
	ST   -Y,R20
	ST   -Y,R18
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoe_GD
_0x3FA:
	MOVW R30,R28
	ADIW R30,20
	CALL SUBOPT_0x122
	RJMP _0x3FB
_0x3F1:
	CPI  R30,LOW(0x73)
	BRNE _0x3FD
	CALL SUBOPT_0x11F
	CALL SUBOPT_0x123
	CALL SUBOPT_0x122
	RJMP _0x3FE
_0x3FD:
	CPI  R30,LOW(0x70)
	BRNE _0x400
	CALL SUBOPT_0x11F
	CALL SUBOPT_0x123
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x3FE:
	ANDI R16,LOW(127)
	CPI  R20,0
	BREQ _0x402
	CP   R20,R17
	BRLO _0x403
_0x402:
	RJMP _0x401
_0x403:
	MOV  R17,R20
_0x401:
_0x3FB:
	LDI  R20,LOW(0)
	LDI  R30,LOW(0)
	STD  Y+18,R30
	LDI  R19,LOW(0)
	RJMP _0x404
_0x400:
	CPI  R30,LOW(0x64)
	BREQ _0x407
	CPI  R30,LOW(0x69)
	BRNE _0x408
_0x407:
	ORI  R16,LOW(4)
	RJMP _0x409
_0x408:
	CPI  R30,LOW(0x75)
	BRNE _0x40A
_0x409:
	LDI  R30,LOW(10)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x40B
	__GETD1N 0x3B9ACA00
	CALL SUBOPT_0x11B
	LDI  R17,LOW(10)
	RJMP _0x40C
_0x40B:
	__GETD1N 0x2710
	CALL SUBOPT_0x11B
	LDI  R17,LOW(5)
	RJMP _0x40C
_0x40A:
	CPI  R30,LOW(0x58)
	BRNE _0x40E
	ORI  R16,LOW(8)
	RJMP _0x40F
_0x40E:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x44D
_0x40F:
	LDI  R30,LOW(16)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x411
	__GETD1N 0x10000000
	CALL SUBOPT_0x11B
	LDI  R17,LOW(8)
	RJMP _0x40C
_0x411:
	__GETD1N 0x1000
	CALL SUBOPT_0x11B
	LDI  R17,LOW(4)
_0x40C:
	CPI  R20,0
	BREQ _0x412
	ANDI R16,LOW(127)
	RJMP _0x413
_0x412:
	LDI  R20,LOW(1)
_0x413:
	SBRS R16,1
	RJMP _0x414
	CALL SUBOPT_0x11F
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	RJMP _0x4BF
_0x414:
	SBRS R16,2
	RJMP _0x416
	CALL SUBOPT_0x11F
	CALL SUBOPT_0x123
	CALL __CWD1
	RJMP _0x4BF
_0x416:
	CALL SUBOPT_0x11F
	CALL SUBOPT_0x123
	CLR  R22
	CLR  R23
_0x4BF:
	__PUTD1S 6
	SBRS R16,2
	RJMP _0x418
	CALL SUBOPT_0x51
	CALL __CPD20
	BRGE _0x419
	CALL SUBOPT_0x52
	CALL __ANEGD1
	CALL SUBOPT_0x5E
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x419:
	LDD  R30,Y+19
	CPI  R30,0
	BREQ _0x41A
	SUBI R17,-LOW(1)
	SUBI R20,-LOW(1)
	RJMP _0x41B
_0x41A:
	ANDI R16,LOW(251)
_0x41B:
_0x418:
	MOV  R19,R20
_0x404:
	SBRC R16,0
	RJMP _0x41C
_0x41D:
	CP   R17,R21
	BRSH _0x420
	CP   R19,R21
	BRLO _0x421
_0x420:
	RJMP _0x41F
_0x421:
	SBRS R16,7
	RJMP _0x422
	SBRS R16,2
	RJMP _0x423
	ANDI R16,LOW(251)
	LDD  R18,Y+19
	SUBI R17,LOW(1)
	RJMP _0x424
_0x423:
	LDI  R18,LOW(48)
_0x424:
	RJMP _0x425
_0x422:
	LDI  R18,LOW(32)
_0x425:
	CALL SUBOPT_0x11E
	SUBI R21,LOW(1)
	RJMP _0x41D
_0x41F:
_0x41C:
_0x426:
	CP   R17,R20
	BRSH _0x428
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x429
	CALL SUBOPT_0x124
	BREQ _0x42A
	SUBI R21,LOW(1)
_0x42A:
	SUBI R17,LOW(1)
	SUBI R20,LOW(1)
_0x429:
	LDI  R30,LOW(48)
	CALL SUBOPT_0x120
	CPI  R21,0
	BREQ _0x42B
	SUBI R21,LOW(1)
_0x42B:
	SUBI R20,LOW(1)
	RJMP _0x426
_0x428:
	MOV  R19,R17
	LDD  R30,Y+18
	CPI  R30,0
	BRNE _0x42C
_0x42D:
	CPI  R19,0
	BREQ _0x42F
	SBRS R16,3
	RJMP _0x430
	CALL SUBOPT_0x121
	LPM  R30,Z
	RJMP _0x4C0
_0x430:
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X+
	STD  Y+10,R26
	STD  Y+10+1,R27
_0x4C0:
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
	BREQ _0x432
	SUBI R21,LOW(1)
_0x432:
	SUBI R19,LOW(1)
	RJMP _0x42D
_0x42F:
	RJMP _0x433
_0x42C:
_0x435:
	CALL SUBOPT_0x116
	CALL SUBOPT_0x51
	CALL __DIVD21U
	MOV  R18,R30
	CPI  R18,10
	BRLO _0x437
	SBRS R16,3
	RJMP _0x438
	SUBI R18,-LOW(55)
	RJMP _0x439
_0x438:
	SUBI R18,-LOW(87)
_0x439:
	RJMP _0x43A
_0x437:
	SUBI R18,-LOW(48)
_0x43A:
	SBRC R16,4
	RJMP _0x43C
	CPI  R18,49
	BRSH _0x43E
	CALL SUBOPT_0x118
	__CPD2N 0x1
	BRNE _0x43D
_0x43E:
	RJMP _0x440
_0x43D:
	CP   R20,R19
	BRLO _0x441
	RJMP _0x4C1
_0x441:
	CP   R21,R19
	BRLO _0x443
	SBRS R16,0
	RJMP _0x444
_0x443:
	RJMP _0x442
_0x444:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x445
_0x4C1:
	LDI  R18,LOW(48)
_0x440:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x446
	CALL SUBOPT_0x124
	BREQ _0x447
	SUBI R21,LOW(1)
_0x447:
_0x446:
_0x445:
_0x43C:
	CALL SUBOPT_0x11E
	CPI  R21,0
	BREQ _0x448
	SUBI R21,LOW(1)
_0x448:
_0x442:
	SUBI R19,LOW(1)
	CALL SUBOPT_0x116
	CALL SUBOPT_0x51
	CALL __MODD21U
	CALL SUBOPT_0x5E
	LDD  R30,Y+18
	CALL SUBOPT_0x118
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __DIVD21U
	CALL SUBOPT_0x11B
	CALL SUBOPT_0x116
	CALL __CPD10
	BREQ _0x436
	RJMP _0x435
_0x436:
_0x433:
	SBRS R16,0
	RJMP _0x449
_0x44A:
	CPI  R21,0
	BREQ _0x44C
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	CALL SUBOPT_0x120
	RJMP _0x44A
_0x44C:
_0x449:
_0x44D:
_0x3EB:
_0x4BE:
	LDI  R17,LOW(0)
_0x3CC:
	RJMP _0x3C7
_0x3C9:
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
_0x44E:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isspace
	CPI  R30,0
	BREQ _0x450
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x44E
_0x450:
	LDI  R30,LOW(0)
	STD  Y+7,R30
	CPI  R21,43
	BRNE _0x451
	RJMP _0x4C2
_0x451:
	CPI  R21,45
	BRNE _0x453
	LDI  R30,LOW(1)
	STD  Y+7,R30
_0x4C2:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x453:
	LDI  R30,LOW(0)
	MOV  R20,R30
	MOV  R21,R30
	__GETWRS 16,17,16
_0x454:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BRNE _0x457
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	LDI  R30,LOW(46)
	CALL __EQB12
	MOV  R21,R30
	CPI  R30,0
	BREQ _0x456
_0x457:
	OR   R20,R21
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x454
_0x456:
	__GETWRS 18,19,16
	CPI  R20,0
	BREQ _0x459
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x45A:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	CPI  R26,LOW(0x2E)
	BREQ _0x45C
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x125
	CALL SUBOPT_0x113
	CALL SUBOPT_0x65
	__GETD1N 0x41200000
	CALL __DIVF21
	CALL SUBOPT_0x7A
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x45A
_0x45C:
_0x459:
	CALL SUBOPT_0x26
	CALL SUBOPT_0x11B
_0x45D:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,1
	STD  Y+16,R26
	STD  Y+16+1,R27
	CP   R26,R16
	CPC  R27,R17
	BRLO _0x45F
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x118
	CALL SUBOPT_0x113
	CALL SUBOPT_0x7D
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x11B
	RJMP _0x45D
_0x45F:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R21,X
	CPI  R21,101
	BREQ _0x461
	CPI  R21,69
	BREQ _0x461
	RJMP _0x460
_0x461:
	LDI  R30,LOW(0)
	MOV  R20,R30
	STD  Y+6,R30
	MOVW R26,R18
	LD   R21,X
	CPI  R21,43
	BRNE _0x463
	RJMP _0x4C3
_0x463:
	CPI  R21,45
	BRNE _0x465
	LDI  R30,LOW(1)
	STD  Y+6,R30
_0x4C3:
	__ADDWRN 18,19,1
_0x465:
_0x466:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BREQ _0x468
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R30,R21
	SUBI R30,LOW(48)
	MOV  R20,R30
	RJMP _0x466
_0x468:
	CPI  R20,39
	BRLO _0x469
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x46A
	__GETD1N 0xFF7FFFFF
	RJMP _0x47F
_0x46A:
	__GETD1N 0x7F7FFFFF
	RJMP _0x47F
_0x469:
	LDI  R21,LOW(32)
	CALL SUBOPT_0x26
	CALL SUBOPT_0x11B
_0x46B:
	CPI  R21,0
	BREQ _0x46D
	CALL SUBOPT_0x116
	CALL SUBOPT_0x118
	CALL __MULF12
	CALL SUBOPT_0x11B
	MOV  R30,R20
	AND  R30,R21
	BREQ _0x46E
	CALL SUBOPT_0x11A
	CALL SUBOPT_0x11B
_0x46E:
	LSR  R21
	RJMP _0x46B
_0x46D:
	LDD  R30,Y+6
	CPI  R30,0
	BREQ _0x46F
	CALL SUBOPT_0x116
	CALL SUBOPT_0x125
	CALL __DIVF21
	RJMP _0x4C4
_0x46F:
	CALL SUBOPT_0x116
	CALL SUBOPT_0x125
	CALL __MULF12
_0x4C4:
	__PUTD1S 8
_0x460:
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x471
	CALL SUBOPT_0x126
	CALL SUBOPT_0x7A
_0x471:
	CALL SUBOPT_0x80
_0x47F:
	CALL __LOADLOCR6
	ADIW R28,18
	RET
_log:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x51
	CALL __CPD02
	BRLT _0x472
	__GETD1N 0xFF7FFFFF
	RJMP _0x47E
_0x472:
	RCALL SUBOPT_0x62
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
	RCALL SUBOPT_0x5E
	RCALL SUBOPT_0x51
	__GETD1N 0x3F3504F3
	CALL __CMPF12
	BRSH _0x473
	RCALL SUBOPT_0x52
	RCALL SUBOPT_0x51
	RCALL SUBOPT_0x50
	__SUBWRN 16,17,1
_0x473:
	RCALL SUBOPT_0x5F
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x52
	__GETD2N 0x3F800000
	CALL __ADDF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	RCALL SUBOPT_0x83
	CALL SUBOPT_0x127
	RCALL SUBOPT_0xAD
	__GETD2N 0x3F654226
	RCALL SUBOPT_0x29
	__GETD1N 0x4054114E
	RCALL SUBOPT_0x2B
	CALL SUBOPT_0x127
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x4F
	__GETD1N 0x3FD4114D
	RCALL SUBOPT_0x2B
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
_0x47E:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,10
	RET
_exp:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x6B
	__GETD1N 0xC2AEAC50
	CALL __CMPF12
	BRSH _0x474
	RCALL SUBOPT_0x79
	RJMP _0x47D
_0x474:
	RCALL SUBOPT_0x49
	CALL __CPD10
	BRNE _0x475
	RCALL SUBOPT_0x26
	RJMP _0x47D
_0x475:
	RCALL SUBOPT_0x6B
	__GETD1N 0x42B17218
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x476
	__GETD1N 0x7F7FFFFF
	RJMP _0x47D
_0x476:
	RCALL SUBOPT_0x6B
	__GETD1N 0x3FB8AA3B
	CALL __MULF12
	RCALL SUBOPT_0x47
	RCALL SUBOPT_0x49
	CALL __PUTPARD1
	CALL _floor
	CALL __CFD1
	MOVW R16,R30
	MOVW R30,R16
	RCALL SUBOPT_0x6B
	CALL __CWD1
	CALL __CDF1
	RCALL SUBOPT_0x2B
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F000000
	RCALL SUBOPT_0x2B
	RCALL SUBOPT_0x83
	CALL SUBOPT_0x127
	RCALL SUBOPT_0xAD
	__GETD2N 0x3D6C4C6D
	CALL __MULF12
	__GETD2N 0x40E6E3A6
	CALL __ADDF12
	CALL SUBOPT_0x127
	RCALL SUBOPT_0x5E
	RCALL SUBOPT_0x53
	__GETD2N 0x41A68D28
	CALL __ADDF12
	RCALL SUBOPT_0x6F
	RCALL SUBOPT_0x52
	RCALL SUBOPT_0x4F
	CALL __ADDF12
	__GETD2N 0x3FB504F3
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x51
	RCALL SUBOPT_0x53
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
_0x47D:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
_pow:
	SBIW R28,4
	RCALL SUBOPT_0x80
	CALL __CPD10
	BRNE _0x477
	RCALL SUBOPT_0x79
	RJMP _0x47C
_0x477:
	CALL SUBOPT_0x125
	CALL __CPD02
	BRGE _0x478
	RCALL SUBOPT_0x1
	CALL __CPD10
	BRNE _0x479
	RCALL SUBOPT_0x26
	RJMP _0x47C
_0x479:
	RCALL SUBOPT_0x80
	RCALL SUBOPT_0x66
	CALL SUBOPT_0x11C
	RCALL SUBOPT_0x7C
	RJMP _0x47C
_0x478:
	RCALL SUBOPT_0x1
	MOVW R26,R28
	CALL __CFD1
	CALL __PUTDP1
	RCALL SUBOPT_0x6
	CALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x1
	CALL __CPD12
	BREQ _0x47A
	RCALL SUBOPT_0x79
	RJMP _0x47C
_0x47A:
	CALL SUBOPT_0x126
	RCALL SUBOPT_0x66
	CALL SUBOPT_0x11C
	RCALL SUBOPT_0x7C
	RCALL SUBOPT_0x7A
	LD   R30,Y
	ANDI R30,LOW(0x1)
	BRNE _0x47B
	RCALL SUBOPT_0x80
	RJMP _0x47C
_0x47B:
	CALL SUBOPT_0x126
_0x47C:
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 65 TIMES, CODE SIZE REDUCTION:125 WORDS
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6:
	__GETD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:41 WORDS
SUBOPT_0x7:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_namebuf)
	SBCI R31,HIGH(-_namebuf)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x8:
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
	RJMP SUBOPT_0x7

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x9:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R17,R30
	CPI  R17,255
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xA:
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
SUBOPT_0xB:
	__POINTW2MN _tprbdata,480
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC:
	MOVW R30,R12
	ADIW R30,1
	ST   -Y,R30
	JMP  _scanner_set_channel

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD:
	__GETD1N 0x186A0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:57 WORDS
SUBOPT_0xE:
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
SUBOPT_0xF:
	MOVW R30,R12
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x10:
	__POINTW1FN _0,33
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R12
	ADIW R30,1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x11:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x12:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x13:
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x14:
	__POINTW1FN _0,41
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0x13

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x15:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x16:
	LDI  R30,LOW(254)
	ST   -Y,R30
	CALL _pgmain_handler
	LDI  R30,LOW(0)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x17:
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(5)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x18:
	__POINTW2MN _rprbdata,480
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x19:
	LDI  R30,LOW(105)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x1A:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x1B:
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	__GETD2N 0x186A0
	CALL __CWD1
	CALL __MULD12
	RJMP SUBOPT_0xE

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1C:
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x1D:
	CALL _nav_read
	CALL __PUTPARD1
	JMP  _mabs

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x1E:
	__PUTD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x1F:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x20:
	LDI  R30,LOW(106)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x21:
	STS  _valuep,R30
	STS  _valuep+1,R31
	STS  _valuep+2,R22
	STS  _valuep+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:51 WORDS
SUBOPT_0x22:
	LDS  R26,_valuep
	LDS  R27,_valuep+1
	LDS  R24,_valuep+2
	LDS  R25,_valuep+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x23:
	RCALL SUBOPT_0x22
	__GETD1N 0x3E570A3D
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x24:
	__GETD1N 0x40000000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x25:
	RCALL SUBOPT_0x22
	__GETD1N 0x3DD70A3D
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 19 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x26:
	__GETD1N 0x3F800000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x27:
	__GETD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x28:
	__POINTW2MN _sysdata,8
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x29:
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x2A:
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 25 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x2B:
	CALL __SWAPD12
	CALL __SUBF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x2C:
	__GETD1MN _rundata,192
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2D:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 40 TIMES, CODE SIZE REDUCTION:153 WORDS
SUBOPT_0x2E:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x2F:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:28 WORDS
SUBOPT_0x30:
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
	RCALL SUBOPT_0xD
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
SUBOPT_0x31:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x32:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x33:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _wnd_msgbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x34:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	RJMP SUBOPT_0x1F

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x35:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R10,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x36:
	LDI  R30,LOW(_pgmain_handler)
	LDI  R31,HIGH(_pgmain_handler)
	LDS  R26,_curr_window
	LDS  R27,_curr_window+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:30 WORDS
SUBOPT_0x37:
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
SUBOPT_0x38:
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:42 WORDS
SUBOPT_0x39:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x3A:
	__POINTW2MN _sysdata,14
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x3B:
	__POINTW1MN _tprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3C:
	__SUBD1N -1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3D:
	LDS  R26,_onesec_cnt
	LDS  R27,_onesec_cnt+1
	LDS  R24,_onesec_cnt+2
	LDS  R25,_onesec_cnt+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x3E:
	__POINTW1MN _rprbdata,480
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x3F:
	CALL __MULW12U
	ADD  R30,R22
	ADC  R31,R23
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x40:
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
SUBOPT_0x41:
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
SUBOPT_0x42:
	SUBI R30,LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
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
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:81 WORDS
SUBOPT_0x44:
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
SUBOPT_0x45:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	STD  Y+4,R26
	STD  Y+4+1,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x46:
	__PUTD1S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0x47:
	__PUTD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x48:
	__GETD2S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x49:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x4A:
	CALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3851B717
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x4B:
	MOVW R26,R16
	__ADDWRN 16,17,1
	CPI  R26,LOW(0x64)
	LDI  R30,HIGH(0x64)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x4C:
	RCALL SUBOPT_0x49
	RCALL SUBOPT_0x48
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x24
	CALL __DIVF21
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x4D:
	__GETD2S 30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x4E:
	CALL __MULF12
	__GETD2S 2
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 14 TIMES, CODE SIZE REDUCTION:23 WORDS
SUBOPT_0x4F:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x50:
	CALL __ADDF12
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 25 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x51:
	__GETD2S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x52:
	__GETD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0x53:
	__GETD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x54:
	RCALL SUBOPT_0x53
	RJMP SUBOPT_0x46

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x55:
	RCALL SUBOPT_0x48
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x56:
	__GETD2S 10
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x57:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x24
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x58:
	LDI  R31,0
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x59:
	__POINTW2MN _rprbdata,480
	LDD  R30,Y+30
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5A:
	__GETD1S 31
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5B:
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x26
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5C:
	RCALL SUBOPT_0x48
	RCALL SUBOPT_0x49
	RJMP SUBOPT_0x4A

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5D:
	RCALL SUBOPT_0x53
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x5E:
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x5F:
	RCALL SUBOPT_0x51
	RCALL SUBOPT_0x26
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x60:
	__GETD2S 26
	CALL __MULF12
	RCALL SUBOPT_0x51
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x61:
	__GETD2S 22
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x62:
	RCALL SUBOPT_0x52
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x63:
	RCALL SUBOPT_0x53
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x64:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	__GETD2S 10
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x65:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x66:
	CALL __PUTPARD1
	RJMP _log

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x67:
	__PUTD1S 2
	RJMP SUBOPT_0x63

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x68:
	MOVW R26,R16
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x69:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	__GETD2S 10
	CALL __MULF12
	RCALL SUBOPT_0x51
	RJMP SUBOPT_0x50

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6A:
	RCALL SUBOPT_0x53
	__GETD2S 10
	CALL __MULF12
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x6B:
	__GETD2S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6C:
	CALL __DIVF21
	RCALL SUBOPT_0x5E
	RCALL SUBOPT_0x52
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x6D:
	CALL __GETD1PF
	__PUTD1S 2
	__GETWRN 16,17,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x6E:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RCALL SUBOPT_0x6B
	CALL __MULF12
	RCALL SUBOPT_0x4F
	CALL __ADDF12
	__PUTD1S 2
	RCALL SUBOPT_0x52
	RCALL SUBOPT_0x6B
	CALL __MULF12
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x6F:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x70:
	__GETD2S 13
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x71:
	STS  _coef,R30
	STS  _coef+1,R31
	RJMP SUBOPT_0x70

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x72:
	__GETD2S 13
	__GETD1N 0x441DA75C
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x73:
	STS  _coef,R30
	STS  _coef+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x74:
	__GETD2S 13
	__GETD1N 0x443E0000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x75:
	__GETD2S 13
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x76:
	__GETD1N 0x448505C3
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x77:
	RCALL SUBOPT_0x75
	__GETD1N 0x44D01000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x78:
	RCALL SUBOPT_0x73
	RCALL SUBOPT_0x75
	RJMP SUBOPT_0x76

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x79:
	__GETD1N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x7A:
	__PUTD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x7B:
	RCALL SUBOPT_0x75
	__GETD1N 0x42FDEFEC
	RJMP SUBOPT_0x2B

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x7C:
	CALL __MULF12
	CALL __PUTPARD1
	RJMP _exp

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x7D:
	CALL __MULF12
	__GETD2S 8
	CALL __ADDF12
	RJMP SUBOPT_0x7A

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x7E:
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x7F:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x80:
	__GETD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x81:
	__GETD1N 0xC3870000
	RJMP SUBOPT_0x46

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x82:
	__GETD1N 0xC2480000
	RCALL SUBOPT_0x46
	__GETD1N 0x44DD0000
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x83:
	RCALL SUBOPT_0x5E
	RJMP SUBOPT_0x52

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x84:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x85:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_lcdcmd_G8)
	SBCI R31,HIGH(-_lcdcmd_G8)
	LD   R30,Z
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x86:
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
SUBOPT_0x87:
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _onecmd
	__CLRD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x88:
	LDI  R30,LOW(3)
	STS  _lcdcmd_G8,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _sendcmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x89:
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
SUBOPT_0x8A:
	MOV  R30,R17
	LDI  R31,0
	__ADDW1MN _lcdcmd_G8,3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x8B:
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
SUBOPT_0x8C:
	LDI  R26,LOW(10)
	STD  Z+0,R26
	LDI  R30,LOW(_scancmd)
	LDI  R31,HIGH(_scancmd)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x8D:
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x8E:
	LDI  R30,LOW(_navcmd_G9)
	LDI  R31,HIGH(_navcmd_G9)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x8F:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	__GETD1N 0xC47A0000
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RJMP SUBOPT_0x8E

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x90:
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
SUBOPT_0x91:
	__GETD1N 0xC47A0000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x92:
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x93:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	RCALL SUBOPT_0x91
	RJMP SUBOPT_0x92

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x94:
	LDS  R30,_navlen_G9
	LDI  R31,0
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x95:
	CALL __PUTPARD1
	JMP  _wnd_floatinput

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x96:
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
	RJMP SUBOPT_0x0

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x97:
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R30,R10
	CPC  R31,R11
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x98:
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
SUBOPT_0x99:
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x9A:
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
SUBOPT_0x9B:
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
SUBOPT_0x9C:
	__POINTW1FN _0,239
	RJMP SUBOPT_0x2E

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x9D:
	MOVW R26,R10
	SBIW R26,1
	CP   R12,R26
	CPC  R13,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0x9E:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_values*2)
	SBCI R31,HIGH(-_pgmain_values*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0x9F:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xA0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xA1:
	__POINTW1FN _0,250
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	SBIW R30,1
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RJMP SUBOPT_0x11

;OPTIMIZER ADDED SUBROUTINE, CALLED 29 TIMES, CODE SIZE REDUCTION:53 WORDS
SUBOPT_0xA2:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xA3:
	__POINTW1MN _strbuf,8
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA4:
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xA5:
	__POINTW2MN _rundata,96
	MOVW R30,R10
	SBIW R30,1
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xA6:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0xC60CA000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xA7:
	__POINTW1FN _0,272
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0xA5

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0xA8:
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
	RJMP SUBOPT_0xA2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xA9:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	CALL __GETW1PF
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xAA:
	LDI  R30,LOW(221)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x3F

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0xAB:
	SBIW R30,2
	ST   -Y,R30
	LDI  R30,LOW(233)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x3F

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xAC:
	__POINTW2MN _sysdata,38
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xAD:
	RCALL SUBOPT_0x6F
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xAE:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgr_options*2)
	SBCI R31,HIGH(-_pgr_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xAF:
	CALL __CWD1
	CALL __PUTPARD1
	RJMP SUBOPT_0xA2

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0xB0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB1:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgt_options*2)
	SBCI R31,HIGH(-_pgt_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xB2:
	CALL __SAVELOCR6
	LDI  R16,6
	LDI  R17,1
	LDI  R18,24
	LDI  R19,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB3:
	LDS  R30,_curr_pos_S4D
	LDS  R31,_curr_pos_S4D+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB4:
	STS  _last_pos_S4D,R30
	STS  _last_pos_S4D+1,R31
	RJMP SUBOPT_0xB3

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xB5:
	STS  _curr_pos_S4D,R30
	STS  _curr_pos_S4D+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xB6:
	LDS  R26,_curr_pos_S4D
	LDS  R27,_curr_pos_S4D+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB7:
	MOV  R30,R18
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB8:
	RCALL SUBOPT_0xB3
	LDS  R26,_last_pos_S4D
	LDS  R27,_last_pos_S4D+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB9:
	LDS  R26,_curr_pos_S4D
	LDS  R27,_curr_pos_S4D+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBA:
	MOV  R30,R19
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBB:
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(255)
	CALL __EEPROMWRB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBC:
	__POINTW1FN _0,522
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0xBD:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBE:
	MOV  R30,R17
	LDI  R31,0
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xBF:
	RCALL SUBOPT_0xB9
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xC0:
	CALL __DIVW21U
	MOVW R26,R30
	MOV  R30,R16
	LDI  R31,0
	CALL __MULW12U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC1:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,LOW(0xFF)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC2:
	__POINTW1FN _0,525
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0xAF

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xC3:
	__POINTW1FN _0,532
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC4:
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC5:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xC6:
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
SUBOPT_0xC7:
	CALL __MODW21U
	MOVW R26,R30
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xC8:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xC9:
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xCA:
	STS  _curr_pos_S4E,R30
	STS  _curr_pos_S4E+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xCB:
	LDS  R30,_curr_pos_S4E
	LDS  R31,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCC:
	STS  _last_pos_S4E,R30
	STS  _last_pos_S4E+1,R31
	RJMP SUBOPT_0xCB

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xCD:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCE:
	RCALL SUBOPT_0xCB
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCF:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD0:
	RCALL SUBOPT_0xCF
	RJMP SUBOPT_0xC9

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD1:
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
SUBOPT_0xD2:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD3:
	CALL __MODW21U
	MOVW R26,R30
	MOVW R30,R20
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xD4:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0xB0

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xD5:
	LDS  R30,_curr_pos_S4F
	LDS  R31,_curr_pos_S4F+1
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xD6:
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xD7:
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RJMP SUBOPT_0xD5

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xD8:
	STS  _curr_pos_S4F,R30
	STS  _curr_pos_S4F+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD9:
	LDS  R30,_curr_pos_S4F
	LDS  R31,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xDA:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xDB:
	RCALL SUBOPT_0xD9
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xDC:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xDD:
	RCALL SUBOPT_0xDC
	RJMP SUBOPT_0xC9

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xDE:
	__POINTW1FN _0,537
	ST   -Y,R31
	ST   -Y,R30
	MOV  R30,R20
	SUBI R30,LOW(1)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xDF:
	CALL __PUTPARD1
	RJMP SUBOPT_0xA2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xE0:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE1:
	CALL __MODW21U
	MOVW R26,R30
	MOV  R30,R21
	RJMP SUBOPT_0x42

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xE2:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0xB0

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xE3:
	__POINTW2MN _tprbdata,480
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0xE4:
	__POINTW2MN _rprbdata,480
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE5:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R8
	RJMP SUBOPT_0xAF

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xE6:
	MOV  R30,R19
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_tplist_options*2)
	SBCI R31,HIGH(-_tplist_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xE7:
	LDS  R30,_curr_sel_S50
	LDS  R31,_curr_sel_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE8:
	STS  _last_sel_S50,R30
	STS  _last_sel_S50+1,R31
	RJMP SUBOPT_0xE7

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE9:
	STS  _curr_sel_S50,R30
	STS  _curr_sel_S50+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xEA:
	LDS  R26,_curr_sel_S50
	LDS  R27,_curr_sel_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xEB:
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xEC:
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xED:
	__POINTW1FN _0,536
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xEE:
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	RJMP SUBOPT_0xBD

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0xEF:
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RJMP SUBOPT_0xDF

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xF0:
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF1:
	__POINTW2MN _rprbdata,96
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF2:
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF3:
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0x7F

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xF4:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_boot_options*2)
	SBCI R31,HIGH(-_boot_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF5:
	LDI  R30,LOW(_databuf)
	LDI  R31,HIGH(_databuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF6:
	LDI  R30,LOW(0)
	STS  _databuf,R30
	STS  _pos_databuf,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF7:
	CALL _LCD_Cls
	LDI  R30,LOW(_datalbl*2)
	LDI  R31,HIGH(_datalbl*2)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF8:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _draw_inputbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xF9:
	LDS  R30,_pos_databuf
	SUBI R30,-LOW(1)
	STS  _pos_databuf,R30
	RJMP SUBOPT_0x42

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xFA:
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
SUBOPT_0xFB:
	LDS  R30,_pos_databuf
	RJMP SUBOPT_0x42

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xFC:
	LDS  R30,_max_databuf
	LDS  R26,_pos_databuf
	CP   R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0xFD:
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
SUBOPT_0xFE:
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CLR  R30
	ADD  R26,R16
	ADC  R27,R30
	RJMP SUBOPT_0xEB

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xFF:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:38 WORDS
SUBOPT_0x100:
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
SUBOPT_0x101:
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,2
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x102:
	ST   -Y,R30
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x103:
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
SUBOPT_0x104:
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x105:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:67 WORDS
SUBOPT_0x106:
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
SUBOPT_0x107:
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
SUBOPT_0x108:
	ADIW R30,8
	CALL __GETW1PF
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x109:
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
SUBOPT_0x10A:
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
SUBOPT_0x10B:
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
SUBOPT_0x10C:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x10D:
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x10E:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10F:
	RCALL SUBOPT_0x4F
	__GETD1N 0x41200000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x110:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x111:
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	JMP  _floor

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x112:
	MOV  R30,R16
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x113:
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x114:
	__GETD1N 0x41200000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x115:
	__GETD2S 4
	RJMP SUBOPT_0x114

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x116:
	__GETD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x117:
	CALL SUBOPT_0x1
	__GETD2S 12
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x118:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x119:
	__GETD1N 0x41200000
	CALL __DIVF21
	__PUTD1S 12
	SUBI R19,-LOW(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x11A:
	RCALL SUBOPT_0x118
	RJMP SUBOPT_0x114

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x11B:
	__PUTD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x11C:
	__GETD2S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x11D:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADIW R26,1
	STD  Y+8,R26
	STD  Y+8+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x11E:
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
SUBOPT_0x11F:
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
SUBOPT_0x120:
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
SUBOPT_0x121:
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ADIW R30,1
	STD  Y+10,R30
	STD  Y+10+1,R31
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x122:
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x123:
	ADIW R30,4
	MOVW R26,R30
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x124:
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
SUBOPT_0x125:
	__GETD2S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x126:
	RCALL SUBOPT_0x80
	CALL __ANEGF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x127:
	RCALL SUBOPT_0x51
	CALL __MULF12
	RET


	.DSEG
_3:
	.BYTE 0x0C
_4:
	.BYTE 0x10
_548:
	.BYTE 0x0F
_549:
	.BYTE 0x10
_550:
	.BYTE 0x0B
_551:
	.BYTE 0x0D
_552:
	.BYTE 0x0F
_553:
	.BYTE 0x0F
_576:
	.BYTE 0x0F
_577:
	.BYTE 0x11
_578:
	.BYTE 0x11
_597:
	.BYTE 0x11
_641:
	.BYTE 0x13
_721:
	.BYTE 0x08
_722:
	.BYTE 0x07
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
_729:
	.BYTE 0x09
_779:
	.BYTE 0x09
_780:
	.BYTE 0x07
_781:
	.BYTE 0x03
_782:
	.BYTE 0x03
_783:
	.BYTE 0x03
_784:
	.BYTE 0x03
_806:
	.BYTE 0x0F
_807:
	.BYTE 0x0D
_808:
	.BYTE 0x0D
_809:
	.BYTE 0x0D
_810:
	.BYTE 0x0D
_829:
	.BYTE 0x0E
_830:
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
