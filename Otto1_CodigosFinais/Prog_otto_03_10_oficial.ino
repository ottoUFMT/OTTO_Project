  //1 uva- 2 peixe- 3 g - 4 sono- teclado 0-5 1-6  2-7 3-8 4-9 5-10 6-11 7-12 8-13 9-14/ 15- liga  16-desliga  17- acertou 18- errou  19-dança codigo utilizado dentro do otto 02/08
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include "Arduino.h"
#include <Keypad.h>
#include <IRremote.h>
#include <avr/wdt.h>
#include "LedControl.h"
#include <Servo.h>

  boolean play_state;
  int posx = 0;
  
  Servo FL, FR; //servos pé
  Servo AL, AR; //servos braços
  
  #define LeftFoot 2 // left foot pin esquerda pé
  #define RightFoot 3 // right foot pin direita pé
  
  #define PIN_AL 6 // left arm pin braço esquerdo
  #define PIN_AR 7 // right arm pin braço direito
  
  #define RST_PIN         5           // RFID
  #define SS_PIN          53          // RFID

//   * Pinagem RFID:
// * --------------------------------------
// *             MFRC522         Arduino   
// *             Reader/PCD      Mega      
// * Signal      Pin             Pin       
// * --------------------------------------
// * RST/Reset   RST             5         
// * SPI SS      SDA(SS)         53        
// * SPI MOSI    MOSI            51        
// * SPI MISO    MISO            50        
// * SPI SCK     SCK             52        

  SoftwareSerial mySoftwareSerial(10, 11);   // RX, TX (Resistor)  //pino DFPLAYE-
  DFRobotDFPlayerMini myDFPlayer;
  MFRC522 mfrc522(SS_PIN, RST_PIN);          // RFID
  MFRC522::MIFARE_Key key;
  
  
  const byte  RECV_PIN = 30;       // pino infravermelho
  IRrecv irrecv(RECV_PIN);


   // attach ->  ativa o pino dos servos, não usar a função se só for utilizar apenas um servo no movimento, ativar na hora de uso
  void attachF() {      //atach pes
    FL.attach(LeftFoot);
    FR.attach(RightFoot);
  }
  
  void attachA() {      //attach braços
    AL.attach(PIN_AL);
    AR.attach(PIN_AR);
  }
   void attachAR() {      //attach braços
   AL.attach(PIN_AL);
    AL.detach();
    AR.attach(PIN_AR);
  }
  // detachall -> desativa os pinos, ele desassocia o pino dos servos, então se utilizado, quando for fazer um movimento temos que usar o attach para associar os pinos nos servos. 
  void detachall() {
    AR.detach();
    AL.detach();
    FR.detach();
    FL.detach();
  }

  String Song = "";    // string recebida
  int play = 0;        // numero musica


  int estadobotao = 0; // matriz botão ligada ou nao
  int estadoliga = 0;  // robo ligado ou nao
  

  const byte LINHAS = 2; // n. Linhas do teclado
  const byte COLUNAS = 5; // n. Colunas do teclado
  
  //cinza, roxo (placa teclado)
  const byte PINOS_LINHAS[LINHAS] = {40, 38}; // Pinos de conexao com as linhas do teclado
  //vermelho, laranja, amarelo, marrom, branco (placa teclado)
  const byte PINOS_COLUNAS[COLUNAS] = {42, 44, 46, 48, 49}; // Pinos de conexao com as colunas do teclado
  
  const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
    {'0', '1', '2', '3', '4'},
    {'5', '6', '7', '8', '9'}
  };

  Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado

#define NBR_MTX 1
  LedControl lc = LedControl(22, 24, 26, NBR_MTX); // DIN, CLK, CS NRDEV pino LED


byte Sorriso[] = {
  B01111000,
  B01101100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01101100,
  B01111000
};

byte Triste[] = {
  B00000110,
  B00011000,
  B00110000,
  B00100000,
  B00100000,
  B00110000,
  B00011000,
  B00000110
};
byte vergonha[] = {
  B00010000,
  B00011000,
  B00001000,
  B00000100,
  B00000100,
  B00001000,
  B00011000,
  B00010000
};

byte Surpreso[] = {
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
};
byte cansado[] = {
  B00001000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000
};

byte sono0[] = {
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000
};
byte sono1[] = {
  B00000000,
  B00000000,
  B00011000,
  B00100100,
  B00100100,
  B00011000,
  B00000000,
  B00000000
};
byte sono2[] = {
  B00111100,
  B01000010,
  B11000011,
  B10000001,
  B10000001,
  B11000011,
  B01000010,
  B00111100
};
byte neutro[] = {
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000,
  B00010000
};
byte vazio[] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};
byte heart[] = {
  B01110000,
  B11111000,
  B01111100,
  B00111110,
  B00111110,
  B01111100,
  B11111000,
  B01110000
};

