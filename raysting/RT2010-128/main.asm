
;CodeVisionAVR C Compiler V1.25.3 Standard
;(C) Copyright 1998-2007 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Chip type              : ATmega128
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

	#pragma AVRPART ADMIN PART_NAME ATmega128
	#pragma AVRPART MEMORY PROG_FLASH 131072
	#pragma AVRPART MEMORY EEPROM 4096
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
	.EQU RAMPZ=0x3B
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

	OUT  RAMPZ,R24

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
;      17 Chip type           : ATmega128
;      18 Program type        : Application
;      19 Clock frequency     : 14.745600 MHz
;      20 Memory model        : Small
;      21 External SRAM size  : 0
;      22 Data Stack size     : 1024
;      23 *****************************************************/
;      24 
;      25 #include <mega128.h>
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
	__GETD1MN _rundata,192
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
	CALL SUBOPT_0x30
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
	CALL SUBOPT_0x31
	__POINTW1FN _0,45
	CALL SUBOPT_0x32
;     463         if(phase == 0)                sprintf(star,"*   ");
	LDS  R30,_phase
	CPI  R30,0
	BRNE _0x56
	CALL SUBOPT_0x31
	__POINTW1FN _0,50
	CALL SUBOPT_0x32
;     464         if(phase == 1)                sprintf(star,"**  ");
_0x56:
	LDS  R26,_phase
	CPI  R26,LOW(0x1)
	BRNE _0x57
	CALL SUBOPT_0x31
	__POINTW1FN _0,55
	CALL SUBOPT_0x32
;     465         if(phase == 2)                sprintf(star,"*** ");
_0x57:
	LDS  R26,_phase
	CPI  R26,LOW(0x2)
	BRNE _0x58
	CALL SUBOPT_0x31
	__POINTW1FN _0,60
	CALL SUBOPT_0x32
;     466         if(phase == 3)                sprintf(star,"****");
_0x58:
	LDS  R26,_phase
	CPI  R26,LOW(0x3)
	BRNE _0x59
	CALL SUBOPT_0x31
	__POINTW1FN _0,65
	CALL SUBOPT_0x32
