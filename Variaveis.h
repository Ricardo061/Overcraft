/*
 * Projeto Artbot - Arquivo .h - Definicao de Variaveis 
 * Author: Joao Ricardo Chaves
 * Date: 
 */

#ifndef VARIAVEIS
#define VARIAVEIS

// =============================
// --- Macros And Constants ---
#define TamMenu     5
#define LedOUTPUT   2

LiquidCrystal_I2C lcd(0x27,20,4);
const char *Menu[TamMenu] = {"Desafio 01","Desafio 02","Desafio 03","Desafio 04","Desafio 05"};
const char *DescriptionLine02[TamMenu] = {  "Acenda a lampada!",        // Desafio 1
                                            "Faca a luz brilhar!",      // Desafio 2
                                            "Ligue com truque!",        // Desafio 3
                                            "Combine os botoes!",       // Desafio 4
                                            "Desafio supremo!"};        // Desafio 6

const char *DescriptionLine03[TamMenu] = {  "Use os dois botoes",       // Desafio 1
                                            "Aperte um botao",          // Desafio 2
                                            "Tem que desligar?",        // Desafio 3
                                            "Descubra o segredo",       // Desafio 4
                                            "Resolva o circuito"};      // Desafio 6

// Desafio 01 -> and
// Desafio 02 -> or 
// Desafio 03 -> not
// Desafio 04 -> and e or
// Desafio 05 -> and or e not


// =============================
// --- BlockType ---
enum class BlockType 
{
  EMPTY,        // VAZIO
  AND_GATE,     // AND gate block
  OR_GATE,      // OR gate block
  NOT_GATE,     // NOT gate block
  UNKNOWN       // DESCONHECIDO
};

// ============================
// --- Controle de Flags ---
volatile bool ResetPinPressed    = false;
volatile bool Start_menu_display = true;    

volatile bool ButtonAPressed     = false;
volatile bool ButtonBPressed     = false;
volatile bool StateOut           = false;
volatile bool StateA             = false;
volatile bool StateB             = false;

volatile bool desafio01          = true;   // PORTA AND
volatile bool desafio02          = true;   // PORTA OR
volatile bool desafio03          = true;   // PORTA NOT
volatile bool desafio04          = false;   // PORTA AND E OR
volatile bool desafio05          = false;   // PORTA AND, OR e NOT
volatile bool desafio06          = false;

volatile bool GameOver           = false;   // FIM DE JOGO 
volatile bool DesafioResolvido   = false;   // DesafioResolvido

// ============================
// --- Definicao Tabuleiro ---
#define lin 3
#define col 3
const int boardAnalogPins[lin][col] = { {34, 39, 36},
                                        {33, 32, 35},
                                        {27, 26, 25} };
BlockType tabuleir[lin][col] = {{BlockType::EMPTY}};

// ============================
// --- Entradas e Botoes ---
const int MUX_PortA = 18;
const int MUX_PortB = 17;
const int MUX_PortC = 16;
const int MUX_OUT   = 23;

const int RESET_PIN = 19;

// ============================
// --- Score ---
volatile int pontos           = 0;
volatile int sumPontos        = 0;

// ========================
// --- Timer ---
volatile int Minutes          = 20,
             Seconds          = 59;

volatile int cont             = 3;
unsigned long tempoAnterior   = 0;
const unsigned long intervalo = 1000; 

// ========================
// --- Logo Overload --- 
typedef struct 
{
  byte data[8];
} CustomChar;

CustomChar tiles[8] = 
{
  // tile 0 - orelha esquerda
  {.data = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11000, 0b11000,0b11000}},
  // tile 1 - olho esquerda
  {.data = {0b11111, 0b11000, 0b11011, 0b11101, 0b11110, 0b11111, 0b11111, 0b11111}},
  // tile 2 - orelha direita
  {.data = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00011, 0b00011, 0b00011}},
  // tile 3 - olho direito
  {.data = {0b11111, 0b00011, 0b11011, 0b10111, 0b01111, 0b11111, 0b11111, 0b11111}},
  // tile 4 - centro do rosto
  {.data = {0b11111, 0b00000, 0b11111, 0b11111, 0b11111, 0b01110, 0b10101, 0b11011}},
  // tile 6 - mandíbula esquerda
  {.data = {0b11111, 0b01111, 0b00111, 0b00011, 0b00001, 0b00000, 0b00000, 0b00000}},
  // tile 7 - mandíbula centro
  {.data = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b01110, 0b00100}},
  // Mandibula direita
  {.data = {0b11111, 0b11110, 0b11100, 0b11000, 0b10000, 0b00000, 0b00000, 0b00000}}
};

#endif

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
