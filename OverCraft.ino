/*
 * Projeto Artbot - Circuitos Logicos -- OverCraft?
 * Author: Joao Ricardo Chaves
 * Date: 18/06/25
 *
 */

// ============================
// --- Libraries ---
#include <LiquidCrystal_I2C.h>
#include "Variaveis.h"
// =============================
// --- Functions Prototypes ---
void Timer(void *parameters);             // Task para o timer do sistema
void ReadResetButton();                   // Interrupcao par leitura de botao
void MenuDisp(void *parameters);          // Layout LCD 20x4
void Score();                             // Pontuacao de acordo com o desafio
void tabuleiro(void *parameters);         // Cria o tabuleiro
float AnalogVoltage(int Voltage);         // Leitura da entrada analogica - 12 bits -> 4095 para 3.3V
BlockType ReadBlock(int analogPin);       // Identifica o bloco
void GameOverLayout();                    // Layout Tela de GameOver
bool ReadMuxChannel(int Channel);         // Controle e leitura dos pinos de selecao do mux
int getMuxChannel(int linha, int coluna); // Verificacao de saida do bloco
bool DesafioCheck(int DesafioAtualIndex); // Verificacao de desafio


// =============================
// --- Main Function ---
void setup()
{
  analogReadResolution(12);
  Serial.begin(9600);
  Serial.println("\n===========");
  Serial.println("DEBUG");
  Serial.print("===========");

  pinMode(RESET_PIN, INPUT_PULLUP);
  pinMode(MUX_PortA,OUTPUT);
  pinMode(MUX_PortB,OUTPUT);
  pinMode(MUX_PortC,OUTPUT);
  pinMode(MUX_OUT, INPUT_PULLDOWN);
  
  attachInterrupt(digitalPinToInterrupt(RESET_PIN),ReadResetButton,FALLING);

  lcd.init();
  lcd.clear();
  vTaskDelay(200/portTICK_PERIOD_MS);
  lcd.backlight();
  // =====================
  // --- Logo Overload ---
  for (int i = 0; i < 8; i++) 
    lcd.createChar(i, tiles[i].data);

  lcd.setCursor(6,0);
  lcd.print("OVERLOAD");

  lcd.setCursor(8, 1); lcd.write(byte(0));  
  lcd.setCursor(10,1); lcd.write(byte(2)); 

  lcd.setCursor(8, 2); lcd.write(byte(1)); 
  lcd.setCursor(9, 2); lcd.write(byte(4));  
  lcd.setCursor(10,2); lcd.write(byte(3)); 

  lcd.setCursor(8, 3); lcd.write(byte(5));  
  lcd.setCursor(9, 3); lcd.write(byte(6));  
  lcd.setCursor(10,3); lcd.write(byte(7)); 
  vTaskDelay(1000/portTICK_PERIOD_MS);
  lcd.clear();

  xTaskCreate(MenuDisp,"MenuDisp",8192,NULL,2,NULL);
  xTaskCreate(Timer,"Timer",2048,NULL,1,NULL);
  xTaskCreate(tabuleiro,"tabuleiro",8192,NULL,2,NULL);
}// end Setup

void loop() 
{
  vTaskDelay(100/portTICK_PERIOD_MS);
}// end Loop

// ======================
// --- Functions ---

// ========================
// --- Timer ---
void Timer(void *parameters)
{
  vTaskDelay(200 / portTICK_PERIOD_MS); 
  while(1)
  {
    unsigned long tempAtual = millis();

    if(ResetPinPressed)
    {
      Minutes  = 4;
      Seconds  = 59;
      pontos   = 0;
      cont     = 0;
      GameOver = false;
      ResetPinPressed    = false;
      Start_menu_display = true;  
      DesafioResolvido   = false;
      Serial.print("\nReset Button Pressed!");
    }//end if
    if (tempAtual - tempoAnterior >= intervalo)
    {
      tempoAnterior = tempAtual;

      if(Seconds == 0)
      {
        if(Minutes > 0)
        {
          Minutes--;
          Seconds = 59;
        }// end if aninhado
        else
        {
          Minutes  = 0;
          Seconds  = 0;
          GameOver = true;
        }// end else
      }// end if aninhado
      else Seconds--;
    }//end if
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }// end While
}// end Timer