;     467         if(IS_BORE_MODE){
_0x59:
	CALL SUBOPT_0x33
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
	CALL SUBOPT_0x34
	CALL SUBOPT_0x35
	__GETD1N 0x186A0
	CALL __DIVD21U
	CALL __PUTPARD1
	MOVW R30,R28
	ADIW R30,16
	CALL SUBOPT_0x36
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
	CALL SUBOPT_0x37
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
	CALL SUBOPT_0x38
;     527     LCD_Init();
	CALL _LCD_Init
;     528     wnd_msgbox(&bootup);
	LDI  R30,LOW(_bootup*2)
	LDI  R31,HIGH(_bootup*2)
	CALL SUBOPT_0x39
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
	CALL SUBOPT_0x38
;     539     State_Init();
	CALL _State_Init
;     540 
;     541     SET_BORE_MODE;
	CALL SUBOPT_0x3A
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3B
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
	CALL SUBOPT_0x3C
;     548 	 sysdata.R0 = 0;
	CALL SUBOPT_0x3D
;     549 	 while(1)
_0x5E:
;     550 	 {
;     551  	 	if(nextwin != 0)
	TST  R4
	BREQ _0x61
;     552 		{
;     553 			SwitchWindow(nextwin);
	ST   -Y,R4
	RCALL _SwitchWindow
;     554 			(*curr_window)(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	__CALL1MN _curr_window,0
;     555 			nextwin = 0;
	CLR  R4
;     556 		}
;     557 		if(key != KEY_INVALID)
_0x61:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE PC+3
	JMP _0x62
;     558 		{
;     559 			if((key == KEY_BTN1)||(key == KEY_BTN2)||(key == KEY_BTN3)||(key == KEY_BTN4))
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
;     560 			{
;     561 			        shortcut = key;
	MOV  R19,R5
;     562                                 //processing shortcut key
;     563 				if(curr_window == pgmain_handler)
	CALL SUBOPT_0x3E
	BRNE _0x66
;     564 				{
;     565 					LCD_Cls();
	CALL _LCD_Cls
;     566 					wnd_msgbox(&modify);
	LDI  R30,LOW(_modify*2)
	LDI  R31,HIGH(_modify*2)
	CALL SUBOPT_0x39
;     567 				}
;     568 				if(shortcut == KEY_BTN1) //mode switch
_0x66:
	CPI  R19,97
	BRNE _0x67
;     569 				{
;     570                			        SET_TOP1MA;
	CALL SUBOPT_0x20
;     571 	                	        SET_TOPT1000;
;     572 					if(IS_BORE_MODE){
	CALL SUBOPT_0x33
	BRNE _0x68
;     573 						SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL _navto120mv
	RJMP _0x4C1
;     574 					}else{
_0x68:
;     575 						SET_BORE_MODE;
	CALL SUBOPT_0x3A
	CALL SUBOPT_0x1E
	CALL _navto1v
_0x4C1:
	LDI  R30,LOW(105)
	CALL SUBOPT_0x40
;     576 					}
;     577 					dlg_cnt = 0;
	CALL SUBOPT_0x41
;     578 					onesec_cnt = 0;
;     579 					phase = 0;      //reset the state machine
;     580 				}
;     581 				if(shortcut == KEY_BTN2) //auto ktt or not
_0x67:
	CPI  R19,98
	BRNE _0x6A
;     582 				{
;     583 					if(IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x6B
;     584 					{
;     585                       			        SET_TOP1MA;
	CALL SUBOPT_0x20
;     586 	                        	        SET_TOPT1000;
;     587 						if((IS_MODE_KTT)){
	CALL SUBOPT_0x22
	BRNE _0x6C
;     588 							CLR_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(0)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(1)
	RJMP _0x4C2
;     589 							SET_PKTT;
;     590 						}else{
_0x6C:
;     591 							SET_MODE_KTT;
	__POINTW2MN _sysdata,63
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	LDI  R30,LOW(98)
	ST   -Y,R30
	LDI  R30,LOW(0)
_0x4C2:
	ST   -Y,R30
	CALL _display_buttons
;     592 							SET_PKTT;
	CALL SUBOPT_0x1D
;     593 						}
;     594         					dlg_cnt = 0;
	CALL SUBOPT_0x41
;     595 	        				onesec_cnt = 0;
;     596 		        			phase = 0;      //reset the state machine
;     597 					}
;     598 				}
_0x6B:
;     599 				if(shortcut == KEY_BTN3) //thermal probe type
_0x6A:
	CPI  R19,99
	BREQ PC+3
	JMP _0x6E
;     600 				{
;     601 					display_buttons(KEY_BTN3,1);
	LDI  R30,LOW(99)
	CALL SUBOPT_0x40
;     602 					if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x6F
;     603 					{
;     604 					        i = sysdata.tid[curr_dispch-1];
	CALL SUBOPT_0x43
	MOV  R16,R30
	CLR  R17
;     605 					        if(i != INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x70
;     606 					        {
;     607                 					if((tprbdata.type[i] >= PRBTYPE_K) &&\
;     608 	                				   (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x44
	CPI  R30,LOW(0x3)
	BRLO _0x72
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xB)
	BRLO _0x73
_0x72:
	RJMP _0x71
_0x73:
;     609 		                			{
;     610 			                			if(tprbdata.type[i] == PRBTYPE_R)
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xA)
	BRNE _0x74
;     611 				                			tprbdata.type[i] = PRBTYPE_K;
	__POINTW1MN _tprbdata,576
	ADD  R30,R16
	ADC  R31,R17
	MOVW R26,R30
	LDI  R30,LOW(3)
	RJMP _0x4C3
;     612 					                	else
_0x74:
;     613 						                	tprbdata.type[i] +=1;
	CALL SUBOPT_0x44
	SUBI R30,-LOW(1)
_0x4C3:
	CALL __EEPROMWRB
;     614         					        }
;     615         					        if(rundata.reading[curr_dispch-1] > -9000)
_0x71:
	CALL SUBOPT_0x45
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BREQ PC+2
	BRCC PC+3
	JMP  _0x76
;     616                                                        		rundata.temperature[curr_dispch-1] = MValueToTValue(rundata.reading[curr_dispch-1], tprbdata.type[i]);
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
;     617         					}
_0x76:
;     618                                         }
_0x70:
;     619 					display_buttons(KEY_BTN3,0);
_0x6F:
	LDI  R30,LOW(99)
	CALL SUBOPT_0x25
;     620 
;     621 				}
;     622 				if(shortcut == KEY_BTN4) //remove zero
_0x6E:
	CPI  R19,100
	BRNE _0x77
;     623 				{
;     624 					display_buttons(KEY_BTN4,1);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x40
;     625 					if(IS_BORE_MODE){
	CALL SUBOPT_0x33
	BRNE _0x78
;     626 					        sysdata.R0 = 0;// rundata.Rx;
	CALL SUBOPT_0x3D
;     627 					}else{
	RJMP _0x79
_0x78:
;     628 					        //sysdata.V0 = nav_read();
;     629 					        nav_command(NAV_ZEROON);
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _nav_command
;     630 					        sleepms(1000*ONEMS);
	LDI  R30,LOW(10000000)
	LDI  R31,HIGH(10000000)
	CALL SUBOPT_0x38
;     631 					}
_0x79:
;     632 					display_buttons(KEY_BTN4,0);
	LDI  R30,LOW(100)
	CALL SUBOPT_0x25
;     633 				}
;     634 				if(curr_window == pgmain_handler)       //redraw the running window
_0x77:
	CALL SUBOPT_0x3E
	BRNE _0x7A
;     635 				{
;     636 					pgmain_handler(MSG_INIT);
	LDI  R30,LOW(255)
	ST   -Y,R30
	CALL _pgmain_handler
;     637 				}
;     638 			        shortcut = KEY_INVALID;
_0x7A:
	LDI  R19,LOW(45)
;     639 
;     640 			}else{
	RJMP _0x7B
_0x63:
;     641 				(*curr_window)(key);
	ST   -Y,R5
	__CALL1MN _curr_window,0
;     642 			}
_0x7B:
;     643 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;     644 	  	}else{
	RJMP _0x7C
_0x62:
;     645 			if(curr_window != pgmain_handler)
	CALL SUBOPT_0x3E
	BREQ _0x7D
;     646 				continue;
	RJMP _0x5E
;     647 			if(dlg_cnt > 1)
_0x7D:
	CALL SUBOPT_0x35
	__CPD2N 0x2
	BRSH PC+3
	JMP _0x7E
;     648 			{
;     649 			        onesec_cnt++;
	LDS  R30,_onesec_cnt
	LDS  R31,_onesec_cnt+1
	LDS  R22,_onesec_cnt+2
	LDS  R23,_onesec_cnt+3
	CALL SUBOPT_0x48
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R31
	STS  _onesec_cnt+2,R22
	STS  _onesec_cnt+3,R23
;     650 			        if(onesec_cnt == (ONESEC-10))
	CALL SUBOPT_0x49
	__CPD2N 0x18696
	BRNE _0x7F
;     651 			        {
;     652         			        updatestate();
	CALL _updatestate
;     653         			}
;     654         			if(onesec_cnt == ONESEC)
_0x7F:
	CALL SUBOPT_0x49
	__CPD2N 0x186A0
	BRNE _0x80
;     655         			        onesec_cnt = 0 ;
	LDI  R30,0
	STS  _onesec_cnt,R30
	STS  _onesec_cnt+1,R30
	STS  _onesec_cnt+2,R30
	STS  _onesec_cnt+3,R30
;     656 				dlg_cnt--;
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
;     657 				continue;
	RJMP _0x5E
;     658 			}
;     659 			updatestate();
_0x7E:
	CALL _updatestate
;     660 			if((IS_THERM_MODE))
	CALL SUBOPT_0x42
	BRNE _0x81
;     661 			{
;     662 				if(therm_state() == 0)
	CALL _therm_state
	CPI  R30,0
	BRNE _0x82
;     663 				        continue;
	RJMP _0x5E
;     664 			}else{
_0x82:
	RJMP _0x83
_0x81:
;     665 				if(bore_state() == 0)
	CALL _bore_state
	CPI  R30,0
	BRNE _0x84
;     666 				        continue;
	RJMP _0x5E
;     667 			}
_0x84:
_0x83:
;     668 
;     669                         //shift to next channel
;     670 
;     671                         while(true)
_0x85:
;     672                         {
;     673                                 ch_to_search += 1;
	MOVW R30,R12
	ADIW R30,1
	MOVW R12,R30
;     674         			if(ch_to_search >= MAX_CH_NUM)
	LDI  R30,LOW(24)
	LDI  R31,HIGH(24)
	CP   R12,R30
	CPC  R13,R31
	BRLT _0x88
;     675         			{
;     676 	        			ch_to_search = 0;
	CLR  R12
	CLR  R13
;     677 	        			break;
	RJMP _0x87
;     678 	        		}
;     679 	        		if(IS_THERM_MODE)
_0x88:
	CALL SUBOPT_0x42
	BRNE _0x89
;     680 	        		{
;     681 	        		        i = sysdata.tid[ch_to_search];
	__POINTW1MN _sysdata,14
	RJMP _0x4C4
;     682 	        		}else{
_0x89:
;     683 	        		        i = sysdata.rid[ch_to_search];
	__POINTW1MN _sysdata,38
_0x4C4:
	ADD  R30,R12
	ADC  R31,R13
	MOVW R26,R30
	CALL __EEPROMRDB
	MOV  R16,R30
	CLR  R17
;     684 	        		}
;     685 	        		if(i == INVALID_PROBE)
	LDI  R30,LOW(255)
	LDI  R31,HIGH(255)
	CP   R30,R16
	CPC  R31,R17
	BREQ _0x85
;     686 	        		        continue;
;     687 	        		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x8C
;     688 	        		{
;     689        					if((tprbdata.type[i] >= PRBTYPE_K) && (tprbdata.type[i] <= PRBTYPE_R))
	CALL SUBOPT_0x44
	CPI  R30,LOW(0x3)
	BRLO _0x8E
	CALL SUBOPT_0x44
	CPI  R30,LOW(0xB)
	BRLO _0x8F
_0x8E:
	RJMP _0x8D
_0x8F:
;     690        					        break;
	RJMP _0x87
;     691 	        		}else{
_0x8D:
	RJMP _0x90
_0x8C:
;     692        		                        if((rprbdata.type[i] <= PRBTYPE_MAX) && (rprbdata.type[i] >= PRBTYPE_MIN))
	CALL SUBOPT_0x4A
	CPI  R30,LOW(0xF4)
	BRSH _0x92
	CALL SUBOPT_0x4A
	CPI  R30,LOW(0xF1)
	BRSH _0x93
_0x92:
	RJMP _0x91
_0x93:
;     693        		        		        break;
	RJMP _0x87
;     694 	        		}
_0x91:
_0x90:
;     695 	                }
	RJMP _0x85
_0x87:
;     696 		}
_0x7C:
;     697 	}
	RJMP _0x5E
;     698 }
_0x94:
	RJMP _0x94
;     699 
;     700 WINDOW flash wins[]={
;     701 	{PG_MAIN,	pgmain_handler},	//running menu
;     702 	{PG_RCONFIG,	pgrconfig_handler},	//main menu of R config
;     703 	{PG_TCONFIG,	pgtconfig_handler},	//main menu of T config
;     704 	{PG_CHSET,	pgchset_handler},	//menu of channel probe setup
;     705 	{PG_PRBLIST,	pgprblist_handler},	//list of probe selection
;     706 	{PG_PRBTYPELIST,pgprbtypelist_handler}, //select probe type
;     707 	{PG_PRBSET,	pgprbset_handler},	//menu of probe setup
;     708 	{PG_PRBCONFIG,	pgprbconfig_handler},  //config of r probe
;     709 	{PG_BOOTTYPE,	pgboottype_handler}	//config of t probe
;     710 };
;     711 
;     712 
;     713 void SwitchWindow(uchar page)
;     714 {
_SwitchWindow:
;     715 	u8 i;
;     716 	for(i = 0; i<sizeof(wins)/sizeof(WINDOW);i++)
	ST   -Y,R17
;	page -> Y+1
;	i -> R17
	LDI  R17,LOW(0)
_0x96:
	CPI  R17,9
	BRSH _0x97
;     717 	{
;     718 		if(wins[i].page_id == page)
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
;     719 		{
;     720 			curr_window = wins[i].handler;
	MOV  R26,R17
	CLR  R27
	__POINTWRFN 22,23,_wins,1
	LDI  R30,LOW(3)
	LDI  R31,HIGH(3)
	CALL SUBOPT_0x4B
	STS  _curr_window,R30
	STS  _curr_window+1,R31
;     721 			return;
	LDD  R17,Y+0
	RJMP _0x4B9
;     722 		}
;     723 	}
_0x98:
	SUBI R17,-1
	RJMP _0x96
_0x97:
;     724 }
	LDD  R17,Y+0
_0x4B9:
	ADIW R28,2
	RET
;     725 /*********************************************************************************/
;     726 //                         UART0 and UART1 Subroutines
;     727 /*********************************************************************************/
;     728 #include <mega128.h>
;     729 #include "uart.h"
;     730 #include "16c554.h"
;     731 #include "global.h"
;     732 
;     733 //extern u8 dsm_rcmd, dsm_rpara,dsm_cmd_received;    /* debug state machine */
;     734 //extern u8 x_modem_databuf[];                       /* store data sent from PC */
;     735 //extern u8 x_modem_one_pack_received;               /* tell main loop one pack has been received */
;     736 //extern u8 x_modem_file_being_transferred;
;     737 //extern u8 x_modem_file_transfer_complete;
;     738 
;     739 /*********************************************************************************/
;     740 //                     UART0  Variables & Buffer Definition
;     741 /*********************************************************************************/
;     742 u8 volatile tx_buffer0[TX_BUFFER_SIZE0];

	.DSEG
_tx_buffer0:
	.BYTE 0x80
;     743 u8 volatile tx_wr_index0,tx_rd_index0,tx_counter0;
_tx_wr_index0:
	.BYTE 0x1
_tx_rd_index0:
	.BYTE 0x1
_tx_counter0:
	.BYTE 0x1
;     744 
;     745 /*********************************************************************************/
;     746 //                     UART1  Variables & Buffer Definition
;     747 /*********************************************************************************/
;     748 u8 volatile tx_buffer1[TX_BUFFER_SIZE1];
_tx_buffer1:
	.BYTE 0x80
;     749 u8 volatile tx_wr_index1,tx_rd_index1,tx_counter1;
_tx_wr_index1:
	.BYTE 0x1
_tx_rd_index1:
	.BYTE 0x1
_tx_counter1:
	.BYTE 0x1
;     750 
;     751 /*
;     752  * 0-9,a-f,A-F char to hex conversion
;     753  * otherwise 0xff will be returned.
;     754  */
;     755 u8 ishexchar(u8 dat){

	.CSEG
;     756         if( (dat >= '0') && (dat <= '9'))
;	dat -> Y+0
;     757                 return dat-'0';
;     758         if( (dat >= 'a') && (dat <= 'f'))
;     759                 return dat-'a'+0x0a;
;     760         if( (dat >= 'A') && (dat <= 'F'))
;     761                 return dat-'A'+0x0a;
;     762         return 0xff;
;     763 }
;     764 
;     765 
;     766 
;     767 interrupt [USART0_RXC] void usart0_rx_isr(void)
;     768 {
_usart0_rx_isr:
	CALL SUBOPT_0x4C
;     769    u8 status,data;
;     770    //read status and data
;     771    status=UCSR0A;
;	status -> R17
;	data -> R16
	IN   R17,11
;     772    data=UDR0;
	IN   R16,12
;     773    cm0_push(data);
	ST   -Y,R16
	RCALL _kbd_uart_push
;     774 }
	CALL SUBOPT_0x4D
	RETI
;     775 
;     776 /*********************************************************************************/
;     777 //                 USART0 Transmitter interrupt service routine
;     778 /*********************************************************************************/
;     779 interrupt [USART0_TXC] void usart0_tx_isr(void)
;     780 {
_usart0_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     781 if (tx_counter0)
	LDS  R30,_tx_counter0
	CPI  R30,0
	BREQ _0xA2
;     782    {
;     783    --tx_counter0;
	SUBI R30,LOW(1)
	STS  _tx_counter0,R30
;     784    UDR0=tx_buffer0[tx_rd_index0++];
	LDS  R30,_tx_rd_index0
	SUBI R30,-LOW(1)
	STS  _tx_rd_index0,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R30,Z
	OUT  0xC,R30
;     785    tx_rd_index0 =tx_rd_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_rd_index0
	ANDI R30,0x7F
	STS  _tx_rd_index0,R30
;     786    };
_0xA2:
;     787 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     788 
;     789 /*********************************************************************************/
;     790 //                      UART1 Receiver interrupt service routine
;     791 /*********************************************************************************/
;     792 interrupt [USART1_RXC] void usart1_rx_isr(void)
;     793 {
_usart1_rx_isr:
	CALL SUBOPT_0x4C
;     794    u8 status,data;
;     795 
;     796    //read status and data
;     797    status=UCSR1A;
;	status -> R17
;	data -> R16
	LDS  R17,155
;     798    data=UDR1;
	LDS  R16,156
;     799    cm1_push(data);
	ST   -Y,R16
	CALL _nav_uart_push
;     800 }
	CALL SUBOPT_0x4D
	RETI
;     801 
;     802 /*********************************************************************************/
;     803 //                   USART1 Transmitter interrupt service routine
;     804 /*********************************************************************************/
;     805 interrupt [USART1_TXC] void usart1_tx_isr(void)
;     806 {
_usart1_tx_isr:
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
;     807 if (tx_counter1)
	LDS  R30,_tx_counter1
	CPI  R30,0
	BREQ _0xA3
;     808    {
;     809    --tx_counter1;
	SUBI R30,LOW(1)
	STS  _tx_counter1,R30
;     810    UDR1=tx_buffer1[tx_rd_index1++];
	LDS  R30,_tx_rd_index1
	SUBI R30,-LOW(1)
	STS  _tx_rd_index1,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R30,Z
	STS  156,R30
;     811    tx_rd_index1=tx_rd_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_rd_index1
	ANDI R30,0x7F
	STS  _tx_rd_index1,R30
;     812    };
_0xA3:
;     813 }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	RETI
;     814 
;     815 void com0_putc(char c)
;     816 {
_com0_putc:
;     817 if(DEBUG == 1)
;	c -> Y+0
;     818         return;
;     819 while (tx_counter0 == TX_BUFFER_SIZE0);
_0xA5:
	LDS  R26,_tx_counter0
	CPI  R26,LOW(0x80)
	BREQ _0xA5
;     820 #asm("cli")
	cli
;     821 if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter0
	CPI  R30,0
	BRNE _0xA9
	SBIC 0xB,5
	RJMP _0xA8
_0xA9:
;     822    {
;     823    tx_buffer0[tx_wr_index0++]=c;
	LDS  R30,_tx_wr_index0
	SUBI R30,-LOW(1)
	STS  _tx_wr_index0,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer0)
	SBCI R31,HIGH(-_tx_buffer0)
	LD   R26,Y
	STD  Z+0,R26
;     824    tx_wr_index0 = tx_wr_index0 & (TX_BUFFER_SIZE0-1);
	LDS  R30,_tx_wr_index0
	ANDI R30,0x7F
	STS  _tx_wr_index0,R30
;     825    ++tx_counter0;
	LDS  R30,_tx_counter0
	SUBI R30,-LOW(1)
	STS  _tx_counter0,R30
;     826    }
;     827 else
	RJMP _0xAB
_0xA8:
;     828    UDR0=c;
	LD   R30,Y
	OUT  0xC,R30
;     829 #asm("sei")
_0xAB:
	sei
;     830 }
	ADIW R28,1
	RET
;     831 
;     832 
;     833 void com1_putc(char c)
;     834 {
_com1_putc:
;     835 while (tx_counter1 == TX_BUFFER_SIZE1);
;	c -> Y+0
_0xAC:
	LDS  R26,_tx_counter1
	CPI  R26,LOW(0x80)
	BREQ _0xAC
;     836 #asm("cli")
	cli
;     837 if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
	LDS  R30,_tx_counter1
	CPI  R30,0
	BRNE _0xB0
	LDS  R30,155
	ANDI R30,LOW(0x20)
	BRNE _0xAF
_0xB0:
;     838    {
;     839    tx_buffer1[tx_wr_index1++]=c;
	LDS  R30,_tx_wr_index1
	SUBI R30,-LOW(1)
	STS  _tx_wr_index1,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_tx_buffer1)
	SBCI R31,HIGH(-_tx_buffer1)
	LD   R26,Y
	STD  Z+0,R26
;     840    tx_wr_index1 = tx_wr_index1 & (TX_BUFFER_SIZE1-1);
	LDS  R30,_tx_wr_index1
	ANDI R30,0x7F
	STS  _tx_wr_index1,R30
;     841    ++tx_counter1;
	LDS  R30,_tx_counter1
	SUBI R30,-LOW(1)
	STS  _tx_counter1,R30
;     842    }
;     843 else
	RJMP _0xB2
_0xAF:
;     844    UDR1=c;
	LD   R30,Y
	STS  156,R30
;     845 #asm("sei")
_0xB2:
	sei
;     846 }
	ADIW R28,1
	RET
;     847 
;     848 /*
;     849 void com0_puthex(u8 a)
;     850 {
;     851 	unsigned char h,l;
;     852 	h = (a & 0xf0) >> 4;
;     853 	l = (a & 0x0f);
;     854 	if(h <= 9)
;     855 		com0_putc(h+'0');
;     856 	else
;     857 		com0_putc(h+'A'-0x0a);
;     858 	if(l <= 9)
;     859 		com0_putc(l+'0');
;     860 	else
;     861 		com0_putc(l+'A'-0x0a);
;     862 }
;     863 */
;     864 
;     865 void com0_putstr(u8 *ptr){
;     866         while(*ptr != 0x00){
;	*ptr -> Y+0
;     867                 com0_putc(*ptr++);
;     868         }
;     869 }
;     870 
;     871 void com1_puthex(u8 a)
;     872 {
;     873 	unsigned char h,l;
;     874 	h = (a & 0xf0) >> 4;
;	a -> Y+2
;	h -> R17
;	l -> R16
;     875 	l = (a & 0x0f);
;     876 	if(h <= 9)
;     877 		com1_putc(h+'0');
;     878 	else
;     879 		com1_putc(h+'A'-0x0a);
;     880 	if(l <= 9)
;     881 		com1_putc(l+'0');
;     882 	else
;     883 		com1_putc(l+'A'-0x0a);
;     884 }
;     885 void com1_putstr(u8 *ptr){
;     886         while(*ptr != 0x00){
;	*ptr -> Y+0
;     887                 com1_putc(*ptr++);
;     888         }
;     889 }
;     890 /*
;     891 void com0_mputs(u8 *buf, u8 size){
;     892         while(size-- > 0){
;     893                 com0_puthex(*buf++);
;     894         }
;     895 }
;     896 */
;     897 void com1_mputs(u8 *buf, u8 size){
;     898         while(size-- > 0){
;	*buf -> Y+1
;	size -> Y+0
;     899                 com1_puthex(*buf++);
;     900         }
;     901 }
;     902 
;     903 /*********************************************************************************/
;     904 //                              Intialize UART
;     905 /*********************************************************************************/
;     906 void Init_UART()
;     907 {
_Init_UART:
;     908    // USART0 initialization
;     909    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     910    // USART0 Receiver: On
;     911    // USART0 Transmitter: On
;     912    // USART0 Mode: Asynchronous
;     913    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     914       UCSR0A=0x00;
	LDI  R30,LOW(0)
	OUT  0xB,R30
;     915       UCSR0B=0xD8;
	LDI  R30,LOW(216)
	OUT  0xA,R30
;     916       UCSR0C=0x06;
	LDI  R30,LOW(6)
	STS  149,R30
;     917       UBRR0H=0x00;
	LDI  R30,LOW(0)
	STS  144,R30
;     918       UBRR0L=0x0F;
	LDI  R30,LOW(15)
	OUT  0x9,R30
;     919 
;     920    // USART1 initialization
;     921    // Communication Parameters: 8 Data, 1 Stop, No Parity
;     922    // USART1 Receiver: On
;     923    // USART1 Transmitter: On
;     924    // USART1 Mode: Asynchronous
;     925    // USART0 Baud rate: 115200 (0x7), 57600 (0xF), 38400 (0x17), 19200 (2F), 9600(0x5F)
;     926       UCSR1A=0x00;
	LDI  R30,LOW(0)
	STS  155,R30
;     927       UCSR1B=0xD8;
	LDI  R30,LOW(216)
	STS  154,R30
;     928       UCSR1C=0x06;
	LDI  R30,LOW(6)
	STS  157,R30
;     929       UBRR1H=0x00;
	LDI  R30,LOW(0)
	STS  152,R30
;     930       UBRR1L=0x0F;
	LDI  R30,LOW(15)
	STS  153,R30
;     931 
;     932    // Initialize buffer variables
;     933       tx_wr_index0 = 0;
	LDI  R30,LOW(0)
	STS  _tx_wr_index0,R30
;     934       tx_rd_index0 = 0;
	STS  _tx_rd_index0,R30
;     935       tx_counter0  = 0;
	STS  _tx_counter0,R30
;     936 
;     937       tx_wr_index1 = 0;
	STS  _tx_wr_index1,R30
;     938       tx_rd_index1 = 0;
	STS  _tx_rd_index1,R30
;     939       tx_counter1  = 0;
	STS  _tx_counter1,R30
;     940 }
	RET
;     941 /********************************************************************************/
;     942 //                             TL16C554 Driver
;     943 // Controller:  MEGA64
;     944 // 16C554 clk:  7.3728MHZ
;     945 //
;     946 // Schematic Description:
;     947 // 16C554 <---> MEGA64
;     948 // INTA   <---> INT4
;     949 // INTB   <---> INT5
;     950 // INTC   <---> INT6
;     951 // INTD   <---> INT7
;     952 // CSA#   <---> A15
;     953 // CSB#   <---> A14
;     954 // CSC#   <---> A13
;     955 // CSD#   <---> A12
;     956 //   A0   <---> A8
;     957 //   A1   <---> A9
;     958 //   A2   <---> A10
;     959 // RESET  <---> PB0
;     960 // INTN#  <---> PE2
;     961 
;     962 // Coded by: fd_lxh@yahoo.com.cn
;     963 /********************************************************************************/
;     964 #include "mega128.h"
;     965 #include "global.h"
;     966 #include "16c554.h"
;     967 #include "uart.h"
;     968 #include "utili.h"
;     969 #define FCR_DATA 0b00000111   // FIFO trigger level: 1 byte
;     970 #define IER_DATA 0b00000001
;     971 // LCR.7 must be cleared to 0 when accessing RBR/THR/IER
;     972 // LCR.7 must be set to 1 when accessing divisor latch
;     973 #define LCR_DATA 0b00011011    // Even Parity 8 data bit, 1 stop bit
;     974 #define DLL_DATA 0x30          // 115200bps: DLL = 0x4, 57600bps: DLL= 0x8, 38400 0xC: 9600 : 0x30
;     975 #define DLM_DATA 0x0
;     976 #define MCR_DATA 0b00001000
;     977 
;     978 #define min(A,B)        ((A<B)?(A):(B))
;     979 /********************************************************************************/
;     980 //               TLC16C554 UART PORT A Interrupt Subroutine
;     981 /********************************************************************************/
;     982 interrupt [EXT_INT4] void ext_int4_isr(void)
;     983 {
_ext_int4_isr:
	CALL SUBOPT_0x4F
;     984    u8 rundata;
;     985    switch(UC_554A_IIR & 0xF)
;	rundata -> R17
	LDS  R30,29184
	ANDI R30,LOW(0xF)
;     986    {
;     987       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC3
;     988           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;     989           break;
	RJMP _0xC2
;     990       case 0x4:                         // Receiver data available or trigger level reached
_0xC3:
	CPI  R30,LOW(0x4)
	BRNE _0xC5
;     991           rundata = UC_554A_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,28672
;     992           cm_pushA(rundata);
	ST   -Y,R17
	RCALL _pc_uart_push
;     993           return;
	CALL SUBOPT_0x50
	RETI
;     994       default:
_0xC5:
;     995           break;
;     996    }
_0xC2:
;     997 
;     998    rundata = UC_554A_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,29952
;     999 }
	CALL SUBOPT_0x50
	RETI
;    1000 /********************************************************************************/
;    1001 //               TLC16C554 UART PORT B Interrupt Subroutine
;    1002 /********************************************************************************/
;    1003 // External Interrupt 5 service routine
;    1004 interrupt [EXT_INT5] void ext_int5_isr(void)
;    1005 {
_ext_int5_isr:
	CALL SUBOPT_0x4F
;    1006    u8 rundata;
;    1007    switch(UC_554B_IIR & 0xF)
;	rundata -> R17
	LDS  R30,45568
	ANDI R30,LOW(0xF)
;    1008    {
;    1009       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xC9
;    1010           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1011           break;
	RJMP _0xC8
;    1012       case 0x4:                         // Receiver data available or trigger level reached
_0xC9:
	CPI  R30,LOW(0x4)
	BRNE _0xCB
;    1013           rundata = UC_554B_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,45056
;    1014           cm_pushB(rundata);
	ST   -Y,R17
	RCALL _scanner_uart_push
;    1015           return;
	CALL SUBOPT_0x50
	RETI
;    1016       default:
_0xCB:
;    1017           break;
;    1018    }
_0xC8:
;    1019    rundata = UC_554B_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,46336
;    1020 }
	CALL SUBOPT_0x50
	RETI
;    1021 /********************************************************************************/
;    1022 //               TLC16C554 UART PORT C Interrupt Subroutine
;    1023 /********************************************************************************/
;    1024 // External Interrupt 6 service routine
;    1025 interrupt [EXT_INT6] void ext_int6_isr(void)
;    1026 {
_ext_int6_isr:
	CALL SUBOPT_0x4F
;    1027    u8 rundata;
;    1028    switch(UC_554C_IIR & 0xF)
;	rundata -> R17
	LDS  R30,53760
	ANDI R30,LOW(0xF)
;    1029    {
;    1030       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xCF
;    1031           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1032           break;
	RJMP _0xCE
;    1033       case 0x4:                         // Receiver data available or trigger level reached
_0xCF:
	CPI  R30,LOW(0x4)
	BRNE _0xD1
;    1034           rundata = UC_554C_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,53248
;    1035           cm_pushC(rundata);
	ST   -Y,R17
	CALL _testA
;    1036           return;
	CALL SUBOPT_0x50
	RETI
;    1037       default:
_0xD1:
;    1038           break;
;    1039    }
_0xCE:
;    1040 
;    1041    rundata = UC_554C_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,54528
;    1042 }
	CALL SUBOPT_0x50
	RETI
;    1043 
;    1044 /********************************************************************************/
;    1045 //               TLC16C554 UART PORT D Interrupt Subroutine
;    1046 /********************************************************************************/
;    1047 // External Interrupt 7 service routine
;    1048 interrupt [EXT_INT7] void ext_int7_isr(void)
;    1049 {
_ext_int7_isr:
	CALL SUBOPT_0x4F
;    1050    u8 rundata;
;    1051    switch(UC_554D_IIR & 0xF)
;	rundata -> R17
	LDS  R30,57856
	ANDI R30,LOW(0xF)
;    1052    {
;    1053       case 0xC:                         // character time out. RBR read to clear interrupt
	CPI  R30,LOW(0xC)
	BRNE _0xD5
;    1054           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1055           break;
	RJMP _0xD4
;    1056       case 0x4:                         // Receiver data available or trigger level reached
_0xD5:
	CPI  R30,LOW(0x4)
	BRNE _0xD7
;    1057           rundata = UC_554D_RBR;        // read data from receiver FIFO. RBR read to clear int
	LDS  R17,57344
;    1058           cm_pushD(rundata);
	ST   -Y,R17
	CALL _testA
;    1059           return;
	CALL SUBOPT_0x50
	RETI
;    1060       default:
_0xD7:
;    1061           break;
;    1062    }
_0xD4:
;    1063    rundata = UC_554D_LSR;        // read LSR to clear interrupt. dummy read
	LDS  R17,58624
;    1064 }
	CALL SUBOPT_0x50
	RETI
;    1065 
;    1066 /********************************************************************************/
;    1067 //                             Reset 16C554
;    1068 /********************************************************************************/
;    1069 void Reset_554(void)
;    1070 {
_Reset_554:
;    1071    // reset 16C554. Reset pin of 16C554 is connected to PB0 of MEGA64
;    1072       PORTE.3 = 1;
	SBI  0x3,3
;    1073       sleepms(5*ONEMS);    // Delay
	LDI  R30,LOW(50000)
	LDI  R31,HIGH(50000)
	CALL SUBOPT_0x38
;    1074       PORTE.3 = 0;
	CBI  0x3,3
;    1075       sleepms(5*ONEMS);    // Delay
	LDI  R30,LOW(50000)
	LDI  R31,HIGH(50000)
	CALL SUBOPT_0x38
;    1076 }
	RET
;    1077 /********************************************************************************/
;    1078 //                             Initialize 16C554
;    1079 /********************************************************************************/
;    1080 void Init_554(void)
;    1081 {
_Init_554:
;    1082       Reset_554();  // Reset 554
	CALL _Reset_554
;    1083 
;    1084    /********************************************************************/
;    1085    //                      Initialize UART A
;    1086    /********************************************************************/
;    1087    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1088       UC_554A_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  29440,R30
;    1089       UC_554A_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  28672,R30
;    1090       UC_554A_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  28928,R30
;    1091 
;    1092    // Set Line Control Register:
;    1093    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1094       UC_554A_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  29440,R30
;    1095 
;    1096    // Set FIFO Control Register:
;    1097    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1098       UC_554A_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  29184,R30
;    1099 
;    1100    // Set Modem Control Register:
;    1101    // Enable enternal interrupt
;    1102       UC_554A_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  29696,R30
;    1103 
;    1104    // Set Interrupt Enable Register:
;    1105    // Enable Received data available / timeout / transmitter holding register empty &
;    1106    // Receiver line status interrupt
;    1107       UC_554A_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  28928,R30
;    1108 
;    1109    /********************************************************************/
;    1110    //                      Initialize UART B
;    1111    /********************************************************************/
;    1112    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1113    // LCR.7 must be set to "1" before setting baud rate
;    1114       UC_554B_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  45824,R30
;    1115       UC_554B_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  45056,R30
;    1116       UC_554B_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  45312,R30
;    1117 
;    1118    // Set Line Control Register:
;    1119    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1120       UC_554B_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  45824,R30
;    1121 
;    1122    // Set FIFO Control Register:
;    1123    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1124       UC_554B_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  45568,R30
;    1125 
;    1126    // Set Modem Control Register:
;    1127    // Enable enternal interrupt
;    1128       UC_554B_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  46080,R30
;    1129 
;    1130    // Set Interrupt Enable Register:
;    1131    // Enable Received data available / timeout / transmitter holding register empty &
;    1132    // Receiver line status interrupt
;    1133       UC_554B_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  45312,R30
;    1134 
;    1135    /********************************************************************/
;    1136    //                      Initialize UART C
;    1137    /********************************************************************/
;    1138    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1139       UC_554C_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  54016,R30
;    1140       UC_554C_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  53248,R30
;    1141       UC_554C_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  53504,R30
;    1142 
;    1143    // Set Line Control Register:
;    1144    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1145       UC_554C_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  54016,R30
;    1146 
;    1147    // Set FIFO Control Register:
;    1148    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1149       UC_554C_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  53760,R30
;    1150 
;    1151    // Set Modem Control Register:
;    1152    // Enable enternal interrupt
;    1153       UC_554C_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  54272,R30
;    1154 
;    1155    // Set Interrupt Enable Register:
;    1156    // Enable Received data available / timeout / transmitter holding register empty &
;    1157    // Receiver line status interrupt
;    1158       UC_554C_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  53504,R30
;    1159 
;    1160    /********************************************************************/
;    1161    //                      Initialize UART D
;    1162    /********************************************************************/
;    1163    // Set Baud rate: 115200bps (0x4), 57600bps (0x8), 38400bps (0xC)
;    1164       UC_554D_LCR = 0x80;
	LDI  R30,LOW(128)
	STS  58112,R30
;    1165       UC_554D_DLL = DLL_DATA;
	LDI  R30,LOW(48)
	STS  57344,R30
;    1166       UC_554D_DLM = DLM_DATA;
	LDI  R30,LOW(0)
	STS  57600,R30
;    1167 
;    1168    // Set Line Control Register:
;    1169    // 8 data bits, 1 stop bit, Even Parity, LCR7 = 1 to access divisor latches.
;    1170       UC_554D_LCR = LCR_DATA;
	LDI  R30,LOW(27)
	STS  58112,R30
;    1171 
;    1172    // Set FIFO Control Register:
;    1173    // Enable FIFO, Clear buffer, reset counter, trigger level at 14 bytes.
;    1174       UC_554D_FCR = FCR_DATA;
	LDI  R30,LOW(7)
	STS  57856,R30
;    1175 
;    1176    // Set Modem Control Register:
;    1177    // Enable enternal interrupt
;    1178       UC_554D_MCR = MCR_DATA;
	LDI  R30,LOW(8)
	STS  58368,R30
;    1179 
;    1180    // Set Interrupt Enable Register:
;    1181    // Enable Received data available / timeout / transmitter holding register empty &
;    1182    // Receiver line status interrupt
;    1183       UC_554D_IER = IER_DATA;
	LDI  R30,LOW(1)
	STS  57600,R30
;    1184 }
	RET
;    1185 
;    1186 
;    1187 /********************************************************************************/
;    1188 //                 Send multi-bytes (less than 16 bytes) via UART
;    1189 // Function:
;    1190 // Send multi-bytes ( < 16 Bytes) through the port specified by "uart_port"
;    1191 // format:
;    1192 // print_554(u8 *str, char uart_port)
;    1193 // arg1: string to be sent. arg2: port (A B,C,D)
;    1194 /********************************************************************************/
;    1195 void prints(u8 *str, u8 length, char uart_port)
;    1196 {
_prints:
;    1197     u8 len, i;
;    1198     //len = length & 0x0F;        //15bytes at most
;    1199     len = length;
	ST   -Y,R17
	ST   -Y,R16
;	*str -> Y+4
;	length -> Y+3
;	uart_port -> Y+2
;	len -> R17
;	i -> R16
	LDD  R17,Y+3
;    1200     switch(uart_port)
	LDD  R30,Y+2
;    1201     {
;    1202        case SPORTA:
	CPI  R30,0
	BRNE _0xDB
;    1203            // wait if data still being transmitted in UART
;    1204            // LSR5: Transmitter Holding Register Empty, 1 Empty, 0: Not Empty
;    1205            // LSR6: Transmitter Register Empty. 1 Empty, 0: Not Empty.
;    1206            // check if FIFO is empty by LSR5.
;    1207            // Note: (UC_554A_LSR) & 0x20 bracket here is necessary because
;    1208            // UC_554A_LSR is not a register name,
;    1209            // it is replaced by "*(volatile u8 *)(UARTA_BASE_ADDR+0x300)" during compilation
;    1210            // thus UC_554A_LSR & 0x20 actually means *(volatile u8 *) ((UARTA_BASE_ADDR+0x300) & 0x20)
;    1211            // here in 16c554.h, a bracket is added
;    1212            while(len)
_0xDC:
	CPI  R17,0
	BREQ _0xDE
;    1213            {
;    1214               while( !(UC_554A_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xDF:
	LDS  R30,29952
	ANDI R30,LOW(0x20)
	BREQ _0xDF
;    1215               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1216                   UC_554A_THR = *str++;
	CALL SUBOPT_0x51
	STS  28672,R30
;    1217               len = len - min(len,16);
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
;    1218            }
	RJMP _0xDC
_0xDE:
;    1219            break;
	RJMP _0xDA
;    1220        case SPORTB:
_0xDB:
	CPI  R30,LOW(0x1)
	BRNE _0xEB
;    1221            while(len)
_0xEC:
	CPI  R17,0
	BREQ _0xEE
;    1222            {
;    1223               while( !(UC_554B_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xEF:
	LDS  R30,46336
	ANDI R30,LOW(0x20)
	BREQ _0xEF
;    1224               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1225                   UC_554B_THR = *str++;
	CALL SUBOPT_0x51
	STS  45056,R30
;    1226               len = len - min(len,16);
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
;    1227            }
	RJMP _0xEC
_0xEE:
;    1228            break;
	RJMP _0xDA
;    1229        case SPORTC:
_0xEB:
	CPI  R30,LOW(0x2)
	BRNE _0xFB
;    1230            while(len)
_0xFC:
	CPI  R17,0
	BREQ _0xFE
;    1231            {
;    1232               while( !(UC_554C_LSR & 0x20) );  /* wait for FIFO to be empty */
_0xFF:
	LDS  R30,54528
	ANDI R30,LOW(0x20)
	BREQ _0xFF
;    1233               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1234                   UC_554C_THR = *str++;
	CALL SUBOPT_0x51
	STS  53248,R30
;    1235               len = len - min(len,16);
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
;    1236            }
	RJMP _0xFC
_0xFE:
;    1237 
;    1238            break;
	RJMP _0xDA
;    1239        case SPORTD:
_0xFB:
	CPI  R30,LOW(0x3)
	BRNE _0x10B
;    1240            while(len)
_0x10C:
	CPI  R17,0
	BREQ _0x10E
;    1241            {
;    1242               while( !(UC_554D_LSR & 0x20) );  /* wait for FIFO to be empty */
_0x10F:
	LDS  R30,58624
	ANDI R30,LOW(0x20)
	BREQ _0x10F
;    1243               for(i=0; i < min(len,16); i++)   /* fill FIFO with 16 bytes  */
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
;    1244                   UC_554D_THR = *str++;
	CALL SUBOPT_0x51
	STS  57344,R30
;    1245               len = len - min(len,16);
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
;    1246            }
	RJMP _0x10C
_0x10E:
;    1247            break;
	RJMP _0xDA
;    1248        case SPORT0:
_0x10B:
	CPI  R30,LOW(0x4)
	BRNE _0x11B
;    1249            while(len-- > 0){
_0x11C:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x11E
;    1250                 com0_putc(*str++);
	CALL SUBOPT_0x51
	ST   -Y,R30
	CALL _com0_putc
;    1251            }
	RJMP _0x11C
_0x11E:
;    1252            break;
	RJMP _0xDA
;    1253        case SPORT1:
_0x11B:
	CPI  R30,LOW(0x5)
	BRNE _0x123
;    1254            while(len-- > 0){
_0x120:
	MOV  R26,R17
	SUBI R17,1
	CPI  R26,LOW(0x1)
	BRLO _0x122
;    1255                 com1_putc(*str++);
	CALL SUBOPT_0x51
	ST   -Y,R30
	CALL _com1_putc
;    1256            }
	RJMP _0x120
_0x122:
;    1257            break;
	RJMP _0xDA
;    1258        default:
_0x123:
;    1259            PORTC = 0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1260            break;
;    1261     }
_0xDA:
;    1262 }
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,6
	RET
;    1263 
;    1264 
;    1265 // System Initialization file
;    1266 
;    1267 #include <mega128.h>
;    1268 #include "global.h"
;    1269 /**************************************************************************************/
;    1270 //                              Initialize Port A-G
;    1271 /**************************************************************************************/
;    1272 void Init_Port()
;    1273 {
_Init_Port:
;    1274  // Input/Output Ports initialization
;    1275  // Port A initialization
;    1276  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In
;    1277  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1278     PORTA=0x00;
	LDI  R30,LOW(0)
	OUT  0x1B,R30
;    1279     DDRA=0x00;
	OUT  0x1A,R30
;    1280 
;    1281  // Port B initialization
;    1282  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=Out
;    1283  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=1
;    1284     PORTB=0xFF;
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    1285     DDRB=0xFF;
	OUT  0x17,R30
;    1286 
;    1287  // Port C initialization
;    1288  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out
;    1289  // State7=1 State6=1 State5=1 State4=1 State3=0 State2=0 State1=0 State0=0
;    1290  // PORTC[7:4]: chip select of 16C554 channels.
;    1291     PORTC=0xF0;
	LDI  R30,LOW(240)
	OUT  0x15,R30
;    1292     DDRC=0xFF;
	LDI  R30,LOW(255)
	OUT  0x14,R30
;    1293 
;    1294  // Port D initialization
;    1295  // Func7=Out Func6=Out Func5=Out Func4=Out Func3=In Func2=In Func1=In Func0=In
;    1296  // State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T
;    1297     PORTD=0x00;
	LDI  R30,LOW(0)
	OUT  0x12,R30
;    1298     DDRD=0x00;
	OUT  0x11,R30
;    1299 
;    1300  // Port E initialization
;    1301  // Func7=In Func6=In Func5=In Func4=In Func3=In Func2=Out Func1=In Func0=In
;    1302  // State7=T State6=T State5=T State4=T State3=T State2=0 State1=T State0=T
;    1303  // PORTE.0: Input (M_RXD0)
;    1304  // PORTE.1: output (M_TXD0)
;    1305  // PORTE.2: output (16C554 INTC#)
;    1306  // PORTE.3: output (16C554 reset)
;    1307     PORTE=0x06;
	LDI  R30,LOW(6)
	OUT  0x3,R30
;    1308     DDRE=0x0E;
	LDI  R30,LOW(14)
	OUT  0x2,R30
;    1309 
;    1310  // Port F initialization
;    1311  // Input
;    1312  // State7=T State6=T State5=T State4=T State3=T State2=T State1=Out State0=Out
;    1313  // PORTF[3:0]: LED[4:1]
;    1314     PORTF=0x0F;
	LDI  R30,LOW(15)
	STS  98,R30
;    1315     DDRF=0x0F;
	STS  97,R30
;    1316 
;    1317  // Port G initialization
;    1318  // Input
;    1319     PORTG=0x00;
	LDI  R30,LOW(0)
	STS  101,R30
;    1320     DDRG=0x00;
	STS  100,R30
;    1321 }
	RET
;    1322 
;    1323 /**************************************************************************************/
;    1324 //                              Initialize Timers
;    1325 /**************************************************************************************/
;    1326 void Init_Timers()
;    1327 {
;    1328  // Timer/Counter 0 initialization
;    1329  // Clock source: System Clock
;    1330  // Clock value: 14.400 kHz
;    1331  // Mode: Normal top=FFh
;    1332  // OC0 output: Disconnected
;    1333     ASSR=0x00;
;    1334     TCCR0=0x07;
;    1335     TCNT0=0x00;
;    1336     OCR0=0x00;
;    1337 
;    1338  // Timer/Counter 1 initialization
;    1339  // Clock source: System Clock
;    1340  // Clock value: 14.400 kHz
;    1341  // Mode: Normal top=FFFFh
;    1342  // OC1A output: Discon.
;    1343  // OC1B output: Discon.
;    1344  // OC1C output: Discon.
;    1345  // Noise Canceler: Off
;    1346  // Input Capture on Falling Edge
;    1347  // Timer 1 Overflow Interrupt: On
;    1348  // Input Capture Interrupt: Off
;    1349  // Compare A Match Interrupt: Off
;    1350  // Compare B Match Interrupt: Off
;    1351  // Compare C Match Interrupt: Off
;    1352     TCCR1A=0x00;
;    1353     TCCR1B=0x05;
;    1354     TCNT1H=0x00;
;    1355     TCNT1L=0x00;
;    1356     ICR1H=0x00;
;    1357     ICR1L=0x00;
;    1358     OCR1AH=0x00;
;    1359     OCR1AL=0x00;
;    1360     OCR1BH=0x00;
;    1361     OCR1BL=0x00;
;    1362     OCR1CH=0x00;
;    1363     OCR1CL=0x00;
;    1364 
;    1365  // Timer/Counter 2 initialization
;    1366  // Clock source: System Clock
;    1367  // Clock value: Timer 2 Stopped
;    1368  // Mode: Normal top=FFh
;    1369  // OC2 output: Disconnected
;    1370     TCCR2=0x00;
;    1371     TCNT2=0x00;
;    1372     OCR2=0x00;
;    1373 
;    1374  // Timer/Counter 3 initialization
;    1375  // Clock source: System Clock
;    1376  // Clock value: Timer 3 Stopped
;    1377  // Mode: Normal top=FFFFh
;    1378  // Noise Canceler: Off
;    1379  // Input Capture on Falling Edge
;    1380  // OC3A output: Discon.
;    1381  // OC3B output: Discon.
;    1382  // OC3C output: Discon.
;    1383  // Timer 3 Overflow Interrupt: Off
;    1384  // Input Capture Interrupt: Off
;    1385  // Compare A Match Interrupt: Off
;    1386  // Compare B Match Interrupt: Off
;    1387  // Compare C Match Interrupt: Off
;    1388     TCCR3A=0x00;
;    1389     TCCR3B=0x00;
;    1390     TCNT3H=0x00;
;    1391     TCNT3L=0x00;
;    1392     ICR3H=0x00;
;    1393     ICR3L=0x00;
;    1394     OCR3AH=0x00;
;    1395     OCR3AL=0x00;
;    1396     OCR3BH=0x00;
;    1397     OCR3BL=0x00;
;    1398     OCR3CH=0x00;
;    1399     OCR3CL=0x00;
;    1400 
;    1401  // Timer(s)/Counter(s) Interrupt(s) initialization
;    1402     TIMSK=0x05;
;    1403     ETIMSK=0x00;
;    1404 }
;    1405 
;    1406 /**************************************************************************************/
;    1407 //                              Enable External Memory
;    1408 /**************************************************************************************/
;    1409 void Enable_XMEM()
;    1410 {
_Enable_XMEM:
;    1411  // External SRAM page configuration:
;    1412  // 1100h - 7FFFh / 8000h - FFFFh
;    1413  // Lower page wait state(s): 2r/w +1 addr
;    1414  // Upper page wait state(s): 2r/w + 1 addr
;    1415     MCUCR=0xC0;
	LDI  R30,LOW(192)
	OUT  0x35,R30
;    1416     //MCUCR = 0x80;
;    1417 
;    1418     XMCRA=0x4E;
	LDI  R30,LOW(78)
	STS  109,R30
;    1419     //XMCRA=0x0A;
;    1420  // PC7:0 can not be released
;    1421     XMCRB=0x80;
	LDI  R30,LOW(128)
	STS  108,R30
;    1422     // XMCRB = 0x07;
;    1423 }
	RET
;    1424 
;    1425 /**************************************************************************************/
;    1426 //                              Initialize External Interrupt
;    1427 /**************************************************************************************/
;    1428 void Init_Ex_Interrupt()
;    1429 {
;    1430  // External Interrupt(s) initialization
;    1431  // INT0: Off
;    1432  // INT1: Off
;    1433  // INT2: Off
;    1434  // INT3: Off
;    1435  // INT4: On
;    1436  // INT4 Mode: Rising Edge
;    1437  // INT5: On
;    1438  // INT5 Mode: Rising Edge
;    1439  // INT6: On
;    1440  // INT6 Mode: Rising Edge
;    1441  // INT7: On
;    1442  // INT7 Mode: Rising Edge
;    1443     EICRA=0x00;
;    1444     EICRB=0xFF;
;    1445     EIMSK=0xF0;
;    1446     EIFR=0xF0;
;    1447 }
;    1448 
;    1449 /**************************************************************************************/
;    1450 //                              Analog Comparator initialization
;    1451 /**************************************************************************************/
;    1452 void Init_ADC()
;    1453 {
;    1454  // Analog Comparator initialization
;    1455  // Analog Comparator: Off
;    1456  // Analog Comparator Input Capture by Timer/Counter 1: Off
;    1457     ACSR=0x80;
;    1458     SFIOR=0x00;
;    1459 }
;    1460 #include "global.h"
;    1461 #include "wendu.h"
;    1462 #include "utili.h"
;    1463 #include "math.h"
;    1464 #define logl log
;    1465 #define powl pow
;    1466 double PT100RToTValue(double r,double r0)
;    1467 {
_PT100RToTValue:
;    1468         double ac,bc,cc,tlow,tup,rnew,tnew;
;    1469         int count;
;    1470         ac = 3.908e-3;
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
;    1471         bc = -5.775E-7;
	__GETD1N 0xB51B057F
	__PUTD1S 22
;    1472         cc = -4.183E-12;
	__GETD1N 0xAC932D1D
	__PUTD1S 18
;    1473         tlow = -200;
	__GETD1N 0xC3480000
	CALL SUBOPT_0x52
;    1474         tup = 850;
	__GETD1N 0x44548000
	CALL SUBOPT_0x53
;    1475 	count = 0;
	__GETWRN 16,17,0
;    1476 
;    1477 	while((tup - tlow > 0.00005) && (count++ < 100))
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
;    1478 	{
;    1479 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1480 		rnew = r0 + r0*ac*tnew + r0*bc*tnew*tnew;
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
;    1481 		if(tnew < 0)
	CALL SUBOPT_0x5C
	CALL __CPD20
	BRGE _0x129
;    1482 		         rnew = rnew + r0*cc*(tnew-100)*tnew*tnew*tnew;
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
;    1483 
;    1484 		if(r < rnew)
_0x129:
	CALL SUBOPT_0x5F
	__GETD2S 34
	CALL __CMPF12
	BRSH _0x12A
;    1485 			tup = tnew;
	CALL SUBOPT_0x60
	CALL SUBOPT_0x53
;    1486 		else
	RJMP _0x12B
_0x12A:
;    1487 			tlow = tnew;
	CALL SUBOPT_0x61
;    1488 	}
_0x12B:
	RJMP _0x124
_0x126:
;    1489 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
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
;    1490 
;    1491 }
;    1492 double RValueToTValue(double r, u8 prbid)
;    1493 {
_RValueToTValue:
;    1494 	double ac,bc,cc,tlow,tup,rnew,tnew;
;    1495 	int count;
;    1496 
;    1497 	ac = rprbdata.param1[prbid];
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
;    1498 	bc = rprbdata.param2[prbid];
	__POINTW2MN _rprbdata,96
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	__PUTD1S 22
;    1499 	cc = rprbdata.param3[prbid];
	__POINTW2MN _rprbdata,192
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	__PUTD1S 18
;    1500 
;    1501 	if(rprbdata.type[prbid] == PRBTYPE_PT100)
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF1)
	BRNE _0x12C
;    1502 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x67
	RJMP _0x4B8
;    1503 	if(rprbdata.type[prbid] == PRBTYPE_PT1000)
_0x12C:
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF3)
	BRNE _0x12D
;    1504 		return PT100RToTValue(r, cc);
	CALL SUBOPT_0x67
	RJMP _0x4B8
;    1505 	if(rprbdata.type[prbid] != PRBTYPE_PT25)
_0x12D:
	CALL SUBOPT_0x66
	CPI  R30,LOW(0xF2)
	BREQ _0x12E
;    1506 		return -9999.999;
	__GETD1N 0xC61C3FFF
	RJMP _0x4B8
;    1507         if(rprbdata.rtp[prbid] > 0.1)
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
;    1508                 r = r/rprbdata.rtp[prbid];
	__POINTW2MN _rprbdata,288
	LDD  R30,Y+30
	CALL SUBOPT_0x65
	CALL __EEPROMRDD
	CALL SUBOPT_0x68
	RJMP _0x4C5
;    1509         else
_0x12F:
;    1510 		r = r/25.0;
	CALL SUBOPT_0x68
	__GETD1N 0x41C80000
_0x4C5:
	CALL __DIVF21
	__PUTD1S 31
;    1511 
;    1512 
;    1513 
;    1514 
;    1515 
;    1516 	//set the search range of T between GetT(r) +/- 1 degree
;    1517 	tlow = GetT(r) - 1;
	CALL __PUTPARD1
	RCALL _GetT
	CALL SUBOPT_0x69
	CALL SUBOPT_0x52
;    1518 	tup = tlow + 2;
	__GETD1S 14
	__GETD2N 0x40000000
	CALL __ADDF12
	CALL SUBOPT_0x53
;    1519 
;    1520 	count = 0;
	__GETWRN 16,17,0
;    1521 
;    1522 	while((tup - tlow > 0.00005) && (count++ < 100))
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
;    1523 	{
;    1524 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1525 		rnew = GetWr(tnew);
	CALL SUBOPT_0x60
	CALL __PUTPARD1
	RCALL _GetWr
	CALL SUBOPT_0x6B
;    1526 		if((tnew >= (83.8058-273.15)) && (tnew <= (273.16-273.15)))
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
;    1527         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*logl(rnew);
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
;    1528                 else
_0x136:
;    1529         		rnew = rnew + ac*(rnew-1) + bc*(rnew-1)*(rnew-1) + cc*(rnew-1)*(rnew-1)*(rnew-1);
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
;    1530 		if(r < rnew)
	CALL SUBOPT_0x5F
	CALL SUBOPT_0x68
	CALL __CMPF12
	BRSH _0x13A
;    1531 			tup = tnew;
	CALL SUBOPT_0x70
;    1532 		else
	RJMP _0x13B
_0x13A:
;    1533 			tlow = tnew;
	CALL SUBOPT_0x61
;    1534 	}
_0x13B:
	RJMP _0x131
_0x133:
;    1535 	return floor((tlow*10000.0+tup*10000)/2.0+0.5)/10000.0;
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
;    1536 
;    1537 
;    1538 //	return r+1;
;    1539 }
;    1540 
;    1541 flash double Ci[] = {2.78157254, 1.64650916 ,-0.13714390, -0.00649767 ,-0.00234444, 0.00511868 ,0.00187982, -0.00204472, -0.00046122 ,0.00045724};
;    1542 flash double Ai[] = {-2.13534729 ,3.18324720 ,-1.80143597, 0.71727204 ,0.50344027, -0.61899395, -0.05332322, 0.28021362, 0.10715224, -0.29302865, 0.04459872 ,0.11868632 ,-0.05248134};
;    1543 flash u8 sizeAi = sizeof(Ai)/sizeof(double);
;    1544 flash u8 sizeCi = sizeof(Ci)/sizeof(double);
;    1545 
;    1546 double GetWr(double t)
;    1547 {
_GetWr:
;    1548 	double result;
;    1549 	double pert;
;    1550 	int i;
;    1551 	if(t < 0)
	CALL SUBOPT_0x71
;	t -> Y+10
;	result -> Y+6
;	pert -> Y+2
;	i -> R16,R17
	BRGE _0x13C
;    1552 	{       //equals to Wr189(t) in mfile
;    1553 		result = Ai[0];
	LDI  R30,LOW(_Ai*2)
	LDI  R31,HIGH(_Ai*2)
	CALL __GETD1PF
	CALL SUBOPT_0x6B
;    1554 		pert = (logl((t+273.15)/273.16)+1.5)/1.5;
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
;    1555 		t = pert;
;    1556 		for(i=1;i<sizeAi;i++)
	__GETWRN 16,17,1
_0x13E:
	LDI  R30,LOW(_sizeAi*2)
	LDI  R31,HIGH(_sizeAi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x13F
;    1557 		{
;    1558 			result = result + Ai[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ai*2)
	LDI  R27,HIGH(_Ai*2)
	CALL SUBOPT_0x76
;    1559 			t = t*pert;
	CALL SUBOPT_0x77
;    1560 		}
	__ADDWRN 16,17,1
	RJMP _0x13E
_0x13F:
;    1561 		result = exp(result);
	CALL SUBOPT_0x6F
	CALL _exp
	CALL SUBOPT_0x6B
;    1562 	}else{
	RJMP _0x140
_0x13C:
;    1563 			 result = Ci[0];
	LDI  R30,LOW(_Ci*2)
	LDI  R31,HIGH(_Ci*2)
	CALL __GETD1PF
	CALL SUBOPT_0x6B
;    1564 			 pert = t/481.0 - 1;
	CALL SUBOPT_0x78
	__GETD1N 0x43F08000
	CALL __DIVF21
	CALL SUBOPT_0x69
	CALL SUBOPT_0x74
;    1565 			 t = pert;
;    1566 			 for(i=1;i<sizeCi;i++)
	__GETWRN 16,17,1
_0x142:
	LDI  R30,LOW(_sizeCi*2)
	LDI  R31,HIGH(_sizeCi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x143
;    1567 			 {
;    1568 				 result = result + Ci[i] * t;
	MOVW R30,R16
	LDI  R26,LOW(_Ci*2)
	LDI  R27,HIGH(_Ci*2)
	CALL SUBOPT_0x76
;    1569 				 t = t*pert;
	CALL SUBOPT_0x77
;    1570 			 }
	__ADDWRN 16,17,1
	RJMP _0x142
_0x143:
;    1571 	}
_0x140:
;    1572 	return result;
	CALL SUBOPT_0x5F
	RJMP _0x4B7
;    1573 }
;    1574 flash double Di[] = {439.932854 ,472.418020, 37.684494 ,7.472018 ,2.920828 ,0.005184, -0.963864, -0.188732, 0.191203, 0.049025};
;    1575 flash double Bi[] = {0.183324722, 0.240975303, 0.209108771, 0.190439972, 0.142648498, 0.077993465, 0.012475611, -0.032267127, -0.075291522, -0.056470670, 0.076201285, 0.123893204, -0.029201193, -0.091173542, 0.001317696, 0.026025526};
;    1576 flash u8 sizeDi = sizeof(Di)/sizeof(double);
;    1577 flash u8 sizeBi = sizeof(Bi)/sizeof(double);
;    1578 
;    1579 double GetT(double w)
;    1580 {
_GetT:
;    1581 	double perw;
;    1582 	double result;
;    1583 	int i;
;    1584 
;    1585 	if(w >= 0)
	CALL SUBOPT_0x71
;	w -> Y+10
;	perw -> Y+6
;	result -> Y+2
;	i -> R16,R17
	BRLT _0x144
;    1586 	{       //t660r
;    1587 		perw = (w-2.64)/1.64;
	CALL SUBOPT_0x78
	__GETD1N 0x4028F5C3
	CALL SUBOPT_0x2D
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x3FD1EB85
	CALL SUBOPT_0x79
;    1588 		w = perw;
;    1589 		result = Di[0];
	LDI  R30,LOW(_Di*2)
	LDI  R31,HIGH(_Di*2)
	CALL SUBOPT_0x7A
;    1590 		for(i=1;i<sizeDi;i++)
_0x146:
	LDI  R30,LOW(_sizeDi*2)
	LDI  R31,HIGH(_sizeDi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x147
;    1591 		{
;    1592 			result = result + Di[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Di*2)
	LDI  R27,HIGH(_Di*2)
	CALL SUBOPT_0x7B
;    1593 			w = w*perw;
;    1594 		}
	__ADDWRN 16,17,1
	RJMP _0x146
_0x147:
;    1595 	}else{   //t189(r)
	RJMP _0x148
_0x144:
;    1596 		perw = (powl(w,1.0/6.0) - 0.65)/0.35;
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
;    1597 		w = perw;
;    1598 		result = Bi[0];
	LDI  R30,LOW(_Bi*2)
	LDI  R31,HIGH(_Bi*2)
	CALL SUBOPT_0x7A
;    1599 		for(i=1;i<sizeBi;i++)
_0x14A:
	LDI  R30,LOW(_sizeBi*2)
	LDI  R31,HIGH(_sizeBi*2)
	LPM  R30,Z
	CALL SUBOPT_0x75
	BRGE _0x14B
;    1600 		{
;    1601 			result = result + Bi[i] * w;
	MOVW R30,R16
	LDI  R26,LOW(_Bi*2)
	LDI  R27,HIGH(_Bi*2)
	CALL SUBOPT_0x7B
;    1602 			w = w*perw;
;    1603 		}
	__ADDWRN 16,17,1
	RJMP _0x14A
_0x14B:
;    1604 		result = 273.15*result - 273.15;
	CALL SUBOPT_0x60
	__GETD2N 0x43889333
	CALL __MULF12
	MOVW R26,R30
	MOVW R24,R22
	__GETD1N 0x43889333
	CALL SUBOPT_0x2D
	CALL SUBOPT_0x7C
;    1605 	}
_0x148:
;    1606 	return result;
	CALL SUBOPT_0x60
_0x4B7:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,14
	RET
;    1607 }
;    1608 
;    1609 flash double TLow[]={0.000000000000E+00,0.387481063640E-01,0.441944343470E-04,0.118443231050E-06,0.200329735540E-07,0.901380195590E-09,0.226511565930E-10,0.360711542050E-12,0.384939398830E-14,0.282135219250E-16,0.142515947790E-18,0.487686622860E-21,0.107955392700E-23,0.139450270620E-26,0.797951539270E-30};
;    1610 flash double THigh[]={0.000000000000E+00,0.387481063640E-01,0.332922278800E-04,0.206182434040E-06,-0.218822568460E-08,0.109968809280E-10,-0.308157587720E-13,0.454791352900E-16,-0.275129016730E-19};
;    1611 
;    1612 flash double SLow[]={0.000000000000E+00,0.540313308631E-02,0.125934289740E-04,-0.232477968689E-07,0.322028823036E-10,-0.331465196389E-13,0.255744251786E-16,-0.125068871393E-19,0.271443176145E-23};
;    1613 flash double SMed[]={0.132900444085E+01,0.334509311344E-02,0.654805192818E-05,-0.164856259209E-08,0.129989605174E-13};
;    1614 flash double SHigh[]={0.146628232636E+03,-0.258430516752E+00,0.163693574641E-03,-0.330439046987E-07,-0.943223690612E-14};
;    1615 
;    1616 flash double RLow[]={0.000000000000E+00,0.528961729765E-02,0.139166589782E-04,-0.238855693017E-07,0.356916001063E-10,-0.462347666298E-13,0.500777441034E-16,-0.373105886191E-19,0.157716482367E-22,-0.281038625251E-26};
;    1617 
;    1618 flash double RMed[]={0.295157925316E+01,-0.252061251332E-02,0.159564501865E-04,-0.764085947576E-08,0.205305291024E-11,-0.293359668173E-15};
;    1619 flash double RHigh[]={0.152232118209E+03,-0.268819888545E+00,0.171280280471E-03,-0.345895706453E-07,-0.934633971046E-14};
;    1620 
;    1621 flash double NLow[]={0.000000000000E+00,0.261591059620E-01,0.109574842280E-04,-0.938411115540E-07,-0.464120397590E-10,-0.263033577160E-11,-0.226534380030E-13,-0.760893007910E-16,-0.934196678350E-19};
;    1622 flash double NHigh[]={0.000000000000E+00,0.259293946010E-01,0.157101418800E-04,0.438256272370E-07,-0.252611697940E-09,0.643118193390E-12,-0.100634715190E-14,0.997453389920E-18,-0.608632456070E-21,0.208492293390E-24,-0.306821961510E-28};
;    1623 
;    1624 flash double JLow[]={0.000000000000E+00,0.503811878150E-01,0.304758369300E-04,-0.856810657200E-07,0.132281952950E-09,-0.170529583370E-12,0.209480906970E-15,-0.125383953360E-18,0.156317256970E-22};
;    1625 flash double JHigh[]={0.296456256810E+03,-0.149761277860E+01,0.317871039240E-02,-0.318476867010E-05,0.157208190040E-08,-0.306913690560E-12};
;    1626 
;    1627 flash double BLow[]={0.000000000000E+00,-0.24650818346E-03,0.59040421171E-05,-0.13257931636E-08,0.15668291901E-11,-0.16944529240E-14,0.62990347094E-18};
;    1628 flash double BHigh[]={-0.389381686210E+01,0.285717474700E-01,-0.848851047850E-04,0.157852801640E-06,-0.168353448640E-09,0.111097940130E-12,-0.445154310330E-16,0.989756408210E-20,-0.937913302890E-24};
;    1629 
;    1630 flash double EHigh[]={0.000000000000E+00,0.586655087100E-01,0.450322755820E-04,0.289084072120E-07,-0.330568966520E-09,0.650244032700E-12,-0.191974955040E-15,-0.125366004970E-17,0.214892175690E-20,-0.143880417820E-23,0.359608994810E-27};
;    1631 flash double ELow[]={0.000000000000E+00,0.586655087080E-01,0.454109771240E-04,-0.779980486860E-06,-0.258001608430E-07,-0.594525830570E-09,-0.932140586670E-11,-0.102876055340E-12,-0.803701236210E-15,-0.439794973910E-17,-0.164147763550E-19,-0.396736195160E-22,-0.558273287210E-25,-0.346578420130E-28};
;    1632 
;    1633 flash double KLow[]={0.000000000000E+00,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};
;    1634 flash double KHigh[]={-0.176004136860E-01,0.389212049750E-01,0.185587700320E-04,-0.994575928740E-07,0.318409457190E-09,-0.560728448890E-12,0.560750590590E-15,-0.320207200030E-18,0.971511471520E-22,-0.121047212750E-25};
;    1635 
;    1636 flash int TLowLen = sizeof(TLow)/sizeof(double);
;    1637 flash int THighLen = sizeof(THigh)/sizeof(double);
;    1638 
;    1639 flash int SLowLen = sizeof(SLow)/sizeof(double);
;    1640 flash int SMedLen = sizeof(SMed)/sizeof(double);
;    1641 flash int SHighLen = sizeof(SHigh)/sizeof(double);
;    1642 
;    1643 flash int RLowLen = sizeof(RLow)/sizeof(double);
;    1644 flash int RMedLen = sizeof(RMed)/sizeof(double);
;    1645 flash int RHighLen = sizeof(RHigh)/sizeof(double);
;    1646 
;    1647 flash int NLowLen = sizeof(NLow)/sizeof(double);
;    1648 flash int NHighLen = sizeof(NHigh)/sizeof(double);
;    1649 
;    1650 flash int JLowLen = sizeof(JLow)/sizeof(double);
;    1651 flash int JHighLen = sizeof(JHigh)/sizeof(double);
;    1652 
;    1653 flash int BLowLen = sizeof(BLow)/sizeof(double);
;    1654 flash int BHighLen = sizeof(BHigh)/sizeof(double);
;    1655 
;    1656 flash int ELowLen = sizeof(ELow)/sizeof(double);
;    1657 flash int EHighLen = sizeof(EHigh)/sizeof(double);
;    1658 
;    1659 flash int KLowLen = sizeof(KLow)/sizeof(double);
;    1660 flash int KHighLen = sizeof(KHigh)/sizeof(double);
;    1661 
;    1662 flash double *coef;

	.DSEG
_coef:
	.BYTE 0x2
;    1663 
;    1664 double GetThmoVolt(double t,char type)
;    1665 {

	.CSEG
_GetThmoVolt:
;    1666 	double result,pert;
;    1667 	int i;
;    1668 	int len = 0;
;    1669 	switch(type)
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
;    1670 	{
;    1671 		case PRBTYPE_T:
	CPI  R30,LOW(0x9)
	BRNE _0x14F
;    1672 			coef = (t < 0) ? TLow : THigh;
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
;    1673 			len = (t < 0) ? TLowLen : THighLen;
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
;    1674 			break;
	RJMP _0x14E
;    1675 		case PRBTYPE_K:
_0x14F:
	CPI  R30,LOW(0x3)
	BRNE _0x156
;    1676 			coef = (t < 0) ? KLow : KHigh;
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
;    1677 			len = (t < 0) ? KLowLen : KHighLen;
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
;    1678 			break;
	RJMP _0x14E
;    1679 		case PRBTYPE_N:
_0x156:
	CPI  R30,LOW(0x4)
	BRNE _0x15D
;    1680 			coef = (t < 0) ? NLow : NHigh;
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
;    1681 			len = (t < 0) ? NLowLen : NHighLen;
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
;    1682 			break;
	RJMP _0x14E
;    1683 		case PRBTYPE_E:
_0x15D:
	CPI  R30,LOW(0x5)
	BRNE _0x164
;    1684 			coef = (t < 0) ? ELow : EHigh;
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
;    1685 			len = (t < 0) ? ELowLen : EHighLen;
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
;    1686 			break;
	RJMP _0x14E
;    1687 		case PRBTYPE_B:
_0x164:
	CPI  R30,LOW(0x6)
	BRNE _0x16B
;    1688 			coef = (t < 630.615) ? BLow : BHigh;
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
;    1689 			len = (t < 630.615) ? BLowLen : BHighLen;
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
;    1690 			break;
	RJMP _0x14E
;    1691 		case PRBTYPE_J:
_0x16B:
	CPI  R30,LOW(0x7)
	BRNE _0x172
;    1692 			coef = (t < 760) ? JLow : JHigh;
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
;    1693 			len = (t < 760) ? JLowLen : JHighLen;
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
;    1694 			break;
	RJMP _0x14E
;    1695 
;    1696 		case PRBTYPE_S:
_0x172:
	CPI  R30,LOW(0x8)
	BRNE _0x179
;    1697 			coef = (t < 1064.18) ? SLow : ((t < 1664.5) ? SMed : SHigh);
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
;    1698 			len = (t < 1064.18) ? SLowLen : ((t < 1664.5) ? SMedLen : SHighLen);
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
;    1699 			break;
	RJMP _0x14E
;    1700 		case PRBTYPE_R:
_0x179:
	CPI  R30,LOW(0xA)
	BRNE _0x193
;    1701 			coef = (t < 1064.18) ? RLow : ((t < 1664.5) ? RMed : RHigh);
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
;    1702 			len = (t < 1064.18) ? RLowLen : ((t < 1664.5) ? RMedLen : RHighLen);
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
;    1703 			break;
	RJMP _0x14E
;    1704 		default:
_0x193:
;    1705 			return 0.0;
	CALL SUBOPT_0x86
	RJMP _0x4B6
;    1706 	}
_0x14E:
;    1707 	if (len == 0)
	MOV  R0,R18
	OR   R0,R19
	BRNE _0x194
;    1708 		return 0.0;
	CALL SUBOPT_0x86
	RJMP _0x4B6
;    1709 
;    1710 	result = coef[0];
_0x194:
	LDS  R30,_coef
	LDS  R31,_coef+1
	CALL __GETD1PF
	CALL SUBOPT_0x87
;    1711 	if(type == PRBTYPE_K)
	LDD  R26,Y+12
	CPI  R26,LOW(0x3)
	BRNE _0x195
;    1712 	{
;    1713 		result = result+(0.118597600000E+00)*exp((-0.118343200000E-03)*(t-0.126968600000E+03)*(t-0.126968600000E+03));
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
;    1714 	}
;    1715 
;    1716 	pert = t;
_0x195:
	__GETD1S 13
	CALL SUBOPT_0x8B
;    1717 	for(i=1;i< len ;i++)
	__GETWRN 16,17,1
_0x197:
	__CPWRR 16,17,18,19
	BRGE _0x198
;    1718 	{
;    1719 		result = result + coef[i] * t;
	MOVW R30,R16
	LDS  R26,_coef
	LDS  R27,_coef+1
	CALL SUBOPT_0x8C
	CALL __GETD1PF
	CALL SUBOPT_0x82
	CALL SUBOPT_0x8A
;    1720 		t = t*pert;
	CALL SUBOPT_0x8D
	CALL SUBOPT_0x82
	CALL __MULF12
	__PUTD1S 13
;    1721 	}
	__ADDWRN 16,17,1
	RJMP _0x197
_0x198:
;    1722 	return result;
	CALL SUBOPT_0x8E
_0x4B6:
	CALL __LOADLOCR4
	ADIW R28,17
	RET
;    1723 }
;    1724 
;    1725 double MValueToTValue(double r,char type)
;    1726 {
_MValueToTValue:
;    1727 	double tlow;
;    1728 	double tup;
;    1729 	double rnew;
;    1730 	double tnew;
;    1731 	int count = 0;
;    1732 	switch(type)
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
;    1733 	{
;    1734 		case PRBTYPE_T:
	CPI  R30,LOW(0x9)
	BRNE _0x19C
;    1735 			tlow =  -270;	tup = 400;
	CALL SUBOPT_0x8F
	__GETD1N 0x43C80000
	CALL SUBOPT_0x53
;    1736 			break;
	RJMP _0x19B
;    1737 		case PRBTYPE_K:
_0x19C:
	CPI  R30,LOW(0x3)
	BRNE _0x19D
;    1738 			tlow =  -270;	tup = 1372;
	CALL SUBOPT_0x8F
	__GETD1N 0x44AB8000
	CALL SUBOPT_0x53
;    1739 			break;
	RJMP _0x19B
;    1740 		case PRBTYPE_N:
_0x19D:
	CPI  R30,LOW(0x4)
	BRNE _0x19E
;    1741 			tlow =  -270;	tup = 1300;
	CALL SUBOPT_0x8F
	__GETD1N 0x44A28000
	CALL SUBOPT_0x53
;    1742 			break;
	RJMP _0x19B
;    1743 		case PRBTYPE_E:
_0x19E:
	CPI  R30,LOW(0x5)
	BRNE _0x19F
;    1744 			tlow =  -270;	tup = 1000;
	CALL SUBOPT_0x8F
	__GETD1N 0x447A0000
	CALL SUBOPT_0x53
;    1745 			break;
	RJMP _0x19B
;    1746 		case PRBTYPE_B:
_0x19F:
	CPI  R30,LOW(0x6)
	BRNE _0x1A0
;    1747 			tlow =  0;	tup = 1800;
	__CLRD1S 14
	__GETD1N 0x44E10000
	CALL SUBOPT_0x53
;    1748 			break;
	RJMP _0x19B
;    1749 		case PRBTYPE_J:
_0x1A0:
	CPI  R30,LOW(0x7)
	BRNE _0x1A1
;    1750 			tlow =  -210;	tup = 1200;
	__GETD1N 0xC3520000
	CALL SUBOPT_0x52
	__GETD1N 0x44960000
	CALL SUBOPT_0x53
;    1751 			break;
	RJMP _0x19B
;    1752 		case PRBTYPE_S:
_0x1A1:
	CPI  R30,LOW(0x8)
	BRNE _0x1A2
;    1753 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x90
;    1754 			break;
	RJMP _0x19B
;    1755 		case PRBTYPE_R:
_0x1A2:
	CPI  R30,LOW(0xA)
	BRNE _0x1A4
;    1756 			tlow =  -50;	tup = 1768;
	CALL SUBOPT_0x90
;    1757 			break;
	RJMP _0x19B
;    1758 		default:
_0x1A4:
;    1759 			return -9999.9999;
	__GETD1N 0xC61C4000
	RJMP _0x4B5
;    1760 	}
_0x19B:
;    1761 
;    1762 	while((tup - tlow > 0.00005) && (count++ < 100))
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
;    1763 	{
;    1764 		tnew = (tlow+tup)/2.0;
	CALL SUBOPT_0x58
;    1765 		rnew = GetThmoVolt(tnew,type);
	CALL SUBOPT_0x60
	CALL __PUTPARD1
	LDD  R30,Y+22
	ST   -Y,R30
	CALL _GetThmoVolt
	CALL SUBOPT_0x91
;    1766 
;    1767 		if(r < rnew)
	__GETD2S 19
	CALL __CMPF12
	BRSH _0x1AA
;    1768 			tup = tnew;
	CALL SUBOPT_0x70
;    1769 		else
	RJMP _0x1AB
_0x1AA:
;    1770 			tlow = tnew;
	CALL SUBOPT_0x61
;    1771 	}
_0x1AB:
	RJMP _0x1A5
_0x1A7:
;    1772 
;    1773 	return floor((tlow*10000.0+tup*10000.0)/2.0+0.5)/10000.0;
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
;    1774 }
;    1775 
;    1776 #include "font.h"
;    1777 
;    1778 #include <MATH.H>
;    1779 #include "typedef.h"
;    1780 #include "global.h"
;    1781 #include "utili.h"
;    1782 #include "key.h"
;    1783 extern uchar key;
;    1784 extern u8 lcd_buffer;
;    1785 /*
;    1786         Communication with LCD/KBD board,
;    1787         Incoming command format:
;    1788                 CMDI_CLEARBUF   reset the LCD buffer count. (like the ack of the command)
;    1789                 key:            key pressed
;    1790         Out command format:
;    1791                 CMDO_DISPLAYBTN,LED_STATUS      update the led status
;    1792 */
;    1793 /**************************主程序********************************/
;    1794 void Key_Init()
;    1795 {
_Key_Init:
;    1796 	key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    1797 }
	RET
;    1798 
;    1799 #include <string.h>
;    1800 #include "lcd.h"
;    1801 #include "font.h"
;    1802 #include "global.h"
;    1803 #include "utili.h"
;    1804 
;    1805 
;    1806 
;    1807 extern uchar key;
;    1808 static uchar kbd_state;

	.DSEG
_kbd_state_G8:
	.BYTE 0x1
;    1809 void kbd_uart_push(uchar data)
;    1810 {

	.CSEG
_kbd_uart_push:
;    1811         if(data == STATUS_IDLE||
;	data -> Y+0
;    1812            data == STATUS_ERR||
;    1813            data == STATUS_DRAW||
;    1814            data == STATUS_DONE) //lcd acknowledge
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
;    1815         {
;    1816                 kbd_state = data;
	LD   R30,Y
	STS  _kbd_state_G8,R30
;    1817                 return;
	RJMP _0x4B1
;    1818         }
;    1819         if(key == KEY_INVALID)
_0x1AC:
	LDI  R30,LOW(45)
	CP   R30,R5
	BRNE _0x1AF
;    1820                 key = data;
	LDD  R5,Y+0
;    1821 }
_0x1AF:
	RJMP _0x4B1
;    1822 static u8 lcdcmd[40];

	.DSEG
_lcdcmd_G8:
	.BYTE 0x28
;    1823 
;    1824 static u8 head[3] = {MARK_HEAD,MARK_HEAD,MARK_HEAD};
_head_G8:
	.BYTE 0x3
;    1825 static u8 tail[3] = {MARK_TAIL,MARK_TAIL,MARK_TAIL};
_tail_G8:
	.BYTE 0x3
;    1826 
;    1827 static u8 hex2ch[2];
_hex2ch_G8:
	.BYTE 0x2
;    1828 void hex2char(uchar a)
;    1829 {

	.CSEG
_hex2char:
;    1830         uchar h,l;
;    1831 	h = (a & 0xf0) >> 4;
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
;    1832 	l = (a & 0x0f);
	LDD  R30,Y+2
	ANDI R30,LOW(0xF)
	MOV  R16,R30
;    1833 	if(h <= 9)
	CPI  R17,10
	BRSH _0x1B2
;    1834 		hex2ch[0] = (h+'0');
	MOV  R30,R17
	SUBI R30,-LOW(48)
	RJMP _0x4C7
;    1835 	else
_0x1B2:
;    1836 		hex2ch[0] = (h+'A'-0x0a);
	MOV  R30,R17
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4C7:
	STS  _hex2ch_G8,R30
;    1837 	if(l <= 9)
	CPI  R16,10
	BRSH _0x1B4
;    1838 		hex2ch[1] = (l+'0');
	MOV  R30,R16
	SUBI R30,-LOW(48)
	RJMP _0x4C8
;    1839 	else
_0x1B4:
;    1840 		hex2ch[1] = (l+'A'-0x0a);
	MOV  R30,R16
	SUBI R30,-LOW(65)
	SUBI R30,LOW(10)
_0x4C8:
	__PUTB1MN _hex2ch_G8,1
;    1841 }
	RJMP _0x4B4
;    1842 void onecmd(uchar len)
;    1843 {
_onecmd:
;    1844      uchar i = 0;
;    1845      uchar sum;
;    1846      sum = 0;
	ST   -Y,R17
	ST   -Y,R16
;	len -> Y+2
;	i -> R17
;	sum -> R16
	LDI  R17,0
	LDI  R16,LOW(0)
;    1847      prints(head,3,PORT_KBD); //send head
	LDI  R30,LOW(_head_G8)
	LDI  R31,HIGH(_head_G8)
	CALL SUBOPT_0x92
;    1848      for(i = 0;i<len;i++)     //caculate cksum
	LDI  R17,LOW(0)
_0x1B7:
	LDD  R30,Y+2
	CP   R17,R30
	BRSH _0x1B8
;    1849      {
;    1850         hex2char(lcdcmd[i]);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
;    1851 	prints(hex2ch,2,PORT_KBD);
;    1852         sum = sum + lcdcmd[i];
	CALL SUBOPT_0x93
	ADD  R16,R30
;    1853      }
	SUBI R17,-1
	RJMP _0x1B7
_0x1B8:
;    1854      lcdcmd[i] = 0xff - sum;
	MOV  R26,R17
	LDI  R27,0
	SUBI R26,LOW(-_lcdcmd_G8)
	SBCI R27,HIGH(-_lcdcmd_G8)
	LDI  R30,LOW(255)
	SUB  R30,R16
	ST   X,R30
;    1855      hex2char(lcdcmd[i]);
	CALL SUBOPT_0x93
	CALL SUBOPT_0x94
;    1856      prints(hex2ch,2,PORT_KBD);
;    1857      prints(tail,3,PORT_KBD); //send tail
	LDI  R30,LOW(_tail_G8)
	LDI  R31,HIGH(_tail_G8)
	CALL SUBOPT_0x92
;    1858 }
_0x4B4:
	LDD  R17,Y+1
	LDD  R16,Y+0
	ADIW R28,3
	RET
;    1859 static u8 idle[10] = {MARK_HEAD,MARK_HEAD,MARK_HEAD,'1','2','E','D',MARK_TAIL,MARK_TAIL,MARK_TAIL};

	.DSEG
_idle_G8:
	.BYTE 0xA
;    1860 void sendcmd(uchar len)
;    1861 {

	.CSEG
_sendcmd:
;    1862      ulong timeout;
;    1863      if(DEBUG == 1)
	SBIW R28,4
;	len -> Y+4
;	timeout -> Y+0
;    1864         return;
;    1865      while(kbd_state != STATUS_IDLE)
_0x1BB:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1BD
;    1866      {
;    1867         if(kbd_state == STATUS_DRAW)
	CPI  R26,LOW(0xF2)
	BREQ _0x1BB
;    1868                 continue;
;    1869         if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1BF
;    1870         {
;    1871                 prints(idle,10,PORT_KBD);
	LDI  R30,LOW(_idle_G8)
	LDI  R31,HIGH(_idle_G8)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(10)
	ST   -Y,R30
	LDI  R30,LOW(4)
	ST   -Y,R30
	CALL _prints
;    1872                 kbd_state = STATUS_DRAW;
	LDI  R30,LOW(242)
	STS  _kbd_state_G8,R30
;    1873         }
;    1874      }
_0x1BF:
	RJMP _0x1BB
_0x1BD:
;    1875      onecmd(len);
	CALL SUBOPT_0x95
;    1876      timeout = 0;
;    1877      kbd_state = STATUS_DONE;
	LDI  R30,LOW(243)
	STS  _kbd_state_G8,R30
;    1878      while(kbd_state != STATUS_IDLE)
_0x1C0:
	LDS  R26,_kbd_state_G8
	CPI  R26,LOW(0xF0)
	BREQ _0x1C2
;    1879      {
;    1880          if(kbd_state == STATUS_ERR)
	CPI  R26,LOW(0xF1)
	BRNE _0x1C3
;    1881          {
;    1882                 onecmd(len);
	CALL SUBOPT_0x95
;    1883                 timeout = 0;
;    1884          }
;    1885          if(timeout < 655350)
_0x1C3:
	CALL SUBOPT_0x1
	__CPD2N 0x9FFF6
	BRSH _0x1C4
;    1886          {
;    1887                 sleepms(1);
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x38
;    1888                 timeout++;
	CALL SUBOPT_0x2
	CALL SUBOPT_0x48
	__PUTD1S 0
;    1889          }else{
	RJMP _0x1C5
_0x1C4:
;    1890                 onecmd(len);
	CALL SUBOPT_0x95
;    1891                 timeout = 0;
;    1892          }
_0x1C5:
;    1893      }
	RJMP _0x1C0
_0x1C2:
;    1894 }
_0x4B3:
	ADIW R28,5
	RET
;    1895 /*------------------初始化-----------------*/
;    1896 void LCD_Init (void)
;    1897 {
_LCD_Init:
;    1898         kbd_state = STATUS_IDLE;
	LDI  R30,LOW(240)
	STS  _kbd_state_G8,R30
;    1899         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x96
;    1900         sendcmd(1);
;    1901         //do it twice avoid the missing of first command on boot up
;    1902         lcdcmd[0] = CMDO_LCD_INIT;
	CALL SUBOPT_0x96
;    1903         sendcmd(1);
;    1904 
;    1905 }
	RET
;    1906 
;    1907 /********************************************************/
;    1908 /* 清屏							*/
;    1909 /********************************************************/
;    1910 void LCD_Cls(void)
;    1911 {
_LCD_Cls:
;    1912         lcdcmd[0] = CMDO_LCD_CLS;
	LDI  R30,LOW(4)
	STS  _lcdcmd_G8,R30
;    1913         sendcmd(1);
	LDI  R30,LOW(1)
	ST   -Y,R30
	CALL _sendcmd
;    1914 }
	RET
;    1915 void LCD_ClsBlock(uchar x1,uchar y1,uchar x2,uchar y2)
;    1916 {
_LCD_ClsBlock:
;    1917         lcdcmd[0] = CMDO_LCD_CLSBLK;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(6)
	CALL SUBOPT_0x97
;    1918         lcdcmd[1] = x1;
;    1919         lcdcmd[2] = y1;
;    1920         lcdcmd[3] = x2;
;    1921         lcdcmd[4] = y2;
;    1922         sendcmd(5);
;    1923 }
	RJMP _0x4B2
;    1924 void display_buttons(uchar btn_pos,uchar btn_val)   //send led status to sub board
;    1925 {
_display_buttons:
;    1926         lcdcmd[0] = CMDO_DISPLAYBTN;
;	btn_pos -> Y+1
;	btn_val -> Y+0
	LDI  R30,LOW(2)
	STS  _lcdcmd_G8,R30
;    1927         if(btn_val == 0)
	LD   R30,Y
	CPI  R30,0
	BRNE _0x1C6
;    1928 	      btn_pos += 4; //change from 'a'-'d' to 'e'-'h'   //add 4
	LDD  R30,Y+1
	SUBI R30,-LOW(4)
	STD  Y+1,R30
;    1929         lcdcmd[1] = btn_pos;
_0x1C6:
	LDD  R30,Y+1
	__PUTB1MN _lcdcmd_G8,1
;    1930         sendcmd(2);
	LDI  R30,LOW(2)
	ST   -Y,R30
	CALL _sendcmd
;    1931 }
	ADIW R28,2
	RET
;    1932 
;    1933 
;    1934 /*	设定图形x,y值*/
;    1935 /*
;    1936 void LCD_GraphSetAddr(uchar x,uchar y)
;    1937 {
;    1938 	uint xy;
;    1939 	xy=y;
;    1940 	xy=xy*16+x+256;
;    1941   	//LCD_CE(0);
;    1942 	LCD_Write2(xy&0xff,xy/256,0x24);
;    1943 }
;    1944 */
;    1945 /*	设定文本x,y值	*/
;    1946 /*
;    1947 void LCD_TextSetAddr(uchar x,uchar y)
;    1948 {
;    1949   	//LCD_CE(0);
;    1950 	LCD_Write2(y*16+x,0,0x24);
;    1951 }
;    1952 */
;    1953 /*	清除一点*/
;    1954 /*
;    1955 void LCD_ClrPixel(uchar x,uchar y)
;    1956 {
;    1957 	uchar b;
;    1958 	uint addr = LCD_GRAPH_HOME_ADDR + y * LCD_WIDTH + x / 8;
;    1959 
;    1960 	b = 7 - (x % 8);
;    1961 
;    1962 	LCD_WriteInt(addr,0x24);
;    1963 	LCD_Comm(0xf0|b);
;    1964 }
;    1965 */
;    1966 /*	点亮一点	*/
;    1967 /*
;    1968 void LCD_Putpixel(uchar x,uchar y)
;    1969 {
;    1970 	uchar b;
;    1971 	uint addr = LCD_GRAPH_HOME_ADDR + ((uint)y * LCD_WIDTH) + x>>3;
;    1972 
;    1973 	b = 7 - (x % 8);
;    1974 
;    1975 	LCD_WriteInt(addr,0x24);
;    1976 	LCD_Comm(0xf8|b);
;    1977 }
;    1978 */
;    1979 /*	x,y处显示光标	*/
;    1980 /*
;    1981 void LCD_ShowCursor(uchar x,uchar y)
;    1982 {
;    1983 	return;
;    1984   	//LCD_CE(0);
;    1985 	LCD_Comm(0x97);	//光标开
;    1986 	LCD_Write2(x,y,0x21);
;    1987 }
;    1988 
;    1989 //	取消光标
;    1990 
;    1991 void LCD_HideCursor(void)
;    1992 {
;    1993 	return;
;    1994   	//LCD_CE(0);
;    1995 	LCD_Comm(0x9c);
;    1996 }
;    1997 */
;    1998 /*
;    1999 void LCD_PrintNumStr(uchar x,uchar y,uchar *s)
;    2000 {
;    2001 	x =(x / 8) * 8;
;    2002 
;    2003 	while(*s)
;    2004 	{
;    2005 		LCD_PutImg(x,y,1,11,Num_Tab + (*s - '0') * 11);
;    2006 
;    2007 		x = x + 8;
;    2008 
;    2009 		s++;
;    2010 	}
;    2011 }
;    2012 
;    2013 
;    2014 void LCD_PrintBlackBlock(uchar x,uchar y,bool not_empty)
;    2015 {
;    2016 	x =(x / 8) * 8;
;    2017 	LCD_PutImg(x,y,1,11,BlockTab + (not_empty?0:1) * 11);
;    2018 }
;    2019 */
;    2020 
;    2021 void LCD_ReverseRect(uchar x,uchar y,uchar w,uchar h)
;    2022 {
_LCD_ReverseRect:
;    2023         lcdcmd[0] = CMDO_LCD_REVERSE;
;	x -> Y+3
;	y -> Y+2
;	w -> Y+1
;	h -> Y+0
	LDI  R30,LOW(5)
	CALL SUBOPT_0x97
;    2024         lcdcmd[1] = x;
;    2025         lcdcmd[2] = y;
;    2026         lcdcmd[3] = w;
;    2027         lcdcmd[4] = h;
;    2028         sendcmd(5);
;    2029 }
	RJMP _0x4B2
;    2030 /*--------------显示字符------------------*/
;    2031 /*
;    2032 void LCD_TextPutchar(uchar x,uchar y,uchar c)
;    2033 {
;    2034   	//LCD_CE(0);
;    2035   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2036   	LCD_Comm(0xb0);
;    2037 	LCD_AutoWrite(c - 0x20);
;    2038   	LCD_Comm(0xb2);
;    2039 }
;    2040 
;    2041 void LCD_TextPrint(uchar x,uchar y,char *s)
;    2042 {
;    2043   	//LCD_CE(0);
;    2044   	LCD_WriteInt(LCD_TEXT_HOME_ADDR + LCD_WIDTH * y + x,0x24);
;    2045   	LCD_Comm(0xb0);
;    2046   	while(*s)
;    2047   	{
;    2048 		LCD_AutoWrite(*s - 0x20);
;    2049 		s++;
;    2050 	}
;    2051   	LCD_Comm(0xb2);
;    2052 }
;    2053 void LCD_TextPrintWord(uchar x,uchar y,uint16 n,uchar start)
;    2054 {
;    2055 	uchar buf[4];
;    2056 	int i;
;    2057 
;    2058 	if(start > 3) start = 0;
;    2059 
;    2060 	for(i = 0; i < 4; i++)
;    2061 	{
;    2062 		buf[i] = n % 10;
;    2063 		n /= 10;
;    2064 	}
;    2065 
;    2066 	for(i = 3-start; i >= 0; i--)
;    2067 	{
;    2068 		LCD_TextPutchar(x,y,'0' + buf[i]);
;    2069 		x ++;
;    2070 	}
;    2071 }
;    2072 
;    2073 void LCD_TextPrintHex(uchar x,uchar y,uchar hex)
;    2074 {
;    2075 	uchar c1,c2;
;    2076 	//低4位
;    2077 	c1 = hex & 0x0f;	//lo
;    2078 	//高4位
;    2079 	c2 = (hex >> 4) & 0x0f; //hi
;    2080 
;    2081 	LCD_TextPutchar(x,y,HexTable[c2]);
;    2082 	LCD_TextPutchar(x+1,y,HexTable[c1]);
;    2083 }
;    2084 */
;    2085 /************************************************/
;    2086 /*画线。任意方向的斜线,直线数学方程 aX+bY=1	*/
;    2087 /************************************************/
;    2088 /*
;    2089 void LCD_Linexy(uchar x0,uchar y0,uchar xt,uchar yt)
;    2090 {
;    2091 	register uchar t;
;    2092 	int xerr=0,yerr=0,delta_x,delta_y,distance;
;    2093 	int incx,incy,uRow,uCol;
;    2094 
;    2095 	delta_x = xt-x0;				//计算坐标增量
;    2096 	delta_y = yt-y0;
;    2097 	uRow = x0;
;    2098 	uCol = y0;
;    2099 	if(delta_x>0) incx=1;				//设置单步方向
;    2100 	else if( delta_x==0 ) incx=0;			//垂直线
;    2101 	else {incx=-1;delta_x=-delta_x;}
;    2102 
;    2103 	if(delta_y>0) incy=1;
;    2104 	else if( delta_y==0 ) incy=0;			//水平线
;    2105 	else {incy=-1;delta_y=-delta_y;}
;    2106 
;    2107 	if( delta_x > delta_y )	distance=delta_x;	//选取基本增量坐标轴
;    2108 	else distance=delta_y;
;    2109 
;    2110   	//LCD_CE(0);
;    2111 	for( t=0;t <= distance+1; t++ )
;    2112         {					//画线输出
;    2113 		LCD_Putpixel(uRow,uCol);			//画点
;    2114 		xerr +=	delta_x	;
;    2115 		yerr +=	delta_y	;
;    2116 
;    2117 		if( xerr > distance )
;    2118                	{
;    2119 			xerr-=distance;
;    2120 			uRow+=incx;
;    2121 		}
;    2122 		if( yerr > distance )
;    2123                	{
;    2124 			yerr-=distance;
;    2125 			uCol+=incy;
;    2126 		}
;    2127 	}
;    2128 }
;    2129 */
;    2130 /*
;    2131 void LCD_LineH(uchar y)
;    2132 {
;    2133 	char i;
;    2134 
;    2135   	//LCD_CE(0);
;    2136   	LCD_WriteInt(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * y ,0x24);
;    2137 	LCD_Comm(0xb0);
;    2138 	for(i=0;i<LCD_WIDTH;i++)
;    2139  		LCD_AutoWrite(0xff);
;    2140 	LCD_Comm(0xb2);
;    2141 }
;    2142 */
;    2143 /*
;    2144 void LCD_LineV(uchar x,uchar y1,uchar y2)
;    2145 {
;    2146 	int i;
;    2147   	//LCD_CE(0);
;    2148 	for(i = y1; i < y2; i++)
;    2149 	{
;    2150 		LCD_Putpixel(x,i);
;    2151 	}
;    2152 }
;    2153 */
;    2154 
;    2155 void LCD_Rectange(uchar x1,uchar y1,uchar x2,uchar y2)
;    2156 {
_LCD_Rectange:
;    2157         lcdcmd[0] = CMDO_LCD_RECT;
;	x1 -> Y+3
;	y1 -> Y+2
;	x2 -> Y+1
;	y2 -> Y+0
	LDI  R30,LOW(7)
	CALL SUBOPT_0x97
;    2158         lcdcmd[1] = x1;
;    2159         lcdcmd[2] = y1;
;    2160         lcdcmd[3] = x2;
;    2161         lcdcmd[4] = y2;
;    2162         sendcmd(5);
;    2163 }
	RJMP _0x4B2
;    2164 void LCD_PrintChar(uchar cmd, uchar x,uchar y,uchar *s)
;    2165 {
_LCD_PrintChar:
;    2166         uchar pos = 0;
;    2167         lcdcmd[0] = cmd;
	ST   -Y,R17
;	cmd -> Y+5
;	x -> Y+4
;	y -> Y+3
;	*s -> Y+1
;	pos -> R17
	LDI  R17,0
	LDD  R30,Y+5
	STS  _lcdcmd_G8,R30
;    2168         lcdcmd[1] = x;
	LDD  R30,Y+4
	__PUTB1MN _lcdcmd_G8,1
;    2169         lcdcmd[2] = y;
	LDD  R30,Y+3
	__PUTB1MN _lcdcmd_G8,2
;    2170         pos = 1;
	LDI  R17,LOW(1)
;    2171         while(*s)
_0x1C7:
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X
	CPI  R30,0
	BREQ _0x1C9
;    2172         {
;    2173                 lcdcmd[(uchar)(pos + 3)] = *s++;
	CALL SUBOPT_0x98
	MOVW R0,R30
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	LD   R30,X+
	STD  Y+1,R26
	STD  Y+1+1,R27
	MOVW R26,R0
	ST   X,R30
;    2174                 pos = pos + 1;
	SUBI R17,-LOW(1)
;    2175         }
	RJMP _0x1C7
_0x1C9:
;    2176         lcdcmd[(uchar)(pos+3)] = 0x00;
	CALL SUBOPT_0x98
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2177         lcdcmd[3] = pos;
	__POINTW1MN _lcdcmd_G8,3
	ST   Z,R17
;    2178         pos = pos + 4;
	SUBI R17,-LOW(4)
;    2179         sendcmd(pos);
	ST   -Y,R17
	CALL _sendcmd
;    2180 }
	LDD  R17,Y+0
	ADIW R28,6
	RET
;    2181 void LCD_PrintHz12(uchar x,uchar y,uchar *s)
;    2182 {
_LCD_PrintHz12:
;    2183         LCD_PrintChar(CMDO_LCD_HZ12,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(8)
	CALL SUBOPT_0x99
;    2184 }
	RJMP _0x4B2
;    2185 void LCD_PrintHz16(uchar x,uchar y,uchar *s)
;    2186 {
_LCD_PrintHz16:
;    2187         LCD_PrintChar(CMDO_LCD_HZ16,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(9)
	CALL SUBOPT_0x99
;    2188 }
	RJMP _0x4B2
;    2189 //显示6x8的数字
;    2190 void LCD_Print6X8(uchar x, uchar y,uchar *s)
;    2191 {
_LCD_Print6X8:
;    2192         LCD_PrintChar(CMDO_LCD_68,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(16)
	CALL SUBOPT_0x99
;    2193 }
	RJMP _0x4B2
;    2194 
;    2195 //显示6x8的数字
;    2196 void LCD_Print8X16(uchar x, uchar y,uchar *s)
;    2197 {
_LCD_Print8X16:
;    2198         LCD_PrintChar(CMDO_LCD_816,x,y,s);
;	x -> Y+3
;	y -> Y+2
;	*s -> Y+0
	LDI  R30,LOW(17)
	CALL SUBOPT_0x99
;    2199 }
_0x4B2:
	ADIW R28,4
	RET
;    2200 
;    2201 //显示24x32的数字
;    2202 /*
;    2203 void LCD_Print24X32(uchar x, uchar y,uchar *s)
;    2204 {
;    2205 	//x坐标必需是8位对齐
;    2206 	x =(x / 8) * 8;
;    2207 	while(*s)
;    2208 	{
;    2209 		if( *s >= '0' && *s <= '9')	//显示
;    2210 		{
;    2211 			LCD_PutImg(x,y,3,32,Font24X32[*s-'0']);
;    2212 			x += 24;
;    2213 		}
;    2214 		else if( *s == ' ')
;    2215 		{
;    2216 			x += 8;
;    2217 		}
;    2218 		else if( *s == '-')
;    2219 		{
;    2220 			LCD_PutImg(x,y,3,32,Font24X32[12]);
;    2221 			x += 24;
;    2222 		}
;    2223 		else
;    2224 		{
;    2225 			LCD_PutImg(x,y+16,1,16,ASC8x16[*s]);	//
;    2226 			x += 8;
;    2227 		}
;    2228 		s ++;
;    2229 	}
;    2230 }*/
;    2231 #include "utili.h"
;    2232 #include "scanner.h"
;    2233 #include "stdlib.h"
;    2234 
;    2235 static uchar navlen = 0;

	.DSEG
_navlen_G9:
	.BYTE 0x1
;    2236 static double reading = -1000;
_reading_G9:
	.BYTE 0x4
;    2237 static char navread[20];
_navread_G9:
	.BYTE 0x14
;    2238 static char navread2[20];
_navread2_G9:
	.BYTE 0x14
;    2239 u8 scancmd[5];
_scancmd:
	.BYTE 0x5
;    2240 u8 eeprom scanner_type = 1; //1: MI, 2: GUIDLINE

	.ESEG
_scanner_type:
	.DB  0x1
;    2241 void scanner_set_mode()
;    2242 {

	.CSEG
_scanner_set_mode:
;    2243         if (IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x1CB
;    2244         {
;    2245             scancmd[0] = '#';
	LDI  R30,LOW(35)
	RJMP _0x4C9
;    2246         }else{
_0x1CB:
;    2247             scancmd[0]= '!';
	LDI  R30,LOW(33)
_0x4C9:
	STS  _scancmd,R30
;    2248         }
;    2249         scancmd[1] = 0x0D;
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2250         scancmd[2] = 0x0A;
	__POINTW1MN _scancmd,2
	CALL SUBOPT_0x9A
;    2251         prints(scancmd,3,PORT_SCANNER);
	LDI  R30,LOW(3)
	CALL SUBOPT_0x9B
;    2252 }
	RET
;    2253 void scanner_set_channel(uchar ch)
;    2254 {
_scanner_set_channel:
;    2255         if(ch == 1)
;	ch -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1CD
;    2256         {
;    2257                 SET_TOCH1;
	CBI  0x18,4
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	CALL SUBOPT_0x38
	SBI  0x18,4
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    2258         }
;    2259         if(ch == 2)
_0x1CD:
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1CE
;    2260         {
;    2261                 SET_TOCH2;
	CBI  0x18,5
	LDI  R30,LOW(600000)
	LDI  R31,HIGH(600000)
	CALL SUBOPT_0x38
	SBI  0x18,5
	LDI  R30,LOW(255)
	OUT  0x18,R30
;    2262                 return;
	RJMP _0x4B1
;    2263         }
;    2264         if(scanner_type == 1) //MI
_0x1CE:
	CALL SUBOPT_0x9C
	CPI  R30,LOW(0x1)
	BRNE _0x1CF
;    2265         {
;    2266                 if(ch < 10)
	LD   R26,Y
	CPI  R26,LOW(0xA)
	BRSH _0x1D0
;    2267                 {
;    2268                         scancmd[0] = (ch + '0');
	LD   R30,Y
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2269                         scancmd[1] = 'A';
	__POINTW1MN _scancmd,1
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    2270                         scancmd[2] = 0x0D;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(13)
	STD  Z+0,R26
;    2271                         scancmd[3] = 0x0A;
	__POINTW1MN _scancmd,3
	CALL SUBOPT_0x9A
;    2272                         prints(scancmd,4,PORT_SCANNER);
	LDI  R30,LOW(4)
	CALL SUBOPT_0x9B
;    2273                         return;
	RJMP _0x4B1
;    2274                 }else{
_0x1D0:
;    2275                         scancmd[0] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	STS  _scancmd,R30
;    2276                         ch = ch % 10;
	CALL SUBOPT_0x9D
;    2277                         scancmd[1] = ch + '0';
	__PUTB1MN _scancmd,1
;    2278                         scancmd[2] = 'A'; scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	__POINTW1MN _scancmd,2
	LDI  R26,LOW(65)
	STD  Z+0,R26
	CALL SUBOPT_0x9E
;    2279                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x9B
;    2280                 }
;    2281         }
;    2282         if(scanner_type == 2) //guidline
_0x1CF:
	CALL SUBOPT_0x9C
	CPI  R30,LOW(0x2)
	BRNE _0x1D2
;    2283         {
;    2284                         scancmd[0] = 'A';
	LDI  R30,LOW(65)
	STS  _scancmd,R30
;    2285                         scancmd[1] = (u8)(ch / 10) + '0';
	LD   R26,Y
	LDI  R30,LOW(10)
	CALL __DIVB21U
	SUBI R30,-LOW(48)
	__PUTB1MN _scancmd,1
;    2286                         ch = ch % 10;
	CALL SUBOPT_0x9D
;    2287                         scancmd[2] = ch + '0';
	__PUTB1MN _scancmd,2
;    2288                         scancmd[3] = 0x0D; scancmd[4] = 0x0A;
	CALL SUBOPT_0x9E
;    2289                         prints(scancmd,5,PORT_SCANNER);
	LDI  R30,LOW(5)
	CALL SUBOPT_0x9B
;    2290         }
;    2291 }
_0x1D2:
	RJMP _0x4B1
;    2292 
;    2293 
;    2294 //incoming data handler of scanner
;    2295 void scanner_uart_push(uchar data)
;    2296 {
_scanner_uart_push:
;    2297 }
	RJMP _0x4B1
;    2298 //incoming data hander of PC
;    2299 void pc_uart_push(uchar data)
;    2300 {
_pc_uart_push:
;    2301 }
	RJMP _0x4B1
;    2302 static uchar navcmd[12];

	.DSEG
_navcmd_G9:
	.BYTE 0xC
;    2303 #define NAV_INVALID     -1000
;    2304 #define NAV_VALID       -2000
;    2305 #define RESETNAV       navlen = 0;   reading = NAV_INVALID;
;    2306 
;    2307 void nav_command(uchar cmd)
;    2308 {

	.CSEG
_nav_command:
;    2309         if(cmd == NAV_30V)
;	cmd -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x2)
	BRNE _0x1D3
;    2310                sprintf(navcmd,"%%01;12;02\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,87
	CALL SUBOPT_0x32
;    2311         if(cmd == NAV_1V)
_0x1D3:
	LD   R26,Y
	CPI  R26,LOW(0x3)
	BRNE _0x1D4
;    2312                sprintf(navcmd,"%%01;12;01\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,99
	CALL SUBOPT_0x32
;    2313         if(cmd == NAV_120MV)
_0x1D4:
	LD   R26,Y
	CPI  R26,LOW(0x4)
	BRNE _0x1D5
;    2314                sprintf(navcmd,"%%01;12;00\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,111
	CALL SUBOPT_0x32
;    2315         if(cmd == NAV_AFLTOFF)
_0x1D5:
	LD   R26,Y
	CPI  R26,LOW(0x5)
	BRNE _0x1D6
;    2316                sprintf(navcmd,"%%01;26\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,123
	CALL SUBOPT_0x32
;    2317         if(cmd == NAV_AFLTON)
_0x1D6:
	LD   R26,Y
	CPI  R26,LOW(0x6)
	BRNE _0x1D7
;    2318                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,132
	CALL SUBOPT_0x32
;    2319         if(cmd == NAV_SLOWMODE)
_0x1D7:
	LD   R26,Y
	CPI  R26,LOW(0x7)
	BRNE _0x1D8
;    2320                sprintf(navcmd,"%%01;27\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,132
	CALL SUBOPT_0x32
;    2321         if(cmd == NAV_ZEROON)
_0x1D8:
	LD   R26,Y
	CPI  R26,LOW(0x8)
	BRNE _0x1D9
;    2322         {
;    2323                sprintf(navcmd,"%%01;06\r");
	CALL SUBOPT_0x9F
	__POINTW1FN _0,141
	CALL SUBOPT_0x32
;    2324         }
;    2325         if(cmd == NAV_INIT)
_0x1D9:
	LD   R26,Y
	CPI  R26,LOW(0x1)
	BRNE _0x1DA
;    2326         {
;    2327                 RESETNAV;
	CALL SUBOPT_0xA0
;    2328                 sprintf(navcmd,"%%01;00\r");
	__POINTW1FN _0,150
	CALL SUBOPT_0x32
;    2329         }
;    2330         if(cmd == NAV_READ)
_0x1DA:
	LD   R26,Y
	CPI  R26,LOW(0x9)
	BRNE _0x1DB
;    2331         {
;    2332                 RESETNAV;
	CALL SUBOPT_0xA0
;    2333                 sprintf(navcmd,"%%01;01\r");
	__POINTW1FN _0,159
	CALL SUBOPT_0x32
;    2334         }
;    2335 //                com1_putc('?');
;    2336         prints(navcmd,strlen(navcmd),PORT_NAV);
_0x1DB:
	CALL SUBOPT_0x9F
	CALL SUBOPT_0x9F
	CALL _strlen
	ST   -Y,R30
	LDI  R30,LOW(5)
	ST   -Y,R30
	CALL _prints
;    2337 }
	RJMP _0x4B1
;    2338 
;    2339 
;    2340 unsigned long navtime;

	.DSEG
_navtime:
	.BYTE 0x4
;    2341 double nav_read()
;    2342 {

	.CSEG
_nav_read:
;    2343         if(DEBUG == 1)
;    2344                 return 1.0;
;    2345         nav_command(NAV_READ);
	CALL SUBOPT_0xA1
;    2346         navtime = 0;
;    2347         while(1)
_0x1DD:
;    2348         {
;    2349                 if(reading < NAV_INVALID) //make sure it is a valid reading
	LDS  R26,_reading_G9
	LDS  R27,_reading_G9+1
	LDS  R24,_reading_G9+2
	LDS  R25,_reading_G9+3
	CALL SUBOPT_0xA2
	CALL __CMPF12
	BRSH _0x1E0
;    2350                 {
;    2351                      reading = atof(navread2);
	LDI  R30,LOW(_navread2_G9)
	LDI  R31,HIGH(_navread2_G9)
	ST   -Y,R31
	ST   -Y,R30
	CALL _atof
	CALL SUBOPT_0xA3
;    2352                      return reading;
	LDS  R30,_reading_G9
	LDS  R31,_reading_G9+1
	LDS  R22,_reading_G9+2
	LDS  R23,_reading_G9+3
	RET
;    2353                 }
;    2354                 if(navtime++ > 655350)
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
;    2355                 {
;    2356                      nav_command(NAV_READ);
	CALL SUBOPT_0xA1
;    2357                      navtime = 0;
;    2358                 }
;    2359                 sleepms(1);
_0x1E1:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0x38
;    2360         }
	RJMP _0x1DD
;    2361         return 1.0;
;    2362 }
;    2363 //incoming data hander of navameter
;    2364 void nav_uart_push(uchar data)
;    2365 {
_nav_uart_push:
;    2366         if(navlen >= 19)
;	data -> Y+0
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x13)
	BRLO _0x1E2
;    2367         {
;    2368                 RESETNAV;
	CALL SUBOPT_0xA5
;    2369         }
;    2370         if(((data >= '0') && (data <= '9')) ||
_0x1E2:
;    2371                 (data == '.') ||
;    2372                 (data == '-') ||
;    2373                 (data == '+') ||
;    2374                 (data == 'e') ||
;    2375                 (data == 'E') )
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
;    2376         {
;    2377                 navread[navlen++] = data;
	LDS  R30,_navlen_G9
	SUBI R30,-LOW(1)
	STS  _navlen_G9,R30
	CALL SUBOPT_0x4E
	SUBI R30,LOW(-_navread_G9)
	SBCI R31,HIGH(-_navread_G9)
	LD   R26,Y
	STD  Z+0,R26
;    2378                 reading = NAV_INVALID;
	CALL SUBOPT_0xA2
	CALL SUBOPT_0xA3
;    2379                 return;
	RJMP _0x4B1
;    2380         }
;    2381 
;    2382         if(navlen < 4) //not enough digits
_0x1E3:
	LDS  R26,_navlen_G9
	CPI  R26,LOW(0x4)
	BRSH _0x1E8
;    2383         {
;    2384                 RESETNAV;
	CALL SUBOPT_0xA5
;    2385                 return;
	RJMP _0x4B1
;    2386         }
;    2387         navread[navlen] = '\0';
_0x1E8:
	CALL SUBOPT_0xA6
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    2388         do
_0x1EA:
;    2389         {
;    2390                 navread2[navlen] = navread[navlen];
	LDS  R26,_navlen_G9
	LDI  R27,0
	SUBI R26,LOW(-_navread2_G9)
	SBCI R27,HIGH(-_navread2_G9)
	CALL SUBOPT_0xA6
	LD   R30,Z
	ST   X,R30
;    2391         }while(navlen-- > 0);
	LDS  R26,_navlen_G9
	SUBI R26,LOW(1)
	STS  _navlen_G9,R26
	SUBI R26,-LOW(1)
	CPI  R26,LOW(0x1)
	BRSH _0x1EA
;    2392         navlen = 0;
	LDI  R30,LOW(0)
	STS  _navlen_G9,R30
;    2393         reading =  NAV_VALID; //valid data
	__GETD1N 0xC4FA0000
	CALL SUBOPT_0xA3
;    2394 }
_0x4B1:
	ADIW R28,1
	RET
;    2395 #include <mega128.h>
;    2396 #include "lcd.h"
;    2397 #include "key.h"
;    2398 #include "utili.h"
;    2399 #include "window.h"
;    2400 #include "utili.h"
;    2401 #include "scanner.h"
;    2402 
;    2403 extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
;    2404 #define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
;    2405                 		if(new_page == 1){\
;    2406 		                	for(i = min_option; i <= max_option; i++){\
;    2407 				                j = ((curr_pos-1)/max_option)*max_option + i;
;    2408 
;    2409 #define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
;    2410 				        if((curr_pos-1) % max_option == (i-1))\
;    2411 					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2412         			        }\
;    2413                                 }else{\
;    2414 	                		for(i = min_option; i <= max_option; i++) {\
;    2415         		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2416 				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
;    2417 			                }\
;    2418 		                }
;    2419 
;    2420 #define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
;    2421 			if(curr_pos > max_index) curr_pos = min_index;\
;    2422 			if(last_pos == curr_pos)	return;\
;    2423 			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}
;    2424 
;    2425 #define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
;    2426 			if(curr_pos < min_index) curr_pos = max_index;\
;    2427 			if(last_pos == curr_pos)  return;\
;    2428 			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}
;    2429 
;    2430 #define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
;    2431 					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
;    2432 
;    2433 #define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
;    2434                                         draw_label(&PRE##options[i-1], SW_NORMAL);\
;    2435 			        }
;    2436 #define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
;    2437                                         return;\
;    2438         			for(i = min_option;i <= max_option; i++){\
;    2439 	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2440         				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
;    2441         			}
;    2442 
;    2443 #define INC_OPTION	last_sel = curr_sel;\
;    2444 			curr_sel++;\
;    2445 			if(curr_sel > max_option)\
;    2446 				curr_sel = min_option;
;    2447 
;    2448 #define DEC_OPTION	last_sel = curr_sel;\
;    2449 			curr_sel--;\
;    2450 			if(curr_sel < min_option)\
;    2451 				curr_sel = max_option;
;    2452 
;    2453 extern int   curr_ch;	//index of current channel in configuation   menu
;    2454 extern int   curr_prb;	//index of current probe selection
;    2455 extern int   curr_dispch; //start of current channel for display in main menu
;    2456 
;    2457 void State_Init() {
_State_Init:
;    2458 	display_buttons(KEY_BTN1,sysdata.prbmode);
	LDI  R30,LOW(97)
	ST   -Y,R30
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2459 	display_buttons(KEY_BTN2,sysdata.kttmode);
	LDI  R30,LOW(98)
	ST   -Y,R30
	__POINTW2MN _sysdata,63
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _display_buttons
;    2460 	display_buttons(KEY_BTN3,0); //probe type
	LDI  R30,LOW(99)
	CALL SUBOPT_0x25
;    2461 	display_buttons(KEY_BTN4,0); //zero
	LDI  R30,LOW(100)
	CALL SUBOPT_0x25
;    2462 }
	RET
;    2463 char* getprbtype(uchar);
;    2464 LABEL flash pgmain_chs[] = {
;    2465 		{LBL_HZ6X8,3, 3,5,strbuf},
;    2466 		{LBL_HZ6X8,3,16,5,strbuf},
;    2467 		{LBL_HZ6X8,3,29,5,strbuf},
;    2468 		{LBL_HZ6X8,3,42,5,strbuf}
;    2469 	};
;    2470 LABEL flash pgmain_values[] = {
;    2471 		{LBL_HZ6X8,40,3,10,strbuf},
;    2472 		{LBL_HZ6X8,40,16,10,strbuf},
;    2473 		{LBL_HZ6X8,40,29,10,strbuf},
;    2474 		{LBL_HZ6X8,40,42,10,strbuf}
;    2475 	};
;    2476 LABEL flash pgmain_temps[] = {
;    2477 		{LBL_HZ6X8,140,3, 10,strbuf},
;    2478 		{LBL_HZ6X8,140,16,10,strbuf},
;    2479 		{LBL_HZ6X8,140,29,10,strbuf},
;    2480 		{LBL_HZ6X8,140,42,10,strbuf}
;    2481 };
;    2482 /*      Main display      */
;    2483 
;    2484 void pgmain_handler(uchar msg) {
_pgmain_handler:
;    2485         int max_option = 4;
;    2486         uchar i,old_ch,isinit;
;    2487                 uchar *cptr;
;    2488 	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
;    2489 
;    2490 	isinit = 0;
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
;    2491         //Menu
;    2492         if(msg == KEY_CE) {
	LDD  R26,Y+8
	CPI  R26,LOW(0x43)
	BRNE _0x1EC
;    2493                 nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2494                 return;
	RJMP _0x4B0
;    2495         }
;    2496 	if(msg == KEY_TAB) {
_0x1EC:
	LDD  R26,Y+8
	CPI  R26,LOW(0x54)
	BRNE _0x1ED
;    2497 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x1EE
;    2498 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4CA
;    2499 		else
_0x1EE:
;    2500 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4CA:
	MOV  R4,R30
;    2501 		return;
	RJMP _0x4B0
;    2502 	}
;    2503         if(key == KEY_NUM1) //R0
_0x1ED:
	LDI  R30,LOW(49)
	CP   R30,R5
	BRNE _0x1F0
;    2504         {
;    2505 /*
;    2506                 window_setup(10);
;    2507                 if(IS_BORE_MODE)
;    2508                 {
;    2509                         sprintf(strbuf,"请输入铂电阻R0阻值");
;    2510 	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
;    2511 		        msg = MSG_INIT;
;    2512 		}else{
;    2513 		        return;
;    2514                         sprintf(strbuf,"请输入V0值");
;    2515 	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
;    2516                         msg = MSG_INIT;
;    2517 		}
;    2518 */
;    2519         }
;    2520         if(key == KEY_NUM2) //Rs1
_0x1F0:
	LDI  R30,LOW(50)
	CP   R30,R5
	BRNE _0x1F1
;    2521         {
;    2522 /*
;    2523                 if(IS_BORE_MODE)
;    2524                 {
;    2525                         window_setup(10);
;    2526                         sprintf(strbuf,"请输入PT100内标准阻值");
;    2527 		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
;    2528         		msg = MSG_INIT;
;    2529                 }
;    2530 */
;    2531         }
;    2532         if(key == KEY_NUM3) //Rs2
_0x1F1:
	LDI  R30,LOW(51)
	CP   R30,R5
	BRNE _0x1F2
;    2533         {
;    2534 /*
;    2535                 if(IS_BORE_MODE)
;    2536                 {
;    2537                         window_setup(10);
;    2538                         sprintf(strbuf,"请输入PT1000内标准阻值");
;    2539 		        sysdata.Rs2 = wnd_floatinput(sysdata.Rs2);
;    2540         		msg = MSG_INIT;
;    2541                 }
;    2542 */
;    2543         }
;    2544 
;    2545 	if(msg == KEY_UP) {
_0x1F2:
	LDD  R26,Y+8
	CPI  R26,LOW(0x55)
	BRNE _0x1F3
;    2546 		DEC_DISPCH;
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
;    2547 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2548 	}
;    2549 	if(msg == KEY_DN) {
_0x1F3:
	LDD  R26,Y+8
	CPI  R26,LOW(0x44)
	BRNE _0x1F5
;    2550 		INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x1F6
	CALL SUBOPT_0x3C
_0x1F6:
;    2551 		msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+8,R30
;    2552 	}
;    2553 
;    2554 	if(msg == MSG_INIT) {
_0x1F5:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFF)
	BRNE _0x1F7
;    2555 	        isinit = 1;
	LDI  R21,LOW(1)
;    2556 		LCD_Cls();
	CALL _LCD_Cls
;    2557 		old_ch = curr_dispch;
	CALL SUBOPT_0xA8
;    2558 
;    2559 	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2560 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x1F9:
	CPI  R19,4
	BRSH _0x1FA
;    2561 		{
;    2562 		        if(curr_dispch == old_ch)
	MOV  R30,R18
	MOVW R26,R10
	CALL SUBOPT_0xA9
	BRNE _0x1FB
;    2563 		        {
;    2564         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2565                                 draw_label(&pgmain_chs[i],SW_NORMAL);
	CALL SUBOPT_0x37
;    2566                                 draw_label(&pgmain_chs[i],SW_REVERSE);
	LDI  R30,LOW(10)
	MUL  R30,R19
	MOVW R30,R0
	SUBI R30,LOW(-_pgmain_chs*2)
	SBCI R31,HIGH(-_pgmain_chs*2)
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(2)
	RJMP _0x4CB
;    2567         		}
;    2568         		else{
_0x1FB:
;    2569         			sprintf(strbuf,"CH%02i:",curr_dispch);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAA
	CALL SUBOPT_0xAB
;    2570         			draw_label(&pgmain_chs[i],SW_NORMAL);
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
_0x4CB:
	ST   -Y,R30
	CALL _draw_label
;    2571         		}
;    2572 			INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x1FD
	CALL SUBOPT_0x3C
_0x1FD:
;    2573 		}
	SUBI R19,-1
	RJMP _0x1F9
_0x1FA:
;    2574 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2575 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+8,R30
;    2576 	}
;    2577 
;    2578 	if(msg == MSG_REFRESH) {
_0x1F7:
	LDD  R26,Y+8
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x1FE
;    2579 		old_ch = curr_dispch;
	CALL SUBOPT_0xA8
;    2580 		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);
;    2581 		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
_0x200:
	CPI  R19,4
	BRLO PC+3
	JMP _0x201
;    2582 		{
;    2583 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BREQ PC+3
	JMP _0x202
;    2584 		        {
;    2585 			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
	CALL SUBOPT_0x43
	__POINTW2MN _tprbdata,576
	CALL SUBOPT_0x2A
	ST   -Y,R30
	RCALL _getprbtype
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2586         			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2587 	        			(cptr[0] == 'P')){
	CALL SUBOPT_0x43
	CPI  R30,LOW(0xFF)
	BREQ _0x204
	LDD  R26,Y+6
	LDD  R27,Y+6+1
	LD   R26,X
	CPI  R26,LOW(0x50)
	BRNE _0x203
_0x204:
;    2588                				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2589                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x207
	CPI  R21,1
	BRNE _0x206
_0x207:
;    2590                                         {
;    2591                         			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x37
;    2592 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4CC
;    2593 		                        }else{
_0x206:
;    2594                         			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2595 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4CC:
	ST   -Y,R30
	CALL _draw_label
;    2596 		                        }
;    2597 			        }else{
	RJMP _0x20A
_0x203:
;    2598 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB1
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2599 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xAD
	BRNE _0x20B
;    2600         					sprintf(strbuf+8,"*mV");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,191
	RJMP _0x4CD
;    2601                                         else
_0x20B:
;    2602                                                 sprintf(strbuf+8," mV");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,195
_0x4CD:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2603                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x20E
	CPI  R21,1
	BRNE _0x20D
_0x20E:
;    2604                                         {
;    2605         					draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x37
;    2606         					if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x210
;    2607         					        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,199
	CALL SUBOPT_0x32
;    2608         					else
	RJMP _0x211
_0x210:
;    2609                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2610 
;    2611                 				sprintf(strbuf+8," ! %c",cptr[0]);
_0x211:
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2612                 			        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4CE
;    2613                                         }else{
_0x20D:
;    2614                                                 draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2615                 				if(rundata.temperature[curr_dispch - 1] < -9000)
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x213
;    2616                 				{
;    2617                 				        sprintf(strbuf," OVER    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,199
	CALL SUBOPT_0x32
;    2618                 				}else{
	RJMP _0x214
_0x213:
;    2619                 				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2620                 				}
_0x214:
;    2621         				        sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2622 	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4CE:
	ST   -Y,R30
	CALL _draw_label
;    2623                                         }
;    2624 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xB7
	BRNE _0x215
;    2625         				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4CF
;    2626         				else                                     //Hz12
_0x215:
;    2627                 				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,16
_0x4CF:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2628 	                        }
_0x20A:
;    2629 			}else{
	RJMP _0x217
_0x202:
;    2630        			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
	CALL SUBOPT_0xBA
	__POINTW2MN _rprbdata,576
	CALL SUBOPT_0x2A
	ST   -Y,R30
	RCALL _getprbtype
	STD  Y+6,R30
	STD  Y+6+1,R31
;    2631         			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
;    2632         				(rundata.temperature[curr_dispch - 1] < -9000) ||\
;    2633 	        			(cptr[0] != 'P'))
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
;    2634 	        		{
;    2635 	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
	CALL SUBOPT_0xBA
	CPI  R30,LOW(0xFF)
	BRNE _0x21B
;    2636                 				sprintf(strbuf," ------- ohm");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,220
	CALL SUBOPT_0x32
;    2637                 			if(rundata.temperature[curr_dispch - 1] < -9000)
_0x21B:
	CALL SUBOPT_0x47
	CALL SUBOPT_0x2E
	CALL SUBOPT_0x46
	BRSH _0x21C
;    2638                         			sprintf(strbuf," NO READ    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,233
	RJMP _0x4D0
;    2639                                         else
_0x21C:
;    2640                                         	sprintf(strbuf," -------    ");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,246
_0x4D0:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2641                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x21F
	CPI  R21,1
	BRNE _0x21E
_0x21F:
;    2642                                         {
;    2643         	        			draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x37
;    2644                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2645 		                		draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4D1
;    2646 		                	}else{
_0x21E:
;    2647         	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2648                 				sprintf(strbuf," -------  ");
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xAC
;    2649 		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4D1:
	ST   -Y,R30
	CALL _draw_label
;    2650 		                	}
;    2651 			        }else{
	RJMP _0x222
_0x218:
;    2652 					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB1
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2653 					if(curr_dispch-1 == ch_to_search)
	CALL SUBOPT_0xAD
	BRNE _0x223
;    2654         					sprintf(strbuf+8,"*ohm");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,259
	RJMP _0x4D2
;    2655                                         else
_0x223:
;    2656         					sprintf(strbuf+8," ohm");
	CALL SUBOPT_0xB3
	__POINTW1FN _0,228
_0x4D2:
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0xB4
;    2657                                         if((curr_dispch-1 == ch_to_search) || (isinit == 1))
	CALL SUBOPT_0xAD
	BREQ _0x226
	CPI  R21,1
	BRNE _0x225
_0x226:
;    2658                                         {
;    2659 	        			        draw_label(&pgmain_values[i],SW_NORMAL);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0x37
;    2660         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2661 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2662 		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(1)
	RJMP _0x4D3
;    2663 		        		 }else{
_0x225:
;    2664 	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAE
	CALL SUBOPT_0xB0
;    2665         		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xB5
	CALL SUBOPT_0x15
	CALL SUBOPT_0xB2
;    2666 	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
	CALL SUBOPT_0xB3
	CALL SUBOPT_0xB6
;    2667 		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
	CALL SUBOPT_0xAF
	LDI  R30,LOW(5)
_0x4D3:
	ST   -Y,R30
	CALL _draw_label
;    2668 		        		 }
;    2669 				        if(pgmain_temps[i].type == LBL_HZ6X8)
	CALL SUBOPT_0xB7
	BRNE _0x229
;    2670         				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,10
	RJMP _0x4D4
;    2671         				else                                     //Hz12
_0x229:
;    2672                 				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
	CALL SUBOPT_0xB8
	CALL SUBOPT_0xB9
	SBIW R30,2
	ADIW R30,16
_0x4D4:
	ST   -Y,R30
	CALL _LCD_Rectange
;    2673         			}
_0x222:
;    2674 	                }
_0x217:
;    2675 			INC_DISPCH;
	CALL SUBOPT_0xA7
	BRGE _0x22B
	CALL SUBOPT_0x3C
_0x22B:
;    2676 		}
	SUBI R19,-1
	RJMP _0x200
_0x201:
;    2677 		curr_dispch = old_ch;
	MOV  R10,R18
	CLR  R11
;    2678 	}
;    2679 }
_0x1FE:
_0x4B0:
	CALL __LOADLOCR6
	ADIW R28,9
	RET
;    2680 
;    2681 //main configuration window of R
;    2682 LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
;    2683 LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
;    2684 LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
;    2685 LABEL flash pgr_options[] = {
;    2686 	{LBL_HZ16, 10,23,7,"1.电流换向"},
;    2687 	{LBL_HZ16,130,23,7,"2.内标准校准"},
;    2688 	{LBL_HZ16, 10,45,7,"3.通道探头选择"},
;    2689 	{LBL_HZ16,130,45,7,"4.设置探头参数"}
;    2690 };
;    2691 
;    2692 //LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2693 #define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
;    2694 void pgcalibrate()
;    2695 {
_pgcalibrate:
;    2696         double oldvalue = 0;
;    2697         window_setup(10);
	SBIW R28,4
	LDI  R24,4
	LDI  R26,LOW(0)
	LDI  R27,HIGH(0)
	LDI  R30,LOW(_0x232*2)
	LDI  R31,HIGH(_0x232*2)
	CALL __INITLOCB
;	oldvalue -> Y+0
	CALL SUBOPT_0xBB
;    2698 	sprintf(strbuf,"请输入外标准(CH1)阻值");
	__POINTW1FN _0,349
	CALL SUBOPT_0x32
;    2699 	oldvalue = wnd_floatinput(oldvalue);
	CALL SUBOPT_0x2
	CALL SUBOPT_0xBC
	__PUTD1S 0
;    2700 	if(oldvalue == 0)
	CALL SUBOPT_0x2
	CALL __CPD10
	BRNE _0x233
;    2701                 return;
	RJMP _0x4AF
;    2702         wnd_msgbox(&pgr_calibrate);
_0x233:
	LDI  R30,LOW(_pgr_calibrate*2)
	LDI  R31,HIGH(_pgr_calibrate*2)
	CALL SUBOPT_0x39
;    2703         if(rprbdata.type[sysdata.rid[curr_dispch-1]] == PRBTYPE_PT1000)
	CALL SUBOPT_0xBA
	__POINTW2MN _rprbdata,576
	CALL SUBOPT_0x2A
	CPI  R30,LOW(0xF3)
	BRNE _0x234
;    2704                 sysdata.Rs2 = (oldvalue + sysdata.R0)*sysdata.Rs2/(rundata.reading[curr_dispch-1]+sysdata.R0);
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
;    2705         else
_0x234:
;    2706                 sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);
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
;    2707 
;    2708         if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))
	CALL SUBOPT_0x5
	CALL SUBOPT_0x6
	BREQ PC+4
	BRCS PC+3
	JMP  _0x237
	CALL SUBOPT_0x5
	CALL SUBOPT_0x7
	BRSH _0x236
_0x237:
;    2709         {
;    2710                 sysdata.Rs1 = 100;
	CALL SUBOPT_0x8
;    2711         }
;    2712         if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))
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
;    2713         {
;    2714                 sysdata.Rs2 = 1000;
	CALL SUBOPT_0xC
;    2715         }
;    2716 }
_0x239:
_0x4AF:
	ADIW R28,4
	RET
;    2717 //main menu of bore config
;    2718 void pgrconfig_handler(uchar msg) {
_pgrconfig_handler:
;    2719 	uchar i;
;    2720 
;    2721 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2722 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S4C:
	.BYTE 0x1

	.CSEG
;    2723 	uchar min_option = 1;
;    2724 	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);
;    2725 
;    2726 	if(msg == KEY_TAB) {
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
;    2727 	        SET_BORE_MODE;
	CALL SUBOPT_0x3A
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3B
;    2728 	        SET_TORS;
	CALL SUBOPT_0x1E
;    2729 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2730 		return;
	RJMP _0x4AE
;    2731 	}
;    2732 	if(msg == KEY_CE)
_0x23E:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x23F
;    2733 	{
;    2734 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2735 	        return;
	RJMP _0x4AE
;    2736 	}
;    2737 	if(msg == MSG_INIT) {
_0x23F:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x240
;    2738 		LCD_Cls();
	CALL _LCD_Cls
;    2739 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S4C,R30
;    2740 		draw_label(&pgr_banner, SW_NORMAL);
	LDI  R30,LOW(_pgr_banner*2)
	LDI  R31,HIGH(_pgr_banner*2)
	CALL SUBOPT_0x37
;    2741 		INIT_OPTIONS(pgr_);
	MOV  R17,R16
_0x242:
	CP   R19,R17
	BRLO _0x243
	CALL SUBOPT_0xBD
	CALL SUBOPT_0x37
	SUBI R17,-1
	RJMP _0x242
_0x243:
;    2742 		sprintf(strbuf,"(%i)",sysdata.ktime);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,371
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDW
	CALL SUBOPT_0x34
	CALL SUBOPT_0xB2
;    2743 		draw_label(&pgr_klbl, SW_NORMAL);
	LDI  R30,LOW(_pgr_klbl*2)
	LDI  R31,HIGH(_pgr_klbl*2)
	CALL SUBOPT_0x37
;    2744 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2745 	}
;    2746 	if(msg == KEY_DN)
_0x240:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x244
;    2747 	{
;    2748 	        last_sel = curr_sel;
	CALL SUBOPT_0xBE
;    2749 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S4C,R30
;    2750 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S4C
	CP   R19,R26
	BRSH _0x245
;    2751 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S4C,R30
;    2752 	        msg = MSG_REFRESH;
_0x245:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2753 	}
;    2754 	if(msg == KEY_UP)
_0x244:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x246
;    2755 	{
;    2756 	        last_sel = curr_sel;
	CALL SUBOPT_0xBE
;    2757 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S4C,R30
;    2758 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x247
;    2759 	                curr_sel = max_option;
	STS  _curr_sel_S4C,R19
;    2760 	        msg = MSG_REFRESH;
_0x247:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2761 	}
;    2762 	if(msg == KEY_OK)
_0x246:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x248
;    2763 	{
;    2764 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S4C
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    2765 	}
;    2766 	if(msg == KEY_NUM1) {
_0x248:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x249
;    2767 		window_setup(4); //4 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xBF
;    2768 		sprintf(strbuf,"电流换向时间设置");
	__POINTW1FN _0,376
	CALL SUBOPT_0x32
;    2769 		sysdata.ktime = wnd_intinput(sysdata.ktime);
	__POINTW2MN _sysdata,12
	CALL __EEPROMRDB
	ST   -Y,R30
	CALL _wnd_intinput
	__POINTW2MN _sysdata,12
	LDI  R31,0
	CALL __EEPROMWRW
;    2770 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2771 		return;
	RJMP _0x4AE
;    2772 	}
;    2773 	if(msg == KEY_NUM2)
_0x249:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x24A
;    2774 	{
;    2775                 pgcalibrate();
	CALL _pgcalibrate
;    2776 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    2777 		return;
	RJMP _0x4AE
;    2778 	}
;    2779 	if(msg == KEY_NUM3) {
_0x24A:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x24B
;    2780 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2781 		return;
	RJMP _0x4AE
;    2782 	}
;    2783 	if(msg == KEY_NUM4) {
_0x24B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x24C
;    2784 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2785 		return;
	RJMP _0x4AE
;    2786 	}
;    2787 	if(msg == MSG_REFRESH) {
_0x24C:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x24D
;    2788 		REFRESH_OPTIONS(pgr_);
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
;    2789 	}
;    2790 }
_0x24D:
	RJMP _0x4AE
;    2791 //main configuration window of T
;    2792 LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
;    2793 LABEL flash pgt_options[] = {
;    2794 	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
;    2795 	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
;    2796 };
;    2797 
;    2798 //main menu of thermo config
;    2799 void pgtconfig_handler(uchar msg) {
_pgtconfig_handler:
;    2800 	uchar i;
;    2801 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2802 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S4D:
	.BYTE 0x1

	.CSEG
;    2803 	uchar min_option = 1;
;    2804 	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
;    2805 	if(msg == KEY_TAB) {
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
;    2806 	        SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    2807 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    2808 		return;
	RJMP _0x4AE
;    2809 	}
;    2810 	if(msg == KEY_CE)
_0x259:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x25A
;    2811 	{
;    2812 	        nextwin = PG_BOOTTYPE;
	LDI  R30,LOW(13)
	MOV  R4,R30
;    2813 	        return;
	RJMP _0x4AE
;    2814 	}
;    2815 	if(msg == MSG_INIT) {
_0x25A:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x25B
;    2816 		LCD_Cls();
	CALL _LCD_Cls
;    2817 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S4D,R30
;    2818 		draw_label(&pgt_banner, SW_NORMAL);
	LDI  R30,LOW(_pgt_banner*2)
	LDI  R31,HIGH(_pgt_banner*2)
	CALL SUBOPT_0x37
;    2819 		INIT_OPTIONS(pgt_);
	MOV  R17,R16
_0x25D:
	CP   R19,R17
	BRLO _0x25E
	CALL SUBOPT_0xC2
	CALL SUBOPT_0x37
	SUBI R17,-1
	RJMP _0x25D
_0x25E:
;    2820 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2821 	}
;    2822 	if(msg == KEY_DN)
_0x25B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x25F
;    2823 	{
;    2824 	        last_sel = curr_sel;
	CALL SUBOPT_0xC3
;    2825 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S4D,R30
;    2826 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S4D
	CP   R19,R26
	BRSH _0x260
;    2827 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S4D,R30
;    2828 	        msg = MSG_REFRESH;
_0x260:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2829 	}
;    2830 	if(msg == KEY_UP)
_0x25F:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x261
;    2831 	{
;    2832 	        last_sel = curr_sel;
	CALL SUBOPT_0xC3
;    2833 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S4D,R30
;    2834 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x262
;    2835 	                curr_sel = max_option;
	STS  _curr_sel_S4D,R19
;    2836 	        msg = MSG_REFRESH;
_0x262:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    2837 	}
;    2838 	if(msg == KEY_OK)
_0x261:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x263
;    2839 	{
;    2840 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S4D
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    2841 	}
;    2842 	if(msg == KEY_NUM1) {
_0x263:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x264
;    2843 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    2844 		return;
	RJMP _0x4AE
;    2845 	}
;    2846 	if(msg == KEY_NUM2) {
_0x264:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x265
;    2847 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    2848 		return;
	RJMP _0x4AE
;    2849 	}
;    2850 	if(msg == MSG_REFRESH) {
_0x265:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x266
;    2851 		REFRESH_OPTIONS(pgt_);
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
;    2852 	}
;    2853 	return;
_0x266:
_0x4AE:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    2854 }
;    2855 //channel probe setup
;    2856 LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
;    2857 LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
;    2858 LABEL flash pgch_items[] = {
;    2859 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2860 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2861 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2862 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2863 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2864 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2865 };
;    2866 
;    2867 //select the channel to config
;    2868 void pgchset_handler(uchar msg) {
_pgchset_handler:
;    2869 	static uint curr_pos = 1; //absolute postion in options 1-24

	.DSEG
_curr_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2870 	static uint last_pos = 1;

	.DSEG
_last_pos_S4E:
	.BYTE 0x2

	.CSEG
;    2871 	uchar min_option = 1;
;    2872 	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
;    2873 	uchar min_index = 1;
;    2874 	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
;    2875 	uchar new_page = 0;
;    2876 	int i,j;
;    2877 	if(msg == KEY_CE) {
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
;    2878 		if(IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x271
;    2879 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	RJMP _0x4D6
;    2880 		else
_0x271:
;    2881 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
_0x4D6:
	MOV  R4,R30
;    2882 		return;
	RJMP _0x4AD
;    2883 	}
;    2884 
;    2885 	if(msg == KEY_UP) {
_0x270:
	LDD  R26,Y+10
	CPI  R26,LOW(0x55)
	BRNE _0x273
;    2886 		DEC_TABLE;
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
;    2887 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2888 	}
;    2889 	if(msg == KEY_DN) {
_0x273:
	LDD  R26,Y+10
	CPI  R26,LOW(0x44)
	BRNE _0x278
;    2890 		INC_TABLE;
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
;    2891 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2892 	}
;    2893 	if(msg == KEY_OK) {	//show window to select the probe
_0x278:
	LDD  R26,Y+10
	CPI  R26,LOW(0x4F)
	BRNE _0x27D
;    2894 		curr_ch = curr_pos;
	__GETWRMN 6,7,0,_curr_pos_S4E
;    2895 		nextwin = PG_PRBLIST;
	LDI  R30,LOW(8)
	MOV  R4,R30
;    2896 		return;
	RJMP _0x4AD
;    2897 	}
;    2898 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x27D:
	LDD  R26,Y+10
	CPI  R26,LOW(0x30)
	BRLO _0x27F
	CPI  R26,LOW(0x3A)
	BRLO _0x280
_0x27F:
_0x280:
;    2899 	/*
;    2900 		KEY_TABLE;
;    2901 		msg = MSG_REFRESH;
;    2902 	*/
;    2903 	}
;    2904 	if(msg == KEY_DOT)
	LDD  R26,Y+10
	CPI  R26,LOW(0x2E)
	BRNE _0x281
;    2905 	{
;    2906 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x282
;    2907 	        {
;    2908 		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4D7
;    2909 	        }else{
_0x282:
;    2910        		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4D7:
	LDS  R30,_curr_pos_S4E
	LDS  R31,_curr_pos_S4E+1
	CALL SUBOPT_0xCD
;    2911 	        }
;    2912 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2913 		new_page = 1; //refresh the whole page
	LDI  R21,LOW(1)
;    2914 	}
;    2915 	if(msg == MSG_INIT) {
_0x281:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFF)
	BRNE _0x284
;    2916 		LCD_Cls();
	CALL _LCD_Cls
;    2917 		draw_label(&pgch_banner, SW_NORMAL);
	LDI  R30,LOW(_pgch_banner*2)
	LDI  R31,HIGH(_pgch_banner*2)
	CALL SUBOPT_0x37
;    2918 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xC7
;    2919 		new_page = 1;
	LDI  R21,LOW(1)
;    2920 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+10,R30
;    2921 	}
;    2922 	if(msg == MSG_REFRESH) {
_0x284:
	LDD  R26,Y+10
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x285
;    2923 
;    2924 		REFRESH_TABLES1(pgch_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xC5
	CALL SUBOPT_0x36
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
;    2925 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x28A
;    2926         	        {
;    2927 				if(sysdata.tid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xD2
	BRNE _0x28B
;    2928 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xB2
;    2929 				}else{
	RJMP _0x28C
_0x28B:
;    2930 			        	//draw line item j
;    2931 				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD4
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xD5
	CALL _tname2b
	CALL SUBOPT_0x36
	CALL SUBOPT_0xD6
;    2932 				}
_0x28C:
;    2933 		        }else{
	RJMP _0x28D
_0x28A:
;    2934 				if(sysdata.rid[j-1] == INVALID_PROBE){
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xD2
	BRNE _0x28E
;    2935 					sprintf(strbuf,"CH%i:*",j);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD3
	CALL SUBOPT_0xB2
;    2936 				}else{
	RJMP _0x28F
_0x28E:
;    2937 			        	//draw line item j
;    2938 				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xD4
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xD5
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xD6
;    2939 				}
_0x28F:
;    2940 		        }
_0x28D:
;    2941 		REFRESH_TABLES2(pgch_);
	CALL SUBOPT_0xD8
	CALL SUBOPT_0x37
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
;    2942 	}
;    2943 	return;
_0x285:
_0x4AD:
	CALL __LOADLOCR6
	ADIW R28,11
	RET
;    2944 }
;    2945 //select probe to edit.
;    2946 LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
;    2947 LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    2948 LABEL flash pgprbset_items[] = {
;    2949 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    2950 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    2951 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    2952 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    2953 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    2954 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    2955 };
;    2956 //select probe to config
;    2957 void pgprbset_handler(uchar msg) {
_pgprbset_handler:
;    2958 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    2959 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2960 	static uint last_pos = 1;

	.DSEG
_last_pos_S4F:
	.BYTE 0x2

	.CSEG
;    2961 	uchar min_option = 1;
;    2962 	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
;    2963 	uchar min_index = 1;
;    2964 	uchar max_index = MAX_PROBE_NUM;
;    2965 	int i,j;
;    2966 	uchar new_page = 1;
;    2967 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
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
;    2968 	/*
;    2969 		KEY_TABLE;
;    2970 		msg = MSG_REFRESH;
;    2971 	*/
;    2972 	}
;    2973 
;    2974 	if(msg == KEY_CE)
	LDD  R26,Y+9
	CPI  R26,LOW(0x43)
	BRNE _0x29E
;    2975 	{
;    2976 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x29F
;    2977 			nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	RJMP _0x4D8
;    2978 		else
_0x29F:
;    2979 			nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
_0x4D8:
	MOV  R4,R30
;    2980 		return;
	RJMP _0x4AC
;    2981 	}
;    2982 	if(msg == KEY_OK)
_0x29E:
	LDD  R26,Y+9
	CPI  R26,LOW(0x4F)
	BRNE _0x2A1
;    2983 	{
;    2984 		curr_prb = curr_pos;
	__GETWRMN 8,9,0,_curr_pos_S4F
;    2985 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    2986 		return;
	RJMP _0x4AC
;    2987 	}
;    2988 	if(msg == MSG_INIT)
_0x2A1:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFF)
	BRNE _0x2A2
;    2989 	{
;    2990 		LCD_Cls();
	CALL _LCD_Cls
;    2991 		draw_label(&pgprbset_banner, SW_NORMAL);
	LDI  R30,LOW(_pgprbset_banner*2)
	LDI  R31,HIGH(_pgprbset_banner*2)
	CALL SUBOPT_0x37
;    2992 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xDC
;    2993 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    2994 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2995 	}
;    2996 	if(msg == KEY_UP) {
_0x2A2:
	LDD  R26,Y+9
	CPI  R26,LOW(0x55)
	BRNE _0x2A3
;    2997 		DEC_TABLE;
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
;    2998 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    2999 	}
;    3000 	if(msg == KEY_DN) {
_0x2A3:
	LDD  R26,Y+9
	CPI  R26,LOW(0x44)
	BRNE _0x2A8
;    3001 		INC_TABLE;
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
;    3002 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+9,R30
;    3003 	}
;    3004 	if(msg == MSG_REFRESH) {
_0x2A8:
	LDD  R26,Y+9
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2AD
;    3005 		REFRESH_TABLES1(pgprbset_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xDD
	CALL SUBOPT_0x36
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
;    3006 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2B2
;    3007 		        {
;    3008 			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xE3
	LDD  R30,Y+15
	LDD  R31,Y+15+1
	SBIW R30,1
	ST   -Y,R30
	CALL _tname2b
	RJMP _0x4DB
;    3009 		        }else{
_0x2B2:
;    3010         		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
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
;    3011 		        }
;    3012         		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xE4
;    3013 		REFRESH_TABLES2(pgprbset_);
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
;    3014 	}
;    3015 	return;
_0x2AD:
_0x4AC:
	CALL __LOADLOCR6
	ADIW R28,10
	RET
;    3016 }
;    3017 //select probe for channel setup
;    3018 LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
;    3019 LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
;    3020 LABEL flash prblist_items[] = {
;    3021 	{LBL_HZ6X8, 10, 30, 10, strbuf},
;    3022 	{LBL_HZ6X8, 130,30, 10, strbuf},
;    3023 	{LBL_HZ6X8, 10, 40, 10, strbuf},
;    3024 	{LBL_HZ6X8, 130,40, 10, strbuf},
;    3025 	{LBL_HZ6X8, 10, 50, 10, strbuf},
;    3026 	{LBL_HZ6X8, 130,50, 10, strbuf}
;    3027 };
;    3028 //select channel's probe
;    3029 void pgprblist_handler(uchar msg) {
_pgprblist_handler:
;    3030 	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
;    3031 	static uint curr_pos = 1;

	.DSEG
_curr_pos_S50:
	.BYTE 0x2

	.CSEG
;    3032 	static uint last_pos = 1;

	.DSEG
_last_pos_S50:
	.BYTE 0x2

	.CSEG
;    3033 	uchar min_option = 1;
;    3034 	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
;    3035 	uchar min_index = 1;
;    3036 	uchar max_index = MAX_PROBE_NUM;
;    3037 	uchar i,j;
;    3038 	uchar new_page = 1;
;    3039 	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
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
;    3040 	/*
;    3041 		KEY_TABLE;
;    3042 		msg = MSG_REFRESH;
;    3043 	*/
;    3044 	}
;    3045 	if(msg == KEY_CE)
	LDD  R26,Y+7
	CPI  R26,LOW(0x43)
	BRNE _0x2C1
;    3046 	{
;    3047 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    3048 		return;
	RJMP _0x4AB
;    3049 	}
;    3050 	if(msg == KEY_OK)
_0x2C1:
	LDD  R26,Y+7
	CPI  R26,LOW(0x4F)
	BRNE _0x2C2
;    3051 	{
;    3052 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2C3
;    3053 	        {
;    3054        	                if(tprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	BREQ _0x2C4
;    3055         		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,14
	CALL SUBOPT_0xE9
	CALL __EEPROMWRB
;    3056 	        }else{
_0x2C4:
	RJMP _0x2C5
_0x2C3:
;    3057                         if(rprbdata.name[curr_pos-1][0] != '\0')
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0xE7
	CALL SUBOPT_0xE8
	BREQ _0x2C6
;    3058 		                sysdata.rid[curr_ch-1] = curr_pos - 1;
	__POINTW2MN _sysdata,38
	CALL SUBOPT_0xE9
	CALL __EEPROMWRB
;    3059 	        }
_0x2C6:
_0x2C5:
;    3060 		nextwin = PG_CHSET;
	LDI  R30,LOW(7)
	MOV  R4,R30
;    3061 		return;
	RJMP _0x4AB
;    3062 	}
;    3063 	if(msg == MSG_INIT)
_0x2C2:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFF)
	BRNE _0x2C7
;    3064 	{
;    3065 		LCD_Cls();
	CALL _LCD_Cls
;    3066 		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,502
	ST   -Y,R31
	ST   -Y,R30
	MOVW R30,R6
	CALL SUBOPT_0x34
	CALL SUBOPT_0xB2
;    3067 		draw_label(&prblist_banner, SW_NORMAL);
	LDI  R30,LOW(_prblist_banner*2)
	LDI  R31,HIGH(_prblist_banner*2)
	CALL SUBOPT_0x37
;    3068 		curr_pos = 1;
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	CALL SUBOPT_0xEA
;    3069 		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    3070 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3071 	}
;    3072 	if(msg == KEY_UP) {
_0x2C7:
	LDD  R26,Y+7
	CPI  R26,LOW(0x55)
	BRNE _0x2C8
;    3073 		DEC_TABLE;
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
;    3074 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3075 	}
;    3076 	if(msg == KEY_DN) {
_0x2C8:
	LDD  R26,Y+7
	CPI  R26,LOW(0x44)
	BRNE _0x2CD
;    3077 		INC_TABLE;
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
;    3078 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3079 	}
;    3080 	if(msg == KEY_DOT){
_0x2CD:
	LDD  R26,Y+7
	CPI  R26,LOW(0x2E)
	BRNE _0x2D2
;    3081 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2D3
;    3082 	        {
;    3083 		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,14
	RJMP _0x4DE
;    3084 	        }else{
_0x2D3:
;    3085 		        sysdata.rid[curr_ch-1] = INVALID_PROBE;
	__POINTW2MN _sysdata,38
_0x4DE:
	MOVW R30,R6
	CALL SUBOPT_0xCD
;    3086 	        }
;    3087        		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+7,R30
;    3088        		new_page = 1;
	LDI  R30,LOW(1)
	STD  Y+6,R30
;    3089 	}
;    3090 	if(msg == MSG_REFRESH) {
_0x2D2:
	LDD  R26,Y+7
	CPI  R26,LOW(0xFE)
	BREQ PC+3
	JMP _0x2D5
;    3091 		REFRESH_TABLES1(prblist_);
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xCE
	CALL SUBOPT_0xEB
	CALL SUBOPT_0x36
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
;    3092 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2DA
;    3093 		        {
;    3094 		                sprintf(strbuf,"%s",tname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xF0
	CALL _tname2b
	RJMP _0x4DF
;    3095 		        }else{
_0x2DA:
;    3096         		        sprintf(strbuf,"%s",rname2b(j-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0xF0
	CALL _rname2b
_0x4DF:
	CLR  R22
	CLR  R23
	CALL SUBOPT_0xF1
;    3097         		}
;    3098 			draw_label(&(prblist_items[i-1]),SW_NORMAL);
	CALL SUBOPT_0xF2
;    3099 		REFRESH_TABLES2(prblist_);
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
;    3100 	}
;    3101 	return;
_0x2D5:
_0x4AB:
	CALL __LOADLOCR6
	ADIW R28,8
	RET
;    3102 }
;    3103 //select probe type list
;    3104 LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
;    3105 LABEL flash tplist_options[] = {
;    3106 	{LBL_HZ6X8,10,20,8,"0.PT1000"},
;    3107 	{LBL_HZ6X8,80,20,8,"1.PT100"},
;    3108 	{LBL_HZ6X8,150,20,8,"2.PT25"},
;    3109 	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
;    3110 	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
;    3111 	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
;    3112 	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
;    3113 	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
;    3114 	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
;    3115         {LBL_HZ6X8,10,50,8,"9.T-TYPE"},
;    3116 	{LBL_HZ6X8,10,50,8,"*.R-TYPE"}
;    3117 
;    3118 };
;    3119 //select probe type thermo
;    3120 void pgprbtypelist_handler(uchar msg) {
_pgprbtypelist_handler:
;    3121 	static int curr_sel = 1;

	.DSEG
_curr_sel_S51:
	.BYTE 0x2

	.CSEG
;    3122 	static int last_sel = 0xff;

	.DSEG
_last_sel_S51:
	.BYTE 0x2

	.CSEG
;    3123 	uchar min_option = 1;
;    3124 	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
;    3125 	uchar i;
;    3126 	if(msg == KEY_DOT)
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
;    3127 	{
;    3128 	        curr_sel = 10;
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL SUBOPT_0xF5
;    3129 	        msg = KEY_OK;
;    3130 	}
;    3131 	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
_0x2F0:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRLO _0x2F2
	CPI  R26,LOW(0x3A)
	BRLO _0x2F3
_0x2F2:
	RJMP _0x2F1
_0x2F3:
;    3132 	        curr_sel = (u8)(msg - KEY_NUM1 + 1) ;
	LDD  R30,Y+4
	SUBI R30,LOW(49)
	SUBI R30,-LOW(1)
	LDI  R31,0
	CALL SUBOPT_0xF5
;    3133 	        msg = KEY_OK;
;    3134 	}
;    3135         if (msg == KEY_NUM0)
_0x2F1:
	LDD  R26,Y+4
	CPI  R26,LOW(0x30)
	BRNE _0x2F4
;    3136         {
;    3137                 curr_sel = 0;
	LDI  R30,0
	STS  _curr_sel_S51,R30
	STS  _curr_sel_S51+1,R30
;    3138                 msg = KEY_OK;
	LDI  R30,LOW(79)
	STD  Y+4,R30
;    3139         }
;    3140 	if(msg == KEY_CE ) {
_0x2F4:
	LDD  R26,Y+4
	CPI  R26,LOW(0x43)
	BRNE _0x2F5
;    3141 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3142 		return;
	RJMP _0x4AA
;    3143 	}
;    3144 	if(msg == KEY_OK)
_0x2F5:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BREQ PC+3
	JMP _0x2F6
;    3145 	{
;    3146 	        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x2F7
;    3147 	        {
;    3148         		switch(curr_sel)
	CALL SUBOPT_0xF6
;    3149 	        	{
;    3150 	        	        case 0:
	SBIW R30,0
	BRNE _0x2FB
;    3151 	        	               return;
	RJMP _0x4AA
;    3152 		        	case 1:
_0x2FB:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x2FC
;    3153         			       return;
	RJMP _0x4AA
;    3154 	        		case 2:
_0x2FC:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x2FE
;    3155 			               return;
	RJMP _0x4AA
;    3156         			default:
_0x2FE:
;    3157 	        		       tprbdata.type[curr_prb-1] = curr_sel - 3+PRBTYPE_K;
	CALL SUBOPT_0xF7
	CALL SUBOPT_0xF6
	SBIW R30,3
	ADIW R30,3
	CALL __EEPROMWRB
;    3158 		        	       break;
;    3159         		}
;    3160         	}else{
	RJMP _0x2FF
_0x2F7:
;    3161         		switch(curr_sel)
	CALL SUBOPT_0xF6
;    3162 	        	{
;    3163 	        	        case 0:
	SBIW R30,0
	BRNE _0x303
;    3164 	        	               rprbdata.type[curr_prb-1] = PRBTYPE_PT1000;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(243)
	CALL __EEPROMWRB
;    3165 	        	               break;
	RJMP _0x302
;    3166 		        	case 1:
_0x303:
	CPI  R30,LOW(0x1)
	LDI  R26,HIGH(0x1)
	CPC  R31,R26
	BRNE _0x304
;    3167 			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(241)
	CALL __EEPROMWRB
;    3168         			       break;
	RJMP _0x302
;    3169 	        		case 2:
_0x304:
	CPI  R30,LOW(0x2)
	LDI  R26,HIGH(0x2)
	CPC  R31,R26
	BRNE _0x306
;    3170                        	               sprintf(strbuf,"输入Rtp:");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,621
	CALL SUBOPT_0x32
;    3171         	                       rprbdata.rtp[curr_prb-1] = wnd_floatinput(rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x8C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x30
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3172 		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;
	CALL SUBOPT_0xF8
	LDI  R30,LOW(242)
	CALL __EEPROMWRB
;    3173 			               break;
	RJMP _0x302
;    3174         			default:
_0x306:
;    3175                                        return;
	RJMP _0x4AA
;    3176         		}
_0x302:
;    3177         	}
_0x2FF:
;    3178 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3179 		return;
	RJMP _0x4AA
;    3180 	}
;    3181 	if(msg == MSG_INIT)
_0x2F6:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x307
;    3182 	{
;    3183 		LCD_Cls();
	CALL _LCD_Cls
;    3184 		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,630
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3185 		draw_label(&tplist_banner, SW_NORMAL);
	LDI  R30,LOW(_tplist_banner*2)
	LDI  R31,HIGH(_tplist_banner*2)
	CALL SUBOPT_0x37
;    3186 		INIT_OPTIONS(tplist_);
	MOV  R19,R17
_0x309:
	CP   R16,R19
	BRLO _0x30A
	CALL SUBOPT_0xFC
	CALL SUBOPT_0x37
	SUBI R19,-1
	RJMP _0x309
_0x30A:
;    3187 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3188 	}
;    3189 	if(msg == KEY_UP) {
_0x307:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x30B
;    3190 		DEC_OPTION;
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
;    3191 		msg = MSG_REFRESH;
_0x30C:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3192 	}
;    3193 	if(msg == KEY_DN) {
_0x30B:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x30D
;    3194 		INC_OPTION;
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
;    3195 		msg = MSG_REFRESH;
_0x30E:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3196 	}
;    3197 	if(msg == MSG_REFRESH) {
_0x30D:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x30F
;    3198 		REFRESH_OPTIONS(tplist_);
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
;    3199 	}
;    3200 	return;
_0x30F:
_0x4AA:
	CALL __LOADLOCR4
	ADIW R28,5
	RET
;    3201 }
;    3202 /*
;    3203  *       Convert the probe type const to string
;    3204  */
;    3205 char PRBSTR[7];

	.DSEG
_PRBSTR:
	.BYTE 0x7
;    3206 char* getprbtype(uchar prbtype)
;    3207 {

	.CSEG
_getprbtype:
;    3208 	switch(prbtype)
;	prbtype -> Y+0
	LD   R30,Y
;    3209 	{
;    3210 	        case PRBTYPE_PT1000:
	CPI  R30,LOW(0xF3)
	BRNE _0x319
;    3211 		        sprintf(PRBSTR,"PT1000");
	CALL SUBOPT_0x101
	__POINTW1FN _0,527
	CALL SUBOPT_0x32
;    3212 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3213 		case PRBTYPE_PT25:
_0x319:
	CPI  R30,LOW(0xF2)
	BRNE _0x31A
;    3214 		        sprintf(PRBSTR,"PT  25");
	CALL SUBOPT_0x101
	__POINTW1FN _0,648
	CALL SUBOPT_0x32
;    3215 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3216 		case PRBTYPE_PT100:
_0x31A:
	CPI  R30,LOW(0xF1)
	BRNE _0x31B
;    3217 		        sprintf(PRBSTR,"PT 100");
	CALL SUBOPT_0x101
	__POINTW1FN _0,655
	CALL SUBOPT_0x32
;    3218 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3219 		case PRBTYPE_T:
_0x31B:
	CPI  R30,LOW(0x9)
	BRNE _0x31C
;    3220 		        sprintf(PRBSTR,"T TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,662
	CALL SUBOPT_0x32
;    3221 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3222 
;    3223 		case PRBTYPE_K:
_0x31C:
	CPI  R30,LOW(0x3)
	BRNE _0x31D
;    3224 		        sprintf(PRBSTR,"K TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,669
	CALL SUBOPT_0x32
;    3225 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3226 		case PRBTYPE_N:
_0x31D:
	CPI  R30,LOW(0x4)
	BRNE _0x31E
;    3227 		        sprintf(PRBSTR,"N TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,676
	CALL SUBOPT_0x32
;    3228 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3229 		case PRBTYPE_E:
_0x31E:
	CPI  R30,LOW(0x5)
	BRNE _0x31F
;    3230 		        sprintf(PRBSTR,"E TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,683
	CALL SUBOPT_0x32
;    3231 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3232 		case PRBTYPE_B:
_0x31F:
	CPI  R30,LOW(0x6)
	BRNE _0x320
;    3233 		        sprintf(PRBSTR,"B TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,690
	CALL SUBOPT_0x32
;    3234 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3235 		case PRBTYPE_J:
_0x320:
	CPI  R30,LOW(0x7)
	BRNE _0x321
;    3236 		        sprintf(PRBSTR,"J TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,697
	CALL SUBOPT_0x32
;    3237 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3238 		case PRBTYPE_S:
_0x321:
	CPI  R30,LOW(0x8)
	BRNE _0x322
;    3239 		        sprintf(PRBSTR,"S TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,704
	CALL SUBOPT_0x32
;    3240 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3241 		case PRBTYPE_R:
_0x322:
	CPI  R30,LOW(0xA)
	BRNE _0x318
;    3242 		        sprintf(PRBSTR,"R TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,711
	CALL SUBOPT_0x32
;    3243 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3244 
;    3245 	}
_0x318:
;    3246 
;    3247 		        sprintf(PRBSTR,"? TYPE");
	CALL SUBOPT_0x101
	__POINTW1FN _0,718
	CALL SUBOPT_0x32
;    3248 		        return PRBSTR;
	LDI  R30,LOW(_PRBSTR)
	LDI  R31,HIGH(_PRBSTR)
	RJMP _0x4A9
;    3249 
;    3250 }
;    3251 //config r probe parameter
;    3252 LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
;    3253 LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
;    3254 LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
;    3255 LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
;    3256 LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
;    3257 LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
;    3258 LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
;    3259 LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
;    3260 LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
;    3261 LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
;    3262 LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
;    3263 LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
;    3264 LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};
;    3265 
;    3266 //configuration of BORE probe parameter
;    3267 void pgprbconfig_handler(uchar msg) {
_pgprbconfig_handler:
;    3268 	if( msg == KEY_CE) {
;	msg -> Y+0
	LD   R26,Y
	CPI  R26,LOW(0x43)
	BRNE _0x32A
;    3269 		nextwin = PG_PRBSET;
	LDI  R30,LOW(10)
	MOV  R4,R30
;    3270 		return;
	RJMP _0x4A9
;    3271 	}
;    3272 	if(msg == MSG_INIT)
_0x32A:
	LD   R26,Y
	CPI  R26,LOW(0xFF)
	BREQ PC+3
	JMP _0x32B
;    3273 	{
;    3274 		LCD_Cls();
	CALL _LCD_Cls
;    3275 		if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x32C
;    3276 		{
;    3277 		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,750
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3278         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x37
;    3279 	        	//name
;    3280 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3281         		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0x103
	CALL _tname2b
	CALL SUBOPT_0x36
	CALL SUBOPT_0xB2
;    3282 	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x37
;    3283         		//type
;    3284 	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0xF7
	CALL SUBOPT_0x104
	CALL SUBOPT_0xB2
;    3285 		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	RJMP _0x4E0
;    3286 		}else{
_0x32C:
;    3287 		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,767
	CALL SUBOPT_0xFB
	CALL SUBOPT_0xB2
;    3288         		draw_label(&pgprbcfg_banner,SW_NORMAL);
	LDI  R30,LOW(_pgprbcfg_banner*2)
	LDI  R31,HIGH(_pgprbcfg_banner*2)
	CALL SUBOPT_0x37
;    3289 	        	//name
;    3290 		        strbuf[0]=':';
	LDI  R30,LOW(58)
	STS  _strbuf,R30
;    3291 		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x32E
;    3292 		        {
;    3293         		        sprintf(strbuf,":%s",rname2b(curr_prb-1));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0x103
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xB2
;    3294         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
	RJMP _0x4E1
;    3295         	        }else{  //PT1000 or PT100
_0x32E:
;    3296                		        sprintf(strbuf,":%s Rtp:%7f",rname2b(curr_prb-1),rprbdata.rtp[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,784
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x103
	CALL SUBOPT_0xD7
	CALL SUBOPT_0xF9
	CALL SUBOPT_0x30
	CALL __EEPROMRDD
	CALL __PUTPARD1
	CALL SUBOPT_0xD6
;    3297         	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
_0x4E1:
	LDI  R30,LOW(_snlbl*2)
	LDI  R31,HIGH(_snlbl*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_snval*2)
	LDI  R31,HIGH(_snval*2)
	CALL SUBOPT_0x37
;    3298         	        }
;    3299                		//type
;    3300                        	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	CALL SUBOPT_0x2F
	CALL SUBOPT_0x102
	CALL SUBOPT_0xF8
	CALL SUBOPT_0x104
	CALL SUBOPT_0xB2
;    3301 	                draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	LDI  R30,LOW(_typelbl*2)
	LDI  R31,HIGH(_typelbl*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_typeval*2)
	LDI  R31,HIGH(_typeval*2)
	CALL SUBOPT_0x37
;    3302 
;    3303 		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x330
;    3304 		        {
;    3305 		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,796
	CALL SUBOPT_0x105
	CALL SUBOPT_0x106
;    3306         		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3b*2)
	LDI  R31,HIGH(_paramlbl3b*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_paramval3b*2)
	LDI  R31,HIGH(_paramval3b*2)
	RJMP _0x4E0
;    3307         		}else{
_0x330:
;    3308         		        //param1
;    3309                 		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,806
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x107
	CALL SUBOPT_0x106
;    3310 	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
	LDI  R30,LOW(_paramlbl1*2)
	LDI  R31,HIGH(_paramlbl1*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_paramval1*2)
	LDI  R31,HIGH(_paramval1*2)
	CALL SUBOPT_0x37
;    3311         	        	//param2
;    3312 	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,814
	ST   -Y,R31
	ST   -Y,R30
	CALL SUBOPT_0x108
	CALL SUBOPT_0x30
	CALL SUBOPT_0x106
;    3313                 		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	LDI  R30,LOW(_paramlbl2*2)
	LDI  R31,HIGH(_paramlbl2*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_paramval2*2)
	LDI  R31,HIGH(_paramval2*2)
	CALL SUBOPT_0x37
;    3314 	                	//param3
;    3315 		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x2F
	__POINTW1FN _0,822
	CALL SUBOPT_0x105
	CALL SUBOPT_0x106
;    3316         		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
	LDI  R30,LOW(_paramlbl3*2)
	LDI  R31,HIGH(_paramlbl3*2)
	CALL SUBOPT_0x37
	LDI  R30,LOW(_paramval3*2)
	LDI  R31,HIGH(_paramval3*2)
_0x4E0:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	RCALL _draw_label
;    3317         		}
;    3318                 }
;    3319 		return;
	RJMP _0x4A9
;    3320 	}
;    3321 	if(msg == KEY_NUM1) {
_0x32B:
	LD   R26,Y
	CPI  R26,LOW(0x31)
	BRNE _0x332
;    3322 		window_setup(7);
	LDI  R30,LOW(7)
	CALL SUBOPT_0xBF
;    3323 		sprintf(strbuf,"请输入探头序列号");
	__POINTW1FN _0,830
	CALL SUBOPT_0x32
;    3324 		prbsninput();
	RCALL _prbsninput
;    3325 		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3326 		return;
	RJMP _0x4A9
;    3327 	}
;    3328 	if(msg == KEY_NUM2){
_0x332:
	LD   R26,Y
	CPI  R26,LOW(0x32)
	BRNE _0x333
;    3329 		nextwin = PG_PRBTYPELIST;
	LDI  R30,LOW(9)
	MOV  R4,R30
;    3330 		return;
	RJMP _0x4A9
;    3331 	}
;    3332 	if(msg == KEY_NUM3 ){
_0x333:
	LD   R26,Y
	CPI  R26,LOW(0x33)
	BRNE _0x334
;    3333 
;    3334 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x335
;    3335 	        {
;    3336                		window_setup(10);
	LDI  R30,LOW(10)
	STS  _max_databuf,R30
;    3337 	                if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF2)
	BREQ _0x336
;    3338 	                {
;    3339         	                sprintf(strbuf,"输入R(0!)");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,847
	CALL SUBOPT_0x32
;    3340         	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
	CALL SUBOPT_0x109
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x107
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	RJMP _0x4E2
;    3341                         }else{
_0x336:
;    3342         	        	sprintf(strbuf,"输入系数a");
	CALL SUBOPT_0x2F
	__POINTW1FN _0,857
	CALL SUBOPT_0x32
;    3343         	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
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
;    3344         	        }
;    3345         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3346         	}
;    3347 		return;
_0x335:
	RJMP _0x4A9
;    3348 	}
;    3349 	if(msg == KEY_NUM4) {
_0x334:
	LD   R26,Y
	CPI  R26,LOW(0x34)
	BRNE _0x338
;    3350         	if(IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x339
;    3351         	{
;    3352 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x33A
;    3353 	                        return;
	RJMP _0x4A9
;    3354 	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
_0x33A:
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF3)
	BRNE _0x33B
;    3355 	                        return;
	RJMP _0x4A9
;    3356         		window_setup(10);
_0x33B:
	CALL SUBOPT_0xBB
;    3357 	        	sprintf(strbuf,"输入系数b");
	__POINTW1FN _0,867
	CALL SUBOPT_0x32
;    3358 		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
	CALL SUBOPT_0x108
	CALL SUBOPT_0x8C
	PUSH R31
	PUSH R30
	CALL SUBOPT_0x108
	CALL SUBOPT_0x30
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3359         		nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3360                 }
;    3361 		return;
_0x339:
	RJMP _0x4A9
;    3362 	}
;    3363 	if(msg == KEY_NUM5) {
_0x338:
	LD   R26,Y
	CPI  R26,LOW(0x35)
	BRNE _0x33C
;    3364 	        if(IS_BORE_MODE)
	CALL SUBOPT_0x33
	BRNE _0x33D
;    3365 	        {
;    3366        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF1)
	BRNE _0x33E
;    3367        	                        return;
	RJMP _0x4A9
;    3368        	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
_0x33E:
	CALL SUBOPT_0xF8
	CALL __EEPROMRDB
	CPI  R30,LOW(0xF3)
	BRNE _0x33F
;    3369        	                        return;
	RJMP _0x4A9
;    3370 	                window_setup(10);
_0x33F:
	CALL SUBOPT_0xBB
;    3371        		        sprintf(strbuf,"输入系数c");
	__POINTW1FN _0,877
	CALL SUBOPT_0x32
;    3372         		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
	CALL SUBOPT_0x109
	PUSH R31
	PUSH R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	CALL SUBOPT_0x30
	CALL SUBOPT_0xFA
	POP  R26
	POP  R27
	CALL __EEPROMWRD
;    3373 		        nextwin = PG_PRBCONFIG;
	LDI  R30,LOW(11)
	MOV  R4,R30
;    3374 	        }
;    3375 		return;
_0x33D:
;    3376 	}
;    3377 	return;
_0x33C:
_0x4A9:
	ADIW R28,1
	RET
;    3378 }
;    3379 
;    3380 LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
;    3381 LABEL flash boot_options[] = {
;    3382  	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
;    3383  	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
;    3384  	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
;    3385  	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
;    3386 };
;    3387 //boot up menu
;    3388 extern u8 eeprom scanner_type;
;    3389 void pgboottype_handler(uchar msg) {
_pgboottype_handler:
;    3390 	uchar i;
;    3391 	static uchar curr_sel = 1;

	.DSEG
_curr_sel_S54:
	.BYTE 0x1

	.CSEG
;    3392 	static uchar last_sel = 0xff;

	.DSEG
_last_sel_S54:
	.BYTE 0x1

	.CSEG
;    3393 
;    3394 	uchar min_option = 1;
;    3395 	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
;    3396 	if(msg == KEY_NUM5) { //select scanner type
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
;    3397        		window_setup(4); //2 char at max
	LDI  R30,LOW(4)
	CALL SUBOPT_0xBF
;    3398 		sprintf(strbuf,"Scanner(1:MI,2:GUIDLINE)");
	__POINTW1FN _0,954
	CALL SUBOPT_0x32
;    3399 		i = scanner_type;
	CALL SUBOPT_0x9C
	MOV  R17,R30
;    3400 		i = wnd_intinput(i);
	ST   -Y,R17
	RCALL _wnd_intinput
	MOV  R17,R30
;    3401 		if(i == 1 || i == 2)
	CPI  R17,1
	BREQ _0x349
	CPI  R17,2
	BRNE _0x348
_0x349:
;    3402 		        scanner_type = i;
	MOV  R30,R17
	RJMP _0x4E3
;    3403 		else
_0x348:
;    3404 		        scanner_type = 1;
	LDI  R30,LOW(1)
_0x4E3:
	LDI  R26,LOW(_scanner_type)
	LDI  R27,HIGH(_scanner_type)
	CALL __EEPROMWRB
;    3405 	        msg = MSG_INIT;
	LDI  R30,LOW(255)
	STD  Y+4,R30
;    3406 	}
;    3407 
;    3408 	if(msg == MSG_INIT) {
_0x347:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFF)
	BRNE _0x34C
;    3409 		LCD_Cls();
	CALL _LCD_Cls
;    3410 		last_sel = 0xff;
	LDI  R30,LOW(255)
	STS  _last_sel_S54,R30
;    3411 		draw_label(&boot_banner, SW_NORMAL);
	LDI  R30,LOW(_boot_banner*2)
	LDI  R31,HIGH(_boot_banner*2)
	CALL SUBOPT_0x37
;    3412 		INIT_OPTIONS(boot_);
	MOV  R17,R16
_0x34E:
	CP   R19,R17
	BRLO _0x34F
	CALL SUBOPT_0x10A
	CALL SUBOPT_0x37
	SUBI R17,-1
	RJMP _0x34E
_0x34F:
;    3413 		msg = MSG_REFRESH;
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3414 	}
;    3415 	if(msg == KEY_DN)
_0x34C:
	LDD  R26,Y+4
	CPI  R26,LOW(0x44)
	BRNE _0x350
;    3416 	{
;    3417 	        last_sel = curr_sel;
	CALL SUBOPT_0x10B
;    3418 	        curr_sel++;
	SUBI R30,-LOW(1)
	STS  _curr_sel_S54,R30
;    3419 	        if(curr_sel > max_option)
	LDS  R26,_curr_sel_S54
	CP   R19,R26
	BRSH _0x351
;    3420 	                curr_sel = 1;
	LDI  R30,LOW(1)
	STS  _curr_sel_S54,R30
;    3421 	        msg = MSG_REFRESH;
_0x351:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3422 	}
;    3423 	if(msg == KEY_UP)
_0x350:
	LDD  R26,Y+4
	CPI  R26,LOW(0x55)
	BRNE _0x352
;    3424 	{
;    3425 	        last_sel = curr_sel;
	CALL SUBOPT_0x10B
;    3426 	        curr_sel--;
	SUBI R30,LOW(1)
	STS  _curr_sel_S54,R30
;    3427 	        if(curr_sel == 0)
	CPI  R30,0
	BRNE _0x353
;    3428 	                curr_sel = max_option;
	STS  _curr_sel_S54,R19
;    3429 	        msg = MSG_REFRESH;
_0x353:
	LDI  R30,LOW(254)
	STD  Y+4,R30
;    3430 	}
;    3431 	if(msg == KEY_OK)
_0x352:
	LDD  R26,Y+4
	CPI  R26,LOW(0x4F)
	BRNE _0x354
;    3432 	{
;    3433 	        msg = curr_sel + KEY_NUM0;
	LDS  R30,_curr_sel_S54
	SUBI R30,-LOW(48)
	STD  Y+4,R30
;    3434 	}
;    3435 	if(msg == KEY_NUM1) {
_0x354:
	LDD  R26,Y+4
	CPI  R26,LOW(0x31)
	BRNE _0x355
;    3436 		SET_BORE_MODE;
	CALL SUBOPT_0x3A
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3B
;    3437 		SET_TORS;
	CALL SUBOPT_0x1E
;    3438 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3439 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3440 	}
;    3441 	if(msg == KEY_NUM2) {
_0x355:
	LDD  R26,Y+4
	CPI  R26,LOW(0x32)
	BRNE _0x356
;    3442 		SET_BORE_MODE;
	CALL SUBOPT_0x3A
	CALL SUBOPT_0x1E
	CALL SUBOPT_0x3B
;    3443 		SET_TORS;
	CALL SUBOPT_0x1E
;    3444 		nextwin = PG_RCONFIG;
	LDI  R30,LOW(3)
	MOV  R4,R30
;    3445 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3446 	}
;    3447 	if(msg == KEY_NUM3) {
_0x356:
	LDD  R26,Y+4
	CPI  R26,LOW(0x33)
	BRNE _0x357
;    3448 		SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    3449 		nextwin = PG_MAIN;
	LDI  R30,LOW(2)
	MOV  R4,R30
;    3450 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3451 	}
;    3452 	if(msg == KEY_NUM4) {
_0x357:
	LDD  R26,Y+4
	CPI  R26,LOW(0x34)
	BRNE _0x358
;    3453 		SET_THERM_MODE;
	CALL SUBOPT_0x3F
	CALL SUBOPT_0xC1
;    3454 		nextwin = PG_TCONFIG;
	LDI  R30,LOW(4)
	MOV  R4,R30
;    3455 		return;
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3456 	}
;    3457 	if(msg == MSG_REFRESH) {
_0x358:
	LDD  R26,Y+4
	CPI  R26,LOW(0xFE)
	BRNE _0x359
;    3458 		REFRESH_OPTIONS(boot_);
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
;    3459 	}
;    3460 }
_0x359:
	CALL __LOADLOCR4
	RJMP _0x4A7
;    3461 #include "utili.h"
;    3462 #include "window.h"
;    3463 #include "stdlib.h"
;    3464 /*---------------延时子程序----------------*/
;    3465 /*
;    3466 void delay1 (uint ms)
;    3467 {
;    3468   	int i,j;
;    3469   	for(i=0;i<ms;i++)
;    3470   	for(j=0;j<1000;j++)
;    3471   		;
;    3472 
;    3473 }
;    3474 */
;    3475 /*
;    3476 char lowc(uchar x)
;    3477 {
;    3478 	x = x&0x0f;
;    3479 	if(x > 0x09)
;    3480 		return 'A'+x-0x0a;
;    3481 	else
;    3482 		return '0'+x;
;    3483 }
;    3484 char highc(uchar x)
;    3485 {
;    3486 
;    3487 	x = x>>4;
;    3488 	if(x > 0x09)
;    3489 		return 'A'+x-0x0a;
;    3490 	else
;    3491 		return '0'+x;
;    3492 }
;    3493 */
;    3494 /*计算字符串长度*/
;    3495 
;    3496 extern uchar pos_databuf;
;    3497 extern uchar data_sign;
;    3498 extern u8 databuf[12];
;    3499 double buf2double()
;    3500 {
_buf2double:
;    3501         return atof(databuf);
	CALL SUBOPT_0x10C
	CALL _atof
	RET
;    3502         /*
;    3503 	double tmp = 0.0;
;    3504 	uchar i = 0;
;    3505 	uchar pos = 0;
;    3506 	for(i=1;i<pos_databuf;i++) {
;    3507 		if(databuf[i] != KEY_DOT)
;    3508 			tmp = tmp * 10.0+(databuf[i] - '0');
;    3509 		else
;    3510 			pos = pos_databuf - i - 2;
;    3511 	}
;    3512 	while(pos > 0) {
;    3513 		tmp = tmp / 10.0;
;    3514 		pos--;
;    3515 	}
;    3516 	if(data_sign == 1)
;    3517 	        tmp = -tmp;
;    3518        	return tmp;
;    3519        	*/
;    3520 }
;    3521 int buf2byte()	    //convert rundata.tempbuf to byte (00-99)
;    3522 {
_buf2byte:
;    3523         return atoi(databuf);
	CALL SUBOPT_0x10C
	CALL _atoi
	RET
;    3524         /*
;    3525 	int tmp = 0;
;    3526 	uchar i;
;    3527 	for(i=0;i<pos_databuf;i++) {
;    3528 		tmp = tmp * 10+(databuf[i] - '0');
;    3529 	}
;    3530 	return tmp;
;    3531 	*/
;    3532 }
;    3533 
;    3534 
;    3535 #include "window.h"
;    3536 #include "utili.h"
;    3537 #include "lcd.h"
;    3538 
;    3539 MSG_HANDLER curr_window = 0;

	.DSEG
_curr_window:
	.BYTE 0x2
;    3540 
;    3541 extern uchar key;
;    3542 
;    3543 u8 databuf[12];
_databuf:
	.BYTE 0xC
;    3544 u8 pos_databuf; //position in data buffer
_pos_databuf:
	.BYTE 0x1
;    3545 u8 max_databuf;
_max_databuf:
	.BYTE 0x1
;    3546 u8 data_sign;   // sign of the data
_data_sign:
	.BYTE 0x1
;    3547 LABEL flash datalbl = {LBL_HZ16,10,10,8,strbuf};

	.CSEG
;    3548 LABEL flash datalbl2 = {LBL_HZ6X8,140,54,8,"UP:+/-,DN:'E'"};
;    3549 LABEL flash datalbl3 = {LBL_HZ6X8,140,54,8,"UP/DN:'A'-'Z'"};
;    3550 LABEL flash databox = {LBL_HZ16,20,30,9,databuf};
;    3551 
;    3552 
;    3553 void prbsninput()
;    3554 {
_prbsninput:
;    3555 	uchar msg,len;
;    3556 	databuf[0] = '\0';
	ST   -Y,R17
	ST   -Y,R16
;	msg -> R17
;	len -> R16
	CALL SUBOPT_0x10D
;    3557 	pos_databuf = 0;
;    3558 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3559 	while(1)
_0x363:
;    3560 	{
;    3561 		if(key == KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x363
;    3562 		        continue;
;    3563 
;    3564 		msg = key;
	MOV  R17,R5
;    3565 
;    3566 
;    3567 		if(msg == MSG_INIT) {
	CPI  R17,255
	BRNE _0x367
;    3568 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3569 			draw_label(&datalbl,SW_NORMAL);
;    3570 			draw_label(&datalbl3,SW_NORMAL);
	LDI  R30,LOW(_datalbl3*2)
	LDI  R31,HIGH(_datalbl3*2)
	CALL SUBOPT_0x37
;    3571 			sprintf(databuf,"");
	CALL SUBOPT_0x10C
	__POINTW1FN _0,11
	CALL SUBOPT_0x32
;    3572 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3573 //			LCD_ShowCursor(databox.x,databox.y);
;    3574 		}
;    3575 		if(msg == KEY_TAB)
_0x367:
	CPI  R17,84
	BRNE _0x368
;    3576 		{
;    3577 		/*
;    3578         		if(pos_databuf < max_databuf)
;    3579         		{
;    3580 			        databuf[pos_databuf++] = '0';
;    3581 			        databuf[pos_databuf] = '\0';
;    3582         			msg = MSG_REFRESH;
;    3583                         }
;    3584                 */
;    3585                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3586                         return;
	RJMP _0x4A8
;    3587 		}
;    3588 		if(msg == KEY_CE) {
_0x368:
	CPI  R17,67
	BRNE _0x369
;    3589 		        key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3590 			return;
	RJMP _0x4A8
;    3591 		}
;    3592 
;    3593 		if(msg == KEY_DN) {
_0x369:
	CPI  R17,68
	BRNE _0x36A
;    3594 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x36B
;    3595 			{
;    3596 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x110
	CALL SUBOPT_0x111
;    3597 				databuf[pos_databuf] = '\0';
;    3598 			}
;    3599 
;    3600 			if(databuf[pos_databuf-1] == '0')
_0x36B:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x30)
	BRNE _0x36C
;    3601 				databuf[pos_databuf-1] = 'Z';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(90)
	STD  Z+0,R26
;    3602 			else if (databuf[pos_databuf-1] == 'A')
	RJMP _0x36D
_0x36C:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x41)
	BRNE _0x36E
;    3603 				databuf[pos_databuf-1] = '9';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(57)
	STD  Z+0,R26
;    3604 			else
	RJMP _0x36F
_0x36E:
;    3605 				databuf[pos_databuf-1] -= 1;
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,LOW(1)
	ST   X,R30
;    3606 			msg = MSG_REFRESH;
_0x36F:
_0x36D:
	LDI  R17,LOW(254)
;    3607 		}
;    3608 		if(msg == KEY_UP) {
_0x36A:
	CPI  R17,85
	BRNE _0x370
;    3609 			if(pos_databuf == 0)
	LDS  R30,_pos_databuf
	CPI  R30,0
	BRNE _0x371
;    3610 			{
;    3611 				databuf[pos_databuf++] = '0';
	CALL SUBOPT_0x110
	CALL SUBOPT_0x111
;    3612 				databuf[pos_databuf] = '\0';
;    3613 			}
;    3614 
;    3615 			if(databuf[pos_databuf-1] == '9')
_0x371:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x39)
	BRNE _0x372
;    3616 				databuf[pos_databuf-1] = 'A';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(65)
	STD  Z+0,R26
;    3617 			else if (databuf[pos_databuf-1] == 'Z')
	RJMP _0x373
_0x372:
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CPI  R30,LOW(0x5A)
	BRNE _0x374
;    3618 				databuf[pos_databuf-1] = '0';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(48)
	STD  Z+0,R26
;    3619 			else
	RJMP _0x375
_0x374:
;    3620 				databuf[pos_databuf-1] +=1;
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	MOVW R26,R30
	LD   R30,X
	SUBI R30,-LOW(1)
	ST   X,R30
;    3621 			msg = MSG_REFRESH;
_0x375:
_0x373:
	LDI  R17,LOW(254)
;    3622 		}
;    3623 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x370:
	CPI  R17,48
	BRLO _0x377
	CPI  R17,58
	BRLO _0x378
_0x377:
	RJMP _0x376
_0x378:
;    3624         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x379
;    3625         		{
;    3626 			        databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3627         			databuf[pos_databuf] = '\0';
;    3628         		}
;    3629 			msg = MSG_REFRESH;
_0x379:
	LDI  R17,LOW(254)
;    3630 		}
;    3631 		if(msg == KEY_OK) {
_0x376:
	CPI  R17,79
	BRNE _0x37A
;    3632 		        if(IS_THERM_MODE)
	CALL SUBOPT_0x42
	BRNE _0x37B
;    3633 		        {
;    3634 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x37D:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x37E
;    3635         		                tprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _tprbdata,384
	CALL SUBOPT_0x115
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3636         			tprbdata.name[curr_prb-1][pos_databuf] = '\0';
	SUBI R16,-1
	RJMP _0x37D
_0x37E:
	__POINTW2MN _tprbdata,384
	RJMP _0x4E4
;    3637         		}else{
_0x37B:
;    3638 			        for(len = 0; len < pos_databuf; len++)
	LDI  R16,LOW(0)
_0x381:
	LDS  R30,_pos_databuf
	CP   R16,R30
	BRSH _0x382
;    3639         		                rprbdata.name[curr_prb-1][len] = databuf[len];
	__POINTW2MN _rprbdata,384
	CALL SUBOPT_0x115
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LD   R30,Z
	CALL __EEPROMWRB
;    3640         			rprbdata.name[curr_prb-1][pos_databuf] = '\0';
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
;    3641         		}
;    3642 			key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3643 			break;
	RJMP _0x365
;    3644 		}
;    3645 		if(msg == MSG_REFRESH) {
_0x37A:
	CPI  R17,254
	BRNE _0x383
;    3646 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3647 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3648 		}
;    3649 		key = KEY_INVALID;
_0x383:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3650 	}
	RJMP _0x363
_0x365:
;    3651 //	LCD_HideCursor();
;    3652 }
_0x4A8:
	LD   R16,Y+
	LD   R17,Y+
	RET
;    3653 
;    3654 uchar wnd_intinput(uchar lastval)
;    3655 {
_wnd_intinput:
;    3656 	uchar msg;
;    3657 
;    3658 	key = MSG_INIT;
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3659 	databuf[0] = '\0';
	CALL SUBOPT_0x10D
;    3660 	pos_databuf = 0;
;    3661         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3662 	while(1)
_0x384:
;    3663 	{
;    3664 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x387
;    3665 		{
;    3666 			msg = key;
	MOV  R17,R5
;    3667 
;    3668 		}else{
	RJMP _0x388
_0x387:
;    3669 			continue;
	RJMP _0x384
;    3670 		}
_0x388:
;    3671 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x389
;    3672 		{
;    3673 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3674 			draw_label(&datalbl,SW_NORMAL);
;    3675 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3676 //			LCD_ShowCursor(databox.x,databox.y);
;    3677 
;    3678 		}
;    3679 
;    3680 		if(msg == KEY_TAB)
_0x389:
	CPI  R17,84
	BRNE _0x38A
;    3681 		{
;    3682 		/*
;    3683         		if(pos_databuf < max_databuf)
;    3684         		{
;    3685 			        databuf[pos_databuf++] = '0';
;    3686 			        databuf[pos_databuf] = '\0';
;    3687         			msg = MSG_REFRESH;
;    3688                         }
;    3689                 */
;    3690 		}
;    3691 
;    3692 		if(msg == KEY_DN) {
_0x38A:
	CPI  R17,68
	BRNE _0x38B
;    3693 		/*
;    3694 			if(pos_databuf == 0)
;    3695 			{
;    3696 				databuf[pos_databuf++] = '0';
;    3697 				databuf[pos_databuf] = '\0';
;    3698 			}
;    3699 
;    3700 			if(databuf[pos_databuf-1] == '0')
;    3701 				databuf[pos_databuf-1] = '9';
;    3702 			else
;    3703 				databuf[pos_databuf-1] -= 1;
;    3704 			msg = MSG_REFRESH;
;    3705                 */
;    3706 		}
;    3707 		if(msg == KEY_UP) {
_0x38B:
	CPI  R17,85
	BRNE _0x38C
;    3708 		/*
;    3709 			if(pos_databuf == 0)
;    3710 			{
;    3711 				databuf[pos_databuf++] = '0';
;    3712 				databuf[pos_databuf] = '\0';
;    3713 			}
;    3714 
;    3715 			if(databuf[pos_databuf-1] == '9')
;    3716 				databuf[pos_databuf-1] = '0';
;    3717 			else
;    3718 				databuf[pos_databuf-1] += 1;
;    3719 			msg = MSG_REFRESH;
;    3720                 */
;    3721 		}
;    3722 		if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
_0x38C:
	CPI  R17,48
	BRLO _0x38E
	CPI  R17,58
	BRLO _0x38F
_0x38E:
	RJMP _0x38D
_0x38F:
;    3723         		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x390
;    3724         		{
;    3725         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3726 	        		databuf[pos_databuf] = '\0';
;    3727 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3728 		        }
;    3729 		}
_0x390:
;    3730 		if(msg == KEY_CE) {
_0x38D:
	CPI  R17,67
	BRNE _0x391
;    3731 //			LCD_HideCursor();
;    3732                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3733 			return lastval;
	LDD  R30,Y+1
	LDD  R17,Y+0
	RJMP _0x4A6
;    3734 		}
;    3735 		if(msg == KEY_OK){
_0x391:
	CPI  R17,79
	BRNE _0x392
;    3736 //			LCD_HideCursor();
;    3737                         key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3738 			return buf2byte();
	CALL _buf2byte
	LDD  R17,Y+0
	RJMP _0x4A6
;    3739 		}
;    3740 		if(msg == MSG_REFRESH) {
_0x392:
	CPI  R17,254
	BRNE _0x393
;    3741 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3742 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3743 		}
;    3744 		key = KEY_INVALID;
_0x393:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3745 	}
	RJMP _0x384
;    3746 }
;    3747 double wnd_floatinput(double lastval)
;    3748 {
_wnd_floatinput:
;    3749 	uchar msg;
;    3750 
;    3751 	databuf[0] = '+';
	ST   -Y,R17
;	lastval -> Y+1
;	msg -> R17
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3752         databuf[1] = '\0';
	__POINTW1MN _databuf,1
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3753         pos_databuf = 1;
	LDI  R30,LOW(1)
	STS  _pos_databuf,R30
;    3754         data_sign = 0;
	LDI  R30,LOW(0)
	STS  _data_sign,R30
;    3755 	key = MSG_INIT;
	LDI  R30,LOW(255)
	MOV  R5,R30
;    3756 	while(1)
_0x394:
;    3757 	{
;    3758 		if(key != KEY_INVALID)
	LDI  R30,LOW(45)
	CP   R30,R5
	BREQ _0x397
;    3759 		{
;    3760 			msg = key;
	MOV  R17,R5
;    3761 
;    3762 		}else{
	RJMP _0x398
_0x397:
;    3763 			continue;
	RJMP _0x394
;    3764 		}
_0x398:
;    3765 		if(msg == MSG_INIT)
	CPI  R17,255
	BRNE _0x399
;    3766 		{
;    3767 			LCD_Cls();
	CALL SUBOPT_0x10E
;    3768 			draw_label(&datalbl,SW_NORMAL);
;    3769 			draw_label(&datalbl2,SW_NORMAL);
	LDI  R30,LOW(_datalbl2*2)
	LDI  R31,HIGH(_datalbl2*2)
	CALL SUBOPT_0x37
;    3770 			draw_inputbox(&databox);
	CALL SUBOPT_0x10F
;    3771 //			LCD_ShowCursor(databox.x,databox.y);
;    3772 		}
;    3773 
;    3774 		if(msg == KEY_TAB)
_0x399:
	CPI  R17,84
	BRNE _0x39A
;    3775 		{
;    3776 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3777 		}
;    3778 		if(msg == KEY_UP) {
_0x39A:
	CPI  R17,85
	BREQ PC+3
	JMP _0x39B
;    3779 		        if(pos_databuf == 1)
	LDS  R26,_pos_databuf
	CPI  R26,LOW(0x1)
	BRNE _0x39C
;    3780 		        {
;    3781 		                if((data_sign & 0x01) == 0)      //no sign
	LDS  R30,_data_sign
	ANDI R30,LOW(0x1)
	BRNE _0x39D
;    3782 		                {
;    3783 		                        databuf[0] = '-';
	LDI  R30,LOW(45)
	STS  _databuf,R30
;    3784 		                        data_sign |= 0x01;
	LDS  R30,_data_sign
	ORI  R30,1
	RJMP _0x4E5
;    3785 		                }else{
_0x39D:
;    3786 		                        databuf[0] = '+';       //
	LDI  R30,LOW(43)
	STS  _databuf,R30
;    3787 		                        data_sign ^= 0x01;
	LDS  R26,_data_sign
	LDI  R30,LOW(1)
	EOR  R30,R26
_0x4E5:
	STS  _data_sign,R30
;    3788 		                }
;    3789 		        }else{
	RJMP _0x39F
_0x39C:
;    3790                        		if((pos_databuf < max_databuf) && (databuf[pos_databuf-2] == 'E'))
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
;    3791                 		{
;    3792 			                if((data_sign & 0x08) == 0)
	LDS  R30,_data_sign
	ANDI R30,LOW(0x8)
	BRNE _0x3A3
;    3793 			                {
;    3794                		        	        databuf[pos_databuf-1] = '-';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(45)
	STD  Z+0,R26
;    3795                		        	        data_sign |= 0x08;
	LDS  R30,_data_sign
	ORI  R30,8
	RJMP _0x4E6
;    3796                		                }else{
_0x3A3:
;    3797                		        	        databuf[pos_databuf-1] = '+';
	CALL SUBOPT_0x112
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3798                		        	        data_sign ^= 0x08;
	LDS  R26,_data_sign
	LDI  R30,LOW(8)
	EOR  R30,R26
_0x4E6:
	STS  _data_sign,R30
;    3799                		                }
;    3800 	        		}
;    3801 		        	msg = MSG_REFRESH;
_0x3A0:
	LDI  R17,LOW(254)
;    3802 		        }
_0x39F:
;    3803 		        msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3804 		}
;    3805 		if(msg == KEY_DN) {
_0x39B:
	CPI  R17,68
	BRNE _0x3A5
;    3806                		if((pos_databuf < max_databuf) && ((data_sign & 0x04) == 0))    //no E in string
	CALL SUBOPT_0x113
	BRSH _0x3A7
	LDS  R30,_data_sign
	ANDI R30,LOW(0x4)
	BREQ _0x3A8
_0x3A7:
	RJMP _0x3A6
_0x3A8:
;    3807         		{
;    3808         			databuf[pos_databuf++] = 'E';
	CALL SUBOPT_0x110
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(69)
	STD  Z+0,R26
;    3809         			databuf[pos_databuf++] = '+';
	CALL SUBOPT_0x110
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(43)
	STD  Z+0,R26
;    3810 	        		databuf[pos_databuf] = '\0';
	LDS  R30,_pos_databuf
	LDI  R31,0
	SUBI R30,LOW(-_databuf)
	SBCI R31,HIGH(-_databuf)
	LDI  R26,LOW(0)
	STD  Z+0,R26
;    3811 	        		data_sign |= 0x04;
	LDS  R30,_data_sign
	ORI  R30,4
	STS  _data_sign,R30
;    3812 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3813 		        }
;    3814 
;    3815 			msg = MSG_REFRESH;
_0x3A6:
	LDI  R17,LOW(254)
;    3816 		}
;    3817 		if( msg == KEY_DOT)
_0x3A5:
	CPI  R17,46
	BRNE _0x3A9
;    3818 		{
;    3819                		if((pos_databuf < max_databuf) && ((data_sign & 0x02) == 0))      //no dot in string
	CALL SUBOPT_0x113
	BRSH _0x3AB
	LDS  R30,_data_sign
	ANDI R30,LOW(0x2)
	BREQ _0x3AC
_0x3AB:
	RJMP _0x3AA
_0x3AC:
;    3820         		{
;    3821         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3822 	        		databuf[pos_databuf] = '\0';
;    3823 	        		data_sign |= 0x02;
	LDS  R30,_data_sign
	ORI  R30,2
	STS  _data_sign,R30
;    3824 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3825 		        }
;    3826 
;    3827 		}
_0x3AA:
;    3828 		if((msg >= KEY_NUM0 && msg <= KEY_NUM9)) {
_0x3A9:
	CPI  R17,48
	BRLO _0x3AE
	CPI  R17,58
	BRLO _0x3AF
_0x3AE:
	RJMP _0x3AD
_0x3AF:
;    3829                		if(pos_databuf < max_databuf)
	CALL SUBOPT_0x113
	BRSH _0x3B0
;    3830         		{
;    3831         			databuf[pos_databuf++] = msg;
	CALL SUBOPT_0x110
	CALL SUBOPT_0x114
;    3832 	        		databuf[pos_databuf] = '\0';
;    3833 		        	msg = MSG_REFRESH;
	LDI  R17,LOW(254)
;    3834 		        }
;    3835 		}
_0x3B0:
;    3836 		if(msg == KEY_CE) {
_0x3AD:
	CPI  R17,67
	BRNE _0x3B1
;    3837 //			LCD_HideCursor();
;    3838 		key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3839 			return lastval;
	__GETD1S 1
	LDD  R17,Y+0
	RJMP _0x4A7
;    3840 		}
;    3841 		if(msg == KEY_OK){
_0x3B1:
	CPI  R17,79
	BRNE _0x3B2
;    3842 				key = KEY_INVALID;
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3843 //			LCD_HideCursor();
;    3844 			return buf2double();
	CALL _buf2double
	LDD  R17,Y+0
	RJMP _0x4A7
;    3845 		}
;    3846 		if(msg == MSG_REFRESH) {
_0x3B2:
	CPI  R17,254
	BRNE _0x3B3
;    3847 			draw_label(&databox,SW_NORMAL);
	CALL SUBOPT_0x116
;    3848 //			LCD_ShowCursor(databox.x+pos_databuf*16,databox.y);
;    3849 		}
;    3850 				key = KEY_INVALID;
_0x3B3:
	LDI  R30,LOW(45)
	MOV  R5,R30
;    3851 	}
	RJMP _0x394
;    3852 }
_0x4A7:
	ADIW R28,5
	RET
;    3853 //display a message box
;    3854 void wnd_msgbox(flash LABEL *lbl)
;    3855 {
_wnd_msgbox:
;    3856 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14 , lbl->y+19);
;	*lbl -> Y+0
	CALL SUBOPT_0x117
	CALL _LCD_ClsBlock
;    3857 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x37
;    3858 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 -14, lbl->y+19);
	CALL SUBOPT_0x117
	CALL _LCD_Rectange
;    3859 	LCD_Rectange(lbl->x - 8,lbl->y - 1,lbl->x + lbl->width * 16 - 16 , lbl->y+17);
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
;    3860 
;    3861 }
	RJMP _0x4A6
;    3862 
;    3863 void draw_inputbox(flash LABEL *lbl)
;    3864 {
_draw_inputbox:
;    3865 	LCD_ClsBlock(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
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
;    3866 	LCD_Rectange(lbl->x - 10,lbl->y - 3,lbl->x + lbl->width * 16 , lbl->y+19);
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
;    3867 	draw_label(lbl, SW_NORMAL);
	LD   R30,Y
	LDD  R31,Y+1
	CALL SUBOPT_0x37
;    3868 }
_0x4A6:
	ADIW R28,2
	RET
;    3869 
;    3870 void draw_label(flash LABEL *lbl,uchar reverse) {
_draw_label:
;    3871 	uchar len = lbl->width;
;    3872 	if(lbl->type == LBL_HZ6X8) {
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
;    3873 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3B5
;    3874 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3B6
;    3875 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 8);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11E
	ADIW R30,8
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3876 			LCD_Print6X8( lbl->x, lbl->y, lbl->param);
_0x3B6:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_Print6X8
;    3877 			return;
	LDD  R17,Y+0
	RJMP _0x4A5
;    3878 		}
;    3879 		if(reverse & SW_REVERSE)
_0x3B5:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3B7
;    3880 			LCD_ReverseRect(lbl->x, lbl->y, len , 8);
	CALL SUBOPT_0x120
	LDI  R30,LOW(8)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3881 		return;
_0x3B7:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3882 	}
;    3883 	if(lbl->type == LBL_HZ8X16) {
_0x3B4:
	CALL SUBOPT_0x11C
	SBIW R30,2
	BRNE _0x3B8
;    3884 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3B9
;    3885 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3BA
;    3886 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<3), lbl->y + 16);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11E
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3887 			LCD_Print8X16( lbl->x, lbl->y, lbl->param);
_0x3BA:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_Print8X16
;    3888 			return;
	LDD  R17,Y+0
	RJMP _0x4A5
;    3889 		}
;    3890 		if(reverse & SW_REVERSE)
_0x3B9:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3BB
;    3891 			LCD_ReverseRect(lbl->x, lbl->y, len * 1, 16);
	CALL SUBOPT_0x120
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3892 		return;
_0x3BB:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3893 	}
;    3894 	/*
;    3895 	if(lbl->type == LBL_HZ24X32) {
;    3896 		if(reverse & SW_NORMAL) {
;    3897 			if((reverse & SW_OVERLAP) == 0)
;    3898 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + len * 24, lbl->y + 32);
;    3899 			LCD_Print24X32( lbl->x, lbl->y, lbl->param);
;    3900 		}
;    3901 		if(reverse & SW_REVERSE)
;    3902 			LCD_ReverseRect(lbl->x, lbl->y, len * 3, 32);
;    3903 		return;
;    3904 	}
;    3905 	*/
;    3906 	if(lbl->type == LBL_HZ12) {
_0x3B8:
	CALL SUBOPT_0x11C
	SBIW R30,4
	BRNE _0x3BC
;    3907 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3BD
;    3908 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3BE
;    3909 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 12);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x121
	ADIW R30,12
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3910 			LCD_PrintHz12( lbl->x, lbl->y, lbl->param);
_0x3BE:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_PrintHz12
;    3911 		}
;    3912 		if(reverse & SW_REVERSE)
_0x3BD:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3BF
;    3913 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 12);
	CALL SUBOPT_0x122
	LDI  R30,LOW(12)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3914 		return;
