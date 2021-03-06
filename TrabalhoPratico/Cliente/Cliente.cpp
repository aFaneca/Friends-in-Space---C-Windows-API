// Cliente.cpp: define o ponto de entrada para o aplicativo.
//

#include "Cliente.h"

#define MAX_LOADSTRING 100

// Variáveis Globais:
HINSTANCE hInst;                                // instância atual
WCHAR szTitle[MAX_LOADSTRING] = _T("FRIENDS IN SPAPCE");                  // O texto da barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // o nome da classe da janela principal

// Declarações de encaminhamento de funções incluídas nesse módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    FimDoJogo(HWND, UINT, WPARAM, LPARAM);

int xAtual = 0;
int yAtual = 0;


void enviarParaNamedPipe(TCHAR * string1, HANDLE h) {
	ZeroMemory(&ov, sizeof(ov));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;

	//_tprintf(TEXT("Escrever no pipe %d"), h);
	if (!WriteFile(h, string1, TAM * sizeof(TCHAR), &n, &ov)) {
		//_tprintf(TEXT("[ERRO] Escrever no pipe %d! (WriteFile)\n"));
		exit(-1);
	}


	GetOverlappedResult(h, &ov, &n, FALSE);
}

TCHAR * lerDoNamedPipe(HANDLE h) {
	TCHAR mensagemLida[256];
	ZeroMemory(&ov, sizeof(ov));
	ZeroMemory(&n, sizeof(n));
	ZeroMemory(mensagemLida, sizeof(mensagemLida));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;


	ReadFile(h, mensagemLida, sizeof(mensagemLida), &n, &ov);
	mensagemLida[n / sizeof(TCHAR)] = '\0';

	WaitForSingleObject(IOReady, INFINITE);
	GetOverlappedResult(h, &ov, &n, FALSE);

	return mensagemLida;
}

// LER ESTRUTURA DO JOGO PELO NAMED PIPE
void lerEstruturaDeJogoDoNamedPipe(HANDLE h) {

	TCHAR mensagemLida[256];
	ZeroMemory(&ov, sizeof(ov));
	ZeroMemory(&n, sizeof(n));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;


	ReadFile(h, &j, sizeof(j), &n, &ov);
	//
	
	HWND hWnd = FindWindow(NULL, szTitle); // Encontra a referência para a janela
	InvalidateRect(hWnd, NULL, TRUE); // Atualiza os seus dados
	//
	//mensagemLida[n / sizeof(TCHAR)] = '\0';
	//_tprintf(_T("\nNAVE: (%d, %d) TOTAL: %d\n"), j->defensores[0].posx, j->defensores[0].posy, j->nDefensores);
	WaitForSingleObject(IOReady, INFINITE);
	GetOverlappedResult(h, &ov, &n, FALSE);
}


DWORD WINAPI lerJogo(LPVOID param) { // Está constantemente a ler do gateway a versão mais atualizada da estrutura de jogo

	HANDLE hPipe = (HANDLE)param;

	while (1) {
		lerEstruturaDeJogoDoNamedPipe(hPipe);

		/*if (!n) {
		_tprintf(TEXT("[LEITOR] %d... (ReadFile)\n"), n);
		break;
		}*/
		//_tprintf(_T("\nNAVE: (%d, %d) TOTAL: %d\n"), j.invasores[0].posx, j.invasores[0].posy, j.nInvasores);
	}


	return 0;
}

HANDLE  hTLerJogo;

