void setup()
{
pinMode(12,OUTPUT);
}
void loop()
{
    digitalWrite(12,LOW);
    delay(10000);
    digitalWrite(12,HIGH);
    delay(10000);
}