_0x3BF:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3915 	}
;    3916 	if(lbl->type == LBL_HZ16) {
_0x3BC:
	CALL SUBOPT_0x11C
	SBIW R30,5
	BRNE _0x3C0
;    3917 		if(reverse & SW_NORMAL) {
	LDD  R30,Y+1
	ANDI R30,LOW(0x1)
	BREQ _0x3C1
;    3918 			if((reverse & SW_OVERLAP) == 0)
	LDD  R30,Y+1
	ANDI R30,LOW(0x4)
	BRNE _0x3C2
;    3919 				LCD_ClsBlock( lbl->x, lbl->y, lbl->x + (len<<4), lbl->y + 16);
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x121
	ADIW R30,16
	ST   -Y,R30
	CALL _LCD_ClsBlock
;    3920 			LCD_PrintHz16( lbl->x, lbl->y, lbl->param);
_0x3C2:
	CALL SUBOPT_0x11D
	CALL SUBOPT_0x11F
	CALL _LCD_PrintHz16
;    3921 		}
;    3922 		if(reverse & SW_REVERSE)
_0x3C1:
	LDD  R30,Y+1
	ANDI R30,LOW(0x2)
	BREQ _0x3C3
;    3923 			LCD_ReverseRect(lbl->x, lbl->y, len * 2, 16);
	CALL SUBOPT_0x122
	LDI  R30,LOW(16)
	ST   -Y,R30
	CALL _LCD_ReverseRect
;    3924 		return;
_0x3C3:
	LDD  R17,Y+0
	RJMP _0x4A5
;    3925 	}
;    3926 }
_0x3C0:
	LDD  R17,Y+0
