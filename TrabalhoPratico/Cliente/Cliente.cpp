// Cliente.cpp : define o ponto de entrada para o aplicativo do console.
//
#include "cliente.h"

void enviarParaNamedPipe(int i, TCHAR * string1, HANDLE h) {
	ZeroMemory(&ov, sizeof(ov));
	ResetEvent(IOReady);
	ov.hEvent = IOReady;

	_tprintf(TEXT("Escrever no pipe %d"), h);
	if (!WriteFile(h, string1, TAM * sizeof(TCHAR), &n, &ov)) {
		_tprintf(TEXT("[ERRO] Escrever no pipe %d! (WriteFile)\n"));
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


int main()
{
	HANDLE hPipe, hT;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif


	_tprintf(TEXT("[LEITOR] Esperar pelo pipe '%s' (WaitNamedPipe)\n"), PIPE_NAME);
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (WaitNamedPipe)\n"), PIPE_NAME);
		exit(-1);
	}

	_tprintf(TEXT("[LEITOR] Ligação ao pipe do escritor... (CreateFile)\n"));
	hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0 | FILE_FLAG_OVERLAPPED, NULL);
	if (hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'! (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}
	_tprintf(TEXT("[LEITOR] Liguei-me...\n"));

	// Mudar modo de funcionamento do pipe para PIPE_READMODE_MESSAGE
	DWORD mode = PIPE_READMODE_MESSAGE;
	SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

	IOReady = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (1) {
		//ZeroMemory(buf, sizeof(buf));
		_tcscpy_s(buf, lerDoNamedPipe(hPipe));
		
		



		

		//buf[n / sizeof(TCHAR)] = '\0';

		if (!n) {
			_tprintf(TEXT("[LEITOR] %d... (ReadFile)\n"), n);
			break;
		}
		_tprintf(TEXT("[LEITOR] Recebi %d bytes: '%s'... (ReadFile)\n"), n, buf);
	}

	CloseHandle(hPipe);
	Sleep(200);


	return 0;
}

