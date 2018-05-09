#pragma once
#define UNICODE
#define _UNICODE

#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdbool.h>
#include <tchar.h>

#define MAXBOMBAS 50
#define MAXTIROS 50
#define MAXDEFENSORES 2
#define MAXINVASORES 20
#define MAXPOWERUPS 100
#define GRELHA_X 50
#define GRELHA_Y 50

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
	char tipo; 
	char nome[50];
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
	char nome[50];
	char duracao; // 'P'ermanente ou 'T'emporário
	char ocorrencia; // 'V'ulgar ou 'I'nvulgar
	int posx, posy;
	int largura, altura;
	int velocidade;
} powerup;

typedef struct CampoDeJogo {
	int grelha[GRELHA_X][GRELHA_Y];
	defensor defensores[MAXDEFENSORES];
	invasor invasores[MAXINVASORES];
	bomba bombas[MAXBOMBAS];
	tiro tiros[MAXTIROS];
	powerup powerups[MAXPOWERUPS];
	int dificuldade, nInvasores, nDefensores, nBombas, nTiros, nPowerups;
	
}jogo;
void adicionarInvasor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia);
void adicionarDefensor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade);
//int contaNaves(jogo *j, char tipo);
void mostraInfo(jogo *j);
char verificaPosicao(jogo *j, int x, int y);
void removerNave(jogo *j, char tipo, int indice);
void alterarPosicao(jogo *j, char tipo, int id, int novoX, int novoY);
void lerComandos();
TCHAR ** processaComando(TCHAR *comando, int *tamCMD);