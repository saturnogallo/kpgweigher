
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
;      59 	1,1,1000};
	.DB  0x1
	.DB  0x1
	.DW  0x0
	.DW  0x447A
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
;     132 //        sprintf(strbuf,"%f\r\n",val);
;     133 //        prints(strbuf,strlen(strbuf),PORT_PC);
;     134 }
	RJMP _0x4BC
;     135 void sdbgout(flash char *l)
;     136 {
;     137         sprintf(strbuf,"%s",l);
;	*l -> Y+0
;     138 prints(strbuf,strlen(strbuf),PORT_PC);
;     139 }
;     140 void InitLED()
;     141 {
_InitLED:
;     142     PORTB = 0xFF;
	LDI  R30,LOW(255)
	OUT  0x18,R30
;     143 /*
;     144     PORTB.7=1;
;     145     PORTB.6=1;
;     146     PORTB.5=1;
;     147     PORTB.4=1;
;     148     PORTD.7=1;
;     149     PORTD.6=0;
;     150 */
;     151 }
	RET
;     152 uchar nav1v = 1;

	.DSEG
_nav1v:
	.BYTE 0x1
;     153 void navto120mv()
;     154 {

	.CSEG
_navto120mv:
;     155    nav1v = 0;
	LDI  R30,LOW(0)
	STS  _nav1v,R30
;     156    nav_command(NAV_120MV);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x0
;     157    sleepms(200*ONEMS);
;     158 }
	RET
;     159 
;     160 void navto1v()
;     161 {
_navto1v:
;     162     nav1v = 1;
	LDI  R30,LOW(1)
	STS  _nav1v,R30
;     163     nav_command(NAV_1V);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x0
;     164     sleepms(200*ONEMS);
;     165 }
	RET
;     166 double mabs(double val)
;     167 {
_mabs:
;     168         if(val < 0)
;	val -> Y+0
	CALL SUBOPT_0x1
	CALL __CPD20
	BRGE _0x9
;     169                 return -val;
	CALL SUBOPT_0x2
	CALL __ANEGF1
	RJMP _0x4BC
;     170         else
_0x9:
;     171                 return val;
	CALL SUBOPT_0x2
;     172 }
_0x4BC:
	ADIW R28,4
	RET
;     173 u8 ToByte(char l, char h)
;     174 {
;     175 	h = (h > 'A')?(h-'A'+0x0A):(h-'0');
;	l -> Y+1
;	h -> Y+0
;     176 	l = (l > 'A')?(l-'A'+0x0A):(l-'0');
;     177 	return (h<<4)|l;
;     178 }
;     179 
;     180 /**************************************************************************************/
;     181 //                              Global Variable
;     182 /**************************************************************************************/
;     183 
;     184 /**************************************************************************************/
;     185 //                               Watchdog
;     186 // Enable watchdog timer.
;     187 // Timeout limit WDTCR[2:0]:
;     188 // 000 - 16.3ms    001 - 32.5ms    010 - 65ms    011 - 0.13s
;     189 // 100 - 0.26ms    101 - 0.52s     110 - 1.0s    111 - 2.1s
;     190 /**************************************************************************************/
;     191 #define WDCE 4
;     192 #define WDE  3
;     193 
;     194 
;     195 
;     196 void Turn_on_Watchdog()
;     197 {
;     198   #asm("cli")
;     199   #asm("wdr")                    // reset watchdog timer to avoid timeout reset
;     200   WDTCR |= (1<<WDCE) | (1<<WDE); // set WDCE and WDE bit
;     201   WDTCR = 0x08;                  // turn on watch dog timer: WDE = 1, WDP = 000, 16.3ms.
;     202   #asm("sei")                    // Enable global interrupt.
;     203 }
;     204 
;     205 char namebuf[8];

	.DSEG
_namebuf:
	.BYTE 0x8
;     206 char* tname2b(u8 i)
;     207 {

	.CSEG
_tname2b:
;     208         u8 j=0;
;     209         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x12:
	CPI  R17,8
	BRSH _0x13
;     210         {
;     211             namebuf[j] = tprbdata.name[i][j];
	CALL SUBOPT_0x3
	MOVW R0,R30
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0x4
;     212             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x13
;     213                 break;
;     214 
;     215             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x16
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x18
_0x16:
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x19
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x18
_0x19:
	RJMP _0x15
_0x18:
;     216                 continue;
	RJMP _0x11
;     217             namebuf[0] = '\0';
_0x15:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     218             break;
	RJMP _0x13
;     219         }
_0x11:
	SUBI R17,-1
	RJMP _0x12
_0x13:
;     220         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
	RJMP _0x4BB
;     221 }
;     222 
;     223 char* rname2b(u8 i)
;     224 {
_rname2b:
;     225         u8 j=0;
;     226         for(j=0;j<8;j++)
	ST   -Y,R17
;	i -> Y+1
;	j -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x1D:
	CPI  R17,8
	BRSH _0x1E
;     227         {
;     228             namebuf[j] = rprbdata.name[i][j];
	CALL SUBOPT_0x3
	MOVW R0,R30
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0x4
;     229             if(namebuf[j] == '\0')
	LD   R30,Z
	CPI  R30,0
	BREQ _0x1E
;     230                 break;
;     231 
;     232             if((namebuf[j] >= '0' && namebuf[j] <= '9') || (namebuf[j] >= 'A' && namebuf[j] <= 'Z'))
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRLO _0x21
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x3A)
	BRLO _0x23
_0x21:
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRLO _0x24
	CALL SUBOPT_0x3
	LD   R30,Z
	CPI  R30,LOW(0x5B)
	BRLO _0x23
_0x24:
	RJMP _0x20
_0x23:
;     233                 continue;
	RJMP _0x1C
;     234             namebuf[0] = '\0';
_0x20:
	LDI  R30,LOW(0)
	STS  _namebuf,R30
;     235             break;
	RJMP _0x1E
;     236         }
_0x1C:
	SUBI R17,-1
	RJMP _0x1D
_0x1E:
;     237         return namebuf;
	LDI  R30,LOW(_namebuf)
	LDI  R31,HIGH(_namebuf)
	LDD  R17,Y+0
_0x4BB:
	ADIW R28,2
	RET
