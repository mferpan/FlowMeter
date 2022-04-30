const int sensorPin = 2;
const int measureInterval = 1000;
volatile int pulseConter;

const int relePin = 8;
const float factorK = 1;

float volume = 0;
long t0 = 0;


void ISRCountPulse()
{
  pulseConter++;
}

float GetFrequency()
{
  pulseConter = 0;

  interrupts();
  delay(measureInterval);
  noInterrupts();

  return (float)pulseConter * 1000 / measureInterval;
}

void SumVolume(float dV)
{
  volume += dV / 60 * (millis() - t0) / 1000.0;
  t0 = millis();
}

void RelayON(){
  digitalWrite(relePin, LOW); // turn the Relay on (HIGH is the voltage level)
}

void RelayOFF(){
  digitalWrite(relePin, HIGH); // turn the Relay off (LOW is the voltage level)
}

void setup()
{
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(sensorPin), ISRCountPulse, RISING);
  t0 = millis();
  pinMode(8, OUTPUT);
}

void loop()
{
  // obtener frecuencia en Hz
  float frequency = GetFrequency();

  // calcular caudal L/min
  float flow_Lmin = frequency / factorK;
  SumVolume(flow_Lmin);

  Serial.print("Caudal: ");
  Serial.print(flow_Lmin, 3);
  Serial.print(" (L/min)\tConsumo:");
  Serial.print(volume, 1);
  Serial.println(" (L)");

  if (flow_Lmin > 1)
  {
    RelayON();
  }
  else
  {
    RelayOFF();
  }
}
