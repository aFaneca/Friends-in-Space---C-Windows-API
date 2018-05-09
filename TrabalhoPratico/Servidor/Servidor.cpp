#include "cabecalho.h"



#define BufferSize 100
#define Buffers 10

#define FILA_SIZE           10
#define FILA_END     (FILA_SIZE - 1)




// Comunicação entre o Servidor e o Gateway


#define BLK_SIZE        100
struct item {                   // os registos na fila
	int    block_num;             // o número de sequencia do bloco
	size_t data_len;        // tamanho dos dados no bloco
	char   data[BLK_SIZE + 1];   // a informação no bloco
};
bool atEOF;          // para saber quando o ficheiro chegou ao fim


struct fila {   
	int  topo;//inicio da fila
	int  fim;  // fim da fila
	long cnt; // nr de registos na fila
	struct item registo[FILA_SIZE];
} f;



//



void init(jogo *j) {
	// Inicialização do jogo
	j->nInvasores = 0;
	j->nDefensores = 0;
	j->nBombas = 0;
	j->nTiros = 0;
	j->nPowerups = 0;

	// Configuração da nave defensora
	adicionarDefensor(j, 'D', 1, 1, 0, 0, 1);
	
	// Configuração das naves invasoras
	adicionarInvasor(j, 'B', 1, 1, 1, 0, 1, 5);
	adicionarInvasor(j, 'E', 1, 1, 2, 0, 1, 5);
	adicionarInvasor(j, 'B', 2, 2, 5, 6, 1, 5); //ID: 3
}


int verificaAlvo(jogo *j, int x, int y, char alvo) {
	// Verificar se o ponto (x,y) se encontra dentro de um retangulo pertencente a uma nave do tipo pretendido ('I' ou 'D')
	int encontrouNave = -1;

	// PROCURA POR INVASORES
	for (int i = 0; i < j->nInvasores; i++) {
		if ((x >= j->invasores[i].posx) && x <= (j->invasores[i].posx + j->invasores[i].largura) &&
			y <= j->invasores[i].posy && y >= (j->invasores[i].posy - j->invasores[i].altura)) {
			encontrouNave = j->invasores[i].id;
			break;
		}
	}

	// PROCURA POR DEFENSORES
	for (int i = 0; i < j->nDefensores; i++) {
		if ((x >= j->defensores[i].posx) && x <= (j->defensores[i].posx + j->defensores[i].largura) &&
			y <= j->defensores[i].posy && y >= (j->defensores[i].posy - j->defensores[i].altura)) {
			encontrouNave = j->invasores[i].id;
			break;
		}
	}
	
	return encontrouNave; // if == -1 <- Não encontrou nave; else -> == nr. do id da nave encontrada
}

char verificaPosicao(jogo *j, int x, int y) { // Verificar se a posição já se encontra ocupada e se sim, porque tipo de objeto
	char objetoEncontrado = 'N'; 

	// PROCURA POR INVASORES
	for (int i = 0; i < j->nInvasores; i++) {
		if ((x >= j->invasores[i].posx) && x <= (j->invasores[i].posx + j->invasores[i].largura) &&
			y <= j->invasores[i].posy && y >= (j->invasores[i].posy - j->invasores[i].altura)) {
			objetoEncontrado = j->invasores[i].tipo;
			break;
		}
	}

	// PROCURA POR DEFENSORES
	for (int i = 0; i < j->nDefensores; i++) {
		if ((x >= j->defensores[i].posx) && x <= (j->defensores[i].posx + j->defensores[i].largura) &&
			y <= j->defensores[i].posy && y >= (j->defensores[i].posy - j->defensores[i].altura)) {
			objetoEncontrado = j->defensores[i].tipo;
			break;
		}
	}

	return objetoEncontrado; // 'N' <- Nenhum | 'I' <- Nave Invasora | 'B' ou 'E' <- Nave Defensora
}

void adicionarDefensor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade) {
	static int i = 0;
	j->defensores[i].id = i;
	j->defensores[i].tipo = tipo;
	j->defensores[i].altura = altura;
	j->defensores[i].largura = largura;
	j->defensores[i].posx = posx;
	j->defensores[i].posy = posy;
	j->defensores[i].velocidade = velocidade;
	j->nDefensores++;
	i++;
}

void adicionarInvasor(jogo *j, char tipo, int altura, int largura, int posx, int posy, int velocidade, int resistencia) {
	static int i = 0;
	j->invasores[i].id = i;
	j->invasores[i].tipo = tipo;
	j->invasores[i].altura = altura;
	j->invasores[i].largura = largura;
	j->invasores[i].posx = posx;
	j->invasores[i].posy = posy;
	j->invasores[i].velocidade = velocidade;
	j->invasores[i].resistencia = resistencia;
	j->nInvasores++;
	i++;
}

void removerNave(jogo *j, char tipo, int indice) {

	if (tipo == 'D') {
		int n = j->nDefensores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j->defensores[c] = j->defensores[c + 1];

		j->nDefensores--;
	}
	else {
		int n = j->nInvasores;

		/*for (int c = indice - 1; c < n - 1; c++)
		j->naves[c] = j->naves[c + 1];*/
		for (int c = indice; c < n - 1; c++)
			j->invasores[c] = j->invasores[c + 1];

		j->nInvasores--;
	}
	
}


