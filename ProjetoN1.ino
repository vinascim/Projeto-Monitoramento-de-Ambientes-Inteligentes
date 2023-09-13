


//incluindo bibliotecas
#include <EEPROM.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <Wire.h> 
#include "RTClib.h" 
RTC_DS3231 rtc; 
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

dht DHT;

int thisChar = 0 ;

//preparando a animação de inicialização
byte chardino1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B11000,
  B11100
};

byte chardino2[8] = {
  B00000,
  B00001,
  B00001,
  B00001,
  B00001,
  B00011,
  B01111,
  B11111
};
byte chardino3[8] = {
  B11111,
  B10111,
  B11111,
  B11111,
  B11100,
  B11111,
  B11100,
  B11100
};
byte chardino4[8] = {
  B10000,
  B11000,
  B11000,
  B11000,
  B00000,
  B10000,
  B00000,
  B00000
};
byte chardino5[8] = {
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00011,
  B00010,
  B00011
};
byte chardino6[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B10110,
  B00010,
  B00010,
  B00011
};

byte chardino7[8] = {
  B11111,
  B11001,
  B10000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


byte charcabeza1[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11100,
  B11111,
  B11100,
  B11100
};

byte charpieder1[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B10110,
  B00011,
  B00000,
  B00000
};

byte charpieder2[8] = {
  B11111,
  B11001,
  B10000,
  B00000,
  B00000,
  B10000,
  B00000,
  B00000
};

byte charpieiz1[8] = {
  B11111,
  B11111,
  B01111,
  B00111,
  B00011,
  B00011,
  B00000,
  B00000
};

byte charpieiz2[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B00110,
  B10010,
  B00010,
  B00011
};

//declarando as variaveis
const int pinoDHT11 = A0;
float mediaTemp = 0;
float mediaUmid = 0;
float mediaLumi = 0;
int contadorMedia = 0;
String logEEPROM = "";
int pinoLed = 7;
int pinoBuzzer = 8; 
int contTimer = 0;

 
void setup(){
  //iniciando o rtc 
  if(! rtc.begin()) { 
    Serial.println("DS3231 não encontrado");
    while(1); 
  }
  //iniciando o lcd e limpando para garantir que nao apareça nada
  lcd.begin(16,2);              
  lcd.clear();

  //indicando para o arduino se a entrada e output ou input
  pinMode(pinoBuzzer, INPUT); 
  pinMode(pinoLed, OUTPUT);
  
  //iniciando o console
  Serial.begin(9600);

 //chamando a função de animação
  letras();
  dinoAnimacion();

    //pegando a data atual pelo rtc
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
 
void loop(){
  
  //pegando a data atual
  DateTime dt = rtc.now(); 

 //contador de tempo para primeira analise
  contTimer++;

  if(contTimer < 60 ){
    lcd.setCursor(13, 1);
    lcd.print(contTimer);
    lcd.setCursor(15, 1);
    lcd.print("s");
  }
  
                 
  //declarando variaveis
  int sensorValue = analogRead(A1);    
  int luminosity = map(sensorValue, 0, 1023, 0, 100); 
  int humidity = DHT.humidity;
  int temp = DHT.temperature;
  DHT.read11(pinoDHT11); 

  //a cada 5 segundos soma os dados dos sensores numa variavel, adiciona um no contador, desliga o led e o buzzer
  if(dt.second() % 5 == 0){
    mediaTemp += temp;
    mediaUmid += humidity;
    mediaLumi += luminosity;
    contadorMedia++;
    noTone(pinoBuzzer);
    digitalWrite(pinoLed, LOW);
  }

  //quando o contador for igual a 12, calcula a media dos sensores e printa no lcd
  if(contadorMedia == 12){
    int mediaTempFinal = mediaTemp / contadorMedia;
    int mediaUmidFinal = mediaUmid / contadorMedia;
    int mediaLumiFinal = mediaLumi / contadorMedia;   

    contTimer = 65;
    lcd.clear(); 
    lcd.setCursor(0,0);           
    lcd.print("Lumi"); 
    lcd.setCursor(0,1);
    lcd.print(mediaLumiFinal);
    lcd.print("%"); 
    lcd.setCursor(6,0);           
    lcd.print("Umid");
    lcd.setCursor(6,1);  
    lcd.print(mediaUmidFinal); 
    lcd.print("%");  
    lcd.setCursor(12,0);           
    lcd.print("Temp");
    lcd.setCursor(12,1);  
    lcd.print(mediaTempFinal);
    lcd.write(B11011111);
    lcd.print("C");
    
    //verifica se as medias estao dentro dos parametros solicitados, se nao tiver salvar na memoria do arduino e ativa o alarme sonoro e o led
    if (!(mediaTempFinal > 15 && mediaTempFinal < 25) || !(mediaLumiFinal > 0 && mediaLumiFinal < 30) || !(mediaUmidFinal > 30 && mediaUmidFinal < 50)){
      logEEPROM = String("Data: ") + dt.day() + "/" + dt.month() + "/" + dt.year() + " " + dt.hour() + ":" + dt.minute() + ":" + dt.second()
      + String(" | Temperatura: ") + String(mediaTempFinal) 
      + String("°C | Luminosidade: ") + String(mediaLumiFinal) 
      + String("% | Umidade: ") + String(mediaUmidFinal) + "%";
        
      tone(pinoBuzzer, 1500);//Ligando o buzzer com uma frequência de 1500 Hz. 
      digitalWrite(pinoLed, HIGH);
      escreveString(0, logEEPROM);
      Serial.println(leString(0));
    }
    
    //zera as variaveis 
    contadorMedia = 0;
    mediaTemp = 0;
    mediaUmid = 0;
    mediaLumi = 0;
  }
  
  delay(1000); 
}

//funçao para escrever string na memoria do arduino
void escreveString(int enderecoBase, String mensagem){ 
  if (mensagem.length()>EEPROM.length() || (enderecoBase+mensagem.length()) >EEPROM.length() ){ 
    Serial.println ("A sua String não cabe na EEPROM"); 
  }
  else{  
    for (int i = 0; i<mensagem.length(); i++){ 
       EEPROM.write(enderecoBase,mensagem[i]); 
       enderecoBase++; 
    }
    EEPROM.write(enderecoBase,'\0');
  }
}


//funçao para ler o que tem salvo na memoria do arduino
String leString(int enderecoBase){
  String mensagem="";
  if (enderecoBase>EEPROM.length()){ 
    return mensagem;
  }
  else { 
    char pos;
    do{
      pos = EEPROM.read(enderecoBase); 
      enderecoBase++; 
      mensagem = mensagem + pos; 
    }
    while (pos != '\0'); 
  }
  return mensagem; 
}


void dinoAnimacion() 
{
  lcd.createChar(1,chardino1);
  lcd.createChar(2,chardino2);
  lcd.createChar(3,chardino3);
  lcd.createChar(4,chardino4);
  lcd.createChar(5,chardino5);
  lcd.createChar(6,chardino6);
  lcd.createChar(7,chardino7);
  
  lcd.setCursor(0,0);
  lcd.write(1);
  lcd.write(2);
  lcd.write(3);
  lcd.write(4);

  lcd.setCursor(0,1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);

  lcd.createChar(6,charpieder1);
  lcd.createChar(7,charpieder2);
  lcd.setCursor(0,1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);

  lcd.createChar(5,charpieiz1);
  lcd.createChar(6,charpieiz2);
  lcd.setCursor(0,1);
  lcd.write(5);
  lcd.write(6);
  lcd.write(7);
}

 void letras()
  {
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Ardudino");
      lcd.setCursor(4,1);
      lcd.print("<<EC-5>>");   
  }
