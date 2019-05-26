EESchema Schematic File Version 4
LIBS:Beats-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 16
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR048
U 1 1 5B5C92CF
P 3700 3700
F 0 "#PWR048" H 3700 3450 50  0001 C CNN
F 1 "GND" H 3700 3550 50  0000 C CNN
F 2 "" H 3700 3700 50  0001 C CNN
F 3 "" H 3700 3700 50  0001 C CNN
	1    3700 3700
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR049
U 1 1 5B5C945C
P 5450 3050
F 0 "#PWR049" H 5450 2900 50  0001 C CNN
F 1 "+12V" H 5450 3190 50  0000 C CNN
F 2 "" H 5450 3050 50  0001 C CNN
F 3 "" H 5450 3050 50  0001 C CNN
	1    5450 3050
	1    0    0    -1  
$EndComp
$Comp
L power:-12V #PWR050
U 1 1 5B5C94D1
P 5450 4250
F 0 "#PWR050" H 5450 4350 50  0001 C CNN
F 1 "-12V" H 5450 4400 50  0000 C CNN
F 2 "" H 5450 4250 50  0001 C CNN
F 3 "" H 5450 4250 50  0001 C CNN
	1    5450 4250
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR054
U 1 1 5B5C99D3
P 7650 3200
F 0 "#PWR054" H 7650 3050 50  0001 C CNN
F 1 "+5V" H 7650 3340 50  0000 C CNN
F 2 "" H 7650 3200 50  0001 C CNN
F 3 "" H 7650 3200 50  0001 C CNN
	1    7650 3200
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5B5DAB7A
P 5200 3050
F 0 "#FLG02" H 5200 3125 50  0001 C CNN
F 1 "PWR_FLAG" H 5200 3200 50  0000 C CNN
F 2 "" H 5200 3050 50  0001 C CNN
F 3 "" H 5200 3050 50  0001 C CNN
	1    5200 3050
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5B5DABA7
P 3700 3600
F 0 "#FLG01" H 3700 3675 50  0001 C CNN
F 1 "PWR_FLAG" H 3700 3750 50  0000 C CNN
F 2 "" H 3700 3600 50  0001 C CNN
F 3 "" H 3700 3600 50  0001 C CNN
	1    3700 3600
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 5B5DABCD
P 5200 4250
F 0 "#FLG03" H 5200 4325 50  0001 C CNN
F 1 "PWR_FLAG" H 5200 4400 50  0000 C CNN
F 2 "" H 5200 4250 50  0001 C CNN
F 3 "" H 5200 4250 50  0001 C CNN
	1    5200 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	4400 3450 4650 3300
Wire Wire Line
	4400 3850 4650 4000
Wire Wire Line
	4950 4000 5200 4000
Wire Wire Line
	3700 3650 3900 3650
Wire Wire Line
	5200 3600 5200 3650
Connection ~ 5200 3650
Connection ~ 5200 3300
Connection ~ 5200 4000
Connection ~ 5450 3300
Wire Wire Line
	5450 4000 5450 4250
Wire Wire Line
	6550 3650 6550 3600
Wire Wire Line
	4950 3300 5200 3300
Wire Wire Line
	6050 3600 6050 3650
Connection ~ 6050 3650
Wire Wire Line
	7050 3650 7050 3600
Connection ~ 6550 3650
Wire Wire Line
	7400 3650 7400 3600
Connection ~ 7050 3650
Wire Wire Line
	7650 3300 7650 3200
Wire Wire Line
	5450 3050 5450 3300
Wire Wire Line
	3700 3600 3700 3650
Connection ~ 3700 3650
Wire Wire Line
	5200 4000 5200 4250
Wire Wire Line
	5200 3300 5200 3050
Wire Wire Line
	4400 3650 5200 3650
Wire Wire Line
	5200 3650 5200 3700
Wire Wire Line
	5200 3650 6050 3650
Wire Wire Line
	5200 3300 5450 3300
Wire Wire Line
	5200 4000 5450 4000
Wire Wire Line
	6050 3650 6550 3650
Wire Wire Line
	6550 3650 7050 3650
Wire Wire Line
	7050 3650 7400 3650
Wire Wire Line
	3700 3650 3700 3700