_0x4A5:
	ADIW R28,4
	RET
;    3927 
;    3928 

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

;OPTIMIZER ADDED SUBROUTINE, CALLED 16 TIMES, CODE SIZE REDUCTION:27 WORDS
SUBOPT_0x30:
	CALL __LSLW2
	ADD  R26,R30
	ADC  R27,R31
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:5 WORDS
SUBOPT_0x31:
	MOVW R30,R28
	ST   -Y,R31
	ST   -Y,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 42 TIMES, CODE SIZE REDUCTION:161 WORDS
SUBOPT_0x32:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R24,0
	CALL _sprintf
	ADIW R28,4
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:21 WORDS
SUBOPT_0x33:
	__POINTW2MN _sysdata,62
	CALL __EEPROMRDB
	CPI  R30,LOW(0x1)
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 14 TIMES, CODE SIZE REDUCTION:23 WORDS
SUBOPT_0x34:
	CALL __CWD1
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x35:
	LDS  R26,_dlg_cnt
	LDS  R27,_dlg_cnt+1
	LDS  R24,_dlg_cnt+2
	LDS  R25,_dlg_cnt+3
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 11 TIMES, CODE SIZE REDUCTION:17 WORDS
SUBOPT_0x36:
	CLR  R22
	CLR  R23
	CALL __PUTPARD1
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 48 TIMES, CODE SIZE REDUCTION:185 WORDS
SUBOPT_0x37:
	ST   -Y,R31
	ST   -Y,R30
	LDI  R30,LOW(1)
	ST   -Y,R30
	JMP  _draw_label

