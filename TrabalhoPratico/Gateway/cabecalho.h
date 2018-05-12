#pragma once

#include <Windows.h>
#include <tchar.h>


#define MAXBOMBAS 50
#define MAXTIROS 50
#define MAXDEFENSORES 2
#define MAXINVASORES 20
#define MAXPOWERUPS 100
#define MAXJOGADORES 5
#define DIM_X 50
#define DIM_Y 50
#define TAM 200


// MANUTENÇÃO DO JOGO EM MEMÓRIA PARTILHADA
//#define BASE_MEM     (VOID*)0x01000000
#define BUF_SIZE 25600
TCHAR nomeDaMemoria[] = TEXT("EstruturaDoJogo");
TCHAR nomeDoMutex[] = TEXT("mutexComunicacao");
TCHAR nomeDoEventoComunicacao[] = TEXT("eventoComunicacao");
//TCHAR szMsg[] = TEXT("Message from process1.");

typedef struct Invasor {
	int id;
	char tipo; // 'B'ásico ou 'E'squivo
	char nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;
	int resistencia;

} invasor;


typedef struct Defensor {
	int id;
	TCHAR tipo;
	TCHAR nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;

} defensor;

typedef struct Bomba {
	int posx, posy;
	int largura;
	int altura;
	int velocidade;
} bomba;

typedef struct Tiro {
	int posx, posy;
	int largura;
	int altura;
	int velocidade;
} tiro;


typedef struct Powerup {
	TCHAR nome[50];
	TCHAR duracao; // 'P'ermanente ou 'T'emporário
	TCHAR ocorrencia; // 'V'ulgar ou 'I'nvulgar
	int posx, posy;
	int largura, altura;
	int velocidade;
} powerup;

typedef struct Jogador {
	TCHAR nome[50];
	TCHAR pword[10];
	int pontuacao;
}jogador;


typedef struct CampoDeJogo {
	defensor defensores[MAXDEFENSORES];
	invasor invasores[MAXINVASORES];
	bomba bombas[MAXBOMBAS];
	tiro tiros[MAXTIROS];
	powerup powerups[MAXPOWERUPS];
	jogador jogadores[MAXJOGADORES];
	int dificuldade, nInvasores, nDefensores, nBombas, nTiros, nPowerups, nJogadores;

}jogo;

//jogo *j = (jogo *)malloc(sizeof(jogo));
jogo *j;


HINSTANCE hDLL; // Handle para a DLL
void(*ler)(); // para receber o método recebe() da DLL
void(*escrever)(TCHAR*); // para receber o método escrever() da DLL