byte bocaAbre1[] = {
  0b00010000,
  0b00101000,
  0b00101000,
  0b00101000,
  0b00101000,
  0b00101000,
  0b00101000,
  0b00010000
};

byte bocaAbre2[] {
  0b00010000,
  0b00101000,
  0b01000100,
  0b01000100,
  0b01000100,
  0b01000100,
  0b00101000,
  0b00010000
};
byte medo[] = {
  B00010000,
  B00001000,
  B00010000,
  B00001000,
  B00010000,
  B00001000,
  B00010000,
  B00001000
};
byte raiva[] = {
  B00011110,
  B00101001,
  B01001001,
  B01001010,
  B01001010,
  B01001001,
  B00101001,
  B00011110
};

byte Barulho[] = {
  B00111100,
  B01111100,
  B11111100,
  B11111100,
  B11111100,
  B11111100,
  B01111100,
  B00111100
};

byte Enjoado[] = {
  0b00011000,
  0b00011000,
  0b00011000,
  0b10011001,
  0b10011001,
  0b11111111,
  0b11111111,
  0b00000000
};


void setup() {

  lc.shutdown(0, false);
  lc.setIntensity(0, 7);
  lc.clearDisplay(0);
  avazio();

  // Comunicacao serial com o modulo
  mySoftwareSerial.begin(9600);
  //Inicializa a serial do Arduino
  irrecv.enableIRIn();
  Serial.begin(9600); // Inicia a serial 9600
  SPI.begin();        // Inicia  SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("A");

  // Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  // Verifica se o modulo esta respondendo e se o
  // cartao SD foi encontrado
  Serial.println(F("Inicializando modulo DFPlayer Mini (3~5 segundos)"));
  if (!myDFPlayer.begin(mySoftwareSerial))
  {
    Serial.println(F("Nao inicializado:"));
    Serial.println(F("1.Cheque as conexoes do DFPlayer Mini"));
    Serial.println(F("2.Insira um cartao SD"));
    while (true);
  }
  Serial.println();
  Serial.println(F("Modulo DFPlayer Mini inicializado!"));

  // Definicoes iniciais
  myDFPlayer.setTimeOut(500); // Timeout serial 500ms
  myDFPlayer.volume(25);      // Volume 0 a 30
  myDFPlayer.EQ(0);           // Equalizacao normal
  //myDFPlayer.play(1);

  // Mostra o menu de comandos
  Serial.println();
  Serial.print("Numero de arquivos no cartao SD: ");
  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
}


void modo_leitura() {
  if (!mfrc522.PICC_ReadCardSerial())
  {
    Serial.print("B");
    return;
  }

  // Obtem os dados do setor 0, bloco 1
  byte sector = 0;
  byte blockAddr = 1;
  byte trailerBlock = 3;
  byte status;
  byte buffer[18];
  byte size = sizeof(buffer);

  // Autenticacao usando chave A
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));


  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  status = mfrc522.MIFARE_Read(blockAddr, buffer, &size);

  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Song = "";
  for (byte i = 0; i < 16; i++) {
    char sound = char(buffer[i]);
    String rc(sound);
    Song += rc;
  }
  play = Song.toInt();

  if ((83 < play && play < 92) || (106 < play && play < 109)) { // parte onde os audios estão
    switch (play) {
      case 90://barulho
        myDFPlayer.play(play);
        abarulho();
        delay(1000); //boca errada e de cabeça para baixo
        aneutro();
        break;
      case 108://cansada
        myDFPlayer.play(play);
        acansado();
        delay(1000); //boca nada a ver
        aneutro();
        break;
      case 87://medo
        myDFPlayer.play(play);
        amedo();
        delay(1000); //ok
        aneutro();
        break;
      case 89:// raiva
        myDFPlayer.play(play);
        araiva();
        mraiva();//ok
        aneutro();
        break;
      case 86:// veergonha
        myDFPlayer.play(play);
        avergonha();
        delay(1000); // b oca nada a ver
        aneutro();
        break;
      case 91://enjoado
        myDFPlayer.play(play);
        aenjoado();
        delay(1000);// colocar para falar
        aneutro();
        break;
      case 85://feliz
        myDFPlayer.play(play);
        aSorriso();
        acertar();
        delay(500); //ok
        aneutro();
        break;
      case 95://te amo
        myDFPlayer.play(play);
        aHEART();
        delay(1000);
        aneutro();
        break;
       case 84://triste
        myDFPlayer.play(play);
        aTriste();
        errar();
        delay(500);
        aneutro();
        break;
      case 107://parabens
        myDFPlayer.play(play);
        acertar();
        delay(1000);
        FALAR();
        delay(500);
        acertar();
        delay(1000);
        FALAR();
        delay(500);
        acertar();
        delay(1000);
        FALAR();
        aneutro();
        break;
    }
  } else {
    Serial.println();
    Serial.println("Reproduzindo musica: ");
    Serial.print(Song);
    myDFPlayer.play(play);
    FALAR();
    aneutro();
  }

  delay(500);

  Serial.println();
  // Halt PICC
  mfrc522.PICC_HaltA();
  //Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  Serial.print("D");
  return;
}