;     238 }
;     239 void sleepms(u16 ms)
;     240 {
_sleepms:
;     241     uint p;
;     242     while(ms-- > 0)
	ST   -Y,R17
	ST   -Y,R16
;	ms -> Y+2
;	p -> R16,R17
_0x27:
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	SBIW R26,1
	STD  Y+2,R26
	STD  Y+2+1,R27
	ADIW R26,1
	CALL __CPW02
	BRSH _0x29
;     243     {
;     244         p = 24;//80;
	__GETWRN 16,17,24
;     245         while(p-- > 0)
_0x2A:
	MOVW R26,R16
	__SUBWRN 16,17,1
	CALL __CPW02
	BRLO _0x2A
;     246                 ;
;     247     }
	RJMP _0x27
_0x29:
;     248 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,4
	RET
;     249 
;     250 
;     251 /**************************************************************************************/
;     252 //      board init related function.
;     253 /**************************************************************************************/
;     254 void init_var()
;     255 {
_init_var:
;     256         if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
	BREQ PC+4
	BRCS PC+3
	JMP  _0x2E
	CALL SUBOPT_0x5
	CALL SUBOPT_0x7
	BRSH _0x2D
_0x2E:
;     257                 sysdata.Rs1 = 100;
	CALL SUBOPT_0x8
;     258         if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
_0x2D:
	CALL SUBOPT_0x9
	CALL SUBOPT_0xA
	BREQ PC+4
	BRCS PC+3
	JMP  _0x31
	CALL SUBOPT_0x9
	CALL SUBOPT_0xB
	BRSH _0x30
_0x31:
;     259                 sysdata.Rs2 = 1000;
	CALL SUBOPT_0xC
;     260 }
_0x30:
	RET
;     261 //state machine of therm type
;     262 //phase 0 : search the current channel and switch to it if apply, add delay, to phase 1, otherwise to phase 2
;     263 //phase 1:get the reading and update the display
;     264 //phase 2: move to next channel
;     265 uchar therm_state()
;     266 {
_therm_state:
;     267         uchar i;
;     268 	i = sysdata.tid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	CALL SUBOPT_0xD
	MOV  R17,R30
;     269 	if(phase > 2)
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRLO _0x33
;     270 	        phase = 0;
	LDI  R30,LOW(0)
	STS  _phase,R30
;     271 	if(i == INVALID_PROBE)
_0x33:
	CPI  R17,255
	BRNE _0x34
;     272 	{
;     273         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0xE
;     274         	phase = 2;
;     275 	}
;     276 	if(phase == 0)
_0x34:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x35
;     277 	{
;     278 		if( (tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0xF
	CPI  R30,LOW(0x3)
	BRLO _0x37
	CALL SUBOPT_0xF
	CPI  R30,LOW(0xB)
	BRLO _0x38
_0x37:
	RJMP _0x36
_0x38:
;     279 		{
;     280 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0x10
;     281 			dlg_cnt = 6*ONESEC;
	__GETD1N 0x927C0
	CALL SUBOPT_0x11
;     282 			onesec_cnt = 0;
;     283 			phase = 1;
	LDI  R30,LOW(1)
	STS  _phase,R30
;     284 			return 0;
	LDI  R30,LOW(0)
	RJMP _0x4BA
;     285 		}else{
_0x36:
;     286 			rundata.temperature[ch_to_search] = -9999; //invalid data
	CALL SUBOPT_0xE
;     287 			phase = 2;
;     288 		}
;     289 	}
;     290 	if(phase == 1)
_0x35:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x3A
;     291 	{
;     292 		rundata.reading[ch_to_search] = nav_read();//-sysdata.V0;
	CALL SUBOPT_0x12
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL _nav_read
	POP  R26
	POP  R27
	CALL SUBOPT_0x13
;     293                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.reading[ch_to_search]);
	CALL SUBOPT_0x14
	CALL SUBOPT_0x12
	CALL SUBOPT_0x15
	CALL SUBOPT_0x16
;     294                 prints(strbuf,strlen(strbuf),PORT_PC);
;     295 
;     296 		rundata.temperature[ch_to_search] = MValueToTValue(rundata.reading[ch_to_search], tprbdata.type[i]);
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x12
	CALL SUBOPT_0x15
	CALL SUBOPT_0xF
	ST   -Y,R30
	CALL _MValueToTValue
	POP  R26
	POP  R27
	CALL SUBOPT_0x13
;     297 //		if(ch_to_search==1) 		     rundata.temperature[ch_to_search] = 10;
;     298                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x17
	CALL SUBOPT_0x18
;     299                 prints(strbuf,strlen(strbuf),PORT_PC);
;     300 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     301 	}
;     302 	if(phase == 2)
_0x3A:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x3B
;     303 	{
;     304 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x19
;     305 		phase = 0;
;     306 	}
;     307 	return 1;       //one reading is updated
_0x3B:
	LDI  R30,LOW(1)
	RJMP _0x4BA
;     308 }
;     309 //state machine of bore measure
;     310 //phase0 the search the current channel and switch to it if apply, switch to rs, pktt, add delay, if not ktt to phase 2 else to phase 1, otherwise to phase n
;     311 //phase1 get reading of rs+, set to nktt, add delay
;     312 //phase2 get reading of rs-, switch to rx, add delay, if  ktt to phase 3 else to phase 4
;     313 //phase3 get reading of rx-, set to pktt, add delay
;     314 //phase4 get reading of rx+, switch to rx, caculate
;     315 //phase5 update display and move to next channel
;     316 uchar bore_state()
;     317 {
_bore_state:
;     318         uchar i;
;     319 	i = sysdata.rid[ch_to_search];
	ST   -Y,R17
;	i -> R17
	CALL SUBOPT_0x1A
	MOV  R17,R30
;     320 	if(phase > 5)
	LDS  R26,_phase
	CPI  R26,LOW(0x6)
	BRLO _0x3C
;     321 	        phase = 0;
	LDI  R30,LOW(0)
	STS  _phase,R30
;     322 	if(i == INVALID_PROBE)
_0x3C:
	CPI  R17,255
	BRNE _0x3D
;     323 	{
;     324         	rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x1B
;     325         	phase = 5;
;     326 	}
;     327 	if(phase == 0)
_0x3D:
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x3E
;     328 	{
;     329 		if((rprbdata.type[i] <= PRBTYPE_MAX) && (rprbdata.type[i] >= PRBTYPE_MIN))
	CALL SUBOPT_0x1C
	CPI  R30,LOW(0xF4)
	BRSH _0x40
	CALL SUBOPT_0x1C
	CPI  R30,LOW(0xF1)
	BRSH _0x41
_0x40:
	RJMP _0x3F
_0x41:
;     330 		{
;     331 			scanner_set_channel(ch_to_search+1);
	CALL SUBOPT_0x10
;     332 			SET_PKTT;
	CALL SUBOPT_0x1D
;     333 			SET_TORS;
	CALL SUBOPT_0x1E
;     334 
;     335 			if(rprbdata.type[i] == PRBTYPE_PT100)
	CALL SUBOPT_0x1C
	CPI  R30,LOW(0xF1)
	BRNE _0x42
;     336 			{
;     337 			        SET_TOPT100;
	CALL SUBOPT_0x1F
;     338 			        SET_TO1MA;
;     339 			        PORTF = 0x0f;
;     340 			}
;     341 			if(rprbdata.type[i] == PRBTYPE_PT25)
_0x42:
	CALL SUBOPT_0x1C
	CPI  R30,LOW(0xF2)
	BRNE _0x43
;     342 			{
;     343 			        SET_TOPT100;
	CALL SUBOPT_0x1F
;     344 			        SET_TO1MA;
;     345 			        PORTF = 0x0f;
;     346 			}
;     347 
;     348 			if(rprbdata.type[i] == PRBTYPE_PT1000)
_0x43:
	CALL SUBOPT_0x1C
	CPI  R30,LOW(0xF3)
	BRNE _0x44
;     349 			{
;     350 			        SET_TOP1MA;
	CALL SUBOPT_0x20
;     351        			        SET_TOPT1000;
;     352        			        PORTF = 0x00;
	LDI  R30,LOW(0)
	STS  98,R30
;     353 			}
;     354 			dlg_cnt =  ONESEC * sysdata.ktime;
_0x44:
	CALL SUBOPT_0x21
;     355 			onesec_cnt = 0;
;     356 			if(IS_MODE_KTT)
	CALL SUBOPT_0x22
	BRNE _0x45
;     357 				phase = 1;
	LDI  R30,LOW(1)
	RJMP _0x4BD
;     358 			else
_0x45:
;     359 				phase = 2;
	LDI  R30,LOW(2)
_0x4BD:
	STS  _phase,R30
;     360        			return 0;
	LDI  R30,LOW(0)
	RJMP _0x4BA
;     361 		}else{
_0x3F:
;     362 			rundata.temperature[ch_to_search] = -9999;
	CALL SUBOPT_0x1B
;     363 			phase = 5;
;     364 		}
;     365 
;     366 	}
;     367 	if(phase == 1)  //-stdV in ktt mode
_0x3E:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x48
;     368 	{
;     369 		rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x23
	CALL SUBOPT_0x24
;     370 		SET_NKTT;
	LDI  R30,LOW(105)
	CALL SUBOPT_0x25
;     371 		dlg_cnt =  ONESEC * sysdata.ktime;
	CALL SUBOPT_0x21
;     372 		onesec_cnt = 0;
;     373 		phase = 2;
	LDI  R30,LOW(2)
	STS  _phase,R30
;     374 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x4BA
;     375 	}
;     376 	if(phase == 2) //final stdV
_0x48:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x49
;     377 	{
;     378 		if(IS_MODE_KTT)
	CALL SUBOPT_0x22
	BRNE _0x4A
;     379 		{
;     380 			rundata.stdV = (rundata.stdV + mabs(nav_read()));
	CALL SUBOPT_0x23
	__GETD2MN _rundata,196
	CALL __ADDF12
	CALL SUBOPT_0x24
;     381 			phase = 3;
	LDI  R30,LOW(3)
	RJMP _0x4BE
;     382 		}else{
_0x4A:
;     383 			rundata.stdV = mabs(nav_read());
	CALL SUBOPT_0x23
	CALL SUBOPT_0x24
;     384 			phase = 4;
	LDI  R30,LOW(4)
_0x4BE:
	STS  _phase,R30
;     385 		}
;     386 		SET_TORX;
	CALL SUBOPT_0x26
;     387 
;     388 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x21
;     389 		onesec_cnt = 0;
;     390 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x4BA
;     391 	}
;     392 	if(phase == 3)  //-V on rx in ktt mode
_0x49:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x4C
;     393 	{
;     394 		valuep = mabs(nav_read());
	CALL SUBOPT_0x23
	CALL SUBOPT_0x27
;     395 		SET_PKTT;
	CALL SUBOPT_0x1D
;     396 		dlg_cnt = ONESEC * sysdata.ktime;
	CALL SUBOPT_0x21
;     397 		onesec_cnt = 0;
;     398 		phase = 4;
	LDI  R30,LOW(4)
	STS  _phase,R30
;     399 		return 0;
	LDI  R30,LOW(0)
	RJMP _0x4BA
;     400 	}
;     401 	if(phase == 4) // final V on rx
_0x4C:
	LDS  R26,_phase
	CPI  R26,LOW(0x4)
	BREQ PC+3
	JMP _0x4D
;     402 	{
;     403 		if(IS_MODE_KTT){
	CALL SUBOPT_0x22
	BRNE _0x4E
;     404 			valuep = (valuep + mabs(nav_read()));
	CALL SUBOPT_0x23
	CALL SUBOPT_0x28
	CALL __ADDF12
	CALL SUBOPT_0x27
;     405 			dbgout(valuep);
	LDS  R30,_valuep
	LDS  R31,_valuep+1
	LDS  R22,_valuep+2
	LDS  R23,_valuep+3
	CALL __PUTPARD1
	CALL _dbgout
;     406 /*
;     407 			if((valuep > 0.21) && (valuep < 2) && (nav1v == 0))
;     408 			{
;     409         		        navto1v();
;     410 			}
;     411 			if((valuep < 0.21) && (valuep > 0.0002) && (nav1v == 1))
;     412 			{
;     413 			        navto120mv();
;     414 			}
;     415 */
;     416 		}else{
	RJMP _0x4F
_0x4E:
;     417 			valuep = mabs(nav_read());
	CALL SUBOPT_0x23
	CALL SUBOPT_0x27
;     418 /*
;     419 			if((valuep > 0.105) && (valuep < 1) && (nav1v == 0))
;     420 			{
;     421         		        navto1v();
;     422 			}
;     423 			if((valuep < 0.105) && (valuep > 0.0001) && (nav1v == 1))
;     424 			{
;     425 			        navto120mv();
;     426 			}
;     427 */
;     428 		}
_0x4F:
;     429 		if(rundata.stdV != 0)
	CALL SUBOPT_0x29
	CALL __CPD10
	BRNE PC+3
	JMP _0x50
;     430 		{
;     431                         if(rprbdata.type[sysdata.rid[ch_to_search]] == PRBTYPE_PT1000)
	__POINTW1MN _rprbdata,576
	MOVW R0,R30
	CALL SUBOPT_0x1A
	MOVW R26,R0
	CALL SUBOPT_0x2A
	CPI  R30,LOW(0xF3)
	BRNE _0x51
;     432               			rundata.reading[ch_to_search] = valuep*sysdata.Rs2/rundata.stdV - sysdata.R0;
	CALL SUBOPT_0x12
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	__POINTW2MN _sysdata,64
	CALL SUBOPT_0x2B
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x2D
	POP  R26
	POP  R27
	RJMP _0x4BF
;     433                         else
_0x51:
;     434         			rundata.reading[ch_to_search] = valuep*sysdata.Rs1/rundata.stdV - sysdata.R0;
	CALL SUBOPT_0x12
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	__POINTW2MN _sysdata,8
	CALL SUBOPT_0x2B
	CALL __DIVF21
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x2D
	POP  R26
	POP  R27
_0x4BF:
	CALL __PUTDP1
;     435 			if(rundata.reading[ch_to_search] > 0)
	CALL SUBOPT_0x12
	CALL SUBOPT_0x2E
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD02
	BRGE _0x53
;     436 			{
;     437                                 sprintf(strbuf,"%2d;%f;",ch_to_search+1,rundata.Rx);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x14
	CALL SUBOPT_0x30
	CALL __PUTPARD1
	CALL SUBOPT_0x16
;     438                                 prints(strbuf,strlen(strbuf),PORT_PC);
;     439 
;     440 			        rundata.temperature[ch_to_search] = RValueToTValue(rundata.reading[ch_to_search],i);
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x12
	CALL SUBOPT_0x15
	ST   -Y,R17
	RCALL _RValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     441                                 rundata.Rx = rundata.reading[ch_to_search];
	CALL SUBOPT_0x12
	CALL SUBOPT_0x2E
	__PUTD1MN _rundata,192
;     442                                 sprintf(strbuf,"%f;\r\n",rundata.temperature[ch_to_search]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x17
	CALL SUBOPT_0x18
;     443                                 prints(strbuf,strlen(strbuf),PORT_PC);
;     444 			}
;     445 		}else{
_0x53:
	RJMP _0x54
_0x50:
;     446 		        rundata.temperature[ch_to_search] = -9999;
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL SUBOPT_0x31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
;     447 		}
_0x54:
;     448 		phase = 5;
	LDI  R30,LOW(5)
	STS  _phase,R30
;     449 	}
;     450 	if(phase == 5) //got one reading
_0x4D:
	LDS  R26,_phase
	CPI  R26,LOW(0x5)
	BRNE _0x55
;     451 	{
;     452 		pgmain_handler(MSG_REFRESH);
	CALL SUBOPT_0x19
;     453 		phase = 0;
;     454 	}
;     455 	return 1;
_0x55:
	LDI  R30,LOW(1)
_0x4BA:
	LD   R17,Y+
	RET
;     456 }
;     457 LABEL flash statelbl = {LBL_HZ6X8,50,55,26,strbuf};
;     458 
;     459 void updatestate()
;     460 {
_updatestate:
;     461         char star[6];
;     462         sprintf(star,"    ");
	SBIW R28,6
;	star -> Y+0
	CALL SUBOPT_0x32
	__POINTW1FN _0,45
	CALL SUBOPT_0x33
;     463         if(phase == 0)                sprintf(star,"*   ");
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x56
	CALL SUBOPT_0x32
	__POINTW1FN _0,50
	CALL SUBOPT_0x33
;     464         if(phase == 1)                sprintf(star,"**  ");
_0x56:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x57
	CALL SUBOPT_0x32
	__POINTW1FN _0,55
	CALL SUBOPT_0x33
;     465         if(phase == 2)                sprintf(star,"*** ");
_0x57:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x58
	CALL SUBOPT_0x32
	__POINTW1FN _0,60
	CALL SUBOPT_0x33
;     466         if(phase == 3)                sprintf(star,"****");
_0x58:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x59
	CALL SUBOPT_0x32
	__POINTW1FN _0,65
	CALL SUBOPT_0x33
;     467         if(IS_BORE_MODE){
_0x59:
	CALL SUBOPT_0x34
	BRNE _0x5A
;     468                 sprintf(strbuf,"(%s:ch%2i,%d,%s)",rname2b(sysdata.rid[ch_to_search]),ch_to_search+1, dlg_cnt/ONESEC,star);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,70
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x1A
	ST   -Y,R30
	CALL _rname2b
	RJMP _0x4C0
;     469         }else{
_0x5A:
;     470                 sprintf(strbuf,"(%s:ch%2i,%d,%s)",tname2b(sysdata.tid[ch_to_search]),ch_to_search+1, dlg_cnt/ONESEC,star);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,70
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xD
	ST   -Y,R30
	CALL _tname2b
_0x4C0:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	MOVW R30,R12
	ADIW R30,1
	CALL SUBOPT_0x35
	CALL SUBOPT_0x36
	__GETD1N 0x186A0
	CALL __DIVD21U
	CALL __PUTPARD1
	MOVW R30,R28
	ADIW R30,16
	CALL SUBOPT_0x37
	LDI  R24,16
	CALL _sprintf
	ADIW R28,20
;     471         }
;     472 
;     473 /*
;     474         if(IS_BORE_MODE){
;     475                 if(IS_MODE_KTT)
;     476                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
;     477                 else
;     478                         sprintf(strbuf,"(ch%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
;     479         }else{
;     480                 sprintf(strbuf,"(ch:%2i,%2d,%s)",ch_to_search+1,dlg_cnt/ONESEC,star);
;     481         }
;     482 */
;     483         draw_label(&statelbl,SW_NORMAL);
	LDI  R30,LOW(_statelbl*2)
	LDI  R31,HIGH(_statelbl*2)
	CALL SUBOPT_0x38
;     484 }
	ADIW R28,6
	RET
;     485 static uchar tA = 0xff;

	.DSEG
_tA_G1:
	.BYTE 0x1
;     486 static uchar tB = 0xff;
_tB_G1:
	.BYTE 0x1
;     487 void testA(uchar data)
;     488 {

	.CSEG
_testA:
;     489         tA = data;
;	data -> Y+0
	LD   R30,Y
	STS  _tA_G1,R30
;     490 }
	ADIW R28,1
	RET
;     491 void testB(uchar data)
;     492 {
;     493         tB = data;
;	data -> Y+0
;     494 }
;     495 /**************************************************************************************/
;     496 //                              Main Function Loop
;     497 /**************************************************************************************/
;     498 extern double GetThmoVolt(double t,char type);
;     499 extern u8 databuf[12];
;     500 extern u8 pos_databuf; //position in data buffer
;     501 void main(void)
;     502 {
_main:
;     503 
;     504     u16 i;
;     505     u8 shortcut = KEY_INVALID;
;     506     // RS485 Node
;     507     init_var();	//init data structure
;	i -> R16,R17
;	shortcut -> R19
	LDI  R19,45
	CALL _init_var
;     508     // System Initialization
;     509     Init_Port();
	RCALL _Init_Port
;     510 //    Init_Timers();
;     511 //    Init_Ex_Interrupt();
;     512     Init_UART();
	RCALL _Init_UART
;     513     Enable_XMEM();
	RCALL _Enable_XMEM
;     514     Init_554();
	RCALL _Init_554
;     515     InitLED();
	CALL _InitLED
;     516     Key_Init();
	CALL _Key_Init
;     517     // Global enable interrupts
;     518     WDTCR = 0x00; //disable dog watch
	LDI  R30,LOW(0)
	OUT  0x21,R30
;     519     #asm("sei")
	sei
;     520     /*********************************************************************/
;     521     // System hardware dection
;     522     /*********************************************************************/
;     523     // intialize LED.
;     524     nextwin = 0;
	CLR  R4
;     525 
;     526     sleepms(20*ONEMS);
	LDI  R30,LOW(200000)
	LDI  R31,HIGH(200000)
	CALL SUBOPT_0x39
;     527     LCD_Init();
	CALL _LCD_Init
;     528     wnd_msgbox(&bootup);
	LDI  R30,LOW(_bootup*2)
	LDI  R31,HIGH(_bootup*2)
	CALL SUBOPT_0x3A
;     529     //init the DMM
;     530     nav_command(NAV_INIT);
	LDI  R30,LOW(1)
	CALL SUBOPT_0x0
;     531     sleepms(200*ONEMS);
;     532     navto1v();
	CALL _navto1v
;     533     nav_command(NAV_SLOWMODE);
	LDI  R30,LOW(7)
	CALL SUBOPT_0x0
;     534     sleepms(200*ONEMS);
;     535     nav_command(NAV_AFLTON);
	LDI  R30,LOW(6)
	CALL SUBOPT_0x0
;     536     sleepms(200*ONEMS);
;     537 
;     538     sleepms(2*ONEMS); //wait until all the node is ready after power up
	LDI  R30,LOW(20000)
	LDI  R31,HIGH(20000)
	CALL SUBOPT_0x39
;     539     State_Init();
	CALL _State_Init
;     540 
;     541     SET_BORE_MODE;
	CALL SUBOPT_0x3B
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3C
;     542 
;     543 	 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;     544 	 key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     545 
;     546 	 curr_ch = 1; //channel for display
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R6,R30
;     547 	 curr_dispch = 1;
	CALL SUBOPT_0x3D
;     548 	 while(1)
_0x5E:
;     549 	 {
;     550  	 	if(nextwin != 0)
	TST  R4
	BREQ _0x61
;     551 		{
;     552 			SwitchWindow(nextwin);
	ST   -Y,R4
	RCALL _SwitchWindow
;     553 			(*curr_window)(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	__CALL1MN _curr_window,0
;     554 			nextwin = 0;
	CLR  R4
;     555 		}
;     556 		if(key != KEY_INVALID)
_0x61:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE PC+3
	JMP _0x62
;     557 		{
;     558 			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
	LDI  R30,LOW(97)
	CP   R30,R5
	BREQ _0x64
	LDI  R30,LOW(98)
	CP   R30,R5
	BREQ _0x64
	LDI  R30,LOW(99)
	CP   R30,R5
	BREQ _0x64
	LDI  R30,LOW(100)
	CP   R30,R5
	BREQ _0x64
	RJMP _0x63
_0x64:
;     559 			{
;     560 			        shortcut = key;
	MOV  R19,R5
;     561                                 //processing shortcut key
;     562 				if(curr_window == pgmain_handler)
	CALL SUBOPT_0x3E
	BRNE _0x66
;     563 				{
;     564 					LCD_Cls();
	CALL _LCD_Cls
;     565 					wnd_msgbox(&modify);
	LDI  R30,LOW(_modify*2)
	LDI  R31,HIGH(_modify*2)
	CALL SUBOPT_0x3A
;     566 				}
;     567 				if(shortcut == KEY_BTN1) //mode switch
_0x66:
	CPI  R19,97
	BRNE _0x67
;     568 				{
;     569                			        SET_TOP1MA;
	CALL SUBOPT_0x20
;     570 	                	        SET_TOPT1000;
;     571 					if(IS_BORE_MODE){
	CALL SUBOPT_0x34
	BRNE _0x68
;     572 						SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL _navto120mv
	RJMP _0x4C1
;     573 					}else{
_0x68:
;     574 						SET_BORE_MODE;
	CALL SUBOPT_0x3B
	CALL SUBOPT_0x1E
	CALL _navto1v
_0x4C1:
	LDI  R30,LOW(105)
	CALL SUBOPT_0x40
;     575 					}
;     576 					dlg_cnt = 0;
	CALL SUBOPT_0x41
;     577 					onesec_cnt = 0;
;     578 					phase = 0;      //reset the state machine
;     579 				}
;     580 				if(shortcut == KEY_BTN2) //auto ktt or not
_0x67:
	CPI  R19,98
	BRNE _0x6A
;     581 				{
;     582 					if(IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x6B
;     583 					{
;     584                       			        SET_TOP1MA;
	CALL SUBOPT_0x20
;     585 	                        	        SET_TOPT1000;
;     586 						if((IS_MODE_KTT)){
	CALL SUBOPT_0x22
	BRNE _0x6C
;     587 							CLR_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(1)
	RJMP _0x4C2
;     588 							SET_PKTT;
;     589 						}else{
_0x6C:
;     590 							SET_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(0)
_0x4C2:
	ST   -Y,R30
	CALL _display_buttons
;     591 							SET_PKTT;
	CALL SUBOPT_0x1D
;     592 						}
;     593         					dlg_cnt = 0;
	CALL SUBOPT_0x41
;     594 	        				onesec_cnt = 0;
;     595 		        			phase = 0;      //reset the state machine
;     596 					}
;     597 				}
_0x6B:
;     598 				if(shortcut == KEY_BTN3) //thermal probe type
_0x6A:
	CPI  R19,99
	BREQ PC+3
	JMP _0x6E
;     599 				{
;     600 					display_buttons(KEY_BTN3,1);
	LDI  R30,LOW(99)
	CALL SUBOPT_0x40
;     601 					if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x6F
;     602 					{
;     603 					        i = sysdata.tid[curr_dispch-1];
	CALL SUBOPT_0x43
	MOV  R16,R30
	CLR  R17
;     604 					        if(i != INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x70
;     605 					        {
;     606                 					if((tprbdata.type[i] >= PRBTYPE_K) &&\
;     607 	                				   (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x44
	CPI  R30,LOW(0x3)
	BRLO _0x72
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xB)
	BRLO _0x73
_0x72:
	RJMP _0x71
_0x73:
;     608 		                			{
;     609 			                			if(tprbdata.type[i] == PRBTYPE_R)
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xA)
	BRNE _0x74
;     610 				                			tprbdata.type[i] = PRBTYPE_K;
	__POINTW1MN _tprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	LDI  R30,LOW(3)
	RJMP _0x4C3
;     611 					                	else
_0x74:
;     612 						                	tprbdata.type[i] +=1;
	CALL SUBOPT_0x44
	SUBI R30,-LOW(1)
_0x4C3:
	CALL __EEPROMWRB
;     613         					        }
;     614         					        if(rundata.reading[curr_dispch-1] > -9000)
_0x71:
	CALL SUBOPT_0x45
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BREQ PC+2
	BRCC PC+3
	JMP  _0x76
;     615                                                        		rundata.temperature[curr_dispch-1] = MValueToTValue(rundata.reading[curr_dispch-1], tprbdata.type[i]);
	CALL SUBOPT_0x47
	ADD  R30,R26
	ADC  R31,R27
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x45
	CALL SUBOPT_0x15
	CALL SUBOPT_0x44
	ST   -Y,R30
	RCALL _MValueToTValue
	POP  R26
	POP  R27
	CALL __PUTDP1
;     616         					}
_0x76:
;     617                                         }
_0x70:
;     618 					display_buttons(KEY_BTN3,0);
_0x6F:
	LDI  R30,LOW(99)
	CALL SUBOPT_0x25
;     619 
;     620 				}
;     621 				if(shortcut == KEY_BTN4) //remove zero
_0x6E:
	CPI  R19,100
	BRNE _0x77
;     622 				{
;     623 					display_buttons(KEY_BTN4,1);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x40
;     624 					if(IS_BORE_MODE){
	CALL SUBOPT_0x34
	BRNE _0x78
;     625 					        sysdata.R0 = rundata.Rx;
	CALL SUBOPT_0x30
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMWRD
;     626 					}else{
	RJMP _0x79
_0x78:
;     627 					        //sysdata.V0 = nav_read();
;     628 					        nav_command(NAV_ZEROON);
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _nav_command
;     629 					        sleepms(1000*ONEMS);
	LDI  R30,LOW(10000000)
	LDI  R31,HIGH(10000000)
	CALL SUBOPT_0x39
;     630 					}
_0x79:
;     631 					display_buttons(KEY_BTN4,0);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x25
;     632 				}
;     633 				if(curr_window == pgmain_handler)       //redraw the running window
_0x77:
	CALL SUBOPT_0x3E
	BRNE _0x7A
;     634 				{
;     635 					pgmain_handler(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _pgmain_handler
;     636 				}
;     637 			        shortcut = KEY_INVALID;
_0x7A:
	LDI  R19,LOW(45)
;     638 
;     639 			}else{
	RJMP _0x7B
_0x63:
;     640 				(*curr_window)(key);
	ST   -Y,R5
	__CALL1MN _curr_window,0
;     641 			}
_0x7B:
;     642 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     643 	  	}else{
	RJMP _0x7C
_0x62:
;     644 			if(curr_window != pgmain_handler)
	CALL SUBOPT_0x3E
	BREQ _0x7D
;     645 				continue;
	RJMP _0x5E
;     646 			if(dlg_cnt > 1)
_0x7D:
	CALL SUBOPT_0x36
	__CPD2N 0x2
	BRSH PC+3
	JMP _0x7E
;     647 			{
;     648 			        onesec_cnt++;
	LDS  R30,_onesec_cnt
	LDS  R31,_onesec_cnt+1
	LDS  R22,_onesec_cnt+2
	LDS  R23,_onesec_cnt+3
	CALL SUBOPT_0x48
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R31
	STS  _onesec_cnt+2,R22
	STS  _onesec_cnt+3,R23
;     649 			        if(onesec_cnt == (ONESEC-10))
	CALL SUBOPT_0x49
	__CPD2N 0x18696
	BRNE _0x7F
;     650 			        {
;     651         			        updatestate();
	CALL _updatestate
;     652         			}
;     653         			if(onesec_cnt == ONESEC)
_0x7F:
	CALL SUBOPT_0x49
	__CPD2N 0x186A0
	BRNE _0x80
;     654         			        onesec_cnt = 0 ;
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
;     655 				dlg_cnt--;
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
;     656 				continue;
	RJMP _0x5E
;     657 			}
;     658 			updatestate();
_0x7E:
	CALL _updatestate
;     659 			if((IS_THERM_MODE))
	CALL SUBOPT_0x42
	BRNE _0x81
;     660 			{
;     661 				if(therm_state() == 0)
	CALL _therm_state
	CPI  R30,0
	BRNE _0x82
;     662 				        continue;
	RJMP _0x5E
;     663 			}else{
_0x82:
	RJMP _0x83
_0x81:
;     664 				if(bore_state() == 0)
	CALL _bore_state
	CPI  R30,0
	BRNE _0x84
;     665 				        continue;
	RJMP _0x5E
;     666 			}
_0x84:
_0x83:
;     667 
;     668                         //shift to next channel
;     669 
;     670                         while(true)
_0x85:
;     671                         {
;     672                                 ch_to_search += 1;
	MOVW R30,R12
	ADIW R30,1
	MOVW R12,R30
;     673         			if(ch_to_search >= MAX_CH_NUM)
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R12,R30
	CPC  R13,R31
	BRLT _0x88
;     674         			{
;     675 	        			ch_to_search = 0;
	CLR  R12
	CLR  R13
;     676 	        			break;
	RJMP _0x87
;     677 	        		}
;     678 	        		if(IS_THERM_MODE)
_0x88:
	CALL SUBOPT_0x42
	BRNE _0x89
;     679 	        		{
;     680 	        		        i = sysdata.tid[ch_to_search];
	__POINTW1MN _sysdata,14
	RJMP _0x4C4
;     681 	        		}else{
_0x89:
;     682 	        		        i = sysdata.rid[ch_to_search];
	__POINTW1MN _sysdata,38
_0x4C4:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R16,R30
	CLR  R17
;     683 	        		}
;     684 	        		if(i == INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x85
;     685 	        		        continue;
;     686 	        		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x8C
;     687 	        		{
;     688        					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x44
	CPI  R30,LOW(0x3)
	BRLO _0x8E
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xB)
	BRLO _0x8F
_0x8E:
	RJMP _0x8D
_0x8F:
;     689        					        break;
	RJMP _0x87
;     690 	        		}else{
_0x8D:
	RJMP _0x90
_0x8C:
;     691        		                        if((rprbdata.type[i] <= PRBTYPE_MAX) && (rprbdata.type[i] >= PRBTYPE_MIN))
	CALL SUBOPT_0x4A
	CPI  R30,LOW(0xF4)
	BRSH _0x92
	CALL SUBOPT_0x4A
	CPI  R30,LOW(0xF1)
	BRSH _0x93
_0x92:
	RJMP _0x91
_0x93:
;     692        		        		        break;
	RJMP _0x87
;     693 	        		}
_0x91:
_0x90:
;     694 	                }
	RJMP _0x85
_0x87:
;     695 		}
_0x7C:
;     696 	}
	RJMP _0x5E
;     697 }
_0x94:
	RJMP _0x94
;     698 
;     699 WINDOW flash wins[]={
;     700 	{PG_MAIN,	pgmain_handler},	//running menu
;     701 	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
;     702 	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
;     703 	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
;     704 	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
;     705 	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
;     706 	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
;     707 	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
;     708 	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
;     709 };
;     710 
;     711 
;     712 void SwitchWindow(uchar page)
;     713 {
_SwitchWindow:
;     714 	u8 i;
;     715 	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	ST   -Y,R17
;	page -> Y+1
;	i -> R17
	LDI  R17,LOW(0)
_0x96:
	CPI  R17,9
	BRSH _0x97
;     716 	{
;     717 		if(wins[i].page_id == page)
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
;     718 		{
;     719 			curr_window = wins[i].handler;
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_wins,1
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	CALL SUBOPT_0x4B
	STS  _curr_window,R30
	STS  _curr_window+1,R31
;     720 			return;
	LDD  R17,Y+0
	RJMP _0x4B9
;     721 		}
;     722 	}
_0x98:
	SUBI R17,-1
	RJMP _0x96
_0x97:
;     723 }
	LDD  R17,Y+0
_0x4B9:
	ADIW R28,2
	RET
;     724 /*********************************************************************************/
;     725 //                         UART0 and UART1 Subroutines
;     726 /*********************************************************************************/
;     727 #include <mega64.h>
;     728 #include "uart.h"
;     729 #include "16c554.h"
;     730 #include "global.h"
;     731 
;     732 //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
;     733 //extern u8 x_modem_databuf[];                       /* store data sent from PC */
;     734 //extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
;     735 //extern u8 x_modem_file_being_transferred;
;     736 //extern u8 x_modem_file_transfer_complete;
;     737 
;     738 /*********************************************************************************/
;     739 //                     UART0  Variables & Buffer Definition
;     740 /*********************************************************************************/
;     741 u8 volatile tx_buffer0[TX_BUFFER_SIZE0];

	.DSEG
_tx_buffer0:
	.BYTE 0x80
;     742 u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
;     743 
;     744 /*********************************************************************************/
;     745 //                     UART1  Variables & Buffer Definition
;     746 /*********************************************************************************/
;     747 u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
_tx_buffer1:
	.BYTE 0x80
;     748 u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;
_tx_wr_index1:
	.BYTE 0x1
_tx_rd_index1:
	.BYTE 0x1
_tx_counter1:
	.BYTE 0x1
;     749 
;     750 /*
;     751  * 0-9,a-f,A-F char to hex conversion
;     752  * otherwise 0xff will be returned.
;     753  */
;     754 u8 ishexchar(u8 dat){

	.CSEG
;     755         if( (dat >= '0') && (dat <= '9'))
;	dat -> Y+0
;     756                 return dat-'0';
;     757         if( (dat >= 'a') && (dat <= 'f'))
;     758                 return dat-'a'+0x0a;
;     759         if( (dat >= 'A') && (dat <= 'F'))
;     760                 return dat-'A'+0x0a;
;     761         return 0xff;
;     762 }
;     763 
;     764 
;     765 
;     766 interrupt [USART0_RXC] void usart0_rx_isr(void)
;     767 {
_usart0_rx_isr:
	CALL SUBOPT_0x4C
;     768    u8 status,data;
;     769    //read status and data
;     770    status=UCSR0A;
;	status -> R17
;	data -> R16
	IN   R17,11
;     771    data=UDR0;
	IN   R16,12
;     772    cm0_push(data);
	ST   -Y,R16
	RCALL _kbd_uart_push
;     773 }
	CALL SUBOPT_0x4D
	RETI
;     774 
;     775 /*********************************************************************************/
;     776 //                 USART0 Transmitter interrupt service routine
;     777 /*********************************************************************************/
;     778 interrupt [USART0_TXC] void usart0_tx_isr(void)
;     779 {
_usart0_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     780 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0xA2
;     781    {
;     782    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
;     783    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	OUT  0xC,R30
;     784    tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_rd_index0
	ANDI R30,0x7F
	STS  _tx_rd_index0,R30
;     785    };
_0xA2:
;     786 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     787 
;     788 /*********************************************************************************/
;     789 //                      UART1 Receiver interrupt service routine
;     790 /*********************************************************************************/
;     791 interrupt [USART1_RXC] void usart1_rx_isr(void)
;     792 {
_usart1_rx_isr:
	CALL SUBOPT_0x4C
;     793    u8 status,data;
;     794 
;     795    //read status and data
;     796    status=UCSR1A;
;	status -> R17
;	data -> R16
	LDS  R17,155
;     797    data=UDR1;
	LDS  R16,156
;     798    cm1_push(data);
	ST   -Y,R16
	CALL _nav_uart_push
;     799 }
	CALL SUBOPT_0x4D
	RETI
;     800 
;     801 /*********************************************************************************/
;     802 //                   USART1 Transmitter interrupt service routine
;     803 /*********************************************************************************/
;     804 interrupt [USART1_TXC] void usart1_tx_isr(void)
;     805 {
_usart1_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     806 if (tx_counter1)
	LDS  R30,_tx_counter1
	CPI  R30,0
	BREQ _0xA3
;     807    {
;     808    --tx_counter1;
	SUBI R30,LOW(1)
	STS  _tx_counter1,R30
;     809    UDR1=tx_buffer1[tx_rd_index1++];
	LDS  R30,_tx_rd_index1
	SUBI R30,-LOW(1)
	STS  _tx_rd_index1,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R30,Z
	STS  156,R30
;     810    tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_rd_index1
	ANDI R30,0x7F
	STS  _tx_rd_index1,R30
;     811    };
_0xA3:
;     812 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     813 
;     814 void com0_putc(char c)
;     815 {
_com0_putc:
;     816 if(DEBUG == 1)
;	c -> Y+0
;     817         return;
;     818 while (tx_counter0 == TX_BUFFER_SIZE0);
_0xA5:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x80)
	BREQ _0xA5
;     819 #asm("cli")
	cli
;     820 if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0xA9
	SBIC 0xB,5
	RJMP _0xA8
_0xA9:
;     821    {
;     822    tx_buffer0[tx_wr_index0++]=c;
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
;     823    tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_wr_index0
	ANDI R30,0x7F
	STS  _tx_wr_index0,R30
;     824    ++tx_counter0;
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
;     825    }
;     826 else
	RJMP _0xAB
_0xA8:
;     827    UDR0=c;
	LD   R30,Y
	OUT  0xC,R30
;     828 #asm("sei")
_0xAB:
	sei
;     829 }
	ADIW R28,1
	RET
;     830 
;     831 
;     832 void com1_putc(char c)
;     833 {
_com1_putc:
;     834 while (tx_counter1 == TX_BUFFER_SIZE1);
;	c -> Y+0
_0xAC:
	LDS  R26,_tx_counter1
	CPI  R26,LOW(0x80)
	BREQ _0xAC
;     835 #asm("cli")
	cli
;     836 if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter1
	CPI  R30,0
	BRNE _0xB0
	LDS  R30,155
	ANDI R30,LOW(0x20)
	BRNE _0xAF
_0xB0:
;     837    {
;     838    tx_buffer1[tx_wr_index1++]=c;
	LDS  R30,_tx_wr_index1
	SUBI R30,-LOW(1)
	STS  _tx_wr_index1,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R26,Y
	STD  Z+0,R26
;     839    tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_wr_index1
	ANDI R30,0x7F
	STS  _tx_wr_index1,R30
;     840    ++tx_counter1;
	LDS  R30,_tx_counter1
	SUBI R30,-LOW(1)
	STS  _tx_counter1,R30
;     841    }
;     842 else
	RJMP _0xB2
_0xAF:
;     843    UDR1=c;
	LD   R30,Y
	STS  156,R30
;     844 #asm("sei")
_0xB2:
	sei
;     845 }
	ADIW R28,1
	RET
;     846 
;     847 /*
;     848 void com0_puthex(u8 a)
;     849 {
;     850 	unsigned char h,l;
;     851 	h = (a & 0xf0) >> 4;
;     852 	l = (a & 0x0f);
;     853 	if(h <= 9)
;     854 		com0_putc(h+'0');
;     855 	else
;     856 		com0_putc(h+'A'-0x0a);
;     857 	if(l <= 9)
;     858 		com0_putc(l+'0');
;     859 	else
;     860 		com0_putc(l+'A'-0x0a);
;     861 }
;     862 */
;     863 
;     864 void com0_putstr(u8 *ptr){
;     865         while(*ptr != 0x00){
;	*ptr -> Y+0
;     866                 com0_putc(*ptr++);
;     867         }
;     868 }
;     869 
;     870 void com1_puthex(u8 a)
;     871 {
;     872 	unsigned char h,l;
;     873 	h = (a & 0xf0) >> 4;
;	a -> Y+2
;	h -> R17
;	l -> R16
;     874 	l = (a & 0x0f);
;     875 	if(h <= 9)
;     876 		com1_putc(h+'0');
;     877 	else
;     878 		com1_putc(h+'A'-0x0a);
;     879 	if(l <= 9)
;     880 		com1_putc(l+'0');
;     881 	else
;     882 		com1_putc(l+'A'-0x0a);
;     883 }
;     884 void com1_putstr(u8 *ptr){
;     885         while(*ptr != 0x00){
;	*ptr -> Y+0
;     886                 com1_putc(*ptr++);
;     887         }
;     888 }
;     889 /*
;     890 void com0_mputs(u8 *buf, u8 size){
;     891         while(size-- > 0){
;     892                 com0_puthex(*buf++);
;     893         }
;     894 }
;     895 */
;     896 void com1_mputs(u8 *buf, u8 size){
;     897         while(size-- > 0){
;	*buf -> Y+1
;	size -> Y+0
;     898                 com1_puthex(*buf++);
;     899         }
;     900 }
;     901 
;     902 /*********************************************************************************/
;     903 //                              Intialize UART
;     904 /*********************************************************************************/
;     905 void Init_UART()
;     906 {
_Init_UART:
;     907    // USART0 initialization
;     908    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     909    // USART0 Receiver: On
;     910    // USART0 Transmitter: On
;     911    // USART0 Mode: Asynchronous
;     912    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     913       UCSR0A=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;     914       UCSR0B=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     915       UCSR0C=0x06;
	LDI  R30,LOW(6)
	STS  149,R30
;     916       UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  144,R30
;     917       UBRR0L=0x0F;
	LDI  R30,LOW(15)
	OUT  0x9,R30
;     918 
;     919    // USART1 initialization
;     920    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     921    // USART1 Receiver: On
;     922    // USART1 Transmitter: On
;     923    // USART1 Mode: Asynchronous
;     924    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     925       UCSR1A=0x00;
	LDI  R30,LOW(0)
	STS  155,R30
;     926       UCSR1B=0xD8;
	LDI  R30,LOW(216)
	STS  154,R30
;     927       UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
;     928       UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
;     929       UBRR1L=0x0F;
	LDI  R30,LOW(15)
	STS  153,R30
;     930 
;     931    // Initialize buffer variables
;     932       tx_wr_index0 = 0;
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
;     933       tx_rd_index0 = 0;
	STS  _tx_rd_index0,R30
;     934       tx_counter0  = 0;
	STS  _tx_counter0,R30
;     935 
;     936       tx_wr_index1 = 0;
	STS  _tx_wr_index1,R30
;     937       tx_rd_index1 = 0;
	STS  _tx_rd_index1,R30
;     938       tx_counter1  = 0;
	STS  _tx_counter1,R30
;     939 }
	RET
;     940 /********************************************************************************/
;     941 //                             TL16C554 Driver
;     942 // Controller:  MEGA64
;     943 // 16C554 clk:  7.3728MHZ
;     944 //
;     945 // Schematic Description:
;     946 // 16C554 <---> MEGA64
;     947 // INTA   <---> INT4
;     948 // INTB   <---> INT5
;     949 // INTC   <---> INT6
;     950 // INTD   <---> INT7
;     951 // CSA#   <---> A15
;     952 // CSB#   <---> A14
;     953 // CSC#   <---> A13
;     954 // CSD#   <---> A12
;     955 //   A0   <---> A8
;     956 //   A1   <---> A9
;     957 //   A2   <---> A10
;     958 // RESET  <---> PB0
;     959 // INTN#  <---> PE2
;     960 
;     961 // Coded by: fd_lxh@yahoo.com.cn
;     962 /********************************************************************************/
;     963 #include "mega64.h"
;     964 #include "global.h"
;     965 #include "16c554.h"
;     966 #include "uart.h"
;     967 #include "utili.h"
;     968 #define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
;     969 #define IER_DATA 0b00000001
;     970 // LCR.7 must be cleared to 0 when accessing RBR/THR/IER
;     971 // LCR.7 must be set to 1 when accessing divisor latch
;     972 #define LCR_DATA 0b00011011    // Even Parity 8 data bit, 1 stop bit
;     973 #define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
;     974 #define DLM_DATA 0x0
;     975 #define MCR_DATA 0b00001000
;     976 
;     977 #define min(A,B)        ((A<B)?(A):(B))
;     978 /********************************************************************************/
;     979 //               TLC16C554 UART PORT A Interrupt Subroutine
;     980 /********************************************************************************/
;     981 interrupt [EXT_INT4] void ext_int4_isr(void)
;     982 {
_ext_int4_isr:
	CALL SUBOPT_0x4F
;     983    u8 rundata;
;     984    switch(UC_554A_IIR & 0xF)
;	rundata -> R17
	LDS  R30,29184
	ANDI R30,LOW(0xF)
;     985    {
;     986       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC3
;     987           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     988           break;
	RJMP _0xC2
;     989       case 0x4:                         // Receiver data available or trigger level reached
_0xC3:
	CPI  R30,LOW(0x4)
	BRNE _0xC5
;     990           rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,28672
;     991           cm_pushA(rundata);
	ST   -Y,R17
	RCALL _pc_uart_push
;     992           return;
	CALL SUBOPT_0x50
	RETI
;     993       default:
_0xC5:
;     994           break;
;     995    }
_0xC2:
;     996 
;     997    rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,29952
;     998 }
	CALL SUBOPT_0x50
	RETI
;     999 /********************************************************************************/
;    1000 //               TLC16C554 UART PORT B Interrupt Subroutine
;    1001 /********************************************************************************/
;    1002 // External Interrupt 5 service routine
;    1003 interrupt [EXT_INT5] void ext_int5_isr(void)
;    1004 {
_ext_int5_isr:
	CALL SUBOPT_0x4F
;    1005    u8 rundata;
;    1006    switch(UC_554B_IIR & 0xF)
;	rundata -> R17
	LDS  R30,45568
	ANDI R30,LOW(0xF)
;    1007    {
;    1008       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC9
;    1009           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1010           break;
	RJMP _0xC8
;    1011       case 0x4:                         // Receiver data available or trigger level reached
_0xC9:
	CPI  R30,LOW(0x4)
	BRNE _0xCB
;    1012           rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,45056
;    1013           cm_pushB(rundata);
	ST   -Y,R17
	RCALL _scanner_uart_push
;    1014           return;
	CALL SUBOPT_0x50
	RETI
;    1015       default:
_0xCB:
;    1016           break;
;    1017    }
_0xC8:
;    1018    rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,46336
;    1019 }
	CALL SUBOPT_0x50
	RETI
;    1020 /********************************************************************************/
;    1021 //               TLC16C554 UART PORT C Interrupt Subroutine
;    1022 /********************************************************************************/
;    1023 // External Interrupt 6 service routine
;    1024 interrupt [EXT_INT6] void ext_int6_isr(void)
;    1025 {
_ext_int6_isr:
	CALL SUBOPT_0x4F
;    1026    u8 rundata;
;    1027    switch(UC_554C_IIR & 0xF)
;	rundata -> R17
	LDS  R30,53760
	ANDI R30,LOW(0xF)
;    1028    {
;    1029       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xCF
;    1030           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1031           break;
	RJMP _0xCE
;    1032       case 0x4:                         // Receiver data available or trigger level reached
_0xCF:
	CPI  R30,LOW(0x4)
	BRNE _0xD1
;    1033           rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,53248
;    1034           cm_pushC(rundata);
	ST   -Y,R17
	CALL _testA
;    1035           return;
	CALL SUBOPT_0x50
	RETI
;    1036       default:
_0xD1:
;    1037           break;
;    1038    }
_0xCE:
;    1039 
;    1040    rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,54528
;    1041 }
	CALL SUBOPT_0x50
	RETI
;    1042 
;    1043 /********************************************************************************/
;    1044 //               TLC16C554 UART PORT D Interrupt Subroutine
;    1045 /********************************************************************************/
;    1046 // External Interrupt 7 service routine
;    1047 interrupt [EXT_INT7] void ext_int7_isr(void)
;    1048 {
_ext_int7_isr:
	CALL SUBOPT_0x4F
;    1049    u8 rundata;
;    1050    switch(UC_554D_IIR & 0xF)
;	rundata -> R17
	LDS  R30,57856
	ANDI R30,LOW(0xF)
;    1051    {
;    1052       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xD5
;    1053           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1054           break;
	RJMP _0xD4
;    1055       case 0x4:                         // Receiver data available or trigger level reached
_0xD5:
	CPI  R30,LOW(0x4)
	BRNE _0xD7
;    1056           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1057           cm_pushD(rundata);
	ST   -Y,R17
	CALL _testA
;    1058           return;
	CALL SUBOPT_0x50
	RETI
;    1059       default:
_0xD7:
;    1060           break;
;    1061    }
_0xD4:
;    1062    rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,58624
;    1063 }
	CALL SUBOPT_0x50
	RETI
;    1064 
;    1065 /********************************************************************************/
;    1066 //                             Reset 16C554
;    1067 /********************************************************************************/
;    1068 void Reset_554(void)
;    1069 {
_Reset_554:
;    1070    // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
;    1071       PORTE.3 = 1;
	SBI  0x3,3
;    1072       sleepms(5*ONEMS);    // Delay
	LDI  R30,LOW(50000)
	LDI  R31,HIGH(50000)
	CALL SUBOPT_0x39
;    1073       PORTE.3 = 0;
	CBI  0x3,3
;    1074       sleepms(5*ONEMS);    // Delay
	LDI  R30,LOW(50000)
	LDI  R31,HIGH(50000)
	CALL SUBOPT_0x39
;    1075 }
	RET
;    1076 /********************************************************************************/
;    1077 //                             Initialize 16C554
;    1078 /********************************************************************************/
;    1079 void Init_554(void)
;    1080 {
_Init_554:
;    1081       Reset_554();  // Reset 554
	CALL _Reset_554
;    1082 
;    1083    /********************************************************************/
;    1084    //                      Initialize UART A
;    1085    /********************************************************************/
;    1086    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1087       UC_554A_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  29440,R30
;    1088       UC_554A_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  28672,R30
;    1089       UC_554A_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  28928,R30
;    1090 
;    1091    // Set Line Control Register:
;    1092    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1093       UC_554A_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  29440,R30
;    1094 
;    1095    // Set FIFO Control Register:
;    1096    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1097       UC_554A_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  29184,R30
;    1098 
;    1099    // Set Modem Control Register:
;    1100    // Enable enternal interrupt
;    1101       UC_554A_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  29696,R30
;    1102 
;    1103    // Set Interrupt Enable Register:
;    1104    // Enable Received data available / timeout / transmitter holding register empty &
;    1105    // Receiver line status interrupt
;    1106       UC_554A_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  28928,R30
;    1107 
;    1108    /********************************************************************/
;    1109    //                      Initialize UART B
;    1110    /********************************************************************/
;    1111    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1112    // LCR.7 must be set to "1" before setting baud rate
;    1113       UC_554B_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  45824,R30
;    1114       UC_554B_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  45056,R30
;    1115       UC_554B_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  45312,R30
;    1116 
;    1117    // Set Line Control Register:
;    1118    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1119       UC_554B_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  45824,R30
;    1120 
;    1121    // Set FIFO Control Register:
;    1122    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1123       UC_554B_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  45568,R30
;    1124 
;    1125    // Set Modem Control Register:
;    1126    // Enable enternal interrupt
;    1127       UC_554B_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  46080,R30
;    1128 
;    1129    // Set Interrupt Enable Register:
;    1130    // Enable Received data available / timeout / transmitter holding register empty &
;    1131    // Receiver line status interrupt
;    1132       UC_554B_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  45312,R30
;    1133 
;    1134    /********************************************************************/
;    1135    //                      Initialize UART C
;    1136    /********************************************************************/
;    1137    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1138       UC_554C_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  54016,R30
;    1139       UC_554C_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  53248,R30
;    1140       UC_554C_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  53504,R30
;    1141 
;    1142    // Set Line Control Register:
;    1143    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1144       UC_554C_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  54016,R30
;    1145 
;    1146    // Set FIFO Control Register:
;    1147    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1148       UC_554C_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  53760,R30
;    1149 
;    1150    // Set Modem Control Register:
;    1151    // Enable enternal interrupt
;    1152       UC_554C_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  54272,R30
;    1153 
;    1154    // Set Interrupt Enable Register:
;    1155    // Enable Received data available / timeout / transmitter holding register empty &
;    1156    // Receiver line status interrupt
;    1157       UC_554C_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  53504,R30
;    1158 
;    1159    /********************************************************************/
;    1160    //                      Initialize UART D
;    1161    /********************************************************************/
;    1162    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1163       UC_554D_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  58112,R30
;    1164       UC_554D_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  57344,R30
;    1165       UC_554D_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  57600,R30
;    1166 
;    1167    // Set Line Control Register:
;    1168    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1169       UC_554D_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  58112,R30
;    1170 
;    1171    // Set FIFO Control Register:
;    1172    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1173       UC_554D_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  57856,R30
;    1174 
;    1175    // Set Modem Control Register:
;    1176    // Enable enternal interrupt
;    1177       UC_554D_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  58368,R30
;    1178 
;    1179    // Set Interrupt Enable Register:
;    1180    // Enable Received data available / timeout / transmitter holding register empty &
;    1181    // Receiver line status interrupt
;    1182       UC_554D_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  57600,R30
;    1183 }
	RET
;    1184 
;    1185 
;    1186 /********************************************************************************/
;    1187 //                 Send multi-bytes (less than 16 bytes) via UART
;    1188 // Function:
;    1189 // Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
;    1190 // format:
;    1191 // print_554(u8 *str, char uart_port)
;    1192 // arg1: string to be sent. arg2: port (A B,C,D)
;    1193 /********************************************************************************/
;    1194 void prints(u8 *str, u8 length, char uart_port)
;    1195 {
_prints:
;    1196     u8 len, i;
;    1197     //len = length & 0x0F;        //15bytes at most
;    1198     len = length;
	ST   -Y,R17
	ST   -Y,R16
;	*str -> Y+4
;	length -> Y+3
;	uart_port -> Y+2
;	len -> R17
;	i -> R16
	LDD  R17,Y+3
;    1199     switch(uart_port)
	LDD  R30,Y+2
;    1200     {
;    1201        case SPORTA:
	CPI  R30,0
	BRNE _0xDB
;    1202            // wait if data still being transmitted in UART
;    1203            // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
;    1204            // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
;    1205            // check if FIFO is empty by LSR5.
;    1206            // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
;    1207            // UC_554A_LSR is not a register name,
;    1208            // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
;    1209            // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
;    1210            // here in 16c554.h, a bracket is added
;    1211            while(len)
_0xDC:
	CPI  R17,0
	BREQ _0xDE
;    1212            {
;    1213               while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xDF:
	LDS  R30,29952
	ANDI R30,LOW(0x20)
	BREQ _0xDF
;    1214               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1215                   UC_554A_THR = *str++;
	CALL SUBOPT_0x51
	STS  28672,R30
;    1216               len = len - min(len,16);
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
;    1217            }
	RJMP _0xDC
_0xDE:
;    1218            break;
	RJMP _0xDA
;    1219        case SPORTB:
_0xDB:
	CPI  R30,LOW(0x1)
	BRNE _0xEB
;    1220            while(len)
_0xEC:
	CPI  R17,0
	BREQ _0xEE
;    1221            {
;    1222               while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xEF:
	LDS  R30,46336
	ANDI R30,LOW(0x20)
	BREQ _0xEF
;    1223               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1224                   UC_554B_THR = *str++;
	CALL SUBOPT_0x51
	STS  45056,R30
;    1225               len = len - min(len,16);
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
;    1226            }
	RJMP _0xEC
_0xEE:
;    1227            break;
	RJMP _0xDA
;    1228        case SPORTC:
_0xEB:
	CPI  R30,LOW(0x2)
	BRNE _0xFB
;    1229            while(len)
_0xFC:
	CPI  R17,0
	BREQ _0xFE
;    1230            {
;    1231               while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xFF:
	LDS  R30,54528
	ANDI R30,LOW(0x20)
	BREQ _0xFF
;    1232               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1233                   UC_554C_THR = *str++;
	CALL SUBOPT_0x51
	STS  53248,R30
;    1234               len = len - min(len,16);
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
;    1235            }
	RJMP _0xFC
_0xFE:
;    1236 
;    1237            break;
	RJMP _0xDA
;    1238        case SPORTD:
_0xFB:
	CPI  R30,LOW(0x3)
	BRNE _0x10B
;    1239            while(len)
_0x10C:
	CPI  R17,0
	BREQ _0x10E
;    1240            {
;    1241               while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
_0x10F:
	LDS  R30,58624
	ANDI R30,LOW(0x20)
	BREQ _0x10F
;    1242               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1243                   UC_554D_THR = *str++;
	CALL SUBOPT_0x51
	STS  57344,R30
;    1244               len = len - min(len,16);
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
;    1245            }
	RJMP _0x10C
_0x10E:
;    1246            break;
	RJMP _0xDA
;    1247        case SPORT0:
_0x10B:
	CPI  R30,LOW(0x4)
	BRNE _0x11B
;    1248            while(len-- > 0){
_0x11C:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x11E
;    1249                 com0_putc(*str++);
	CALL SUBOPT_0x51
	ST   -Y,R30
	CALL _com0_putc
;    1250            }
	RJMP _0x11C
_0x11E:
;    1251            break;
	RJMP _0xDA
;    1252        case SPORT1:
_0x11B:
	CPI  R30,LOW(0x5)
	BRNE _0x123
;    1253            while(len-- > 0){
_0x120:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x122
;    1254                 com1_putc(*str++);
	CALL SUBOPT_0x51
	ST   -Y,R30
	CALL _com1_putc
;    1255            }
	RJMP _0x120
_0x122:
;    1256            break;
	RJMP _0xDA
;    1257        default:
_0x123:
;    1258            PORTC = 0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1259            break;
;    1260     }
_0xDA:
;    1261 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    1262 
;    1263 
;    1264 // System Initialization file
;    1265 
;    1266 #include <mega64.h>
;    1267 #include "global.h"
;    1268 /**************************************************************************************/
;    1269 //                              Initialize Port A-G
;    1270 /**************************************************************************************/
;    1271 void Init_Port()
;    1272 {
_Init_Port:
;    1273  // Input/Output Ports initialization
;    1274  // Port A initialization
;    1275  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;    1276  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1277     PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;    1278     DDRA=0x00;
	OUT  0x1A,R30
;    1279 
;    1280  // Port B initialization
;    1281  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
;    1282  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
;    1283     PORTB=0xFF;
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    1284     DDRB=0xFF;
	OUT  0x17,R30
;    1285 
;    1286  // Port C initialization
;    1287  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
;    1288  // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
;    1289  // PORTC[7:4]: chip select of 16C554 channels.
;    1290     PORTC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1291     DDRC=0xFF;
	LDI  R30,LOW(255)
	OUT  0x14,R30
;    1292 
;    1293  // Port D initialization
;    1294  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
;    1295  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1296     PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1297     DDRD=0x00;
	OUT  0x11,R30
;    1298 
;    1299  // Port E initialization
;    1300  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
;    1301  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
;    1302  // PORTE.0: Input (M_RXD0)
;    1303  // PORTE.1: output (M_TXD0)
;    1304  // PORTE.2: output (16C554 INTC#)
;    1305  // PORTE.3: output (16C554 reset)
;    1306     PORTE=0x06;
	LDI  R30,LOW(6)
	OUT  0x3,R30
;    1307     DDRE=0x0E;
	LDI  R30,LOW(14)
	OUT  0x2,R30
;    1308 
;    1309  // Port F initialization
;    1310  // Input
;    1311  // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
;    1312  // PORTF[3:0]: LED[4:1]
;    1313     PORTF=0x0F;
	LDI  R30,LOW(15)
	STS  98,R30
;    1314     DDRF=0x0F;
	STS  97,R30
;    1315 
;    1316  // Port G initialization
;    1317  // Input
;    1318     PORTG=0x00;
	LDI  R30,LOW(0)
	STS  101,R30
;    1319     DDRG=0x00;
	STS  100,R30
;    1320 }
	RET
;    1321 
;    1322 /**************************************************************************************/
;    1323 //                              Initialize Timers
;    1324 /**************************************************************************************/
;    1325 void Init_Timers()
;    1326 {
;    1327  // Timer/Counter 0 initialization
;    1328  // Clock source: System Clock
;    1329  // Clock value: 14.400 kHz
;    1330  // Mode: Normal top=FFh
;    1331  // OC0 output: Disconnected
;    1332     ASSR=0x00;
;    1333     TCCR0=0x07;
;    1334     TCNT0=0x00;
;    1335     OCR0=0x00;
;    1336 
;    1337  // Timer/Counter 1 initialization
;    1338  // Clock source: System Clock
;    1339  // Clock value: 14.400 kHz
;    1340  // Mode: Normal top=FFFFh
;    1341  // OC1A output: Discon.
;    1342  // OC1B output: Discon.
;    1343  // OC1C output: Discon.
;    1344  // Noise Canceler: Off
;    1345  // Input Capture on Falling Edge
;    1346  // Timer 1 Overflow Interrupt: On
;    1347  // Input Capture Interrupt: Off
;    1348  // Compare A Match Interrupt: Off
;    1349  // Compare B Match Interrupt: Off
;    1350  // Compare C Match Interrupt: Off
;    1351     TCCR1A=0x00;
;    1352     TCCR1B=0x05;
;    1353     TCNT1H=0x00;
;    1354     TCNT1L=0x00;
;    1355     ICR1H=0x00;
;    1356     ICR1L=0x00;
;    1357     OCR1AH=0x00;
;    1358     OCR1AL=0x00;
;    1359     OCR1BH=0x00;
;    1360     OCR1BL=0x00;
;    1361     OCR1CH=0x00;
;    1362     OCR1CL=0x00;
;    1363 
;    1364  // Timer/Counter 2 initialization
;    1365  // Clock source: System Clock
;    1366  // Clock value: Timer 2 Stopped
;    1367  // Mode: Normal top=FFh
;    1368  // OC2 output: Disconnected
;    1369     TCCR2=0x00;
;    1370     TCNT2=0x00;
;    1371     OCR2=0x00;
;    1372 
;    1373  // Timer/Counter 3 initialization
;    1374  // Clock source: System Clock
;    1375  // Clock value: Timer 3 Stopped
;    1376  // Mode: Normal top=FFFFh
;    1377  // Noise Canceler: Off
;    1378  // Input Capture on Falling Edge
;    1379  // OC3A output: Discon.
;    1380  // OC3B output: Discon.
;    1381  // OC3C output: Discon.
;    1382  // Timer 3 Overflow Interrupt: Off
;    1383  // Input Capture Interrupt: Off
;    1384  // Compare A Match Interrupt: Off
;    1385  // Compare B Match Interrupt: Off
;    1386  // Compare C Match Interrupt: Off
;    1387     TCCR3A=0x00;
;    1388     TCCR3B=0x00;
;    1389     TCNT3H=0x00;
;    1390     TCNT3L=0x00;
;    1391     ICR3H=0x00;
;    1392     ICR3L=0x00;
;    1393     OCR3AH=0x00;
;    1394     OCR3AL=0x00;
;    1395     OCR3BH=0x00;
;    1396     OCR3BL=0x00;
;    1397     OCR3CH=0x00;
;    1398     OCR3CL=0x00;
;    1399 
;    1400  // Timer(s)/Counter(s) Interrupt(s) initialization
;    1401     TIMSK=0x05;
;    1402     ETIMSK=0x00;
;    1403 }
;    1404 
;    1405 /**************************************************************************************/
;    1406 //                              Enable External Memory
;    1407 /**************************************************************************************/
;    1408 void Enable_XMEM()
;    1409 {
_Enable_XMEM:
;    1410  // External SRAM page configuration:
;    1411  // 1100h - 7FFFh / 8000h - FFFFh
;    1412  // Lower page wait state(s): 2r/w +1 addr
;    1413  // Upper page wait state(s): 2r/w + 1 addr
;    1414     MCUCR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x35,R30
;    1415     //MCUCR = 0x80;
;    1416 
;    1417     XMCRA=0x4E;
	LDI  R30,LOW(78)
	STS  109,R30
;    1418     //XMCRA=0x0A;
;    1419  // PC7:0 can not be released
;    1420     XMCRB=0x80;
	LDI  R30,LOW(128)
	STS  108,R30
;    1421     // XMCRB = 0x07;
;    1422 }
	RET
;    1423 
;    1424 /**************************************************************************************/
;    1425 //                              Initialize External Interrupt
;    1426 /**************************************************************************************/
;    1427 void Init_Ex_Interrupt()
;    1428 {
;    1429  // External Interrupt(s) initialization
;    1430  // INT0: Off
;    1431  // INT1: Off
;    1432  // INT2: Off
;    1433  // INT3: Off
;    1434  // INT4: On
;    1435  // INT4 Mode: Rising Edge
;    1436  // INT5: On
;    1437  // INT5 Mode: Rising Edge
;    1438  // INT6: On
;    1439  // INT6 Mode: Rising Edge
;    1440  // INT7: On
;    1441  // INT7 Mode: Rising Edge
;    1442     EICRA=0x00;
;    1443     EICRB=0xFF;
;    1444     EIMSK=0xF0;
;    1445     EIFR=0xF0;
;    1446 }
;    1447 
;    1448 /**************************************************************************************/
;    1449 //                              Analog Comparator initialization
;    1450 /**************************************************************************************/
;    1451 void Init_ADC()
;    1452 {
;    1453  // Analog Comparator initialization
;    1454  // Analog Comparator: Off
;    1455  // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1456     ACSR=0x80;
;    1457     SFIOR=0x00;
;    1458 }
;    1459 #include "global.h"
;    1460 #include "wendu.h"
;    1461 #include "utili.h"
;    1462 #include "math.h"
;    1463 #define logl log
;    1464 #define powl pow
;    1465 double PT100RToTValue(double r,double r0)
;    1466 {
_PT100RToTValue:
;    1467         double ac,bc,cc,tlow,tup,rnew,tnew;
;    1468         int count;
;    1469         ac = 3.908e-3;
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
;    1470         bc = -5.775E-7;
	__GETD1N 0xB51B057F
	__PUTD1S 22
;    1471         cc = -4.183E-12;
	__GETD1N 0xAC932D1D
	__PUTD1S 18
;    1472         tlow = -200;
	__GETD1N 0xC3480000
	CALL SUBOPT_0x52
;    1473         tup = 850;
	__GETD1N 0x44548000
	CALL SUBOPT_0x53
;    1474 	count = 0;
	__GETWRN 16,17,0
;    1475 
;    1476 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x124:
	CALL SUBOPT_0x54
	CALL SUBOPT_0x55
	CALL SUBOPT_0x56
	BREQ PC+2
	BRCC PC+3
	JMP  _0x127
	CALL SUBOPT_0x57
	BRLT _0x128
_0x127:
	RJMP _0x126
_0x128:
;    1477 	{
;    1478 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1479 		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
	__GETD1S 26
	CALL SUBOPT_0x59
	CALL SUBOPT_0x5A
	CALL SUBOPT_0x59
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5B
	CALL SUBOPT_0x59
	CALL SUBOPT_0x5A
	CALL SUBOPT_0x5C
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x5D
;    1480 		if(tnew < 0)
	CALL SUBOPT_0x5C
	CALL __CPD20
	BRGE _0x129
;    1481 		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
	__GETD1S 18
	CALL SUBOPT_0x59
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x5C
	__GETD1N 0x42C80000
	CALL SUBOPT_0x2D
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x5A
	CALL SUBOPT_0x5C
	CALL SUBOPT_0x5A
	CALL SUBOPT_0x5E
	CALL SUBOPT_0x5D
;    1482 
;    1483 		if(r < rnew)
_0x129:
	CALL SUBOPT_0x5F
	__GETD2S 34
	CALL __CMPF12
	BRSH _0x12A
;    1484 			tup = tnew;
	CALL SUBOPT_0x60
	CALL SUBOPT_0x53
;    1485 		else
	RJMP _0x12B
_0x12A:
;    1486 			tlow = tnew;
	CALL SUBOPT_0x61
;    1487 	}
_0x12B:
	RJMP _0x124
_0x126:
;    1488 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x62
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x63
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x64
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,38
	RET
;    1489 
;    1490 }
;    1491 double RValueToTValue(double r, u8 prbid)
;    1492 {
_RValueToTValue:
;    1493 	double ac,bc,cc,tlow,tup,rnew,tnew;
;    1494 	int count;
;    1495 
;    1496 	ac = rprbdata.param1[prbid];
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
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	__PUTD1S 26
;    1497 	bc = rprbdata.param2[prbid];
	__POINTW2MN _rprbdata,96
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	__PUTD1S 22
;    1498 	cc = rprbdata.param3[prbid];
	__POINTW2MN _rprbdata,192
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	__PUTD1S 18
;    1499 
;    1500 	if(rprbdata.type[prbid] == PRBTYPE_PT100)
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF1)
	BRNE _0x12C
;    1501 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x67
	RJMP _0x4B8
;    1502 	if(rprbdata.type[prbid] == PRBTYPE_PT1000)
_0x12C:
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF3)
	BRNE _0x12D
;    1503 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x67
	RJMP _0x4B8
;    1504 	if(rprbdata.type[prbid] != PRBTYPE_PT25)
_0x12D:
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF2)
	BREQ _0x12E
;    1505 		return -9999.999;
	__GETD1N 0xC61C3FFF
	RJMP _0x4B8
;    1506         if(rprbdata.rtp[prbid] > 0.1)
_0x12E:
	__POINTW2MN _rprbdata,288
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3DCCCCCD
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x12F
;    1507                 r = r/rprbdata.rtp[prbid];
	__POINTW2MN _rprbdata,288
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	CALL SUBOPT_0x68
	RJMP _0x4C5
;    1508         else
_0x12F:
;    1509 		r = r/25.0;
	CALL SUBOPT_0x68
	__GETD1N 0x41C80000
_0x4C5:
	CALL __DIVF21
	__PUTD1S 31
;    1510 
;    1511 
;    1512 
;    1513 
;    1514 
;    1515 	//set the search range of T between GetT(r) +/- 1 degree
;    1516 	tlow = GetT(r) - 1;
	CALL __PUTPARD1
	RCALL _GetT
	CALL SUBOPT_0x69
	CALL SUBOPT_0x52
;    1517 	tup = tlow + 2;
	__GETD1S 14
	__GETD2N 0x40000000
	CALL __ADDF12
	CALL SUBOPT_0x53
;    1518 
;    1519 	count = 0;
	__GETWRN 16,17,0
;    1520 
;    1521 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x131:
	CALL SUBOPT_0x6A
	BREQ PC+2
	BRCC PC+3
	JMP  _0x134
	CALL SUBOPT_0x57
	BRLT _0x135
_0x134:
	RJMP _0x133
_0x135:
;    1522 	{
;    1523 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1524 		rnew = GetWr(tnew);
	CALL SUBOPT_0x60
	CALL __PUTPARD1
	RCALL _GetWr
	CALL SUBOPT_0x6B
;    1525 		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
	CALL SUBOPT_0x5C
	__GETD1N 0xC33D581D
	CALL __CMPF12
	BRLO _0x137
	CALL SUBOPT_0x5C
	__GETD1N 0x3C23D70A
	CALL __CMPF12
	BREQ PC+4
	BRCS PC+3
	JMP  _0x137
	RJMP _0x138
_0x137:
	RJMP _0x136
_0x138:
;    1526         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
	CALL SUBOPT_0x6C
	CALL SUBOPT_0x6D
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6C
	CALL SUBOPT_0x6E
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6F
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
	RJMP _0x4C6
;    1527                 else
_0x136:
;    1528         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
	CALL SUBOPT_0x6C
	CALL SUBOPT_0x6D
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6C
	CALL SUBOPT_0x6E
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6C
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
	CALL SUBOPT_0x6C
	__GETD2S 18
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6C
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x6C
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
_0x4C6:
	CALL __ADDF12
	CALL SUBOPT_0x6B
;    1529 		if(r < rnew)
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x68
	CALL __CMPF12
	BRSH _0x13A
;    1530 			tup = tnew;
	CALL SUBOPT_0x70
;    1531 		else
	RJMP _0x13B
_0x13A:
;    1532 			tlow = tnew;
	CALL SUBOPT_0x61
;    1533 	}
_0x13B:
	RJMP _0x131
_0x133:
;    1534 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x62
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x63
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x64
_0x4B8:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,35
	RET
;    1535 
;    1536 
;    1537 //	return r+1;
;    1538 }
;    1539 
;    1540 flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
;    1541 flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
;    1542 flash u8 sizeAi = sizeof(Ai)/sizeof(double);
;    1543 flash u8 sizeCi = sizeof(Ci)/sizeof(double);
;    1544 
;    1545 double GetWr(double t)
;    1546 {
_GetWr:
;    1547 	double result;
;    1548 	double pert;
;    1549 	int i;
;    1550 	if(t < 0)
	CALL SUBOPT_0x71
;	t -> Y+10
;	result -> Y+6
;	pert -> Y+2
;	i -> R16,R17
	BRGE _0x13C
;    1551 	{       //equals to Wr189(t) in mfile
;    1552 		result = Ai[0];
	LDI  R30,LOW(_Ai*2)
	LDI  R31,HIGH(_Ai*2)
	CALL __GETD1PF
	CALL SUBOPT_0x6B
;    1553 		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
	CALL SUBOPT_0x55
	__GETD2N 0x43889333
	CALL SUBOPT_0x72
	__GETD1N 0x4388947B
	CALL __DIVF21
	CALL SUBOPT_0x73
	__GETD2N 0x3FC00000
	CALL SUBOPT_0x72
	__GETD1N 0x3FC00000
	CALL __DIVF21
	CALL SUBOPT_0x74
;    1554 		t = pert;
;    1555 		for(i=1;i<sizeAi;i++)
	__GETWRN 16,17,1
_0x13E:
	LDI  R30,LOW(_sizeAi*2)
	LDI  R31,HIGH(_sizeAi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x13F
;    1556 		{
;    1557 			result = result + Ai[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ai*2)
	LDI  R27,HIGH(_Ai*2)
	CALL SUBOPT_0x76
;    1558 			t = t*pert;
	CALL SUBOPT_0x77
;    1559 		}
	__ADDWRN 16,17,1
	RJMP _0x13E
_0x13F:
;    1560 		result = exp(result);
	CALL SUBOPT_0x6F
	CALL _exp
	CALL SUBOPT_0x6B
;    1561 	}else{
	RJMP _0x140
_0x13C:
;    1562 			 result = Ci[0];
	LDI  R30,LOW(_Ci*2)
	LDI  R31,HIGH(_Ci*2)
	CALL __GETD1PF
	CALL SUBOPT_0x6B
;    1563 			 pert = t/481.0 - 1;
	CALL SUBOPT_0x78
	__GETD1N 0x43F08000
	CALL __DIVF21
	CALL SUBOPT_0x69
	CALL SUBOPT_0x74
;    1564 			 t = pert;
;    1565 			 for(i=1;i<sizeCi;i++)
	__GETWRN 16,17,1
_0x142:
	LDI  R30,LOW(_sizeCi*2)
	LDI  R31,HIGH(_sizeCi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x143
;    1566 			 {
;    1567 				 result = result + Ci[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ci*2)
	LDI  R27,HIGH(_Ci*2)
	CALL SUBOPT_0x76
;    1568 				 t = t*pert;
	CALL SUBOPT_0x77
;    1569 			 }
	__ADDWRN 16,17,1
	RJMP _0x142
_0x143:
;    1570 	}
_0x140:
;    1571 	return result;
	CALL SUBOPT_0x5F
	RJMP _0x4B7
;    1572 }
;    1573 flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
;    1574 flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
;    1575 flash u8 sizeDi = sizeof(Di)/sizeof(double);
;    1576 flash u8 sizeBi = sizeof(Bi)/sizeof(double);
;    1577 
;    1578 double GetT(double w)
;    1579 {
_GetT:
;    1580 	double perw;
;    1581 	double result;
;    1582 	int i;
;    1583 
;    1584 	if(w >= 0)
	CALL SUBOPT_0x71
;	w -> Y+10
;	perw -> Y+6
;	result -> Y+2
;	i -> R16,R17
	BRLT _0x144
;    1585 	{       //t660r
;    1586 		perw = (w-2.64)/1.64;
	CALL SUBOPT_0x78
	__GETD1N 0x4028F5C3
	CALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3FD1EB85
	CALL SUBOPT_0x79
;    1587 		w = perw;
;    1588 		result = Di[0];
	LDI  R30,LOW(_Di*2)
	LDI  R31,HIGH(_Di*2)
	CALL SUBOPT_0x7A
;    1589 		for(i=1;i<sizeDi;i++)
_0x146:
	LDI  R30,LOW(_sizeDi*2)
	LDI  R31,HIGH(_sizeDi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x147
;    1590 		{
;    1591 			result = result + Di[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Di*2)
	LDI  R27,HIGH(_Di*2)
	CALL SUBOPT_0x7B
;    1592 			w = w*perw;
;    1593 		}
	__ADDWRN 16,17,1
	RJMP _0x146
_0x147:
;    1594 	}else{   //t189(r)
	RJMP _0x148
_0x144:
;    1595 		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
	CALL SUBOPT_0x55
	CALL __PUTPARD1
	__GETD1N 0x3E2AAAAB
	CALL __PUTPARD1
	CALL _pow
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F266666
	CALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3EB33333
	CALL SUBOPT_0x79
;    1596 		w = perw;
;    1597 		result = Bi[0];
	LDI  R30,LOW(_Bi*2)
	LDI  R31,HIGH(_Bi*2)
	CALL SUBOPT_0x7A
;    1598 		for(i=1;i<sizeBi;i++)
_0x14A:
	LDI  R30,LOW(_sizeBi*2)
	LDI  R31,HIGH(_sizeBi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x14B
;    1599 		{
;    1600 			result = result + Bi[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Bi*2)
	LDI  R27,HIGH(_Bi*2)
	CALL SUBOPT_0x7B
;    1601 			w = w*perw;
;    1602 		}
	__ADDWRN 16,17,1
	RJMP _0x14A
_0x14B:
;    1603 		result = 273.15*result - 273.15;
	CALL SUBOPT_0x60
	__GETD2N 0x43889333
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x43889333
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x7C
;    1604 	}
_0x148:
;    1605 	return result;
	CALL SUBOPT_0x60
_0x4B7:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
;    1606 }
;    1607 
;    1608 flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
;    1609 flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};
;    1610 
;    1611 flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
;    1612 flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
;    1613 flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};
;    1614 
;    1615 flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};
;    1616 
;    1617 flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
;    1618 flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};
;    1619 
;    1620 flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
;    1621 flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};
;    1622 
;    1623 flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
;    1624 flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};
;    1625 
;    1626 flash double BLow[]={0.000000000000E+00,-0.24650818346E-03,0.59040421171E-05,-0.13257931636E-08,0.15668291901E-11,-0.16944529240E-14,0.62990347094E-18};
;    1627 flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};
;    1628 
;    1629 flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1630 flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};
;    1631 
;    1632 flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
;    1633 flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};
;    1634 
;    1635 flash int TLowLen = sizeof(TLow)/sizeof(double);
;    1636 flash int THighLen = sizeof(THigh)/sizeof(double);
;    1637 
;    1638 flash int SLowLen = sizeof(SLow)/sizeof(double);
;    1639 flash int SMedLen = sizeof(SMed)/sizeof(double);
;    1640 flash int SHighLen = sizeof(SHigh)/sizeof(double);
;    1641 
;    1642 flash int RLowLen = sizeof(RLow)/sizeof(double);
;    1643 flash int RMedLen = sizeof(RMed)/sizeof(double);
;    1644 flash int RHighLen = sizeof(RHigh)/sizeof(double);
;    1645 
;    1646 flash int NLowLen = sizeof(NLow)/sizeof(double);
;    1647 flash int NHighLen = sizeof(NHigh)/sizeof(double);
;    1648 
;    1649 flash int JLowLen = sizeof(JLow)/sizeof(double);
;    1650 flash int JHighLen = sizeof(JHigh)/sizeof(double);
;    1651 
;    1652 flash int BLowLen = sizeof(BLow)/sizeof(double);
;    1653 flash int BHighLen = sizeof(BHigh)/sizeof(double);
;    1654 
;    1655 flash int ELowLen = sizeof(ELow)/sizeof(double);
;    1656 flash int EHighLen = sizeof(EHigh)/sizeof(double);
;    1657 
;    1658 flash int KLowLen = sizeof(KLow)/sizeof(double);
;    1659 flash int KHighLen = sizeof(KHigh)/sizeof(double);
;    1660 
;    1661 flash double *coef;

	.DSEG
_coef:
	.BYTE 0x2
;    1662 
;    1663 double GetThmoVolt(double t,char type)
;    1664 {

	.CSEG
_GetThmoVolt:
;    1665 	double result,pert;
;    1666 	int i;
;    1667 	int len = 0;
;    1668 	switch(type)
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
;    1669 	{
;    1670 		case PRBTYPE_T:
	CPI  R30,LOW(0x9)
	BRNE _0x14F
;    1671 			coef = (t < 0) ? TLow : THigh;
	CALL SUBOPT_0x7D
	BRGE _0x150
	LDI  R30,LOW(_TLow*2)
	LDI  R31,HIGH(_TLow*2)
	RJMP _0x151
_0x150:
	LDI  R30,LOW(_THigh*2)
	LDI  R31,HIGH(_THigh*2)
_0x151:
	CALL SUBOPT_0x7E
;    1672 			len = (t < 0) ? TLowLen : THighLen;
	BRGE _0x153
	LDI  R30,LOW(_TLowLen*2)
	LDI  R31,HIGH(_TLowLen*2)
	CALL __GETW1PF
	RJMP _0x154
_0x153:
	LDI  R30,LOW(_THighLen*2)
	LDI  R31,HIGH(_THighLen*2)
	CALL __GETW1PF
_0x154:
	MOVW R18,R30
;    1673 			break;
	RJMP _0x14E
;    1674 		case PRBTYPE_K:
_0x14F:
	CPI  R30,LOW(0x3)
	BRNE _0x156
;    1675 			coef = (t < 0) ? KLow : KHigh;
	CALL SUBOPT_0x7D
	BRGE _0x157
	LDI  R30,LOW(_KLow*2)
	LDI  R31,HIGH(_KLow*2)
	RJMP _0x158
_0x157:
	LDI  R30,LOW(_KHigh*2)
	LDI  R31,HIGH(_KHigh*2)
_0x158:
	CALL SUBOPT_0x7E
;    1676 			len = (t < 0) ? KLowLen : KHighLen;
	BRGE _0x15A
	LDI  R30,LOW(_KLowLen*2)
	LDI  R31,HIGH(_KLowLen*2)
	CALL __GETW1PF
	RJMP _0x15B
_0x15A:
	LDI  R30,LOW(_KHighLen*2)
	LDI  R31,HIGH(_KHighLen*2)
	CALL __GETW1PF
_0x15B:
	MOVW R18,R30
;    1677 			break;
	RJMP _0x14E
;    1678 		case PRBTYPE_N:
_0x156:
	CPI  R30,LOW(0x4)
	BRNE _0x15D
;    1679 			coef = (t < 0) ? NLow : NHigh;
	CALL SUBOPT_0x7D
	BRGE _0x15E
	LDI  R30,LOW(_NLow*2)
	LDI  R31,HIGH(_NLow*2)
	RJMP _0x15F
_0x15E:
	LDI  R30,LOW(_NHigh*2)
	LDI  R31,HIGH(_NHigh*2)
_0x15F:
	CALL SUBOPT_0x7E
;    1680 			len = (t < 0) ? NLowLen : NHighLen;
	BRGE _0x161
	LDI  R30,LOW(_NLowLen*2)
	LDI  R31,HIGH(_NLowLen*2)
	CALL __GETW1PF
	RJMP _0x162
_0x161:
	LDI  R30,LOW(_NHighLen*2)
	LDI  R31,HIGH(_NHighLen*2)
	CALL __GETW1PF
_0x162:
	MOVW R18,R30
;    1681 			break;
	RJMP _0x14E
;    1682 		case PRBTYPE_E:
_0x15D:
	CPI  R30,LOW(0x5)
	BRNE _0x164
;    1683 			coef = (t < 0) ? ELow : EHigh;
	CALL SUBOPT_0x7D
	BRGE _0x165
	LDI  R30,LOW(_ELow*2)
	LDI  R31,HIGH(_ELow*2)
	RJMP _0x166
_0x165:
	LDI  R30,LOW(_EHigh*2)
	LDI  R31,HIGH(_EHigh*2)
_0x166:
	CALL SUBOPT_0x7E
;    1684 			len = (t < 0) ? ELowLen : EHighLen;
	BRGE _0x168
	LDI  R30,LOW(_ELowLen*2)
	LDI  R31,HIGH(_ELowLen*2)
	CALL __GETW1PF
	RJMP _0x169
_0x168:
	LDI  R30,LOW(_EHighLen*2)
	LDI  R31,HIGH(_EHighLen*2)
	CALL __GETW1PF
_0x169:
	MOVW R18,R30
;    1685 			break;
	RJMP _0x14E
;    1686 		case PRBTYPE_B:
_0x164:
	CPI  R30,LOW(0x6)
	BRNE _0x16B
;    1687 			coef = (t < 630.615) ? BLow : BHigh;
	CALL SUBOPT_0x7F
	BRSH _0x16C
	LDI  R30,LOW(_BLow*2)
	LDI  R31,HIGH(_BLow*2)
	RJMP _0x16D
_0x16C:
	LDI  R30,LOW(_BHigh*2)
	LDI  R31,HIGH(_BHigh*2)
_0x16D:
	CALL SUBOPT_0x80
;    1688 			len = (t < 630.615) ? BLowLen : BHighLen;
	CALL SUBOPT_0x7F
	BRSH _0x16F
	LDI  R30,LOW(_BLowLen*2)
	LDI  R31,HIGH(_BLowLen*2)
	CALL __GETW1PF
	RJMP _0x170
_0x16F:
	LDI  R30,LOW(_BHighLen*2)
	LDI  R31,HIGH(_BHighLen*2)
	CALL __GETW1PF
_0x170:
	MOVW R18,R30
;    1689 			break;
	RJMP _0x14E
;    1690 		case PRBTYPE_J:
_0x16B:
	CPI  R30,LOW(0x7)
	BRNE _0x172
;    1691 			coef = (t < 760) ? JLow : JHigh;
	CALL SUBOPT_0x81
	BRSH _0x173
	LDI  R30,LOW(_JLow*2)
	LDI  R31,HIGH(_JLow*2)
	RJMP _0x174
_0x173:
	LDI  R30,LOW(_JHigh*2)
	LDI  R31,HIGH(_JHigh*2)
_0x174:
	CALL SUBOPT_0x80
;    1692 			len = (t < 760) ? JLowLen : JHighLen;
	CALL SUBOPT_0x81
	BRSH _0x176
	LDI  R30,LOW(_JLowLen*2)
	LDI  R31,HIGH(_JLowLen*2)
	CALL __GETW1PF
	RJMP _0x177
_0x176:
	LDI  R30,LOW(_JHighLen*2)
	LDI  R31,HIGH(_JHighLen*2)
	CALL __GETW1PF
_0x177:
	MOVW R18,R30
;    1693 			break;
	RJMP _0x14E
;    1694 
;    1695 		case PRBTYPE_S:
_0x172:
	CPI  R30,LOW(0x8)
	BRNE _0x179
;    1696 			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
	CALL SUBOPT_0x82
	CALL SUBOPT_0x83
	BRSH _0x17A
	LDI  R30,LOW(_SLow*2)
	LDI  R31,HIGH(_SLow*2)
	RJMP _0x17B
_0x17A:
	CALL SUBOPT_0x84
	BRSH _0x17D
	LDI  R30,LOW(_SMed*2)
	LDI  R31,HIGH(_SMed*2)
	RJMP _0x17E
_0x17D:
	LDI  R30,LOW(_SHigh*2)
	LDI  R31,HIGH(_SHigh*2)
_0x17E:
_0x17B:
	CALL SUBOPT_0x85
;    1697 			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
	BRSH _0x180
	LDI  R30,LOW(_SLowLen*2)
	LDI  R31,HIGH(_SLowLen*2)
	CALL __GETW1PF
	RJMP _0x181
_0x180:
	CALL SUBOPT_0x84
	BRSH _0x183
	LDI  R30,LOW(_SMedLen*2)
	LDI  R31,HIGH(_SMedLen*2)
	CALL __GETW1PF
	RJMP _0x184
_0x183:
	LDI  R30,LOW(_SHighLen*2)
	LDI  R31,HIGH(_SHighLen*2)
	CALL __GETW1PF
_0x184:
_0x181:
	MOVW R18,R30
;    1698 			break;
	RJMP _0x14E
;    1699 		case PRBTYPE_R:
_0x179:
	CPI  R30,LOW(0xA)
	BRNE _0x193
;    1700 			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
	CALL SUBOPT_0x82
	CALL SUBOPT_0x83
	BRSH _0x187
	LDI  R30,LOW(_RLow*2)
	LDI  R31,HIGH(_RLow*2)
	RJMP _0x188
_0x187:
	CALL SUBOPT_0x84
	BRSH _0x18A
	LDI  R30,LOW(_RMed*2)
	LDI  R31,HIGH(_RMed*2)
	RJMP _0x18B
_0x18A:
	LDI  R30,LOW(_RHigh*2)
	LDI  R31,HIGH(_RHigh*2)
_0x18B:
_0x188:
	CALL SUBOPT_0x85
;    1701 			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
	BRSH _0x18D
	LDI  R30,LOW(_RLowLen*2)
	LDI  R31,HIGH(_RLowLen*2)
	CALL __GETW1PF
	RJMP _0x18E
_0x18D:
	CALL SUBOPT_0x84
	BRSH _0x190
	LDI  R30,LOW(_RMedLen*2)
	LDI  R31,HIGH(_RMedLen*2)
	CALL __GETW1PF
	RJMP _0x191
_0x190:
	LDI  R30,LOW(_RHighLen*2)
	LDI  R31,HIGH(_RHighLen*2)
	CALL __GETW1PF
_0x191:
_0x18E:
	MOVW R18,R30
;    1702 			break;
	RJMP _0x14E
;    1703 		default:
_0x193:
;    1704 			return 0.0;
	CALL SUBOPT_0x86
	RJMP _0x4B6
;    1705 	}
_0x14E:
;    1706 	if (len == 0)
	MOV  R0,R18
	OR   R0,R19
	BRNE _0x194
;    1707 		return 0.0;
	CALL SUBOPT_0x86
	RJMP _0x4B6
;    1708 
;    1709 	result = coef[0];
_0x194:
	LDS  R30,_coef
	LDS  R31,_coef+1
	CALL __GETD1PF
	CALL SUBOPT_0x87
;    1710 	if(type == PRBTYPE_K)
	LDD  R26,Y+12
	CPI  R26,LOW(0x3)
	BRNE _0x195
;    1711 	{
;    1712 		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
	CALL SUBOPT_0x88
	__GETD2N 0xB8F82F06
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x88
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x89
	__GETD2N 0x3DF2E34C
	CALL SUBOPT_0x8A
;    1713 	}
;    1714 
;    1715 	pert = t;
_0x195:
	__GETD1S 13
	CALL SUBOPT_0x8B
;    1716 	for(i=1;i< len ;i++)
	__GETWRN 16,17,1
_0x197:
	__CPWRR 16,17,18,19
	BRGE _0x198
;    1717 	{
;    1718 		result = result + coef[i] * t;
	MOVW R30,R16
	LDS  R26,_coef
	LDS  R27,_coef+1
	CALL SUBOPT_0x8C
	CALL __GETD1PF
	CALL SUBOPT_0x82
	CALL SUBOPT_0x8A
;    1719 		t = t*pert;
	CALL SUBOPT_0x8D
	CALL SUBOPT_0x82
	CALL __MULF12
	__PUTD1S 13
;    1720 	}
	__ADDWRN 16,17,1
	RJMP _0x197
_0x198:
;    1721 	return result;
	CALL SUBOPT_0x8E
_0x4B6:
	CALL __LOADLOCR4
	ADIW R28,17
	RET
;    1722 }
;    1723 
;    1724 double MValueToTValue(double r,char type)
;    1725 {
_MValueToTValue:
;    1726 	double tlow;
;    1727 	double tup;
;    1728 	double rnew;
;    1729 	double tnew;
;    1730 	int count = 0;
;    1731 	switch(type)
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
;    1732 	{
;    1733 		case PRBTYPE_T:
	CPI  R30,LOW(0x9)
	BRNE _0x19C
;    1734 			tlow =  -270;	tup = 400;
	CALL SUBOPT_0x8F
	__GETD1N 0x43C80000
	CALL SUBOPT_0x53
;    1735 			break;
	RJMP _0x19B
;    1736 		case PRBTYPE_K:
_0x19C:
	CPI  R30,LOW(0x3)
	BRNE _0x19D
;    1737 			tlow =  -270;	tup = 1372;
	CALL SUBOPT_0x8F
	__GETD1N 0x44AB8000
	CALL SUBOPT_0x53
;    1738 			break;
	RJMP _0x19B
;    1739 		case PRBTYPE_N:
_0x19D:
	CPI  R30,LOW(0x4)
	BRNE _0x19E
;    1740 			tlow =  -270;	tup = 1300;
	CALL SUBOPT_0x8F
	__GETD1N 0x44A28000
	CALL SUBOPT_0x53
;    1741 			break;
	RJMP _0x19B
;    1742 		case PRBTYPE_E:
_0x19E:
	CPI  R30,LOW(0x5)
	BRNE _0x19F
;    1743 			tlow =  -270;	tup = 1000;
	CALL SUBOPT_0x8F
	__GETD1N 0x447A0000
	CALL SUBOPT_0x53
;    1744 			break;
	RJMP _0x19B
;    1745 		case PRBTYPE_B:
_0x19F:
	CPI  R30,LOW(0x6)
	BRNE _0x1A0
;    1746 			tlow =  0;	tup = 1800;
	__CLRD1S 14
	__GETD1N 0x44E10000
	CALL SUBOPT_0x53
;    1747 			break;
	RJMP _0x19B
;    1748 		case PRBTYPE_J:
_0x1A0:
	CPI  R30,LOW(0x7)
	BRNE _0x1A1
;    1749 			tlow =  -210;	tup = 1200;
	__GETD1N 0xC3520000
	CALL SUBOPT_0x52
	__GETD1N 0x44960000
	CALL SUBOPT_0x53
;    1750 			break;
	RJMP _0x19B
;    1751 		case PRBTYPE_S:
_0x1A1:
	CPI  R30,LOW(0x8)
	BRNE _0x1A2
;    1752 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x90
;    1753 			break;
	RJMP _0x19B
;    1754 		case PRBTYPE_R:
_0x1A2:
	CPI  R30,LOW(0xA)
	BRNE _0x1A4
;    1755 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x90
;    1756 			break;
	RJMP _0x19B
;    1757 		default:
_0x1A4:
;    1758 			return -9999.9999;
	__GETD1N 0xC61C4000
	RJMP _0x4B5
;    1759 	}
_0x19B:
;    1760 
;    1761 	while((tup - tlow > 0.00005) && (count++ < 100))
_0x1A5:
	CALL SUBOPT_0x6A
	BREQ PC+2
	BRCC PC+3
	JMP  _0x1A8
	CALL SUBOPT_0x57
	BRLT _0x1A9
_0x1A8:
	RJMP _0x1A7
_0x1A9:
;    1762 	{
;    1763 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1764 		rnew = GetThmoVolt(tnew,type);
	CALL SUBOPT_0x60
	CALL __PUTPARD1
	LDD  R30,Y+22
	ST   -Y,R30
	CALL _GetThmoVolt
	CALL SUBOPT_0x91
;    1765 
;    1766 		if(r < rnew)
	__GETD2S 19
	CALL __CMPF12
	BRSH _0x1AA
;    1767 			tup = tnew;
	CALL SUBOPT_0x70
;    1768 		else
	RJMP _0x1AB
_0x1AA:
;    1769 			tlow = tnew;
	CALL SUBOPT_0x61
;    1770 	}
_0x1AB:
	RJMP _0x1A5
_0x1A7:
;    1771 
;    1772 	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
	CALL SUBOPT_0x62
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x63
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL SUBOPT_0x64
_0x4B5:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,23
	RET
;    1773 }
;    1774 
;    1775 #include "font.h"
;    1776 
;    1777 #include <MATH.H>
;    1778 #include "typedef.h"
;    1779 #include "global.h"
;    1780 #include "utili.h"
;    1781 #include "key.h"
;    1782 extern uchar key;
;    1783 extern u8 lcd_buffer;
;    1784 /*
;    1785         Communication with LCD/KBD board,
;    1786         Incoming command format:
;    1787                 CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
;    1788                 key:            key pressed
;    1789         Out command format:
;    1790                 CMDO_DISPLAYBTN,LED_STATUS      update the led status
;    1791 */
;    1792 /**************************主程序********************************/
;    1793 void Key_Init()
;    1794 {
_Key_Init:
;    1795 	key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    1796 }
	RET
;    1797 
;    1798 #include <string.h>
;    1799 #include "lcd.h"
;    1800 #include "font.h"
;    1801 #include "global.h"
;    1802 #include "utili.h"
;    1803 
;    1804 
;    1805 
;    1806 extern uchar key;
;    1807 static uchar kbd_state;

	.DSEG
_kbd_state_G8:
	.BYTE 0x1
;    1808 void kbd_uart_push(uchar data)
;    1809 {

	.CSEG
_kbd_uart_push:
;    1810         if(data == STATUS_IDLE||
;	data -> Y+0
;    1811            data == STATUS_ERR||
;    1812            data == STATUS_DRAW||
;    1813            data == STATUS_DONE) //lcd acknowledge
	LD   R26,Y
	CPI  R26,LOW(0xF0)
	BREQ _0x1AD
	CPI  R26,LOW(0xF1)
	BREQ _0x1AD
	CPI  R26,LOW(0xF2)
	BREQ _0x1AD
	CPI  R26,LOW(0xF3)
	BRNE _0x1AC
_0x1AD:
;    1814         {
;    1815                 kbd_state = data;
	LD   R30,Y
	STS  _kbd_state_G8,R30
;    1816                 return;
	RJMP _0x4B1
;    1817         }
;    1818         if(key == KEY_INVALID)
_0x1AC:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE _0x1AF
;    1819                 key = data;
	LDD  R5,Y+0
;    1820 }
_0x1AF:
	RJMP _0x4B1
;    1821 static u8 lcdcmd[40];

	.DSEG
_lcdcmd_G8:
	.BYTE 0x28
;    1822 
;    1823 static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
_head_G8:
	.BYTE 0x3
;    1824 static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};
_tail_G8:
	.BYTE 0x3