// ========================
// --- ReadResetButton ---
void IRAM_ATTR ReadResetButton()
{
  static unsigned long lastMillis = 0;
  unsigned long newMillis = millis();
  if(newMillis - lastMillis < 100)
  {
  }// end if
  else if(digitalRead(RESET_PIN) == LOW)
  {
    lastMillis = newMillis;
    ResetPinPressed = true;

  }//end else if
}// end ReadButton

// ========================
// --- GameOver layout ---
void GameOverLayout()
{
  lcd.clear();
  lcd.setCursor(11,0);
  lcd.print("OVERLOAD");

  lcd.setCursor(13, 1); lcd.write(byte(0));  
  lcd.setCursor(15,1); lcd.write(byte(2)); 

  lcd.setCursor(13, 2); lcd.write(byte(1)); 
  lcd.setCursor(14, 2); lcd.write(byte(4));  
  lcd.setCursor(15,2); lcd.write(byte(3)); 

  lcd.setCursor(13, 3); lcd.write(byte(5));  
  lcd.setCursor(14, 3); lcd.write(byte(6));  
  lcd.setCursor(15,3); lcd.write(byte(7)); 
  lcd.setCursor(0, 1);
  lcd.print("GameOver!");
  lcd.setCursor(0, 2);
  lcd.print("Score:");
  lcd.print(pontos);
}// end GameOverLayout

// ========================
// --- MenuDisp ---
void MenuDisp(void *parameters)
{
  vTaskDelay(200/portTICK_PERIOD_MS); 
  while(1)
  {
    if(GameOver)
    {
      if(Start_menu_display)
      {
        GameOverLayout();
        Start_menu_display = false;
      }// end if aninhado
      digitalWrite(LedOUTPUT, HIGH);
      vTaskDelay(250 / portTICK_PERIOD_MS);
      digitalWrite(LedOUTPUT, LOW);
      vTaskDelay(250 / portTICK_PERIOD_MS);
    }//end if
    else
    {
      if(DesafioResolvido)
      {
        DesafioResolvido = false;
        if(cont == (TamMenu - 1))
          GameOver = true;
        else
          cont++;
        Start_menu_display = true;
      }
      if(Start_menu_display)
      {
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print(Menu[cont]);
        lcd.setCursor(0,1);
        lcd.print(DescriptionLine02[cont]);
        lcd.setCursor(0,2);
        lcd.print(DescriptionLine03[cont]);
        lcd.setCursor(0,3);
        lcd.print("Score:");
        Start_menu_display = false;
      }//end if

      lcd.setCursor(15,3); 
      if(Minutes <10)lcd.print("0");
      lcd.print(Minutes);
      lcd.print(":");
      if(Seconds < 10)lcd.print("0");
      lcd.print(Seconds);
        
      lcd.setCursor(6,3); 
      if(pontos<10)lcd.print("0");
      lcd.print(pontos);
      lcd.print(" ");

      if((Minutes == 0) && (Seconds == 0) )
      {
        lcd.setCursor(15,3); 
        lcd.print("00:00");
        vTaskDelay(200/portTICK_PERIOD_MS);
        lcd.setCursor(15,3); 
        lcd.print("  :  ");
        vTaskDelay(200/portTICK_PERIOD_MS);
        Start_menu_display = true;
      }// end if
      vTaskDelay(200/portTICK_PERIOD_MS);
    }// end else
    }//end while
}// end MenuDisp;

// ========================
// --- Score ---
void Score()
{
  if(desafio01)
  {
    pontos += 5;
    desafio01 = false;
  }// end if
  else if(desafio02)
  {  
    pontos += 5;
    desafio02 = false;
  }// end else if
  else if(desafio03)
  {  
    pontos += 5;
    desafio03 = false;
  }// end else if
  else if(desafio04)
  {  
    pontos += 15; 
    desafio04 = false;
  }// end else if
  else if(desafio05)
  {  
    pontos += 30; 
    desafio05 = false;
  }// end else if
  else if(desafio06)
  {  
    pontos += 40; 
    desafio06 = false;
  }// end else if
}//end Score

// ======================
// --- ReadMuxChannel ---
bool ReadMuxChannel(int Channel)
{
  digitalWrite(MUX_PortA, (Channel & 0x01));
  digitalWrite(MUX_PortB, (Channel & 0x02) >> 1);
  digitalWrite(MUX_PortC, (Channel & 0x04) >> 2);

  vTaskDelay(1 / portTICK_PERIOD_MS);

  return digitalRead(MUX_OUT);
}// end ReadMuxChannel

