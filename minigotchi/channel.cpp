/////////////////////////////////////////////
// channel.cpp: handles channel switching  //
/////////////////////////////////////////////

#include "channel.h"

/* developer note: 
*
* i am using the ideal 2.4 GHz channels, which are 1, 6, and 11.
* the reason for that is so we don't interfere with other devices on our frequency.
* there are probably plenty more reasons but this is a good practice for such iot devices.
*
*/ 

Channel::Channel() {
    // nothing here as raw handles the initial channel
}

int Channel::channelList[] = {1, 6, 11};

void Channel::cycle() { 
    // get channels
    int numChannels = sizeof(channelList) / sizeof(channelList[0]);

    // select a random one
    int randomIndex = random(numChannels);
    int newChannel = channelList[randomIndex];

    // switch here
    switchC(newChannel);
}

void Channel::switchC(int newChannel) {
    // stop raw80211 from being on this one channel
    Serial.print("(-.-) Switching to channel ");
    Serial.println(newChannel);
    Raw80211::stop();

    // monitor this one channel
    wifi_promiscuous_enable(0);
    wifi_set_channel(newChannel);
    wifi_promiscuous_enable(1);

    // switched channel
    Serial.print("('-') Currently on channel ");
    Serial.println(newChannel);
}

int Channel::getChannel() {
    return wifi_get_channel();
}

int Channel::list() {
    return channelList[3];
}