void setup() {
  // put your setup code here, to run once:
  pinMode(52,OUTPUT);
  Serial.begin(115200);
}

const char eot = 0x4; //end of transmission
const int numChars = 64;
char chars[numChars];

void loop() {
  resetChars();
  readFromConsole();
  if(chars[0] != eot)
  {
    sendBits();
  }
}

void resetChars()
{
  for(int i = 0; i < numChars; i++)
  {
    chars[i] = eot;
  }
}

void readFromConsole() 
{
  byte ctr = 0;
  char ch;
  while (Serial.available())
  {
    ch = Serial.read();
    if(ch != '\n' || ch != '\0')
    {
      if(ctr == numChars)
      {
        break;
      }
      chars[ctr] = ch;
      ctr++;
    }
  }
  for(int i = 0; i < ctr; i++)
  {
    Serial.print(chars[i]);
  }
  Serial.print('\n');
}

void sendBits()
{
  sendPadding();
  bool isEndReached = false;
  for(int i = 0; i < numChars; i++)
  {
    if(chars[i] == eot)
    {
      isEndReached = true;
    }
    for(int j = 0; j < 8; j++)
    {
      if(chars[i] & (1 << j))
      {
        digitalWrite(52,HIGH);
        delay(8);
      }
      else
      {
        digitalWrite(52,LOW);
        delay(8);
      }
    }
    if(isEndReached)
    {
      break;
    }
  }
}

void sendPadding()
{
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(52,HIGH);
    delay(8);
  }
}
