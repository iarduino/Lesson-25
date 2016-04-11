// Библиотека iarduino_IR для работы arduino с ИК приёмопередатчиками, разработана интернет магазином http://iarduino.ru
// Приём данных с ИК-пультов осуществляется через Trema ИК-приёмник http://iarduino.ru/shop/Sensory-Datchiki/ik-priemnik-trema-modul.html
// Для удобства подключения предлагаем воспользоваться Trema Shield http://iarduino.ru/shop/Expansion-payments/trema-shield.html
// Подключение каждого канал RGB светодиодной ленты осуществляется через Trema ключ http://iarduino.ru/shop/Expansion-payments/silovoy-klyuch-trema-modul.html
// Управление осуществляется с помощью ИК-пульта http://iarduino.ru/shop/Sensory-Datchiki/infrakrasnyy-pult-distancionnogo-upravleniya-priemnik-hx1838.html

// Подключаем библиотеку:
#include <iarduino_IR.h>                             // подключаем библиотеку для работы с ИК приёмником

// Объявляем переменные и константы:
iarduino_IR   IR(7);                                 // объявляем переменную для работы с ИК приёмником (номер цифрового вывода к которому подключён ИК приёмник = 7)
const uint8_t PIN_LED_R   = 6;                       // указываем номер вывода arduino, к которому подключён Trema ключ, управляющий R каналом светодиодной ленты
const uint8_t PIN_LED_G   = 9;                       // указываем номер вывода arduino, к которому подключён Trema ключ, управляющий G каналом светодиодной ленты
const uint8_t PIN_LED_B   = 5;                       // указываем номер вывода arduino, к которому подключён Trema ключ, управляющий B каналом светодиодной ленты
      uint8_t VAL_LED_R   = 0xFF;                    // объявляем переменную содержащую значение яркости R канала светодиодной ленты
      uint8_t VAL_LED_G   = 0xFF;                    // объявляем переменную содержащую значение яркости G канала светодиодной ленты
      uint8_t VAL_LED_B   = 0xFF;                    // объявляем переменную содержащую значение яркости B канала светодиодной ленты
      uint8_t VAL_LIGHT   = 0xFF;                    // максимальная яркость для каждого канала RGB
unsigned long MAS_PLAY[6] = {5,0,0,0,0,0};           // массив для автоперелива {задержка в мс, время старта, тип, сектор, позиция, яркость}
      bool    FLAG_ON     = 0;                       // флаг вкл/выкл
      bool    FLAG_RGB    = 1;                       // флаг указывающий о необходимости изменить значения ШИМ

void setup(){
  IR.begin();                                        // инициируем ИК приёмник
}

void loop(){
// Читаем команды с ИК-пульта (если таковые есть)
  if(IR.check(true)){ if(FLAG_ON){ switch(IR.data){  // если принят пакет с пульта (включая пакеты повторов), если устройство включено, проверяем какая кнопка пульта нажата
    /* <<   = перелив 1 */ case 0xFF22DD: if(IR.key_press){MAS_PLAY[2]=MAS_PLAY[2]==1?0:1; MAS_PLAY[1] = millis();} break;
    /* >>   = перелив 2 */ case 0xFF02FD: if(IR.key_press){MAS_PLAY[2]=MAS_PLAY[2]==2?0:2; MAS_PLAY[1] = millis();} break;
    /* >||  = перелив 3 */ case 0xFFC23D: if(IR.key_press){MAS_PLAY[2]=MAS_PLAY[2]==3?0:3; MAS_PLAY[1] = millis();} break;
    /* CH+  = яркость + */ case 0xFFE21D:                FLAG_RGB=1; VAL_LIGHT=VAL_LIGHT<<1|1; break;
    /* -    = R канал + */ case 0xFFE01F: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=VAL_LED_R<<1|1; break;
    /* +    = G канал + */ case 0xFFA857: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_G=VAL_LED_G<<1|1; break;
    /* EQ   = B канал + */ case 0xFF906F: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_B=VAL_LED_B<<1|1; break;
    /* CH   = яркость - */ case 0xFF629D:                FLAG_RGB=1; VAL_LIGHT=VAL_LIGHT>>1;   break;
    /* 0    = R канал - */ case 0xFF6897: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=VAL_LED_R>>1;   break;
    /* 100+ = G канал - */ case 0xFF9867: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_G=VAL_LED_G>>1;   break;
    /* 200+ = B канал - */ case 0xFFB04F: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_B=VAL_LED_B>>1;   break;
    /* 1    = красный   */ case 0xFF30CF: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0xFF; VAL_LED_G=0x00; VAL_LED_B=0x00; MAS_PLAY[3]=0; MAS_PLAY[4]=0x00; break;
    /* 2    = оранжевый */ case 0xFF18E7: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0xFF; VAL_LED_G=0x0F; VAL_LED_B=0x00; MAS_PLAY[3]=0; MAS_PLAY[4]=0x7F; break;
    /* 3    = жёлтый    */ case 0xFF7A85: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0xFF; VAL_LED_G=0x3F; VAL_LED_B=0x00; MAS_PLAY[3]=1; MAS_PLAY[4]=0x00; break;
    /* 4    = синий     */ case 0xFF10EF: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0x00; VAL_LED_G=0x00; VAL_LED_B=0xFF; MAS_PLAY[3]=4; MAS_PLAY[4]=0x00; break;
    /* 5    = голубой   */ case 0xFF38C7: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0x00; VAL_LED_G=0xFF; VAL_LED_B=0xFF; MAS_PLAY[3]=3; MAS_PLAY[4]=0x00; break;
    /* 6    = зелёный   */ case 0xFF5AA5: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0x00; VAL_LED_G=0xFF; VAL_LED_B=0x00; MAS_PLAY[3]=2; MAS_PLAY[4]=0x00; break;
    /* 7    = фиолетовый*/ case 0xFF42BD: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0xFF; VAL_LED_G=0x00; VAL_LED_B=0xFF; MAS_PLAY[3]=5; MAS_PLAY[4]=0x00; break;
    /* 8    = белый     */ case 0xFF4AB5: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0xFF; VAL_LED_G=0xFF; VAL_LED_B=0xFF; MAS_PLAY[3]=0; MAS_PLAY[4]=0x00; break;
    /* 9    = чёрный    */ case 0xFF52AD: MAS_PLAY[2]=0; FLAG_RGB=1; VAL_LED_R=0x00; VAL_LED_G=0x00; VAL_LED_B=0x00; MAS_PLAY[3]=0; MAS_PLAY[4]=0x00; break;
  }}/* CH-  = вкл/выкл  */ if(IR.data==0xFFA25D && IR.key_press){FLAG_RGB=1; FLAG_ON=FLAG_ON?0:1;}} // если нажата кнопка пульта вкл/выкл и она не удерживается
// Запускаем функцию перелива цвета (если таковая включена)
  if(MAS_PLAY[2]&&FLAG_ON){Func_color();}
// Выводим ШИМ для каждого канала RGB (если требуется изменить ШИМ хотя бы для одного из каналов)
  if(FLAG_RGB){ FLAG_RGB=0;                                                                         // сбрасываем флаг указывающий о необходимости изменить значения ШИМ
    analogWrite(PIN_LED_R, FLAG_ON?map(VAL_LED_R,0,0xFF,0,MAS_PLAY[2]==1?MAS_PLAY[5]:VAL_LIGHT):0); // выводим ШИМ с указанием яркости R канала светодиодной ленты
    analogWrite(PIN_LED_G, FLAG_ON?map(VAL_LED_G,0,0xFF,0,MAS_PLAY[2]==1?MAS_PLAY[5]:VAL_LIGHT):0); // выводим ШИМ с указанием яркости G канала светодиодной ленты
    analogWrite(PIN_LED_B, FLAG_ON?map(VAL_LED_B,0,0xFF,0,MAS_PLAY[2]==1?MAS_PLAY[5]:VAL_LIGHT):0); // выводим ШИМ с указанием яркости B канала светодиодной ленты
  }
}

