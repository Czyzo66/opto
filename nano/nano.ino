void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  Serial.begin(115200);
}

const char eot = 0x4; //end of transmission

void loop() {
  if(paddingReceived())
  {
    readChars();
  }
}

bool paddingReceived()
{
  int counter = 0;
  while(true)
  {
    if(digitalRead(2) == HIGH)
    {
      counter++;
    }
    else
    {
      counter = 0;
    }
    delay(2);
    if(counter >= 30)
    {
      return true;
    }
  }
}

void readChars()
{
  char ch = 0;
  while(ch != eot) //end of transmission
  {
    for(int i = 0; i < 8; i++)
    {
      int counter = 0;
      for(int j = 0; j < 4; j++)
      {
        if (digitalRead(2) == HIGH)
        {
          counter++;
        }
        delay(2);
      }
      if(counter >= 2)
      {
        ch |= (1 << i);
      }
      else
      {
        ch &= ~(1 << i);
      }
    }
    Serial.print(ch);
  }
  Serial.print('\n');
}
