/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 02 - Árvore B
 *
 * RA: 743512
 * Aluno: Bianca Gomes Rodrigues
 * ========================================================================== */

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* Tamanho dos campos dos registros */
#define TAM_PRIMARY_KEY 11
#define TAM_NOME 51
#define TAM_MARCA 51
#define TAM_DATA 11
#define TAM_ANO 3
#define TAM_PRECO 8
#define TAM_DESCONTO 4
#define TAM_CATEGORIA 51
#define TAM_STRING_INDICE (TAM_MARCA + TAM_NOME)


#define TAM_REGISTRO 192
#define MAX_REGISTROS 1000
#define MAX_ORDEM 150
#define TAM_ARQUIVO (MAX_REGISTROS * TAM_REGISTRO + 1)


/* Saídas do usuário */
#define OPCAO_INVALIDA 				"Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE 		"Memoria insuficiente!\n"
#define REGISTRO_N_ENCONTRADO 		"Registro(s) nao encontrado!\n"
#define CAMPO_INVALIDO 				"Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA 			"ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO 				"Arquivo vazio!"
#define INICIO_BUSCA 				"********************************BUSCAR********************************\n"
#define INICIO_LISTAGEM				"********************************LISTAR********************************\n"
#define INICIO_ALTERACAO 			"********************************ALTERAR*******************************\n"
#define INICIO_ARQUIVO				"********************************ARQUIVO*******************************\n"
#define INICIO_INDICE_PRIMARIO      "***************************INDICE PRIMÁRIO****************************\n"
#define INICIO_INDICE_SECUNDARIO    "***************************INDICE SECUNDÁRIO**************************\n"
#define SUCESSO  				 	"OPERACAO REALIZADA COM SUCESSO!\n"
#define FALHA 					 	"FALHA AO REALIZAR OPERACAO!\n"
#define NOS_PERCORRIDOS_IP 			"Busca por %s. Nos percorridos:\n"
#define NOS_PERCORRIDOS_IS 			"Busca por %s.\nNos percorridos:\n"


/* Registro do Produto */
typedef struct {
	char pk[TAM_PRIMARY_KEY];
	char nome[TAM_NOME];
	char marca[TAM_MARCA];
	char data[TAM_DATA];	/* DD/MM/AAAA */
	char ano[TAM_ANO];
	char preco[TAM_PRECO];
	char desconto[TAM_DESCONTO];
	char categoria[TAM_CATEGORIA];
} Produto;


/*Estrutura da chave de um nó do Índice Primário*/
typedef struct Chaveip{
	char pk[TAM_PRIMARY_KEY];
	int rrn;
} Chave_ip;

/*Estrutura da chave de um  do Índice Secundário*/
typedef struct Chaveis{
	char string[TAM_STRING_INDICE];
	char pk[TAM_PRIMARY_KEY];
} Chave_is;

/* Estrutura das Árvores-B */
typedef struct nodeip{
	int num_chaves;			/* numero de chaves armazenadas*/
	Chave_ip *chave;		/* vetor das chaves e rrns [m-1]*/
	int *desc;				/* ponteiros para os descendentes, *desc[m]*/
	char folha;				/* flag folha da arvore*/
} node_Btree_ip;

typedef struct nodeis{
	int num_chaves;			/* numero de chaves armazenadas*/
	Chave_is *chave;		/* vetor das chaves e rrns [m-1]*/
	int *desc;				/* ponteiros para os descendentes, *desc[m]*/
	char folha;				/* flag folha da arvore*/
} node_Btree_is;

typedef struct {
	int raiz;
} Indice;

/* Variáveis globais */
char ARQUIVO[MAX_REGISTROS * TAM_REGISTRO + 1];
char ARQUIVO_IP[2000*sizeof(Chave_ip)];
char ARQUIVO_IS[2000*sizeof(Chave_is)];
/* Ordem das arvores */
int ordem_ip;
int ordem_is;
int nregistros;
int nregistrosip; /*Número de nós presentes no ARQUIVO_IP*/
int nregistrosis; /*Número de nós presentes no ARQUIVO_IS*/
/*Quantidade de bytes que ocupa cada nó da árvore nos arquivos de índice:*/
int tamanho_registro_ip;
int tamanho_registro_is;
/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

Produto recuperar_registro(int RRN);

