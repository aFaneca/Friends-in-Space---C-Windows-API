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
#define MAXJOGADORES 5
#define GRELHA_X 50
#define GRELHA_Y 50
#define TAM 200

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
	int grelha[GRELHA_X][GRELHA_Y];
	defensor defensores[MAXDEFENSORES];
	invasor invasores[MAXINVASORES];
	bomba bombas[MAXBOMBAS];
	tiro tiros[MAXTIROS];
	powerup powerups[MAXPOWERUPS];
	jogador jogadores[MAXJOGADORES];
	int dificuldade, nInvasores, nDefensores, nBombas, nTiros, nPowerups, nJogadores;
	
}jogo;

DWORD thrBasicosId; //Id da thread a ser criada
HANDLE hTBasicos;

DWORD thrEsquivosId; //Id da thread a ser criada
HANDLE hTEsquivos;

DWORD thrTirosBombasPowerupsId; //Id da thread a ser criada
HANDLE hTTirosBombasPowerups;

DWORD thrTempEfeitosId; //Id da thread a ser criada
HANDLE hTTempEfeitos;

DWORD thrJogadoresId; //Id da thread a ser criada
HANDLE hTJogadores;



void adicionarInvasor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia);
void adicionarDefensor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade);
//int contaNaves(jogo *j, char tipo);
void mostraInfo(jogo *j);
char verificaPosicao(jogo *j, int x, int y);
void removerNave(jogo *j, char tipo, int indice);
void alterarPosicao(jogo *j, char tipo, int id, int novoX, int novoY);
void lerComandos(jogo *j);
TCHAR ** processaComando(TCHAR *comando, int *tamCMD);
void iniciarJogo(jogo *j);
DWORD WINAPI GerirJogadores(LPVOID param);
DWORD WINAPI GerirBasicos(LPVOID param);
DWORD WINAPI GerirEsquivos(LPVOID param);
DWORD WINAPI GerirTirosBombasPowerups(LPVOID param);
DWORD WINAPI GerirTempEfeitos(LPVOID param);
int getPontuacao(jogo *j, TCHAR *nomeDoJogador);
void guardarPontuacao(jogo *j, TCHAR *nomeDoJogador);
void adicionarJogador(jogo *j, TCHAR *nome, TCHAR *pword);
int getPontuacao(jogo *j, TCHAR *nomeDoJogador);
int recuperarPontuacao(jogo *j, TCHAR *nomeDoJogador);