void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  Serial.begin(2000000);
}

const int del = 1;      //delay
const int delMul = 4;   //how many times larger is delay on transmitter
const char eot = 0x80;  //end of transmission
const int numChars = 64;
const int numBits = 8 * numChars;
char chars[numChars];
byte bits[numBits];

void loop() {
  if (paddingReceived())
  {
    readBits();
    printChars();
  }
}

bool paddingReceived()
{
  while (true)
  {
    if (digitalRead(2) == HIGH)
    {
      delay(del * delMul);
      if (digitalRead(2) == LOW)
      {
        delay(del * delMul);
        if (digitalRead(2) == HIGH)
        {
          delay(del * delMul);
          if (digitalRead(2) == LOW)
          {
            delay(del * delMul);
            return true;
          }
        }
      }
    }
  }
}

void readBits()
{
  byte counter = 0;
  int bitCounter = 0;
  int charCounter = 0;
  while (true)
  {
    if (digitalRead(2) == HIGH)
    {
      while (digitalRead(2) == HIGH)
      {
        counter++;
        delay(del);
      }
      if (!handleReceivedBits(true, counter, bitCounter, charCounter))
      {
        return;
      }
      counter = 0;
    }
    else if (digitalRead(2) == LOW)
    {
      while (digitalRead(2) == LOW)
      {
        counter++;
        delay(del);
      }
      if (!handleReceivedBits(false, counter, bitCounter, charCounter))
      {
        return;
      }
      counter = 0;
    }
  }
}

bool handleReceivedBits(bool bitType, byte counter, int& bitCounter, int& charCounter)
{
  byte readBits = counter / delMul;
  if (counter % (delMul) >= delMul - 2)
  {
    readBits++;
  }
  for (byte i = 0; i < readBits; i++)
  {
    if (bitType == true)
    {
      bits[bitCounter] = 1;
    }
    else
    {
      bits[bitCounter] = 0;
    }
    bitCounter++;
    if (bitCounter % 8 == 0)
    {
      saveChar(bitCounter, charCounter);
      if (chars[charCounter] == eot)
      {
        return false;
      }
      charCounter++;
    }
  }
  return true;
}

void saveChar(int bitCounter, int charCounter)
{
  if (bitCounter == 0)
  {
    return;
  }
  int numberOfBit = bitCounter - 8;
  if (charCounter == numChars)
  {
    chars[charCounter] = eot;
    return;
  }
  for (byte i = 0; i < 8; i++)
  {
    if (bits[numberOfBit] == 1)
    {
      chars[charCounter] |= (1 << i);
      numberOfBit++;
    }
    else
    {
      chars[charCounter] &= ~(1 << i);
      numberOfBit++;
    }
  }
}

void printChars()
{
  for (byte i = 0; i < numChars; i++)
  {
    if (chars[i] == eot)
    {
      Serial.print("\n");
      return;
    }
    Serial.print(chars[i]);
  }
}