//int contaNaves(jogo *j, char tipo) {
//	int contagem = 0;
//
//	for (int i = 0; i < j->nNaves; i++) {
//		if (j->naves[i].tipo == tipo)
//			contagem++;
//	}
//
//	return contagem;
//}

void mostraInfo(jogo *j) {

	_tprintf(TEXT("\n### JOGO ###\n"));
	_tprintf(TEXT("\nNr. de Naves: %d"), j->nDefensores + j->nInvasores);
	_tprintf(TEXT("\n\t-> Naves Defensoras: %d"), j->nDefensores);

	for (int i = 0; i < j->nDefensores; i++)
			_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->defensores[i].id, j->defensores[i].posx, j->defensores[i].posy);
	

	_tprintf(TEXT("\n\t-> Naves Invasoras: %d"), j->nInvasores);
	for (int i = 0; i < j->nInvasores; i++)
			_tprintf(TEXT("\n\t\t=> NAVE %d (%d, %d)"), j->invasores[i].id, j->invasores[i].posx, j->invasores[i].posy);
	
	_tprintf(TEXT("\nNr. de Bombas: %d"), j->nBombas);
	_tprintf(TEXT("\nNr. de Tiros: %d"), j->nTiros);
	_tprintf(TEXT("\nNr. de Powerups: %d\n"), j->nPowerups);
}

void alterarPosicao(jogo *j, char tipo, int id, int novoX, int novoY) { // Alterar posição de uma nave (identificada pelo ID)
	// NESTA FASE, AS VERIFICAÇÕES DE ACORDO COM A NAVE E COM A NOVA POSIÇÃO A OCUPAR JÁ TÊM QUE TER SIDO FEITAS
	
	if (tipo == 'D') {
		for (int i = 0; i < j->nDefensores; i++) {
			if (j->defensores[i].id == id) {
				j->defensores[i].posx = novoX;
				j->defensores[i].posy = novoY;
			}
		}
	}
	else {
		for (int i = 0; i < j->nInvasores; i++) {
			if (j->invasores[i].id == id) {
				j->invasores[i].posx = novoX;
				j->invasores[i].posy = novoY;
			}
		}
	}
	
}

TCHAR ** processaComando(TCHAR *comando, int *tamCMD) {

	TCHAR ** cmd = NULL;
	TCHAR **next_token = (TCHAR **)malloc(sizeof(TCHAR));


	TCHAR *  p = _tcstok_s(comando, TEXT(" "),next_token);
	int n_espacos = 0;

	while (p) { 	/* divide a string em palavras */
		cmd = (TCHAR **)realloc(cmd, sizeof(TCHAR*) * ++n_espacos);

		if (cmd == NULL)
			exit(-1); /* se alocação de memória falhar */

		cmd[n_espacos - 1] = p;
		p = _tcstok_s(NULL, TEXT(" "), next_token);
	}

	*tamCMD = n_espacos;

	return cmd;
}

void lerComandos(jogo *j) {
	TCHAR **cmd;
	TCHAR *comando = NULL;
	comando = (TCHAR *)malloc(sizeof(TCHAR) * 20);
	TCHAR *comandoCopia = NULL;
	comandoCopia = (TCHAR *)malloc(sizeof(TCHAR) * 20);
	int tamCMD = 0;

	while (0 == 0) {

		_tprintf(_T("> "));
		_fgetts(comandoCopia, 25, stdin);
		int i = 0;

		for (i = 0; i < _tcslen(comandoCopia) - 1; i++) {
			comando[i] = comandoCopia[i];
		}

		comando[_tcslen(comandoCopia) - 1] = '\0';

		cmd = processaComando(comando, &tamCMD);

		if (tamCMD != 0) {
			if (_tcscmp(cmd[0], TEXT("info")) == 0) {
				if (tamCMD == 1) {
					mostraInfo(j);
				}
				else {
					_tprintf(_T("Erro de Sintaxe. <info>\n"));
				}
			}
			else if (_tcscmp(cmd[0], _T("sair")) == 0) {
				break;
			}
			else {

				_tprintf(TEXT("Erro de Sintaxe.\n"));
			}
		}
		

		comando[0] = '\0';
		comandoCopia[0] = '\0';
	}
}



int _tmain()
{
#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	jogo *j = (jogo *)malloc(sizeof(jogo));
	init(j);

	lerComandos(j);



	/*removerNave(j, 'B', 1);
	mostraInfo(j);

	int testeX = 6;
	int testeY = 4;
	_tprintf(TEXT("A atacar posição (%d, %d)"), testeX, testeY);
	int resultado = verificaAlvo(j, testeX, testeY, 'I');

	if (resultado == -1) {
		_tprintf(TEXT("\nNão encontrou inimigos nessa posição."));
	}else {
		_tprintf(TEXT("\nEncontrou o inimigo %d\n"), resultado);
	}*/


	
	return 0;
}