;    1825 
;    1826 static u8 hex2ch[2];
_hex2ch_G8:
	.BYTE 0x2
;    1827 void hex2char(uchar a)
;    1828 {

	.CSEG
_hex2char:
;    1829         uchar h,l;
;    1830 	h = (a & 0xf0) >> 4;
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
;    1831 	l = (a & 0x0f);
	LDD  R30,Y+2
	ANDI R30,LOW(0xF)
	MOV  R16,R30
;    1832 	if(h <= 9)
	CPI  R17,10
	BRSH _0x1B2
;    1833 		hex2ch[0] = (h+'0');
	MOV  R30,R17
	SUBI R30,-LOW(48)
	RJMP _0x4C7
;    1834 	else
_0x1B2:
;    1835 		hex2ch[0] = (h+'A'-0x0a);
	MOV  R30,R17
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4C7:
	STS  _hex2ch_G8,R30
;    1836 	if(l <= 9)
	CPI  R16,10
	BRSH _0x1B4
;    1837 		hex2ch[1] = (l+'0');
	MOV  R30,R16
	SUBI R30,-LOW(48)
	RJMP _0x4C8
;    1838 	else
_0x1B4:
;    1839 		hex2ch[1] = (l+'A'-0x0a);
	MOV  R30,R16
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4C8:
	__PUTB1MN _hex2ch_G8,1
;    1840 }
	RJMP _0x4B4
