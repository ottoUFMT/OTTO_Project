// OTTO Robot - Código Refatorado e Otimizado
// Versão Melhorada - Outubro 2024

#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <IRremote.h>
#include <avr/wdt.h>
#include <LedControl.h>
#include <Servo.h>

// ============================================
// DEFINIÇÕES DE PINOS
// ============================================
#define PIN_LEFT_FOOT    2
#define PIN_RIGHT_FOOT   3
#define PIN_LEFT_ARM     6
#define PIN_RIGHT_ARM    7
#define PIN_RFID_RST     5
#define PIN_RFID_SS      53
#define PIN_IR_RECV      30
#define PIN_LED_DIN      22
#define PIN_LED_CLK      24
#define PIN_LED_CS       26

// ============================================
// DEFINIÇÕES DE ÁUDIO (DFPLAYER)
// ============================================
enum AudioFiles {
  AUDIO_LIGA = 1,
  AUDIO_DESLIGA = 2,
  AUDIO_ACERTOU = 3,
  AUDIO_ERROU = 4,
  AUDIO_DANCA = 5,
  AUDIO_TRISTE = 84,
  AUDIO_FELIZ = 85,
  AUDIO_VERGONHA = 86,
  AUDIO_MEDO = 87,
  AUDIO_RAIVA = 89,
  AUDIO_BARULHO = 90,
  AUDIO_ENJOADO = 91,
  AUDIO_TE_AMO = 95,
  AUDIO_TECLADO_0 = 97,
  AUDIO_TECLADO_1 = 98,
  AUDIO_PARABENS = 107,
  AUDIO_CANSADA = 108
};

// ============================================
// DEFINIÇÕES DE CONTROLE IR
// ============================================
#define IR_POWER    0xBA45FF00
#define IR_BTN_1    0xF30CFF00
#define IR_BTN_3    0xA15EFF00
#define IR_BTN_5    0xE31CFF00
#define IR_BTN_VERDE 0xB847FF00

// ============================================
// OBJETOS GLOBAIS
// ============================================
Servo servoLeftFoot, servoRightFoot;
Servo servoLeftArm, servoRightArm;

SoftwareSerial dfPlayerSerial(10, 11);
DFRobotDFPlayerMini dfPlayer;
MFRC522 rfid(PIN_RFID_SS, PIN_RFID_RST);
MFRC522::MIFARE_Key rfidKey;
IRrecv irReceiver(PIN_IR_RECV);
LedControl ledMatrix = LedControl(PIN_LED_DIN, PIN_LED_CLK, PIN_LED_CS, 1);

// ============================================
// TECLADO MATRICIAL
// ============================================
const byte KEYBOARD_ROWS = 2;
const byte KEYBOARD_COLS = 5;
const byte rowPins[KEYBOARD_ROWS] = {40, 38};
const byte colPins[KEYBOARD_COLS] = {42, 44, 46, 48, 49};
const char keys[KEYBOARD_ROWS][KEYBOARD_COLS] = {
  {'0', '1', '2', '3', '4'},
  {'5', '6', '7', '8', '9'}
};
Keypad keyboard = Keypad(makeKeymap(keys), rowPins, colPins, KEYBOARD_ROWS, KEYBOARD_COLS);

// ============================================
// VARIÁVEIS DE ESTADO
// ============================================
struct RobotState {
  bool isOn;
  bool keyboardEnabled;
  int currentPosition;
} robot = {false, false, 0};

// ============================================
// EXPRESSÕES FACIAIS (LED MATRIX)
// ============================================
const byte FACE_SMILE[] = {
  B01111000, B01101100, B01100110, B01100110,
  B01100110, B01100110, B01101100, B01111000
};

const byte FACE_SAD[] = {
  B00000110, B00011000, B00110000, B00100000,
  B00100000, B00110000, B00011000, B00000110
};

const byte FACE_SHY[] = {
  B00010000, B00011000, B00001000, B00000100,
  B00000100, B00001000, B00011000, B00010000
};

const byte FACE_SURPRISED[] = {
  B00111100, B01111110, B11111111, B11111111,
  B11111111, B11111111, B01111110, B00111100
};

const byte FACE_TIRED[] = {
  B00001000, B00010000, B00010000, B00010000,
  B00010000, B00010000, B00010000, B00010000
};

const byte FACE_NEUTRAL[] = {
  B00010000, B00010000, B00010000, B00010000,
  B00010000, B00010000, B00010000, B00010000
};

const byte FACE_EMPTY[] = {
  B00000000, B00000000, B00000000, B00000000,
  B00000000, B00000000, B00000000, B00000000
};

const byte FACE_HEART[] = {
  B01110000, B11111000, B01111100, B00111110,
  B00111110, B01111100, B11111000, B01110000
};

const byte FACE_MOUTH_OPEN1[] = {
  B00010000, B00101000, B00101000, B00101000,
  B00101000, B00101000, B00101000, B00010000
};

const byte FACE_MOUTH_OPEN2[] = {
  B00010000, B00101000, B01000100, B01000100,
  B01000100, B01000100, B00101000, B00010000
};

const byte FACE_SCARED[] = {
  B00010000, B00001000, B00010000, B00001000,
  B00010000, B00001000, B00010000, B00001000
};

const byte FACE_ANGRY[] = {
  B00011110, B00101001, B01001001, B01001010,
  B01001010, B01001001, B00101001, B00011110
};

const byte FACE_NOISE[] = {
  B00111100, B01111100, B11111100, B11111100,
  B11111100, B11111100, B01111100, B00111100
};

const byte FACE_SICK[] = {
  B00011000, B00011000, B00011000, B10011001,
  B10011001, B11111111, B11111111, B00000000
};

