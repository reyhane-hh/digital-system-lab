void setup() {
for(int i=6;i<=13;i++){
  pinMode(i,OUTPUT);
  }
}

void loop() {
  for(int i=0;i<=255;i++){
   for(int j=6;j<=13;j++){
    digitalWrite(j,bitRead(i,j-6));
   }
   delay(250);
  }
}
