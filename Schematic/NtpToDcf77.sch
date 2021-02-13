EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Regulator_Linear:LF33_TO220 U1
U 1 1 602827EC
P 3200 2050
F 0 "U1" H 3200 2292 50  0000 C CNN
F 1 "LF33_TO220" H 3200 2201 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3200 2275 50  0001 C CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/c4/0e/7e/2a/be/bc/4c/bd/CD00000546.pdf/files/CD00000546.pdf/jcr:content/translations/en.CD00000546.pdf" H 3200 2000 50  0001 C CNN
	1    3200 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C4
U 1 1 60285ED9
P 4250 2300
F 0 "C4" H 4368 2346 50  0000 L CNN
F 1 "470µF / 6,3V" H 4368 2255 50  0000 L CNN
F 2 "" H 4288 2150 50  0001 C CNN
F 3 "~" H 4250 2300 50  0001 C CNN
	1    4250 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C5
U 1 1 602861ED
P 5050 2300
F 0 "C5" H 5168 2346 50  0000 L CNN
F 1 "470µF / 25V" H 5168 2255 50  0000 L CNN
F 2 "" H 5088 2150 50  0001 C CNN
F 3 "~" H 5050 2300 50  0001 C CNN
	1    5050 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 6028638A
P 1900 2300
F 0 "C1" H 2018 2346 50  0000 L CNN
F 1 "47µF / 25V" H 2018 2255 50  0000 L CNN
F 2 "" H 1938 2150 50  0001 C CNN
F 3 "~" H 1900 2300 50  0001 C CNN
	1    1900 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 602868D7
P 2450 2300
F 0 "C2" H 2565 2346 50  0000 L CNN
F 1 "100nF" H 2565 2255 50  0000 L CNN
F 2 "" H 2488 2150 50  0001 C CNN
F 3 "~" H 2450 2300 50  0001 C CNN
	1    2450 2300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 60286E66
P 3750 2300
F 0 "C3" H 3865 2346 50  0000 L CNN
F 1 "100nF" H 3865 2255 50  0000 L CNN
F 2 "" H 3788 2150 50  0001 C CNN
F 3 "~" H 3750 2300 50  0001 C CNN
	1    3750 2300
	1    0    0    -1  
$EndComp
$Comp
L power:+15V #PWR01
U 1 1 6028804E
P 1900 1800
F 0 "#PWR01" H 1900 1650 50  0001 C CNN
F 1 "+15V" H 1915 1973 50  0000 C CNN
F 2 "" H 1900 1800 50  0001 C CNN
F 3 "" H 1900 1800 50  0001 C CNN
	1    1900 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 602886A5
P 1900 2650
F 0 "#PWR02" H 1900 2400 50  0001 C CNN
F 1 "GND" H 1905 2477 50  0000 C CNN
F 2 "" H 1900 2650 50  0001 C CNN
F 3 "" H 1900 2650 50  0001 C CNN
	1    1900 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6050 2350 5750 2350
Wire Wire Line
	5750 2350 5750 2250
Wire Wire Line
	5750 2050 5050 2050
Wire Wire Line
	3750 2150 3750 2050
Connection ~ 3750 2050
Wire Wire Line
	3750 2050 3500 2050
Wire Wire Line
	4250 2150 4250 2050
Connection ~ 4250 2050
Wire Wire Line
	4250 2050 3750 2050
Wire Wire Line
	5050 2150 5050 2050
Connection ~ 5050 2050
Wire Wire Line
	5050 2050 4250 2050
Wire Wire Line
	1900 2650 1900 2550
Wire Wire Line
	5050 2450 5050 2550
Wire Wire Line
	5050 2550 4250 2550
Connection ~ 1900 2550
Wire Wire Line
	1900 2550 1900 2450
Wire Wire Line
	1900 2150 1900 2050
Wire Wire Line
	2900 2050 2450 2050
Connection ~ 1900 2050
Wire Wire Line
	1900 2050 1900 1800
Wire Wire Line
	2450 2150 2450 2050
Connection ~ 2450 2050
Wire Wire Line
	2450 2050 1900 2050
Wire Wire Line
	2450 2450 2450 2550
Connection ~ 2450 2550
Wire Wire Line
	2450 2550 1900 2550
Wire Wire Line
	3200 2350 3200 2550