$Comp
L Regulator_Linear:L7805 U6
U 1 1 5B8C4349
P 6550 3300
F 0 "U6" H 6550 3542 50  0000 C CNN
F 1 "L7805" H 6550 3451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6575 3150 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 6550 3250 50  0001 C CNN
	1    6550 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C18
U 1 1 5B8C43CA
P 6050 3450
F 0 "C18" H 6165 3496 50  0000 L CNN
F 1 "100n" H 6165 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6088 3300 50  0001 C CNN
F 3 "~" H 6050 3450 50  0001 C CNN
	1    6050 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C15
U 1 1 5B8C4434
P 7400 3450
F 0 "C15" H 7518 3496 50  0000 L CNN
F 1 "10u" H 7518 3405 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.8" H 7438 3300 50  0001 C CNN
F 3 "~" H 7400 3450 50  0001 C CNN
	1    7400 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C16
U 1 1 5B8C4452
P 5200 3450
F 0 "C16" H 5318 3496 50  0000 L CNN
F 1 "10u" H 5318 3405 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.8" H 5238 3300 50  0001 C CNN
F 3 "~" H 5200 3450 50  0001 C CNN
	1    5200 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C21
U 1 1 5B8C4492
P 7050 4400
F 0 "C21" H 7168 4446 50  0000 L CNN
F 1 "10u" H 7168 4355 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.8" H 7088 4250 50  0001 C CNN
F 3 "~" H 7050 4400 50  0001 C CNN
	1    7050 4400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C19
U 1 1 5B8C44DC
P 7050 3450
F 0 "C19" H 7165 3496 50  0000 L CNN
F 1 "100n" H 7165 3405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7088 3300 50  0001 C CNN
F 3 "~" H 7050 3450 50  0001 C CNN
	1    7050 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:D D2
U 1 1 5B8C4538
P 4800 3300
F 0 "D2" H 4800 3084 50  0000 C CNN
F 1 "DFLR1400-7" H 4800 3175 50  0000 C CNN
F 2 "Diode_SMD:D_PowerDI-123" H 4800 3300 50  0001 C CNN
F 3 "~" H 4800 3300 50  0001 C CNN
	1    4800 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:D D3
U 1 1 5B8C45A3
P 4800 4000
F 0 "D3" H 4800 4216 50  0000 C CNN
F 1 "DFLR1400-7" H 4800 4125 50  0000 C CNN
F 2 "Diode_SMD:D_PowerDI-123" H 4800 4000 50  0001 C CNN
F 3 "~" H 4800 4000 50  0001 C CNN
	1    4800 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J17
U 1 1 5B8C48A5
P 4100 3650
F 0 "J17" H 4150 4067 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 4150 3976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x05_P2.54mm_Vertical" H 4100 3650 50  0001 C CNN
F 3 "~" H 4100 3650 50  0001 C CNN
	1    4100 3650
	1    0    0    1   
$EndComp
$Comp
L power:+5V #PWR055
U 1 1 5B8C93F8
P 8250 3200
F 0 "#PWR055" H 8250 3050 50  0001 C CNN
F 1 "+5V" H 8250 3340 50  0000 C CNN
F 2 "" H 8250 3200 50  0001 C CNN
F 3 "" H 8250 3200 50  0001 C CNN
	1    8250 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 5B8C943A
P 8250 3800
F 0 "D4" V 8288 3683 50  0000 R CNN
F 1 "LED" V 8197 3683 50  0000 R CNN
F 2 "LED_SMD:LED_0603_1608Metric_Castellated" H 8250 3800 50  0001 C CNN
F 3 "~" H 8250 3800 50  0001 C CNN
	1    8250 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R25
U 1 1 5B8CA688
P 8250 3450
F 0 "R25" H 8320 3496 50  0000 L CNN
F 1 "330" H 8320 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 8180 3450 50  0001 C CNN
F 3 "~" H 8250 3450 50  0001 C CNN
	1    8250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 3300 8250 3200
Wire Wire Line
	8250 3600 8250 3650
$Comp
L power:GND #PWR056
U 1 1 5B8CB2C3
P 8250 4100
F 0 "#PWR056" H 8250 3850 50  0001 C CNN
F 1 "GND" H 8255 3927 50  0000 C CNN
F 2 "" H 8250 4100 50  0001 C CNN
F 3 "" H 8250 4100 50  0001 C CNN
	1    8250 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4100 8250 3950
