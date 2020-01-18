void setup() {
  pinMode(52, OUTPUT);
  Serial.begin(250000);
}

const int del = 1000;     //delayMicroseconds
const char eot = 0x80; //end of transmission
const int numChars = 64;
char chars[numChars];

void loop() {
  resetChars();
  readFromConsole();
  if (chars[0] != eot)
  {
    sendBits();
  }
}

void resetChars()
{
  for (int i = 0; i < numChars; i++)
  {
    chars[i] = eot;
  }
  chars[numChars - 1] = eot;
}

void readFromConsole()
{
  if (Serial.available())
  {
    delay(5);
    readChars();
  }
  else
  {
    return;
  }
}

void readChars()
{
  byte ctr = 0;
  char ch;
  while (Serial.available())
  {
    ch = Serial.read();
    if (ch != '\n')
    {
      if (ctr == numChars - 1)
      {
        break;
      }
      chars[ctr] = ch;
      ctr++;
    }
  }
  if (chars[0] != eot)
  {
    for (int i = 0; i < ctr; i++)
    {
      Serial.print(chars[i]);
    }
    Serial.print("\n");
  }
}

void sendBits()
{
  bool isEndReached = false;
  int i, j;
  sendPadding();
  delayMicroseconds(del);
  for (i = 0; i < numChars; i++)
  {
    if (chars[i] == eot)
    {
      isEndReached = true;
    }
    for (j = 0; j < 8; j++)
    {
      if (chars[i] & (1 << j))
      {
        digitalWrite(52, HIGH);
        delayMicroseconds(del);
      }
      else
      {
        digitalWrite(52, LOW);
        delayMicroseconds(del);
      }
    }
    if (isEndReached)
    {
      digitalWrite(52, LOW);
      break;
    }
  }
}

void sendPadding()
{
  int i;
  for (i = 0; i < 2; i++)
  {
    digitalWrite(52, HIGH);
    delayMicroseconds(del);
    digitalWrite(52, LOW);
    delayMicroseconds(del);
  }
}
