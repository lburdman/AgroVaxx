#include "StateMachine.h"

// Time constants in milliseconds
constexpr uint64_t BLINK_WAITING_MS = 1000;
constexpr uint64_t BLINK_RECORDING_MS = 200;
constexpr uint64_t CONFIRM_TIMEOUT_MS =
    5000; // 5 seconds to confirm after scanning

StateMachine::StateMachine(IRfidReader *r, IBluetoothModule *b, IButton *btn,
                           IStatusLeds *l, ITimeProvider *t)
    : rfid(r), ble(b), button(btn), leds(l), timeProvider(t),
      state(SystemState::INIT_SESSION), lastLedToggleTime(0),
      stateEntryTime(0) {}

void StateMachine::init() {
  rfid->init();
  ble->init();
  button->init();
  leds->init();
  state = SystemState::INIT_SESSION;
}

void StateMachine::processMessages() {
  std::string msg = ble->readMessage();
  if (!msg.empty()) {
    // Simple protocol: VACCINE:NAME
    if (msg.rfind("VACCINE:", 0) == 0) {
      currentVaccine = msg.substr(8);
      vaccinatedTags.clear(); // New session, clear cache
      state = SystemState::WAITING_RFID;
      stateEntryTime = timeProvider->getTicksMs();
      ble->sendMessage("ACK_VACCINE:" + currentVaccine + "\n");
    }
  }
}

void StateMachine::updateLeds() {
  // Blue LED ON when Bluetooth connected
  leds->setBlue(ble->isConnected());

  uint64_t now = timeProvider->getTicksMs();

  // Green LED behavior by state
  switch (state) {
  case SystemState::INIT_SESSION:
    leds->setGreen(false);
    break;

  case SystemState::WAITING_RFID:
    if (now - lastLedToggleTime >= BLINK_WAITING_MS) {
      leds->toggleGreen();
      lastLedToggleTime = now;
    }
    break;

  case SystemState::RFID_READ:
    leds->setGreen(true); // Solid
    break;

  case SystemState::RECORDING:
    if (now - lastLedToggleTime >= BLINK_RECORDING_MS) {
      leds->toggleGreen();
      lastLedToggleTime = now;
    }
    break;
  }
}

void StateMachine::update() {
  processMessages();

  uint64_t now = timeProvider->getTicksMs();

  switch (state) {
  case SystemState::INIT_SESSION:
    // Waiting for a vaccine to be selected via Bluetooth
    break;

  case SystemState::WAITING_RFID:
    if (rfid->isNewTagPresent()) {
      std::string tag = rfid->readTagUid();
      if (!tag.empty()) {
        if (vaccinatedTags.find(tag) != vaccinatedTags.end()) {
          // Already vaccinated in this session, ignore
          ble->sendMessage("ALREADY_VACCINATED:" + tag + "\n");
          // We do NOT change state, ignore and keep waiting
        } else {
          currentTag = tag;
          state = SystemState::RFID_READ;
          stateEntryTime = now;
        }
      }
    }
    break;

  case SystemState::RFID_READ:
    if (button->isPressed()) {
      state = SystemState::RECORDING;
      stateEntryTime = now;
      lastLedToggleTime = now;

      // Send vaccination info to app
      ble->sendMessage("VACCINATED," + currentVaccine + "," + currentTag +
                       "\n");
      vaccinatedTags.insert(currentTag);
    } else if (now - stateEntryTime > CONFIRM_TIMEOUT_MS) {
      // Timeout waiting for confirmation
      currentTag = "";
      state = SystemState::WAITING_RFID;
    }
    break;

  case SystemState::RECORDING:
    if (now - stateEntryTime < 1000) {
      // Just wait for 1 second showing the fast blink
      if (now - lastLedToggleTime >= BLINK_RECORDING_MS) {
        leds->toggleGreen();
        lastLedToggleTime = now;
      }
    } else {
      currentTag = "";
      state = SystemState::WAITING_RFID;
    }
    break;
  }

  updateLeds();
}