// ============================================
// FUNÇÕES DE CONTROLE DE SERVOS
// ============================================

/**
 * attachFeet()
 * Ativa os servos das pernas do robô
 * 
 * Descrição: Associa os pinos físicos aos objetos Servo das pernas,
 * permitindo que elas sejam controladas. Deve ser chamada antes de
 * qualquer movimento de pernas.
 */
void attachFeet() {
  servoLeftFoot.attach(PIN_LEFT_FOOT);   // Ativa servo da perna esquerda no pino 2
  servoRightFoot.attach(PIN_RIGHT_FOOT); // Ativa servo da perna direita no pino 3
}

/**
 * attachArms()
 * Ativa os servos dos braços do robô
 * 
 * Descrição: Associa os pinos físicos aos objetos Servo dos braços,
 * permitindo que eles sejam controlados. Deve ser chamada antes de
 * qualquer movimento de braços.
 */
void attachArms() {
  servoLeftArm.attach(PIN_LEFT_ARM);   // Ativa servo do braço esquerdo no pino 6
  servoRightArm.attach(PIN_RIGHT_ARM); // Ativa servo do braço direito no pino 7
}

/**
 * attachRightArm()
 * Ativa apenas o servo do braço direito
 * 
 * Descrição: Função especial que ativa temporariamente o braço esquerdo
 * e depois desativa, mantendo apenas o braço direito ativo. Útil para
 * movimentos que usam apenas um braço (como acenar).
 */
void attachRightArm() {
  servoLeftArm.attach(PIN_LEFT_ARM);  // Ativa temporariamente braço esquerdo
  servoLeftArm.detach();              // Desativa braço esquerdo imediatamente
  servoRightArm.attach(PIN_RIGHT_ARM); // Mantém apenas braço direito ativo
}

/**
 * detachAll()
 * Desativa todos os servos do robô
 * 
 * Descrição: Desassocia todos os servos de seus pinos, liberando-os
 * e reduzindo consumo de energia. Os servos param de consumir corrente
 * e ficam "soltos". Deve ser chamada após completar movimentos.
 */
void detachAll() {
  servoLeftFoot.detach();  // Desativa servo da perna esquerda
  servoRightFoot.detach(); // Desativa servo da perna direita
  servoLeftArm.detach();   // Desativa servo do braço esquerdo
  servoRightArm.detach();  // Desativa servo do braço direito
}

// ============================================
// FUNÇÕES DE DISPLAY (LED MATRIX)
// ============================================

/**
 * showFace(faceData)
 * Exibe uma expressão facial na matriz de LEDs
 * 
 * @param faceData - Array de 8 bytes representando a face (cada byte é uma linha)
 * 
 * Descrição: Recebe um padrão de face (8x8 pixels) e envia linha por linha
 * para a matriz de LEDs. Cada byte representa uma linha horizontal de 8 LEDs,
 * onde 1 = LED aceso e 0 = LED apagado.
 * 
 * Exemplo: FACE_SMILE mostra um sorriso na matriz
 */
void showFace(const byte* faceData) {
  for (int i = 0; i < 8; i++) {              // Loop pelas 8 linhas da matriz
    ledMatrix.setRow(0, i, faceData[i]);     // Define cada linha com o padrão correspondente
  }
}

/**
 * animateSleeping()
 * Animação de "dormindo" com transições suaves
 * 
 * Descrição: Cria uma animação de sono em 5 frames, simulando o robô
 * piscando e ficando sonolento. Útil para demonstrar cansaço ou inatividade.
 * A animação dura aproximadamente 3.5 segundos no total.
 * 
 * Sequência:
 * 1. Face cansada (700ms)
 * 2. Face neutra (700ms) 
 * 3. Face surpresa - "Z" pequeno (700ms)
 * 4. Face neutra (700ms)
 * 5. Face cansada final (1000ms)
 */
void animateSleeping() {
  const byte* sleepFrames[] = {FACE_TIRED, FACE_NEUTRAL, FACE_SURPRISED, FACE_NEUTRAL, FACE_TIRED};
  const int frameDelays[] = {700, 700, 700, 700, 1000};
  
  for (int i = 0; i < 5; i++) {              // Percorre os 5 frames da animação
    showFace(sleepFrames[i]);                // Mostra o frame atual
    delay(frameDelays[i]);                   // Aguarda tempo específico do frame
  }
}

/**
 * animateTalking()
 * Animação de "falando" com movimento de boca
 * 
 * Descrição: Simula o robô falando abrindo e fechando a boca em 3 etapas.
 * Deve ser chamada enquanto áudio está tocando para sincronizar movimento
 * visual com som. Total de 1.7 segundos de animação.
 * 
 * Sequência:
 * 1. Boca pouco aberta (500ms)
 * 2. Boca muito aberta (500ms)
 * 3. Boca pouco aberta novamente (700ms)
 */
void animateTalking() {
  showFace(FACE_MOUTH_OPEN1);                // Boca levemente aberta
  delay(500);
  showFace(FACE_MOUTH_OPEN2);                // Boca bem aberta
  delay(500);
  showFace(FACE_MOUTH_OPEN1);                // Boca levemente aberta novamente
  delay(700);
}

// ============================================
// FUNÇÕES DE MOVIMENTO - POSIÇÕES
// ============================================

/**
 * moveToInitialPosition()
 * Move os braços para a posição de repouso/inicial
 * 
 * Descrição: Retorna ambos os braços para uma posição neutra e confortável
 * após executar movimentos. Braço direito vai para ~84° e esquerdo para ~76°.
 * Esta posição é considerada a "pose padrão" do robô quando parado.
 * 
 * Uso: Chamar após completar qualquer sequência de movimento dos braços
 */
