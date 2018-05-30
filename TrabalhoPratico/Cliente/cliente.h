#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define MAXBOMBAS 50
#define MAXTIROS 50
#define MAXDEFENSORES 2
#define MAXINVASORES 20
#define MAXPOWERUPS 100
#define MAXJOGADORES 5
#define DIM_X 50
#define DIM_Y 50
#define TAM 200
#define PIPE_NAME TEXT("\\\\.\\pipe\\tpso2") 
#define NCLIENTES 10
#define TAM 256

typedef struct Invasor {
	int id;
	char tipo; // 'B'ásico ou 'E'squivo
	char nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;
	int resistencia;
	int direcaoDoMovimento; // 1 - esquerda para a direita | -1 - direita para a esquerda

} invasor;


typedef struct Defensor {
	int id;
	TCHAR tipo;
	TCHAR nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;
	int direcaoDoMovimento; // 1 - esquerda para a direita | -1 - direita para a esquerda

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

//jogo *j = (jogo *)malloc(sizeof(jogo)); <- a alocação é feita pelo próprio MapViewOfFile, logo esta linha era redundante
jogo *j;

OVERLAPPED ov;
HANDLE IOReady;
HANDLE hPipe;
TCHAR buf[256];
DWORD n;
HANDLE hClientes[NCLIENTES]; // Tabela de handles para diferentes clientes




void enviarParaNamedPipe(TCHAR * string, HANDLE h);