;OPTIMIZER ADDED SUBROUTINE, CALLED 9 TIMES, CODE SIZE REDUCTION:13 WORDS
SUBOPT_0x38:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _sleepms

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x39:
	ST   -Y,R31
	ST   -Y,R30
	JMP  _wnd_msgbox

;OPTIMIZER ADDED SUBROUTINE, CALLED 5 TIMES, CODE SIZE REDUCTION:29 WORDS
SUBOPT_0x3A:
	__POINTW2MN _sysdata,62
	LDI  R30,LOW(1)
	CALL __EEPROMWRB
	CALL _scanner_set_mode
	LDI  R30,LOW(97)
	RJMP SUBOPT_0x25

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3B:
	CALL _navto1v
	RJMP SUBOPT_0x1D

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3C:
	LDI  R30,LOW(1)
	LDI  R31,HIGH(1)
	MOVW R10,R30
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0x3D:
	LDI  R26,LOW(_sysdata)
	LDI  R27,HIGH(_sysdata)
	__GETD1N 0x0
	CALL __EEPROMWRD
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
	RJMP SUBOPT_0x30

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
	RJMP SUBOPT_0x34

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
	RJMP SUBOPT_0x32

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
	RJMP SUBOPT_0x34

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:3 WORDS
SUBOPT_0xD4:
	__POINTW1FN _0,469
	ST   -Y,R31
	ST   -Y,R30
	LDD  R30,Y+10
	LDD  R31,Y+10+1
	RJMP SUBOPT_0x34

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
	RJMP SUBOPT_0x36

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
	RJMP SUBOPT_0x34

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:6 WORDS
SUBOPT_0xE4:
	MOVW R26,R20
	SBIW R26,1
	LDI  R30,LOW(10)
	LDI  R31,HIGH(10)
	CALL __MULW12U
	SUBI R30,LOW(-_pgprbset_items*2)
	SBCI R31,HIGH(-_pgprbset_items*2)
	RJMP SUBOPT_0x37

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
	RJMP SUBOPT_0x37

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
	RJMP SUBOPT_0x34

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
	RJMP SUBOPT_0x36

;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:4 WORDS
SUBOPT_0x105:
	ST   -Y,R31
	ST   -Y,R30
	__POINTW2MN _rprbdata,192
	MOVW R30,R8
	SBIW R30,1
	RJMP SUBOPT_0x30

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
	RJMP SUBOPT_0x30

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
	RJMP SUBOPT_0x37

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
	RJMP SUBOPT_0x37

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
