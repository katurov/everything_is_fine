#include <Arduino.h>
#include <WiFi.h>

#include <ser.h>

#define PINGHOSTS 	2
#define WEBSETIES	2

const char* ssid = "<NETWORK NAME>";
const char* password =  "<PASSWORD>";

/*
This scruct for hosts you want to ping. NB: you should use IP address
*/
struct HostToPing {
  const char* name;         // Name to show on rectangle, e.g. "Google"
  IPAddress host_ip;        // IP address, e.g. 8.8.8.8 to ping Google
  unsigned int avg_delay;   // How many ms is "normal" for your ping, e.g. 25
  unsigned int position;    // Position of this rectangle on screen
};

/*
This struct for web-sites you want to ping. Use pases with HTTP 200 responce
*/
struct SiteToCheck {
	const char* name;         // Name to show on rectangle, e.g. "Google"
	const char* host;         // Hostname, e.g. google.com
	const char* get_string;   // What to use as HTTP request
	const char* hst_string;   // What to use as hostname
	const char* crt;          // SSL certificate to verify host
	unsigned int position;    // Position of this rectangle on screen
};

/*
  Here are examples 
*/
struct HostToPing hosts[PINGHOSTS] = {	
	"Gateway", IPAddress(192, 168, 107, 1), 25, 1, // this will ping local gateway
	"Google", IPAddress(8, 8, 8, 8), 50, 2         // this will ping Google
};

/*
  Certs (and how to make 'em see in ser.h
*/
struct SiteToCheck websites[WEBSETIES] = {
	"Yandex", "yandex.ru", "GET https://yandex.ru/  HTTP/1.0", "Host: yandex.ru", crtR3, 3,
	"Google", "google.com", "GET https://google.com/ HTTP/1.0", "Host: google.com", crtSectigo, 4
};