//// funçoes mov de motores

// para entender o movimento, testar para ver se se começa no 0 ou 180. 
void braco1() {
  for (posx = 90; posx <= 180; posx += 1) { // vai de 0 a 180 - lembrando que se vc quiser diminuir o movimento alterar o 90 ou o 180
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(150 - posx);
    delay(8);                       // waits 15 ms for the servo to reach the position
  }
}

void aceno() { // ARRUMADO 29.08
  attachAR();
  
  AR.attach(PIN_AR); // ativando o pino
  //AL.attach(PIN_AL);
  delay(50);
  //  emerg();
  // quando colocamos -2 ou +2 lembrar que tem que ele vai mais rapido e pula etapas, se achar que o movimento está estranho utilizar 1 

  for (posx = 100; posx >= 50; posx -= 2) { // goes from 0 degrees to 180 degrees  // 150 e 0
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(13);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  //  emerg();
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(12);
  }
  //  emerg();
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(12);
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);
  }
  for (posx = 150; posx >= 50 ; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(12);
    AL.write(posx);
  }

  // emerg();
  detachall();
  posinicial();
}
void acertar() { // NÃOOO MEXER - ALTERADO 29.08
  //som acertar
  attachA();
  delay(50);
  for (posx = 20; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    delay(20);
    AR.write(posx);
    AL.write(180 - posx); // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write(180 - posx); // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  //  emerg();
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(8);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(8);
  }
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(8);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(8);
  }
  //  emerg();
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(12);
  }
  for (posx = 0; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);
    AL.write (180 - posx); // tell servo to go to position in variable 'pos'
    delay(12);
  }
  for (posx = 150; posx >= 50 ; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    AL.write(180 - posx);
    delay(12);
  }
  detachall();
  posinicial();
}
  
