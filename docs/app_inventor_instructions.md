# AgroVaxx - MIT App Inventor Baseline App

The AgroVaxx system operates by receiving a vaccination code from your smartphone, waiting for you to scan an object, and finally sending back the event data.

## Protocol Specification

**1) Initialization (App -> STM32)**
- The smartphone connects via Bluetooth.
- The app sends the selected vaccine string over UART:
  `VACCINE:<VACCINE_NAME>` (e.g., `VACCINE:AFTOSA`)
- The STM32 confirms with:
  `ACK_VACCINE:<VACCINE_NAME>\n`

**2) Vaccination (STM32 -> App)**
- When a tag is scanned and the physical button is pressed:
  `VACCINATED,<VACCINE_NAME>,<TAG_UID>\n`
- The App must parse this comma-separated value (CSV) and append it to its internal list/database.

**3) Duplicates (STM32 -> App)**
- If a tag has already been scanned for the current vaccine session:
  `ALREADY_VACCINATED:<TAG_UID>\n`
- The App should show a warning to the user.

## Designing the App in MIT App Inventor

### 1. User Interface (Designer Tab)
- Add a **ListPicker** for Bluetooth devices (Name: `BT_Connect`).
- Add a **TextBox** + **Button** to manually type and send the Vaccine name (Name: `Send_Vaccine_Btn`).
- Add a **Label** for connection status (Name: `Status_Label`).
- Add a **ListView** or **TextBox** (multi-line) to display the log of vaccinated tags (Name: `Log_List`).
- Add a **Button** to "Export Log" (Name: `Export_Btn`).
- **Non-Visible Components:** 
  - `BluetoothClient1`
  - `Clock1` (TimerInterval = 100ms)
  - `File1` (for exporting logs)
  - `Notifier1` (for alerts)

### 2. Blocks (Logic Tab)

#### Connecting
- **When BT_Connect.BeforePicking**: Set `BT_Connect.Elements` to `BluetoothClient1.AddressesAndNames`.
- **When BT_Connect.AfterPicking**: 
  - Call `BluetoothClient1.Connect` (address: `BT_Connect.Selection`).
  - If successful, set `Status_Label.Text` to "Connected. Send Vaccine!"

#### Sending Vaccine Selection
- **When Send_Vaccine_Btn.Click**:
  - Call `BluetoothClient1.SendText` (text: join("VACCINE:", `Vaccine_TextBox.Text`)).

#### Receiving Data & Logging (The Clock)
- **When Clock1.Timer**:
  - If `BluetoothClient1.IsConnected` and `BluetoothClient1.BytesAvailableToReceive > 0`:
    - Set local variable `Incoming` to `BluetoothClient1.ReceiveText(numberOfBytes: -1)`
    - If `Incoming` contains `VACCINATED,`:
      - Timestamp it: join(Clock.Now(), ",", Incoming)
      - Add it to `Log_List`.
    - If `Incoming` contains `ALREADY_VACCINATED:`:
      - Call `Notifier1.ShowAlert` ("Tag Already Vaccinated!").

#### Exporting Log
- **When Export_Btn.Click**:
  - Call `File1.SaveFile` with text from `Log_List` to a file name like `/Download/AgroVaxx_Log.csv`.
