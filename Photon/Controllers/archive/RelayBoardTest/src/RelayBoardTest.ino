/*
 * RelayBoardTest
 * Description: This sketch tests the pins of a 16 channel relay board.
 * Author: Ron Lisle
 * Date: 9/22/17
 *
 * Hardware
 * 1. Photon
 * 2. 5x5min Photon IoT v2 PCB
 * 3. Sainsmart 16 channel relay board
 * 4. Ribbon cable + pins to 16 channel Relay board
 *    D0-D8, A0, A1, 2, A4, A5, RX, TX, WKP
 *    Note: A3 and DAC not used because not 5v tolerant
 *
 * History
 *  9/22/17 Initial creation
 */

long duration = 3000;
long lastTime = millis();
int  state = HIGH;

void setup() {

    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);
    pinMode(RX, OUTPUT);
    pinMode(TX, OUTPUT);
    pinMode(WKP, OUTPUT);

    writeOutputs(state);
}

void loop()
{
    // Update timer. If 3 seconds have elapsed, reverse outputs
    long newTime = millis();
    if(newTime > lastTime + duration)
    {
        lastTime = newTime;
        state = 1-state;
        writeOutputs(state);
    }
}

void writeOutputs(int state)
{
    digitalWrite(D0, state);
    digitalWrite(D1, 1-state);
    digitalWrite(D2, state);
    digitalWrite(D3, 1-state);
    digitalWrite(D4, state);
    digitalWrite(D5, 1-state);
    digitalWrite(D6, state);
    digitalWrite(D7, 1-state);
    digitalWrite(A0, state);
    digitalWrite(A1, 1-state);
    digitalWrite(A2, state);
    digitalWrite(A4, 1-state);
    digitalWrite(A5, state);
    digitalWrite(RX, 1-state);
    digitalWrite(TX, state);
    digitalWrite(WKP, 1-state);
}
