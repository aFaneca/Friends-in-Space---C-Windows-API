#pragma once

#include "resource.h"


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
#define TAM_JANELA_X 1200
#define TAM_JANELA_Y 800
#define DIM_X TAM_JANELA_X - 50
#define DIM_Y TAM_JANELA_Y - 50
#define TAM 200
#define PIPE_NAME TEXT("\\\\.\\pipe\\tpso2") 
#define NCLIENTES 10
#define TAM 256

typedef struct Invasor {
	int id;
	char tipo; // 'B'�sico ou 'E'squivo
	TCHAR nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;
	int resistencia;
	int direcaoDoMovimento; // 1 - esquerda para a direita | -1 - direita para a esquerda
	int movimentos;

} invasor;


typedef struct Defensor {
	int id;
	TCHAR tipo;
	TCHAR nome[50];
	int posx, posy;
	int largura, altura;
	int velocidade;
	int direcaoDoMovimento; // 1 - esquerda para a direita | -1 - direita para a esquerda
	int idDosPowerups[100];
} defensor;

typedef struct Bomba {
	int posx, posy;
	int largura;
	int altura;
	int velocidade;
	int acertou; // 0 - n�o | 1 - acertou em algo
} bomba;

typedef struct Tiro {
	int posx, posy;
	int largura;
	int altura;
	int velocidade;
	int acertou; // 0 - n�o | 1 - acertou em algo
} tiro;


typedef struct Powerup {
	TCHAR nome[50];
	TCHAR duracao; // 'P'ermanente ou 'T'empor�rio
	TCHAR ocorrencia; // 'V'ulgar ou 'I'nvulgar
	int posx, posy;
	int largura, altura;
	int velocidade;
	int acertou;
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
jogo j;

//jogo *j = (jogo *)malloc(sizeof(jogo));
//jogo *j;

OVERLAPPED ov;
HANDLE IOReady;
HANDLE hPipe;
TCHAR buf[256];
DWORD n;
HANDLE hClientes[NCLIENTES]; // Tabela de handles para diferentes clientes




//void enviarPorNamedPipeParaClientes(TCHAR * string, HANDLE h);
void enviarParaNamedPipe(TCHAR * string1, HANDLE h);
TCHAR * lerDoNamedPipe(HANDLE h);
void lerEstruturaDeJogoDoNamedPipe(HANDLE h);
DWORD WINAPI lerJogo(LPVOID param);
void inicializa();
void desenhaNaves(HWND hWnd, HDC auxDC);

BITMAP bm;
HDC hdcMem;
HBITMAP hbmOld;



HDC hdc = NULL;
HDC auxDC = NULL;
HBRUSH bg = NULL;
int nX = 0, nY = 0;


HBITMAP auxBM = NULL;
HBITMAP hNaveBasica = NULL;
HBITMAP hNaveDefensora = NULL;
HBITMAP hNaveEsquiva = NULL;
HBITMAP hPowerUp = NULL;
HBITMAP hTiro = NULL;
HBITMAP hBomba = NULL;
BITMAP bmNave;
BITMAP bmTiro;
BITMAP bmPowerUp;
BITMAP bmBomba;
HDC hdcBasica;
HDC hdcPowerUp;
HDC hdcDefensora;
HDC hdcEsquiva;
HDC hdcTiro;
HDC hdcBomba;