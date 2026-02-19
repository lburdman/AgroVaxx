#include "../core/StateMachine.h"
#include "MockDrivers.h"
#include <cassert>
#include <iostream>

// Simple testing macros
#define TEST_CASE(name) void name()
#define EXPECT_EQ(a, b)                                                        \
  do {                                                                         \
    if ((a) != (b)) {                                                          \
      std::cerr << "Assertion failed: " << #a << " == " << #b << " at line "   \
                << __LINE__ << std::endl;                                      \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)
#define EXPECT_TRUE(a) EXPECT_EQ(a, true)
#define EXPECT_FALSE(a) EXPECT_EQ(a, false)

struct TestFixture {
  MockRfid rfid;
  MockBluetooth ble;
  MockButton button;
  MockLeds leds;
  MockTime timeProvider;
  StateMachine sm;

  TestFixture() : sm(&rfid, &ble, &button, &leds, &timeProvider) { sm.init(); }
};

TEST_CASE(InitialStateIsInitSession) {
  TestFixture f;
  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::INIT_SESSION));
}

TEST_CASE(TransitionsToWaitRfidOnVaccineSelect) {
  TestFixture f;
  f.ble.incomingMsgs.push("VACCINE:AFTOSA");
  f.sm.update();
  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::WAITING_RFID));
  EXPECT_EQ(f.sm.getCurrentVaccine(), "AFTOSA");
  EXPECT_EQ(f.ble.outgoingMsgs.size(), 1);
  EXPECT_EQ(f.ble.outgoingMsgs[0], "ACK_VACCINE:AFTOSA\n");
}

TEST_CASE(IgnoresRfidBeforeVaccineSelection) {
  TestFixture f;
  f.rfid.hasNewTag = true;
  f.rfid.nextTag = "TAG123";
  f.sm.update();
  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::INIT_SESSION));
}

TEST_CASE(TransitionsToRfidReadOnValidTag) {
  TestFixture f;
  f.ble.incomingMsgs.push("VACCINE:BRUCELLA");
  f.sm.update();

  f.rfid.hasNewTag = true;
  f.rfid.nextTag = "TAG456";
  f.sm.update();
  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::RFID_READ));
  EXPECT_TRUE(f.leds.greenOn);
}

TEST_CASE(ValidatesAlreadyVaccinatedTag) {
  TestFixture f;
  f.ble.incomingMsgs.push("VACCINE:BRUCELLA");
  f.sm.update();
  f.ble.outgoingMsgs.clear();

  f.rfid.hasNewTag = true;
  f.rfid.nextTag = "TAG456";
  f.sm.update();
  f.button.pressed = true;
  f.sm.update();                // Moves to RECORDING
  f.timeProvider.advance(1500); // Wait out the blinking time
  f.sm.update();                // Moves to WAITING_RFID

  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::WAITING_RFID));
  EXPECT_EQ(f.ble.outgoingMsgs.size(), 1);
  EXPECT_EQ(f.ble.outgoingMsgs[0], "VACCINATED,BRUCELLA,TAG456\n");
  f.ble.outgoingMsgs.clear();

  f.button.pressed = false;
  f.rfid.hasNewTag = true;
  f.rfid.nextTag = "TAG456";
  f.sm.update();

  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::WAITING_RFID));
  EXPECT_EQ(f.ble.outgoingMsgs.size(), 1);
  EXPECT_EQ(f.ble.outgoingMsgs[0], "ALREADY_VACCINATED:TAG456\n");
}

TEST_CASE(RfidReadTimesOutWithoutConfirmation) {
  TestFixture f;
  f.ble.incomingMsgs.push("VACCINE:A");
  f.sm.update();

  f.rfid.hasNewTag = true;
  f.rfid.nextTag = "TAG_TMOUT";
  f.sm.update();
  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::RFID_READ));

  f.timeProvider.advance(6000);
  f.sm.update();

  EXPECT_EQ(static_cast<int>(f.sm.getState()),
            static_cast<int>(SystemState::WAITING_RFID));
}

int main() {
  std::cout << "Running tests..." << std::endl;
  InitialStateIsInitSession();
  TransitionsToWaitRfidOnVaccineSelect();
  IgnoresRfidBeforeVaccineSelection();
  TransitionsToRfidReadOnValidTag();
  ValidatesAlreadyVaccinatedTag();
  RfidReadTimesOutWithoutConfirmation();
  std::cout << "All tests passed!" << std::endl;
  return 0;
}