void moveToInitialPosition() {
  attachArms();                              // Ativa os servos dos braços
  delay(10);                                 // Aguarda estabilização
  
  for (int pos = 120; pos >= 84; pos -= 2) { // Move braços gradualmente (de 120° até 84°)
    servoRightArm.write(pos);                // Posiciona braço direito
    servoLeftArm.write(160 - pos);           // Posiciona braço esquerdo (espelhado)
    delay(12);                               // Delay entre passos para movimento suave
  }
  
  detachAll();                               // Desativa servos para economizar energia
}

/**
 * smoothServoMove(servo, startPos, endPos, stepSize, delayMs)
 * Move um servo suavemente de uma posição para outra
 * 
 * @param servo - Referência ao objeto Servo a ser movido
 * @param startPos - Posição inicial em graus (0-180)
 * @param endPos - Posição final em graus (0-180)
 * @param stepSize - Tamanho do passo em graus (quanto maior, mais rápido)
 * @param delayMs - Delay em milissegundos entre cada passo
 * 
 * Descrição: Função genérica que move qualquer servo de forma suave,
 * detectando automaticamente se deve incrementar ou decrementar.
 * Elimina código duplicado e torna movimentos mais previsíveis.
 * 
 * Exemplo: smoothServoMove(servoRightArm, 0, 180, 2, 10)
 *          Move braço direito de 0° a 180° em passos de 2° com 10ms entre passos
 */
void smoothServoMove(Servo &servo, int startPos, int endPos, int stepSize, int delayMs) {
  if (startPos < endPos) {                   // Se posição inicial é menor que final
    for (int pos = startPos; pos <= endPos; pos += stepSize) {  // Incrementa posição
      servo.write(pos);                      // Move servo para posição atual
      delay(delayMs);                        // Aguarda antes do próximo passo
    }
  } else {                                   // Se posição inicial é maior que final
    for (int pos = startPos; pos >= endPos; pos -= stepSize) {  // Decrementa posição
      servo.write(pos);                      // Move servo para posição atual
      delay(delayMs);                        // Aguarda antes do próximo passo
    }
  }
}

// ============================================
// MOVIMENTOS - BRAÇOS
// ============================================

/**
 * waveHand()
 * Faz o robô acenar com o braço direito
 * 
 * Descrição: Movimento de cumprimento/despedida usando principalmente o braço direito.
 * O braço faz uma série de movimentos de vai-e-vem simulando um aceno amigável.
 * Usado quando o robô liga/desliga ou cumprimenta alguém.
 * 
 * Sequência do movimento:
 * 1. Braço sobe para posição inicial (100° → 50°)
 * 2. Acena 3 vezes (0° ↔ 150° com velocidades variadas)
 * 3. Ambos os braços voltam juntos à posição inicial
 * 
 * Duração total: ~3-4 segundos
 */
void waveHand() {
  attachRightArm();                          // Ativa apenas braço direito
  delay(50);                                 // Aguarda estabilização do servo
  
  // Sequência de posições do braço durante o aceno
  // Cada posição representa um ponto-chave do movimento
  int waveSequence[] = {100, 50, 0, 150, 0, 150, 0, 150, 50};
  int waveDelays[] = {13, 12, 12, 12, 10, 12, 12}; // Velocidades variadas para naturalidade
  
  // Executa sequência de acenos
  for (int i = 0; i < 7; i++) {
    smoothServoMove(servoRightArm, waveSequence[i], waveSequence[i+1], 2, waveDelays[i]);
  }
  
  // Retorna à posição inicial com movimento sincronizado
  smoothServoMove(servoLeftArm, 150, 50, 2, 12);
  
  detachAll();                               // Desativa servos
  moveToInitialPosition();                   // Retorna à pose padrão
}

/**
 * celebrateSuccess()
 * Movimento de comemoração - braços para cima e para baixo
 * 
 * Descrição: Animação de alegria/vitória onde ambos os braços sobem e descem
 * repetidamente de forma sincronizada e espelhada. Usado quando o robô
 * acerta algo, está feliz ou parabeniza alguém.
 * 
 * Movimento:
 * - 4 ciclos completos de braços subindo e descendo
 * - Braços se movem de forma espelhada (simétrica)
 * - Velocidade aumenta gradualmente (primeiros ciclos mais lentos)
 * - Braço direito: 150° → 0° → 150°
 * - Braço esquerdo: 30° → 180° → 30° (espelhado)
 * 
 * Duração total: ~5-6 segundos
 */
void celebrateSuccess() {
  attachArms();                              // Ativa ambos os braços
  delay(50);
  
  // 4 ciclos de comemoração com velocidades variadas
  for (int cycle = 0; cycle < 4; cycle++) {
    
    // Braços vão para baixo (posição 0° / 180°)
    for (int pos = 150; pos >= 0; pos -= 2) {
      servoRightArm.write(pos);              // Braço direito desce
      servoLeftArm.write(180 - pos);         // Braço esquerdo sobe (espelhado)
      delay(cycle == 0 ? 12 : 8);            // Primeiro ciclo mais lento
    }
    
    // Braços voltam para cima (posição 150° / 30°)
    for (int pos = 0; pos <= 150; pos += 2) {
      servoRightArm.write(pos);              // Braço direito sobe
      servoLeftArm.write(180 - pos);         // Braço esquerdo desce (espelhado)
      delay(cycle < 2 ? 12 : 8);             // Primeiros 2 ciclos mais lentos
    }
  }
  
  // Retorna suavemente à posição de repouso
  smoothServoMove(servoRightArm, 150, 50, 2, 12);
  smoothServoMove(servoLeftArm, 30, 50, 2, 12);
  
  detachAll();
  moveToInitialPosition();
}