// ======================
// --- getMuxChannel ---
int getMuxChannel(int linha, int coluna)
{
  return (linha * col) + coluna; 
}// end getMuxChannel

// ======================
// --- DesafioCheck ---
bool DesafioCheck(int DesafioAtualIndex)
{

  if( (DesafioAtualIndex == 0 && desafio01 ) ||
      (DesafioAtualIndex == 1 && desafio02 ) ||
      (DesafioAtualIndex == 2 && desafio03 ) ||
      (DesafioAtualIndex == 3 && desafio04 ) ||
      (DesafioAtualIndex == 4 && desafio05 ) ||
      (DesafioAtualIndex == 5 && desafio06 ) )
      return false;

  Serial.println("\nVerificando Desafio...");

  // Verificacao se existe um bloco na primeira posicao, se nao tiver o jogo nao funciona
  bool FirstBlock = false;
  for(int i = 0; i < lin; i++)
  {
    if(tabuleir[i][0] != BlockType::EMPTY)
    {
      FirstBlock = true;
      break;
    }// end if
  }// end for

  if(!FirstBlock)
  {
    Serial.println("\n FALHA CRITICA: Nao foi encontrado NENHUM Bloco!");
    return false;
  }// end if

  bool DesafioConcluido = false;
  bool OutMux_Atual = false;
  
  switch(DesafioAtualIndex)
  {
    case 0:
      Serial.println("Verificando Desafio 01 (PORTA AND)...");
      for(int i = 0; i < lin ; i++)
      {
        if(tabuleir[i][0] == BlockType::AND_GATE )
        {
          int channelToRead = getMuxChannel(i, 0);
          OutMux_Atual = ReadMuxChannel(channelToRead);
          if(OutMux_Atual == HIGH)
          {
            DesafioConcluido = true;
            break; 
          }//end if aninhado
        }// end if
      }
      break;
    case 1:
      Serial.println("Verificando Desafio 02 (PORTA OR)...");
      for(int i = 0; i < lin; i++)
      {
        if(tabuleir[i][0] == BlockType::OR_GATE)
        {
          int channelToRead = getMuxChannel(i, 0);
          OutMux_Atual = ReadMuxChannel(channelToRead);
          if(OutMux_Atual == HIGH)
          {
            DesafioConcluido = true;
            break;
          }
        }// end if
      }// end for 
      break;
    case 2:
      Serial.println("Verificando Desafio 03 (PORTA NOT)...");
      for(int i = 0; i < lin; i++)
      {
        if(tabuleir[i][0] == BlockType::NOT_GATE)
        {
          int channelToRead = getMuxChannel(i, 0);
          OutMux_Atual = ReadMuxChannel(channelToRead);
          if(OutMux_Atual == LOW)
          {
            DesafioConcluido = true;
            break;
          }// end if aninhado
        }// end if
      }// end for 
      break;
    case 3:
      Serial.println("Verificando Desadfio 04 (PORTA AND e OR)...");
      for(int i = 0; i < lin; i++)
      {
        bool CombinationAND_OR = (tabuleir[i][0] == BlockType::AND_GATE && 
                                  tabuleir[i][1] == BlockType::OR_GATE);

        bool CombinantionOR_AND = (tabuleir[i][0] == BlockType::OR_GATE && 
                                   tabuleir[i][1] == BlockType::AND_GATE);

        if(CombinationAND_OR || CombinantionOR_AND)
        {
          int channelToRead = getMuxChannel(i, 1);
          OutMux_Atual = ReadMuxChannel(channelToRead);
          
          if(OutMux_Atual == HIGH)
          {
            DesafioConcluido = true;
            break;
          }// end if aninhado
        }// end if
      }//end for
      break;
    case 4:
      Serial.println("Verificando Desadfio 05 (PORTA AND, OR e NOT)...");
      for(int i = 0; i < lin; i++)
      {
        bool temAND = false;
        bool temOR  = false;
        bool temNOT = false;
        for(int j = 0; j < 3; j++)
        {
          if (tabuleir[i][j] == BlockType::AND_GATE) temAND = true;
          if (tabuleir[i][j] == BlockType::OR_GATE)  temOR  = true;
          if (tabuleir[i][j] == BlockType::NOT_GATE) temNOT = true;
          Serial.println("==============");
          Serial.println(temAND);
          Serial.println(temOR);
          Serial.println(temNOT);
          Serial.println("==============");
        }// end for
  
        if(temAND && temOR && temNOT)
        { 
          int ChannelToRead = getMuxChannel(i, 2);
          OutMux_Atual = ReadMuxChannel(ChannelToRead);
          if(OutMux_Atual == HIGH)
          {
            DesafioConcluido = true;
            break;
          }// end if aninhado
        }// end if
      }//end for
      break;
    default:
      Serial.println("Desafio Desconhecido ou Nao Implementado.");
      return false;
  }// end switch

  if(DesafioConcluido)
  {
    Serial.println("\n Desafio Concluido!");
    if(DesafioAtualIndex == 0)      desafio01 = true;
    else if(DesafioAtualIndex == 1) desafio02 = true;
    else if(DesafioAtualIndex == 2) desafio03 = true;
    else if(DesafioAtualIndex == 3) desafio04 = true;
    else if(DesafioAtualIndex == 4) desafio05 = true;
    else if(DesafioAtualIndex == 5) desafio06 = true;

    Score();
    DesafioResolvido = true;
    return true;
  }// end if
  else  return false;
}// end DesafioCheck

