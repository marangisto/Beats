EESchema Schematic File Version 4
LIBS:Beats-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 16
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
L Reference_Voltage:LM4040DBZ-10 D1
U 1 1 5C03F10C
P 4950 3300
F 0 "D1" V 4904 3379 50  0000 L CNN
F 1 "LM4040DBZ-10" V 4995 3379 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 4950 3100 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm4040-n.pdf" H 4950 3300 50  0001 C CIN
	1    4950 3300
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR047
U 1 1 5C03F195
P 4950 3550
F 0 "#PWR047" H 4950 3300 50  0001 C CNN
F 1 "GND" H 4955 3377 50  0000 C CNN
F 2 "" H 4950 3550 50  0001 C CNN
F 3 "" H 4950 3550 50  0001 C CNN
	1    4950 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3550 4950 3450
$Comp
L Device:R R24
U 1 1 5C03F1FD
P 4950 2850
F 0 "R24" H 5020 2896 50  0000 L CNN
F 1 "2k2" H 5020 2805 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4880 2850 50  0001 C CNN
F 3 "~" H 4950 2850 50  0001 C CNN
	1    4950 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3000 4950 3050
$Comp
L power:-12V #PWR046
U 1 1 5C03F3AF
P 4950 2550
F 0 "#PWR046" H 4950 2650 50  0001 C CNN
F 1 "-12V" H 4965 2723 50  0000 C CNN
F 2 "" H 4950 2550 50  0001 C CNN
F 3 "" H 4950 2550 50  0001 C CNN
	1    4950 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2550 4950 2700
Wire Wire Line
	5300 3050 4950 3050
Connection ~ 4950 3050
Wire Wire Line
	4950 3050 4950 3150
Text GLabel 5300 3050 2    50   Input ~ 0
VRef-10
$EndSCHEMATC
