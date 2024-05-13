#define DISTANCE_DANGER_LEVEL1 15
#define DISTANCE_DANGER_LEVEL2 30
#define DISTANCE_DANGER_LEVEL3 45
#define DISTANCE_DANGER_LEVEL4 60
#define DISTANCE_NOT_MEASURED 0

int sure=0; //Ses dalgasının gidip gelme süresi değişkeni
int distance=0;

const int enablePin1 = 5; // Enable pini 1 (Motor 1 için); hatam asil pinlerin hepsi dogru 
                          //Fakat enablePinleri capraz verdigim icin ayni anda motor dönyüor fakat 
                          //tek istedgiimde dönmüyor cünkü enable 2 ile calistirmaya caliyormusum.
const int enablePin2 = 6; // Enable pini 2 (Motor 2 için)

// Motor 1 yön kontrol pinleri (örnek)
const int motor1PinA = 11; // DC motor 1'in A pini
const int motor1PinB = 10; // DC motor 1'in B pini

// Motor 2 yön kontrol pinleri (örnek)
const int motor2PinA = 8; // DC motor 2'nin ileri yön pini
const int motor2PinB = 9; // DC motor 2'nin geri yön pini

const int buzzerPin = 3; // Buzzer pini
const int trigPin = 10; // Ultrasonik sensör trig pini
const int echoPin = 13; // Ultrasonik sensör echo pini
const int LedPin=2;

int mesafe; // Mesafe sensöründen gelen değeri saklamak için değişken

typedef enum 
{
  notMeasured = 0,
  lowerThan15 = 1,
  lowerThan30 = 2,
  lowerThan45 = 3,
  lowerThan60 = 4,
  noDanger = 5
} DistanceLevel_t;

DistanceLevel_t getDistanceLevel()
{
  digitalWrite(trigPin, LOW);                           //Ultrasonik sensör ile ölçüm sekansını başlatıyoruz.
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH, 11600);                 //Ses dalgasının gidip gelme süresini ölçüyoruz.
  distance= sure*0.0345/2;                               //Ölçülen süre ile uzaklık hesabı yapıyoruz.

  Serial.print("distance:");  
  Serial.println(distance);
  
  DistanceLevel_t distanceLevel = noDanger;
  if(distance < 2) // this sensor can not measure smaller than 2cm
  {
    distanceLevel = notMeasured;
  }
  else if(distance < DISTANCE_DANGER_LEVEL1)
  {
    distanceLevel = lowerThan15;
  }
  else if(distance < DISTANCE_DANGER_LEVEL2)
  {
    distanceLevel = lowerThan30;
  }
  else if(distance < DISTANCE_DANGER_LEVEL3)
  {
    distanceLevel = lowerThan45;
  }
  else if(distance < DISTANCE_DANGER_LEVEL4)
  {
    distanceLevel = lowerThan60;
  }
  else
  {
    distanceLevel = noDanger;
  }

  return distanceLevel;

}

int getMotorAnalogValue(DistanceLevel_t distanceLevel)
{
  if(distanceLevel == lowerThan15)
  {
    return 0; //we are in danger! STOP!!!
  }
  else if(distanceLevel == lowerThan30)
  {
    return 100; 
  }
  else if(distanceLevel == lowerThan45)
  {
    return 150; 
  }
  else if(distanceLevel == lowerThan60)
  {
    return 200; 
  }
  else
  {
    return 255; 
  }
}

void stopMotors(void)
{

 // Her iki motoru durdurun
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);

  // Belirli bir süre bekleyin (saniye cinsinden)
  delay(100); // 1 saniye
}

void controlSpeed()
{
  DistanceLevel_t distanceLevel = getDistanceLevel();
  Serial.print("distanceLevel:");  
  Serial.println(distanceLevel);  
  if(distanceLevel == notMeasured)
  {
    delay(1000);
    return;
  }

  controlBuzzer(distanceLevel);
  controlLED(distanceLevel);
   
  int motorAnalogValue = getMotorAnalogValue(distanceLevel);
  Serial.print("motorAnalogValue:");
  Serial.println(motorAnalogValue);
  
  // Her iki motoru da çalıştırın
  analogWrite(enablePin1, motorAnalogValue);
  analogWrite(enablePin2, motorAnalogValue);

  // Her iki motoru da ileri yönde döndürün
  digitalWrite(motor1PinA, HIGH);
  digitalWrite(motor1PinB, LOW);
  digitalWrite(motor2PinA, HIGH);
  digitalWrite(motor2PinB, LOW);

  // Belirli bir süre bekleyin (saniye cinsinden)
  delay(200); // 2 saniye

  stopMotors();
}
//wurde aktualisiert
void controlBuzzer(DistanceLevel_t distanceLevel)
 {
  if(distanceLevel == noDanger)
  {
    digitalWrite(buzzerPin , LOW);
    return;
  }
  else if(distanceLevel == lowerThan15)
  {
    digitalWrite(buzzerPin , HIGH);
    delay(distanceLevel * 50);
    return;
  }
    
  digitalWrite(buzzerPin , HIGH);
  delay(distanceLevel * 50);
  digitalWrite(buzzerPin , LOW);
  delay(distanceLevel * 50);
}

void controlLED(DistanceLevel_t distanceLevel)
 {
  if(distanceLevel == noDanger)
  {
    digitalWrite(LedPin , LOW);
    return;
  }
  else if(distanceLevel == lowerThan15)
  {
    digitalWrite(LedPin , HIGH);
    delay(distanceLevel * 50);
    return;
  }
    
  digitalWrite(LedPin , HIGH);
  delay(distanceLevel * 50);
  digitalWrite(LedPin , LOW);
  delay(distanceLevel * 50);
}



void setup() {
  Serial.begin(9600);
  
  // Motor pinlerini, enable pinlerini, buzzer pinini ve sensör pinlerini çıkış olarak ayarlayın
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
    
  pinMode(motor1PinA, OUTPUT);
  pinMode(motor1PinB, OUTPUT);
  pinMode(motor2PinA, OUTPUT);
  pinMode(motor2PinB, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  pinMode(LedPin, OUTPUT);


  // Enable pinlerini başlangıçta düşük (kapalı) olarak ayarlayın
  digitalWrite(enablePin1, LOW);
  digitalWrite(enablePin2, LOW);
}

void loop() {
  // Motorları kontrol edin
  controlSpeed();

  // Mesafeyi okuyun ve buzzer'ı çalın
  //mesafeBuzzer(); // Yeni fonksiyon adını kullanın

  // Belirli bir süre bekleyin (saniye cinsinden)
  delay(100); // 10 saniye
}
