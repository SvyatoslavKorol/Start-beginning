/*
* 130922
* С. Новиков
* nsv47nsv36@yandex.ru
* Программа для умного мигания светодиодом
* Arduino, которое управляется с дисплея Nextion
*/

#include <SoftwareSerial.h>


#define rPIN  A5 // вводим переменную для подключения фоторезистора к ардуино


const uint8_t pin_softRX = 5;
const uint8_t pin_softTX = 9;

SoftwareSerial softSerial(pin_softRX,pin_softTX); 

// Массив режимов работы светодиода
byte modes[] = {
   0B00000000, //Светодиод выключен
   0B11111111, //Горит постоянно
   0B00001111, //Мигание по 0.5 сек
   0B00000001, //Короткая вспышка раз в секунду
   0B00000101, //Две короткие вспышки раз в секунду
   0B00010101, //Три короткие вспышки раз в секунду
   0B01010101  //Частые короткие вспышки (4 раза в секунду)
};

  

uint32_t ms, ms1 = 0, ms2 = 0;
uint8_t  blink_loop = 0;
uint8_t  blink_mode = 0;
uint8_t  modes_count = 0; 

void control_from_the_display(){

 /* pinMode(19, OUTPUT);  // D10 как выход
  pinMode(6, OUTPUT);  // A3 как выход
  pinMode(10, OUTPUT);  // A5 как выход (Nano/UNO)
  digitalWrite(19, LOW); // высокий сигнал на D10
  digitalWrite(6, LOW);    // высокий сигнал на A3
  digitalWrite(10, LOW);    // высокий сигнал на A5*/
  
	if(softSerial.available()>0){         // Если есть данные принятые от дисплея, то ...
		String str;                         // Объявляем строку для получения этих данных
		while(softSerial.available()){
			str+=char(softSerial.read());     // Читаем принятые от дисплея данные побайтно в строку str
			delay(10);
		}
		Serial.println(str);                
		for(int i=0; i<str.length(); i++){ // Проходимся по каждому символу строки str
			//----------------------------------------------
			if(memcmp(&str[i],"set_but_0" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
				i+=8; 
       
				blink_mode = modes[0];
				
			}else
			//----------------------------------------------
			if(memcmp(&str[i],"set_but_1" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
				i+=8; 
       
				blink_mode = modes[1];
			
			}else
			//----------------------------------------------
			if(memcmp(&str[i],"set_but_2" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
				i+=8; 
     
				blink_mode = modes[2];
			
			}
			//----------------------------------------------
      if(memcmp(&str[i],"set_but_3" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
        i+=8; 
       
        blink_mode = modes[3];
      }else
      //----------------------------------------------
      if(memcmp(&str[i],"set_but_4" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
        i+=8; 
            
        blink_mode = modes[4];
      }else
      //----------------------------------------------
      if(memcmp(&str[i],"set_but_5" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
        i+=8; 
      
        blink_mode = modes[5];
         }else
      //----------------------------------------------
      if(memcmp(&str[i],"set_but_6" , 9)==0){ // Если в строке str начиная с символа i находится текст "movingUp",  значит кнопка дисплея была включена
        i+=8; 
      
        blink_mode = modes[6];
      
      }
		}
   

	}
}

void control_for_illumination(){

      delay(1000);
      int svet = analogRead(rPIN); //считывание и запись значения датчика освещености
     // String str1 = "t0.txt=";
      //str1 += svet;
      //Serial.println(str1);
      softSerial.print((String)"t0.txt=\""+analogRead(rPIN)+"\""+char(255)+char(255)+char(255));
      // если на фоторезистор не падает свет
      if(svet < 300)
      {
        digitalWrite(2, HIGH); // то включаем светодиод
      } 
      // если на фоторезистор падает свет
      if(svet >= 400)
      {
        digitalWrite(2, LOW); // то выключаем светодиод

      } 
  
      
    }

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW); 
  modes_count = 0;
  blink_mode = modes[modes_count];
  
  Serial.begin(9600);
  
  softSerial.begin(9600);

  pinMode(2, OUTPUT); // активируем светодиод на вывод
}

void loop() {
	control_from_the_display();
  		
	ms = millis();
	// Событие срабатывающее каждые 125 мс   
	if( ( ms - ms1 ) > 125|| ms < ms1 ){
		ms1 = ms;
	// Режим светодиода ищем по битовой маске       
	if(blink_mode & 1<<(blink_loop&0x07)) 
		digitalWrite(13, HIGH); 
	else  digitalWrite(13, LOW);
		blink_loop++;  

    control_for_illumination();


}
    
// Этот код служит для демонстрации переключения режимов    
// Один раз в 5 секунд меняем эффект  
/* 
   if( ( ms - ms2 ) > 5000|| ms < ms2 ){
       ms2 = ms;
       blink_mode = modes[modes_count++];
       if( modes_count >= 7 )modes_count = 1;
   }
*/   
}