;    1841 void onecmd(uchar len)
;    1842 {
_onecmd:
;    1843      uchar i = 0;
;    1844      uchar sum;
;    1845      sum = 0;
	ST   -Y,R17
	ST   -Y,R16
;	len -> Y+2
;	i -> R17
;	sum -> R16
	LDI  R17,0
	LDI  R16,LOW(0)
;    1846      prints(head,3,PORT_KBD); //send head
	LDI  R30,LOW(_head_G8)
	LDI  R31,HIGH(_head_G8)
	CALL SUBOPT_0x92
;    1847      for(i = 0;i<len;i++)     //caculate cksum
	LDI  R17,LOW(0)
_0x1B7:
	LDD  R30,Y+2
	CP   R17,R30
	BRSH _0x1B8
;    1848      {
;    1849         hex2char(lcdcmd[i]);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
;    1850 	prints(hex2ch,2,PORT_KBD);
;    1851         sum = sum + lcdcmd[i];
	CALL SUBOPT_0x93
	ADD  R16,R30
;    1852      }
	SUBI R17,-1
	RJMP _0x1B7
_0x1B8:
;    1853      lcdcmd[i] = 0xff - sum;
	MOV  R26,R17
	LDI  R27,0
	SUBI R26,LOW(-_lcdcmd_G8)
	SBCI R27,HIGH(-_lcdcmd_G8)
	LDI  R30,LOW(255)
	SUB  R30,R16
	ST   X,R30
;    1854      hex2char(lcdcmd[i]);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
;    1855      prints(hex2ch,2,PORT_KBD);
;    1856      prints(tail,3,PORT_KBD); //send tail
	LDI  R30,LOW(_tail_G8)
	LDI  R31,HIGH(_tail_G8)
	CALL SUBOPT_0x92
;    1857 }
_0x4B4:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,3
	RET
;    1858 static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

	.DSEG
_idle_G8:
	.BYTE 0xA
;    1859 void sendcmd(uchar len)
;    1860 {

	.CSEG
_sendcmd:
;    1861      ulong timeout;
;    1862      if(DEBUG == 1)
	SBIW R28,4
;	len -> Y+4
;	timeout -> Y+0
;    1863         return;
;    1864      while(kbd_state != STATUS_IDLE)
_0x1BB:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1BD
;    1865      {
;    1866         if(kbd_state == STATUS_DRAW)
	CPI  R26,LOW(0xF2)
	BREQ _0x1BB
;    1867                 continue;
;    1868         if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1BF
;    1869         {
;    1870                 prints(idle,10,PORT_KBD);
	LDI  R30,LOW(_idle_G8)
	LDI  R31,HIGH(_idle_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _prints
;    1871                 kbd_state = STATUS_DRAW;
	LDI  R30,LOW(242)
	STS  _kbd_state_G8,R30
;    1872         }
;    1873      }
_0x1BF:
	RJMP _0x1BB
_0x1BD:
;    1874      onecmd(len);
	CALL SUBOPT_0x95
;    1875      timeout = 0;
;    1876      kbd_state = STATUS_DONE;
	LDI  R30,LOW(243)
	STS  _kbd_state_G8,R30
;    1877      while(kbd_state != STATUS_IDLE)
_0x1C0:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1C2
;    1878      {
;    1879          if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1C3
;    1880          {
;    1881                 onecmd(len);
	CALL SUBOPT_0x95
;    1882                 timeout = 0;
;    1883          }
;    1884          if(timeout < 655350)
_0x1C3:
	CALL SUBOPT_0x1
	__CPD2N 0x9FFF6
	BRSH _0x1C4
;    1885          {
;    1886                 sleepms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x39
;    1887                 timeout++;
	CALL SUBOPT_0x2
	CALL SUBOPT_0x48
	__PUTD1S 0
;    1888          }else{
	RJMP _0x1C5
_0x1C4:
;    1889                 onecmd(len);
	CALL SUBOPT_0x95
;    1890                 timeout = 0;
;    1891          }
_0x1C5:
;    1892      }
	RJMP _0x1C0
_0x1C2:
;    1893 }
_0x4B3:
	ADIW R28,5
	RET
;    1894 /*------------------初始化-----------------*/
;    1895 void LCD_Init (void)
;    1896 {
_LCD_Init:
;    1897         kbd_state = STATUS_IDLE;
	LDI  R30,LOW(240)
	STS  _kbd_state_G8,R30
;    1898         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x96
;    1899         sendcmd(1);
;    1900         //do it twice avoid the missing of first command on boot up
;    1901         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x96
;    1902         sendcmd(1);
;    1903 
;    1904 }
	RET
;    1905 
;    1906 /********************************************************/
;    1907 /* 清屏							*/
;    1908 /********************************************************/
;    1909 void LCD_Cls(void)
;    1910 {
_LCD_Cls:
;    1911         lcdcmd[0] = CMDO_LCD_CLS;
	LDI  R30,LOW(4)
	STS  _lcdcmd_G8,R30
;    1912         sendcmd(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _sendcmd
;    1913 }
	RET
;    1914 void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
;    1915 {
_LCD_ClsBlock:
;    1916         lcdcmd[0] = CMDO_LCD_CLSBLK;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(6)
	CALL SUBOPT_0x97
;    1917         lcdcmd[1] = x1;
;    1918         lcdcmd[2] = y1;
;    1919         lcdcmd[3] = x2;
;    1920         lcdcmd[4] = y2;
;    1921         sendcmd(5);
;    1922 }
	RJMP _0x4B2
;    1923 void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
;    1924 {
_display_buttons:
;    1925         lcdcmd[0] = CMDO_DISPLAYBTN;
;	btn_pos -> Y+1
;	btn_val -> Y+0
	LDI  R30,LOW(2)
	STS  _lcdcmd_G8,R30
;    1926         if(btn_val == 0)
	LD   R30,Y
	CPI  R30,0
	BRNE _0x1C6
;    1927 	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
	LDD  R30,Y+1
	SUBI R30,-LOW(4)
	STD  Y+1,R30
;    1928         lcdcmd[1] = btn_pos;
_0x1C6:
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,1
;    1929         sendcmd(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _sendcmd
;    1930 }
	ADIW R28,2
	RET
;    1931 
;    1932 
;    1933 /*	设定图形x,y值*/
;    1934 /*
;    1935 void LCD_GraphSetAddr(uchar x,uchar y)
;    1936 {
;    1937 	uint xy;
;    1938 	xy=y;
;    1939 	xy=xy*16+x+256;
;    1940   	//LCD_CE(0);
;    1941 	LCD_Write2(xy&0xff,xy/256,0x24);
;    1942 }
;    1943 */
;    1944 /*	设定文本x,y值	*/
;    1945 /*
;    1946 void LCD_TextSetAddr(uchar x,uchar y)
;    1947 {
;    1948   	//LCD_CE(0);
;    1949 	LCD_Write2(y*16+x,0,0x24);
;    1950 }
;    1951 */
;    1952 /*	清除一点*/
;    1953 /*
;    1954 void LCD_ClrPixel(uchar x,uchar y)
;    1955 {
;    1956 	uchar b;
;    1957 	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;
;    1958 
;    1959 	b = 7 - (x % 8);
;    1960 
;    1961 	LCD_WriteInt(addr,0x24);
;    1962 	LCD_Comm(0xf0|b);
;    1963 }
;    1964 */
;    1965 /*	点亮一点	*/
;    1966 /*
;    1967 void LCD_Putpixel(uchar x,uchar y)
;    1968 {
;    1969 	uchar b;
;    1970 	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;
;    1971 
;    1972 	b = 7 - (x % 8);
;    1973 
;    1974 	LCD_WriteInt(addr,0x24);
;    1975 	LCD_Comm(0xf8|b);
;    1976 }
;    1977 */
;    1978 /*	x,y处显示光标	*/
;    1979 /*
;    1980 void LCD_ShowCursor(uchar x,uchar y)
;    1981 {
;    1982 	return;
;    1983   	//LCD_CE(0);
;    1984 	LCD_Comm(0x97);	//光标开
;    1985 	LCD_Write2(x,y,0x21);
;    1986 }
;    1987 
;    1988 //	取消光标
;    1989 
;    1990 void LCD_HideCursor(void)
;    1991 {
;    1992 	return;
;    1993   	//LCD_CE(0);
;    1994 	LCD_Comm(0x9c);
;    1995 }
;    1996 */
;    1997 /*
;    1998 void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
;    1999 {
;    2000 	x =(x / 8) * 8;
;    2001 
;    2002 	while(*s)
;    2003 	{
;    2004 		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
;    2005 
;    2006 		x = x + 8;
;    2007 
;    2008 		s++;
;    2009 	}
;    2010 }
;    2011 
;    2012 
;    2013 void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
;    2014 {
;    2015 	x =(x / 8) * 8;
;    2016 	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
;    2017 }
;    2018 */
;    2019 
;    2020 void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
;    2021 {
_LCD_ReverseRect:
;    2022         lcdcmd[0] = CMDO_LCD_REVERSE;
;	x -> Y+3
;	y -> Y+2
;	w -> Y+1
;	h -> Y+0
	LDI  R30,LOW(5)
	CALL SUBOPT_0x97
;    2023         lcdcmd[1] = x;
;    2024         lcdcmd[2] = y;
;    2025         lcdcmd[3] = w;
;    2026         lcdcmd[4] = h;
;    2027         sendcmd(5);
;    2028 }
	RJMP _0x4B2
;    2029 /*--------------显示字符------------------*/
;    2030 /*
;    2031 void LCD_TextPutchar(uchar x,uchar y,uchar c)
;    2032 {
;    2033   	//LCD_CE(0);
;    2034   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2035   	LCD_Comm(0xb0);
;    2036 	LCD_AutoWrite(c - 0x20);
;    2037   	LCD_Comm(0xb2);
;    2038 }
;    2039 
;    2040 void LCD_TextPrint(uchar x,uchar y,char *s)
;    2041 {
;    2042   	//LCD_CE(0);
;    2043   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2044   	LCD_Comm(0xb0);
;    2045   	while(*s)
;    2046   	{
;    2047 		LCD_AutoWrite(*s - 0x20);
;    2048 		s++;
;    2049 	}
;    2050   	LCD_Comm(0xb2);
;    2051 }
;    2052 void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
;    2053 {
;    2054 	uchar buf[4];
;    2055 	int i;
;    2056 
;    2057 	if(start > 3) start = 0;
;    2058 
;    2059 	for(i = 0; i < 4; i++)
;    2060 	{
;    2061 		buf[i] = n % 10;
;    2062 		n /= 10;
;    2063 	}
;    2064 
;    2065 	for(i = 3-start; i >= 0; i--)
;    2066 	{
;    2067 		LCD_TextPutchar(x,y,'0' + buf[i]);
;    2068 		x ++;
;    2069 	}
;    2070 }
;    2071 
;    2072 void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
;    2073 {
;    2074 	uchar c1,c2;
;    2075 	//低4位
;    2076 	c1 = hex & 0x0f;	//lo
;    2077 	//高4位
;    2078 	c2 = (hex >> 4) & 0x0f; //hi
;    2079 
;    2080 	LCD_TextPutchar(x,y,HexTable[c2]);
;    2081 	LCD_TextPutchar(x+1,y,HexTable[c1]);
;    2082 }
;    2083 */
;    2084 /************************************************/
;    2085 /*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
;    2086 /************************************************/
;    2087 /*
;    2088 void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
;    2089 {
;    2090 	register uchar t;
;    2091 	int xerr=0,yerr=0,delta_x,delta_y,distance;
;    2092 	int incx,incy,uRow,uCol;
;    2093 
;    2094 	delta_x = xt-x0;				//计算坐标增量
;    2095 	delta_y = yt-y0;
;    2096 	uRow = x0;
;    2097 	uCol = y0;
;    2098 	if(delta_x>0) incx=1;				//设置单步方向
;    2099 	else if( delta_x==0 ) incx=0;			//垂直线
;    2100 	else {incx=-1;delta_x=-delta_x;}
;    2101 
;    2102 	if(delta_y>0) incy=1;
;    2103 	else if( delta_y==0 ) incy=0;			//水平线
;    2104 	else {incy=-1;delta_y=-delta_y;}
;    2105 
;    2106 	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
;    2107 	else distance=delta_y;
;    2108 
;    2109   	//LCD_CE(0);
;    2110 	for( t=0;t <= distance+1; t++ )
;    2111         {					//画线输出
;    2112 		LCD_Putpixel(uRow,uCol);			//画点
;    2113 		xerr +=	delta_x	;
;    2114 		yerr +=	delta_y	;
;    2115 
;    2116 		if( xerr > distance )
;    2117                	{
;    2118 			xerr-=distance;
;    2119 			uRow+=incx;
;    2120 		}
;    2121 		if( yerr > distance )
;    2122                	{
;    2123 			yerr-=distance;
;    2124 			uCol+=incy;
;    2125 		}
;    2126 	}
;    2127 }
;    2128 */
;    2129 /*
;    2130 void LCD_LineH(uchar y)
;    2131 {
;    2132 	char i;
;    2133 
;    2134   	//LCD_CE(0);
;    2135   	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
;    2136 	LCD_Comm(0xb0);
;    2137 	for(i=0;i<LCD_WIDTH;i++)
;    2138  		LCD_AutoWrite(0xff);
;    2139 	LCD_Comm(0xb2);
;    2140 }
;    2141 */
;    2142 /*
;    2143 void LCD_LineV(uchar x,uchar y1,uchar y2)
;    2144 {
;    2145 	int i;
;    2146   	//LCD_CE(0);
;    2147 	for(i = y1; i < y2; i++)
;    2148 	{
;    2149 		LCD_Putpixel(x,i);
;    2150 	}
;    2151 }
;    2152 */
;    2153 
;    2154 void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
;    2155 {
_LCD_Rectange:
;    2156         lcdcmd[0] = CMDO_LCD_RECT;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(7)
	CALL SUBOPT_0x97
;    2157         lcdcmd[1] = x1;
;    2158         lcdcmd[2] = y1;
;    2159         lcdcmd[3] = x2;
;    2160         lcdcmd[4] = y2;
;    2161         sendcmd(5);
;    2162 }
	RJMP _0x4B2
;    2163 void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
;    2164 {
_LCD_PrintChar:
;    2165         uchar pos = 0;
;    2166         lcdcmd[0] = cmd;
	ST   -Y,R17
;	cmd -> Y+5
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	pos -> R17
	LDI  R17,0
	LDD  R30,Y+5
	STS  _lcdcmd_G8,R30
;    2167         lcdcmd[1] = x;
	LDD  R30,Y+4
	__PUTB1MN _lcdcmd_G8,1
;    2168         lcdcmd[2] = y;
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,2
;    2169         pos = 1;
	LDI  R17,LOW(1)
;    2170         while(*s)
_0x1C7:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x1C9
;    2171         {
;    2172                 lcdcmd[(uchar)(pos + 3)] = *s++;
	CALL SUBOPT_0x98
	MOVW R0,R30
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOVW R26,R0
	ST   X,R30
;    2173                 pos = pos + 1;
	SUBI R17,-LOW(1)
;    2174         }
	RJMP _0x1C7
_0x1C9:
;    2175         lcdcmd[(uchar)(pos+3)] = 0x00;
	CALL SUBOPT_0x98
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2176         lcdcmd[3] = pos;
	__POINTW1MN _lcdcmd_G8,3
	ST   Z,R17
;    2177         pos = pos + 4;
	SUBI R17,-LOW(4)
;    2178         sendcmd(pos);
	ST   -Y,R17
	CALL _sendcmd
;    2179 }
	LDD  R17,Y+0
	ADIW R28,6
	RET
;    2180 void LCD_PrintHz12(uchar x,uchar y,uchar *s)
;    2181 {
_LCD_PrintHz12:
;    2182         LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(8)
	CALL SUBOPT_0x99
;    2183 }
	RJMP _0x4B2
;    2184 void LCD_PrintHz16(uchar x,uchar y,uchar *s)
;    2185 {
_LCD_PrintHz16:
;    2186         LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(9)
	CALL SUBOPT_0x99
;    2187 }
	RJMP _0x4B2
;    2188 //显示6x8的数字
;    2189 void LCD_Print6X8(uchar x, uchar y,uchar *s)
;    2190 {
_LCD_Print6X8:
;    2191         LCD_PrintChar(CMDO_LCD_68,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(16)
	CALL SUBOPT_0x99
;    2192 }
	RJMP _0x4B2
;    2193 
;    2194 //显示6x8的数字
;    2195 void LCD_Print8X16(uchar x, uchar y,uchar *s)
;    2196 {
_LCD_Print8X16:
;    2197         LCD_PrintChar(CMDO_LCD_816,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(17)
	CALL SUBOPT_0x99
;    2198 }
_0x4B2:
	ADIW R28,4
	RET
;    2199 
;    2200 //显示24x32的数字
;    2201 /*
;    2202 void LCD_Print24X32(uchar x, uchar y,uchar *s)
;    2203 {
;    2204 	//x坐标必需是8位对齐
;    2205 	x =(x / 8) * 8;
;    2206 	while(*s)
;    2207 	{
;    2208 		if( *s >= '0' && *s <= '9')	//显示
;    2209 		{
;    2210 			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
;    2211 			x += 24;
;    2212 		}
;    2213 		else if( *s == ' ')
;    2214 		{
;    2215 			x += 8;
;    2216 		}
;    2217 		else if( *s == '-')
;    2218 		{
;    2219 			LCD_PutImg(x,y,3,32,Font24X32[12]);
;    2220 			x += 24;
;    2221 		}
;    2222 		else
;    2223 		{
;    2224 			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
;    2225 			x += 8;
;    2226 		}
;    2227 		s ++;
;    2228 	}
;    2229 }*/
;    2230 #include "utili.h"
;    2231 #include "scanner.h"
;    2232 #include "stdlib.h"
;    2233 
;    2234 static uchar navlen = 0;

	.DSEG
_navlen_G9:
	.BYTE 0x1
;    2235 static double reading = -1000;
_reading_G9:
	.BYTE 0x4
;    2236 static char navread[20];
_navread_G9:
	.BYTE 0x14
;    2237 static char navread2[20];
_navread2_G9:
	.BYTE 0x14
;    2238 u8 scancmd[5];
_scancmd:
	.BYTE 0x5
;    2239 u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE

	.ESEG
_scanner_type:
	.DB  0x1
;    2240 void scanner_set_mode()
;    2241 {

	.CSEG
_scanner_set_mode:
;    2242         if (IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x1CB
;    2243         {
;    2244             scancmd[0] = '#';
	LDI  R30,LOW(35)
	RJMP _0x4C9
;    2245         }else{
_0x1CB:
;    2246             scancmd[0]= '!';
	LDI  R30,LOW(33)
_0x4C9:
	STS  _scancmd,R30
;    2247         }
;    2248         scancmd[1] = 0x0D;
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2249         scancmd[2] = 0x0A;
	__POINTW1MN _scancmd,2
	CALL SUBOPT_0x9A
;    2250         prints(scancmd,3,PORT_SCANNER);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x9B
;    2251 }
	RET
;    2252 void scanner_set_channel(uchar ch)
;    2253 {
_scanner_set_channel:
;    2254         if(ch == 1)
;	ch -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1CD
;    2255         {
;    2256                 SET_TOCH1;
	CBI  0x18,4
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	CALL SUBOPT_0x39
	SBI  0x18,4
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    2257         }
;    2258         if(ch == 2)
_0x1CD:
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1CE
;    2259         {
;    2260                 SET_TOCH2;
	CBI  0x18,5
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	CALL SUBOPT_0x39
	SBI  0x18,5
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    2261                 return;
	RJMP _0x4B1
;    2262         }
;    2263         if(scanner_type == 1) //MI
_0x1CE:
	CALL SUBOPT_0x9C
	CPI  R30,LOW(0x1)
	BRNE _0x1CF
;    2264         {
;    2265                 if(ch < 10)
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRSH _0x1D0
;    2266                 {
;    2267                         scancmd[0] = (ch + '0');
	LD   R30,Y
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2268                         scancmd[1] = 'A';
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    2269                         scancmd[2] = 0x0D;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2270                         scancmd[3] = 0x0A;
	__POINTW1MN _scancmd,3
	CALL SUBOPT_0x9A
;    2271                         prints(scancmd,4,PORT_SCANNER);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x9B
;    2272                         return;
	RJMP _0x4B1
;    2273                 }else{
_0x1D0:
;    2274                         scancmd[0] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2275                         ch = ch % 10;
	CALL SUBOPT_0x9D
;    2276                         scancmd[1] = ch + '0';
	__PUTB1MN _scancmd,1
;    2277                         scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(65)
	STD  Z+0,R26
	CALL SUBOPT_0x9E
;    2278                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x9B
;    2279                 }
;    2280         }
;    2281         if(scanner_type == 2) //guidline
_0x1CF:
	CALL SUBOPT_0x9C
	CPI  R30,LOW(0x2)
	BRNE _0x1D2
;    2282         {
;    2283                         scancmd[0] = 'A';
	LDI  R30,LOW(65)
	STS  _scancmd,R30
;    2284                         scancmd[1] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	__PUTB1MN _scancmd,1
;    2285                         ch = ch % 10;
	CALL SUBOPT_0x9D
;    2286                         scancmd[2] = ch + '0';
	__PUTB1MN _scancmd,2
;    2287                         scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	CALL SUBOPT_0x9E
;    2288                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x9B
;    2289         }
;    2290 }
_0x1D2:
	RJMP _0x4B1
;    2291 
;    2292 
;    2293 //incoming data handler of scanner
;    2294 void scanner_uart_push(uchar data)
;    2295 {
_scanner_uart_push:
;    2296 }
	RJMP _0x4B1
;    2297 //incoming data hander of PC
;    2298 void pc_uart_push(uchar data)
;    2299 {
_pc_uart_push:
;    2300 }
	RJMP _0x4B1
;    2301 static uchar navcmd[12];

	.DSEG
_navcmd_G9:
	.BYTE 0xC
;    2302 #define NAV_INVALID     -1000
;    2303 #define NAV_VALID       -2000
;    2304 #define RESETNAV       navlen = 0;   reading = NAV_INVALID;
;    2305 
;    2306 void nav_command(uchar cmd)
;    2307 {

	.CSEG
_nav_command:
;    2308         if(cmd == NAV_30V)
;	cmd -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1D3
;    2309                sprintf(navcmd,"%%01;12;02\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,87
	CALL SUBOPT_0x33
;    2310         if(cmd == NAV_1V)
_0x1D3:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x1D4
;    2311                sprintf(navcmd,"%%01;12;01\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,99
	CALL SUBOPT_0x33
;    2312         if(cmd == NAV_120MV)
_0x1D4:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x1D5
;    2313                sprintf(navcmd,"%%01;12;00\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,111
	CALL SUBOPT_0x33
;    2314         if(cmd == NAV_AFLTOFF)
_0x1D5:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x1D6
;    2315                sprintf(navcmd,"%%01;26\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,123
	CALL SUBOPT_0x33
;    2316         if(cmd == NAV_AFLTON)
_0x1D6:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x1D7
;    2317                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,132
	CALL SUBOPT_0x33
;    2318         if(cmd == NAV_SLOWMODE)
_0x1D7:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x1D8
;    2319                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,132
	CALL SUBOPT_0x33
;    2320         if(cmd == NAV_ZEROON)
_0x1D8:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x1D9
;    2321         {
;    2322                sprintf(navcmd,"%%01;06\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,141
	CALL SUBOPT_0x33
;    2323         }
;    2324         if(cmd == NAV_INIT)
_0x1D9:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1DA
;    2325         {
;    2326                 RESETNAV;
	CALL SUBOPT_0xA0
;    2327                 sprintf(navcmd,"%%01;00\r");
	__POINTW1FN _0,150
	CALL SUBOPT_0x33
;    2328         }
;    2329         if(cmd == NAV_READ)
_0x1DA:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0x1DB
;    2330         {
;    2331                 RESETNAV;
	CALL SUBOPT_0xA0
;    2332                 sprintf(navcmd,"%%01;01\r");
	__POINTW1FN _0,159
	CALL SUBOPT_0x33
;    2333         }
;    2334 //                com1_putc('?');
;    2335         prints(navcmd,strlen(navcmd),PORT_NAV);
_0x1DB:
	CALL SUBOPT_0x9F
	CALL SUBOPT_0x9F
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _prints
;    2336 }
	RJMP _0x4B1
;    2337 
;    2338 
;    2339 unsigned long navtime;

	.DSEG
_navtime:
	.BYTE 0x4
;    2340 double nav_read()
;    2341 {

	.CSEG
_nav_read:
;    2342         if(DEBUG == 1)
;    2343                 return 1.0;
;    2344         nav_command(NAV_READ);
	CALL SUBOPT_0xA1
;    2345         navtime = 0;
;    2346         while(1)
_0x1DD:
;    2347         {
;    2348                 if(reading < NAV_INVALID) //make sure it is a valid reading
	LDS  R26,_reading_G9
	LDS  R27,_reading_G9+1
	LDS  R24,_reading_G9+2
	LDS  R25,_reading_G9+3
	CALL SUBOPT_0xA2
	CALL __CMPF12
	BRSH _0x1E0
;    2349                 {
;    2350                      reading = atof(navread2);
	LDI  R30,LOW(_navread2_G9)
	LDI  R31,HIGH(_navread2_G9)
	ST   -Y,R31
	ST   -Y,R30
	CALL _atof
	CALL SUBOPT_0xA3
;    2351                      return reading;
	LDS  R30,_reading_G9
	LDS  R31,_reading_G9+1
	LDS  R22,_reading_G9+2
	LDS  R23,_reading_G9+3
	RET
;    2352                 }
;    2353                 if(navtime++ > 655350)
_0x1E0:
	LDS  R30,_navtime
	LDS  R31,_navtime+1
	LDS  R22,_navtime+2
	LDS  R23,_navtime+3
	CALL SUBOPT_0x48
	STS  _navtime,R30
	STS  _navtime+1,R31
	STS  _navtime+2,R22
	STS  _navtime+3,R23
	SBIW R30,1
	SBCI R22,0
	SBCI R23,0
	__CPD1N 0x9FFF7
	BRLO _0x1E1
;    2354                 {
;    2355                      nav_command(NAV_READ);
	CALL SUBOPT_0xA1
;    2356                      navtime = 0;
;    2357                 }
;    2358                 sleepms(1);
_0x1E1:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x39
;    2359         }
	RJMP _0x1DD
;    2360         return 1.0;
;    2361 }
;    2362 //incoming data hander of navameter
;    2363 void nav_uart_push(uchar data)
;    2364 {
_nav_uart_push:
;    2365         if(navlen >= 19)
;	data -> Y+0
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x13)
	BRLO _0x1E2
;    2366         {
;    2367                 RESETNAV;
	CALL SUBOPT_0xA5
;    2368         }
;    2369         if(((data >= '0') && (data <= '9')) ||
_0x1E2:
;    2370                 (data == '.') ||
;    2371                 (data == '-') ||
;    2372                 (data == '+') ||
;    2373                 (data == 'e') ||
;    2374                 (data == 'E') )
	LD   R26,Y
	CPI  R26,LOW(0x30)
	BRLO _0x1E4
	CPI  R26,LOW(0x3A)
	BRLO _0x1E6
_0x1E4:
	LD   R26,Y
	CPI  R26,LOW(0x2E)
	BREQ _0x1E6
	CPI  R26,LOW(0x2D)
	BREQ _0x1E6
	CPI  R26,LOW(0x2B)
	BREQ _0x1E6
	CPI  R26,LOW(0x65)
	BREQ _0x1E6
	CPI  R26,LOW(0x45)
	BRNE _0x1E3
_0x1E6:
;    2375         {
;    2376                 navread[navlen++] = data;
	LDS  R30,_navlen_G9
	SUBI R30,-LOW(1)
	STS  _navlen_G9,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	LD   R26,Y
	STD  Z+0,R26
;    2377                 reading = NAV_INVALID;
	CALL SUBOPT_0xA2
	CALL SUBOPT_0xA3
;    2378                 return;
	RJMP _0x4B1
;    2379         }
;    2380 
;    2381         if(navlen < 4) //not enough digits
_0x1E3:
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x4)
	BRSH _0x1E8
;    2382         {
;    2383                 RESETNAV;
	CALL SUBOPT_0xA5
;    2384                 return;
	RJMP _0x4B1
;    2385         }
;    2386         navread[navlen] = '\0';
_0x1E8:
	CALL SUBOPT_0xA6
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2387         do
_0x1EA:
;    2388         {
;    2389                 navread2[navlen] = navread[navlen];
	LDS  R26,_navlen_G9
	LDI  R27,0
	SUBI R26,LOW(-_navread2_G9)
	SBCI R27,HIGH(-_navread2_G9)
	CALL SUBOPT_0xA6
	LD   R30,Z
	ST   X,R30
;    2390         }while(navlen-- > 0);
	LDS  R26,_navlen_G9
	SUBI R26,LOW(1)
	STS  _navlen_G9,R26
	SUBI R26,-LOW(1)
	CPI  R26,LOW(0x1)
	BRSH _0x1EA
;    2391         navlen = 0;
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
;    2392         reading =  NAV_VALID; //valid data
	__GETD1N 0xC4FA0000
	CALL SUBOPT_0xA3
;    2393 }
_0x4B1:
	ADIW R28,1
	RET
;    2394 #include <mega64.h>
;    2395 #include "lcd.h"
;    2396 #include "key.h"
;    2397 #include "utili.h"
;    2398 #include "window.h"
;    2399 #include "utili.h"
;    2400 #include "scanner.h"
;    2401 
;    2402 extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
;    2403 #define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
;    2404                 		if(new_page == 1){\
;    2405 		                	for(i = min_option; i <= max_option; i++){\
;    2406 				                j = ((curr_pos-1)/max_option)*max_option + i;
;    2407 
;    2408 #define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
;    2409 				        if((curr_pos-1) % max_option == (i-1))\
;    2410 					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2411         			        }\
;    2412                                 }else{\
;    2413 	                		for(i = min_option; i <= max_option; i++) {\
;    2414         		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2415 				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2416 			                }\
;    2417 		                }
;    2418 
;    2419 #define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
;    2420 			if(curr_pos > max_index) curr_pos = min_index;\
;    2421 			if(last_pos == curr_pos)	return;\
;    2422 			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}
;    2423 
;    2424 #define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
;    2425 			if(curr_pos < min_index) curr_pos = max_index;\
;    2426 			if(last_pos == curr_pos)  return;\
;    2427 			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}
;    2428 
;    2429 #define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
;    2430 					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
;    2431 
;    2432 #define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
;    2433                                         draw_label(&PRE##options[i-1], SW_NORMAL);\
;    2434 			        }
;    2435 #define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
;    2436                                         return;\
;    2437         			for(i = min_option;i <= max_option; i++){\
;    2438 	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2439         				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2440         			}
;    2441 
;    2442 #define INC_OPTION	last_sel = curr_sel;\
;    2443 			curr_sel++;\
;    2444 			if(curr_sel > max_option)\
;    2445 				curr_sel = min_option;
;    2446 
;    2447 #define DEC_OPTION	last_sel = curr_sel;\
;    2448 			curr_sel--;\
;    2449 			if(curr_sel < min_option)\
;    2450 				curr_sel = max_option;
;    2451 
;    2452 extern int   curr_ch;	//index of current channel in configuation   menu
;    2453 extern int   curr_prb;	//index of current probe selection
;    2454 extern int   curr_dispch; //start of current channel for display in main menu
;    2455 
;    2456 void State_Init() {
_State_Init:
;    2457 	display_buttons(KEY_BTN1,sysdata.prbmode);
	LDI  R30,LOW(97)
	ST   -Y,R30
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2458 	display_buttons(KEY_BTN2,sysdata.kttmode);
	LDI  R30,LOW(98)
	ST   -Y,R30
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2459 	display_buttons(KEY_BTN3,0); //probe type
	LDI  R30,LOW(99)
	CALL SUBOPT_0x25
;    2460 	display_buttons(KEY_BTN4,0); //zero
	LDI  R30,LOW(100)
	CALL SUBOPT_0x25
;    2461 }
	RET
;    2462 char* getprbtype(uchar);
;    2463 LABEL flash pgmain_chs[] = {
;    2464 		{LBL_HZ6X8,3, 3,5,strbuf},
;    2465 		{LBL_HZ6X8,3,16,5,strbuf},
;    2466 		{LBL_HZ6X8,3,29,5,strbuf},
;    2467 		{LBL_HZ6X8,3,42,5,strbuf}
;    2468 	};
;    2469 LABEL flash pgmain_values[] = {
;    2470 		{LBL_HZ6X8,40,3,10,strbuf},
;    2471 		{LBL_HZ6X8,40,16,10,strbuf},
;    2472 		{LBL_HZ6X8,40,29,10,strbuf},
;    2473 		{LBL_HZ6X8,40,42,10,strbuf}
;    2474 	};
;    2475 LABEL flash pgmain_temps[] = {
;    2476 		{LBL_HZ6X8,140,3, 10,strbuf},
;    2477 		{LBL_HZ6X8,140,16,10,strbuf},
;    2478 		{LBL_HZ6X8,140,29,10,strbuf},
;    2479 		{LBL_HZ6X8,140,42,10,strbuf}
;    2480 };
;    2481 /*      Main display      */
;    2482 
;    2483 void pgmain_handler(uchar msg) {
_pgmain_handler:
;    2484         int max_option = 4;
;    2485         uchar i,old_ch,isinit;
;    2486                 uchar *cptr;
;    2487 	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
;    2488 
;    2489 	isinit = 0;
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
;    2490         //Menu
;    2491         if(msg == KEY_CE) {
	LDD  R26,Y+8
	CPI  R26,LOW(0x43)
	BRNE _0x1EC
;    2492                 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2493                 return;
	RJMP _0x4B0
;    2494         }
;    2495 	if(msg == KEY_TAB) {
_0x1EC:
	LDD  R26,Y+8
	CPI  R26,LOW(0x54)
	BRNE _0x1ED
;    2496 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x1EE
;    2497 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4CA
;    2498 		else
_0x1EE:
;    2499 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4CA:
	MOV  R4,R30
;    2500 		return;
	RJMP _0x4B0
;    2501 	}
;    2502         if(key == KEY_NUM1) //R0
_0x1ED:
	LDI  R30,LOW(49)
	CP   R30,R5
	BRNE _0x1F0
;    2503         {
;    2504 /*
;    2505                 window_setup(10);
;    2506                 if(IS_BORE_MODE)
;    2507                 {
;    2508                         sprintf(strbuf,"请输入铂电阻R0阻值");
;    2509 	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
;    2510 		        msg = MSG_INIT;
;    2511 		}else{
;    2512 		        return;
;    2513                         sprintf(strbuf,"请输入V0值");
;    2514 	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
;    2515                         msg = MSG_INIT;
;    2516 		}
;    2517 */
;    2518         }
;    2519         if(key == KEY_NUM2) //Rs1
_0x1F0:
	LDI  R30,LOW(50)
	CP   R30,R5
	BRNE _0x1F1
;    2520         {
;    2521 /*
;    2522                 if(IS_BORE_MODE)
;    2523                 {
;    2524                         window_setup(10);
;    2525                         sprintf(strbuf,"请输入PT100内标准阻值");
;    2526 		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
;    2527         		msg = MSG_INIT;
;    2528                 }
;    2529 */
;    2530         }
;    2531         if(key == KEY_NUM3) //Rs2
_0x1F1:
	LDI  R30,LOW(51)
	CP   R30,R5
	BRNE _0x1F2
;    2532         {
;    2533 /*
;    2534                 if(IS_BORE_MODE)
;    2535                 {
;    2536                         window_setup(10);
;    2537                         sprintf(strbuf,"请输入PT1000内标准阻值");
;    2538 		        sysdata.Rs2 = wnd_floatinput(sysdata.Rs2);
;    2539         		msg = MSG_INIT;
;    2540                 }
;    2541 */
;    2542         }
;    2543 
;    2544 	if(msg == KEY_UP) {
_0x1F2:
	LDD  R26,Y+8
	CPI  R26,LOW(0x55)
	BRNE _0x1F3
;    2545 		DEC_DISPCH;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	__SUBWRR 10,11,30,31
	MOV  R0,R10
	OR   R0,R11
	BRNE _0x1F4
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	MOVW R10,R30
_0x1F4:
;    2546 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2547 	}
;    2548 	if(msg == KEY_DN) {
_0x1F3:
	LDD  R26,Y+8
	CPI  R26,LOW(0x44)
	BRNE _0x1F5
;    2549 		INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x1F6
	CALL SUBOPT_0x3D
_0x1F6:
;    2550 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2551 	}
;    2552 
;    2553 	if(msg == MSG_INIT) {
_0x1F5:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFF)
	BRNE _0x1F7
;    2554 	        isinit = 1;
	LDI  R21,LOW(1)
;    2555 		LCD_Cls();
	CALL _LCD_Cls
;    2556 		old_ch = curr_dispch;
	CALL SUBOPT_0xA8
;    2557 
;    2558 	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2559 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1F9:
	CPI  R19,4
	BRSH _0x1FA
;    2560 		{
;    2561 		        if(curr_dispch == old_ch)
	MOV  R30,R18
	MOVW R26,R10
	CALL SUBOPT_0xA9
	BRNE _0x1FB
;    2562 		        {
;    2563         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2564                                 draw_label(&pgmain_chs[i],SW_NORMAL);
	CALL SUBOPT_0x38
;    2565                                 draw_label(&pgmain_chs[i],SW_REVERSE);
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	RJMP _0x4CB
;    2566         		}
;    2567         		else{
_0x1FB:
;    2568         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2569         			draw_label(&pgmain_chs[i],SW_NORMAL);
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
_0x4CB:
	ST   -Y,R30
	CALL _draw_label
;    2570         		}
;    2571 			INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x1FD
	CALL SUBOPT_0x3D
_0x1FD:
;    2572 		}
	SUBI R19,-1
	RJMP _0x1F9
_0x1FA:
;    2573 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2574 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+8,R30
;    2575 	}
;    2576 
;    2577 	if(msg == MSG_REFRESH) {
_0x1F7:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x1FE
;    2578 		old_ch = curr_dispch;
	CALL SUBOPT_0xA8
;    2579 		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2580 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x200:
	CPI  R19,4
	BRLO PC+3
	JMP _0x201
;    2581 		{
;    2582 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BREQ PC+3
	JMP _0x202
;    2583 		        {
;    2584 			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
	CALL SUBOPT_0x43
	__POINTW2MN _tprbdata,576
	CALL SUBOPT_0x2A
	ST   -Y,R30
	RCALL _getprbtype
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2585         			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2586 	        			(cptr[0] == 'P')){
	CALL SUBOPT_0x43
	CPI  R30,LOW(0xFF)
	BREQ _0x204
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BRNE _0x203
_0x204:
;    2587                				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2588                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x207
	CPI  R21,1
	BRNE _0x206
_0x207:
;    2589                                         {
;    2590                         			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x38
;    2591 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4CC
;    2592 		                        }else{
_0x206:
;    2593                         			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2594 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4CC:
	ST   -Y,R30
	CALL _draw_label
;    2595 		                        }
;    2596 			        }else{
	RJMP _0x20A
_0x203:
;    2597 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB1
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2598 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xAD
	BRNE _0x20B
;    2599         					sprintf(strbuf+8,"*mV");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,191
	RJMP _0x4CD
;    2600                                         else
_0x20B:
;    2601                                                 sprintf(strbuf+8," mV");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,195
_0x4CD:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2602                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x20E
	CPI  R21,1
	BRNE _0x20D
_0x20E:
;    2603                                         {
;    2604         					draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x38
;    2605         					if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x210
;    2606         					        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,199
	CALL SUBOPT_0x33
;    2607         					else
	RJMP _0x211
_0x210:
;    2608                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2609 
;    2610                 				sprintf(strbuf+8," ! %c",cptr[0]);
_0x211:
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2611                 			        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4CE
;    2612                                         }else{
_0x20D:
;    2613                                                 draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2614                 				if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x213
;    2615                 				{
;    2616                 				        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,199
	CALL SUBOPT_0x33
;    2617                 				}else{
	RJMP _0x214
_0x213:
;    2618                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2619                 				}
_0x214:
;    2620         				        sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2621 	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4CE:
	ST   -Y,R30
	CALL _draw_label
;    2622                                         }
;    2623 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xB7
	BRNE _0x215
;    2624         				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4CF
;    2625         				else                                     //Hz12
_0x215:
;    2626                 				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,16
_0x4CF:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2627 	                        }
_0x20A:
;    2628 			}else{
	RJMP _0x217
_0x202:
;    2629        			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
	CALL SUBOPT_0xBA
	__POINTW2MN _rprbdata,576
	CALL SUBOPT_0x2A
	ST   -Y,R30
	RCALL _getprbtype
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2630         			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2631         				(rundata.temperature[curr_dispch - 1] < -9000) ||\
;    2632 	        			(cptr[0] != 'P'))
	CALL SUBOPT_0xBA
	CPI  R30,LOW(0xFF)
	BREQ _0x219
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRLO _0x219
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BREQ _0x218
_0x219:
;    2633 	        		{
;    2634 	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
	CALL SUBOPT_0xBA
	CPI  R30,LOW(0xFF)
	BRNE _0x21B
;    2635                 				sprintf(strbuf," ------- ohm");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,220
	CALL SUBOPT_0x33
;    2636                 			if(rundata.temperature[curr_dispch - 1] < -9000)
_0x21B:
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x21C
;    2637                         			sprintf(strbuf," NO READ    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,233
	RJMP _0x4D0
;    2638                                         else
_0x21C:
;    2639                                         	sprintf(strbuf," -------    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,246
_0x4D0:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2640                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x21F
	CPI  R21,1
	BRNE _0x21E
_0x21F:
;    2641                                         {
;    2642         	        			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x38
;    2643                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2644 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4D1
;    2645 		                	}else{
_0x21E:
;    2646         	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2647                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2648 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4D1:
	ST   -Y,R30
	CALL _draw_label
;    2649 		                	}
;    2650 			        }else{
	RJMP _0x222
_0x218:
;    2651 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB1
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2652 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xAD
	BRNE _0x223
;    2653         					sprintf(strbuf+8,"*ohm");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,259
	RJMP _0x4D2
;    2654                                         else
_0x223:
;    2655         					sprintf(strbuf+8," ohm");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,228
_0x4D2:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2656                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x226
	CPI  R21,1
	BRNE _0x225
_0x226:
;    2657                                         {
;    2658 	        			        draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x38
;    2659         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2660 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2661 		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4D3
;    2662 		        		 }else{
_0x225:
;    2663 	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2664         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2665 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2666 		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4D3:
	ST   -Y,R30
	CALL _draw_label
;    2667 		        		 }
;    2668 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xB7
	BRNE _0x229
;    2669         				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4D4
;    2670         				else                                     //Hz12
_0x229:
;    2671                 				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,16
_0x4D4:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2672         			}
_0x222:
;    2673 	                }
_0x217:
;    2674 			INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x22B
	CALL SUBOPT_0x3D
_0x22B:
;    2675 		}
	SUBI R19,-1
	RJMP _0x200
_0x201:
;    2676 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2677 	}
;    2678 }
_0x1FE:
_0x4B0:
	CALL __LOADLOCR6
	ADIW R28,9
	RET
;    2679 
;    2680 //main configuration window of R
;    2681 LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
;    2682 LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
;    2683 LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
;    2684 LABEL flash pgr_options[] = {
;    2685 	{LBL_HZ16, 10,23,7,"1.电流换向"},
;    2686 	{LBL_HZ16,130,23,7,"2.内标准校准"},
;    2687 	{LBL_HZ16, 10,45,7,"3.通道探头选择"},
;    2688 	{LBL_HZ16,130,45,7,"4.设置探头参数"}
;    2689 };
;    2690 
;    2691 //LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2692 #define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
;    2693 void pgcalibrate()
;    2694 {
_pgcalibrate:
;    2695         double oldvalue = 0;
;    2696         window_setup(10);
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x232*2)
	LDI  R31,HIGH(_0x232*2)
	CALL __INITLOCB
;	oldvalue -> Y+0
	CALL SUBOPT_0xBB
;    2697 	sprintf(strbuf,"请输入外标准(CH1)阻值");
	__POINTW1FN _0,349
	CALL SUBOPT_0x33
;    2698 	oldvalue = wnd_floatinput(oldvalue);
	CALL SUBOPT_0x2
	CALL SUBOPT_0xBC
	__PUTD1S 0
;    2699 	if(oldvalue == 0)
	CALL SUBOPT_0x2
	CALL __CPD10
	BRNE _0x233
;    2700                 return;
	RJMP _0x4AF
;    2701         wnd_msgbox(&pgr_calibrate);
_0x233:
	LDI  R30,LOW(_pgr_calibrate*2)
	LDI  R31,HIGH(_pgr_calibrate*2)
	CALL SUBOPT_0x3A
;    2702         if(rprbdata.type[sysdata.rid[curr_dispch-1]] == PRBTYPE_PT1000)
	CALL SUBOPT_0xBA
	__POINTW2MN _rprbdata,576
	CALL SUBOPT_0x2A
	CPI  R30,LOW(0xF3)
	BRNE _0x234
;    2703                 sysdata.Rs2 = (oldvalue + sysdata.R0)*sysdata.Rs2/(rundata.reading[curr_dispch-1]+sysdata.R0);
	__POINTW1MN _sysdata,64
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x1
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	__POINTW2MN _sysdata,64
	CALL __EEPROMRDD
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x45
	CALL SUBOPT_0x2E
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
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
	RJMP _0x4D5
;    2704         else
_0x234:
;    2705                 sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);
	__POINTW1MN _sysdata,8
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
	CALL SUBOPT_0x1
	CALL __ADDF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	__POINTW2MN _sysdata,8
	CALL __EEPROMRDD
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x45
	CALL SUBOPT_0x2E
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x2C
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
_0x4D5:
	CALL __EEPROMWRD
;    2706 
;    2707         if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
	BREQ PC+4
	BRCS PC+3
	JMP  _0x237
	CALL SUBOPT_0x5
	CALL SUBOPT_0x7
	BRSH _0x236
_0x237:
;    2708         {
;    2709                 sysdata.Rs1 = 100;
	CALL SUBOPT_0x8
;    2710         }
;    2711         if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
_0x236:
	CALL SUBOPT_0x9
	CALL SUBOPT_0xA
	BREQ PC+4
	BRCS PC+3
	JMP  _0x23A
	CALL SUBOPT_0x9
	CALL SUBOPT_0xB
	BRSH _0x239
_0x23A:
;    2712         {
;    2713                 sysdata.Rs2 = 1000;
	CALL SUBOPT_0xC
;    2714         }
;    2715 }
_0x239:
_0x4AF:
	ADIW R28,4
	RET
;    2716 //main menu of bore config
;    2717 void pgrconfig_handler(uchar msg) {
_pgrconfig_handler:
;    2718 	uchar i;
;    2719 
;    2720 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2721 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2722 	uchar min_option = 1;
;    2723 	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);
;    2724 
;    2725 	if(msg == KEY_TAB) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BRNE _0x23E
;    2726 	        SET_BORE_MODE;
	CALL SUBOPT_0x3B
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3C
;    2727 	        SET_TORS;
	CALL SUBOPT_0x1E
;    2728 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2729 		return;
	RJMP _0x4AE
;    2730 	}
;    2731 	if(msg == KEY_CE)
_0x23E:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x23F
;    2732 	{
;    2733 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2734 	        return;
	RJMP _0x4AE
;    2735 	}
;    2736 	if(msg == MSG_INIT) {
_0x23F:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x240
;    2737 		LCD_Cls();
	CALL _LCD_Cls
;    2738 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S4C,R30
;    2739 		draw_label(&pgr_banner, SW_NORMAL);
	LDI  R30,LOW(_pgr_banner*2)
	LDI  R31,HIGH(_pgr_banner*2)
	CALL SUBOPT_0x38
;    2740 		INIT_OPTIONS(pgr_);
	MOV  R17,R16
_0x242:
	CP   R19,R17
	BRLO _0x243
	CALL SUBOPT_0xBD
	CALL SUBOPT_0x38
	SUBI R17,-1
	RJMP _0x242
_0x243:
;    2741 		sprintf(strbuf,"(%i)",sysdata.ktime);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,371
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	CALL SUBOPT_0x35
	CALL SUBOPT_0xB2
;    2742 		draw_label(&pgr_klbl, SW_NORMAL);
	LDI  R30,LOW(_pgr_klbl*2)
	LDI  R31,HIGH(_pgr_klbl*2)
	CALL SUBOPT_0x38
;    2743 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2744 	}
;    2745 	if(msg == KEY_DN)
_0x240:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x244
;    2746 	{
;    2747 	        last_sel = curr_sel;
	CALL SUBOPT_0xBE
;    2748 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S4C,R30
;    2749 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S4C
	CP   R19,R26
	BRSH _0x245
;    2750 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S4C,R30
;    2751 	        msg = MSG_REFRESH;
_0x245:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2752 	}
;    2753 	if(msg == KEY_UP)
_0x244:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x246
;    2754 	{
;    2755 	        last_sel = curr_sel;
	CALL SUBOPT_0xBE
;    2756 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S4C,R30
;    2757 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x247
;    2758 	                curr_sel = max_option;
	STS  _curr_sel_S4C,R19
;    2759 	        msg = MSG_REFRESH;
_0x247:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2760 	}
;    2761 	if(msg == KEY_OK)
_0x246:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x248
;    2762 	{
;    2763 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S4C
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    2764 	}
;    2765 	if(msg == KEY_NUM1) {
_0x248:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x249
;    2766 		window_setup(4); //4 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xBF
;    2767 		sprintf(strbuf,"电流换向时间设置");
	__POINTW1FN _0,376
	CALL SUBOPT_0x33
;    2768 		sysdata.ktime = wnd_intinput(sysdata.ktime);
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _wnd_intinput
	__POINTW2MN _sysdata,12
	LDI  R31,0
	CALL __EEPROMWRW
;    2769 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2770 		return;
	RJMP _0x4AE
;    2771 	}
;    2772 	if(msg == KEY_NUM2)
_0x249:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x24A
;    2773 	{
;    2774                 pgcalibrate();
	CALL _pgcalibrate
;    2775 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2776 		return;
	RJMP _0x4AE
;    2777 	}
;    2778 	if(msg == KEY_NUM3) {
_0x24A:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x24B
;    2779 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2780 		return;
	RJMP _0x4AE
;    2781 	}
;    2782 	if(msg == KEY_NUM4) {
_0x24B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x24C
;    2783 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2784 		return;
	RJMP _0x4AE
;    2785 	}
;    2786 	if(msg == MSG_REFRESH) {
_0x24C:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x24D
;    2787 		REFRESH_OPTIONS(pgr_);
	LDS  R30,_curr_sel_S4C
	LDS  R26,_last_sel_S4C
	CP   R30,R26
	BRNE _0x24E
	RJMP _0x4AE
_0x24E:
	MOV  R17,R16
_0x250:
	CP   R19,R17
	BRLO _0x251
	LDS  R26,_last_sel_S4C
	CP   R17,R26
	BRNE _0x252
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xC0
_0x252:
	LDS  R26,_curr_sel_S4C
	CP   R17,R26
	BRNE _0x253
	CALL SUBOPT_0xBD
	CALL SUBOPT_0xC0
_0x253:
	SUBI R17,-1
	RJMP _0x250
_0x251:
;    2788 	}
;    2789 }
_0x24D:
	RJMP _0x4AE
;    2790 //main configuration window of T
;    2791 LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
;    2792 LABEL flash pgt_options[] = {
;    2793 	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
;    2794 	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
;    2795 };
;    2796 
;    2797 //main menu of thermo config
;    2798 void pgtconfig_handler(uchar msg) {
_pgtconfig_handler:
;    2799 	uchar i;
;    2800 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2801 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2802 	uchar min_option = 1;
;    2803 	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
;    2804 	if(msg == KEY_TAB) {
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,2
	LDD  R26,Y+4
	CPI  R26,LOW(0x54)
	BRNE _0x259
;    2805 	        SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    2806 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2807 		return;
	RJMP _0x4AE
;    2808 	}
;    2809 	if(msg == KEY_CE)
_0x259:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x25A
;    2810 	{
;    2811 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2812 	        return;
	RJMP _0x4AE
;    2813 	}
;    2814 	if(msg == MSG_INIT) {
_0x25A:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x25B
;    2815 		LCD_Cls();
	CALL _LCD_Cls
;    2816 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S4D,R30
;    2817 		draw_label(&pgt_banner, SW_NORMAL);
	LDI  R30,LOW(_pgt_banner*2)
	LDI  R31,HIGH(_pgt_banner*2)
	CALL SUBOPT_0x38
;    2818 		INIT_OPTIONS(pgt_);
	MOV  R17,R16
_0x25D:
	CP   R19,R17
	BRLO _0x25E
	CALL SUBOPT_0xC2
	CALL SUBOPT_0x38
	SUBI R17,-1
	RJMP _0x25D
_0x25E:
;    2819 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2820 	}
;    2821 	if(msg == KEY_DN)
_0x25B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x25F
;    2822 	{
;    2823 	        last_sel = curr_sel;
	CALL SUBOPT_0xC3
;    2824 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S4D,R30
;    2825 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S4D
	CP   R19,R26
	BRSH _0x260
;    2826 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S4D,R30
;    2827 	        msg = MSG_REFRESH;
_0x260:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2828 	}
;    2829 	if(msg == KEY_UP)
_0x25F:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x261
;    2830 	{
;    2831 	        last_sel = curr_sel;
	CALL SUBOPT_0xC3
;    2832 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S4D,R30
;    2833 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x262
;    2834 	                curr_sel = max_option;
	STS  _curr_sel_S4D,R19
;    2835 	        msg = MSG_REFRESH;
_0x262:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2836 	}
;    2837 	if(msg == KEY_OK)
_0x261:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x263
;    2838 	{
;    2839 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S4D
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    2840 	}
;    2841 	if(msg == KEY_NUM1) {
_0x263:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x264
;    2842 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2843 		return;
	RJMP _0x4AE
;    2844 	}
;    2845 	if(msg == KEY_NUM2) {
_0x264:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x265
;    2846 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2847 		return;
	RJMP _0x4AE
;    2848 	}
;    2849 	if(msg == MSG_REFRESH) {
_0x265:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x266
;    2850 		REFRESH_OPTIONS(pgt_);
	LDS  R30,_curr_sel_S4D
	LDS  R26,_last_sel_S4D
	CP   R30,R26
	BRNE _0x267
	RJMP _0x4AE
_0x267:
	MOV  R17,R16
_0x269:
	CP   R19,R17
	BRLO _0x26A
	LDS  R26,_last_sel_S4D
	CP   R17,R26
	BRNE _0x26B
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xC0
_0x26B:
	LDS  R26,_curr_sel_S4D
	CP   R17,R26
	BRNE _0x26C
	CALL SUBOPT_0xC2
	CALL SUBOPT_0xC0
_0x26C:
	SUBI R17,-1
	RJMP _0x269
_0x26A:
;    2851 	}
;    2852 	return;
_0x266:
_0x4AE:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2853 }
;    2854 //channel probe setup
;    2855 LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
;    2856 LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
;    2857 LABEL flash pgch_items[] = {
;    2858 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2859 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2860 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2861 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2862 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2863 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2864 };
;    2865 
;    2866 //select the channel to config
;    2867 void pgchset_handler(uchar msg) {
_pgchset_handler:
;    2868 	static uint curr_pos = 1; //absolute postion in options 1-24

	.DSEG
_curr_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2869 	static uint last_pos = 1;

	.DSEG
_last_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2870 	uchar min_option = 1;
;    2871 	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
;    2872 	uchar min_index = 1;
;    2873 	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
;    2874 	uchar new_page = 0;
;    2875 	int i,j;
;    2876 	if(msg == KEY_CE) {
	SBIW R28,4
	CALL SUBOPT_0xC4
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
	BRNE _0x270
;    2877 		if(IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x271
;    2878 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	RJMP _0x4D6
;    2879 		else
_0x271:
;    2880 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
_0x4D6:
	MOV  R4,R30
;    2881 		return;
	RJMP _0x4AD
;    2882 	}
;    2883 
;    2884 	if(msg == KEY_UP) {
_0x270:
	LDD  R26,Y+10
	CPI  R26,LOW(0x55)
	BRNE _0x273
;    2885 		DEC_TABLE;
	CALL SUBOPT_0xC5
	CALL SUBOPT_0xC6
	SBIW R30,1
	CALL SUBOPT_0xC7
	MOV  R30,R19
	CALL SUBOPT_0xC8
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x274
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xC7
_0x274:
	CALL SUBOPT_0xCA
	BRNE _0x275
	RJMP _0x4AD
_0x275:
	MOV  R30,R16
	CALL SUBOPT_0xC8
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x276
	LDI  R21,LOW(1)
	RJMP _0x277
_0x276:
	LDI  R21,LOW(0)
_0x277:
;    2886 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2887 	}
;    2888 	if(msg == KEY_DN) {
_0x273:
	LDD  R26,Y+10
	CPI  R26,LOW(0x44)
	BRNE _0x278
;    2889 		INC_TABLE;
	CALL SUBOPT_0xC5
	CALL SUBOPT_0xC6
	ADIW R30,1
	CALL SUBOPT_0xC7
	MOV  R30,R18
	CALL SUBOPT_0xCB
	CALL SUBOPT_0xA9
	BRSH _0x279
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xC7
_0x279:
	CALL SUBOPT_0xCA
	BRNE _0x27A
	RJMP _0x4AD
_0x27A:
	MOV  R30,R16
	CALL SUBOPT_0xC8
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x27B
	LDI  R21,LOW(1)
	RJMP _0x27C
_0x27B:
	LDI  R21,LOW(0)
_0x27C:
;    2890 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2891 	}
;    2892 	if(msg == KEY_OK) {	//show window to select the probe
_0x278:
	LDD  R26,Y+10
	CPI  R26,LOW(0x4F)
	BRNE _0x27D
;    2893 		curr_ch = curr_pos;
	__GETWRMN 6,7,0,_curr_pos_S4E
;    2894 		nextwin = PG_PRBLIST;
	LDI  R30,LOW(8)
	MOV  R4,R30
;    2895 		return;
	RJMP _0x4AD
;    2896 	}
;    2897 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x27D:
	LDD  R26,Y+10
	CPI  R26,LOW(0x30)
	BRLO _0x27F
	CPI  R26,LOW(0x3A)
	BRLO _0x280
_0x27F:
_0x280:
;    2898 	/*
;    2899 		KEY_TABLE;
;    2900 		msg = MSG_REFRESH;
;    2901 	*/
;    2902 	}
;    2903 	if(msg == KEY_DOT)
	LDD  R26,Y+10
	CPI  R26,LOW(0x2E)
	BRNE _0x281
;    2904 	{
;    2905 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x282
;    2906 	        {
;    2907 		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4D7
;    2908 	        }else{
_0x282:
;    2909        		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4D7:
	LDS  R30,_curr_pos_S4E
	LDS  R31,_curr_pos_S4E+1
	CALL SUBOPT_0xCD
;    2910 	        }
;    2911 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2912 		new_page = 1; //refresh the whole page
	LDI  R21,LOW(1)
;    2913 	}
;    2914 	if(msg == MSG_INIT) {
_0x281:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFF)
	BRNE _0x284
;    2915 		LCD_Cls();
	CALL _LCD_Cls
;    2916 		draw_label(&pgch_banner, SW_NORMAL);
	LDI  R30,LOW(_pgch_banner*2)
	LDI  R31,HIGH(_pgch_banner*2)
	CALL SUBOPT_0x38
;    2917 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xC7
;    2918 		new_page = 1;
	LDI  R21,LOW(1)
;    2919 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2920 	}
;    2921 	if(msg == MSG_REFRESH) {
_0x284:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x285
;    2922 
;    2923 		REFRESH_TABLES1(pgch_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xC5
	CALL SUBOPT_0x37
	CALL SUBOPT_0xB2
	CPI  R21,1
	BREQ PC+3
	JMP _0x286
	CALL SUBOPT_0xCF
_0x288:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0xA9
	BRGE PC+3
	JMP _0x289
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xD1
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADD  R30,R26
	ADC  R31,R27
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2924 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x28A
;    2925         	        {
;    2926 				if(sysdata.tid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xD2
	BRNE _0x28B
;    2927 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xB2
;    2928 				}else{
	RJMP _0x28C
_0x28B:
;    2929 			        	//draw line item j
;    2930 				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD4
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xD5
	CALL _tname2b
	CALL SUBOPT_0x37
	CALL SUBOPT_0xD6
;    2931 				}
_0x28C:
;    2932 		        }else{
	RJMP _0x28D
_0x28A:
;    2933 				if(sysdata.rid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xD2
	BRNE _0x28E
;    2934 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xB2
;    2935 				}else{
	RJMP _0x28F
_0x28E:
;    2936 			        	//draw line item j
;    2937 				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD4
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xD5
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xD6
;    2938 				}
_0x28F:
;    2939 		        }
_0x28D:
;    2940 		REFRESH_TABLES2(pgch_);
	CALL SUBOPT_0xD8
	CALL SUBOPT_0x38
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xD9
	BRNE _0x290
	CALL SUBOPT_0xD8
	CALL SUBOPT_0xC0
_0x290:
	CALL SUBOPT_0xDA
	RJMP _0x288
_0x289:
	RJMP _0x291
_0x286:
	CALL SUBOPT_0xCF
_0x293:
	MOV  R30,R16
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	CALL SUBOPT_0xA9
	BRLT _0x294
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xD9
	BRNE _0x295
	CALL SUBOPT_0xD8
	CALL SUBOPT_0xC0
_0x295:
	CALL SUBOPT_0xD0
	CALL SUBOPT_0xD9
	BRNE _0x296
	CALL SUBOPT_0xD8
	CALL SUBOPT_0xC0
_0x296:
	CALL SUBOPT_0xDA
	RJMP _0x293
_0x294:
_0x291:
;    2941 	}
;    2942 	return;
_0x285:
_0x4AD:
	CALL __LOADLOCR6
	ADIW R28,11
	RET
;    2943 }
;    2944 //select probe to edit.
;    2945 LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
;    2946 LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2947 LABEL flash pgprbset_items[] = {
;    2948 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2949 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2950 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2951 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2952 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2953 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2954 };
;    2955 //select probe to config
;    2956 void pgprbset_handler(uchar msg) {
_pgprbset_handler:
;    2957 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2958 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2959 	static uint last_pos = 1;

	.DSEG
_last_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2960 	uchar min_option = 1;
;    2961 	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
;    2962 	uchar min_index = 1;
;    2963 	uchar max_index = MAX_PROBE_NUM;
;    2964 	int i,j;
;    2965 	uchar new_page = 1;
;    2966 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,3
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x29A*2)
	LDI  R31,HIGH(_0x29A*2)
	CALL __INITLOCB
	CALL SUBOPT_0xC4
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
	BRLO _0x29C
	CPI  R26,LOW(0x3A)
	BRLO _0x29D
_0x29C:
_0x29D:
;    2967 	/*
;    2968 		KEY_TABLE;
;    2969 		msg = MSG_REFRESH;
;    2970 	*/
;    2971 	}
;    2972 
;    2973 	if(msg == KEY_CE)
	LDD  R26,Y+9
	CPI  R26,LOW(0x43)
	BRNE _0x29E
;    2974 	{
;    2975 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x29F
;    2976 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4D8
;    2977 		else
_0x29F:
;    2978 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4D8:
	MOV  R4,R30
;    2979 		return;
	RJMP _0x4AC
;    2980 	}
;    2981 	if(msg == KEY_OK)
_0x29E:
	LDD  R26,Y+9
	CPI  R26,LOW(0x4F)
	BRNE _0x2A1
;    2982 	{
;    2983 		curr_prb = curr_pos;
	__GETWRMN 8,9,0,_curr_pos_S4F
;    2984 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2985 		return;
	RJMP _0x4AC
;    2986 	}
;    2987 	if(msg == MSG_INIT)
_0x2A1:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFF)
	BRNE _0x2A2
