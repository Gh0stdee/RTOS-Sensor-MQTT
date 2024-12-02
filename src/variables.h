//local time
//https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 8*3600;   //UTC +- n -> 3600*n
const int   daylightOffset_sec = 0; //No datelight saving

//Photoresistor Module
int sensPin = 34;
int darkness;

//timer
int detectGap = 10;
int usFactor = 60000000;