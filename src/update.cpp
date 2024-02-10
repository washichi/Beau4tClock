#include "update.h"
#include "web.h"
#include "globals.h"
#include "secrets.h"
// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

const char IRG_Root_X1[] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIFbTCCBFWgAwIBAgIRAKiEsP8oq4C3ET9LGW9FeX0wDQYJKoZIhvcNAQELBQAw
RjELMAkGA1UEBhMCVVMxIjAgBgNVBAoTGUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBM
TEMxEzARBgNVBAMTCkdUUyBDQSAxUDUwHhcNMjMxMjEzMTcwODE5WhcNMjQwMzEy
MTcwODE4WjAXMRUwEwYDVQQDEwxob21lc3BvdC5kZXYwggEiMA0GCSqGSIb3DQEB
AQUAA4IBDwAwggEKAoIBAQDygsiSbrqeVuKUHCxNL0LgoQUQimDqBEuxNOKd77Ov
CjKo+SP0I37lgNFyD7DrW/9KSCOuHLXuOvS6cqAoATRDIUkNcJLXA2znSlAGVclW
oq+DYX+5UFIgBOtUOPH6ErW/zdMFnEFU/ocJ6hV0ZINHaw4LZAlBL/l2mL94mrDL
PsUoNwGOulti+q5RgvTOizBCdpsd6hZMRD95ku8RaiPUNhp1msiqRdUJ4Mq856Qh
bAwtjuIn1eXrfSVvEXfRuC8DYc9IWJUOyxWU/vZsryiA4pkJRY8pg2Sny20sMVfH
AHSYvLQbY6Pc8sKd8f8MyaUJF6vydpRDSCDau02PeukxAgMBAAGjggKDMIICfzAO
BgNVHQ8BAf8EBAMCBaAwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDAYDVR0TAQH/BAIw
ADAdBgNVHQ4EFgQUCDIirOjjPkd3nlU5M29QRl7xJT0wHwYDVR0jBBgwFoAU1fye
Dd8eyt0Il5duK8VfxSv17LgweAYIKwYBBQUHAQEEbDBqMDUGCCsGAQUFBzABhilo
dHRwOi8vb2NzcC5wa2kuZ29vZy9zL2d0czFwNS9HQ0RfVzBNOEs1dzAxBggrBgEF
BQcwAoYlaHR0cDovL3BraS5nb29nL3JlcG8vY2VydHMvZ3RzMXA1LmRlcjAnBgNV
HREEIDAeggxob21lc3BvdC5kZXaCDiouaG9tZXNwb3QuZGV2MCEGA1UdIAQaMBgw
CAYGZ4EMAQIBMAwGCisGAQQB1nkCBQMwPAYDVR0fBDUwMzAxoC+gLYYraHR0cDov
L2NybHMucGtpLmdvb2cvZ3RzMXA1L08zM2J0cFN6clljLmNybDCCAQQGCisGAQQB
1nkCBAIEgfUEgfIA8AB2AEiw42vapkc0D+VqAvqdMOscUgHLVt0sgdm7v6s52IRz
AAABjGRdiFoAAAQDAEcwRQIhAPr6aGxMT+F5hiQ2b2dW49jJSjhuXjeffuXPqxVL
Hv/FAiBgJ4+gsUSgQYIJ2NP0DHCijK3O013/Uul7CYd2tRZ4tAB2AO7N0GTV2xrO
xVy3nbTNE6Iyh0Z8vOzew1FIWUZxH7WbAAABjGRdiDYAAAQDAEcwRQIhAPjSHAk1
MIgR1b95t47frhm0zCB23ygzGd+wOfi3K+4SAiAkt1VssB2dcMfnKgQgjNDNf9Mm
FUe8UNt762MYqnVhATANBgkqhkiG9w0BAQsFAAOCAQEAp7o6Sosk5ydwnZbuAsIT
HNWFMXYDzrZo17s5xZkVaKGqsRw9LOje99HWernV+ZCSznEoTCwH7BT2mDvX87VE
1VsqdabK6Q7sCaNXi9HnkYnHspyMghD/7HEm6qKUzwTyBB4BEsO4XOG0A6aDeYGy
ROTRD62IK1rWX8NzzSjTgArsT4SfBf36ijVQqgZYEaCZkFt7IeP2+CQZBSOpxMiJ
8o1NZZogwqCUtpeBt62T8G54L+ZBPCBAw43ivzXd3yi5AGncEpuK3pT8eg0swyJu
SnU6AX1a26OQxga0sLMb3XAaVmvRYOeCM2bUpZ85BYjZzbnDqFRndXY0BbfsAcoS
+A==
-----END CERTIFICATE-----
)CERT";

void updateFirmware()
{
  Serial.println(F(":::: updateFirmware ::::"));
  Serial.setDebugOutput(true);
  //
  // Set time via NTP, as required for x.509 validation
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    //Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  //

    // Create a list of certificates with the server certificate
  X509List cert(IRG_Root_X1);

  WiFiClientSecure client;
  // client.setSSLVersion(BR_TLS10);

  client.setTrustAnchors(&cert);


  HTTPClient https;
  https.setTimeout(5UL * 60UL * 1000UL);
  client.setTimeout(5UL * 60UL * 1000UL);

  String serverUrl = "https://" + String(SERVER_HOSTNAME) + "/update?chipid=" + String(ESP.getChipId()) + "&type=firmware&currentversion=" + FIRMWARE_VERSION;

  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.print("Connecting to server: ");
    Serial.println(serverUrl);

    t_httpUpdate_return ret = ESPhttpUpdate.update(client, serverUrl, FIRMWARE_VERSION);

    switch (ret)
    {
    case HTTP_UPDATE_FAILED:
      Serial.printf("[update] Update failed (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[update] Update no Update.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("[update] Update ok.");
      break;
    }
  }
}

void updateFileSystem()
{
}
