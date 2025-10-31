# 🤖 Robô Otto 1.0 - Repositório Oficial

<div align="center">

![Arduino](https://img.shields.io/badge/Arduino-1.8.19-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Version](https://img.shields.io/badge/Version-2.0-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

**Robô educacional interativo com reconhecimento RFID, controle remoto IR, teclado matricial e matriz de LEDs**

[📖 Documentação](#-documentação) • [⚙️ Instalação](#️-instalação) • [🎯 Funcionalidades](#-funcionalidades) • [🚀 Roadmap](#-próximas-funcionalidades)

</div>

---

## 📋 Índice

- [Sobre o Projeto](#-sobre-o-projeto)
- [Características Principais](#-características-principais)
- [Documentação](#-documentação)
- [Pré-requisitos](#-pré-requisitos)
- [Instalação](#️-instalação)
- [Componentes de Hardware](#-componentes-de-hardware)
- [Bibliotecas e Versões](#-bibliotecas-e-versões)
- [Funcionalidades Atuais](#-funcionalidades-atuais)
- [Esquema de Conexões](#-esquema-de-conexões)
- [Como Usar](#-como-usar)
- [Próximas Funcionalidades](#-próximas-funcionalidades)
- [Modelagem 3D](#-modelagem-3d)
- [Troubleshooting](#-troubleshooting)
- [Contribuindo](#-contribuindo)
- [Autores](#-autores)
- [Referências](#-referências)
- [Licença](#-licença)

---

## 🎯 Sobre o Projeto

O **Otto 1.0** é um robô educacional open-source desenvolvido para ensinar programação, eletrônica e robótica de forma interativa e divertida. Este repositório contém a versão 2.0 do código, completamente refatorada e documentada, além dos arquivos de modelagem 3D para impressão.

### ✨ Características Principais

- 🎵 **Sistema de Áudio**: Reprodução de músicas e efeitos sonoros via DFPlayer Mini
- 😊 **Expressões Faciais**: Matriz LED 8x8 com 10+ emoções diferentes
- 🎮 **Controle Remoto**: Interface via controle infravermelho
- 💳 **Leitor RFID**: Reconhecimento de cartões para comandos personalizados
- ⌨️ **Teclado Matricial**: 10 teclas para interação direta
- 🕺 **Movimentos**: 4 servos para braços e pernas com coreografias
- 🧠 **Sistema Inteligente**: Respostas emocionais e comportamentos interativos

---

## 📖 Documentação

### 📂 Estrutura do Projeto

```
OTTO_v1_v2/
├── Prog_otto_03_10_oficial.ino    # Código principal (versão original)
├── Otto_Improved.ino               # Código refatorado v2.0
├── otto_1.0-stl/                   # Arquivos para impressão 3D
│   ├── corpo.stl
│   ├── pernas.stl
│   ├── bracos.stl
│   └── cabeca.stl
├── Esquemático Circuito.jpeg       # Diagrama de conexões
├── Reprodutor de MP3.jpeg          # Ilustração do sistema de áudio
└── README.md                       # Este arquivo
```

### 🔧 Linguagem e Plataforma

- **Linguagem**: C++ (Arduino Framework)
- **Plataforma**: Arduino Mega 2560
- **IDE**: Arduino IDE 1.8.19 ou superior
- **Compilador**: AVR-GCC

---

## 💻 Pré-requisitos

Antes de começar, certifique-se de ter:

### Software

- ✅ **[Visual Studio](https://visualstudio.microsoft.com/pt-br/#vs-section)** - Redistributable C++ (necessário para Arduino IDE)
- ✅ **[Java JRE](https://www.java.com/pt-BR/download/ie_manual.jsp?locale=pt_BR)** 8 ou superior - Runtime para Arduino IDE
- ✅ **[Arduino IDE](https://www.arduino.cc/en/software)** 1.8.19 - Ambiente de desenvolvimento

### Hardware Necessário

| Componente | Quantidade | Especificação |
|------------|-----------|---------------|
| Arduino Mega 2560 | 1x | Microcontrolador principal |
| Servo Motor SG90 | 4x | Movimentação de braços e pernas |
| DFPlayer Mini | 1x | Módulo de áudio MP3 |
| Alto-falante | 1x | 3W, 8Ω |
| MFRC522 | 1x | Leitor RFID 13.56MHz |
| Matriz LED 8x8 | 1x | MAX7219 |
| Receptor IR | 1x | VS1838B ou similar |
| Teclado Matricial | 1x | 2x5 (10 teclas) |
| Regulador de Tensão | 1x | LM2596 ou similar (5V, 3A) |
| Capacitor | 1x | 1000µF, 16V (estabilização) |
| Bateria | 1x | 7.4V LiPo 2S ou 6x AA |
| Resistores | Diversos | 1kΩ, 10kΩ |

---

## ⚙️ Instalação

### 1️⃣ Preparar o Ambiente

```bash
# 1. Instale Visual Studio Redistributable C++
# Baixe em: https://visualstudio.microsoft.com/pt-br/downloads/

# 2. Instale Java JRE
# Baixe em: https://www.java.com/pt-BR/download/

# 3. Instale Arduino IDE 1.8.19
# Baixe em: https://www.arduino.cc/en/software/OldSoftwareReleases
```

### 2️⃣ Instalar Bibliotecas

Abra o Arduino IDE e instale as bibliotecas via **Sketch → Include Library → Manage Libraries**:

| Biblioteca | Versão | Instalação |
|------------|--------|------------|
| **DFRobotDFPlayerMini** | 1.0.5 | Gerenciador de Bibliotecas |
| **LedControl** | 1.0.6 | Gerenciador de Bibliotecas |
| **MFRC522** | 1.4.10 | Gerenciador de Bibliotecas |
| **IRremote** | 4.0.0+ | [GitHub](https://github.com/Arduino-IRremote/Arduino-IRremote) |
| **Keypad** | 3.1.1 | Gerenciador de Bibliotecas |

**Bibliotecas Nativas** (já incluídas no Arduino):
- `Servo.h`
- `SPI.h`
- `SoftwareSerial.h`
- `avr/wdt.h`

### 3️⃣ Preparar Cartão SD

1. Formate o cartão SD em **FAT32**
2. Crie uma pasta chamada `mp3` na raiz
3. Adicione arquivos de áudio nomeados como:
   - `0001.mp3` - Som de ligar
   - `0002.mp3` - Som de desligar
   - `0084.mp3` - Emoção: Triste
   - `0085.mp3` - Emoção: Feliz
   - (Continue conforme a tabela de áudios)

### 4️⃣ Upload do Código

```bash
# 1. Conecte o Arduino Mega via USB
# 2. Selecione a placa: Tools → Board → Arduino Mega 2560
# 3. Selecione a porta: Tools → Port → COMx (Windows) ou /dev/ttyUSBx (Linux)
# 4. Clique em Upload (→)
```

---

## 🔌 Componentes de Hardware

### Pinagem Detalhada

#### **Servos Motores**
```cpp
Pino 2  → Perna Esquerda (Left Foot)
Pino 3  → Perna Direita (Right Foot)
Pino 6  → Braço Esquerdo (Left Arm)
Pino 7  → Braço Direito (Right Arm)
```

#### **DFPlayer Mini (Áudio)**
```cpp
Pino 10 → RX (DFPlayer)
Pino 11 → TX (DFPlayer)
        → VCC → 5V (regulado)
        → GND → GND
        → SPK+ → Alto-falante +
        → SPK- → Alto-falante -
```

#### **MFRC522 (RFID)**
```cpp
Pino 5  → RST
Pino 53 → SDA/SS
Pino 51 → MOSI
Pino 50 → MISO
Pino 52 → SCK
        → 3.3V → 3.3V
        → GND → GND
```

#### **Matriz LED 8x8 (MAX7219)**
```cpp
Pino 22 → DIN (Data In)
Pino 24 → CLK (Clock)
Pino 26 → CS (Chip Select)
        → VCC → 5V
        → GND → GND
```

#### **Receptor IR**
```cpp
Pino 30 → Signal
        → VCC → 5V
        → GND → GND
```

#### **Teclado Matricial 2x5**
```cpp
Linhas:  Pino 40, 38
Colunas: Pino 42, 44, 46, 48, 49
```

---

## 📚 Bibliotecas e Versões

| Biblioteca | Versão | Descrição | Download |
|------------|--------|-----------|----------|
| **Arduino IDE** | 1.8.19 / 1.8.57.0 | Ambiente de desenvolvimento | [Link](https://www.arduino.cc/en/software/OldSoftwareReleases) |
| **DFRobotDFPlayerMini** | 1.0.5 | Controle do módulo MP3 | [GitHub](https://github.com/DFRobot/DFRobotDFPlayerMini) |
| **LedControl** | 1.0.6 | Controle de matriz LED MAX7219 | [Arduino Lib](https://github.com/wayoda/LedControl) |
| **MFRC522** | 1.4.10 | Biblioteca para leitor RFID | [GitHub](https://github.com/miguelbalboa/rfid) |
| **IRremote** | 4.0.0+ | Recepção de sinais infravermelhos | [GitHub](https://github.com/Arduino-IRremote/Arduino-IRremote) |
| **Keypad** | 3.1.1 | Leitura de teclado matricial | [Playground](https://playground.arduino.cc/Code/Keypad/) |
| **Servo** | Nativa | Controle de servos motores | Incluída no Arduino |
| **SPI** | Nativa | Comunicação SPI | Incluída no Arduino |
| **SoftwareSerial** | Nativa | Serial por software | Incluída no Arduino |
| **avr/wdt** | Nativa | Watchdog Timer | Incluída no Arduino |

---

## 🎯 Funcionalidades Atuais

### 🎵 Sistema de Áudio
- ✅ Reprodução de 100+ arquivos MP3
- ✅ Controle de volume (0-30)
- ✅ Sons de emoções sincronizados
- ✅ Efeitos sonoros para movimentos

### 😊 Expressões Faciais (LED Matrix)
| Emoção | Código | Descrição |
|--------|--------|-----------|
| 😊 Feliz | 85 | Sorriso + comemoração |
| 😢 Triste | 84 | Face triste + braços caídos |
| 😠 Raiva | 89 | Face brava + braços tremendo |
| 😨 Medo | 87 | Face assustada |
| 😳 Vergonha | 86 | Face tímida |
| 🤢 Enjoado | 91 | Face nauseada |
| 😴 Cansado | 108 | Face sonolenta |
| 😲 Surpreso | 90 | Face de susto |
| ❤️ Amor | 95 | Coração nos olhos |
| 😐 Neutro | - | Face padrão |

### 🎮 Controle Remoto IR
| Botão | Código | Função |
|-------|--------|--------|
| POWER | 0xBA45FF00 | Liga/Desliga robô |
| 1 | 0xF30CFF00 | Resposta positiva (acertou) |
| 3 | 0xA15EFF00 | Resposta negativa (errou) |
| 5 | 0xE31CFF00 | Modo dança |
| VERDE | 0xB847FF00 | Liga/Desliga teclado |

### 💳 Cartões RFID
- Emoções: Cartões 84-91, 106-108
- Músicas: Qualquer outro número
- Comandos especiais: Parabéns (107)

### ⌨️ Teclado Matricial
- Teclas 0-9 tocam áudios 97-106
- Habilitação via controle remoto
- Feedback sonoro imediato

### 🕺 Movimentos
- **Acenar**: Cumprimento com braço direito
- **Comemorar**: Braços para cima e baixo (4 ciclos)
- **Tristeza**: Braços caem desanimados
- **Raiva**: Braços tremendo rapidamente
- **Dança**: Coreografia completa (pernas + braços)

---

## 🔗 Esquema de Conexões

### Diagrama (desconsidere o potenciometro, ele foi retirado, o alto falante é ligado diretamente no DFPlayer)
![Esquemático do Circuito](Esquemático%20Circuito.jpeg)

### Esquema Eletrico
![Esquema elétrico](https://github.com/ottoUFMT/OTTO_v1_v2/blob/75738cb566f7fa2944ed9302c6d7c8f4769628b4/Esquema%20completo/Conex%C3%B5es_OTTO.png)

### Componentes Principais
```
┌─────────────────────────────────────────┐
│         Arduino Mega 2560               │
│                                         │
│  [Servos] [DFPlayer] [RFID] [IR] [LED] │
│     │         │         │      │     │  │
└─────┼─────────┼─────────┼──────┼─────┼──┘
      │         │         │      │     │
    Braços   Alto-    Cartões  Controle LEDs
    Pernas   Falante           Remoto   Matriz
```

---

## 🚀 Como Usar

### Inicialização

1. **Ligue o robô**: Pressione o botão POWER do controle
2. **Aguarde**: Otto mostra coração ❤️ e acena 👋
3. **Pronto**: Face neutra indica que está pronto

### Interações Básicas

#### Via Controle Remoto
```
POWER → Liga/Desliga
  1   → Elogio (acertou) → Otto comemora 🎉
  3   → Feedback negativo (errou) → Otto fica triste 😢
  5   → Inicia dança 🕺
VERDE → Habilita/desabilita teclado
```

#### Via Teclado Matricial
```
1. Pressione VERDE no controle para habilitar
2. Pressione qualquer tecla (0-9)
3. Otto reproduz áudio correspondente
```

#### Via Cartões RFID
```
1. Aproxime cartão do leitor
2. Otto identifica o número gravado
3. Executa ação correspondente:
   - Emoções (84-91): Som + expressão + movimento
   - Músicas (outros): Som + animação de fala
```

### Desligamento
```
1. Pressione POWER no controle
2. Otto acena tchau 👋
3. Toca som de despedida
4. Display apaga e sistema desliga
```

---

## 🚀 Próximas Funcionalidades

### 📋 Roadmap v3.0

#### 🔧 Hardware
- [ ] **Teclado Touch Capacitivo**: Substituir teclado mecânico por touch
- [ ] **Orelhas Móveis**: Adicionar servos para movimento de orelhas
- [ ] **Corpo Articulado**: Movimento de torção do corpo
- [ ] **Olhos RGB**: LEDs coloridos nos olhos com efeitos
- [ ] **Cabos Blindados**: Substituir jumpers por cabos blindados para reduzir interferência
- [ ] **PCB Personalizada**: Board com conectores parafusados para regulador de tensão
- [ ] **Proteção Elétrica**: Fusível de proteção para bateria

#### 🎨 Melhorias
- [ ] **Mais Emoções**: Expandir biblioteca de expressões
- [ ] **Coreografias**: Novas danças e movimentos
- [ ] **Jogos Interativos**: Mini-games educativos

---

## 🎨 Modelagem 3D

### 📐 Arquivos para Impressão

Todos os arquivos STL estão na pasta `otto_1.0-stl/`

### Requisitos de Impressão

| Especificação | Valor |
|---------------|-------|
| **Impressora** | Mesa mínima 25x25cm |
| **Resolução** | 0.2mm (camada) |
| **Preenchimento** | 20-30% |
| **Suporte** | Necessário em algumas peças |
| **Material** | PLA ou PETG |
| **Temperatura Bico** | 200-210°C (PLA) |
| **Temperatura Mesa** | 60°C |

### Software Recomendado

- **Modelagem**: [Autodesk Fusion 360](https://www.autodesk.com/products/fusion-360/)
- **Fatiamento**: [Ultimaker Cura](https://ultimaker.com/software/ultimaker-cura) 3.0+
- **Visualização**: Qualquer visualizador STL

### Peças para Imprimir

```
📦 otto_1.0-stl/
├── 🦴 corpo.stl           (1x) - 4-5 horas
├── 🦿 perna_esquerda.stl  (1x) - 2 horas
├── 🦿 perna_direita.stl   (1x) - 2 horas
├── 💪 braco_esquerdo.stl  (1x) - 1 hora
├── 💪 braco_direito.stl   (1x) - 1 hora
├── 🤖 cabeca_frente.stl   (1x) - 3 horas
└── 🤖 cabeca_tras.stl     (1x) - 2 horas

⏱️ Tempo total estimado: 15-20 horas
```

---

## 🐛 Troubleshooting

### Problemas Comuns

#### 1. DFPlayer não inicializa
```
Erro: "Nao inicializado: Cheque as conexoes"

Soluções:
✅ Verifique conexões TX/RX (pinos 10 e 11)
✅ Use resistor de 1kΩ no RX do DFPlayer
✅ Formate cartão SD em FAT32
✅ Renomeie arquivos: 0001.mp3, 0002.mp3...
✅ Teste cartão em outro dispositivo
```

#### 2. RFID não lê cartões
```
Erro: Cartão não é detectado

Soluções:
✅ Verifique alimentação 3.3V (não use 5V!)
✅ Confirme conexões SPI (MOSI, MISO, SCK)
✅ Aproxime cartão a menos de 3cm
✅ Teste com diferentes cartões
✅ Verifique se biblioteca MFRC522 está instalada
```

#### 3. Servos tremem ou não respondem
```
Problema: Movimento irregular

Soluções:
✅ Use fonte externa de 5V, 3A mínimo
✅ Adicione capacitor de 1000µF na alimentação
✅ Verifique se fios não estão soltos
✅ Calibre posição inicial dos servos (90°)
✅ Reduza carga (remova peso extra)
```

#### 4. IR não responde
```
Problema: Controle remoto não funciona

Soluções:
✅ Troque baterias do controle
✅ Aponte diretamente para o receptor
✅ Verifique pino 30 do receptor IR
✅ Teste com Serial Monitor para ver códigos
✅ Troque receptor se necessário
```

#### 5. Matriz LED não acende
```
Problema: Display não mostra nada

Soluções:
✅ Verifique pinos DIN (22), CLK (24), CS (26)
✅ Confirme alimentação 5V estável
✅ Teste com código de exemplo do LedControl
✅ Verifique orientação do módulo MAX7219
```

---

## 🤝 Contribuindo

Contribuições são sempre bem-vindas! Siga os passos:

### Como Contribuir

1. **Fork** o projeto
2. **Clone** seu fork
   ```bash
   git clone https://github.com/seu-usuario/OTTO_v1_v2.git
   ```
3. Crie uma **branch** para sua feature
   ```bash
   git checkout -b feature/MinhaFeature
   ```
4. **Commit** suas mudanças
   ```bash
   git commit -m "Adiciona: Minha nova feature"
   ```
5. **Push** para a branch
   ```bash
   git push origin feature/MinhaFeature
   ```
6. Abra um **Pull Request**

### Diretrizes

- 📝 Comente seu código adequadamente
- ✅ Teste antes de enviar
- 📚 Atualize documentação se necessário
- 🎯 Mantenha código limpo e organizado
- 🐛 Reporte bugs com detalhes

---

## 👥 Autores

### 💻 Desenvolvimento de Software

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/defaultdayanni">
        <img src="https://github.com/defaultdayanni.png" width="100px;" alt="Foto Dayanni"/><br>
        <sub>
          <b>Dayanni</b><br>
          <small>Programação & Eletrônica</small>
        </sub>
         <td align="center">
      <a href="https://github.com/DebbieMatt" title="Colaboradora">
        <img src="https://avatars.githubusercontent.com/u/112919058?v=4" width="100px;" alt="Foto da Débora"/><br>
        <sub>
          <b>Débora Mateus</b><br>
          <small>Programação & Eletrônica</small>
         </sub>
</table>

### 🎨 Modelagem 3D

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/gabereboucas">
        <img src="https://avatars.githubusercontent.com/u/95108527?v=4" width="100px;" alt="Foto do Gabriel Rebouças"/><br>
        <sub>
          <b>Gabriel Rebouças</b><br>
          <small>Modelagem & Impressão 3D</small>
        </sub>
      </a>
    </td>
  </tr>
</table>

---

## 📚 Referências

### Documentação Oficial

- [Arduino - Referência Oficial](https://www.arduino.cc/reference/en/)
- [Arduino IDE - Download](https://www.arduino.cc/en/software)
- [Arduino - Versões Anteriores](https://www.arduino.cc/en/software/OldSoftwareReleases)

### Bibliotecas

- [DFPlayer Mini - DFRobot](https://github.com/DFRobot/DFRobotDFPlayerMini)
- [MFRC522 - RFID Library](https://github.com/miguelbalboa/rfid)
- [IRremote - Arduino](https://github.com/Arduino-IRremote/Arduino-IRremote)
- [Keypad - Arduino Playground](https://playground.arduino.cc/Code/Keypad/)
- [LedControl - Wayoda](https://github.com/wayoda/LedControl)

### Ferramentas

- [Visual Studio C++](https://visualstudio.microsoft.com/pt-br/)
- [Java Runtime Environment](https://www.java.com/pt-BR/download/)
- [Autodesk Fusion 360](https://www.autodesk.com/products/fusion-360/)
- [Ultimaker Cura](https://ultimaker.com/software/ultimaker-cura)

### Projetos Relacionados

- [Otto DIY - Projeto Original](https://www.ottodiy.com/)
- [Arduino Servo Library](https://www.arduino.cc/reference/en/libraries/servo/)
- [SPI Communication](https://www.arduino.cc/en/reference/SPI)

---

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

```
MIT License

Copyright (c) 2024 Otto UFMT

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software...
```

---


### 💙 Se este projeto foi útil, considere dar uma ⭐!

**[⬆ Voltar ao topo](#-robô-otto-10---repositório-oficial)**

---

**Desenvolvido com ❤️ pela equipe Otto UFMT**

[![GitHub](https://img.shields.io/badge/GitHub-ottoUFMT-181717?style=for-the-badge&logo=github)](https://github.com/ottoUFMT)

</div>
