/*
   bingo
   AUTOR:  Luan Pereira Pinheiro
   SKETCH: Bingo com Arduino
   DATA:   18/12/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <LiquidCrystal.h>

// DEFINIÇÃO DE PINOS DE SAÍDA ANÓDICAS
/* #define A-G pinoArduino //pinoDisplayYY3641BH */
#define A 10 //11
#define B 7 //7
#define C 4 //4
#define D 5 //2
#define E 6 //1
#define F 9 //10
#define G 3 //5

// DEFINIÇÃO DE PINOS DE SAÍDA CATÓDICAS
#define unidade 2 //6
#define dezena 8 //8

// DEFINIÇÃO DE PINOS DE ENTRADA
#define bingo 12
#define proximo 13

// DEFINIÇÃO DE CONSTANTES AUXILIARES
#define bounce 800

// DECLARAÇÃO DE FUNÇÕES DE SORTEIO
void sorteado0 (byte classe, byte noclasse);
void sorteado1 (byte classe, byte noclasse);
void sorteado2 (byte classe, byte noclasse);
void sorteado3 (byte classe, byte noclasse);
void sorteado4 (byte classe, byte noclasse);
void sorteado5 (byte classe, byte noclasse);
void sorteado6 (byte classe, byte noclasse);
void sorteado7 (byte classe, byte noclasse);
void sorteado8 (byte classe, byte noclasse);
void sorteado9 (byte classe, byte noclasse);

// DECLARAÇÃO DE FUNÇÕES FACILITADORAS
bool retencaoproximo ();
bool retencaobingo ();
void (*reset)() = 0;

// DECLARAÇÃO DE VARIÁVEIS GLOBAIS
byte sorteio;
boolean valido;
byte numSorteado[75];
byte quantidade = 0;

// DECLARAÇÃO DO DISPLAY
LiquidCrystal lcd(11, 19, 15, 16, 17, 18);

// DECLARAÇÃO DO é
byte eagudo[8] = {
  B00010,
  B00100,
  B01110,
  B10001,
  B11111,
  B10000,
  B01110
};

void setup() {
  // INÍCIO DO DISPLAY
  Serial.begin (9600);
  lcd.begin(16, 2);

  // CONFIGURAÇÃO DOS PINOS DE SAÍDA
  for (byte i = 2; i <= 10; i++) {
    pinMode (i, OUTPUT);
  }

  // CONFIGURAÇÃO DOS PINOS DE ENTRADA
  pinMode(bingo, INPUT);
  pinMode(proximo, INPUT);

  // CRIAÇÃO DA SEMENTE INSTÁVEL PARA SORTEIO
  randomSeed(analogRead(0));

  // DEFINIÇÃO DO VALOR PADRÃO PARA OS TERMOS DO VETOR
  for (byte i = 0; i < 75; i++) {
    numSorteado[i] = 0;
  }

  // IMPOSIÇÃO DO é NO BYTE 0 DA TABELA A01
  lcd.createChar(0, eagudo);

  // TEXTO INICIAL APARENTE NO DISPLAY
  lcd.setCursor(0, 0);
  lcd.print("Seja bem-vindo ao");
  lcd.setCursor(0, 1);
  lcd.print("Pilueto's Bingo!");
}