/**
 * showSadness()
 * Movimento de tristeza - braços caem desanimados
 * 
 * Descrição: Simula tristeza/decepção fazendo os braços caírem lentamente
 * para baixo e ficarem pendurados por 2 segundos antes de voltar.
 * Usado quando o robô erra algo ou mostra emoção de tristeza.
 * 
 * Movimento:
 * 1. Braços caem gradualmente até posição 0° (totalmente para baixo)
 * 2. Pausa de 2 segundos na posição caída (efeito dramático)
 * 3. Braços voltam lentamente para posição inicial
 * 
 * Duração total: ~5 segundos (incluindo pausa)
 */
void showSadness() {
  attachArms();
  delay(50);
  
  // Braços caem lentamente (movimento de desânimo)
  for (int pos = 150; pos >= 0; pos -= 2) {
    servoLeftArm.write(pos);                 // Braço esquerdo cai
    servoRightArm.write(180 - pos);          // Braço direito cai (espelhado)
    delay(12);                               // Movimento lento para dramaticidade
  }
  
  delay(2000);                               // Pausa com braços caídos (2 segundos)
  
  // Retorna à posição inicial de forma gradual
  for (int pos = 150; pos >= 50; pos -= 2) {
    servoRightArm.write(pos);
    servoLeftArm.write(160 - pos);
    delay(12);
  }
  
  detachAll();
  moveToInitialPosition();
}

/**
 * showAnger()
 * Movimento de raiva - braços tremendo de forma agressiva
 * 
 * Descrição: Simula raiva fazendo os braços moverem-se rapidamente
 * de cima para baixo múltiplas vezes, como se estivesse batendo os punhos.
 * Os braços se movem de forma espelhada e sincronizada.
 * 
 * Movimento:
 * - 3 ciclos completos de "tremor de raiva"
 * - Cada ciclo: braços vão de 180° até 10° e voltam
 * - Movimento rápido (delay de apenas 10ms) para passar agressividade
 * - Braços se movem sempre espelhados (simétricos)
 * 
 * Duração total: ~2-3 segundos
 */
void showAnger() {
  attachArms();
  delay(50);
  
  // 3 ciclos de movimento agressivo
  for (int cycle = 0; cycle < 3; cycle++) {
    // Movimento inicial único do braço esquerdo
    smoothServoMove(servoLeftArm, 180, 10, 2, 10);
    smoothServoMove(servoLeftArm, 15, 180, 2, 10);
    
    // Movimento sincronizado de ambos os braços (tremor de raiva)
    for (int pos = 180; pos >= 10; pos -= 2) {
      servoLeftArm.write(pos);               // Braço esquerdo desce
      servoRightArm.write(180 - pos);        // Braço direito sobe (espelhado)
      delay(10);                             // Movimento rápido e agressivo
    }
  }
  
  // Acalma os braços voltando à posição neutra
  smoothServoMove(servoLeftArm, 0, 90, 2, 10);
  
  detachAll();
  moveToInitialPosition();
}

// ============================================
// MOVIMENTOS - PERNAS
// ============================================

/**
 * legMovement(leg, start, end, stepSize, delayMs)
 * Executa movimento completo de uma perna (ida e volta)
 * 
 * @param leg - Referência ao servo da perna a ser movida
 * @param start - Posição inicial em graus
 * @param end - Posição final em graus  
 * @param stepSize - Tamanho do passo (velocidade)
 * @param delayMs - Delay entre passos em milissegundos
 * 
 * Descrição: Função auxiliar que move uma perna de uma posição para outra
 * e depois retorna à posição inicial. Útil para passos de dança e movimentos
 * repetitivos das pernas. Exemplo: levantar perna e abaixar.
 * 
 * Uso típico: legMovement(servoLeftFoot, 50, 180, 1, 10)
 *             Levanta perna esquerda de 50° até 180° e volta
 */
void legMovement(Servo &leg, int start, int end, int stepSize, int delayMs) {
  smoothServoMove(leg, start, end, stepSize, delayMs);     // Vai: posição inicial → final
  smoothServoMove(leg, end, start, stepSize, delayMs);     // Volta: posição final → inicial
}

// ============================================
// ROTINAS DE DANÇA
// ============================================
void danceRoutine1() {
  attachFeet();
  delay(100);
  
  legMovement(servoLeftFoot, 50, 180, 1, 10);
  legMovement(servoLeftFoot, 50, 180, 1, 10);
  
  delay(100);
}

void danceRoutine2() {
  attachArms();
  delay(50);
  
  for (int cycle = 0; cycle < 3; cycle++) {
    for (int pos = 90; pos >= 0; pos -= 2) {
      servoLeftArm.write(pos);
      servoRightArm.write(pos);
      delay(10);
    }
    
    for (int pos = 0; pos <= 180; pos += 2) {
      servoLeftArm.write(pos);
      servoRightArm.write(pos);
      delay(cycle == 2 ? 7 : 10);
    }
  }
  
  smoothServoMove(servoLeftArm, 180, 90, 2, 7);
  detachAll();
}

void performDance() {
  Serial.println("Iniciando dança");
  
  // Sequência de movimentos de dança
  danceRoutine1();
  delay(30);
  danceRoutine2();
  delay(20);
  
  moveToInitialPosition();
  detachAll();
}

// ============================================
// PROCESSAMENTO RFID
// ============================================

