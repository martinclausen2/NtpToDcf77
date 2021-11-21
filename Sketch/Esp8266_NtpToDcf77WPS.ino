/*

 Emulator DCF77
 Simulate a DCF77 radio receiver with a ESP8266, esp01 model
 Emits a complete three minute pulses train from the GPIO2 output
 the train is preceded by a single pulse at the lacking 59° pulse to allow some clock model synchronization
 of the beginning frame
 after the three pulses train one more single pulse is sent to safely close the frame
 get the time from the ntp service

 Uses Time library to facilitate time computation

 
 Known issue:
 -When the DayLightSaving mode change, the three minutes packet is not checked for possible changes of hour across the frame
  itself, moreover the daylight saving is changed normally at 3 o clock in the morning (here in italy), while I don't correct for it
  so the time will be probably incorrect before at least 03:03 then dayLightSaving changes
 -the exact "second" precision is not guaranteed because of the semplicity of the NTP imnplementation
  normally the packet transit delay would be taken into account, but here is not

 Fuso68 05/12/2015

 19/12/2015 added disconnect and reconnect if wlan fail, reconnect also after three failed ntp requests
 20/12/2015 increased wlan connect timeout to 30 seconds

 16/01/2021 added auto light sleep, fetch time only every hour
 17/01/2021 added WPS button on pin 0, removed WLAN disconnects to keep values obtained via WPS
 
   
 Based upon:
 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov
 modified
 by Martin Clausen

 This code is in the public domain.

 */

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Ticker.h>
#include <TimeLib.h>

// Auto Light Sleep https://blog.creations.de/?p=149
extern "C" {  
  #include "user_interface.h"
}

WiFiUDP ntpUDP;
// more generic NTP server "europe.pool.ntp.org"
// time offset 3600 => 1 hour => Central European Time
// refresh time frequency 600000 => 10 minutes
NTPClient timeClient(ntpUDP, "fritz.box", 3600, 600000);

// sleep duration between blocks of active transmission of time data to DCF77 receiver
#define sleepcount 48

//routine timer 100 msec
Ticker DcfOutTimer;

// define pinout
#define LedPin 2
#define WPS 0

//how many total pulses we have
//three complete minutes + 2 head pulses and one tail pulse
#define MaxPulseNumber 183
#define FirstMinutePulseBegin 2
#define SecondMinutePulseBegin 62
#define ThirdMinutePulseBegin 122

//complete array of pulses for three minutes
//0 = no pulse, 1=100msec, 2=200msec
int PulseArray[MaxPulseNumber];

int PulseCount = 0;
int DCFOutputOn = 0;
int PartialPulseCount = 0;
int ThisHour,ThisMinute,ThisSecond,ThisDay,ThisMonth,ThisYear,DayOfW;

int Dls;                    //DayLightSaving

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("INIT DCF77 emulator");
 
  pinMode(LedPin, OUTPUT);
  digitalWrite(LedPin, LOW);
  
  //handle DCF pulses
  DcfOutTimer.attach_ms(100, DcfOut);

  //first 2 pulses: 1 + blank to simulate the packet beginning
  //il primo bit e' un 1
  PulseArray[0] = 1;
  //missing pulse indicates start of minute
  PulseArray[1] = 0;

  //last pulse after the third 59° blank
  PulseArray[MaxPulseNumber - 1] = 1;
  
  PulseCount = 0;
  DCFOutputOn = 0;    //we begin with the output OFF

  wifi_set_sleep_type(LIGHT_SLEEP_T);
  
  //first connect to wifi network
  //NOTE testing WiFi.status() BEFThisHour the FIRST WiFi.begin() seems to hang the system
  //so we attempt a first connection BEFORE the main loop
  
  Serial.printf("\nVersuche Verbindung mit gespeicherter SSID '%s'\n", WiFi.SSID().c_str());
  pinMode(WPS, INPUT_PULLUP); //Taster Eingang aktivieren
  ConnectToWiFi();
  CheckConnection();
  // wait 10 seconds for clock to come up
  delay(10000);
  timeClient.begin();
}

void loop(){
  int n;
  //check the WLAN status
  if (WiFi.status() == WL_CONNECTED)
  {
    // word clock reads every hour. so we present a stretch of ~75 minutes signal in every activity block
    for (n=0;n<25;n++) {
      ReadAndDecodeTime();
      delay(6000);
      if (WiFi.status() != WL_CONNECTED)
        break;
    };
    WiFi.mode( WIFI_OFF );
    WiFi.forceSleepBegin();
    delay(1);
    Serial.println("Beginne Ruhezustand.");
    // wait between blocks of active transmission
    for (n=0;n<sleepcount;n++) {
      Serial.printf("Aufwachen in %d Minuten.\n", (sleepcount - n) * 10);
      delay(600000);   //10 Minuten
    }
    Serial.println("Ende Ruhezustand.");
    Serial.printf("\nVersuche Verbindung mit gespeicherter SSID '%s'\n", WiFi.SSID().c_str());
    WiFi.forceSleepWake();
    delay(1);
    WiFi.mode( WIFI_STA );
    WiFi.begin();
    AwaitConnection();
    Serial.println();
  }
  else
  {
    // wait 1 Minute
    delay(60000);
    ReconnectToWiFi();
  }
}

