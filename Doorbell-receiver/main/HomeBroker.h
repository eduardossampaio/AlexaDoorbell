#ifndef _HOMEBROKER_H_
#define _HOMEBROKER_H_

#include <SinricProDevice.h>
#include <Capabilities/Doorbell.h>
//#include <Capabilities/PushNotification.h>
#include <RCSwitch.h>

class HomeBroker 
: public SinricProDevice
, public Doorbell<HomeBroker> {
//, public PushNotification<HomeBroker> {
  friend class Doorbell<HomeBroker>;
  //friend class PushNotification<HomeBroker>;
public:
  HomeBroker(const String &deviceId) : SinricProDevice(deviceId, "HomeBroker") {};
};

#endif