/**
 * processRFIDCard()
 * Lê cartão/tag RFID e executa comando correspondente
 * 
 * Descrição: Função completa de leitura RFID que:
 * 1. Verifica se há cartão presente e legível
 * 2. Autentica usando chave padrão de fábrica
 * 3. Lê dados do bloco 1 do setor 0 (16 bytes)
 * 4. Converte dados em número de arquivo de áudio
 * 5. Executa comando associado ao número
 * 6. Finaliza comunicação com o cartão
 * 
 * Protocolo RFID MIFARE:
 * - Usa autenticação com chave A (0xFFFFFFFFFFFF - padrão)
 * - Lê setor 0, bloco 1 (primeiros dados do usuário)
 * - Trailer block 3 contém chaves de autenticação
 * 
 * Fluxo de erros:
 * - Se cartão não responde: retorna silenciosamente
 * - Se autenticação falha: imprime erro e retorna
 * - Se leitura falha: imprime erro e continua
 * 
 * Uso: Chamada no loop() quando PICC_IsNewCardPresent() é verdadeiro
 */
void processRFIDCard() {
  // Verifica se consegue ler o serial number do cartão
  if (!rfid.PICC_ReadCardSerial()) {
    return;                                  // Cartão não respondeu, sai da função
  }
  
  // Definições de endereços MIFARE para leitura
  byte sector = 0;                           // Setor 0 (primeiro setor do cartão)
  byte blockAddr = 1;                        // Bloco 1 (primeiro bloco de dados do usuário)
  byte trailerBlock = 3;                     // Bloco 3 (bloco de controle com chaves)
  byte buffer[18];                           // Buffer para receber 16 bytes + 2 de CRC
  byte size = sizeof(buffer);                // Tamanho do buffer
  
  // Tenta autenticar usando chave A (padrão: FF FF FF FF FF FF)
  byte status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 
                                      trailerBlock, &rfidKey, &(rfid.uid));
  
  // Verifica se autenticação foi bem sucedida
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na autenticação: "));
    Serial.println(rfid.GetStatusCodeName(status));  // Imprime código de erro
    return;                                  // Sai sem executar comando
  }
  
  // Lê 16 bytes de dados do bloco especificado
  status = rfid.MIFARE_Read(blockAddr, buffer, &size);
  
  // Verifica se leitura foi bem sucedida
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Falha na leitura: "));
    Serial.println(rfid.GetStatusCodeName(status));  // Imprime código de erro
    // Continua para finalizar comunicação corretamente
  }
  
  // Converte os 16 bytes lidos em string e depois em número inteiro
  String audioStr = "";
  for (byte i = 0; i < 16; i++) {            // Percorre os 16 bytes
    audioStr += (char)buffer[i];             // Converte byte em caractere e adiciona à string
  }
  int audioNum = audioStr.toInt();           // Converte string numérica em inteiro
  
  // Processa o comando de áudio correspondente ao número lido
  processAudioCommand(audioNum);
  
  delay(500);                                // Aguarda antes de poder ler próximo cartão
  
  // Finaliza comunicação com o cartão corretamente
  rfid.PICC_HaltA();                         // Coloca cartão em estado HALT (hibernação)
  rfid.PCD_StopCrypto1();                    // Para comunicação criptografada
}

/**
 * processAudioCommand(audioNum)
 * Processa comandos de áudio e executa ações correspondentes
 * 
 * @param audioNum - Número do arquivo de áudio/comando (84-108 ou outros)
 * 
 * Descrição: Função central que interpreta números lidos de cartões RFID
 * e executa a combinação apropriada de: som + expressão facial + movimento.
 * Divide comandos em duas categorias:
 * 
 * COMANDOS ESPECIAIS (84-91, 106-108): Emoções com movimentos
 * - Cada número representa uma emoção específica do robô
 * - Toca áudio, mostra face correspondente e executa movimento
 * - Exemplos: feliz, triste, raiva, medo, etc.
 * 
 * COMANDOS NORMAIS (outros números): Músicas/áudios gerais
 * - Toca áudio e faz animação de "falando"
 * - Usado para músicas, frases ou sons diversos
 * 
 * Lista completa de comandos especiais:
 * 90  - Barulho: mostra face assustada com barulho
 * 108 - Cansada: mostra face cansada/entediada
 * 87  - Medo: mostra face com medo/susto
 * 89  - Raiva: face brava + movimento agressivo de braços
 * 86  - Vergonha: face tímida/envergonhada
 * 91  - Enjoado: face de náusea/mal estar
 * 85  - Feliz: sorriso + comemoração de vitória
 * 95  - Te amo: mostra coração por 1 segundo
 * 84  - Triste: face triste + movimento de desânimo
 * 107 - Parabéns: comemoração tripla com fala
 */