void ConnectToWiFi(){
  WiFi.mode(WIFI_STA);
  String ssid = WiFi.SSID();
  if (ssid.length() > 0)
  {
    Serial.print("Connecting to ");
    Serial.println(WiFi.SSID().c_str());
    WiFi.begin(); // letzte gespeicherte Zugangsdaten
    if (!AwaitConnection())
      return;
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.print("Cannot connect, no SSID yet set.");
  }
}

void ReconnectToWiFi(){
  Serial.println("\nReconnecting to WLAN");
  WiFi.reconnect();
  AwaitConnection();
}

bool AwaitConnection(){
  int Timeout = 60;
  int timeoutCounter = 0;
  bool success = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (timeoutCounter++ > Timeout){
      Serial.println("\nImpossible to connect to WLAN!");
      success = false;
      break;
      }
    };
  return success;
}

void CheckConnection(){
  wl_status_t status = WiFi.status();
  if(status == WL_CONNECTED) {
    Serial.printf("\nErfolgreich angemeldet an SSID '%s'\n", WiFi.SSID().c_str());
  } else {
    //Wir waren nicht erfolgreich starten daher WPS
    while (WiFi.status() != WL_CONNECTED) {
      Serial.printf("\nKann keine WiFi Verbindung herstellen. Status ='%d'\n", status); 
      Serial.println("WPS Taste am Router drücken.\n Dann WPS Taste am ESP drücken!");
      while (digitalRead(WPS)!=0) {yield();}   
      if(!startWPS())
        Serial.println("Keine Verbindung über WPS herstellbar");  
    }
  } 
}

//Startet die WPS Konfiguration
bool startWPS() {
  Serial.println("WPS Konfiguration gestartet");
  bool wpsSuccess = WiFi.beginWPSConfig();
  if(wpsSuccess) {
      // Muss nicht immer erfolgreich heißen! Nach einem Timeout ist die SSID leer
      String newSSID = WiFi.SSID();
      if(newSSID.length() > 0) {
        // Nur wenn eine SSID gefunden wurde waren wir erfolgreich 
        Serial.printf("WPS fertig. Erfolgreich angemeldet an SSID '%s'\n", newSSID.c_str());
        wpsSuccess = AwaitConnection();
      } else {
        wpsSuccess = false;
      }
  }
  return wpsSuccess; 
}

void ReadAndDecodeTime() {
    int DayToEndOfMonth,DayOfWeekToEnd,DayOfWeekToSunday;

    //get a random server from the pool
    timeClient.update();

    // avoid sending the wrong time
    if(timeClient.isTimeSet()) {
     
      // now convert NTP time into everyday time:
      Serial.print("Unix time = ");
      //note: we add two minutes because the dcf protocol send the time of the FOLLOWING minute
      //and our transmission begins the next minute more
      time_t ThisTime = timeClient.getEpochTime() + 120;
      // print Unix time:
      Serial.println(ThisTime);
  
      //calculate actual day to evaluate the summer/winter time of day ligh saving
      DayOfW = weekday(ThisTime);
      ThisDay = day(ThisTime);
      ThisMonth = month(ThisTime);
      ThisYear = year(ThisTime);
      Serial.print("Local time ");       // UTC is the time at Greenwich Meridian (GMT)
      Serial.print(ThisDay);
      Serial.print('/');
      Serial.print(ThisMonth);
      Serial.print('/');
      Serial.print(ThisYear);
      Serial.print(' ');
    
      //check daylight saving
      Dls = 0;    //default winter time
      //From April to september we are surely on summer time
      if (ThisMonth > 3 && ThisMonth < 10) {
        Dls = 1;
      };
      //March, month of change winter->summer time, last Sunday of the month
      //March has 31 days so from 25 included on Sunday we can be in summer time
      if (ThisMonth == 3 && ThisDay > 24) {
        DayToEndOfMonth = 31 - ThisDay;
        DayOfWeekToSunday = 7 - DayOfW;
        if (DayOfWeekToSunday >= DayToEndOfMonth)
          Dls = 1;
      };
      //October, month of change summer->winter time, last Sunday of the month
      //October has 31 days so from 25 included on Sunday we can be in winter time
      if (ThisMonth == 10) {
        Dls = 1;
        if (ThisDay > 24) {
          DayToEndOfMonth = 31 - ThisDay;
          DayOfWeekToEnd = 7 - DayOfW;
          if (DayOfWeekToEnd >= DayToEndOfMonth)
          Dls = 0;
        };
      };
  
      Serial.print("Dls:");
      Serial.print(Dls);
      Serial.print(' ');
      //add one hour if we are in summer time
      if (Dls == 1)
        ThisTime += 3600;
  
      //now that we know the dls state, we can calculate the time to
      // print the hour, minutes and seconds:
      ThisHour = hour(ThisTime);
      ThisMinute = minute(ThisTime);
      ThisSecond = second(ThisTime);
      Serial.print(ThisHour); // print the hour
      Serial.print(':');
      Serial.print(ThisMinute); // print the minute
      Serial.print(':');
      Serial.println(ThisSecond); // print the second
  
      //if we are over about the 56° second we risk to begin the pulses too late, so it's better
      //to skip at the half of the next minute and NTP+recalculate all again
      if (ThisSecond > 56){
        delay(30000);
        return;      
      }
  
      //calculate bis array for the first minute
      CalculateArray(FirstMinutePulseBegin);
  
      //add one minute and calculate array again for the second minute
      ThisTime += 60;
      DayOfW = weekday(ThisTime);
      ThisDay = day(ThisTime);
      ThisMonth = month(ThisTime);
      ThisYear = year(ThisTime);
      ThisHour = hour(ThisTime);
      ThisMinute = minute(ThisTime);
      ThisSecond = second(ThisTime);
      CalculateArray(SecondMinutePulseBegin);
  
      //one minute more for the third minute
      ThisTime += 60;
      DayOfW = weekday(ThisTime);
      ThisDay = day(ThisTime);
      ThisMonth = month(ThisTime);
      ThisYear = year(ThisTime);
      ThisHour = hour(ThisTime);
      ThisMinute = minute(ThisTime);
      ThisSecond = second(ThisTime);
      CalculateArray(ThirdMinutePulseBegin);
  
     
      //how many to the minute end ?
      //don't forget that we begin transmission at second 58°
      int SkipSeconds = 58 - ThisSecond;
      delay(SkipSeconds * 1000);
      //begin
      DCFOutputOn = 1;
  
      //three minutes are needed to transmit all the packet
      //then wait more 30 secs to locate safely at the half of minute
      //NB 150+60=210sec, 60secs are lost from main routine
      delay(150000);
    }
    else
    {
      Serial.println("No time received via NTP. Sending no DCF signal.");
    }
}