;    2988 	{
;    2989 		LCD_Cls();
	CALL _LCD_Cls
;    2990 		draw_label(&pgprbset_banner, SW_NORMAL);
	LDI  R30,LOW(_pgprbset_banner*2)
	LDI  R31,HIGH(_pgprbset_banner*2)
	CALL SUBOPT_0x38
;    2991 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xDC
;    2992 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2993 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2994 	}
;    2995 	if(msg == KEY_UP) {
_0x2A2:
	LDD  R26,Y+9
	CPI  R26,LOW(0x55)
	BRNE _0x2A3
;    2996 		DEC_TABLE;
	CALL SUBOPT_0xDD
	CALL SUBOPT_0xDE
	SBIW R30,1
	CALL SUBOPT_0xDC
	MOV  R30,R19
	CALL SUBOPT_0xDF
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x2A4
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xDC
_0x2A4:
	CALL SUBOPT_0xE0
	BRNE _0x2A5
	RJMP _0x4AC
_0x2A5:
	MOV  R30,R16
	CALL SUBOPT_0xDF
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x2A6
	LDI  R30,LOW(1)
	RJMP _0x4D9
_0x2A6:
	LDI  R30,LOW(0)
_0x4D9:
	STD  Y+6,R30
;    2997 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2998 	}
;    2999 	if(msg == KEY_DN) {
_0x2A3:
	LDD  R26,Y+9
	CPI  R26,LOW(0x44)
	BRNE _0x2A8
;    3000 		INC_TABLE;
	CALL SUBOPT_0xDD
	CALL SUBOPT_0xDE
	ADIW R30,1
	CALL SUBOPT_0xDC
	MOV  R30,R18
	CALL SUBOPT_0xE1
	CALL SUBOPT_0xA9
	BRSH _0x2A9
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xDC
_0x2A9:
	CALL SUBOPT_0xE0
	BRNE _0x2AA
	RJMP _0x4AC
_0x2AA:
	MOV  R30,R16
	CALL SUBOPT_0xDF
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x2AB
	LDI  R30,LOW(1)
	RJMP _0x4DA
_0x2AB:
	LDI  R30,LOW(0)
_0x4DA:
	STD  Y+6,R30
;    3001 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    3002 	}
;    3003 	if(msg == MSG_REFRESH) {
_0x2A8:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2AD
;    3004 		REFRESH_TABLES1(pgprbset_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xDD
	CALL SUBOPT_0x37
	CALL SUBOPT_0xB2
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x2AE
	MOV  R20,R17
	CLR  R21
_0x2B0:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0xA9
	BRLT _0x2B1
	CALL SUBOPT_0xE2
	CALL SUBOPT_0xD1
	ADD  R30,R20
	ADC  R31,R21
	STD  Y+7,R30
	STD  Y+7+1,R31
;    3005 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2B2
;    3006 		        {
;    3007 			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xE3
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,1
	ST   -Y,R30
	CALL _tname2b
	RJMP _0x4DB
;    3008 		        }else{
_0x2B2:
;    3009         		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xE3
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,1
	ST   -Y,R30
	CALL _rname2b
_0x4DB:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	CALL SUBOPT_0xD6
;    3010 		        }
;    3011         		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xE4
;    3012 		REFRESH_TABLES2(pgprbset_);
	CALL SUBOPT_0xE4
	CALL SUBOPT_0xE2
	CALL SUBOPT_0xE5
	BRNE _0x2B4
	CALL SUBOPT_0xE6
_0x2B4:
	__ADDWRN 20,21,1
	RJMP _0x2B0
_0x2B1:
	RJMP _0x2B5
_0x2AE:
	MOV  R20,R17
	CLR  R21
_0x2B7:
	MOV  R30,R16
	MOVW R26,R20
	CALL SUBOPT_0xA9
	BRLT _0x2B8
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xE5
	BRNE _0x2B9
	CALL SUBOPT_0xE6
_0x2B9:
	CALL SUBOPT_0xE2
	CALL SUBOPT_0xE5
	BRNE _0x2BA
	CALL SUBOPT_0xE6
_0x2BA:
	__ADDWRN 20,21,1
	RJMP _0x2B7
_0x2B8:
_0x2B5:
;    3013 	}
;    3014 	return;
_0x2AD:
_0x4AC:
	CALL __LOADLOCR6
	ADIW R28,10
	RET
;    3015 }
;    3016 //select probe for channel setup
;    3017 LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
;    3018 LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    3019 LABEL flash prblist_items[] = {
;    3020 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    3021 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    3022 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    3023 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    3024 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    3025 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    3026 };
;    3027 //select channel's probe
;    3028 void pgprblist_handler(uchar msg) {
_pgprblist_handler:
;    3029 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    3030 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S50:
	.BYTE 0x2

	.CSEG
;    3031 	static uint last_pos = 1;

	.DSEG
_last_pos_S50:
	.BYTE 0x2

	.CSEG
;    3032 	uchar min_option = 1;
;    3033 	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
;    3034 	uchar min_index = 1;
;    3035 	uchar max_index = MAX_PROBE_NUM;
;    3036 	uchar i,j;
;    3037 	uchar new_page = 1;
;    3038 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	SBIW R28,1
	LDI  R24,1
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x2BD*2)
	LDI  R31,HIGH(_0x2BD*2)
	CALL __INITLOCB
	CALL SUBOPT_0xC4
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
	BRLO _0x2BF
	CPI  R26,LOW(0x3A)
	BRLO _0x2C0
_0x2BF:
_0x2C0:
;    3039 	/*
;    3040 		KEY_TABLE;
;    3041 		msg = MSG_REFRESH;
;    3042 	*/
;    3043 	}
;    3044 	if(msg == KEY_CE)
	LDD  R26,Y+7
	CPI  R26,LOW(0x43)
	BRNE _0x2C1
;    3045 	{
;    3046 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    3047 		return;
	RJMP _0x4AB
;    3048 	}
;    3049 	if(msg == KEY_OK)
_0x2C1:
	LDD  R26,Y+7
	CPI  R26,LOW(0x4F)
	BRNE _0x2C2