void processAudioCommand(int audioNum) {
  // Verifica se é comando especial de emoção (ranges específicos)
  if ((audioNum >= 84 && audioNum <= 91) || (audioNum >= 106 && audioNum <= 108)) {
    
    switch (audioNum) {
      
      case AUDIO_BARULHO:                    // 90 - Reação a barulho alto
        dfPlayer.play(audioNum);             // Toca som de susto
        showFace(FACE_NOISE);                // Mostra face assustada/surpresa
        delay(1000);                         // Mantém expressão por 1 segundo
        showFace(FACE_NEUTRAL);              // Volta ao neutro
        break;
        
      case AUDIO_CANSADA:                    // 108 - Demonstra cansaço
        dfPlayer.play(audioNum);             // Toca som de cansaço/bocejo
        showFace(FACE_TIRED);                // Mostra face cansada
        delay(1000);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_MEDO:                       // 87 - Demonstra medo/susto
        dfPlayer.play(audioNum);             // Toca som de medo
        showFace(FACE_SCARED);               // Mostra face assustada tremendo
        delay(1000);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_RAIVA:                      // 89 - Demonstra raiva intensa
        dfPlayer.play(audioNum);             // Toca som de raiva/irritação
        showFace(FACE_ANGRY);                // Mostra face brava
        showAnger();                         // Executa movimento agressivo de braços
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_VERGONHA:                   // 86 - Demonstra vergonha/timidez
        dfPlayer.play(audioNum);             // Toca som envergonhado
        showFace(FACE_SHY);                  // Mostra face tímida
        delay(1000);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_ENJOADO:                    // 91 - Demonstra náusea/mal estar
        dfPlayer.play(audioNum);             // Toca som de enjoo
        showFace(FACE_SICK);                 // Mostra face enjoada
        delay(1000);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_FELIZ:                      // 85 - Demonstra felicidade/vitória
        dfPlayer.play(audioNum);             // Toca som feliz/comemoração
        showFace(FACE_SMILE);                // Mostra sorriso
        celebrateSuccess();                  // Braços comemoram para cima e baixo
        delay(500);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_TE_AMO:                     // 95 - Declara amor/carinho
        dfPlayer.play(audioNum);             // Toca "te amo"
        showFace(FACE_HEART);                // Mostra coração nos olhos
        delay(1000);                         // Mantém coração por 1 segundo
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_TRISTE:                     // 84 - Demonstra tristeza
        dfPlayer.play(audioNum);             // Toca som triste
        showFace(FACE_SAD);                  // Mostra face triste
        showSadness();                       // Braços caem desanimados
        delay(500);
        showFace(FACE_NEUTRAL);
        break;
        
      case AUDIO_PARABENS:                   // 107 - Comemoração de parabéns
        dfPlayer.play(audioNum);             // Toca "parabéns"
        // Sequência tripla de comemoração (3x)
        for (int i = 0; i < 3; i++) {
          celebrateSuccess();                // Braços comemoram
          delay(500);
          animateTalking();                  // Boca se move como se cantasse
          delay(500);
        }
        showFace(FACE_NEUTRAL);
        break;
    }
    
  } else {
    // Comando normal - toca música/áudio qualquer
    Serial.print("Reproduzindo música: ");
    Serial.println(audioNum);
    dfPlayer.play(audioNum);                 // Toca o arquivo de áudio
    animateTalking();                        // Faz animação de boca falando
    showFace(FACE_NEUTRAL);                  // Retorna a face neutra
  }
}

// ============================================
// CONTROLE DE SISTEMA
// ============================================

/**
 * shutdownRobot()
 * Desliga o robô de forma segura e completa
 * 
 * Descrição: Executa sequência de desligamento controlado do robô:
 * 1. Ativa watchdog timer para forçar reset em 15ms
 * 2. Atualiza estado do robô (isOn = false)
 * 3. Desabilita teclado numérico
 * 4. Pausa reprodução de áudio
 * 5. Aguarda reset do watchdog
 * 
 * Watchdog Timer (WDT):
 * - Timer de hardware que força reset se não for "alimentado" periodicamente
 * - Configurado para 15ms (WDTO_15MS)
 * - Usado aqui para garantir reset limpo do sistema
 * - Após 15ms sem wdt_reset(), Arduino reinicia automaticamente
 * 
 * IMPORTANTE: Esta função efetivamente reinicia o Arduino.
 * Use apenas quando realmente quiser desligar completamente o robô.
 * 
 * Chamada: Quando botão POWER é pressionado com robô ligado
 */
void shutdownRobot() {
  wdt_enable(WDTO_15MS);                     // Ativa watchdog com timeout de 15ms
  delay(20);                                 // Aguarda mais que timeout (força reset)
  robot.isOn = false;                        // Marca robô como desligado
  robot.keyboardEnabled = false;             // Desabilita teclado
  dfPlayer.pause();                          // Pausa áudio em reprodução
  Serial.println("Robot desligado");         // Log final antes do reset
  // Arduino reseta automaticamente após 15ms
}

// ============================================
// SETUP
// ============================================

/**
 * setup()
 * Inicialização completa do sistema do robô OTTO
 * 
 * Descrição: Função executada UMA ÚNICA VEZ quando Arduino liga ou reseta.
 * Responsável por inicializar todos os componentes de hardware e software
 * do robô, preparando-o para operação normal.
 * 
 * COMPONENTES INICIALIZADOS:
 * 
 * 1. LED MATRIX (Matriz de LEDs 8x8):
 *    - Remove modo sleep/shutdown
 *    - Define brilho em nível 7 (0-15)
 *    - Limpa display
 *    - Mostra face vazia inicial
 * 
 * 2. COMUNICAÇÕES SERIAIS:
 *    - DFPlayer: 9600 baud (comunicação com módulo de áudio)
 *    - Serial Monitor: 9600 baud (debug via USB)
 *    - SPI: Comunicação com leitor RFID
 * 
 * 3. SENSORES/ENTRADA:
 *    - IR Receiver: Habilita recepção de controle remoto
 *    - RFID (MFRC522): Inicializa leitor de cartões
 * 
 * 4. DFPLAYER MINI (Módulo de Áudio):
 *    - Timeout: 500ms (tempo máximo de espera por resposta)
 *    - Volume: 25/30 (83% do volume máximo)
 *    - Equalização: Normal (sem ajustes de graves/agudos)
 *    - Verifica presença de cartão SD com arquivos
 * 
 * 5. RFID:
 *    - Prepara chave de autenticação padrão (0xFFFFFFFFFFFF)
 *    - Esta é a chave de fábrica dos cartões MIFARE
 * 
 * VALIDAÇÕES:
 * - Se DFPlayer não responder: entra em loop infinito com mensagens de erro
 * - Se cartão SD não for detectado: mesmo comportamento
 * - Mensagens detalhadas no Serial Monitor para troubleshooting
 * 
 * SEQUÊNCIA DE INICIALIZAÇÃO:
 * 1. Hardware básico (LED, serial)
 * 2. Comunicações (IR, SPI, RFID)  
 * 3. DFPlayer com validação
 * 4. Configurações finais (chave RFID, volume)
 * 5. Mensagem "Sistema pronto!"
 * 
 * Tempo total estimado: 3-5 segundos
 */
