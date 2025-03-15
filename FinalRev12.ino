#include <ZMPT101B.h>
#define SENSITIVITY 314.75f
ZMPT101B voltageSensor(A0, 50.0);
const int c1 = 2, c2 = 3, c3 = 4, c4 = 5, volt = A0, regularDealy = 6000, retryDelay = 180000, tickLength = 20000, tolleranceTemp = 180000, sleepDelay = 1800000;
float voltage = 0,start = 0;
int position = 0;
bool hasStarted = 0;
int mini=210, maxi=235, critMini=190, critMaxi=250;
int now = 0, in = 0, tries = 0, out = 1, keep = 0, inmax=0, inmin=0;

int down(int currentPosition) {
  if (currentPosition > 0) {
    currentPosition -= 1;
  }
  return currentPosition;
}

int up(int currentPosition) {
  if (currentPosition < 5) {
    currentPosition += 1;
  }
  return currentPosition;
}

void setup() {
  voltageSensor.setSensitivity(SENSITIVITY);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);

  digitalWrite(c1, HIGH);
  digitalWrite(c2, HIGH);
  digitalWrite(c3, HIGH);
  digitalWrite(c4, HIGH);
  Serial.begin(9600);
}

void changePosition(int position) {
  out = 1;
  in = 0;
  Serial.print("Position : ");
  Serial.println(position);
  if (position == 0) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
  } else if (position == 1) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    delay(regularDealy);
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
  } else if (position == 2) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    delay(regularDealy);
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
  } else if (position == 3) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    delay(regularDealy);
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, LOW);
    digitalWrite(c4, HIGH);
  } else if (position == 4) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    delay(regularDealy);
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, LOW);
  } else if (position == 5) {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    delay(regularDealy);
  } else {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
  }
}


void retry() {
  if(hasStarted)
  {
    if (position == 5) 
    {
      digitalWrite(c1, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(c3, HIGH);
      digitalWrite(c4, HIGH);

      if (tries > 2)
      {
        Serial.println("Tries > 2 delay");
        delay(1800000);
        tries = 0;
      }
      tries++;

      Serial.println("delay(300000); if position 5");
      delay(180000);
      position = down(position);
      changePosition(position);
      delay(regularDealy);
    }
    if (position == 0) {
      if (tries > 2) {
        Serial.println("Tries > 2 delay");
        delay(1800000);
        tries = 0;
      }
      tries++;
      digitalWrite(c1, HIGH);
      digitalWrite(c2, HIGH);
      digitalWrite(c3, HIGH);
      digitalWrite(c4, HIGH);
      if (hasStarted) {
        Serial.println("delay(300000); if position 0");
        delay(180000);
        position = up(position);
        changePosition(position);
        delay(regularDealy);
      }
    }
  }
}

void testing()
{
  Serial.println("Enter voltage: ");
  int b = Serial.read();
  if(b==49)
  {
    voltage = 180;
    Serial.print("Voltage set to : ");
    Serial.println(voltage);
  }
  if(b==50)
  {
    voltage = 200;
    Serial.print("Voltage set to : ");
    Serial.println(voltage);
  }
  if(b==51)
  {
    voltage = 220;
    Serial.print("Voltage set to : ");
    Serial.println(voltage);
  }
  if(b==52)
  {
    voltage = 240;
    Serial.print("Voltage set to : ");
    Serial.println(voltage);
  }
  if(b==53)
  {
    voltage = 260;
    Serial.print("Voltage set to : ");
    Serial.println(voltage);
  }
}

void loop() {
  if(hasStarted)
  {
    delay(tickLength);
  }
  if (position == 5) {
    retry();
  }
  else if (position == 0) {
    retry();
  }
  else
  {
    
    tries = 0;
  }

  voltage = voltageSensor.getRmsVoltage();
  //testing();
  /*voltage = analogRead(A0);
  voltage *=(230/1023)*/
  Serial.print("Voltage : ");
  Serial.println(voltage);
  if (voltage < 0) {
    voltage = -voltage;
  }

  if (voltage < mini || voltage > maxi)
  {
    if(in == 0)
    {
      start = millis();
    }
    out = 0;
    in = 1;
  }

  if (voltage > mini && voltage < maxi && in == 1 && out == 0) {
    out = 1;
    in = 0;
  }

  if(in == 1 && out == 0)
  {
    if (voltage < mini)
    {
      now = millis();
      if(inmin == 0 && inmax == 1)
      {
        inmin = 1;
        inmax = 0;
        start = millis();
      }
      inmin = 1;
      inmax = 0;
      if (millis() - start > 180000)
      {
        Serial.print("tollerance :");
        Serial.print(millis());
        Serial.print("-");
        Serial.print(start);
        Serial.print("=");
        Serial.println(millis() - start);
        position = up(position);
        changePosition(position);
        start = millis();
      }
    }
    if (voltage > maxi)
    {
      now = millis();
      if(inmin == 1 && inmax == 0)
      {
        inmin = 0;
        inmax = 1;
        start = millis();
      }
      inmin = 0;
      inmax = 1;
      if (millis() - start > 180000)
      {
        Serial.print("tollerance :");
        Serial.print(millis());
        Serial.print("-");
        Serial.print(start);
        Serial.print("=");
        Serial.println(millis() - start);
        position = down(position);
        changePosition(position);
        start = millis();
      }
    }
  }

  Serial.print("tollerance");
  Serial.println(millis() - start);

  if (voltage < critMini) {
    in = 0;
    out = 1;
    Serial.println("critMini");
    position = up(position);
    changePosition(position);
  }
  if (voltage > critMaxi) {
    in = 0;
    out = 1;
    Serial.println("critMaxi");
    position = down(position);
    changePosition(position);
  }
  hasStarted = 1;
}


//225 450  680  900
