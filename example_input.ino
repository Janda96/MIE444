int test_row1[] = {1, 2, 3, 4};
char test_row2[4] = {'w', 'a', 's', 'd'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
void loop() {
  for (int i=0; i < 4; i++)
  {
    Serial.println(test_row1[i]);
    Serial.println(test_row2[i]);
    delay(1000);
  }
  Serial.end();
  while(true);
}
