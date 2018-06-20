#pragma once


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
#define DIM_X 500
#define DIM_Y 500
#define TAM 200
#define VELOCIDADE_BASE_INVASORES 1
#define INTERVALO_MOVIMENTACAO 1000 //ms

// MANUTEN��O DO JOGO EM MEM�RIA PARTILHADA
TCHAR nomeDaMemoria[] = TEXT("EstruturaDoJogo");
TCHAR nomeDoMutex[] = TEXT("mutexComunicacao");
TCHAR nomeDoEventoComunicacao[] = TEXT("eventoComunicacao");



typedef struct Invasor {
	int id;
	char tipo; // 'B'�sico ou 'E'squivo
	TCHAR nome[50];
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
	TCHAR duracao; // 'P'ermanente ou 'T'empor�rio
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
jogo j;

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

DWORD thrEnviarJogo; //Id da thread a ser criada
HANDLE hTEnviarJogo;



void adicionarInvasor(char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia);
void adicionarDefensor(char tipo, int altura, int largura, int posx, int posy, int velocidade);
//int contaNaves(jogo *j, char tipo);
void mostraInfo();
char verificaPosicao(int x, int y);
void removerNave(char tipo, int indice);
void alterarPosicao(char tipo, int id, int novoX, int novoY);
void lerComandos();
TCHAR ** processaComando(TCHAR *comando, int *tamCMD);
void iniciarJogo();
DWORD WINAPI GerirJogadores(LPVOID param);
DWORD WINAPI GerirBasicos(LPVOID param);
DWORD WINAPI GerirEsquivos(LPVOID param);
DWORD WINAPI GerirTirosBombasPowerups(LPVOID param);
DWORD WINAPI GerirTempEfeitos(LPVOID param);
DWORD WINAPI enviarJogo(LPVOID param);
int getPontuacao(TCHAR *nomeDoJogador);
void guardarPontuacao(TCHAR *nomeDoJogador);
void adicionarJogador(TCHAR *nome, TCHAR *pword);
int getPontuacao(TCHAR *nomeDoJogador);
int recuperarPontuacao(TCHAR *nomeDoJogador);
int pushInfoDoJogo();
// COMUNICA��O COM GATEWAY (ATRAV�S DA DLL)
HINSTANCE hDLL; // Handle para a DLL
void(*ler)(); // para receber o m�todo recebe() da DLL
void(*escrever)(TCHAR*); // para receber o m�todo escrever() da DLL

// PUSH INFO DO JOGO
HANDLE hMapFile;
LPCTSTR pBuf;