void setup() {
  // ==========================================
  // INICIALIZA LED MATRIX
  // ==========================================
  ledMatrix.shutdown(0, false);              // Remove matriz do modo sleep (false = ativo)
  ledMatrix.setIntensity(0, 7);              // Brilho 7 de 15 (médio-alto, não cansa olhos)
  ledMatrix.clearDisplay(0);                 // Limpa qualquer lixo de memória do display
  showFace(FACE_EMPTY);                      // Mostra face vazia (todos LEDs apagados)
  
  // ==========================================
  // INICIALIZA COMUNICAÇÕES SERIAIS
  // ==========================================
  dfPlayerSerial.begin(9600);                // Inicia serial software para DFPlayer (pinos 10,11)
  irReceiver.enableIRIn();                   // Habilita recepção do sensor infravermelho
  Serial.begin(9600);                        // Inicia serial de debug (USB, para monitor serial)
  SPI.begin();                               // Inicia barramento SPI (usado pelo RFID)
  rfid.PCD_Init();                           // Inicializa leitor RFID MFRC522
  
  Serial.println("Sistema iniciando...");    // Primeira mensagem de log
  
  // ==========================================
  // PREPARA CHAVE RFID PADRÃO DE FÁBRICA
  // ==========================================
  // Cartões MIFARE vêm de fábrica com chave: FF FF FF FF FF FF
  // Esta chave permite ler/escrever nos setores do cartão
  for (byte i = 0; i < 6; i++) {             // Chave tem 6 bytes
    rfidKey.keyByte[i] = 0xFF;               // Define cada byte como 0xFF
  }
  
  // ==========================================
  // INICIALIZA E VALIDA DFPLAYER MINI
  // ==========================================
  Serial.println(F("Inicializando DFPlayer Mini (3-5 segundos)"));
  
  // Tenta inicializar DFPlayer (pode demorar até 5 segundos)
  if (!dfPlayer.begin(dfPlayerSerial)) {
    // Se falhar, mostra instruções de troubleshooting
    Serial.println(F("Erro ao inicializar DFPlayer:"));
    Serial.println(F("1. Verifique conexões do DFPlayer Mini"));
    Serial.println(F("2. Insira cartão SD formatado em FAT32"));
    Serial.println(F("3. Verifique se há arquivos MP3 no cartão"));
    while (true);                            // Loop infinito - sistema não pode continuar
  }
  
  Serial.println(F("DFPlayer Mini inicializado!"));
  
  // ==========================================
  // CONFIGURA PARÂMETROS DE ÁUDIO
  // ==========================================
  dfPlayer.setTimeOut(500);                  // Timeout de 500ms para comandos
  dfPlayer.volume(25);                       // Volume 25 de 30 (83% - alto mas não distorce)
  dfPlayer.EQ(0);                            // EQ Normal (0=Normal, 1=Pop, 2=Rock, etc)
  
  // ==========================================
  // VERIFICA CARTÃO SD E ARQUIVOS
  // ==========================================
  Serial.print("Arquivos no SD: ");
  Serial.println(dfPlayer.readFileCounts(DFPLAYER_DEVICE_SD));  // Conta e mostra quantidade
  
  Serial.println("Sistema pronto!");         // Mensagem final - sistema operacional
  // Setup completo! Agora loop() começa a executar
}

// ============================================
// LOOP PRINCIPAL
// ============================================

/**
 * loop()
 * Ciclo principal de execução - roda continuamente após setup()
 * 
 * Descrição: Função que executa INFINITAMENTE após a inicialização.
 * Responsável por monitorar todas as entradas (IR, teclado, RFID) e
 * responder adequadamente a cada comando recebido.
 * 
 * ESTRUTURA DO LOOP:
 * 1. Reset do watchdog (previne reset acidental)
 * 2. Processa comandos do controle remoto IR
 * 3. Processa teclas do teclado matricial
 * 4. Processa cartões RFID
 * 5. Aguarda 500ms antes de repetir
 * 
 * ENTRADAS MONITORADAS:
 * 
 * A) CONTROLE REMOTO INFRAVERMELHO (IR):
 *    - Power (0xBA45FF00): Liga/desliga robô
 *    - Botão 1 (0xF30CFF00): Acertou - comemoração
 *    - Botão 3 (0xA15EFF00): Errou - tristeza
 *    - Botão 5 (0xE31CFF00): Dança
 *    - Botão Verde (0xB847FF00): Liga/desliga teclado
 * 
 * B) TECLADO MATRICIAL (0-9):
 *    - Apenas funciona se teclado estiver habilitado
 *    - Cada tecla (0-9) toca áudio correspondente (97-106)
 * 
 * C) LEITOR RFID:
 *    - Apenas funciona se robô estiver ligado
 *    - Lê cartões e executa comandos de emoção/música
 * 
 * PROTEÇÕES E SEGURANÇA:
 * - Watchdog reset a cada ciclo (previne travamentos)
 * - Desativa servos após cada comando IR (economia de energia)
 * - Verifica estado do robô antes de permitir ações
 * - Delay de 500ms evita leituras duplicadas
 * 
 * FLUXO DE PRIORIDADE:
 * 1º IR (mais prioritário - pode ligar/desligar tudo)
 * 2º Teclado (depende de estar habilitado)
 * 3º RFID (depende de robô estar ligado)
 * 
 * Performance: ~2 ciclos por segundo (500ms de delay)
 */