void errar() { //ARRUMADO 29.08
  attachA();
  delay(50);
  //som errar
  // emerg();
  for (posx = 150; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(180 - posx);
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  delay(2000);

  for (posx = 150; posx >= 50 ; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    AL.write(160 - posx);
    delay(12);
  }
  detachall();
  posinicial();
}

void perna1() {
  for (posx = 50; posx <= 180; posx += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  //emerg();
  for (posx = 180; posx >= 0; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
}
void perna2() {
  for (posx = 90; posx >= 0; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  // emerg();
  for (posx = 0; posx <= 90; posx += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
}
void perna1b() {
 for (posx = 0; posx <= 100; posx += 1) {
    FR.write (posx);
    delay (10);
  }
  for (posx = 120; posx >= 0; posx -= 1) {
    FR.write (posx);
    delay (10);
  }
}
void perna2b() {
  for (posx = 90; posx <= 150; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  //emerg();
  for (posx = 150; posx >= 0; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
}

void perna3() {
  FL.attach(LeftFoot);
  AL.attach(PIN_AL);
  delay(20);
  for (posx = 90; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 180; posx >= 0; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
  // emerg();
  for (posx = 180; posx >= 90; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  FL.detach();
  AL.detach();
  posinicial();
}
void posinicial(){
  attachA();
  delay (10);
  for (posx = 120; posx >= 84 ; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    AL.write(160 - posx);
    delay(12);
  }
  AL.detach();
  AR.detach();
  }
void perna4() {
  FL.attach(LeftFoot);
  delay(10);
  for (posx = 180; posx >= 30; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  // emerg();
  AR.attach(PIN_AR);
  delay(10);
  for (posx = 90; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
   for (posx = 0; posx <= 90; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AR.write(posx);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 180; posx += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
  
   for (posx = 180; posx >= 0; posx -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    FL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }

  // emerg();
 
  AR.detach();
  FL.detach();
}

void dancap1() {
  attachF();
  delay(100);
  //  emerg();
  perna1();
  //emerg();
  //perna2();
  // emerg();
  delay(100);
  perna1();
  //emerg();
  //perna2();
  //emerg();
  delay(100);
}
void dancap1b() {
  attachF();
  delay(100);
  //dancapesq();
  perna1b();
  //emerg();
  perna2b();
  //emerg();
  perna1b();
  //emerg();
  perna2b();
  //emerg();
  delay(100);
}
void dancabraco(){
   attachA();
  delay(50);
  for (posx = 90; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  }
void dancap2() { // mão
  attachA();
  delay(50);
  for (posx = 90; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  // emerg();
  for (posx = 180; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  //emerg();
  for (posx = 0; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  //emerg();
  for (posx = 180; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  //emerg();
  for (posx = 0; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(7);                       // waits 15 ms for the servo to reach the position
  } for (posx = 180; posx >= 90; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
  detachall();
}
void dancabraco1() { // mão
  attachA();
  delay(50);
  for (posx = 90; posx >= 0; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 0; posx <= 90; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
 
}
void dancap3() {
  detachall();
  delay(20);
  perna3();
  delay(15);
  //perna3();
  delay(15);
  perna4();
  delay(15);
  dancabraco();
  //perna4();
  delay(15);
   perna3();
 // dancabraco1();
  delay(15);

  //delay(20);
}
void dancapdire() { //sobe desce 2x perna  OK
  FR.attach(RightFoot);// ALTERAR ATTAC
 
   for (posx = 0; posx <= 180; posx += 2) {
    FR.write (posx);
    delay (10);
   }
   for (posx = 180; posx >= 0; posx -= 2) {
    FR.write (posx);
    delay (10);
  }
  
}

void dancapesq() { //sobe e chao perna esquerda 1x
 FL.attach(LeftFoot);

  for (posx = 180; posx >= 0; posx -= 1) {
    FL.write (posx); 
    delay (5);
  }
   for (posx = 0; posx <= 180; posx += 1) {
    FL.write (posx);
    delay (5);
  }
   for (posx = 180; posx >= 0; posx -= 1) {
    FL.write (posx); 
    delay (5);
  }
   for (posx = 0; posx <= 180; posx += 1) {
    FL.write (posx);
    delay (5);
  }
}

void dancapesq2() { //sobe e chao perna esquerda 1x
 FL.attach(LeftFoot);

  for (posx = 180; posx >= 0; posx -= 1) {
    FL.write (posx); 
    delay (5);
  }
   for (posx = 0; posx <= 180; posx += 1) {
    FL.write (posx);
    delay (5);

}
}
void danca3(){
  attachA();
  delay(20);
  for (posx = 90; posx >= 20; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
 
  FL.attach(LeftFoot);

  for (posx = 180; posx >= 0; posx -= 1) {
    FL.write (posx); 
    delay (5);
  }
   for (posx = 0; posx <= 180; posx += 1) {
    FL.write (posx);
    delay (5);

}
 for (posx = 0; posx <= 120; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  for (posx = 120; posx <= 50; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(160-posx);              // tell servo to go to position in variable 'pos'
    AR.write(posx);
    delay(10);                       // waits 15 ms for the servo to reach the position
  }
  
  
}
void TapF() { // DESCOBRIR
  AL.attach(PIN_AL);
  for (posx = 90; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
 dancapesq2();
  //    emerg();
  delay(10);
  
  for (posx = 180; posx >= 90; posx -= 2) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    AL.write(posx);              // tell servo to go to position in variable 'pos'
    delay(7);                       // waits 15 ms for the servo to reach the position
  }
  detachall();
}

  void parar()
  {
    wdt_enable(WDTO_15MS);
    delay(20);
    //    detachall();
    estadoliga = 0;
    estadobotao = 0;
    myDFPlayer.pause();
    Serial.println("parou");
  }
  void aSorriso() {
    for (int i = 0; i < 8; i++)  {
      lc.setRow(0, i, Sorriso[i]);
    }
  }
  
  void aTriste() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, Triste[i]);
    }
  }
  void aHEART()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, heart[i]);
    }
  }
  void aneutro()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, neutro[i]);
      Serial.println("otto fica com a boca parada");
    }
  }
  void asono()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, sono0[i]);
    }
    delay(700);
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, sono1[i]);
    }
    delay(700);
  
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, sono2[i]);
    }
    delay(700);
  
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, sono1[i]);
    }
    delay(700);
  
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, sono0[i]);
    }
    delay(1000);
  }
  void avazio()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, vazio[i]);
    }
  }
  
  void aenjoado()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, Enjoado[i]);
    }
  }
  
  void FALAR()
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, bocaAbre1[i]);
    }
    delay(500);
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, bocaAbre2[i]);
    }
    delay(500);
  
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0, i, bocaAbre1[i]);
    }
    delay(700);
  }
  void aSurpreso() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, Surpreso[i]);
    }
  }
  void amedo() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, medo[i]);
    }
  }
  void acansado() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, cansado[i]);
    }
  }
  void araiva() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, raiva[i]);
    }
  }
  
  void abarulho() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, Barulho[i]);
    }
  }
  void avergonha() {
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, vergonha[i]);
    }
  }
  void mraiva() {
    attachA();
    delay(50);
  //  braco1();
    for (posx = 180; posx >= 10; posx -= 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }
    delay(1000);
    for (posx = 15; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (posx = 180; posx >= 10; posx -= 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (posx = 15; posx <= 180; posx += 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (posx = 180; posx >= 10; posx -= 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }
    for (posx = 0; posx <= 90; posx += 2) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      AL.write(posx);              // tell servo to go to position in variable 'pos'
      AR.write(180 - posx);
      delay(10);                       // waits 15 ms for the servo to reach the position
    }

  detachall();
  posinicial();
}


void loop () {

  if (IrReceiver.decode())
  {
    unsigned long CODIGO = IrReceiver.decodedIRData.decodedRawData;
    Serial.println(CODIGO, HEX);
    detachall();
    if (CODIGO == 0XBA45FF00) // POWER CONTROLE - liga/desliga OTTO
    { Serial.println("liga/desliga");
      if (estadoliga == 0)
      { Serial.println("liga otto");
        play = 1;
        myDFPlayer.play(play);
        aHEART();
        estadoliga = 1;
        aceno();
        delay(1000);
        aneutro();
      }
      else
      {
        Serial.println("desliga robo");
        play = 2;
        myDFPlayer.play(play);
        Serial.println("OTTO FALA TCHAU");
        aceno();
        delay(1500);
        avazio();
        parar();
      }
    }
    if (estadoliga == 1)
      if (CODIGO == 0xF30CFF00) // BOTÃO 1 CONTROLE (POSITIVO)
      {
        play = 3;
        myDFPlayer.play(play);
        aSorriso();
        acertar();
        Serial.println("Acertou-aplausos");
        delay (1000);
        aneutro();
      }
    if (CODIGO == 0xA15EFF00) //BOTÃO 3 CONTROLE (NEGATIVO)
    {
      play = 4;
      myDFPlayer.play(play);
      aTriste();
      errar();
      Serial.println("Errou");
      delay (1000);
      aneutro();
    }
    if (CODIGO == 0xE31CFF00) //BOTÃO 5 CONTROLE (DANÇA)
    {
      play = 5;
      myDFPlayer.play(play);
      detachall();
      delay(50);
      
   // dancapdire();
        dancapesq();
        detachall();
        // dancap1b();
         delay(30);
                TapF();
            delay(20);
            detachall();
            dancap2();
            detachall();
            delay(20);
            dancapesq();
            dancabraco();
             TapF();
             dancapesq();
             dancap2;
             danca3();
            posinicial();
          
      detachall();//
      delay(20);
      Serial.println("Dança");
    }
    
    if (CODIGO == 0xB847FF00) // Verifica se a tecla VERDE FOI PRESSIONADA
    {
      if (estadobotao == 0)
      {
        Serial.println("liga botões");
        estadobotao = 1;
      }
      else
      {
        Serial.println("desliga botões");
        estadobotao = 0;
      }
    }
  }

  Serial.println(estadoliga);
  irrecv.resume(); // Le o próximo valor
  delay(500);


  //teclado numérico
  char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado

  if (leitura_teclas) { // Se alguma tecla foi pressionada
    Serial.println(leitura_teclas); // Imprime a tecla pressionada na porta serial
  }
  if (estadobotao == 1) {
    if (leitura_teclas == '1') {
      play = 98;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '2') {
      play = 99;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '3') {
      play = 100;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '4') {
      play = 101;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '5') {
      play = 102;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '6') {
      play = 103;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '7') {
      play = 104;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '8') {
      play = 105;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '9') {
      play = 106;
      myDFPlayer.play(play);
    }
    if (leitura_teclas == '0') {
      play = 97;
      myDFPlayer.play(play);
    }
  }
  if (estadoliga == 1) {
    if (mfrc522.PICC_IsNewCardPresent())
    {
      modo_leitura();
    }
  }

  wdt_reset();
}
