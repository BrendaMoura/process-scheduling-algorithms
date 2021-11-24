//Brenda Cristina Dourado Moura
//Engenharia de Software
//Turma 2
//Mat�ria: Sistemas Operacionais
//Lista 2 - Primeiro tipo de escalonador
//Escalonador do tipo Prioridade

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

//ESTADOS
//0 - criado
//1 - Apto
//2 - Executando
//3 - Bloqueado
//4 - Destru�do

int processoInicial = -1;
//Essa variavel verifica se o usuario j� criou o processo inicial

//https://www.ti-enxame.com/pt/c/como-dividir-uma-string-em-2-strings-em-c/968225763/
//https://www.ti-enxame.com/pt/c/como-remover-o-caractere-em-um-vazio-indice-de-uma-string-em-c/971615844/
//https://labdegaragem.com/forum/topics/converter-char-para-int
//https://www.ti-enxame.com/pt/c/como-voce-limpa-tela-do-console-em-c/968047218/

struct no{
    int identificador;
	int quantum;
	int tempo;
	int prioridade;
	int estado;

	//Arvore
    struct no *pai;
    struct no *primFilho;
    struct no *proxIrmao;

    //fila
    struct no *prox;
    struct no *ant;
};

//Identidade da parada toda
struct registrador{
    struct no *raiz;

    struct no *cabeca;
	struct no *cauda;
};

//C�digo oferecido
int chamadaSistema(){
	return rand()% 2;
}

int desbloqueio(){
	return rand()% 3;
}

int determinarQuantum(){
	return (rand()% 10)+15;
}

int determinarTempo(){
	return (rand()% 10)+15;
}

int determinarPrioridade(){
	return (rand()% 10)+1;
}

//Torna os processos da fila aptos para a execu��o
void tornarApto(struct registrador *processos){
	struct no *aux = processos->cabeca;
	while(aux){
		aux->estado = 1;
		aux = aux->prox;
	}
}

//Busca processo
struct no *buscarProcesso(struct no *raiz, int chave){
    if(raiz==NULL)return NULL;
    if((raiz->identificador)==chave)return raiz;
    struct no *p = raiz->primFilho;
    while(p){
        struct no *resp = buscarProcesso(p, chave);
        if(resp)return resp;
        p = p->proxIrmao;
    }
    return NULL;
}

//Inicializa��o da identifica��o da fila
void inicializa(struct registrador *processos){
    processos->raiz = NULL;

	processos->cabeca = NULL;
	processos->cauda = NULL;
}

//Cria um novo processo
struct no *novoProcesso(int chave){
	struct no *novo = (struct no*)malloc(sizeof(struct no));
	if(novo){
        //Inicializa��o referente � arvore
        novo->pai = NULL;
        novo->primFilho = NULL;
        novo->proxIrmao = NULL;

        //Inicializa��o referente � fila
		novo->ant = NULL;
		novo->prox = NULL;

		novo->identificador = chave;
		novo->quantum = 0;
		novo->tempo = determinarTempo();
		novo->prioridade = 0;
		novo->estado = 0;
		return novo;
	}
}

//Insere um novo processo na arvore
int inserirProcesso(int px,int py, struct registrador *processos){
    //Busca o pai do novo processo
    struct no *pai = buscarProcesso((processos->raiz),px);
    if(!pai) return 0;
    struct no *filho = novoProcesso(py);
    //Adiciona o pai dele na certid�o de nascimento
    filho->pai = pai;
    struct no *p = pai->primFilho;
    if(!p)pai->primFilho = filho;
    else{
        while(p->proxIrmao){
            p = p->proxIrmao;
        }
        p->proxIrmao = filho;
    }

    //Adiciona as informa��es da fila
    //Organizando a fila pelo menor trabalho
    if(processos->cabeca==NULL && processos->cauda==NULL){
        //Quando n�o tem nada da fila
        processos->cabeca = filho;
        processos->cauda = filho;
    }else{
         if((filho->tempo)<=(processos->cabeca->tempo)){
            //Verifica se tem que botar na cabe�a
            processos->cabeca->ant = filho;
            filho->prox = processos->cabeca;
            processos->cabeca = filho;
        }else if((filho->tempo)>=(processos->cauda->tempo)){
            //Verifica se tem que botar na cauda
            processos->cauda->prox = filho;
            filho->ant = processos->cauda;
            processos->cauda = filho;
        }else{
            //Verifica se tem que botar no meio
            struct no *p = processos->cabeca;
            while(p){
                if(p->prox==NULL){
                    //Verifica se tem apenas um processo
                    p->prox = filho;
                    filho->ant = p;
                    processos->cauda = filho;
                    break;
                }
                else{
                    //Verifica se tem mais de um processo
                    if((filho->tempo)>=(p->tempo)){
                        p->ant->prox = filho;
                        filho->ant = p->ant;
                        filho->prox = p;
                        p->ant = filho;
                        break;
                    }
                }
                p = p->prox;
            }
        }
    }
    return 1;
}