void CalculateArray(int ArrayOffset) {
  int n,Tmp,TmpIn;
  int ParityCount = 0;

  //first 20 bits are logical 0s
  for (n=0;n<20;n++)
    PulseArray[n+ArrayOffset] = 1;

  //DayLightSaving bit
  if (Dls == 1)
    PulseArray[17+ArrayOffset] = 2;
  else
    PulseArray[18+ArrayOffset] = 2;
    
  //bit 20 must be 1 to indicate time active
  PulseArray[20+ArrayOffset] = 2;

  //calculate minutes bits
  TmpIn = Bin2Bcd(ThisMinute);
  for (n=21;n<28;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  };
  if ((ParityCount & 1) == 0)
    PulseArray[28+ArrayOffset] = 1;
  else
    PulseArray[28+ArrayOffset] = 2;

  //calculate hour bits
  ParityCount = 0;
  TmpIn = Bin2Bcd(ThisHour);
  for (n=29;n<35;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  }
  if ((ParityCount & 1) == 0)
    PulseArray[35+ArrayOffset] = 1;
  else
    PulseArray[35+ArrayOffset] = 2;
   ParityCount = 0;
  //calculate day bits
  TmpIn = Bin2Bcd(ThisDay);
  for (n=36;n<42;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  }
  //calculate weekday bits
  TmpIn = Bin2Bcd(DayOfW);
  for (n=42;n<45;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  }
  //calculate month bits
  TmpIn = Bin2Bcd(ThisMonth);
  for (n=45;n<50;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  }
  //calculate year bits
  TmpIn = Bin2Bcd(ThisYear - 2000);   //a noi interesa solo l'anno con ... il millenniumbug !
  for (n=50;n<58;n++) {
    Tmp = TmpIn & 1;
    PulseArray[n+ArrayOffset] = Tmp + 1;
    ParityCount += Tmp;
    TmpIn >>= 1;
  }
  //date parity
  if ((ParityCount & 1) == 0)
    PulseArray[58+ArrayOffset] = 1;
  else
    PulseArray[58+ArrayOffset] = 2;

  //last missing pulse
  PulseArray[59+ArrayOffset] = 0;

  /* for debug: print the whole 180 secs array
   * Serial.print(':');
  for (n=0;n<60;n++)
    Serial.print(PulseArray[n+ArrayOffset]);*/

}

//called every 100msec
//for DCF77 output
void DcfOut() {

  if (DCFOutputOn == 1) {
    switch (PartialPulseCount++) {
      case 0:
        if (PulseArray[PulseCount] != 0)
          digitalWrite(LedPin, 0);
        break;
      case 1:
        if (PulseArray[PulseCount] == 1)
          digitalWrite(LedPin, 1);
        break;
      case 2:
        digitalWrite(LedPin, 1);
        break;
      case 9:
        if (PulseCount++ == (MaxPulseNumber -1 )){     //one less because we FIRST tx the pulse THEN count it
          PulseCount = 0;
          DCFOutputOn = 0;
        };
        PartialPulseCount = 0;
        break;
    };
  };

}

int Bin2Bcd(int dato) {
  int msb,lsb;

  if (dato < 10)
    return dato;
  msb = (dato / 10) << 4;
  lsb = dato % 10; 
  return msb + lsb;
}
