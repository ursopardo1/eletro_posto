#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define relay 4
#define cable_pin 8
#define prox_sensor 9

LiquidCrystal_I2C lcd(0x27,16,2);

const int yell_led = 5, red_led = 6, green_led = 7;
boolean SNFC, IDREQ, CABLE_CONNECTED, CABLE_ERROR, NFC_OK, NFC_NOK, WAIT_IR = false;
boolean newData = false;
String state_check, rasp_resp;

void setup() {
  
    pinMode(relay, OUTPUT);
    pinMode(prox_sensor, OUTPUT);
    pinMode(cable_pin, INPUT);
    digitalWrite(relay, LOW);
    Serial.begin(9600);
    while(!Serial){}

    lcd.init();
    fsm_rst();
}

void loop() { 
  
  //Serial.println("Estou no LOOP!");
  if(Serial.available()) {
     while(Serial.available() > 0 && newData == false) {
       state_check = Serial.readString();
       fsm();
       newData = true;
     }
  }
  fsm_out();
  
}

////////////////* MÁQUINA DE ESTADOS *///////////////////////

void fsm_rst() {   
  
  Serial.println("Executando RST");
  state_check == "0";
  WAIT_IR = true;
  SNFC = false;
  IDREQ = false;
  NFC_OK = false;
  NFC_NOK = false;
  CABLE_CONNECTED = false;
  CABLE_ERROR = false;
  fsm_out();
  
}

void fsm() { 
  
  //Serial.println("Atualizando a FSM");
  Serial.println(state_check);
  if(state_check == "1") // SNFC SISTEMA DO RFCARD ATIVADO
  {
    WAIT_IR = false;
    SNFC = true;
    IDREQ = false;
    NFC_OK = false;
    NFC_NOK = true;
    CABLE_CONNECTED = false;
    CABLE_ERROR = false;
  }
    else {
      if(state_check == "2")  // IDREQ ENVIO DO ID E CHECK DA WHITE_LIST
      {
        WAIT_IR = false;
        SNFC = false;
        IDREQ = true;
        NFC_OK = false;
        NFC_NOK = false;
        CABLE_CONNECTED = false;
        CABLE_ERROR = false; 
      }
        else {
          if(state_check == "3") //  NFC-OK CARTÃO LIBERADO
          {
            WAIT_IR = false;
            SNFC = false;
            IDREQ = true;
            NFC_OK = true;
            NFC_NOK = false;
            CABLE_CONNECTED = false;
            CABLE_ERROR = false;   
          }
            else {
              if(state_check == "4") // NFC-NOK CARTÃO NÃO LIBERADO
              {
                WAIT_IR = false;
                SNFC = false;
                IDREQ = true;
                NFC_OK = false;
                NFC_NOK = true;
                CABLE_CONNECTED = false;
                CABLE_ERROR = false;
              }
                else {
                  if(state_check == "5") // CABLE_CONNECTED CABO CONECTADO
                  {
                    WAIT_IR = false;
                    SNFC = false;
                    IDREQ = false;
                    NFC_OK = false;
                    NFC_NOK = false;
                    CABLE_CONNECTED = true;
                    CABLE_ERROR = false;
                  }
                    else {
                      if(state_check == "0")
                      {
                        WAIT_IR = true;
                        SNFC = false;
                        IDREQ = false;
                        NFC_OK = false;
                        NFC_NOK = false;
                        CABLE_CONNECTED = false;
                        CABLE_ERROR = false;
                      }
                    }
                }
              }
          }
    }
}

void fsm_out() {
  
  //Serial.println("Executando Saídas");
  if(newData == true){
    newData = false;
      if (WAIT_IR == true) {
      Serial.println("WAIT_IR");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Aguardando IR");
      lcd.setBacklight(LOW);
      }
      
      if (SNFC == true) {   
      Serial.println("SNFC");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Aprox. o card");
      delay(3000);
      lcd.setBacklight(LOW);
      }
      
      if(IDREQ == true) {
      Serial.println("IDREQ");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("RFID:");
      lcd.setCursor(0,1);
      lcd.print("D6 D7 D8 DF");
      delay(3000);
      lcd.setBacklight(LOW); 
      //** X **//
      }
      
      if(NFC_OK == true) {
      Serial.println("NFC_OK");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cartão OK");
      lcd.setCursor(0,1);
      lcd.print("Liberando Mec.");
      delay(3000);
      lcd.setBacklight(LOW); 
      }
      
      if(NFC_NOK == true) {
      Serial.println("NFC_NOK");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cartão nOK");
      lcd.setCursor(0,1);
      lcd.print("Reiniciando Mec.");
      delay(3000);
      lcd.setBacklight(LOW); 
      fsm_rst();
      }
      
      if(CABLE_CONNECTED == true) {
      Serial.println("CABLE_CONNECTED");
      lcd.setBacklight(HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Mec. Ativado");
      lcd.setCursor(0,1);
      lcd.print("Desconecte cabo");
      delay(3000);
      lcd.setBacklight(LOW); 
      delay(500);
      }
  }
}      
