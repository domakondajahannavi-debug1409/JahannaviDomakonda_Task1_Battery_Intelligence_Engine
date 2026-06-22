#define NUM_CELLS 4

int cellPins[NUM_CELLS] = {34, 35, 32, 33};
float cellVoltage[NUM_CELLS];

float CELL_MIN = 3.0;
float CELL_MAX = 4.2;

float MINOR_IMBALANCE = 2.0;
float CRITICAL_IMBALANCE = 5.0;

float readCellVoltage(int pin) {
  int adcValue = analogRead(pin);
  float voltage = CELL_MIN + ((float)adcValue / 4095.0) * (CELL_MAX - CELL_MIN);
  return voltage;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Adaptive Multi-Cell Battery Intelligence Engine");
  Serial.println("System Started...");
}

void loop() {
  float packVoltage = 0;
  float weakestCell = 100;
  float strongestCell = 0;

  int weakestCellIndex = 0;
  int strongestCellIndex = 0;

  for (int i = 0; i < NUM_CELLS; i++) {
    cellVoltage[i] = readCellVoltage(cellPins[i]);
    packVoltage += cellVoltage[i];

    if (cellVoltage[i] < weakestCell) {
      weakestCell = cellVoltage[i];
      weakestCellIndex = i;
    }

    if (cellVoltage[i] > strongestCell) {
      strongestCell = cellVoltage[i];
      strongestCellIndex = i;
    }
  }

  float averageVoltage = packVoltage / NUM_CELLS;
  float imbalancePercent = ((strongestCell - weakestCell) / averageVoltage) * 100.0;

  String batteryState;

  if (weakestCell <= 3.05 || strongestCell >= 4.15) {
    batteryState = "PACK FAILURE";
  } 
  else if (imbalancePercent < MINOR_IMBALANCE) {
    batteryState = "HEALTHY";
  } 
  else if (imbalancePercent < CRITICAL_IMBALANCE) {
    batteryState = "MINOR IMBALANCE";
  } 
  else {
    batteryState = "CRITICAL IMBALANCE";
  }

  Serial.println("------------------------------------");

  for (int i = 0; i < NUM_CELLS; i++) {
    Serial.print("Cell ");
    Serial.print(i + 1);
    Serial.print(" Voltage: ");
    Serial.print(cellVoltage[i], 2);
    Serial.println(" V");
  }

  Serial.print("Pack Voltage: ");
  Serial.print(packVoltage, 2);
  Serial.println(" V");

  Serial.print("Average Voltage: ");
  Serial.print(averageVoltage, 2);
  Serial.println(" V");

  Serial.print("Weakest Cell: Cell ");
  Serial.print(weakestCellIndex + 1);
  Serial.print(" = ");
  Serial.print(weakestCell, 2);
  Serial.println(" V");

  Serial.print("Strongest Cell: Cell ");
  Serial.print(strongestCellIndex + 1);
  Serial.print(" = ");
  Serial.print(strongestCell, 2);
  Serial.println(" V");

  Serial.print("Imbalance Percentage: ");
  Serial.print(imbalancePercent, 2);
  Serial.println(" %");

  Serial.print("Battery Health State: ");
  Serial.println(batteryState);

  delay(2000);
}