// ======================
// --- tabuleiro ---
void tabuleiro(void *parameters)
{
  vTaskDelay(200 / portTICK_PERIOD_MS); 
  while(1)
  {
    Serial.println("\nTabuleiro State:");
    Serial.println("================");
    for(int i = 0; i < lin; i++)
    {
      for(int j = 0; j < col; j++)
      {
        int currentAnalogPin = boardAnalogPins[i][j];
        int rawADC = analogRead(currentAnalogPin);
        float voltage = AnalogVoltage(rawADC);
        BlockType detectedType = ReadBlock(currentAnalogPin);

        tabuleir[i][j] = detectedType; 

        const char* blockTypeString;
        switch (detectedType) 
        {
            case BlockType::EMPTY: blockTypeString = "VAZIO"; break;
            case BlockType::AND_GATE: blockTypeString = "PORTA_AND"; break;
            case BlockType::OR_GATE: blockTypeString = "PORTA_OR"; break;
            case BlockType::NOT_GATE: blockTypeString = "PORTA_NOT"; break;
            case BlockType::UNKNOWN: blockTypeString = "DESCONHECIDO"; break;
        }// end switch case
        Serial.printf("(%d,%d): Pin %-2d - Volt: %-5.2fV - %-13s | ",
                      i, j, currentAnalogPin, voltage, blockTypeString);
      }// end for aninhado
      Serial.print("\n"); 
    }// end for
    Serial.println("================");

    DesafioCheck(cont);

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }// end while
}// end tabuleiro

// ==================================
// --- AnalogVoltage ---
float AnalogVoltage(int Voltage)
{
  return ((float)Voltage * 3.3) / 4095.0;
}// end AnalogVoltage

// ======================
// --- ReadBlock ---
BlockType ReadBlock(int analogPin)
{
    int analogReadValue = analogRead(analogPin); 
    float blockVoltage = AnalogVoltage(analogReadValue); 

    if (blockVoltage >= 0.0 && blockVoltage < 0.3)  
      return BlockType::EMPTY;
    else if (blockVoltage >= 0.3 && blockVoltage < 0.7) // 220 ohm
      return BlockType::AND_GATE;
    else if (blockVoltage >= 0.7 && blockVoltage < 1.2) // 470 ohm
      return BlockType::OR_GATE;
    else if (blockVoltage >= 1.2 && blockVoltage < 2.0) // 1k
      return BlockType::NOT_GATE;
    else
      return BlockType::UNKNOWN;
}// end ReadBlock










/*===================================================================================================================================================================
                                                             
                                       _                      
                                      / \                     
                                     |oo >                    
                                     _\=/_                    
                    ___         #   /  _  \   #               
                   /<> \         \\//|/.\|\\//                
                 _|_____|_        \/  \_/  \/                 
                | | === | |          |\ /|                    
                |_|  0  |_|          \_ _/                    
                 ||  0  ||           | | |                    
                 ||__*__||           | | |                    
                |* \___/ *|          []|[]                    
                /=\ /=\ /=\          | | |                    
________________[_]_[_]_[_]_________/_]_[_\_______________________________
                                                              
                                                              
===================================================================================================================================================================== */

// ==================================
// --- End of Program ---