//Cria o processo inicial
struct no *criarProcessoInicial(int chave){
    return (novoProcesso(chave));
}

//Contar quantidade de processos
int quantidadeProcessos(struct no *raiz){
    if(raiz==NULL){
        return 0;
    }
    return (1 + quantidadeProcessos(raiz->primFilho) + quantidadeProcessos(raiz->proxIrmao));
}

//Imprimir processos
void imprimirProcessos(struct no *raiz){
    if(raiz!=NULL){
        printf("%d-Estado: %d, Prioridade: %d(", raiz->identificador,raiz->estado, raiz->prioridade);
        struct no *p = raiz->primFilho;
        while(p){
            imprimirProcessos(p);
            p = p->proxIrmao;
        }
        printf(")");
    }
}

//Imprimir fila de prioridade
void imprimirFilaPorTempo(struct registrador *processos){
    struct no *p = processos->cabeca;
    while(p){
        printf("%d-Estado: %d, Tempo: %d\n", p->identificador,p->estado, p->tempo);
        p = p->prox;
    }
}

//Retorna o tempo atual
struct tm *tempoAtual(){
	//http://linguagemc.com.br/exibindo-data-e-hora-com-time-h/

	//vari�vel do tipo time_t para armazenar o tempo em segundos
	struct tm *tempo;

	//vari�vel do tipo time_t para armazenar o tempo em segundos
  	time_t segundos;

	//obtendo o tempo em segundos
	time(&segundos);

	//para converter de segundos para o tempo local, utilizamos a fun��o localtime
	tempo = localtime(&segundos);

	return tempo;
}

//Escreve no documento
void escreveDocumento(int identificador){
	//http://linguagemc.com.br/arquivos-em-c-categoria-usando-arquivos/
	//http://wiki.icmc.usp.br/images/8/82/Manipulacao_arquivos.pdf

	FILE *arq;

	arq = fopen("ciclo.txt", "a");

	char msg[20], chave[20];

	sprintf(chave, "%d", identificador);
	strcpy(msg,"Processo ");
	strcat(msg,chave);

	if(arq){
		fprintf(arq, "%s\n", msg);
	}else{
		printf("Erro ao abrir o arquivo!");
	}

	fclose(arq);
}

//Execu��o da simula��o
void executando(struct registrador *processos){
	int n, tRestante=0;
	struct no *aux = processos->cabeca;

	//ponteiro para struct que armazena data e hora
	struct tm *tInicial, *tFinal;

	while(aux){
		//Verifica qual � o pr�ximo processo apto
		if(aux->estado==1){
			//Muda o estado para executando
			aux->estado = 2;

			//Pega o tempo restante de processamento
			//T� errado
			tRestante = aux->tempo;
			printf("\nEXECUTANDO SIMULACAO - PROCESSO: %d | TEMPO RESTANTE: %d\n", aux->identificador, tRestante);

			//Loop de executar as coisinhas
			while(1){
				//Marca o tempo inicial
				tInicial = tempoAtual();

				//Escreve no documento
				escreveDocumento(aux->identificador);

				//Verifica se houve uma chamada de sistema
				if(chamadaSistema() == 1){
					//Define o estado para bloqueado
					aux->estado = 3;

					//Para o contador
					//Marca o tempo final
					tFinal = tempoAtual();
					tRestante = tRestante-((tFinal->tm_sec)-(tInicial->tm_sec));
                    tInicial = tFinal;


					while(1){
						int resultado = desbloqueio();
						if(resultado!=2){
							if(resultado==0){
								//Muda o estado para apto
								aux->estado = 1;
							}else if(resultado==1){
								//Muda o estado para destru�do
								aux->estado = 4;
							}
							break;
						}
					}

				}

				//Verifica se a pessoa quer que imprima o diagrama de estado
				printf("\nDeseja imprimir o diagrama de estado (1 - SIM/ 2 - NAO):");
				scanf("%d", &n);
				if(n==1){
					imprimirProcessos(processos->cabeca);
				}

				//Marca o tempo final
				tFinal = tempoAtual();
				tRestante = tRestante-((tFinal->tm_sec)-(tInicial->tm_sec));

				//verifica se j� terminou o tempo
                if(tRestante<=0 || aux->estado==4){
                    break;
                }
                printf("\nEXECUTANDO SIMULACAO - PROCESSO: %d | TEMPO RESTANTE: %d\n", aux->identificador, tRestante);
			}
		}
		aux = aux->prox;
	}
}