;    3050 	{
;    3051 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2C3
;    3052 	        {
;    3053        	                if(tprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	BREQ _0x2C4
;    3054         		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xE9
	CALL __EEPROMWRB
;    3055 	        }else{
_0x2C4:
	RJMP _0x2C5
_0x2C3:
;    3056                         if(rprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	BREQ _0x2C6
;    3057 		                sysdata.rid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xE9
	CALL __EEPROMWRB
;    3058 	        }
_0x2C6:
_0x2C5:
;    3059 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    3060 		return;
	RJMP _0x4AB
;    3061 	}
;    3062 	if(msg == MSG_INIT)
_0x2C2:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFF)
	BRNE _0x2C7
;    3063 	{
;    3064 		LCD_Cls();
	CALL _LCD_Cls
;    3065 		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,502
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL SUBOPT_0x35
	CALL SUBOPT_0xB2
;    3066 		draw_label(&prblist_banner, SW_NORMAL);
	LDI  R30,LOW(_prblist_banner*2)
	LDI  R31,HIGH(_prblist_banner*2)
	CALL SUBOPT_0x38
;    3067 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xEA
;    3068 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    3069 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3070 	}
;    3071 	if(msg == KEY_UP) {
_0x2C7:
	LDD  R26,Y+7
	CPI  R26,LOW(0x55)
	BRNE _0x2C8
;    3072 		DEC_TABLE;
	CALL SUBOPT_0xEB
	STS  _last_pos_S50,R30
	STS  _last_pos_S50+1,R31
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xEA
	MOV  R30,R19
	CALL SUBOPT_0xEC
	CP   R26,R30
	CPC  R27,R31
	BRSH _0x2C9
	CALL SUBOPT_0xC9
	CALL SUBOPT_0xEA
_0x2C9:
	CALL SUBOPT_0xED
	BRNE _0x2CA
	RJMP _0x4AB
_0x2CA:
	MOV  R30,R16
	CALL SUBOPT_0xEC
	CALL __MODW21U
	SBIW R30,0
	BRNE _0x2CB
	LDI  R30,LOW(1)
	RJMP _0x4DC
_0x2CB:
	LDI  R30,LOW(0)
_0x4DC:
	STD  Y+6,R30
;    3073 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3074 	}
;    3075 	if(msg == KEY_DN) {
_0x2C8:
	LDD  R26,Y+7
	CPI  R26,LOW(0x44)
	BRNE _0x2CD
;    3076 		INC_TABLE;
	CALL SUBOPT_0xEB
	STS  _last_pos_S50,R30
	STS  _last_pos_S50+1,R31
	CALL SUBOPT_0xEB
	ADIW R30,1
	CALL SUBOPT_0xEA
	MOV  R30,R18
	CALL SUBOPT_0xEE
	CALL SUBOPT_0xA9
	BRSH _0x2CE
	CALL SUBOPT_0xCC
	CALL SUBOPT_0xEA
_0x2CE:
	CALL SUBOPT_0xED
	BRNE _0x2CF
	RJMP _0x4AB
_0x2CF:
	MOV  R30,R16
	CALL SUBOPT_0xEC
	CALL __MODW21U
	SBIW R30,1
	BRNE _0x2D0
	LDI  R30,LOW(1)
	RJMP _0x4DD
_0x2D0:
	LDI  R30,LOW(0)
_0x4DD:
	STD  Y+6,R30
;    3077 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3078 	}
;    3079 	if(msg == KEY_DOT){
_0x2CD:
	LDD  R26,Y+7
	CPI  R26,LOW(0x2E)
	BRNE _0x2D2
;    3080 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2D3
;    3081 	        {
;    3082 		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4DE
;    3083 	        }else{
_0x2D3:
;    3084 		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4DE:
	MOVW R30,R6
	CALL SUBOPT_0xCD
;    3085 	        }
;    3086        		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3087        		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    3088 	}
;    3089 	if(msg == MSG_REFRESH) {
_0x2D2:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2D5
;    3090 		REFRESH_TABLES1(prblist_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xEB
	CALL SUBOPT_0x37
	CALL SUBOPT_0xB2
	LDD  R26,Y+6
	CPI  R26,LOW(0x1)
	BRNE _0x2D6
	MOV  R21,R17
_0x2D8:
	CP   R16,R21
	BRLO _0x2D9
	CALL SUBOPT_0xEF
	CALL SUBOPT_0xD1
	MOVW R26,R30
	MOV  R30,R21
	LDI  R31,0
	ADD  R30,R26
	MOV  R20,R30
;    3091 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2DA
;    3092 		        {
;    3093 		                sprintf(strbuf,"%s",tname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xF0
	CALL _tname2b
	RJMP _0x4DF
;    3094 		        }else{
_0x2DA:
;    3095         		        sprintf(strbuf,"%s",rname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xF0
	CALL _rname2b
_0x4DF:
	CLR  R22
	CLR  R23
	CALL SUBOPT_0xF1
;    3096         		}
;    3097 			draw_label(&(prblist_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xF2
;    3098 		REFRESH_TABLES2(prblist_);
	CALL SUBOPT_0xF2
	CALL SUBOPT_0xEF
	CALL SUBOPT_0xF3
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2DC
	CALL SUBOPT_0xF4
_0x2DC:
	SUBI R21,-1
	RJMP _0x2D8
_0x2D9:
	RJMP _0x2DD
_0x2D6:
	MOV  R21,R17
_0x2DF:
	CP   R16,R21
	BRLO _0x2E0
	LDS  R26,_last_pos_S50
	LDS  R27,_last_pos_S50+1
	CALL SUBOPT_0xDB
	CALL SUBOPT_0xF3
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2E1
	CALL SUBOPT_0xF4
_0x2E1:
	CALL SUBOPT_0xEF
	CALL SUBOPT_0xF3
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x2E2
	CALL SUBOPT_0xF4
_0x2E2:
	SUBI R21,-1
	RJMP _0x2DF
_0x2E0:
_0x2DD:
;    3099 	}
;    3100 	return;
_0x2D5:
_0x4AB:
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;    3101 }
;    3102 //select probe type list
;    3103 LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
;    3104 LABEL flash tplist_options[] = {
;    3105 	{LBL_HZ6X8,10,20,8,"0.PT1000"},
;    3106 	{LBL_HZ6X8,80,20,8,"1.PT100"},
;    3107 	{LBL_HZ6X8,150,20,8,"2.PT25"},
;    3108 	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
;    3109 	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
;    3110 	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
;    3111 	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
;    3112 	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
;    3113 	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
;    3114         {LBL_HZ6X8,10,50,8,"9.T-TYPE"},
;    3115 	{LBL_HZ6X8,10,50,8,"*.R-TYPE"}
;    3116 
;    3117 };
;    3118 //select probe type thermo
;    3119 void pgprbtypelist_handler(uchar msg) {
_pgprbtypelist_handler:
;    3120 	static int curr_sel = 1;

	.DSEG
_curr_sel_S51:
	.BYTE 0x2

	.CSEG
;    3121 	static int last_sel = 0xff;

	.DSEG
_last_sel_S51:
	.BYTE 0x2

	.CSEG
;    3122 	uchar min_option = 1;
;    3123 	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
;    3124 	uchar i;
;    3125 	if(msg == KEY_DOT)
	CALL __SAVELOCR4
;	msg -> Y+4
;	min_option -> R17
;	max_option -> R16
;	i -> R19
	LDI  R16,11
	LDI  R17,1
	LDD  R26,Y+4
	CPI  R26,LOW(0x2E)
	BRNE _0x2F0
;    3126 	{
;    3127 	        curr_sel = 10;
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL SUBOPT_0xF5
;    3128 	        msg = KEY_OK;
;    3129 	}
;    3130 	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
_0x2F0:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRLO _0x2F2
	CPI  R26,LOW(0x3A)
	BRLO _0x2F3
_0x2F2:
	RJMP _0x2F1
_0x2F3:
;    3131 	        curr_sel = (u8)(msg - KEY_NUM1 + 1) ;
	LDD  R30,Y+4
	SUBI R30,LOW(49)
	SUBI R30,-LOW(1)
	LDI  R31,0
	CALL SUBOPT_0xF5
;    3132 	        msg = KEY_OK;
;    3133 	}
;    3134         if (msg == KEY_NUM0)
_0x2F1:
	LDD  R26,Y+4
	CPI  R26,LOW(0x30)
	BRNE _0x2F4
;    3135         {
;    3136                 curr_sel = 0;
	LDI  R30,0
	STS  _curr_sel_S51,R30
	STS  _curr_sel_S51+1,R30
;    3137                 msg = KEY_OK;
	LDI  R30,LOW(79)
	STD  Y+4,R30
;    3138         }
;    3139 	if(msg == KEY_CE ) {
_0x2F4:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x2F5
;    3140 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3141 		return;
	RJMP _0x4AA
;    3142 	}
;    3143 	if(msg == KEY_OK)
_0x2F5:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BREQ PC+3
	JMP _0x2F6
;    3144 	{
;    3145 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2F7
;    3146 	        {
;    3147         		switch(curr_sel)
	CALL SUBOPT_0xF6
;    3148 	        	{
;    3149 	        	        case 0:
	SBIW R30,0
	BRNE _0x2FB
;    3150 	        	               return;
	RJMP _0x4AA
;    3151 		        	case 1:
_0x2FB:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x2FC
;    3152         			       return;
	RJMP _0x4AA
;    3153 	        		case 2:
_0x2FC:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x2FE
;    3154 			               return;
	RJMP _0x4AA
;    3155         			default:
_0x2FE:
;    3156 	        		       tprbdata.type[curr_prb-1] = curr_sel - 3+PRBTYPE_K;
	CALL SUBOPT_0xF7
	CALL SUBOPT_0xF6
	SBIW R30,3
	ADIW R30,3
	CALL __EEPROMWRB
;    3157 		        	       break;
;    3158         		}
;    3159         	}else{
	RJMP _0x2FF
_0x2F7:
;    3160         		switch(curr_sel)
	CALL SUBOPT_0xF6
;    3161 	        	{
;    3162 	        	        case 0:
	SBIW R30,0
	BRNE _0x303
;    3163 	        	               rprbdata.type[curr_prb-1] = PRBTYPE_PT1000;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(243)
	CALL __EEPROMWRB
;    3164 	        	               break;
	RJMP _0x302
;    3165 		        	case 1:
_0x303:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x304
;    3166 			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(241)
	CALL __EEPROMWRB
;    3167         			       break;
	RJMP _0x302
;    3168 	        		case 2:
_0x304:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x306
;    3169                        	               sprintf(strbuf,"输入Rtp:");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,621
	CALL SUBOPT_0x33
;    3170         	                       rprbdata.rtp[curr_prb-1] = wnd_floatinput(rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x8C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x31
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3171 		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(242)
	CALL __EEPROMWRB
;    3172 			               break;
	RJMP _0x302
;    3173         			default:
_0x306:
;    3174                                        return;
	RJMP _0x4AA
;    3175         		}
_0x302:
;    3176         	}
_0x2FF:
;    3177 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3178 		return;
	RJMP _0x4AA
;    3179 	}
;    3180 	if(msg == MSG_INIT)
_0x2F6:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x307
;    3181 	{
;    3182 		LCD_Cls();
	CALL _LCD_Cls
;    3183 		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,630
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3184 		draw_label(&tplist_banner, SW_NORMAL);
	LDI  R30,LOW(_tplist_banner*2)
	LDI  R31,HIGH(_tplist_banner*2)
	CALL SUBOPT_0x38
;    3185 		INIT_OPTIONS(tplist_);
	MOV  R19,R17
_0x309:
	CP   R16,R19
	BRLO _0x30A
	CALL SUBOPT_0xFC
	CALL SUBOPT_0x38
	SUBI R19,-1
	RJMP _0x309
_0x30A:
;    3186 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3187 	}
;    3188 	if(msg == KEY_UP) {
_0x307:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x30B
;    3189 		DEC_OPTION;
	CALL SUBOPT_0xFD
	SBIW R30,1
	CALL SUBOPT_0xFE
	MOV  R30,R17
	CALL SUBOPT_0xFF
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	BRGE _0x30C
	CALL SUBOPT_0x100
	CALL SUBOPT_0xFE
;    3190 		msg = MSG_REFRESH;
_0x30C:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3191 	}
;    3192 	if(msg == KEY_DN) {
_0x30B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x30D
;    3193 		INC_OPTION;
	CALL SUBOPT_0xFD
	ADIW R30,1
	CALL SUBOPT_0xFE
	MOV  R30,R16
	CALL SUBOPT_0xFF
	CALL SUBOPT_0xA9
	BRGE _0x30E
	MOV  R30,R17
	LDI  R31,0
	CALL SUBOPT_0xFE
;    3194 		msg = MSG_REFRESH;
_0x30E:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3195 	}
;    3196 	if(msg == MSG_REFRESH) {
_0x30D:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x30F
;    3197 		REFRESH_OPTIONS(tplist_);
	CALL SUBOPT_0xF6
	LDS  R26,_last_sel_S51
	LDS  R27,_last_sel_S51+1
	CP   R30,R26
	CPC  R31,R27
	BRNE _0x310
	RJMP _0x4AA
_0x310:
	MOV  R19,R17
_0x312:
	CP   R16,R19
	BRLO _0x313
	MOV  R30,R19
	LDS  R26,_last_sel_S51
	LDS  R27,_last_sel_S51+1
	CALL SUBOPT_0xA9
	BRNE _0x314
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xC0
_0x314:
	MOV  R30,R19
	CALL SUBOPT_0xFF
	CALL SUBOPT_0xA9
	BRNE _0x315
	CALL SUBOPT_0xFC
	CALL SUBOPT_0xC0
_0x315:
	SUBI R19,-1
	RJMP _0x312
_0x313:
;    3198 	}
;    3199 	return;
_0x30F:
_0x4AA:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    3200 }
;    3201 /*
;    3202  *       Convert the probe type const to string
;    3203  */
;    3204 char PRBSTR[7];

	.DSEG
_PRBSTR:
	.BYTE 0x7
;    3205 char* getprbtype(uchar prbtype)
;    3206 {

	.CSEG
_getprbtype:
;    3207 	switch(prbtype)
;	prbtype -> Y+0
	LD   R30,Y
;    3208 	{
;    3209 	        case PRBTYPE_PT1000:
	CPI  R30,LOW(0xF3)
	BRNE _0x319
;    3210 		        sprintf(PRBSTR,"PT1000");
	CALL SUBOPT_0x101
	__POINTW1FN _0,527
	CALL SUBOPT_0x33
;    3211 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3212 		case PRBTYPE_PT25:
_0x319:
	CPI  R30,LOW(0xF2)
	BRNE _0x31A
;    3213 		        sprintf(PRBSTR,"PT  25");
	CALL SUBOPT_0x101
	__POINTW1FN _0,648
	CALL SUBOPT_0x33
;    3214 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3215 		case PRBTYPE_PT100:
_0x31A:
	CPI  R30,LOW(0xF1)
	BRNE _0x31B
;    3216 		        sprintf(PRBSTR,"PT 100");
	CALL SUBOPT_0x101
	__POINTW1FN _0,655
	CALL SUBOPT_0x33
;    3217 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3218 		case PRBTYPE_T:
_0x31B:
	CPI  R30,LOW(0x9)
	BRNE _0x31C
;    3219 		        sprintf(PRBSTR,"T TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,662
	CALL SUBOPT_0x33
;    3220 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3221 
;    3222 		case PRBTYPE_K:
_0x31C:
	CPI  R30,LOW(0x3)
	BRNE _0x31D
;    3223 		        sprintf(PRBSTR,"K TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,669
	CALL SUBOPT_0x33
;    3224 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3225 		case PRBTYPE_N:
_0x31D:
	CPI  R30,LOW(0x4)
	BRNE _0x31E
;    3226 		        sprintf(PRBSTR,"N TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,676
	CALL SUBOPT_0x33
;    3227 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3228 		case PRBTYPE_E:
_0x31E:
	CPI  R30,LOW(0x5)
	BRNE _0x31F
;    3229 		        sprintf(PRBSTR,"E TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,683
	CALL SUBOPT_0x33
;    3230 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3231 		case PRBTYPE_B:
_0x31F:
	CPI  R30,LOW(0x6)
	BRNE _0x320
;    3232 		        sprintf(PRBSTR,"B TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,690
	CALL SUBOPT_0x33
;    3233 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3234 		case PRBTYPE_J:
_0x320:
	CPI  R30,LOW(0x7)
	BRNE _0x321
;    3235 		        sprintf(PRBSTR,"J TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,697
	CALL SUBOPT_0x33
;    3236 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3237 		case PRBTYPE_S:
_0x321:
	CPI  R30,LOW(0x8)
	BRNE _0x322
;    3238 		        sprintf(PRBSTR,"S TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,704
	CALL SUBOPT_0x33
;    3239 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3240 		case PRBTYPE_R:
_0x322:
	CPI  R30,LOW(0xA)
	BRNE _0x318
;    3241 		        sprintf(PRBSTR,"R TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,711
	CALL SUBOPT_0x33
;    3242 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3243 
;    3244 	}
_0x318:
;    3245 
;    3246 		        sprintf(PRBSTR,"? TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,718
	CALL SUBOPT_0x33
;    3247 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3248 
;    3249 }
;    3250 //config r probe parameter
;    3251 LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
;    3252 LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
;    3253 LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
;    3254 LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
;    3255 LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
;    3256 LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
;    3257 LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
;    3258 LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
;    3259 LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
;    3260 LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
;    3261 LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
;    3262 LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
;    3263 LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};
;    3264 
;    3265 //configuration of BORE probe parameter
;    3266 void pgprbconfig_handler(uchar msg) {
_pgprbconfig_handler:
;    3267 	if( msg == KEY_CE) {
;	msg -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x43)
	BRNE _0x32A
;    3268 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    3269 		return;
	RJMP _0x4A9
;    3270 	}
;    3271 	if(msg == MSG_INIT)
_0x32A:
	LD   R26,Y
	CPI  R26,LOW(0xFF)
	BREQ PC+3
	JMP _0x32B
;    3272 	{
;    3273 		LCD_Cls();
	CALL _LCD_Cls
;    3274 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x32C
;    3275 		{
;    3276 		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,750
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3277         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x38
;    3278 	        	//name
;    3279 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3280         		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0x103
	CALL _tname2b
	CALL SUBOPT_0x37
	CALL SUBOPT_0xB2
;    3281 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x38
;    3282         		//type
;    3283 	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0xF7
	CALL SUBOPT_0x104
	CALL SUBOPT_0xB2
;    3284 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	RJMP _0x4E0
;    3285 		}else{
_0x32C:
;    3286 		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,767
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3287         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x38
;    3288 	        	//name
;    3289 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3290 		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x32E
;    3291 		        {
;    3292         		        sprintf(strbuf,":%s",rname2b(curr_prb-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0x103
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xB2
;    3293         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	RJMP _0x4E1
;    3294         	        }else{  //PT1000 or PT100
_0x32E:
;    3295                		        sprintf(strbuf,":%s Rtp:%7f",rname2b(curr_prb-1),rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,784
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x103
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x31
	CALL __EEPROMRDD
	CALL __PUTPARD1
	CALL SUBOPT_0xD6
;    3296         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
_0x4E1:
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x38
;    3297         	        }
;    3298                		//type
;    3299                        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0xF8
	CALL SUBOPT_0x104
	CALL SUBOPT_0xB2
;    3300 	                draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	CALL SUBOPT_0x38
;    3301 
;    3302 		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x330
;    3303 		        {
;    3304 		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,796
	CALL SUBOPT_0x105
	CALL SUBOPT_0x106
;    3305         		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3b*2)
	LDI  R31,HIGH(_paramlbl3b*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_paramval3b*2)
	LDI  R31,HIGH(_paramval3b*2)
	RJMP _0x4E0
;    3306         		}else{
_0x330:
;    3307         		        //param1
;    3308                 		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,806
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x107
	CALL SUBOPT_0x106
;    3309 	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
	LDI  R30,LOW(_paramlbl1*2)
	LDI  R31,HIGH(_paramlbl1*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_paramval1*2)
	LDI  R31,HIGH(_paramval1*2)
	CALL SUBOPT_0x38
;    3310         	        	//param2
;    3311 	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,814
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x108
	CALL SUBOPT_0x31
	CALL SUBOPT_0x106
;    3312                 		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	LDI  R30,LOW(_paramlbl2*2)
	LDI  R31,HIGH(_paramlbl2*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_paramval2*2)
	LDI  R31,HIGH(_paramval2*2)
	CALL SUBOPT_0x38
;    3313 	                	//param3
;    3314 		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,822
	CALL SUBOPT_0x105
	CALL SUBOPT_0x106
;    3315         		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3*2)
	LDI  R31,HIGH(_paramlbl3*2)
	CALL SUBOPT_0x38
	LDI  R30,LOW(_paramval3*2)
	LDI  R31,HIGH(_paramval3*2)
_0x4E0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _draw_label
;    3316         		}
;    3317                 }
;    3318 		return;
	RJMP _0x4A9
;    3319 	}
;    3320 	if(msg == KEY_NUM1) {
_0x32B:
	LD   R26,Y
	CPI  R26,LOW(0x31)
	BRNE _0x332
;    3321 		window_setup(7);
	LDI  R30,LOW(7)
	CALL SUBOPT_0xBF
;    3322 		sprintf(strbuf,"请输入探头序列号");
	__POINTW1FN _0,830
	CALL SUBOPT_0x33
;    3323 		prbsninput();
	RCALL _prbsninput
;    3324 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3325 		return;
	RJMP _0x4A9
;    3326 	}
;    3327 	if(msg == KEY_NUM2){
_0x332:
	LD   R26,Y
	CPI  R26,LOW(0x32)
	BRNE _0x333
;    3328 		nextwin = PG_PRBTYPELIST;
	LDI  R30,LOW(9)
	MOV  R4,R30
;    3329 		return;
	RJMP _0x4A9
;    3330 	}
;    3331 	if(msg == KEY_NUM3 ){
_0x333:
	LD   R26,Y
	CPI  R26,LOW(0x33)
	BRNE _0x334
;    3332 
;    3333 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x335
;    3334 	        {
;    3335                		window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    3336 	                if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x336
;    3337 	                {
;    3338         	                sprintf(strbuf,"输入R(0!)");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,847
	CALL SUBOPT_0x33
;    3339         	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x109
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x107
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	RJMP _0x4E2
;    3340                         }else{
_0x336:
;    3341         	        	sprintf(strbuf,"输入系数a");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,857
	CALL SUBOPT_0x33
;    3342         	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	CALL SUBOPT_0x8C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x107
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
_0x4E2:
	CALL __EEPROMWRD
;    3343         	        }
;    3344         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3345         	}
;    3346 		return;
_0x335:
	RJMP _0x4A9
;    3347 	}
;    3348 	if(msg == KEY_NUM4) {
_0x334:
	LD   R26,Y
	CPI  R26,LOW(0x34)
	BRNE _0x338
;    3349         	if(IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x339
;    3350         	{
;    3351 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x33A
;    3352 	                        return;
	RJMP _0x4A9
;    3353 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
_0x33A:
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF3)
	BRNE _0x33B
;    3354 	                        return;
	RJMP _0x4A9
;    3355         		window_setup(10);
_0x33B:
	CALL SUBOPT_0xBB
;    3356 	        	sprintf(strbuf,"输入系数b");
	__POINTW1FN _0,867
	CALL SUBOPT_0x33
;    3357 		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x108
	CALL SUBOPT_0x8C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x108
	CALL SUBOPT_0x31
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3358         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3359                 }
;    3360 		return;
_0x339:
	RJMP _0x4A9
;    3361 	}
;    3362 	if(msg == KEY_NUM5) {
_0x338:
	LD   R26,Y
	CPI  R26,LOW(0x35)
	BRNE _0x33C
;    3363 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x34
	BRNE _0x33D
;    3364 	        {
;    3365        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x33E
;    3366        	                        return;
	RJMP _0x4A9
;    3367        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
_0x33E:
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF3)
	BRNE _0x33F
;    3368        	                        return;
	RJMP _0x4A9
;    3369 	                window_setup(10);
_0x33F:
	CALL SUBOPT_0xBB
;    3370        		        sprintf(strbuf,"输入系数c");
	__POINTW1FN _0,877
	CALL SUBOPT_0x33
;    3371         		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x109
	PUSH R31
	PUSH R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	CALL SUBOPT_0x31
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3372 		        nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3373 	        }
;    3374 		return;
_0x33D:
;    3375 	}
;    3376 	return;
_0x33C:
_0x4A9:
	ADIW R28,1
	RET
;    3377 }
;    3378 
;    3379 LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
;    3380 LABEL flash boot_options[] = {
;    3381  	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
;    3382  	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
;    3383  	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
;    3384  	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
;    3385 };
;    3386 //boot up menu
;    3387 extern u8 eeprom scanner_type;
;    3388 void pgboottype_handler(uchar msg) {
_pgboottype_handler:
;    3389 	uchar i;
;    3390 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S54:
	.BYTE 0x1

	.CSEG
;    3391 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S54:
	.BYTE 0x1

	.CSEG
;    3392 
;    3393 	uchar min_option = 1;
;    3394 	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
;    3395 	if(msg == KEY_NUM5) { //select scanner type
	CALL __SAVELOCR4
;	msg -> Y+4
;	i -> R17
;	min_option -> R16
;	max_option -> R19
	LDI  R16,1
	LDI  R19,4
	LDD  R26,Y+4
	CPI  R26,LOW(0x35)
	BRNE _0x347
;    3396        		window_setup(4); //2 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xBF
;    3397 		sprintf(strbuf,"Scanner(1:MI,2:GUIDLINE)");
	__POINTW1FN _0,954
	CALL SUBOPT_0x33
;    3398 		i = scanner_type;
	CALL SUBOPT_0x9C
	MOV  R17,R30
;    3399 		i = wnd_intinput(i);
	ST   -Y,R17
	RCALL _wnd_intinput
	MOV  R17,R30
;    3400 		if(i == 1 || i == 2)
	CPI  R17,1
	BREQ _0x349
	CPI  R17,2
	BRNE _0x348
_0x349:
;    3401 		        scanner_type = i;
	MOV  R30,R17
	RJMP _0x4E3
;    3402 		else
_0x348:
;    3403 		        scanner_type = 1;
	LDI  R30,LOW(1)
_0x4E3:
	LDI  R26,LOW(_scanner_type)
	LDI  R27,HIGH(_scanner_type)
	CALL __EEPROMWRB
;    3404 	        msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+4,R30
;    3405 	}
;    3406 
;    3407 	if(msg == MSG_INIT) {
_0x347:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x34C
;    3408 		LCD_Cls();
	CALL _LCD_Cls
;    3409 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S54,R30
;    3410 		draw_label(&boot_banner, SW_NORMAL);
	LDI  R30,LOW(_boot_banner*2)
	LDI  R31,HIGH(_boot_banner*2)
	CALL SUBOPT_0x38
;    3411 		INIT_OPTIONS(boot_);
	MOV  R17,R16
_0x34E:
	CP   R19,R17
	BRLO _0x34F
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x38
	SUBI R17,-1
	RJMP _0x34E
_0x34F:
;    3412 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3413 	}
;    3414 	if(msg == KEY_DN)
_0x34C:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x350
;    3415 	{
;    3416 	        last_sel = curr_sel;
	CALL SUBOPT_0x10B
;    3417 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S54,R30
;    3418 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S54
	CP   R19,R26
	BRSH _0x351
;    3419 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S54,R30
;    3420 	        msg = MSG_REFRESH;
_0x351:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3421 	}
;    3422 	if(msg == KEY_UP)
_0x350:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x352
;    3423 	{
;    3424 	        last_sel = curr_sel;
	CALL SUBOPT_0x10B
;    3425 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S54,R30
;    3426 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x353
;    3427 	                curr_sel = max_option;
	STS  _curr_sel_S54,R19
;    3428 	        msg = MSG_REFRESH;
_0x353:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3429 	}
;    3430 	if(msg == KEY_OK)
_0x352:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x354
;    3431 	{
;    3432 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S54
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    3433 	}
;    3434 	if(msg == KEY_NUM1) {
_0x354:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x355
;    3435 		SET_BORE_MODE;
	CALL SUBOPT_0x3B
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3C
;    3436 		SET_TORS;
	CALL SUBOPT_0x1E
;    3437 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3438 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3439 	}
;    3440 	if(msg == KEY_NUM2) {
_0x355:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x356
;    3441 		SET_BORE_MODE;
	CALL SUBOPT_0x3B
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3C
;    3442 		SET_TORS;
	CALL SUBOPT_0x1E
;    3443 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    3444 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3445 	}
;    3446 	if(msg == KEY_NUM3) {
_0x356:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x357
;    3447 		SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    3448 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3449 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3450 	}
;    3451 	if(msg == KEY_NUM4) {
_0x357:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x358
;    3452 		SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    3453 		nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	MOV  R4,R30
;    3454 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3455 	}
;    3456 	if(msg == MSG_REFRESH) {
_0x358:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x359
;    3457 		REFRESH_OPTIONS(boot_);
	LDS  R30,_curr_sel_S54
	LDS  R26,_last_sel_S54
	CP   R30,R26
	BRNE _0x35A
	CALL __LOADLOCR4
	RJMP _0x4A7
_0x35A:
	MOV  R17,R16
_0x35C:
	CP   R19,R17
	BRLO _0x35D
	LDS  R26,_last_sel_S54
	CP   R17,R26
	BRNE _0x35E
	CALL SUBOPT_0x10A
	CALL SUBOPT_0xC0
_0x35E:
	LDS  R26,_curr_sel_S54
	CP   R17,R26
	BRNE _0x35F
	CALL SUBOPT_0x10A
	CALL SUBOPT_0xC0
_0x35F:
	SUBI R17,-1
	RJMP _0x35C
_0x35D:
;    3458 	}
;    3459 }
_0x359:
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3460 #include "utili.h"
;    3461 #include "window.h"
;    3462 #include "stdlib.h"
;    3463 /*---------------延时子程序----------------*/
;    3464 /*
;    3465 void delay1 (uint ms)
;    3466 {
;    3467   	int i,j;
;    3468   	for(i=0;i<ms;i++)
;    3469   	for(j=0;j<1000;j++)
;    3470   		;
;    3471 
;    3472 }
;    3473 */
;    3474 /*
;    3475 char lowc(uchar x)
;    3476 {
;    3477 	x = x&0x0f;
;    3478 	if(x > 0x09)
;    3479 		return 'A'+x-0x0a;
;    3480 	else
;    3481 		return '0'+x;
;    3482 }
;    3483 char highc(uchar x)
;    3484 {
;    3485 
;    3486 	x = x>>4;
;    3487 	if(x > 0x09)
;    3488 		return 'A'+x-0x0a;
;    3489 	else
;    3490 		return '0'+x;
;    3491 }
;    3492 */
;    3493 /*计算字符串长度*/
;    3494 
;    3495 extern uchar pos_databuf;
;    3496 extern uchar data_sign;
;    3497 extern u8 databuf[12];
;    3498 double buf2double()
;    3499 {
_buf2double:
;    3500         return atof(databuf);
	CALL SUBOPT_0x10C
	CALL _atof
	RET
;    3501         /*
;    3502 	double tmp = 0.0;
;    3503 	uchar i = 0;
;    3504 	uchar pos = 0;
;    3505 	for(i=1;i<pos_databuf;i++) {
;    3506 		if(databuf[i] != KEY_DOT)
;    3507 			tmp = tmp * 10.0+(databuf[i] - '0');
;    3508 		else
;    3509 			pos = pos_databuf - i - 2;
;    3510 	}
;    3511 	while(pos > 0) {
;    3512 		tmp = tmp / 10.0;
;    3513 		pos--;
;    3514 	}
;    3515 	if(data_sign == 1)
;    3516 	        tmp = -tmp;
;    3517        	return tmp;
;    3518        	*/
;    3519 }
;    3520 int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
;    3521 {
_buf2byte:
;    3522         return atoi(databuf);
	CALL SUBOPT_0x10C
	CALL _atoi
	RET
;    3523         /*
;    3524 	int tmp = 0;
;    3525 	uchar i;
;    3526 	for(i=0;i<pos_databuf;i++) {
;    3527 		tmp = tmp * 10+(databuf[i] - '0');
;    3528 	}
;    3529 	return tmp;
;    3530 	*/
;    3531 }
;    3532 
;    3533 
;    3534 #include "window.h"
;    3535 #include "utili.h"
;    3536 #include "lcd.h"
;    3537 
;    3538 MSG_HANDLER curr_window = 0;

	.DSEG
_curr_window:
	.BYTE 0x2
;    3539 
;    3540 extern uchar key;
;    3541 
;    3542 u8 databuf[12];
_databuf:
	.BYTE 0xC
;    3543 u8 pos_databuf; //position in data buffer
_pos_databuf:
	.BYTE 0x1
;    3544 u8 max_databuf;
_max_databuf:
	.BYTE 0x1
;    3545 u8 data_sign;   // sign of the data
_data_sign:
	.BYTE 0x1
;    3546 LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};

	.CSEG
;    3547 LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
;    3548 LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
;    3549 LABEL flash databox = {LBL_HZ16,20,30,9,databuf};
;    3550 
;    3551 
;    3552 void prbsninput()
;    3553 {
_prbsninput:
;    3554 	uchar msg,len;
;    3555 	databuf[0] = '\0';
	ST   -Y,R17
	ST   -Y,R16
;	msg -> R17
;	len -> R16
	CALL SUBOPT_0x10D
;    3556 	pos_databuf = 0;
;    3557 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3558 	while(1)
_0x363:
;    3559 	{
;    3560 		if(key == KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x363
;    3561 		        continue;
;    3562 
;    3563 		msg = key;
	MOV  R17,R5
;    3564 
;    3565 
;    3566 		if(msg == MSG_INIT) {
	CPI  R17,255
	BRNE _0x367
;    3567 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3568 			draw_label(&datalbl,SW_NORMAL);
;    3569 			draw_label(&datalbl3,SW_NORMAL);
	LDI  R30,LOW(_datalbl3*2)
	LDI  R31,HIGH(_datalbl3*2)
	CALL SUBOPT_0x38
;    3570 			sprintf(databuf,"");
	CALL SUBOPT_0x10C
	__POINTW1FN _0,11
	CALL SUBOPT_0x33
;    3571 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3572 //			LCD_ShowCursor(databox.x,databox.y);
;    3573 		}
;    3574 		if(msg == KEY_TAB)
_0x367:
	CPI  R17,84
	BRNE _0x368
;    3575 		{
;    3576 		/*
;    3577         		if(pos_databuf < max_databuf)
;    3578         		{
;    3579 			        databuf[pos_databuf++] = '0';
;    3580 			        databuf[pos_databuf] = '\0';
;    3581         			msg = MSG_REFRESH;
;    3582                         }
;    3583                 */
;    3584                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3585                         return;
	RJMP _0x4A8
;    3586 		}
;    3587 		if(msg == KEY_CE) {
_0x368:
	CPI  R17,67
	BRNE _0x369
;    3588 		        key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3589 			return;
	RJMP _0x4A8
;    3590 		}
;    3591 
;    3592 		if(msg == KEY_DN) {
_0x369:
	CPI  R17,68
	BRNE _0x36A
;    3593 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x36B
;    3594 			{
;    3595 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x110
	CALL SUBOPT_0x111
;    3596 				databuf[pos_databuf] = '\0';
;    3597 			}
;    3598 
;    3599 			if(databuf[pos_databuf-1] == '0')
_0x36B:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRNE _0x36C
;    3600 				databuf[pos_databuf-1] = 'Z';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(90)
	STD  Z+0,R26
;    3601 			else if (databuf[pos_databuf-1] == 'A')
	RJMP _0x36D
_0x36C:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRNE _0x36E
;    3602 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(57)
	STD  Z+0,R26
;    3603 			else
	RJMP _0x36F
_0x36E:
;    3604 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,LOW(1)
	ST   X,R30
;    3605 			msg = MSG_REFRESH;
_0x36F:
_0x36D:
	LDI  R17,LOW(254)
;    3606 		}
;    3607 		if(msg == KEY_UP) {
_0x36A:
	CPI  R17,85
	BRNE _0x370
;    3608 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x371
;    3609 			{
;    3610 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x110
	CALL SUBOPT_0x111
;    3611 				databuf[pos_databuf] = '\0';
;    3612 			}
;    3613 
;    3614 			if(databuf[pos_databuf-1] == '9')
_0x371:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x39)
	BRNE _0x372
;    3615 				databuf[pos_databuf-1] = 'A';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    3616 			else if (databuf[pos_databuf-1] == 'Z')
	RJMP _0x373
_0x372:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x5A)
	BRNE _0x374
;    3617 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
;    3618 			else
	RJMP _0x375
_0x374:
;    3619 				databuf[pos_databuf-1] +=1;
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
;    3620 			msg = MSG_REFRESH;
_0x375:
_0x373:
	LDI  R17,LOW(254)
;    3621 		}
;    3622 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x370:
	CPI  R17,48
	BRLO _0x377
	CPI  R17,58
	BRLO _0x378
_0x377:
	RJMP _0x376
_0x378:
;    3623         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x379
;    3624         		{
;    3625 			        databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3626         			databuf[pos_databuf] = '\0';
;    3627         		}
;    3628 			msg = MSG_REFRESH;
_0x379:
	LDI  R17,LOW(254)
;    3629 		}
;    3630 		if(msg == KEY_OK) {
_0x376:
	CPI  R17,79
	BRNE _0x37A
;    3631 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x37B
;    3632 		        {
;    3633 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x37D:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x37E
;    3634         		                tprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0x115
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3635         			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x37D
_0x37E:
	__POINTW2MN _tprbdata,384
	RJMP _0x4E4
;    3636         		}else{
_0x37B:
;    3637 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x381:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x382
;    3638         		                rprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0x115
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3639         			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x381
_0x382:
	__POINTW2MN _rprbdata,384
_0x4E4:
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
;    3640         		}
;    3641 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3642 			break;
	RJMP _0x365
;    3643 		}
;    3644 		if(msg == MSG_REFRESH) {
_0x37A:
	CPI  R17,254
	BRNE _0x383
;    3645 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3646 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3647 		}
;    3648 		key = KEY_INVALID;
_0x383:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3649 	}
	RJMP _0x363
_0x365:
;    3650 //	LCD_HideCursor();
;    3651 }
_0x4A8:
	LD   R16,Y+
	LD   R17,Y+
	RET
;    3652 
;    3653 uchar wnd_intinput(uchar lastval)
;    3654 {
_wnd_intinput:
;    3655 	uchar msg;
;    3656 
;    3657 	key = MSG_INIT;
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3658 	databuf[0] = '\0';
	CALL SUBOPT_0x10D
;    3659 	pos_databuf = 0;
;    3660         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3661 	while(1)
_0x384:
;    3662 	{
;    3663 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x387
;    3664 		{
;    3665 			msg = key;
	MOV  R17,R5
;    3666 
;    3667 		}else{
	RJMP _0x388
_0x387:
;    3668 			continue;
	RJMP _0x384
;    3669 		}
_0x388:
;    3670 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x389
;    3671 		{
;    3672 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3673 			draw_label(&datalbl,SW_NORMAL);
;    3674 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3675 //			LCD_ShowCursor(databox.x,databox.y);
;    3676 
;    3677 		}
;    3678 
;    3679 		if(msg == KEY_TAB)
_0x389:
	CPI  R17,84
	BRNE _0x38A
;    3680 		{
;    3681 		/*
;    3682         		if(pos_databuf < max_databuf)
;    3683         		{
;    3684 			        databuf[pos_databuf++] = '0';
;    3685 			        databuf[pos_databuf] = '\0';
;    3686         			msg = MSG_REFRESH;
;    3687                         }
;    3688                 */
;    3689 		}
;    3690 
;    3691 		if(msg == KEY_DN) {
_0x38A:
	CPI  R17,68
	BRNE _0x38B
;    3692 		/*
;    3693 			if(pos_databuf == 0)
;    3694 			{
;    3695 				databuf[pos_databuf++] = '0';
;    3696 				databuf[pos_databuf] = '\0';
;    3697 			}
;    3698 
;    3699 			if(databuf[pos_databuf-1] == '0')
;    3700 				databuf[pos_databuf-1] = '9';
;    3701 			else
;    3702 				databuf[pos_databuf-1] -= 1;
;    3703 			msg = MSG_REFRESH;
;    3704                 */
;    3705 		}
;    3706 		if(msg == KEY_UP) {
_0x38B:
	CPI  R17,85
	BRNE _0x38C
;    3707 		/*
;    3708 			if(pos_databuf == 0)
;    3709 			{
;    3710 				databuf[pos_databuf++] = '0';
;    3711 				databuf[pos_databuf] = '\0';
;    3712 			}
;    3713 
;    3714 			if(databuf[pos_databuf-1] == '9')
;    3715 				databuf[pos_databuf-1] = '0';
;    3716 			else
;    3717 				databuf[pos_databuf-1] += 1;
;    3718 			msg = MSG_REFRESH;
;    3719                 */
;    3720 		}
;    3721 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x38C:
	CPI  R17,48
	BRLO _0x38E
	CPI  R17,58
	BRLO _0x38F
_0x38E:
	RJMP _0x38D
_0x38F:
;    3722         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x390
;    3723         		{
;    3724         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3725 	        		databuf[pos_databuf] = '\0';
;    3726 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3727 		        }
;    3728 		}
_0x390:
;    3729 		if(msg == KEY_CE) {
_0x38D:
	CPI  R17,67
	BRNE _0x391
;    3730 //			LCD_HideCursor();
;    3731                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3732 			return lastval;
	LDD  R30,Y+1
	LDD  R17,Y+0
	RJMP _0x4A6
;    3733 		}
;    3734 		if(msg == KEY_OK){
_0x391:
	CPI  R17,79
	BRNE _0x392
;    3735 //			LCD_HideCursor();
;    3736                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3737 			return buf2byte();
	CALL _buf2byte
	LDD  R17,Y+0
	RJMP _0x4A6
;    3738 		}
;    3739 		if(msg == MSG_REFRESH) {
_0x392:
	CPI  R17,254
	BRNE _0x393
;    3740 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3741 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3742 		}
;    3743 		key = KEY_INVALID;
_0x393:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3744 	}
	RJMP _0x384
;    3745 }
;    3746 double wnd_floatinput(double lastval)
;    3747 {
_wnd_floatinput:
;    3748 	uchar msg;
;    3749 
;    3750 	databuf[0] = '+';
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3751         databuf[1] = '\0';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3752         pos_databuf = 1;
	LDI  R30,LOW(1)
	STS  _pos_databuf,R30
;    3753         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3754 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3755 	while(1)
_0x394:
;    3756 	{
;    3757 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x397
;    3758 		{
;    3759 			msg = key;
	MOV  R17,R5
;    3760 
;    3761 		}else{
	RJMP _0x398
_0x397:
;    3762 			continue;
	RJMP _0x394
;    3763 		}
_0x398:
;    3764 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x399
;    3765 		{
;    3766 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3767 			draw_label(&datalbl,SW_NORMAL);
;    3768 			draw_label(&datalbl2,SW_NORMAL);
	LDI  R30,LOW(_datalbl2*2)
	LDI  R31,HIGH(_datalbl2*2)
	CALL SUBOPT_0x38
;    3769 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3770 //			LCD_ShowCursor(databox.x,databox.y);
;    3771 		}
;    3772 
;    3773 		if(msg == KEY_TAB)
_0x399:
	CPI  R17,84
	BRNE _0x39A
;    3774 		{
;    3775 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3776 		}
;    3777 		if(msg == KEY_UP) {
_0x39A:
	CPI  R17,85
	BREQ PC+3
	JMP _0x39B
;    3778 		        if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x39C
;    3779 		        {
;    3780 		                if((data_sign & 0x01) == 0)      //no sign
	LDS  R30,_data_sign
	ANDI R30,LOW(0x1)
	BRNE _0x39D
;    3781 		                {
;    3782 		                        databuf[0] = '-';
	LDI  R30,LOW(45)
	STS  _databuf,R30
;    3783 		                        data_sign |= 0x01;
	LDS  R30,_data_sign
	ORI  R30,1
	RJMP _0x4E5
;    3784 		                }else{
_0x39D:
;    3785 		                        databuf[0] = '+';       //
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3786 		                        data_sign ^= 0x01;
	LDS  R26,_data_sign
	LDI  R30,LOW(1)
	EOR  R30,R26
_0x4E5:
	STS  _data_sign,R30
;    3787 		                }
;    3788 		        }else{
	RJMP _0x39F
_0x39C:
;    3789                        		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
	CALL SUBOPT_0x113
	BRSH _0x3A1
	LDS  R30,_pos_databuf
	SUBI R30,LOW(2)
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x45)
	BREQ _0x3A2
_0x3A1:
	RJMP _0x3A0
_0x3A2:
;    3790                 		{
;    3791 			                if((data_sign & 0x08) == 0)
	LDS  R30,_data_sign
	ANDI R30,LOW(0x8)
	BRNE _0x3A3
;    3792 			                {
;    3793                		        	        databuf[pos_databuf-1] = '-';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(45)
	STD  Z+0,R26
;    3794                		        	        data_sign |= 0x08;
	LDS  R30,_data_sign
	ORI  R30,8
	RJMP _0x4E6
;    3795                		                }else{
_0x3A3:
;    3796                		        	        databuf[pos_databuf-1] = '+';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3797                		        	        data_sign ^= 0x08;
	LDS  R26,_data_sign
	LDI  R30,LOW(8)
	EOR  R30,R26
_0x4E6:
	STS  _data_sign,R30
;    3798                		                }
;    3799 	        		}
;    3800 		        	msg = MSG_REFRESH;
_0x3A0:
	LDI  R17,LOW(254)
;    3801 		        }
_0x39F:
;    3802 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3803 		}
;    3804 		if(msg == KEY_DN) {
_0x39B:
	CPI  R17,68
	BRNE _0x3A5
;    3805                		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
	CALL SUBOPT_0x113
	BRSH _0x3A7
	LDS  R30,_data_sign
	ANDI R30,LOW(0x4)
	BREQ _0x3A8
_0x3A7:
	RJMP _0x3A6
_0x3A8:
;    3806         		{
;    3807         			databuf[pos_databuf++] = 'E';
	CALL SUBOPT_0x110
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(69)
	STD  Z+0,R26
;    3808         			databuf[pos_databuf++] = '+';
	CALL SUBOPT_0x110
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3809 	        		databuf[pos_databuf] = '\0';
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3810 	        		data_sign |= 0x04;
	LDS  R30,_data_sign
	ORI  R30,4
	STS  _data_sign,R30
;    3811 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3812 		        }
;    3813 
;    3814 			msg = MSG_REFRESH;
_0x3A6:
	LDI  R17,LOW(254)
;    3815 		}
;    3816 		if( msg == KEY_DOT)
_0x3A5:
	CPI  R17,46
	BRNE _0x3A9
;    3817 		{
;    3818                		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
	CALL SUBOPT_0x113
	BRSH _0x3AB
	LDS  R30,_data_sign
	ANDI R30,LOW(0x2)
	BREQ _0x3AC
_0x3AB:
	RJMP _0x3AA
_0x3AC:
;    3819         		{
;    3820         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3821 	        		databuf[pos_databuf] = '\0';
;    3822 	        		data_sign |= 0x02;
	LDS  R30,_data_sign
	ORI  R30,2
	STS  _data_sign,R30
;    3823 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3824 		        }
;    3825 
;    3826 		}
_0x3AA:
;    3827 		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
_0x3A9:
	CPI  R17,48
	BRLO _0x3AE
	CPI  R17,58
	BRLO _0x3AF
_0x3AE:
	RJMP _0x3AD
_0x3AF:
;    3828                		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x3B0
;    3829         		{
;    3830         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3831 	        		databuf[pos_databuf] = '\0';
;    3832 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3833 		        }
;    3834 		}
_0x3B0:
;    3835 		if(msg == KEY_CE) {
_0x3AD:
	CPI  R17,67
	BRNE _0x3B1
;    3836 //			LCD_HideCursor();
;    3837 		key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3838 			return lastval;
	__GETD1S 1
	LDD  R17,Y+0
	RJMP _0x4A7
;    3839 		}
;    3840 		if(msg == KEY_OK){
_0x3B1:
	CPI  R17,79
	BRNE _0x3B2
;    3841 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3842 //			LCD_HideCursor();
;    3843 			return buf2double();
	CALL _buf2double
	LDD  R17,Y+0
	RJMP _0x4A7
;    3844 		}
;    3845 		if(msg == MSG_REFRESH) {
_0x3B2:
	CPI  R17,254
	BRNE _0x3B3
;    3846 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3847 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3848 		}
;    3849 				key = KEY_INVALID;
_0x3B3:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3850 	}
	RJMP _0x394
;    3851 }
_0x4A7:
	ADIW R28,5
	RET
;    3852 //display a message box
;    3853 void wnd_msgbox(flash LABEL *lbl)
;    3854 {
_wnd_msgbox:
;    3855 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x117
	CALL _LCD_ClsBlock
;    3856 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x38
;    3857 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	CALL SUBOPT_0x117
	CALL _LCD_Rectange
;    3858 	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
	CALL SUBOPT_0x118
	SBIW R30,8
	CALL SUBOPT_0x119
	SBIW R30,1
	CALL SUBOPT_0x11A
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(16)
	LDI  R31,HIGH(16)
	CALL __SWAPW12
	SUB  R30,R26
	SBC  R31,R27
	CALL SUBOPT_0x11B
	ADIW R30,17
	ST   -Y,R30
	CALL _LCD_Rectange
;    3859 
;    3860 }
	RJMP _0x4A6
;    3861 
;    3862 void draw_inputbox(flash LABEL *lbl)
;    3863 {
_draw_inputbox:
;    3864 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x118
	SBIW R30,10
	CALL SUBOPT_0x119
	SBIW R30,3
	CALL SUBOPT_0x11A
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x11B
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3865 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
	CALL SUBOPT_0x118
	SBIW R30,10
	CALL SUBOPT_0x119
	SBIW R30,3
	CALL SUBOPT_0x11A
	ADD  R30,R26
	ADC  R31,R27
	CALL SUBOPT_0x11B
	ADIW R30,19
	ST   -Y,R30
	CALL _LCD_Rectange
;    3866 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x38
;    3867 }
_0x4A6:
	ADIW R28,2
	RET
;    3868 
;    3869 void draw_label(flash LABEL *lbl,uchar reverse) {
_draw_label:
;    3870 	uchar len = lbl->width;
;    3871 	if(lbl->type == LBL_HZ6X8) {
	ST   -Y,R17
;	*lbl -> Y+2
;	reverse -> Y+1
;	len -> R17
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	ADIW R30,6
	CALL __GETW1PF
	MOV  R17,R30
	CALL SUBOPT_0x11C
	SBIW R30,1
	BRNE _0x3B4
;    3872 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3B5
;    3873 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3B6
;    3874 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11E
	ADIW R30,8
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3875 			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
_0x3B6:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_Print6X8
;    3876 			return;
	LDD  R17,Y+0
	RJMP _0x4A5
;    3877 		}
;    3878 		if(reverse & SW_REVERSE)
_0x3B5:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3B7
;    3879 			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
	CALL SUBOPT_0x120
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3880 		return;
_0x3B7:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3881 	}
;    3882 	if(lbl->type == LBL_HZ8X16) {
_0x3B4:
	CALL SUBOPT_0x11C
	SBIW R30,2
	BRNE _0x3B8
;    3883 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3B9
;    3884 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3BA
;    3885 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11E
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3886 			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
_0x3BA:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_Print8X16
;    3887 			return;
	LDD  R17,Y+0
	RJMP _0x4A5
;    3888 		}
;    3889 		if(reverse & SW_REVERSE)
_0x3B9:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3BB
;    3890 			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
	CALL SUBOPT_0x120
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3891 		return;
_0x3BB:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3892 	}
;    3893 	/*
;    3894 	if(lbl->type == LBL_HZ24X32) {
;    3895 		if(reverse & SW_NORMAL) {
;    3896 			if((reverse & SW_OVERLAP) == 0)
;    3897 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
;    3898 			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
;    3899 		}
;    3900 		if(reverse & SW_REVERSE)
;    3901 			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
;    3902 		return;
;    3903 	}
;    3904 	*/
;    3905 	if(lbl->type == LBL_HZ12) {
_0x3B8:
	CALL SUBOPT_0x11C
	SBIW R30,4
	BRNE _0x3BC
;    3906 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3BD
;    3907 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3BE
;    3908 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x121
	ADIW R30,12
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3909 			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
_0x3BE:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_PrintHz12
;    3910 		}
;    3911 		if(reverse & SW_REVERSE)
_0x3BD:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3BF
;    3912 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
	CALL SUBOPT_0x122
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3913 		return;
_0x3BF:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3914 	}
;    3915 	if(lbl->type == LBL_HZ16) {
_0x3BC:
	CALL SUBOPT_0x11C
	SBIW R30,5
	BRNE _0x3C0
;    3916 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3C1
;    3917 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3C2
;    3918 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x121
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3919 			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
_0x3C2:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_PrintHz16
;    3920 		}
;    3921 		if(reverse & SW_REVERSE)
_0x3C1:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3C3
;    3922 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
	CALL SUBOPT_0x122
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3923 		return;
_0x3C3:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3924 	}
;    3925 }
_0x3C0:
	LDD  R17,Y+0
_0x4A5:
	ADIW R28,4
	RET
;    3926 
;    3927 

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
	BREQ _0x3C4
	LDD  R26,Y+2
	LDD  R27,Y+2+1
	CALL __GETW1P
	MOVW R16,R30
	SBIW R30,0
	BREQ _0x3C6
	__CPWRN 16,17,2
	BRLO _0x3C7
	MOVW R30,R16
	SBIW R30,1
	MOVW R16,R30
	ST   X+,R30
	ST   X,R31
_0x3C6:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	CALL SUBOPT_0x123
	LDD  R26,Y+6
	STD  Z+0,R26
_0x3C7:
	RJMP _0x3C8
_0x3C4:
	LDD  R30,Y+6
	ST   -Y,R30
	CALL _putchar
_0x3C8:
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
	BRLO _0x3C9
	LDI  R30,LOW(6)
	STD  Y+8,R30
_0x3C9:
	LDD  R30,Y+8
	LDI  R26,LOW(__fround_GD*2)
	LDI  R27,HIGH(__fround_GD*2)
	LDI  R31,0
	CALL SUBOPT_0x8C
	CALL __GETD1PF
	CALL SUBOPT_0x124
	CALL __ADDF12
	CALL SUBOPT_0x125
	LDI  R17,LOW(0)
	CALL SUBOPT_0xA4
	CALL SUBOPT_0x7C
_0x3CA:
	CALL SUBOPT_0x60
	CALL SUBOPT_0x124
	CALL __CMPF12
	BRLO _0x3CC
	CALL SUBOPT_0x126
	CALL SUBOPT_0x127
	SUBI R17,-LOW(1)
	RJMP _0x3CA
_0x3CC:
	CPI  R17,0
	BRNE _0x3CD
	CALL SUBOPT_0x128
	LDI  R30,LOW(48)
	ST   X,R30
	RJMP _0x3CE
_0x3CD:
_0x3CF:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3D1
	CALL SUBOPT_0x126
	CALL SUBOPT_0x129
	CALL SUBOPT_0x7C
	CALL SUBOPT_0x60
	CALL SUBOPT_0x124
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x128
	CALL SUBOPT_0x12A
	CALL SUBOPT_0x5C
	CALL SUBOPT_0x12B
	CALL __MULF12
	CALL SUBOPT_0x124
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x125
	RJMP _0x3CF
_0x3D1:
_0x3CE:
	LDD  R30,Y+8
	CPI  R30,0
	BRNE _0x3D2
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
	RJMP _0x4A4
_0x3D2:
	CALL SUBOPT_0x128
	LDI  R30,LOW(46)
	ST   X,R30
_0x3D3:
	LDD  R30,Y+8
	SUBI R30,LOW(1)
	STD  Y+8,R30
	SUBI R30,-LOW(1)
	BREQ _0x3D5
	CALL SUBOPT_0x124
	CALL SUBOPT_0x12C
	CALL SUBOPT_0x125
	__GETD1S 9
	CALL __CFD1U
	MOV  R16,R30
	CALL SUBOPT_0x128
	CALL SUBOPT_0x12A
	CALL SUBOPT_0x124
	CALL SUBOPT_0x12B
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x125
	RJMP _0x3D3
_0x3D5:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LDI  R30,LOW(0)
	ST   X,R30
_0x4A4:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,13
	RET
__ftoe_GD:
	SBIW R28,4
	CALL __SAVELOCR4
	CALL SUBOPT_0xA4
	CALL SUBOPT_0x8B
	LDD  R26,Y+11
	CPI  R26,LOW(0x7)
	BRLO _0x3D6
	LDI  R30,LOW(6)
	STD  Y+11,R30
_0x3D6:
	LDD  R17,Y+11
_0x3D7:
	MOV  R30,R17
	SUBI R17,1
	CPI  R30,0
	BREQ _0x3D9
	CALL SUBOPT_0x12D
	CALL SUBOPT_0x8B
	RJMP _0x3D7
_0x3D9:
	CALL SUBOPT_0x12E
	CALL __CPD10
	BRNE _0x3DA
	LDI  R19,LOW(0)
	CALL SUBOPT_0x12D
	CALL SUBOPT_0x8B
	RJMP _0x3DB
_0x3DA:
	LDD  R19,Y+11
	CALL SUBOPT_0x12F
	BREQ PC+2
	BRCC PC+3
	JMP  _0x3DC
	CALL SUBOPT_0x12D
	CALL SUBOPT_0x8B
_0x3DD:
	CALL SUBOPT_0x12F
	BRLO _0x3DF
	CALL SUBOPT_0x130
	CALL SUBOPT_0x131
	RJMP _0x3DD
_0x3DF:
	RJMP _0x3E0
_0x3DC:
_0x3E1:
	CALL SUBOPT_0x12F
	BRSH _0x3E3
	CALL SUBOPT_0x132
	CALL SUBOPT_0x133
	SUBI R19,LOW(1)
	RJMP _0x3E1
_0x3E3:
	CALL SUBOPT_0x12D
	CALL SUBOPT_0x8B
_0x3E0:
	CALL SUBOPT_0x12E
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL SUBOPT_0x133
	CALL SUBOPT_0x12F
	BRLO _0x3E4
	CALL SUBOPT_0x130
	CALL SUBOPT_0x131
_0x3E4:
_0x3DB:
	LDI  R17,LOW(0)
_0x3E5:
	LDD  R30,Y+11
	CP   R30,R17
	BRLO _0x3E7
	CALL SUBOPT_0x134
	__GETD1N 0x41200000
	CALL SUBOPT_0x129
	CALL SUBOPT_0x8B
	CALL SUBOPT_0x8D
	CALL SUBOPT_0x130
	CALL __DIVF21
	CALL __CFD1
	MOV  R16,R30
	CALL SUBOPT_0x135
	CALL SUBOPT_0x12A
	CALL SUBOPT_0x12B
	CALL SUBOPT_0x134
	CALL __MULF12
	CALL SUBOPT_0x130
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x133
	MOV  R30,R17
	SUBI R17,-1
	CPI  R30,0
	BRNE _0x3E5
	CALL SUBOPT_0x135
	LDI  R30,LOW(46)
	ST   X,R30
	RJMP _0x3E5
_0x3E7:
	CALL SUBOPT_0xDA
	SBIW R30,1
	LDD  R26,Y+10
	STD  Z+0,R26
	CPI  R19,0
	BRGE _0x3E9
	CALL SUBOPT_0x135
	LDI  R30,LOW(45)
	ST   X,R30
	NEG  R19
_0x3E9:
	CPI  R19,10
	BRLT _0x3EA
	CALL SUBOPT_0xDA
	SBIW R30,1
	MOVW R22,R30
	MOV  R26,R19
	LDI  R30,LOW(10)
	CALL __DIVB21
	SUBI R30,-LOW(48)
	MOVW R26,R22
	ST   X,R30
_0x3EA:
	CALL SUBOPT_0xDA
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
_0x3EB:
	MOVW R26,R28
	SUBI R26,LOW(-(90))
	SBCI R27,HIGH(-(90))
	CALL SUBOPT_0x123
	LPM  R30,Z
	MOV  R18,R30
	CPI  R30,0
	BRNE PC+3
	JMP _0x3ED
	MOV  R30,R17
	CPI  R30,0
	BRNE _0x3F1
	CPI  R18,37
	BRNE _0x3F2
	LDI  R17,LOW(1)
	RJMP _0x3F3
_0x3F2:
	CALL SUBOPT_0x136
_0x3F3:
	RJMP _0x3F0
_0x3F1:
	CPI  R30,LOW(0x1)
	BRNE _0x3F4
	CPI  R18,37
	BRNE _0x3F5
	CALL SUBOPT_0x136
	RJMP _0x4E7
_0x3F5:
	LDI  R17,LOW(2)
	LDI  R30,LOW(0)
	STD  Y+19,R30
	LDI  R16,LOW(0)
	CPI  R18,45
	BRNE _0x3F6
	LDI  R16,LOW(1)
	RJMP _0x3F0
_0x3F6:
	CPI  R18,43
	BRNE _0x3F7
	LDI  R30,LOW(43)
	STD  Y+19,R30
	RJMP _0x3F0
_0x3F7:
	CPI  R18,32
	BRNE _0x3F8
	LDI  R30,LOW(32)
	STD  Y+19,R30
	RJMP _0x3F0
_0x3F8:
	RJMP _0x3F9
_0x3F4:
	CPI  R30,LOW(0x2)
	BRNE _0x3FA
_0x3F9:
	LDI  R21,LOW(0)
	LDI  R17,LOW(3)
	CPI  R18,48
	BRNE _0x3FB
	ORI  R16,LOW(128)
	RJMP _0x3F0
_0x3FB:
	RJMP _0x3FC
_0x3FA:
	CPI  R30,LOW(0x3)
	BRNE _0x3FD
_0x3FC:
	CPI  R18,48
	BRLO _0x3FF
	CPI  R18,58
	BRLO _0x400
_0x3FF:
	RJMP _0x3FE
_0x400:
	MOV  R26,R21
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R21,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R21,R30
	RJMP _0x3F0
_0x3FE:
	LDI  R20,LOW(0)
	CPI  R18,46
	BRNE _0x401
	LDI  R17,LOW(4)
	RJMP _0x3F0
_0x401:
	RJMP _0x402
_0x3FD:
	CPI  R30,LOW(0x4)
	BRNE _0x404
	CPI  R18,48
	BRLO _0x406
	CPI  R18,58
	BRLO _0x407
_0x406:
	RJMP _0x405
_0x407:
	ORI  R16,LOW(32)
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	MOV  R20,R30
	MOV  R30,R18
	SUBI R30,LOW(48)
	ADD  R20,R30
	RJMP _0x3F0
_0x405:
_0x402:
	CPI  R18,108
	BRNE _0x408
	ORI  R16,LOW(2)
	LDI  R17,LOW(5)
	RJMP _0x3F0
_0x408:
	RJMP _0x409
_0x404:
	CPI  R30,LOW(0x5)
	BREQ PC+3
	JMP _0x3F0
_0x409:
	MOV  R30,R18
	CPI  R30,LOW(0x63)
	BRNE _0x40E
	CALL SUBOPT_0x137
	LDD  R30,Z+4
	CALL SUBOPT_0x138
	RJMP _0x40F
_0x40E:
	CPI  R30,LOW(0x45)
	BREQ _0x412
	CPI  R30,LOW(0x65)
	BRNE _0x413
_0x412:
	RJMP _0x414
_0x413:
	CPI  R30,LOW(0x66)
	BREQ PC+3
	JMP _0x415
_0x414:
	MOVW R30,R28
	ADIW R30,20
	STD  Y+10,R30
	STD  Y+10+1,R31
	CALL SUBOPT_0x137
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	CALL SUBOPT_0x6B
	MOVW R26,R30
	MOVW R24,R22
	CALL __CPD20
	BRLT _0x416
	LDD  R26,Y+19
	CPI  R26,LOW(0x2B)
	BREQ _0x418
	RJMP _0x419
_0x416:
	CALL SUBOPT_0x5F
	CALL __ANEGF1
	CALL SUBOPT_0x6B
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x418:
	SBRS R16,7
	RJMP _0x41A
	LDD  R30,Y+19
	CALL SUBOPT_0x138
	RJMP _0x41B
_0x41A:
	CALL SUBOPT_0x139
	LDD  R26,Y+19
	STD  Z+0,R26
_0x41B:
_0x419:
	SBRS R16,5
	LDI  R20,LOW(6)
	CPI  R18,102
	BRNE _0x41D
	CALL SUBOPT_0x6F
	ST   -Y,R20
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoa_GD
	RJMP _0x41E
_0x41D:
	CALL SUBOPT_0x6F
	ST   -Y,R20
	ST   -Y,R18
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ST   -Y,R31
	ST   -Y,R30
	CALL __ftoe_GD
_0x41E:
	MOVW R30,R28
	ADIW R30,20
	CALL SUBOPT_0x13A
	RJMP _0x41F
_0x415:
	CPI  R30,LOW(0x73)
	BRNE _0x421
	CALL SUBOPT_0x137
	CALL SUBOPT_0x13B
	CALL SUBOPT_0x13A
	RJMP _0x422
_0x421:
	CPI  R30,LOW(0x70)
	BRNE _0x424
	CALL SUBOPT_0x137
	CALL SUBOPT_0x13B
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlenf
	MOV  R17,R30
	ORI  R16,LOW(8)
_0x422:
	ANDI R16,LOW(127)
	CPI  R20,0
	BREQ _0x426
	CP   R20,R17
	BRLO _0x427
_0x426:
	RJMP _0x425
_0x427:
	MOV  R17,R20
_0x425:
_0x41F:
	LDI  R20,LOW(0)
	LDI  R30,LOW(0)
	STD  Y+18,R30
	LDI  R19,LOW(0)
	RJMP _0x428
_0x424:
	CPI  R30,LOW(0x64)
	BREQ _0x42B
	CPI  R30,LOW(0x69)
	BRNE _0x42C
_0x42B:
	ORI  R16,LOW(4)
	RJMP _0x42D
_0x42C:
	CPI  R30,LOW(0x75)
	BRNE _0x42E
_0x42D:
	LDI  R30,LOW(10)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x42F
	__GETD1N 0x3B9ACA00
	CALL SUBOPT_0x133
	LDI  R17,LOW(10)
	RJMP _0x430
_0x42F:
	__GETD1N 0x2710
	CALL SUBOPT_0x133
	LDI  R17,LOW(5)
	RJMP _0x430
_0x42E:
	CPI  R30,LOW(0x58)
	BRNE _0x432
	ORI  R16,LOW(8)
	RJMP _0x433
_0x432:
	CPI  R30,LOW(0x78)
	BREQ PC+3
	JMP _0x471
_0x433:
	LDI  R30,LOW(16)
	STD  Y+18,R30
	SBRS R16,1
	RJMP _0x435
	__GETD1N 0x10000000
	CALL SUBOPT_0x133
	LDI  R17,LOW(8)
	RJMP _0x430
_0x435:
	__GETD1N 0x1000
	CALL SUBOPT_0x133
	LDI  R17,LOW(4)
_0x430:
	CPI  R20,0
	BREQ _0x436
	ANDI R16,LOW(127)
	RJMP _0x437
_0x436:
	LDI  R20,LOW(1)
_0x437:
	SBRS R16,1
	RJMP _0x438
	CALL SUBOPT_0x137
	ADIW R30,4
	MOVW R26,R30
	CALL __GETD1P
	RJMP _0x4E8
_0x438:
	SBRS R16,2
	RJMP _0x43A
	CALL SUBOPT_0x137
	CALL SUBOPT_0x13B
	CALL __CWD1
	RJMP _0x4E8
_0x43A:
	CALL SUBOPT_0x137
	CALL SUBOPT_0x13B
	CLR  R22
	CLR  R23
_0x4E8:
	__PUTD1S 6
	SBRS R16,2
	RJMP _0x43C
	CALL SUBOPT_0x5E
	CALL __CPD20
	BRGE _0x43D
	CALL SUBOPT_0x5F
	CALL __ANEGD1
	CALL SUBOPT_0x6B
	LDI  R30,LOW(45)
	STD  Y+19,R30
_0x43D:
	LDD  R30,Y+19
	CPI  R30,0
	BREQ _0x43E
	SUBI R17,-LOW(1)
	SUBI R20,-LOW(1)
	RJMP _0x43F
_0x43E:
	ANDI R16,LOW(251)
_0x43F:
_0x43C:
	MOV  R19,R20
_0x428:
	SBRC R16,0
	RJMP _0x440
_0x441:
	CP   R17,R21
	BRSH _0x444
	CP   R19,R21
	BRLO _0x445
_0x444:
	RJMP _0x443
_0x445:
	SBRS R16,7
	RJMP _0x446
	SBRS R16,2
	RJMP _0x447
	ANDI R16,LOW(251)
	LDD  R18,Y+19
	SUBI R17,LOW(1)
	RJMP _0x448
_0x447:
	LDI  R18,LOW(48)
_0x448:
	RJMP _0x449
_0x446:
	LDI  R18,LOW(32)
_0x449:
	CALL SUBOPT_0x136
	SUBI R21,LOW(1)
	RJMP _0x441
_0x443:
_0x440:
_0x44A:
	CP   R17,R20
	BRSH _0x44C
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x44D
	CALL SUBOPT_0x13C
	BREQ _0x44E
	SUBI R21,LOW(1)
_0x44E:
	SUBI R17,LOW(1)
	SUBI R20,LOW(1)
_0x44D:
	LDI  R30,LOW(48)
	CALL SUBOPT_0x138
	CPI  R21,0
	BREQ _0x44F
	SUBI R21,LOW(1)
_0x44F:
	SUBI R20,LOW(1)
	RJMP _0x44A
_0x44C:
	MOV  R19,R17
	LDD  R30,Y+18
	CPI  R30,0
	BRNE _0x450
_0x451:
	CPI  R19,0
	BREQ _0x453
	SBRS R16,3
	RJMP _0x454
	CALL SUBOPT_0x139
	LPM  R30,Z
	RJMP _0x4E9
_0x454:
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X+
	STD  Y+10,R26
	STD  Y+10+1,R27
_0x4E9:
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
	BREQ _0x456
	SUBI R21,LOW(1)
_0x456:
	SUBI R19,LOW(1)
	RJMP _0x451
_0x453:
	RJMP _0x457
_0x450:
_0x459:
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x5E
	CALL __DIVD21U
	MOV  R18,R30
	CPI  R18,10
	BRLO _0x45B
	SBRS R16,3
	RJMP _0x45C
	SUBI R18,-LOW(55)
	RJMP _0x45D
_0x45C:
	SUBI R18,-LOW(87)
_0x45D:
	RJMP _0x45E
_0x45B:
	SUBI R18,-LOW(48)
_0x45E:
	SBRC R16,4
	RJMP _0x460
	CPI  R18,49
	BRSH _0x462
	CALL SUBOPT_0x130
	__CPD2N 0x1
	BRNE _0x461
_0x462:
	RJMP _0x464
_0x461:
	CP   R20,R19
	BRLO _0x465
	RJMP _0x4EA
_0x465:
	CP   R21,R19
	BRLO _0x467
	SBRS R16,0
	RJMP _0x468
_0x467:
	RJMP _0x466
_0x468:
	LDI  R18,LOW(32)
	SBRS R16,7
	RJMP _0x469
_0x4EA:
	LDI  R18,LOW(48)
_0x464:
	ORI  R16,LOW(16)
	SBRS R16,2
	RJMP _0x46A
	CALL SUBOPT_0x13C
	BREQ _0x46B
	SUBI R21,LOW(1)
_0x46B:
_0x46A:
_0x469:
_0x460:
	CALL SUBOPT_0x136
	CPI  R21,0
	BREQ _0x46C
	SUBI R21,LOW(1)
_0x46C:
_0x466:
	SUBI R19,LOW(1)
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x5E
	CALL __MODD21U
	CALL SUBOPT_0x6B
	LDD  R30,Y+18
	CALL SUBOPT_0x130
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __DIVD21U
	CALL SUBOPT_0x133
	CALL SUBOPT_0x12E
	CALL __CPD10
	BREQ _0x45A
	RJMP _0x459
_0x45A:
_0x457:
	SBRS R16,0
	RJMP _0x46D
_0x46E:
	CPI  R21,0
	BREQ _0x470
	SUBI R21,LOW(1)
	LDI  R30,LOW(32)
	CALL SUBOPT_0x138
	RJMP _0x46E
_0x470:
_0x46D:
_0x471:
_0x40F:
_0x4E7:
	LDI  R17,LOW(0)
_0x3F0:
	RJMP _0x3EB
_0x3ED:
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
_0x472:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isspace
	CPI  R30,0
	BREQ _0x474
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x472
_0x474:
	LDI  R30,LOW(0)
	STD  Y+7,R30
	CPI  R21,43
	BRNE _0x475
	RJMP _0x4EB
_0x475:
	CPI  R21,45
	BRNE _0x477
	LDI  R30,LOW(1)
	STD  Y+7,R30
_0x4EB:
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x477:
	LDI  R30,LOW(0)
	MOV  R20,R30
	MOV  R21,R30
	__GETWRS 16,17,16
_0x478:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BRNE _0x47B
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	LDI  R30,LOW(46)
	CALL __EQB12
	MOV  R21,R30
	CPI  R30,0
	BREQ _0x47A
_0x47B:
	OR   R20,R21
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	ADIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x478
_0x47A:
	__GETWRS 18,19,16
	CPI  R20,0
	BREQ _0x47D
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
_0x47E:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R26,X
	CPI  R26,LOW(0x2E)
	BREQ _0x480
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x13D
	CALL SUBOPT_0x12B
	CALL SUBOPT_0x72
	__GETD1N 0x41200000
	CALL __DIVF21
	CALL SUBOPT_0x87
	LDD  R30,Y+16
	LDD  R31,Y+16+1
	SBIW R30,1
	STD  Y+16,R30
	STD  Y+16+1,R31
	RJMP _0x47E
_0x480:
_0x47D:
	CALL SUBOPT_0xA4
	CALL SUBOPT_0x133
_0x481:
	LDD  R26,Y+16
	LDD  R27,Y+16+1
	SBIW R26,1
	STD  Y+16,R26
	STD  Y+16+1,R27
	CP   R26,R16
	CPC  R27,R17
	BRLO _0x483
	LD   R30,X
	SUBI R30,LOW(48)
	CALL SUBOPT_0x130
	CALL SUBOPT_0x12B
	CALL SUBOPT_0x8A
	CALL SUBOPT_0x132
	CALL SUBOPT_0x133
	RJMP _0x481
_0x483:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R21,X
	CPI  R21,101
	BREQ _0x485
	CPI  R21,69
	BREQ _0x485
	RJMP _0x484
_0x485:
	LDI  R30,LOW(0)
	MOV  R20,R30
	STD  Y+6,R30
	MOVW R26,R18
	LD   R21,X
	CPI  R21,43
	BRNE _0x487
	RJMP _0x4EC
_0x487:
	CPI  R21,45
	BRNE _0x489
	LDI  R30,LOW(1)
	STD  Y+6,R30
_0x4EC:
	__ADDWRN 18,19,1
_0x489:
_0x48A:
	MOVW R26,R18
	__ADDWRN 18,19,1
	LD   R30,X
	MOV  R21,R30
	ST   -Y,R30
	CALL _isdigit
	CPI  R30,0
	BREQ _0x48C
	MOV  R26,R20
	LDI  R30,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	ADD  R30,R21
	SUBI R30,LOW(48)
	MOV  R20,R30
	RJMP _0x48A
_0x48C:
	CPI  R20,39
	BRLO _0x48D
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x48E
	__GETD1N 0xFF7FFFFF
	RJMP _0x4A3
_0x48E:
	__GETD1N 0x7F7FFFFF
	RJMP _0x4A3
_0x48D:
	LDI  R21,LOW(32)
	CALL SUBOPT_0xA4
	CALL SUBOPT_0x133
_0x48F:
	CPI  R21,0
	BREQ _0x491
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x130
	CALL __MULF12
	CALL SUBOPT_0x133
	MOV  R30,R20
	AND  R30,R21
	BREQ _0x492
	CALL SUBOPT_0x132
	CALL SUBOPT_0x133
_0x492:
	LSR  R21
	RJMP _0x48F
_0x491:
	LDD  R30,Y+6
	CPI  R30,0
	BREQ _0x493
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x13D
	CALL __DIVF21
	RJMP _0x4ED
_0x493:
	CALL SUBOPT_0x12E
	CALL SUBOPT_0x13D
	CALL __MULF12
_0x4ED:
	__PUTD1S 8
_0x484:
	LDD  R30,Y+7
	CPI  R30,0
	BREQ _0x495
	CALL SUBOPT_0x13E
	CALL SUBOPT_0x87
_0x495:
	CALL SUBOPT_0x8E
_0x4A3:
	CALL __LOADLOCR6
	ADIW R28,18
	RET
_log:
	SBIW R28,4
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x5E
	CALL __CPD02
	BRLT _0x496
	__GETD1N 0xFF7FFFFF
	RJMP _0x4A2
_0x496:
	RCALL SUBOPT_0x6F
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
	RCALL SUBOPT_0x6B
	RCALL SUBOPT_0x5E
	__GETD1N 0x3F3504F3
	CALL __CMPF12
	BRSH _0x497
	RCALL SUBOPT_0x5F
	RCALL SUBOPT_0x5E
	RCALL SUBOPT_0x5D
	__SUBWRN 16,17,1
_0x497:
	RCALL SUBOPT_0x6C
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x5F
	__GETD2N 0x3F800000
	CALL __ADDF12
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __DIVF21
	RCALL SUBOPT_0x91
	RCALL SUBOPT_0x5E
	CALL SUBOPT_0x127
	RCALL SUBOPT_0x60
	__GETD2N 0x3F654226
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x4054114E
	RCALL SUBOPT_0x2D
	RCALL SUBOPT_0x5E
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x5C
	__GETD1N 0x3FD4114D
	RCALL SUBOPT_0x2D
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
_0x4A2:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,10
	RET
_exp:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	RCALL SUBOPT_0x78
	__GETD1N 0xC2AEAC50
	CALL __CMPF12
	BRSH _0x498
	RCALL SUBOPT_0x86
	RJMP _0x4A1
_0x498:
	RCALL SUBOPT_0x55
	CALL __CPD10
	BRNE _0x499
	RCALL SUBOPT_0xA4
	RJMP _0x4A1
_0x499:
	RCALL SUBOPT_0x78
	__GETD1N 0x42B17218
	CALL __CMPF12
	BREQ PC+2
	BRCC PC+3
	JMP  _0x49A
	__GETD1N 0x7F7FFFFF
	RJMP _0x4A1
_0x49A:
	RCALL SUBOPT_0x78
	__GETD1N 0x3FB8AA3B
	CALL __MULF12
	RCALL SUBOPT_0x53
	RCALL SUBOPT_0x55
	CALL __PUTPARD1
	CALL _floor
	CALL __CFD1
	MOVW R16,R30
	MOVW R30,R16
	RCALL SUBOPT_0x78
	CALL __CWD1
	CALL __CDF1
	RCALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F000000
	RCALL SUBOPT_0x2D
	RCALL SUBOPT_0x91
	RCALL SUBOPT_0x5E
	CALL SUBOPT_0x127
	RCALL SUBOPT_0x60
	__GETD2N 0x3D6C4C6D
	CALL __MULF12
	__GETD2N 0x40E6E3A6
	CALL __ADDF12
	RCALL SUBOPT_0x5E
	CALL __MULF12
	RCALL SUBOPT_0x6B
	RCALL SUBOPT_0x60
	__GETD2N 0x41A68D28
	CALL __ADDF12
	RCALL SUBOPT_0x7C
	RCALL SUBOPT_0x5F
	RCALL SUBOPT_0x5C
	CALL __ADDF12
	__GETD2N 0x3FB504F3
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	RCALL SUBOPT_0x5E
	RCALL SUBOPT_0x60
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
_0x4A1:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
_pow:
	SBIW R28,4
	RCALL SUBOPT_0x8E
	CALL __CPD10
	BRNE _0x49B
	RCALL SUBOPT_0x86
	RJMP _0x4A0
_0x49B:
	CALL SUBOPT_0x13D
	CALL __CPD02
	BRGE _0x49C
	RCALL SUBOPT_0x8D
	CALL __CPD10
	BRNE _0x49D
	RCALL SUBOPT_0xA4
	RJMP _0x4A0
_0x49D:
	RCALL SUBOPT_0x8E
	RCALL SUBOPT_0x73
	CALL SUBOPT_0x134
	RCALL SUBOPT_0x89
	RJMP _0x4A0
_0x49C:
	RCALL SUBOPT_0x8D
	MOVW R26,R28
	CALL __CFD1
	CALL __PUTDP1
	RCALL SUBOPT_0x2
	CALL __CDF1
	MOVW R26,R30
	MOVW R24,R22
	RCALL SUBOPT_0x8D
	CALL __CPD12
	BREQ _0x49E
	RCALL SUBOPT_0x86
	RJMP _0x4A0
_0x49E:
	CALL SUBOPT_0x13E
	RCALL SUBOPT_0x73
	CALL SUBOPT_0x134
	RCALL SUBOPT_0x89
	RCALL SUBOPT_0x87
	LD   R30,Y
	ANDI R30,LOW(0x1)
	BRNE _0x49F
	RCALL SUBOPT_0x8E
	RJMP _0x4A0
_0x49F:
	CALL SUBOPT_0x13E
_0x4A0:
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x0:
	ST   -Y,R30
	CALL _nav_command
	LDI  R30,LOW(2000000)
	LDI  R31,HIGH(2000000)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x1:
	__GETD2S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x2:
	__GETD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:41 WORDS
SUBOPT_0x3:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_namebuf)
	SBCI R31,HIGH(-_namebuf)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x4:
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
	RJMP SUBOPT_0x3

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x5:
	__POINTW2MN _sysdata,8
	CALL __EEPROMRDD
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6:
	__GETD1N 0x42CA0000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x7:
	__GETD1N 0x42C60000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x8:
	__POINTW2MN _sysdata,8
	__GETD1N 0x42C80000
	CALL __EEPROMWRD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x9:
	__POINTW2MN _sysdata,64
	CALL __EEPROMRDD
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xA:
	__GETD1N 0x447C8000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB:
	__GETD1N 0x44778000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xC:
	__POINTW2MN _sysdata,64
	__GETD1N 0x447A0000
	CALL __EEPROMWRD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xD:
	__POINTW1MN _sysdata,14
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xE:
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
SUBOPT_0xF:
	__POINTW2MN _tprbdata,576
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10:
	MOVW R30,R12
	ADIW R30,1
	ST   -Y,R30
	JMP  _scanner_set_channel

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:57 WORDS
SUBOPT_0x11:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x12:
	MOVW R30,R12
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x13:
	CALL __PUTDP1
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x14:
	__POINTW1FN _0,31
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R12
	ADIW R30,1
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:41 WORDS
SUBOPT_0x15:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x16:
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
	MOVW R30,R12
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x17:
	__POINTW1FN _0,39
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL __LSLW2
	RJMP SUBOPT_0x15

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x18:
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x19:
	LDI  R30,LOW(254)
	ST   -Y,R30
	CALL _pgmain_handler
	LDI  R30,LOW(0)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x1A:
	__POINTW1MN _sysdata,38
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x1B:
	__POINTW2MN _rundata,96
	MOVW R30,R12
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	__GETD1N 0xC61C3C00
	CALL __PUTDP1
	LDI  R30,LOW(5)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x1C:
	__POINTW2MN _rprbdata,576
	CLR  R30
	ADD  R26,R17
	ADC  R27,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x1D:
	LDI  R30,LOW(105)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0x1E:
	CBI  0x18,7
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,7
	LDI  R30,LOW(255)
	OUT  0x18,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0x1F:
	CBI  0x18,2
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,2
	LDI  R30,LOW(255)
	OUT  0x18,R30
	CBI  0x18,1
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,1
	LDI  R30,LOW(255)
	OUT  0x18,R30
	LDI  R30,LOW(15)
	STS  98,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x20:
	CBI  0x18,0
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,0
	LDI  R30,LOW(255)
	OUT  0x18,R30
	CBI  0x18,3
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,3
	LDI  R30,LOW(255)
	OUT  0x18,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x21:
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	__GETD2N 0x186A0
	CALL __CWD1
	CALL __MULD12
	RJMP SUBOPT_0x11

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x22:
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x23:
	CALL _nav_read
	CALL __PUTPARD1
	JMP  _mabs

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x24:
	__PUTD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x25:
	ST   -Y,R30
	LDI  R30,LOW(0)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x26:
	CBI  0x18,6
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	ST   -Y,R31
	ST   -Y,R30
	CALL _sleepms
	SBI  0x18,6
	LDI  R30,LOW(255)
	OUT  0x18,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x27:
	STS  _valuep,R30
	STS  _valuep+1,R31
	STS  _valuep+2,R22
	STS  _valuep+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x28:
	LDS  R26,_valuep
	LDS  R27,_valuep+1
	LDS  R24,_valuep+2
	LDS  R25,_valuep+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x29:
	__GETD1MN _rundata,196
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x2A:
	LDI  R31,0
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x2B:
	CALL __EEPROMRDD
	RCALL SUBOPT_0x28
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	RJMP SUBOPT_0x29

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x2C:
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	CALL __EEPROMRDD
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 26 TIMES, CODE SIZE REDUCTION:47 WORDS
SUBOPT_0x2D:
	CALL __SWAPD12
	CALL __SUBF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x2E:
	ADD  R26,R30
	ADC  R27,R31
	CALL __GETD1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 54 TIMES, CODE SIZE REDUCTION:103 WORDS
SUBOPT_0x2F:
	LDI  R30,LOW(_strbuf)
	LDI  R31,HIGH(_strbuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x30:
	__GETD1MN _rundata,192
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x31:
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x32:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 42 TIMES, CODE SIZE REDUCTION:161 WORDS
SUBOPT_0x33:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x34:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 14 TIMES, CODE SIZE REDUCTION:23 WORDS
SUBOPT_0x35:
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x36:
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x37:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 48 TIMES, CODE SIZE REDUCTION:185 WORDS
SUBOPT_0x38:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x39:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x3A:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _wnd_msgbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x3B:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	RJMP SUBOPT_0x25

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3C:
	CALL _navto1v
	RJMP SUBOPT_0x1D

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3D:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R10,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x3E:
	LDI  R30,LOW(_pgmain_handler)
	LDI  R31,HIGH(_pgmain_handler)
	LDS  R26,_curr_window
	LDS  R27,_curr_window+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x3F:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _display_buttons
	RJMP SUBOPT_0x26

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x40:
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _display_buttons

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:16 WORDS
SUBOPT_0x41:
	LDI  R30,0
	STS  _dlg_cnt,R30
	STS  _dlg_cnt+1,R30
	STS  _dlg_cnt+2,R30
	STS  _dlg_cnt+3,R30
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
	LDI  R30,LOW(0)
	STS  _phase,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:42 WORDS
SUBOPT_0x42:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x43:
	__POINTW2MN _sysdata,14
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x44:
	__POINTW1MN _tprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:22 WORDS
SUBOPT_0x45:
	MOVW R30,R10
	SBIW R30,1
	LDI  R26,LOW(_rundata)
	LDI  R27,HIGH(_rundata)
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x46:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0xC60CA000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:37 WORDS
SUBOPT_0x47:
	__POINTW2MN _rundata,96
	MOVW R30,R10
	SBIW R30,1
	CALL __LSLW2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x48:
	__SUBD1N -1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x49:
	LDS  R26,_onesec_cnt
	LDS  R27,_onesec_cnt+1
	LDS  R24,_onesec_cnt+2
	LDS  R25,_onesec_cnt+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x4A:
	__POINTW1MN _rprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x4B:
	CALL __MULW12U
	ADD  R30,R22
	ADC  R31,R23
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:10 WORDS
SUBOPT_0x4C:
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
SUBOPT_0x4D:
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
SUBOPT_0x4E:
	SUBI R30,LOW(1)
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0x4F:
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
SUBOPT_0x50:
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
SUBOPT_0x51:
	LDD  R26,Y+4
	LDD  R27,Y+4+1
	LD   R30,X+
	STD  Y+4,R26
	STD  Y+4+1,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x52:
	__PUTD1S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 22 TIMES, CODE SIZE REDUCTION:39 WORDS
SUBOPT_0x53:
	__PUTD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x54:
	__GETD2S 14
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x55:
	__GETD1S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x56:
	CALL __SUBF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3851B717
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x57:
	MOVW R26,R16
	__ADDWRN 16,17,1
	CPI  R26,LOW(0x64)
	LDI  R30,HIGH(0x64)
	CPC  R27,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x58:
	RCALL SUBOPT_0x55
	RCALL SUBOPT_0x54
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x40000000
	CALL __DIVF21
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x59:
	__GETD2S 30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x5A:
	CALL __MULF12
	__GETD2S 2
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x5B:
	__GETD1S 22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x5C:
	__GETD2S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x5D:
	CALL __ADDF12
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 25 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x5E:
	__GETD2S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x5F:
	__GETD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 20 TIMES, CODE SIZE REDUCTION:35 WORDS
SUBOPT_0x60:
	__GETD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x61:
	RCALL SUBOPT_0x60
	RJMP SUBOPT_0x52

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x62:
	RCALL SUBOPT_0x54
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x63:
	__GETD2S 10
	__GETD1N 0x461C4000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:49 WORDS
SUBOPT_0x64:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x40000000
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

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x65:
	LDI  R31,0
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x66:
	__POINTW2MN _rprbdata,576
	LDD  R30,Y+30
	RJMP SUBOPT_0x2A

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x67:
	__GETD1S 31
	CALL __PUTPARD1
	RCALL SUBOPT_0x5B
	CALL __PUTPARD1
	JMP  _PT100RToTValue

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x68:
	__GETD2S 31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x69:
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3F800000
	RJMP SUBOPT_0x2D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6A:
	RCALL SUBOPT_0x54
	RCALL SUBOPT_0x55
	RJMP SUBOPT_0x56

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x6B:
	__PUTD1S 6
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:45 WORDS
SUBOPT_0x6C:
	RCALL SUBOPT_0x5E
	__GETD1N 0x3F800000
	RJMP SUBOPT_0x2D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6D:
	__GETD2S 26
	CALL __MULF12
	RCALL SUBOPT_0x5E
	CALL __ADDF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x6E:
	__GETD2S 22
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x6F:
	RCALL SUBOPT_0x5F
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x70:
	RCALL SUBOPT_0x60
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x71:
	SBIW R28,8
	ST   -Y,R17
	ST   -Y,R16
	__GETD2S 10
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x72:
	CALL __ADDF12
	MOVW R26,R30
	MOVW R24,R22
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x73:
	CALL __PUTPARD1
	RJMP _log

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x74:
	__PUTD1S 2
	RJMP SUBOPT_0x70

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x75:
	MOVW R26,R16
	LDI  R31,0
	CP   R26,R30
	CPC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x76:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	__GETD2S 10
	CALL __MULF12
	RCALL SUBOPT_0x5E
	RJMP SUBOPT_0x5D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x77:
	RCALL SUBOPT_0x60
	__GETD2S 10
	CALL __MULF12
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 10 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x78:
	__GETD2S 10
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x79:
	CALL __DIVF21
	RCALL SUBOPT_0x6B
	RCALL SUBOPT_0x5F
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x7A:
	CALL __GETD1PF
	__PUTD1S 2
	__GETWRN 16,17,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x7B:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	RCALL SUBOPT_0x78
	CALL __MULF12
	RCALL SUBOPT_0x5C
	CALL __ADDF12
	__PUTD1S 2
	RCALL SUBOPT_0x5F
	RCALL SUBOPT_0x78
	CALL __MULF12
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x7C:
	__PUTD1S 2
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x7D:
	__GETD2S 13
	CALL __CPD20
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x7E:
	STS  _coef,R30
	STS  _coef+1,R31
	RJMP SUBOPT_0x7D

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x7F:
	__GETD2S 13
	__GETD1N 0x441DA75C
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x80:
	STS  _coef,R30
	STS  _coef+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x81:
	__GETD2S 13
	__GETD1N 0x443E0000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x82:
	__GETD2S 13
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x83:
	__GETD1N 0x448505C3
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x84:
	RCALL SUBOPT_0x82
	__GETD1N 0x44D01000
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x85:
	RCALL SUBOPT_0x80
	RCALL SUBOPT_0x82
	RJMP SUBOPT_0x83

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x86:
	__GETD1N 0x0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x87:
	__PUTD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x88:
	RCALL SUBOPT_0x82
	__GETD1N 0x42FDEFEC
	RJMP SUBOPT_0x2D

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x89:
	CALL __MULF12
	CALL __PUTPARD1
	RJMP _exp

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x8A:
	CALL __MULF12
	__GETD2S 8
	CALL __ADDF12
	RJMP SUBOPT_0x87

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x8B:
	__PUTD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x8C:
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x8D:
	__GETD1S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x8E:
	__GETD1S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x8F:
	__GETD1N 0xC3870000
	RJMP SUBOPT_0x52

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x90:
	__GETD1N 0xC2480000
	RCALL SUBOPT_0x52
	__GETD1N 0x44DD0000
	RJMP SUBOPT_0x53

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x91:
	RCALL SUBOPT_0x6B
	RJMP SUBOPT_0x5F

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x92:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(3)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x93:
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_lcdcmd_G8)
	SBCI R31,HIGH(-_lcdcmd_G8)
	LD   R30,Z
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x94:
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
SUBOPT_0x95:
	LDD  R30,Y+4
	ST   -Y,R30
	CALL _onecmd
	__CLRD1S 0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x96:
	LDI  R30,LOW(3)
	STS  _lcdcmd_G8,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _sendcmd

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x97:
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
SUBOPT_0x98:
	MOV  R30,R17
	LDI  R31,0
	__ADDW1MN _lcdcmd_G8,3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:24 WORDS
SUBOPT_0x99:
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
SUBOPT_0x9A:
	LDI  R26,LOW(10)
	STD  Z+0,R26
	LDI  R30,LOW(_scancmd)
	LDI  R31,HIGH(_scancmd)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x9B:
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _prints

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x9C:
	LDI  R26,LOW(_scanner_type)
	LDI  R27,HIGH(_scanner_type)
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x9D:
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __MODB21U
	ST   Y,R30
	SUBI R30,-LOW(48)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x9E:
	__POINTW1MN _scancmd,3
	LDI  R26,LOW(13)
	STD  Z+0,R26
	__POINTW1MN _scancmd,4
	RJMP SUBOPT_0x9A

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x9F:
	LDI  R30,LOW(_navcmd_G9)
	LDI  R31,HIGH(_navcmd_G9)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xA0:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	__GETD1N 0xC47A0000
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RJMP SUBOPT_0x9F

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xA1:
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
SUBOPT_0xA2:
	__GETD1N 0xC47A0000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xA3:
	STS  _reading_G9,R30
	STS  _reading_G9+1,R31
	STS  _reading_G9+2,R22
	STS  _reading_G9+3,R23
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xA4:
	__GETD1N 0x3F800000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xA5:
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
	RCALL SUBOPT_0xA2
	RJMP SUBOPT_0xA3

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xA6:
	LDS  R30,_navlen_G9
	LDI  R31,0
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xA7:
	MOVW R30,R10
	ADIW R30,1
	MOVW R10,R30
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R30,R10
	CPC  R31,R11
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0xA8:
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
SUBOPT_0xA9:
	LDI  R31,0
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xAA:
	__POINTW1FN _0,168
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R10
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xAB:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xAC:
	__POINTW1FN _0,176
	RJMP SUBOPT_0x33

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0xAD:
	MOVW R26,R10
	SBIW R26,1
	CP   R12,R26
	CPC  R13,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xAE:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_values*2)
	SBCI R31,HIGH(-_pgmain_values*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0xAF:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB1:
	__POINTW1FN _0,187
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0x45

;OPTIMIZER ADDED SUBROUTINE, CALLED 29 TIMES, CODE SIZE REDUCTION:53 WORDS
SUBOPT_0xB2:
	LDI  R24,4
	CALL _sprintf
	ADIW R28,8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xB3:
	__POINTW1MN _strbuf,8
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xB4:
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xB5:
	__POINTW1FN _0,209
	ST   -Y,R31
	ST   -Y,R30
	RJMP SUBOPT_0x47

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:33 WORDS
SUBOPT_0xB6:
	__POINTW1FN _0,214
	ST   -Y,R31
	ST   -Y,R30
	LDD  R26,Y+10
	LDD  R27,Y+10+1
	LD   R30,X
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RJMP SUBOPT_0xB2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xB7:
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_temps*2)
	SBCI R31,HIGH(-_pgmain_temps*2)
	CALL __GETW1PF
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xB8:
	LDI  R30,LOW(221)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x4B

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0xB9:
	SBIW R30,2
	ST   -Y,R30
	LDI  R30,LOW(233)
	ST   -Y,R30
	MOV  R26,R19
	CLR  R27
	__POINTWRFN 22,23,_pgmain_temps,4
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	RJMP SUBOPT_0x4B

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xBA:
	__POINTW2MN _sysdata,38
	MOVW R30,R10
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xBB:
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
	RJMP SUBOPT_0x2F

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xBC:
	CALL __PUTPARD1
	JMP  _wnd_floatinput

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xBD:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgr_options*2)
	SBCI R31,HIGH(-_pgr_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBE:
	LDS  R30,_curr_sel_S4C
	STS  _last_sel_S4C,R30
	LDS  R30,_curr_sel_S4C
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xBF:
	STS  _max_databuf,R30
	RJMP SUBOPT_0x2F

;OPTIMIZER ADDED SUBROUTINE, CALLED 17 TIMES, CODE SIZE REDUCTION:61 WORDS
SUBOPT_0xC0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC1:
	CALL _navto120mv
	RJMP SUBOPT_0x1D

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC2:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_pgt_options*2)
	SBCI R31,HIGH(-_pgt_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC3:
	LDS  R30,_curr_sel_S4D
	STS  _last_sel_S4D,R30
	LDS  R30,_curr_sel_S4D
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xC4:
	CALL __SAVELOCR6
	LDI  R16,6
	LDI  R17,1
	LDI  R18,24
	LDI  R19,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xC5:
	LDS  R30,_curr_pos_S4E
	LDS  R31,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC6:
	STS  _last_pos_S4E,R30
	STS  _last_pos_S4E+1,R31
	RJMP SUBOPT_0xC5

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xC7:
	STS  _curr_pos_S4E,R30
	STS  _curr_pos_S4E+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xC8:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xC9:
	MOV  R30,R18
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCA:
	RCALL SUBOPT_0xC5
	LDS  R26,_last_pos_S4E
	LDS  R27,_last_pos_S4E+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xCB:
	LDS  R26,_curr_pos_S4E
	LDS  R27,_curr_pos_S4E+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCC:
	MOV  R30,R19
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCD:
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	LDI  R30,LOW(255)
	CALL __EEPROMWRB
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCE:
	__POINTW1FN _0,459
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xCF:
	MOV  R30,R17
	LDI  R31,0
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xD0:
	RCALL SUBOPT_0xCB
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xD1:
	CALL __DIVW21U
	MOVW R26,R30
	MOV  R30,R16
	LDI  R31,0
	CALL __MULW12U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD2:
	LDD  R30,Y+6
	LDD  R31,Y+6+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,LOW(0xFF)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD3:
	__POINTW1FN _0,462
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD4:
	__POINTW1FN _0,469
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD5:
	LDD  R30,Y+14
	LDD  R31,Y+14+1
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD6:
	LDI  R24,8
	CALL _sprintf
	ADIW R28,12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xD7:
	CALL _rname2b
	RJMP SUBOPT_0x37

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:18 WORDS
SUBOPT_0xD8:
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
SUBOPT_0xD9:
	CALL __MODW21U
	MOVW R26,R30
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xDA:
	LDD  R30,Y+8
	LDD  R31,Y+8+1
	ADIW R30,1
	STD  Y+8,R30
	STD  Y+8+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0xDB:
	SBIW R26,1
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xDC:
	STS  _curr_pos_S4F,R30
	STS  _curr_pos_S4F+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 7 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xDD:
	LDS  R30,_curr_pos_S4F
	LDS  R31,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xDE:
	STS  _last_pos_S4F,R30
	STS  _last_pos_S4F+1,R31
	RJMP SUBOPT_0xDD

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xDF:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE0:
	RCALL SUBOPT_0xDD
	LDS  R26,_last_pos_S4F
	LDS  R27,_last_pos_S4F+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE1:
	LDS  R26,_curr_pos_S4F
	LDS  R27,_curr_pos_S4F+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xE2:
	RCALL SUBOPT_0xE1
	RJMP SUBOPT_0xDB

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xE3:
	__POINTW1FN _0,496
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+11
	LDD  R31,Y+11+1
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xE4:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x38

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE5:
	CALL __MODW21U
	MOVW R26,R30
	MOVW R30,R20
	SBIW R30,1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0xE6:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0xC0

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0xE7:
	LDS  R30,_curr_pos_S50
	LDS  R31,_curr_pos_S50+1
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xE8:
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CALL __EEPROMRDB
	CPI  R30,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xE9:
	MOVW R30,R6
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RJMP SUBOPT_0xE7

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xEA:
	STS  _curr_pos_S50,R30
	STS  _curr_pos_S50+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xEB:
	LDS  R30,_curr_pos_S50
	LDS  R31,_curr_pos_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xEC:
	LDS  R26,_curr_pos_S50
	LDS  R27,_curr_pos_S50+1
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xED:
	RCALL SUBOPT_0xEB
	LDS  R26,_last_pos_S50
	LDS  R27,_last_pos_S50+1
	CP   R30,R26
	CPC  R31,R27
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xEE:
	LDS  R26,_curr_pos_S50
	LDS  R27,_curr_pos_S50+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xEF:
	RCALL SUBOPT_0xEE
	RJMP SUBOPT_0xDB

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF0:
	__POINTW1FN _0,28
	ST   -Y,R31
	ST   -Y,R30
	MOV  R30,R20
	SUBI R30,LOW(1)
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF1:
	CALL __PUTPARD1
	RJMP SUBOPT_0xB2

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0xF2:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0x38

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xF3:
	CALL __MODW21U
	MOVW R26,R30
	MOV  R30,R21
	RJMP SUBOPT_0x4E

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xF4:
	MOV  R30,R21
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_prblist_items*2)
	SBCI R31,HIGH(-_prblist_items*2)
	RJMP SUBOPT_0xC0

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xF5:
	STS  _curr_sel_S51,R30
	STS  _curr_sel_S51+1,R31
	LDI  R30,LOW(79)
	STD  Y+4,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0xF6:
	LDS  R30,_curr_sel_S51
	LDS  R31,_curr_sel_S51+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0xF7:
	__POINTW2MN _tprbdata,576
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:47 WORDS
SUBOPT_0xF8:
	__POINTW2MN _rprbdata,576
	MOVW R30,R8
	SBIW R30,1
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xF9:
	__POINTW2MN _rprbdata,288
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFA:
	CALL __EEPROMRDD
	RJMP SUBOPT_0xBC

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0xFB:
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R8
	RJMP SUBOPT_0x35

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0xFC:
	MOV  R30,R19
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_tplist_options*2)
	SBCI R31,HIGH(-_tplist_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xFD:
	RCALL SUBOPT_0xF6
	STS  _last_sel_S51,R30
	STS  _last_sel_S51+1,R31
	RJMP SUBOPT_0xF6

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xFE:
	STS  _curr_sel_S51,R30
	STS  _curr_sel_S51+1,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0xFF:
	LDS  R26,_curr_sel_S51
	LDS  R27,_curr_sel_S51+1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x100:
	MOV  R30,R16
	LDI  R31,0
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 12 TIMES, CODE SIZE REDUCTION:19 WORDS
SUBOPT_0x101:
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x102:
	__POINTW1FN _0,473
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x103:
	MOVW R30,R8
	SBIW R30,1
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x104:
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _getprbtype
	RJMP SUBOPT_0x37

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x105:
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x106:
	CALL __EEPROMRDD
	RJMP SUBOPT_0xF1

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x107:
	MOVW R30,R8
	SBIW R30,1
	LDI  R26,LOW(_rprbdata)
	LDI  R27,HIGH(_rprbdata)
	RJMP SUBOPT_0x31

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x108:
	__POINTW2MN _rprbdata,96
	MOVW R30,R8
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:2 WORDS
SUBOPT_0x109:
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0x8C

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x10A:
	MOV  R30,R17
	SUBI R30,LOW(1)
	LDI  R26,LOW(10)
	MUL  R30,R26
	MOVW R30,R0
	SUBI R30,LOW(-_boot_options*2)
	SBCI R31,HIGH(-_boot_options*2)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10B:
	LDS  R30,_curr_sel_S54
	STS  _last_sel_S54,R30
	LDS  R30,_curr_sel_S54
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10C:
	LDI  R30,LOW(_databuf)
	LDI  R31,HIGH(_databuf)
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x10D:
	LDI  R30,LOW(0)
	STS  _databuf,R30
	STS  _pos_databuf,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x10E:
	CALL _LCD_Cls
	LDI  R30,LOW(_datalbl*2)
	LDI  R31,HIGH(_datalbl*2)
	RJMP SUBOPT_0x38

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x10F:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	ST   -Y,R31
	ST   -Y,R30
	JMP  _draw_inputbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:32 WORDS
SUBOPT_0x110:
	LDS  R30,_pos_databuf
	SUBI R30,-LOW(1)
	STS  _pos_databuf,R30
	RJMP SUBOPT_0x4E

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:8 WORDS
SUBOPT_0x111:
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
SUBOPT_0x112:
	LDS  R30,_pos_databuf
	RJMP SUBOPT_0x4E

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:12 WORDS
SUBOPT_0x113:
	LDS  R30,_max_databuf
	LDS  R26,_pos_databuf
	CP   R26,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x114:
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
SUBOPT_0x115:
	MOVW R30,R8
	SBIW R30,1
	CALL __LSLW3
	ADD  R26,R30
	ADC  R27,R31
	CLR  R30
	ADD  R26,R16
	ADC  R27,R30
	RJMP SUBOPT_0x100

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x116:
	LDI  R30,LOW(_databox*2)
	LDI  R31,HIGH(_databox*2)
	RJMP SUBOPT_0x38

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:38 WORDS
SUBOPT_0x117:
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
SUBOPT_0x118:
	LD   R30,Y
	LDD  R31,Y+1
	ADIW R30,2
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x119:
	ST   -Y,R30
	LDD  R30,Y+1
	LDD  R31,Y+1+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x11A:
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
SUBOPT_0x11B:
	ST   -Y,R30
	LDD  R30,Y+3
	LDD  R31,Y+3+1
	ADIW R30,4
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x11C:
	LDD  R30,Y+2
	LDD  R31,Y+2+1
	CALL __GETW1PF
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:67 WORDS
SUBOPT_0x11D:
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
SUBOPT_0x11E:
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
SUBOPT_0x11F:
	ADIW R30,8
	CALL __GETW1PF
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x120:
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
SUBOPT_0x121:
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
SUBOPT_0x122:
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
SUBOPT_0x123:
	LD   R30,X+
	LD   R31,X+
	ADIW R30,1
	ST   -X,R31
	ST   -X,R30
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 6 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x124:
	__GETD2S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x125:
	__PUTD1S 9
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x126:
	RCALL SUBOPT_0x5C
	__GETD1N 0x41200000
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x127:
	CALL __MULF12
	RJMP SUBOPT_0x7C

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x128:
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	ADIW R26,1
	STD  Y+6,R26
	STD  Y+6+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:7 WORDS
SUBOPT_0x129:
	CALL __DIVF21
	__GETD2N 0x3F000000
	CALL __ADDF12
	CALL __PUTPARD1
	JMP  _floor

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x12A:
	MOV  R30,R16
	SUBI R30,-LOW(48)
	ST   X,R30
	MOV  R30,R16
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x12B:
	CLR  R31
	CLR  R22
	CLR  R23
	CALL __CDF1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:25 WORDS
SUBOPT_0x12C:
	__GETD1N 0x41200000
	CALL __MULF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x12D:
	__GETD2S 4
	RJMP SUBOPT_0x12C

;OPTIMIZER ADDED SUBROUTINE, CALLED 8 TIMES, CODE SIZE REDUCTION:11 WORDS
SUBOPT_0x12E:
	__GETD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:15 WORDS
SUBOPT_0x12F:
	RCALL SUBOPT_0x8D
	__GETD2S 12
	CALL __CMPF12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x130:
	__GETD2S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0x131:
	__GETD1N 0x41200000
	CALL __DIVF21
	__PUTD1S 12
	SUBI R19,-LOW(1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x132:
	RCALL SUBOPT_0x130
	RJMP SUBOPT_0x12C

;OPTIMIZER ADDED SUBROUTINE, CALLED 13 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x133:
	__PUTD1S 12
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x134:
	__GETD2S 4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x135:
	LDD  R26,Y+8
	LDD  R27,Y+8+1
	ADIW R26,1
	STD  Y+8,R26
	STD  Y+8+1,R27
	SBIW R26,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:36 WORDS
SUBOPT_0x136:
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
SUBOPT_0x137:
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
SUBOPT_0x138:
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
SUBOPT_0x139:
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	ADIW R30,1
	STD  Y+10,R30
	STD  Y+10+1,R31
	SBIW R30,1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x13A:
	STD  Y+10,R30
	STD  Y+10+1,R31
	ST   -Y,R31
	ST   -Y,R30
	CALL _strlen
	MOV  R17,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x13B:
	ADIW R30,4
	MOVW R26,R30
	CALL __GETW1P
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:14 WORDS
SUBOPT_0x13C:
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
SUBOPT_0x13D:
	__GETD2S 8
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x13E:
	RCALL SUBOPT_0x8E
	CALL __ANEGF1
	RET


	.DSEG
_3:
	.BYTE 0x0C
_4:
	.BYTE 0x10
_556:
	.BYTE 0x0F
_557:
	.BYTE 0x10
_558:
	.BYTE 0x0B
_559:
	.BYTE 0x0D
_560:
	.BYTE 0x0F
_561:
	.BYTE 0x0F
_596:
	.BYTE 0x0F
_597:
	.BYTE 0x11
_598:
	.BYTE 0x11
_621:
	.BYTE 0x11
_663:
	.BYTE 0x13
_739:
	.BYTE 0x09
_740:
	.BYTE 0x08
_741:
	.BYTE 0x07
_742:
	.BYTE 0x09
_743:
	.BYTE 0x09
_744:
	.BYTE 0x09
_745:
	.BYTE 0x09
_746:
	.BYTE 0x09
_747:
	.BYTE 0x09
_748:
	.BYTE 0x09
_749:
	.BYTE 0x09
_804:
	.BYTE 0x09
_805:
	.BYTE 0x07
_806:
	.BYTE 0x03
_807:
	.BYTE 0x03
_808:
	.BYTE 0x03
_809:
	.BYTE 0x03
_832:
	.BYTE 0x0F
_833:
	.BYTE 0x0D
_834:
	.BYTE 0x0D
_835:
	.BYTE 0x0D
_836:
	.BYTE 0x0D
_865:
	.BYTE 0x0E
_866:
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
