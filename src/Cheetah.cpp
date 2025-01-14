#include "Cheetah.h"

CheetahSerial::CheetahSerial(uint16_t msg_size)
{
  this->contA = 1;
  this->contD = 0;
  this->vel = 30;
  memset(this->payload , 0 , MSG_SIZE);
  memset(this->digital , 0 , N_SENSORES_DISCRETO);
}

void CheetahSerial::addAnalogSensor(uint16_t value)
{
  this->payload[contA] =  highByte(value);
  this->payload[contA+1] = lowByte(value);
  this->contA += 2;
}

void CheetahSerial::addDigitalSensor(uint16_t value)
{
  this->digital[contD] = value;
  this->contD++;
}

void CheetahSerial::sendPayload()
{
  int temp[N_SENSORES_DISCRETO/8];
  for(int i = 0 ; i < N_SENSORES_DISCRETO/8 ; i++)
  {
    int aux = 0;
    int bin = 1;
    for(int j = i*8 + 7 ; j >= i*8 ; j--)
    {
      aux += this->digital[j] * bin;
      bin *=2;
    }
    temp[i] = aux;
  }

  int cont = 0;
  for (int i = 1 + (N_SENSORES_MEDICAO * 2) ; i < MSG_SIZE-1 ; i++)
  {
    this->payload[i] = temp[cont];
    cont++;
  }
  //this->payload[5] = 255;

  this->contA = 1;
  this->contD = 0;

  this->payload[0] = 255;
  this->payload[MSG_SIZE-1] = 254;
  Serial.write(this->payload, MSG_SIZE);
}

void CheetahSerial::modoTeste()
{
  int qtdTeste = 2;
  for(int th = 0 ; th < qtdTeste ; th++)
  {
    if(this->subindo)
    {
      this->vel+=1;
      if(this->vel>4096)
        this->subindo = false;
    }
    else
    {
      this->vel-=1;
      if(this->vel<1)
        this->subindo = true;
      for(int i = 1 ; i < MSG_SIZE - 1 ; i++)
      {
        this->payload[i] = random(0,255);
      }
    }
  }
  this->payload[1] = highByte(this->vel);
  this->payload[2] = lowByte(this->vel);
  this->payload[3] = highByte(this->vel);
  this->payload[4] = lowByte(this->vel);
  this->payload[5] = highByte(this->vel);
  this->payload[6] = lowByte(this->vel);
  this->payload[7] = highByte(this->vel);
  this->payload[8] = lowByte(this->vel);
  this->payload[9] = highByte(this->vel);
  this->payload[10] = lowByte(this->vel);
  this->payload[11] = highByte(this->vel);
  this->payload[12] = lowByte(this->vel);
  this->payload[13] = highByte(this->vel);
  this->payload[14] = lowByte(this->vel);
  this->payload[15] = highByte(this->vel);
  this->payload[16] = lowByte(this->vel);
  this->payload[17] = highByte(this->vel);
  this->payload[18] = lowByte(this->vel);
  this->payload[19] = highByte(this->vel);
  this->payload[20] = lowByte(this->vel);
}


Acelerometro::Acelerometro()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);

  memset(variaveis, 0, sizeof(variaveis));
}

uint16_t* Acelerometro::leituraVariaveis()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);

  for(int i = 0; i < 7; i++)
  {
    variaveis[i] = Wire.read()<<8|Wire.read();
  }

  return variaveis; 
  
}