void gerarChave(Produto * Novo);


/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */
 
 /********************FUNÇÕES DO MENU*********************/
 void cadastrar(Indice* iprimary, Indice* ibrand);

 int alterar(Indice iprimary);
 
 void buscar(Indice iprimary,Indice ibrand);
 
 void listar(Indice iprimary,Indice ibrand);
 
 /*******************************************************/
 
 /* Recebe do usuário uma string simulando o arquivo completo e retorna o número
  * de registros. */
 int carregar_arquivo();
 
/* (Re)faz o Cria iprimary*/
void criar_iprimary(Indice *iprimary);
 
/* (Re)faz o índice de jogos  */
void criar_ibrand(Indice *ibrand) ;
 
/*Escreve um nó da árvore no arquivo de índice,
* O terceiro parametro serve para informar qual indice se trata */

//void write_btree(void *salvar, int rrn, char ip);

/* ---------------------- */
void write_btree_ip(node_Btree_ip *salvar, int rrn);

void write_btree_is(node_Btree_is *salvar, int rrn);
/* ---------------------- */

/*Lê um nó do arquivo de índice e retorna na estrutura*/
//void *read_btree(int rrn, char ip);

/* ---------------------- */
node_Btree_ip *read_btree_ip(int rrn);

node_Btree_is *read_btree_is(int rrn);
/* ---------------------- */
 
/* Aloca um nó de árvore para ser utilizado em memória primária, o primeiro parametro serve para informar que árvore se trata
* É conveniente que essa função também inicialize os campos necessários com valores nulos*/
//void *criar_no(char ip);
 
node_Btree_ip *CriarNode_IP();

node_Btree_is *CriarNode_IS();

/*Libera todos os campos dinâmicos do nó, inclusive ele mesmo*/
void libera_no(void *node, char ip);
 
/*
*   Caro aluno,
*   caso não queira trabalhar com void*, é permitido dividir as funções que utilizam
* em duas, sendo uma para cada índice...
* Um exemplo, a write_btree e read_btree ficariam como:
*
*   void write_btree_ip(node_Btree_ip *salvar, int rrn),  node_Btree_ip *read_btree_ip(int rrn),
*   void write_btree_is(node_Btree_is *salvar, int rrn) e node_Btree_is *read_btree_is(int rrn).
*/
 
/* Atualiza os dois índices com o novo registro inserido */
//void inserir_registro_indices(Indice *iprimary, Indice *ibrand, Jogo j);
 
/* Exibe o jogo */
int exibir_registro(int rrn);

