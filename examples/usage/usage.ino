// Example usage for LTC298X library by Luca Zimmermann.

#include "LTC298X.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

constexpr unsigned long sbaud = 9600; // Serial port baud

// Create instance
LTC298X ltc(A5);

const LogCategoryFilters filters = {
    { "app", LogLevel::LOG_LEVEL_ALL}
};
SerialLogHandler logHandler(LogLevel::LOG_LEVEL_WARN, filters);

void setup() {
    Serial.begin(sbaud);
    waitUntil(Serial.isConnected);
    delay(1s);
    Log.info("Starting");

    // Call functions on initialize hardware
    ltc.begin();
    ltc.setupSenseResistor(2, 5000);
    ltc.setupRTD(3, LTC298X_TYPE_PT_1000, 2, 2, 0, RTD_CURRENT_50uA, RTD_CURVE_EUROPEAN);
    ltc.setupSenseResistor(5, 5000);
    ltc.setupRTD(6, LTC298X_TYPE_PT_1000, 5, 2, 0, RTD_CURRENT_50uA, RTD_CURVE_EUROPEAN);
    ltc.setupSenseResistor(8, 5000);
    ltc.setupRTD(9, LTC298X_TYPE_PT_1000, 8, 2, 0, RTD_CURRENT_50uA, RTD_CURVE_EUROPEAN);
    ltc.setupSenseResistor(11, 5000);
    ltc.setupRTD(12, LTC298X_TYPE_PT_1000, 11, 2, 0, RTD_CURRENT_50uA, RTD_CURVE_EUROPEAN);
    ltc.setupADC(17, true);
    ltc.setupADC(18, true);
    ltc.setupADC(19, true);
    ltc.reportCelsius();
    ltc.reject6050Hz();
    ltc.setMuxDelay(0);
    Log.info("Setup complete");
}

void loop() {
    // Use the library
    if (ltc.selectConversionChannels(LTC298X_CH3|LTC298X_CH6|LTC298X_CH17)) {
        Log.trace("selectConversionChannels succeeded");
    }
    else {
        Log.trace("selectConversionChannels failed");
    }
    ltc.beginMultipleConversion();
    Log.trace("beginMultipleConversion done");
    do {
        delay(1s);
    } while (!ltc.isDone());
    Log.trace("conversion isDone");
    double val = ltc.readTemperature(3);
    Log.trace("readTemperature channel 3 state=%d val=%f", ltc.getState(), val);
    val = ltc.readTemperature(6);
    Log.trace("readTemperature channel 6 state=%d val=%f", ltc.getState(), val);
    val = ltc.readADC(17);
    Log.trace("readADC channel 17 state=%d val=%f", ltc.getState(), val);
    Log.trace(" ");
    delay(10s);
}
