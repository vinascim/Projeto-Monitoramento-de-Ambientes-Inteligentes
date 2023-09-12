


//incluindo bibliotecas
#include <EEPROM.h>
#include <dht.h>
#include <LiquidCrystal.h>
#include <Wire.h> 
#include "RTClib.h" 
RTC_DS3231 rtc; 
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

dht DHT;

//declarando as variaveis
const int pinoDHT11 = A0;
float mediaTemp = 0;
float mediaUmid = 0;
float mediaLumi = 0;
int contadorMedia = 0;
String logEEPROM = "";
int pinoLed = 7;
int pinoBuzzer = 8; 

 
void setup(){
  //iniciando o rtc 
  if(! rtc.begin()) { 
    Serial.println("DS3231 não encontrado");
    while(1); 
  }
  //iniciando o lcd e limpando para garantir que nao apareça nada
  lcd.begin(16,2);              
  lcd.clear();
  
  //pegando a data atual pelo rtc
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //printando na segunda linha do lcd uma mensagem de inicio
  lcd.setCursor(0,1);
  lcd.print("Analisando...");

  //indicando para o arduino se a entrada e output ou input
  pinMode(pinoBuzzer, INPUT); 
  pinMode(pinoLed, OUTPUT);
  
  //iniciando o console
  Serial.begin(9600);
}
 
void loop(){
  
  //pegando a data atual
  DateTime dt = rtc.now(); 
                 
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