Connection ~ 3200 2550
Wire Wire Line
	3200 2550 2450 2550
Wire Wire Line
	3750 2450 3750 2550
Connection ~ 3750 2550
Wire Wire Line
	3750 2550 3200 2550
Wire Wire Line
	4250 2450 4250 2550
Connection ~ 4250 2550
Wire Wire Line
	4250 2550 3750 2550
Wire Wire Line
	7950 2050 8550 2050
Wire Wire Line
	8550 2050 8550 3250
$Comp
L power:GND #PWR04
U 1 1 6028B35F
P 8550 3250
F 0 "#PWR04" H 8550 3000 50  0001 C CNN
F 1 "GND" H 8555 3077 50  0000 C CNN
F 2 "" H 8550 3250 50  0001 C CNN
F 3 "" H 8550 3250 50  0001 C CNN
	1    8550 3250
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 6028BC71
P 8200 2950
F 0 "SW1" V 8154 3098 50  0000 L CNN
F 1 "WPS" V 8245 3098 50  0000 L CNN
F 2 "" H 8200 3150 50  0001 C CNN
F 3 "~" H 8200 3150 50  0001 C CNN
	1    8200 2950
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 6028C304
P 8200 2500
F 0 "R2" H 8270 2546 50  0000 L CNN
F 1 "1k" H 8270 2455 50  0000 L CNN
F 2 "" V 8130 2500 50  0001 C CNN
F 3 "~" H 8200 2500 50  0001 C CNN
	1    8200 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 6028D4E9
P 8200 3250
F 0 "#PWR03" H 8200 3000 50  0001 C CNN
F 1 "GND" H 8205 3077 50  0000 C CNN
F 2 "" H 8200 3250 50  0001 C CNN
F 3 "" H 8200 3250 50  0001 C CNN
	1    8200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2250 8200 2250
Wire Wire Line
	8200 2250 8200 2350
Wire Wire Line
	8200 2650 8200 2750
Wire Wire Line
	8200 3150 8200 3250
Wire Wire Line
	6050 2250 5750 2250
Connection ~ 5750 2250
Wire Wire Line
	5750 2250 5750 2150
Wire Wire Line
	6050 2150 5750 2150
Connection ~ 5750 2150
Wire Wire Line
	5750 2150 5750 2050
Wire Wire Line
	7950 2150 8800 2150
$Comp
L Device:R R1
U 1 1 60291747
P 8200 1800
F 0 "R1" H 8270 1846 50  0000 L CNN
F 1 "10k" H 8270 1755 50  0000 L CNN
F 2 "" V 8130 1800 50  0001 C CNN
F 3 "~" H 8200 1800 50  0001 C CNN
	1    8200 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 1650 8200 1550
Wire Wire Line
	8200 1550 5750 1550
Wire Wire Line
	5750 1550 5750 2050
Connection ~ 5750 2050
Wire Wire Line
	8200 1950 8200 2250
Connection ~ 8200 2250
$Comp
L Connector_Generic:Conn_01x01 J1
U 1 1 60292E54
P 9450 2150
F 0 "J1" H 9530 2192 50  0000 L CNN
F 1 "DCF77 Signal" H 9530 2101 50  0000 L CNN
F 2 "" H 9450 2150 50  0001 C CNN
F 3 "~" H 9450 2150 50  0001 C CNN
	1    9450 2150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 60297385
P 8950 2150
F 0 "R3" V 8743 2150 50  0000 C CNN
F 1 "1k" V 8834 2150 50  0000 C CNN
F 2 "" V 8880 2150 50  0001 C CNN
F 3 "~" H 8950 2150 50  0001 C CNN
	1    8950 2150
	0    1    1    0   
$EndComp
Wire Wire Line
	9100 2150 9250 2150
$Comp
L ESP-01:ESP-01v090 U?
U 1 1 60284412
P 7000 2200
F 0 "U?" H 7000 2715 50  0000 C CNN
F 1 "ESP-01v090" H 7000 2624 50  0000 C CNN
F 2 "" H 7000 2200 50  0001 C CNN
F 3 "http://l0l.org.uk/2014/12/esp8266-modules-hardware-guide-gotta-catch-em-all/" H 7000 2200 50  0001 C CNN
	1    7000 2200
	1    0    0    -1  
$EndComp
$EndSCHEMATC