Wire Wire Line
	3900 3850 4400 3850
Wire Wire Line
	3900 3750 4400 3750
Wire Wire Line
	3900 3550 4400 3550
Wire Wire Line
	3900 3450 4400 3450
Wire Wire Line
	3900 3550 3900 3650
Connection ~ 3900 3650
Wire Wire Line
	3900 3650 3900 3750
Wire Wire Line
	4400 3750 4400 3650
Connection ~ 4400 3650
Wire Wire Line
	4400 3550 4400 3650
Wire Wire Line
	5450 3300 5750 3300
Wire Wire Line
	6850 3300 7050 3300
$Comp
L power:+3.3V #PWR053
U 1 1 5D1A7951
P 7600 4100
F 0 "#PWR053" H 7600 3950 50  0001 C CNN
F 1 "+3.3V" H 7615 4273 50  0000 C CNN
F 2 "" H 7600 4100 50  0001 C CNN
F 3 "" H 7600 4100 50  0001 C CNN
	1    7600 4100
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3VA #PWR052
U 1 1 5D1A7957
P 7300 4100
F 0 "#PWR052" H 7300 3950 50  0001 C CNN
F 1 "+3.3VA" H 7315 4273 50  0000 C CNN
F 2 "" H 7300 4100 50  0001 C CNN
F 3 "" H 7300 4100 50  0001 C CNN
	1    7300 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7300 4100 7300 4200
Wire Wire Line
	7050 4200 7050 4250
Wire Wire Line
	7600 4200 7600 4100
Wire Wire Line
	6850 4200 7050 4200
Connection ~ 7300 4200
$Comp
L power:GND #PWR051
U 1 1 5D1A7963
P 6550 4700
F 0 "#PWR051" H 6550 4450 50  0001 C CNN
F 1 "GND" H 6555 4527 50  0000 C CNN
F 2 "" H 6550 4700 50  0001 C CNN
F 3 "" H 6550 4700 50  0001 C CNN
	1    6550 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 4700 6550 4600
Connection ~ 7050 4200
Wire Wire Line
	7050 4200 7300 4200
$Comp
L Device:C C17
U 1 1 5D1A796C
P 6000 4400
F 0 "C17" H 6115 4446 50  0000 L CNN
F 1 "100n" H 6115 4355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6038 4250 50  0001 C CNN
F 3 "~" H 6000 4400 50  0001 C CNN
	1    6000 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4250 6000 4200
Wire Wire Line
	6000 4200 6250 4200
Wire Wire Line
	6000 4550 6000 4600
Connection ~ 6550 4600
Wire Wire Line
	6550 4600 6550 4500
Wire Wire Line
	6550 4600 7050 4600
Wire Wire Line
	7050 4600 7050 4550
Wire Wire Line
	6000 4600 6550 4600
Wire Wire Line
	7300 4200 7600 4200
$Comp
L Regulator_Linear:LM1117-3.3 U7
U 1 1 5D1A798F
P 6550 4200
F 0 "U7" H 6550 4442 50  0000 C CNN
F 1 "LM1117-3.3" H 6550 4351 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 6550 4200 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm1117.pdf" H 6550 4200 50  0001 C CNN
	1    6550 4200
	1    0    0    -1  
$EndComp
Connection ~ 6050 3300
Wire Wire Line
	6050 3300 6250 3300
Connection ~ 7050 3300
Wire Wire Line
	7050 3300 7400 3300
Connection ~ 7400 3300
Wire Wire Line
	7400 3300 7650 3300
Wire Wire Line
	6000 4200 5750 4200
Wire Wire Line
	5750 4200 5750 3300
Connection ~ 6000 4200
Connection ~ 5750 3300
Wire Wire Line
	5750 3300 6050 3300
$Comp
L Device:CP C20
U 1 1 5D1A794B
P 5200 3850
F 0 "C20" H 5318 3896 50  0000 L CNN
F 1 "10u" H 5318 3805 50  0000 L CNN
F 2 "Capacitor_SMD:CP_Elec_4x5.8" H 5238 3700 50  0001 C CNN
F 3 "~" H 5200 3850 50  0001 C CNN
	1    5200 3850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