int main(){
    //Declara��o de vari�veis de controle
	int i=0,j=0,aux=0,n=0, pai=0,filho=0;
	char parametro[6];
	char p1[6],p2[6];
	char *token;

	//Identificador da parada toda
	struct registrador processos;

	//Chama a fun��o de inicializa��o
	inicializa(&processos);

	while(1){
        int opcao;
        printf("\nMENU\n");
        if(processoInicial==-1){
            printf("1 - Criar processos\n");
            printf("2 - Sair\n");
            printf("-Para ter acesso as outras opcoes do sistema, crie o processo inicial-");
            printf("\n\nDigite o numero da opcao desejada:");
            scanf("%d", &opcao);
            switch(opcao){
                case 1:
                    //Funcao para criar o processo inicial
                    processoInicial = 1;
                    processos.raiz = criarProcessoInicial(0);
                    printf("\nChave do processo inicial: %d\n\n", processos.raiz->identificador);


                    //LEITURA DOS PROCESSOS
                    printf("Digite a quantidade de processos: ");
                    //Verifica a quantidade total de processos
                    scanf("%d",&n);

                    //Leitura dos processos
                    for(i=0;i<n;i++){
                        printf("Digite as informacoes do processo: ");
                        //Recebe as informa��es do processo
                        scanf("%s", parametro);

                        //Trata o parametro
                        token = strtok(parametro, "-");
                        strcpy(p1,token);
                        strcpy(p1,&p1[1]);

                        token = strtok(NULL, " ");
                        strcpy(p2,token);
                        strcpy(p2,&p2[1]);

                        pai = atoi(p1);
                        filho = atoi(p2);

                        //Insere o processo
                        if((inserirProcesso(pai,filho, &processos))==0){
                            printf("\nPai escolhido nao existe\n\n");
                            i--;
                        }else{
                            printf("Processo adicionado com sucesso!\n");
                        }
                    }

                    break;
                case 2:
                    printf("Obrigada!");
                    break;
                default:
                   printf("\nOpcao invalida! Digite uma opcao valida para continuar.\n");
                   break;
            }
            if(opcao==2){
                break;
            }
        }
        else{
            //Aqui vai ficar a m�gica
            printf("1 - Iniciar simulacao\n");
            printf("2 - Imprimir arvore\n");
            printf("3 - Contar a quantidade de processos abertos\n");
            printf("4 - Sair\n");
            printf("\n\nDigite o numero da opcao desejada:");
            scanf("%d", &opcao);
            switch(opcao){
                case 1:
                    printf("INICIANDO SIMULACAO!\n");
                    tornarApto(&processos);
                    printf("Processos estao aptos!\n");
                    executando(&processos);
                    break;
                case 2:
                    printf("ESTADOS DOS PROCESSOS\n");
                    printf("0 - Criado\n");
                    printf("1 - Apto\n");
                    printf("2 - Executando\n");
                    printf("3 - Bloqueado\n");
                    printf("4 - Destru�do\n");
                    printf("\nPROCESSOS EXISTENTES: ");
                    imprimirProcessos((processos.raiz));
                    printf("\n\n");

                    printf("FILA POR TEMPO DE TRABALHO\n");
                    imprimirFilaPorTempo(&processos);
                    break;
                case 3:
                    printf("\nQuantidade de processos existentes: %d",quantidadeProcessos((processos.raiz)));
                    break;
                case 4:
                    printf("Obrigada!");
                    break;
                default:
                   printf("\nOpcao invalida! Digite uma opcao valida para continuar.\n");
                   break;
            }
            //IDEIA DE LIMPAR A TELA
            //system("cls");
            if(opcao==4){
                break;
            }
        }
    }
    return 0;
}
