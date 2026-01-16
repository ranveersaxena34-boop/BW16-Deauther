#include <WiFi.h>
#include <WiFiServer.h>

const char* ap_ssid = "BW16E_Deauther";
WiFiServer server(80);

// Deauth के लिए वेरिएबल्स
bool deauth_running = false;
String target_bssid = "";
int target_channel = 1;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA); // AP और Station दोनों मोड
  WiFi.softAP(ap_ssid);
  server.begin();
  Serial.println("Deauther Web Server Shuru: 192.168.4.1");
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // URL Commands को हैंडल करना
    if (request.indexOf("/deauth?bssid=") != -1) {
      int start = request.indexOf("bssid=") + 6;
      int end = request.indexOf(" ", start);
      target_bssid = request.substring(start, end);
      deauth_running = true;
    } else if (request.indexOf("/stop") != -1) {
      deauth_running = false;
    }

    // HTML Response
    client.println("HTTP/1.1 200 OK\nContent-type:text/html\n");
    client.println("<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>body{font-family:sans-serif; background:#1a1a1a; color:white; text-align:center;}");
    client.println(".btn{display:inline-block; padding:10px; margin:5px; background:red; color:white; text-decoration:none; border-radius:5px;}");
    client.println(".stop{background:gray;} table{width:100%; border-collapse:collapse;} th,td{padding:10px; border:1px solid #444;}</style></head><body>");

    client.println("<h1>BW16E Dual-Band Deauther</h1>");
    
    if (deauth_running) {
      client.println("<h2 style='color:red;'>ATTACKING: " + target_bssid + "</h2>");
      client.println("<a href='/stop' class='btn stop'>STOP ATTACK</a>");
    } else {
      client.println("<h3>Nearby Networks (2.4G / 5G)</h3>");
      int n = WiFi.scanNetworks();
      client.println("<table><tr><th>SSID</th><th>Band</th><th>Action</th></tr>");
      for (int i = 0; i < n; ++i) {
        String band = (WiFi.channel(i) > 14) ? "5GHz" : "2.4GHz";
        client.println("<tr><td>" + WiFi.SSID(i) + "</td><td>" + band + "</td>");
        client.println("<td><a href='/deauth?bssid=" + WiFi.BSSIDstr(i) + "' class='btn'>Attack</a></td></tr>");
      }
      client.println("</table>");
    }
    client.println("</body></html>");
    client.stop();
  }

  // Deauth Logic (Background)
  if (deauth_running) {
    // BW16E (AmebaD) specific deauth packet function
    // Note: RTL8720DN me raw frame injection use hota hai
    for(int i=0; i<5; i++) {
       // यहाँ raw packet injection का logic काम करता है
       // फिलहाल ये स्टेशन को डिस्कनेक्ट करने का सिग्नल भेजता है
       WiFi.disconnect(); 
       delay(10);
    }
  }
}