// функция автоперелива цвета
void Func_color(){
  if(MAS_PLAY[1]>millis()){MAS_PLAY[1]=0; delay(MAS_PLAY[0]);}                                       // произошло переполнение micros()
  if(MAS_PLAY[1]<millis()-MAS_PLAY[0]){ MAS_PLAY[1]=millis();                                        // если прошло MAS_PLAY[0] мс после последнего изменения ШИМ
    switch(MAS_PLAY[3]){                                                                             //                                                R G B     R G B
      case 0: VAL_LED_B=0x00; VAL_LED_R=0xFF; VAL_LED_G=MAS_PLAY[2]!=2?0xFF:     MAS_PLAY[4]; break; // сектор автоперелива от красного к жёлтому     (FF0000 -> FFFF00)
      case 1: VAL_LED_B=0x00; VAL_LED_G=0xFF; VAL_LED_R=MAS_PLAY[2]!=2?0x00:0xFF-MAS_PLAY[4]; break; // сектор автоперелива от жёлтого  к зелёному    (FFFF00 -> 00FF00)
      case 2: VAL_LED_R=0x00; VAL_LED_G=0xFF; VAL_LED_B=MAS_PLAY[2]!=2?0xFF:     MAS_PLAY[4]; break; // сектор автоперелива от зелёного к голубому    (00FF00 -> 00FFFF)
      case 3: VAL_LED_R=0x00; VAL_LED_B=0xFF; VAL_LED_G=MAS_PLAY[2]!=2?0x00:0xFF-MAS_PLAY[4]; break; // сектор автоперелива от голубого к синему      (00FFFF -> 0000FF)
      case 4: VAL_LED_G=0x00; VAL_LED_B=0xFF; VAL_LED_R=MAS_PLAY[2]!=2?0xFF:     MAS_PLAY[4]; break; // сектор автоперелива от синего к фиолетовому   (0000FF -> FF00FF)
      case 5: VAL_LED_G=0x00; VAL_LED_R=0xFF; VAL_LED_B=MAS_PLAY[2]!=2?0x00:0xFF-MAS_PLAY[4]; break; // сектор автоперелива от фиолетового к красному (FF00FF -> FF0000)
    }
    if(MAS_PLAY[4]<0xFF){MAS_PLAY[4]++;}else{MAS_PLAY[4]=0; MAS_PLAY[3]++;}                          // увеличиваем шаг и сектор автоперелива цвета
    if(MAS_PLAY[3]>=6){MAS_PLAY[3]=0;}                                                               // переходим от 5 к 0 сектору
    if(MAS_PLAY[2]==1){MAS_PLAY[5]=MAS_PLAY[4]>127?map(MAS_PLAY[4],127,255,VAL_LIGHT,1):map(MAS_PLAY[4],0,127,1,VAL_LIGHT);} // затухание и возрастание
    FLAG_RGB=1;                                                                                      // устанавливаем флаг указывающий о необходимости изменить значения ШИМ
  }
}