void loop() {
  // ==========================================
  // WATCHDOG RESET - PREVINE TRAVAMENTOS
  // ==========================================
  wdt_reset();                               // "Alimenta" watchdog dizendo "estou vivo!"
                                             // Se não chamar isso, Arduino reseta após timeout
  
  // ==========================================
  // PROCESSA CONTROLE REMOTO INFRAVERMELHO
  // ==========================================
  if (IrReceiver.decode()) {                 // Verifica se recebeu sinal IR
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;  // Pega código recebido
    Serial.print("IR Code: ");               // Log para debug
    Serial.println(code, HEX);               // Mostra código em hexadecimal
    
    detachAll();                             // Desativa todos servos antes de processar
    
    // === SWITCH PARA PROCESSAR CADA BOTÃO DO CONTROLE ===
    switch (code) {
      
      // ------------------------------------------
      // BOTÃO POWER - LIGA/DESLIGA ROBÔ
      // ------------------------------------------
      case IR_POWER:
        Serial.println("Power button");
        
        if (!robot.isOn) {                   // Se robô está DESLIGADO
          Serial.println("Ligando OTTO");
          dfPlayer.play(AUDIO_LIGA);         // Toca som de inicialização
          showFace(FACE_HEART);              // Mostra coração (carinho ao ligar)
          robot.isOn = true;                 // Marca robô como ligado
          waveHand();                        // Acena cumprimentando
          delay(1000);                       // Mantém coração por 1 segundo
          showFace(FACE_NEUTRAL);            // Volta à face neutra
          
        } else {                             // Se robô está LIGADO
          Serial.println("Desligando OTTO");
          dfPlayer.play(AUDIO_DESLIGA);      // Toca som de despedida
          waveHand();                        // Acena se despedindo
          delay(1500);                       // Aguarda término do aceno
          showFace(FACE_EMPTY);              // Apaga face (display vazio)
          shutdownRobot();                   // Executa desligamento completo
        }
        break;
      
      // ------------------------------------------
      // BOTÃO 1 - RESPOSTA POSITIVA (ACERTOU)
      // ------------------------------------------
      case IR_BTN_1:
        if (robot.isOn) {                    // Só funciona se robô estiver ligado
          dfPlayer.play(AUDIO_ACERTOU);      // Toca som de acerto/aplausos
          showFace(FACE_SMILE);              // Mostra sorriso
          celebrateSuccess();                // Braços comemoram
          Serial.println("Acertou - Aplausos");
          delay(1000);
          showFace(FACE_NEUTRAL);
        }
        break;
      
      // ------------------------------------------
      // BOTÃO 3 - RESPOSTA NEGATIVA (ERROU)
      // ------------------------------------------
      case IR_BTN_3:
        if (robot.isOn) {                    // Só funciona se robô estiver ligado
          dfPlayer.play(AUDIO_ERROU);        // Toca som de erro
          showFace(FACE_SAD);                // Mostra face triste
          showSadness();                     // Braços caem desanimados
          Serial.println("Errou");
          delay(1000);
          showFace(FACE_NEUTRAL);
        }
        break;
      
      // ------------------------------------------
      // BOTÃO 5 - MODO DANÇA
      // ------------------------------------------
      case IR_BTN_5:
        if (robot.isOn) {                    // Só funciona se robô estiver ligado
          dfPlayer.play(AUDIO_DANCA);        // Toca música de dança
          performDance();                    // Executa coreografia completa
          Serial.println("Dança");
        }
        break;
      
      // ------------------------------------------
      // BOTÃO VERDE - LIGA/DESLIGA TECLADO
      // ------------------------------------------
      case IR_BTN_VERDE:
        robot.keyboardEnabled = !robot.keyboardEnabled;  // Inverte estado (toggle)
        Serial.print("Teclado: ");
        Serial.println(robot.keyboardEnabled ? "LIGADO" : "DESLIGADO");
        break;
    }
    
    irReceiver.resume();                     // Prepara para receber próximo sinal IR
  }
  
  // ==========================================
  // PROCESSA TECLADO MATRICIAL
  // ==========================================
  char key = keyboard.getKey();              // Lê tecla pressionada (retorna 0 se nenhuma)
  
  if (key && robot.keyboardEnabled) {        // Se há tecla E teclado está habilitado
    Serial.print("Tecla: ");
    Serial.println(key);
    
    // Converte caractere da tecla em número de arquivo de áudio
    // '0' → 97, '1' → 98, '2' → 99, ... '9' → 106
    int audioFile = AUDIO_TECLADO_0 + (key - '0');
    
    // Valida se número está no range correto (97 a 106)
    if (audioFile >= AUDIO_TECLADO_0 && audioFile <= AUDIO_TECLADO_1 + 9) {
      dfPlayer.play(audioFile);              // Toca áudio correspondente à tecla
    }
  }
  
  // ==========================================
  // PROCESSA CARTÕES RFID
  // ==========================================
  // Verifica se robô está ligado E se há cartão novo presente
  if (robot.isOn && rfid.PICC_IsNewCardPresent()) {
    processRFIDCard();                       // Lê cartão e executa comando
  }
  
  // ==========================================
  // DELAY ENTRE CICLOS
  // ==========================================
  delay(500);                                // Aguarda 500ms antes do próximo ciclo
                                             // Evita leituras múltiplas e dá tempo para processar
  
  // Loop reinicia automaticamente - volta ao início do loop()
}