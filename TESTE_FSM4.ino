boolean SNFC, IDREQ, CABLE_CONNECTED, CABLE_ERROR, NFC_OK, NFC_NOK, WAIT_IR = false;
boolean newData = false;
String state_check, rasp_resp;

void setup() {
  
   Serial.begin(9600);
   while(!Serial){}
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
  WAIT_IR = false;
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
      Serial.println("Aguardando aproximação de veículo...");
    }
      else  {
        if (SNFC == true) {   
          Serial.println("SNFC");
        }
          else {      
            if(IDREQ == true) {
              Serial.println("D6 D7 D8 DF");
              
              /////* Espera resposta serial *//////
              
              if(NFC_OK == true) {
                Serial.println("Cartão OK - Aguardando mecanismo");
              }
                else
                { 
                  if(NFC_NOK == true) {
                  Serial.println("Cartão NOK - Reniciando o mecanismo");
                  fsm_rst();
                  }
                }
              }
                else {
                  if(CABLE_CONNECTED == true) {
                    Serial.println("Desconecte o cabo");
                    delay(500);
                  }
                }
              }
        }
  }
}
