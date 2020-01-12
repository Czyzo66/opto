void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  Serial.begin(2000000);
}

const int del = 2;    //delay
const int delMul = 8; //how many times larger is delay on transmitter
const char eot = 0x4; //end of transmission
const int numChars = 64;
char chars[numChars];

void loop() {
  if(paddingReceived())
  {
    readChars();
    printChars();
  }
}

bool paddingReceived()
{
  int counter = 0;
  while(true)
  {
    if(digitalRead(2) == HIGH)
    {
      delay(1);
      if(!receivedHigh())
      {
        return false;      
      }
      if(!receivedLow())
      {
        return false;      
      }
      if(!receivedHigh())
      {
        Serial.print("High2\n");
        return false;      
      }
      if(!receivedLow())
      {
        Serial.print("Low2\n");
        return false;      
      }
      //Serial.print("Transmission:\n");
      return true;
    }
  }
}

bool receivedLow()
{
  int counter = 0;
  for(int i = 0; i < delMul; i++)
  {
    if(digitalRead(2) == LOW)
    {
      //Serial.print("L");
      counter++;
      delay(del);
    }
  }
  if(counter < delMul-2)
  {
    return false;
  }
  return true;
}

bool receivedHigh()
{
  int counter = 0;
  for(int i = 0; i < delMul; i++)
  {
    if(digitalRead(2) == HIGH)
    {
      //Serial.print("H");
      counter++;
      delay(del);
    }
  }
  if(counter < delMul-2)
  {
    return false;
  }
  return true;
}

void readChars()
{
  char ch = 0;
  int charCount = 0;
  while(ch != eot) //end of transmission
  {
    for(int i = 0; i < 8; i++)
    {
      int counter = 0;
      for(int j = 0; j < delMul; j++)
      {
        if (digitalRead(2) == HIGH)
        {
          //Serial.print('1');
          counter++;
        }
        delay(del);
      }
      if(counter >= delMul-4)
      {
        //Serial.print('1');
        ch |= (1 << i);
      }
      else
      { 
        ch &= ~(1 << i);
      }
    }
    if(ch != 0)
    {
      chars[charCount] = ch;
      charCount++;
    }
    else
    {
      return;
    }  
  }
}

void printChars()
{
  for(int i = 0; i < numChars; i++)
  {
    if(chars[i] == eot)
    {
      Serial.print("\n");
      return;
    }
    Serial.print(chars[i]);
  }
}
