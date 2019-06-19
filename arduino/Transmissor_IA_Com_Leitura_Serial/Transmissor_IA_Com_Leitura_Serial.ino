#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define CE 7 //define o pino 7 como CE
#define CSN 8 //define o pino 8 como CS

RF24 radio(CE, CSN); // Define quais serão os pinos do arduino usados para conectar o CE e o CS no rádio

uint8_t dados[8];
int tx_dados[7];
int i;
//dado={id,velocidade1,velocidade2,velocidade3,velocidade4,power,dribler}

const uint64_t pipe[6] = {0xF1F1F1F1E1LL, 0xF0F0F0F0E2LL, 0xF0F0F0F0E3LL, 0xF0F0F0F0E4LL, 0xF0F0F0F0E5LL, 0xF0F0F0F0E6LL};  // Endereços do radio
uint8_t c;

int cont;
void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setChannel(73);
  radio.openWritingPipe(pipe[1]); // Começa a enviar msg para os robos

}

void loop()
{
  if(Serial.available()) {
    c = Serial.read();
    //Serial.println(c, DEC);
    if(c >= 128) {
      //Serial.println(c, DEC);
      cont = 1;
      dados[0] = c - 128;
    }
    else if (cont > 0) {
      dados[cont] = c;
      cont++;
    }
    if (cont == sizeof(dados)) {
      cont = 0;
      tx_dados[0] = dados[0];
      tx_dados[5] = dados[6];
      tx_dados[6] = dados[7];

      
      tx_dados[1] = dados[1] | (uint8_t) (dados[5] << 7);
      tx_dados[2] = dados[2] | (uint8_t) ((dados[5] >> 1) << 7);
      tx_dados[3] = dados[3] | (uint8_t) ((dados[5] >> 2) << 7);
      tx_dados[4] = dados[4] | (uint8_t) ((dados[5] >> 3) << 7);

      for(int i = 0; i<7;i++) {
        Serial.print("DADO ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(tx_dados[i], DEC);
      }

            
      radio.write(&tx_dados, sizeof(tx_dados));
    }
  }
  /*for (i = 1; i < 5; i++) {
    dado[i] = map(Dado[i], -3000, 3000, 0, 255);
  }
  Serial.println("Começa aqui");
  for (int cont = 0; cont < 7; cont++)
  {
    Serial.println(dado[cont]);
  }
  radio.write( &dado, sizeof(dado)); // Envia as informações para o rádio*/
}