void loop() {
  // DECLARAÇÃO DE VARIÁVEIS DE LOOP
  byte dezSorteio, uniSorteio;
  byte BINGO[] = {0, 0, 0, 0, 0};
  unsigned long delay1;
  boolean acabou = true;
  byte estadobingo = 0;

  // SORTEIO
  if (retencaoproximo()) {
    do {
      sorteio = random (1, 76);
      valido = true;
      for (byte i = 0; i < 75; i++) {
        if (sorteio == numSorteado[i]) {
          valido = false;
          break;
        }
      }
      for (byte i = 0; i < 75; i++) {
        if (numSorteado[i] == 0) {
          acabou = false;
          break;
        }
      }
      // ACABA O BINGO APÓS TODOS OS NÚMEROS SEREM SORTEADOS
      if (acabou) {
        lcd.clear();
        lcd.print("O Bingo acabou!");
        sorteado0(unidade, dezena);
        delay(1000);
        reset();
        break;
      }
    } while (!valido);
    numSorteado[sorteio - 1] = sorteio;

    // LETRAS CORRESPONDENTES A CADA NÚMERO
    if (sorteio <= 15) {
      lcd.clear();
      lcd.print("B");
    } else if (sorteio <= 30) {
      lcd.clear();
      lcd.print("I");
    } else if (sorteio <= 45) {
      lcd.clear();
      lcd.print("N");
    } else if (sorteio <= 60) {
      lcd.clear();
      lcd.print("G");
    } else {
      lcd.clear();
      lcd.print("O");
    }
    quantidade++;
    lcd.print(" ");
    lcd.print(quantidade);
  }

  // IMPRESSÃO DE NÚMEROS NA TELA QUANDO SE APERTA O BOTÃO "BINGO"
  if (retencaobingo()) {
    while (!retencaobingo()) {
      for (byte i = 0; i < 5; i++) {
        BINGO[i] = 0;
      }
      switch (estadobingo) {
        case 0:
          lcd.clear();
          lcd.print("B:");
          for (byte i = 0; i < 15; i++) {
            if (numSorteado[i] != 0) {
              lcd.print(" ");
              lcd.print(numSorteado[i]);
              Serial.println(numSorteado[i]);
            }
            BINGO[0]++;
          }
          // APÓS IMPRIMIR OS NÚMEROS O DISPLAY SERÁ GIRADO
          for (byte i = 0; i < ((BINGO[0] - 5) * 4); i++) {
            // AO APERTAR O BOTÃO PRÓXIMO A PRÓXIMA LETRA E SEUS NÚMEROS CORRESPONDENTES SERÃO MOSTRADOS
            if (retencaoproximo()) {
              estadobingo = 1;
              break;
            }
            // AO APERTAR O BOTÃO BINGO SERÁ QUESTIONADO SE DEU BINGO
            if (retencaobingo()) {
              lcd.clear();
              while (!retencaobingo() || retencaoproximo()) {
                lcd.home();
                lcd.print("BINGO!?");
                // O JOGO ACABARÁ AO APERTAR O BOTÃO BINGO
                if (retencaobingo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Parab");
                  lcd.write(byte(0));
                  lcd.print("ns ao");
                  lcd.setCursor(3, 1);
                  lcd.print("vencedor!");
                  delay(1000);
                  reset();
                  break;
                }
                // O JOGO CONTINUARÁ AO APERTAR O BOTÃO PRÓXIMO
                if (retencaoproximo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Continue se");
                  lcd.setCursor(2, 1);
                  lcd.print("divertindo!");
                  goto vai;
                  break;
                }
              }
            }
            delay1 = millis();
            while (millis() - delay1 < 500) {
              if (millis() - delay1 >= 500) {
                lcd.scrollDisplayLeft();
              }
            }
          }
          estadobingo = 1;
          break;
        case 1:
          lcd.clear();
          lcd.print("I:");
          for (byte i = 15; i < 30; i++) {
            if (numSorteado[i] != 0) {
              lcd.print(" ");
              lcd.print(numSorteado[i]);
              Serial.println(numSorteado[i]);
            }
            BINGO[1]++;
          }
          for (byte i = 0; i < ((BINGO[1] - 5) * 4); i++) {
            if (retencaoproximo()) {
              estadobingo = 2;
              break;
            }
            if (retencaobingo()) {
              lcd.clear();
              while (!retencaobingo() || retencaoproximo()) {
                lcd.home();
                lcd.print("BINGO!?");
                if (retencaobingo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Parab");
                  lcd.write(byte(0));
                  lcd.print("ns ao");
                  lcd.setCursor(3, 1);
                  lcd.print("vencedor!");
                  delay(1000);
                  reset();
                  break;
                }
                if (retencaoproximo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Continue se");
                  lcd.setCursor(2, 1);
                  lcd.print("divertindo!");
                  goto vai;
                  break;
                }
              }
            }
            delay1 = millis();
            while (millis() - delay1 < 500) {
              if (millis() - delay1 >= 500) {
                lcd.scrollDisplayLeft();
              }
            }
          }
          estadobingo = 2;
          break;
        case 2:
          lcd.clear();
          lcd.print("N:");
          for (byte i = 30; i < 45; i++) {
            if (numSorteado[i] != 0) {
              lcd.print(" ");
              lcd.print(numSorteado[i]);
              Serial.println(numSorteado[i]);
            }
            BINGO[2]++;
          }
          for (byte i = 0; i < ((BINGO[2] - 5) * 4); i++) {
            if (retencaoproximo()) {
              estadobingo = 3;
              break;
            }
            if (retencaobingo()) {
              lcd.clear();
              while (!retencaobingo() || retencaoproximo()) {
                lcd.home();
                lcd.print("BINGO!?");
                if (retencaobingo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Parab");
                  lcd.write(byte(0));
                  lcd.print("ns ao");
                  lcd.setCursor(3, 1);
                  lcd.print("vencedor!");
                  delay(1000);
                  reset();
                  break;
                }
                if (retencaoproximo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Continue se");
                  lcd.setCursor(2, 1);
                  lcd.print("divertindo!");
                  goto vai;
                  break;
                }
              }
            }
            delay1 = millis();
            while (millis() - delay1 < 500) {
              if (millis() - delay1 >= 500) {
                lcd.scrollDisplayLeft();
              }
            }
          }
          estadobingo = 3;
          break;
        case 3:
          lcd.clear();
          lcd.print("G:");
          for (byte i = 45; i < 60; i++) {
            if (numSorteado[i] != 0) {
              lcd.print(" ");
              lcd.print(numSorteado[i]);
              Serial.println(numSorteado[i]);
            }
            BINGO[3]++;
          }
          for (byte i = 0; i < ((BINGO[3] - 5) * 4); i++) {
            if (retencaoproximo()) {
              estadobingo = 4;
              break;
            }
            if (retencaobingo()) {
              lcd.clear();
              while (!retencaobingo() || retencaoproximo()) {
                lcd.home();
                lcd.print("BINGO!?");
                if (retencaobingo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Parab");
                  lcd.write(byte(0));
                  lcd.print("ns ao");
                  lcd.setCursor(3, 1);
                  lcd.print("vencedor!");
                  delay(1000);
                  reset();
                  break;
                }
                if (retencaoproximo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Continue se");
                  lcd.setCursor(2, 1);
                  lcd.print("divertindo!");
                  goto vai;
                  break;
                }
              }
            }
            delay1 = millis();
            while (millis() - delay1 < 500) {
              if (millis() - delay1 >= 500) {
                lcd.scrollDisplayLeft();
              }
            }
          }
          estadobingo = 4;
          break;
        case 4:
          lcd.clear();
          lcd.print("O:");
          for (byte i = 60; i < 75; i++) {
            if (numSorteado[i] != 0) {
              lcd.print(" ");
              lcd.print(numSorteado[i]);
              Serial.println(numSorteado[i]);
            }
            BINGO[4]++;
          }
          for (byte i = 0; i < ((BINGO[4] - 5) * 4); i++) {
            if (retencaoproximo()) {
              estadobingo = 0;
              break;
            }
            if (retencaobingo()) {
              lcd.clear();
              while (!retencaobingo() || retencaoproximo()) {
                lcd.home();
                lcd.print("BINGO!?");
                if (retencaobingo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Parab");
                  lcd.write(byte(0));
                  lcd.print("ns ao");
                  lcd.setCursor(3, 1);
                  lcd.print("vencedor!");
                  delay(1000);
                  reset();
                  break;
                }
                if (retencaoproximo()) {
                  lcd.clear();
                  lcd.setCursor(2, 0);
                  lcd.print("Continue se");
                  lcd.setCursor(2, 1);
                  lcd.print("divertindo!");
                  goto vai;
                  break;
                }
              }
            }
            delay1 = millis();
            while (millis() - delay1 < 500) {
              if (millis() - delay1 >= 500) {
                lcd.scrollDisplayLeft();
              }
            }
          }
          estadobingo = 0;
          break;
      }
    }
  }
  
vai: // usado devido à necessidade de saída de vários loops

  // SEPARAÇÃO ENTRE DEZENAS E UNIDADES PARA POSTERIOR VISUALIZAÇÃO NO DISPLAY 7 SEGMENTOS
  uniSorteio = sorteio % 10;
  dezSorteio = sorteio / 10;

  // VISUALIZAÇÃO NO DISPLAY 7 SEGMENTOS, ALTERNANDO ENTRE DEZENAS E UNIDADES RÁPIDA E CONTINUAMENTE
  switch (dezSorteio) {
    case 0:
      sorteado0 (dezena, unidade);
      delay(5);
      break;
    case 1:
      sorteado1 (dezena, unidade);
      delay(5);
      break;
    case 2:
      sorteado2 (dezena, unidade);
      delay(5);
      break;
    case 3:
      sorteado3 (dezena, unidade);
      delay(5);
      break;
    case 4:
      sorteado4 (dezena, unidade);
      delay(5);
      break;
    case 5:
      sorteado5 (dezena, unidade);
      delay(5);
      break;
    case 6:
      sorteado6 (dezena, unidade);
      delay(5);
      break;
    case 7:
      sorteado7 (dezena, unidade);
      delay(5);
      break;
    case 8:
      sorteado8 (dezena, unidade);
      delay(5);
      break;
    case 9:
      sorteado9 (dezena, unidade);
      delay(5);
      break;
  } switch (uniSorteio) {
    case 0:
      sorteado0 (unidade, dezena);
      delay(5);
      break;
    case 1:
      sorteado1 (unidade, dezena);
      delay(5);
      break;
    case 2:
      sorteado2 (unidade, dezena);
      delay(5);
      break;
    case 3:
      sorteado3 (unidade, dezena);
      delay(5);
      break;
    case 4:
      sorteado4 (unidade, dezena);
      delay(5);
      break;
    case 5:
      sorteado5 (unidade, dezena);
      delay(5);
      break;
    case 6:
      sorteado6 (unidade, dezena);
      delay(5);
      break;
    case 7:
      sorteado7 (unidade, dezena);
      delay(5);
      break;
    case 8:
      sorteado8 (unidade, dezena);
      delay(5);
      break;
    case 9:
      sorteado9 (unidade, dezena);
      delay(5);
      break;
  }
}