int main()
{
	char *p; /* # */
  /* Arquivo */
	int carregarArquivo = 0; nregistros = 0, nregistrosip = 0, nregistrosis = 0;
	scanf("%d\n", &carregarArquivo); /* 1 (sim) | 0 (nao) */
	if (carregarArquivo)
		nregistros = carregar_arquivo();

	scanf("%d %d%*c", &ordem_ip, &ordem_is);

	tamanho_registro_ip = ordem_ip*3 + 4 + (-1 + ordem_ip)*14;
	tamanho_registro_is = ordem_is*3 + 4 + (-1 + ordem_is)* (	TAM_STRING_INDICE +9);

	/* Índice primário */
	Indice iprimary ;
	//criar_iprimary(&iprimary);

	/* Índice secundário de nomes dos Produtos */
	Indice ibrand;
	//criar_ibrand(&ibrand);

	/* Execução do programa */
	int opcao = 0;
	while(1)
	{
		scanf("%d%*c", &opcao);
		switch(opcao) {
		case 1: /* Cadastrar um novo Produto */
			cadastrar(&iprimary, &ibrand);
			break;
		case 2: /* Alterar o desconto de um Produto */
			printf(INICIO_ALTERACAO);
			// if (alterar(iprimary))
			// 	printf(SUCESSO);
			// else
			// 	printf(FALHA);
			break;
		case 3: /* Buscar um Produto */
			printf(INICIO_BUSCA);
			//buscar(iprimary, ibrand);
			break;
		case 4: /* Listar todos os Produtos */
			printf(INICIO_LISTAGEM);
			//listar(iprimary, ibrand);
			break;
		case 5: /* Imprimir o arquivo de dados */
			printf(INICIO_ARQUIVO);
			printf("%s\n", (*ARQUIVO!='\0') ? ARQUIVO : ARQUIVO_VAZIO);
			break;
		case 6: /* Imprime o Arquivo de Índice Primário*/
			printf(INICIO_INDICE_PRIMARIO);
			if(!*ARQUIVO_IP)
				puts(ARQUIVO_VAZIO);
			else
				for(p = ARQUIVO_IP; *p!='\0'; p+=tamanho_registro_ip)
				{
					fwrite( p , 1 ,tamanho_registro_ip,stdout);
					puts("");
				}
			break;
		case 7: /* Imprime o Arquivo de Índice Secundário*/
			printf(INICIO_INDICE_SECUNDARIO);
			if(!*ARQUIVO_IS)
				puts(ARQUIVO_VAZIO);
			else
				for(p = ARQUIVO_IS; *p!='\0'; p+=tamanho_registro_is)
				{
					fwrite( p , 1 ,tamanho_registro_is,stdout);
					puts("");

				}
			break;
		case 8: /*Libera toda memória alocada dinâmicamente (se ainda houver) e encerra*/
			return 0;
		default: /* exibe mensagem de erro */
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return -1;
}

/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo()
{
	scanf("%[^\n]\n", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}

Produto recuperar_registro(int RRN){
	
	char Registro[193], *p;
	strncpy(Registro, ARQUIVO + ((RRN)*192), 192);
	Registro[192] = '\0';

	Produto A;
	p = strtok(Registro,"@");
	strcpy(A.pk,p);
	p = strtok(NULL,"@");
	strcpy(A.nome,p);
	p = strtok(NULL,"@");
	strcpy(A.marca,p);
	p = strtok(NULL,"@");
	strcpy(A.data,p);
	p = strtok(NULL,"@");
	strcpy(A.ano,p);
	p = strtok(NULL,"@");
	strcpy(A.preco,p);
	p = strtok(NULL,"@");
	strcpy(A.desconto,p);
	p = strtok(NULL,"@");
	strcpy(A.categoria,p);
	
	return A;

}

/* Exibe o Produto */
int exibir_registro(int rrn)
{
	if(rrn < 0)
		return 0;

	float preco;
	int desconto;
	Produto j = recuperar_registro(rrn);
    char *cat, categorias[TAM_CATEGORIA];
	
	printf("%s\n", j.pk);
	printf("%s\n", j.nome);
	printf("%s\n", j.marca);
	printf("%s\n", j.data);
	printf("%s\n", j.ano);
	sscanf(j.desconto,"%d",&desconto);
	sscanf(j.preco,"%f",&preco);
	preco = preco *  (100-desconto);
	preco = ((int) preco)/ (float) 100 ;
	printf("%07.2f\n",  preco);
	strcpy(categorias, j.categoria);
	
	for (cat = strtok (categorias, "|"); cat != NULL; cat = strtok (NULL, "|"))
    	printf("%s ", cat);
	printf("\n");

	return 1;
}

 /*================================= FUNÇÕES ================================*/

void write_btree_ip(node_Btree_ip *salvar, int rrn){

	salvar->num_chaves = ordem_ip-1;
	//printf("%d\n", salvar->num_chaves );

	//printf("%d\n", tamanho_registro_ip);
	int Tamanho = tamanho_registro_ip+1;
	char Filho[Tamanho];
	Filho[Tamanho-1] = '\0';

	Produto Auxiliar = recuperar_registro(rrn);
	
	char PrimaryKey[TAM_PRIMARY_KEY];

	strcpy(PrimaryKey, Auxiliar.pk);

	//printf("Primary Key %s\n", PrimaryKey);

	/*Auxiliar para transformar, caso seja necessário, o número "nregistrosip" em um número de três bytes*/
	char nRegistrosIP[4];

	/*Auxiliar para transformar, caso seja necessário, o "rrn" em um número de quatro bytes*/ 
	char RRN[5];

	snprintf(nRegistrosIP, sizeof(nRegistrosIP), "%03d", nregistrosip);

	snprintf(RRN, sizeof(RRN), "%04d", rrn);

	sprintf(Filho, "%s%s%s", nRegistrosIP, PrimaryKey , RRN);
	
	nregistrosip++;

	//int TamanhoFilho = strlen(Filho);
	//printf("Tamanho do Filho %d\n", Tamanho);
	//printf("%s\n", Filho);

 }

void write_btree_is(node_Btree_is *salvar, int rrn){


}

node_Btree_ip *CriarNode_IP(){

}

node_Btree_is *CriarNode_IS(){

}


void gerarChave(Produto * Novo){

	Novo->pk[0] = Novo->nome[0];
	Novo->pk[1] = Novo->nome[1];
	Novo->pk[2] = Novo->marca[0];
	Novo->pk[3] = Novo->marca[1];
	Novo->pk[4] = Novo->data[0];
	Novo->pk[5] = Novo->data[1];
	Novo->pk[6] = Novo->data[3];
	Novo->pk[7] = Novo->data[4];
	Novo->pk[8] = Novo->ano[0];
	Novo->pk[9] = Novo->ano[1]; 
	Novo->pk[10] = '\0';

}

 void cadastrar(Indice* iprimary, Indice* ibrand){

	//Código - NÃO é inserido pelo usuário 
	// char pk[TAM_PRIMARY_KEY];
	// gerarChave(novo);

	/*-----------------------*/

	/* Interação com o Usuário */
	Produto Novo;
	/* CAMPOS DE TAMANHO VARIÁVEL */
	
	//Nome do Produto ou Modelo
	// char Nome[TAM_NOME];
	scanf("%[^\n]s", Novo.nome);
	getchar();
	//Marca
	// char Marca[TAM_MARCA];
	scanf("%[^\n]s", Novo.marca);
	getchar();
	/*-----------------------*/

	/* CAMPOS DE TAMANHO FIXO */

	//Data de Registro
	// char Data[TAM_DATA];	/* DD/MM/AAAA */
	scanf("%[^\n]s", Novo.data);
	getchar();
	//Ano de Lançamento
	// char Ano[TAM_ANO];
	scanf("%[^\n]s", Novo.ano);
	getchar();
	//Preço-Base
	// char Preço[TAM_PRECO];
	scanf("%[^\n]s", Novo.preco);
	getchar();
	//Desconto
	// char Desconto[TAM_DESCONTO];
	scanf("%[^\n]s", Novo.desconto);
	getchar();
	//Categorias
	// char Categoria[TAM_CATEGORIA];
	scanf("%[^\n]s", Novo.categoria);
	getchar();
	/*-----------------------*/

	gerarChave(&Novo);

	//Verifica se o PRODUTO existe
	// if(Busca != NULL) {
			// printf(ERRO_PK_REPETIDA, Novo->pk);
			// return;
 	// }

	// else{
		
		nregistros++;

		//Registro Auxiliar
		char rAuxiliar[193]; //TAM_REGISTRO
		rAuxiliar[192] = '\0';

		sprintf(rAuxiliar, "%s@%s@%s@%s@%s@%s@%s@%s@", Novo.pk, Novo.nome, Novo.marca, Novo.data, Novo.ano, Novo.preco, Novo.desconto, Novo.categoria);

		//Precisamos obter o TAMANHO do REGISTRO AUXILIAR (rAuxiliar) para sabermos quantos "bytes" faltam para preencher totalmento o REGISTRO.
		int Tamanho = strlen(rAuxiliar);

		// printf("\nTamanho = %d\n", Tamanho);

		int i;
		//Preenchendo o REGISTRO por completo (192bytes)
		for(i = Tamanho; i < 192; i++)
			rAuxiliar[i] = '#';

		// printf("\nTamanho - Final = %d", strlen(rAuxiliar));
		// printf("\n Registro: %s \n", rAuxiliar);

		strcat(ARQUIVO, rAuxiliar);

		//int tRegistro = strlen(rAuxiliar);
		//printf("%d\n", tRegistro);
		
		//printf("%s\n", rAuxiliar);
		
		//printf("%s\n", ARQUIVO);

		node_Btree_ip newNode; //= CriarNode_IP();

		/*O número de registros já foi incrementado, então precisa preciso subtrair um */
		write_btree_ip(&newNode, nregistros-1);
	
	// }
	
	

}

//  int alterar(Indice iprimary){

//  }
 
//  void buscar(Indice iprimary,Indice ibrand){

//  }
 
//  void listar(Indice iprimary,Indice ibrand){

//  }