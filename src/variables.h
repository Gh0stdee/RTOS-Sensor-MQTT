//local time
//https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 8*3600;   //UTC +- n -> 3600*n
const int   daylightOffset_sec = 0; //No datelight saving

//photoresistor module
int light;

//deep sleep
bool espState = false;    //set ESP to be Off for default
int darkness;
bool voltageWake;         //Touch pin voltage 
int voltageWake_count;

//timer
int detectGap = 10; //minutes
int usFactor = 60000000; //us to minutes