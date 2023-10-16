#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")

using namespace std;

class cFuncoes
{
private:

	SQLHENV Env;
	SQLHDBC Dbc;
	SQLSMALLINT Saida;
	SQLRETURN Retorno;
	SQLHANDLE Identificador;

public:

	void AlocarIdentificador()
	{
		SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Env);
		SQLSetEnvAttr(Env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
		SQLAllocHandle(SQL_HANDLE_DBC, Env, &Dbc);
	}

	void MensagemDeErro()
	{
		SQLWCHAR Dados[MAX_PATH];
		SQLWCHAR Error[MAX_PATH];
		SQLSMALLINT Quantidade;

		//SQLGetDiagRec para recuperarmos o texto do erro.
		SQLGetDiagRec(SQL_HANDLE_DBC, Dbc, 1, Dados, 0, Error, sizeof(Error), &Quantidade);

		cout << "Ocorreu um erro, verifique a mensagem..\n";
		_tprintf(L"Mensagem de erro: %s\n", Error);
	}

	/*
	*
	* O Argumento será a cadeia de caracteres onde se localiza o drive e o banco de dados para a conexão, usando os parâmetros de conexão.
	*
	*/
	void InicializarConexao(wstring Argumento)
	{
		Retorno = SQLAllocConnect(Env, &Dbc);
		if (Retorno == SQL_SUCCESS)
		{
			Retorno = SQLDriverConnect(Dbc, GetDesktopWindow(), (SQLWCHAR*)Argumento.c_str(), SQL_NTS,
				0, 0, &Saida, SQL_DRIVER_NOPROMPT);
			if (Retorno == SQL_SUCCESS_WITH_INFO)
			{
				cout << "Banco de dados conectado com sucesso..\n";
			}
			else
			{
				MensagemDeErro();
			}
		}
	}

	void InserirDados(wstring Nome, wstring Sobrenome, wstring Idade, wstring Telefone)
	{
		SQLAllocHandle(SQL_HANDLE_STMT, Dbc, &Identificador);

		//Estes itens listados são os que estão inseridos na tabela.
		//Você poderá alterar estes itens, acrescentar ou retirar.
		/*
		wstring Nome = L"Marcos";
		wstring Sobrenome = L"Cezar";
		wstring Idade = L"20";
		wstring Telefone = L"+552700000-0000";
		*/

		wstring comandoSQL = L"insert into Usuario ( Nome, Sobrenome, Idade, Telefone ) values ('" + Nome + L"','" + Sobrenome + L"','" + Idade + L"','" + Telefone + L"')";

		//Inserindo dados em tabela, nas colunas de informações.
		Retorno = SQLExecDirect(Identificador, (SQLWCHAR*)comandoSQL.c_str(), comandoSQL.length());
		if (Retorno == SQL_SUCCESS)
		{
			cout << "Os dados solicitados foram inseridos no banco de dados com sucesso..\n";
		}
		else
		{
			MensagemDeErro();
		}

		//Desconectar-se do servidor e liberar todos os identificadores quando não for mais necessário.
		if (Retorno == SQL_SUCCESS)
		{
			SQLFreeHandle(SQL_HANDLE_STMT, Identificador);
			SQLFreeHandle(SQL_HANDLE_DBC, Dbc);
			SQLFreeHandle(SQL_HANDLE_ENV, Env);
			SQLDisconnect(Dbc);
		}
	}

}Funcoes;

int main()
{

	cout << "O assistente está conectando-se ao banco de dados e criando um registro...\n";

	Funcoes.AlocarIdentificador();
	Funcoes.InicializarConexao(L"DRIVER={SQL Server};Server=ORLAN;Database=TESTE;Trusted_Connection=yes;");

	Sleep(8 * 1000);

	Funcoes.InserirDados(L"Marcos", L"Cezar", L"20", L"+552700000-0000");

	system("pause");
}