void inicializa() {
	//HANDLE hPipe;
	#ifdef UNICODE
		_setmode(_fileno(stdin), _O_WTEXT);
		_setmode(_fileno(stdout), _O_WTEXT);
	#endif

	// TEMPORÁRIO
	//_tprintf(_T(">> Prima qualquer tecla para iniciar o CLIENTE...\n"));
	//_gettch();
	//

	//_tprintf(TEXT("[CLIENTE] Esperar pelo pipe '%s' (WaitNamedPipe)\n"), PIPE_NAME);
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		//_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (WaitNamedPipe)\n"), PIPE_NAME);
		exit(-1);
	}

	//_tprintf(TEXT("[CLIENTE] Ligação ao pipe do escritor... (CreateFile)\n"));
	hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0 | FILE_FLAG_OVERLAPPED, NULL);
	if (hPipe == NULL) {
		//_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}
	//_tprintf(TEXT("[CLIENTE] Liguei-me...\n"));

	// Mudar modo de funcionamento do pipe para PIPE_READMODE_MESSAGE
	DWORD mode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

	IOReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	hTLerJogo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)lerJogo, (LPVOID)hPipe, 0, NULL);
	if (hTLerJogo == NULL) {
		//_tprintf(TEXT("[ERR0] Lançamento da Thread de leitura de jogo. %d\n"));
		exit(EXIT_FAILURE);
	}

	// Adiciona música de fundo a tocar em loop
	PlaySound(_T("friends.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	//Sleep(200000);
	//Sleep(2000);
	
}

void desenhaNaves(HWND hWnd, HDC auxDC)
{

	// Desenha naves defensoras
	for(int cnt = 0; cnt < j.nDefensores; cnt++){
		(auxDC, j.defensores[cnt].posx, j.defensores[cnt].posy, 
			j.defensores[cnt].posx + j.defensores[cnt].largura, 
			j.defensores[cnt].posy + j.defensores[cnt].altura);

		SetStretchBltMode(auxDC, BLACKONWHITE);
		StretchBlt(auxDC, j.defensores[cnt].posx, j.defensores[cnt].posy, 30, 30, hdcDefensora, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
	}

	// Desenha naves invasoras
	for (int k = 0; k < j.nInvasores; k++) {

		if (j.invasores[k].tipo == 'B') {
			SetStretchBltMode(auxDC, BLACKONWHITE);
			StretchBlt(auxDC, j.invasores[k].posx, j.invasores[k].posy, 30, 30, hdcBasica, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
		}
		else {
			SetStretchBltMode(auxDC, BLACKONWHITE);
			StretchBlt(auxDC, j.invasores[k].posx, j.invasores[k].posy, 30, 30, hdcEsquiva, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
		}
		
		
	}
	// DESENHA TIROS
	for (int l = 0; l < j.nTiros; l++) {
		if (j.tiros[l].acertou == 0) {
			SetStretchBltMode(auxDC, BLACKONWHITE);
			StretchBlt(auxDC, j.tiros[l].posx, j.tiros[l].posy, j.tiros[l].largura, j.tiros[l].altura, hdcTiro, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
		}
	}

	// DESENHO POWERUPS
	for (int p = 0; p < j.nPowerups; p++) {
		if (j.powerups[p].acertou == 0) {
			SetStretchBltMode(auxDC, BLACKONWHITE);
			StretchBlt(auxDC, j.powerups[p].posx, j.powerups[p].posy, j.powerups[p].largura, j.powerups[p].altura, hdcPowerUp, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
		}
	}

	
	// DESENHA BOMBAS
	for (int n = 0; n < j.nBombas; n++) {
		if (j.bombas[n].acertou == 0) {
			SetStretchBltMode(auxDC, BLACKONWHITE);
			StretchBlt(auxDC, j.bombas[n].posx, j.bombas[n].posy, j.bombas[n].largura, j.bombas[n].altura, hdcBomba, 0, 0, bmNave.bmWidth, bmNave.bmHeight, SRCCOPY);
		}
	}
	static int chamadas = 0, mostrada = 0;
	if ((j.nDefensores == 0 || j.nInvasores == 0) && chamadas++ > 1 && mostrada == 0) {
		DialogBox(hInst, MAKEINTRESOURCE(IDD_FIMDOJOGO), FindWindow(NULL, szTitle), FimDoJogo);
		mostrada = 1;
	}
	
}


//TCHAR letras[10] = TEXT(" ");
//UINT x = 0, y = 0;



// EX2 - ShowBItmap
//HBITMAP hNave = NULL;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance); //regista a class da janela
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);//receber input do teclado
INT_PTR CALLBACK    FimDoJogo(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CLIENTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENTE));

	MSG msg;


	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_INFORMATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENTE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, TEXT("Friends in Space"), WS_OVERLAPPEDWINDOW,
		100, 100, TAM_JANELA_X, TAM_JANELA_Y, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//	int ret;

	static int x, y;
	PAINTSTRUCT ps;
	char *ordem;

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_ARQUIVO_INICIAR:
			inicializa();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	
	case WM_CHAR:

		break;
	case WM_LBUTTONDOWN:

		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);

		hdc = GetDC(hWnd);

		GetObject(hNaveEsquiva, sizeof(bm), &bm);
		hdcMem = CreateCompatibleDC(hdc);
		hbmOld = (HBITMAP)SelectObject(hdcMem, hNaveEsquiva);

		BitBlt(auxDC, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

		// SetStretchBltMode(auxDC, BLACKONWHITE);
		// StretchBlt(auxDC, x, y, 30, 30, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteDC(hdcMem);

		ReleaseDC(hWnd, hdc);

		InvalidateRect(hWnd, NULL, TRUE);


		break;
break;

	case WM_CREATE:

		hNaveBasica = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(NAVE_BASICA), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(hNaveBasica, sizeof(bmNave), &bmNave);
		hdcBasica = CreateCompatibleDC(hdc);
		SelectObject(hdcBasica, hNaveBasica);


		hNaveDefensora = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(NAVE_DEFENSORA), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		GetObject(hNaveDefensora, sizeof(bmNave), &bmNave);
		hdcDefensora = CreateCompatibleDC(hdc);
		SelectObject(hdcDefensora, hNaveDefensora);

		hNaveEsquiva = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(NAVE_ESQUIVA), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		GetObject(hNaveDefensora, sizeof(bmNave), &bmNave);
		hdcEsquiva = CreateCompatibleDC(hdc);
		SelectObject(hdcEsquiva, hNaveEsquiva);

		hTiro = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(TIRO), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		GetObject(hTiro, sizeof(bmNave), &bmNave);
		hdcTiro = CreateCompatibleDC(hdc);
		SelectObject(hdcTiro, hTiro);

		hBomba = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(BOMBA), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		GetObject(hBomba, sizeof(bmNave), &bmNave);
		hdcBomba = CreateCompatibleDC(hdc);
		SelectObject(hdcBomba, hBomba);

		hPowerUp = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(POWER_UP), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		GetObject(hPowerUp, sizeof(bmNave), &bmNave);
		hdcPowerUp = CreateCompatibleDC(hdc);
		SelectObject(hdcPowerUp, hPowerUp);



		// CARREGA 'BITMAP' E ASSOCIA A UM 'DC' EM MEMORIA PARA A NAVE...
		//bg = CreateSolidBrush(RGB(255, 128, 128));
		
		nX = GetSystemMetrics(SM_CXSCREEN);
		nY = GetSystemMetrics(SM_CYSCREEN);
		bg = CreatePatternBrush(LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(BACKGROUND)));

		// PREPARA 'BITMAP' E ASSOCIA A UM 'DC' EM MEMORIA... 
		auxDC = CreateCompatibleDC(hdc);
		auxBM = CreateCompatibleBitmap(hdc, nX, nY);
		SelectObject(auxDC, auxBM);
		SelectObject(auxDC, bg);
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_KEYDOWN:
		// CODIGO ASSOCIADO A TECLA SETA PARA A DIREITA...
		if (wParam == VK_RIGHT) {
			enviarParaNamedPipe((TCHAR *)TEXT("direita"), hPipe);
			WaitForSingleObject(IOReady, INFINITE);
		}
		else if (wParam == VK_LEFT) {
			enviarParaNamedPipe((TCHAR *)TEXT("esquerda"), hPipe);
			WaitForSingleObject(IOReady, INFINITE);
		}
		else if (wParam == VK_UP) {
			enviarParaNamedPipe((TCHAR *)TEXT("cima"), hPipe);
			WaitForSingleObject(IOReady, INFINITE);
		}
		else if (wParam == VK_DOWN) {
			enviarParaNamedPipe((TCHAR *)TEXT("baixo"), hPipe);
			WaitForSingleObject(IOReady, INFINITE);
		}
		else if (wParam == VK_SPACE) {
			enviarParaNamedPipe((TCHAR *)TEXT("bala"), hPipe);
			WaitForSingleObject(IOReady, INFINITE);
		}


		
		break;

	case WM_PAINT:
		
		// DESENHA NO 'DC' EM MEMORIA... 
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);								//Limpar o fundo
		desenhaNaves(hWnd, auxDC);
		
		//TextOut(auxDC, x + 5, y + 5, letras, 1);
		


		// COPIA INFORMACAO DO 'DC' EM MEMORIA PARA O DISPLAY... 
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, nX, nY, auxDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;

	case WM_ERASEBKGND:
		return 1L;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		CloseHandle(hPipe);
		CloseHandle(hTLerJogo);
		// LIBERTA RECURSO RELATIVOS AO 'BITMAP' E AO 'DC' EM MEMORIA... 
		DeleteObject(bg);
		DeleteObject(auxBM);
		DeleteDC(auxDC);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Manipulador de mensagem para caixa 'Sobre'.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Manipulador de mensagem para caixa 'FimDoJogo'.
INT_PTR CALLBACK FimDoJogo(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			exit(1);
			CloseWindow(FindWindow(NULL, szTitle));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
