#include <DFRobotDFPlayerMini.h>

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
String Song = "";    // string recebida
int play = 0;        // numero musica
//DFRobotDFPlayerMini myDFPlayer;

MFRC522 mfrc522(SS_PIN, RST_PIN);          // RFID
MFRC522::MIFARE_Key key; 



void setup()
{

    // Comunicacao serial com o modulo
    // mySoftwareSerial.begin(9600);
    // Inicializa a serial do Arduino

    Serial.begin(9600); // Inicia a serial
    SPI.begin();        // Inicia  SPI bus
    mfrc522.PCD_Init(); // Inicia MFRC522
    Serial.println("A");

    // Prepara chave - padrao de fabrica = FFFFFFFFFFFFh
    for (byte i = 0; i < 6; i++)
        key.keyByte[i] = 0xFF;

    // Definicoes iniciais
    //   myDFPlayer.setTimeOut(500); // Timeout serial 500ms
    //    myDFPlayer.volume(30);      // Volume 5
    //   myDFPlayer.EQ(0);           // Equalizacao normal

    // Mostra o menu de comandos
    //  Serial.println();
    //  Serial.print("Numero de arquivos no cartao SD: ");
    //  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
}


void modo_leitura()
{

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
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A,
                                      trailerBlock, &key, &(mfrc522.uid));


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
    for (byte i = 0; i < 16; i++){
        char sound = char(buffer[i]);
        String rc(sound);
        Song += rc;
    }
    play = Song.toInt();

    if (play > 10)
    {
        Serial.println();
        Serial.print("Reproduzindo musica: ");
        Serial.print(Song);
        //myDFPlayer.play(play);
    }
    delay(1000);

    Serial.println();
    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    //    myDFPlayer.play(play);

    Serial.print("D");
    return;
}

void loop (){

 if (mfrc522.PICC_IsNewCardPresent())
                {
                    modo_leitura(); // deixo o rfid sempre ligado
                }
}
