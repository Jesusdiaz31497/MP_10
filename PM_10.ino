
uint8_t data_request[] = {2, 68, 65, 3};
uint8_t pm[4], temperatura[4], humedad[4], Presion[4], error[4], Tx_msg[14], num, tiempo, pos;
bool flag_rx;
char data;
uint16_t Var;


void setup() {
  Serial3.begin(9600);
  Serial2.begin(9600);

  pinMode(13, OUTPUT);

  TCCR1A = 0;
  TCCR1B = 5;
  TCNT1 = 49911;
  TIMSK1 = 1;
  sei();

}

void loop() {


  while (Serial3.available()) {

    if (Serial3.available() > 0) {
      data = Serial3.read();

      if (data == 3) {
        flag_rx = false;                // END OF DATA RX
        pos = 0;
        Send_data();
      }

      if (flag_rx)   {                  // SAVE DATA RX
        pos++;
        num = data - 48;
        Save_data();
      }

      if (data == 2) flag_rx = true;    // START OF DATA RX

    }
  }



}


ISR(TIMER1_OVF_vect) {
  TCNT1 = 49911;
  tiempo++;

  if (tiempo == 60) {

    for (uint8_t x = 0; x < 4; x++) {
      Serial3.write(data_request[x]);
    }
    tiempo = 0;
  }
}

void Save_data() {
  if (pos == 71) pm[0] = num;
  if (pos == 72) pm[1] = num;
  if (pos == 73) pm[2] = num;
  if (pos == 74) pm[3] = num;
  if (pos == 77) Tx_msg[2] = num;

  if (pos == 101) temperatura[0] = num;
  if (pos == 102) temperatura[1] = num;
  if (pos == 103) temperatura[2] = num;
  if (pos == 104) temperatura[3] = num;
  if (pos == 107) Tx_msg[5] = num;

  if (pos == 131) humedad[0] = num;
  if (pos == 132) humedad[1] = num;
  if (pos == 133) humedad[2] = num;
  if (pos == 134) humedad[3] = num;
  if (pos == 137) Tx_msg[8] = num;

  if (pos == 161) Presion[0] = num;
  if (pos == 162) Presion[1] = num;
  if (pos == 163) Presion[2] = num;
  if (pos == 164) Presion[3] = num;
  if (pos == 167) Tx_msg[11] = num;

  if (pos == 221) error[0] = num;
  if (pos == 222) error[1] = num;
  if (pos == 223) error[2] = num;
  if (pos == 224) error[3] = num;

}

void Send_data() {
  Var = pm[0] * 1000 + pm[1] * 100 + pm[2] * 10 + pm[3];
  Tx_msg[0] = (Var >> 8) & (0xFF);
  Tx_msg[1] = Var & 0xFF;

  Var = temperatura[0] * 1000 + temperatura[1] * 100 + temperatura[2] * 10 + temperatura[3];
  Tx_msg[3] = (Var >> 8) & (0xFF);
  Tx_msg[4] = Var & 0xFF;

  Var = humedad[0] * 1000 + humedad[1] * 100 + humedad[2] * 10 + humedad[3];
  Tx_msg[6] = (Var >> 8) & (0xFF);
  Tx_msg[7] = Var & 0xFF;

  Var = Presion[0] * 1000 + Presion[1] * 100 + Presion[2] * 10 + Presion[3];
  Tx_msg[9] = (Var >> 8) & (0xFF);
  Tx_msg[10] = Var & 0xFF;

  Var = error[0] * 1000 + error[1] * 100 + error[2] * 10 + error[3];
  Tx_msg[12] = (Var >> 8) & (0xFF);
  Tx_msg[13] = Var & 0xFF;

  digitalWrite(13, HIGH);             // SEND LORA
  delay(200);
  digitalWrite(13, LOW);

  for (uint8_t x = 0; x < 14; x++) {
    Serial2.write(Tx_msg[x]);
  } Serial2.write(0x6D); Serial2.write(0x70);

  for (uint8_t x = 0; x < 14; x++) {
    Tx_msg[x] = 0;
  }
}

// hola git
