#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Interfaces.h"
#include "TagDedupFilter.h"
#include <string>
#include <unordered_set>

enum class SystemState {
  INIT_SESSION, // Waiting for Smartphone to send vaccine type
  WAITING_RFID, // Waiting for tag scan
  RFID_READ,    // Tag scanned, waiting for confirmation or timeout
  RECORDING     // Writing tag + vaccine to Bluetooth
};

class StateMachine {
private:
  IRfidReader *rfid;
  IBluetoothModule *ble;
  IButton *button;
  IStatusLeds *leds;
  ITimeProvider *timeProvider;

  SystemState state;
  std::string currentVaccine;
  std::string currentTag;

  uint64_t lastLedToggleTime;
  uint64_t stateEntryTime;

  std::unordered_set<std::string> vaccinatedTags;
  TagDedupFilter dedupFilter;

  void updateLeds();
  void processMessages();

public:
  StateMachine(IRfidReader *r, IBluetoothModule *b, IButton *btn,
               IStatusLeds *l, ITimeProvider *t);

  void init();
  void update();

  SystemState getState() const { return state; }
  std::string getCurrentVaccine() const { return currentVaccine; }
};

#endif // STATEMACHINE_H