void sorteado0 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (F, HIGH);
  digitalWrite (G, LOW);
}

void sorteado1 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, LOW);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (F, LOW);
  digitalWrite (G, LOW);
}

void sorteado2 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, LOW);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (F, LOW);
  digitalWrite (G, HIGH);
}

void sorteado3 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, LOW);
  digitalWrite (F, LOW);
  digitalWrite (G, HIGH);
}

void sorteado4 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, LOW);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (F, HIGH);
  digitalWrite (G, HIGH);
}

void sorteado5 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, LOW);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, LOW);
  digitalWrite (F, HIGH);
  digitalWrite (G, HIGH);
}

void sorteado6 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, LOW);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (F, HIGH);
  digitalWrite (G, HIGH);
}

void sorteado7 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, LOW);
  digitalWrite (E, LOW);
  digitalWrite (F, LOW);
  digitalWrite (G, LOW);
}

void sorteado8 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, HIGH);
  digitalWrite (F, HIGH);
  digitalWrite (G, HIGH);
}

void sorteado9 (byte classe, byte noclasse) {
  digitalWrite (classe, LOW);
  digitalWrite (noclasse, HIGH);

  digitalWrite (A, HIGH);
  digitalWrite (B, HIGH);
  digitalWrite (C, HIGH);
  digitalWrite (D, HIGH);
  digitalWrite (E, LOW);
  digitalWrite (F, HIGH);
  digitalWrite (G, HIGH);
}

bool retencaoproximo () {
  static bool estadoBotao;
  static bool estadoAnt;
  bool estadoRet = false;
  unsigned long delayBotao = 0;

  if ((millis() - delayBotao) > bounce) {
    estadoBotao = digitalRead(proximo);
    if (estadoBotao && !estadoAnt) {
      estadoRet = !estadoRet;
      delayBotao = millis();
    }
    estadoAnt = estadoBotao;
  }
  return estadoRet;
}

bool retencaobingo () {
  static bool estadoBotao;
  static bool estadoAnt;
  bool estadoRet = false;
  unsigned long delayBotao = 0;

  if ((millis() - delayBotao) > bounce) {
    estadoBotao = digitalRead(bingo);
    if (estadoBotao && !estadoAnt) {
      estadoRet = !estadoRet;
      delayBotao = millis();
    }
    estadoAnt = estadoBotao;
  }
  return estadoRet;
}
