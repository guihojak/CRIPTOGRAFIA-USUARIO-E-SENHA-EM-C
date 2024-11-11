#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>  

#define MAX 100
#define FILE_NAME "usuarios.txt"
#define CHAVE 'X' 

// Fun��o para criptografar e descriptografar usando XOR
void criptografar(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] ^= CHAVE;
        i++;
    }
}

// Fun��o para verificar se um usu�rio existe e, se sim, retornar a senha criptografada
int usuarioExiste(const char *nome, char *senha) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return 0;

    char nomeArquivo[MAX], senhaArquivo[MAX];
    while (fscanf(file, "%s %s", nomeArquivo, senhaArquivo) != EOF) {
        if (strcmp(nomeArquivo, nome) == 0) {
            if (senha) strcpy(senha, senhaArquivo); // Copia a senha criptografada, se solicitada
            fclose(file);
            return 1; // Usu�rio encontrado
        }
    }
    fclose(file);
    return 0; // Usu�rio n�o encontrado
}

// Fun��o para cadastrar um novo usu�rio com verifica��o de exist�ncia
void cadastrarUsuario() {
    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[MAX], senha[MAX], senhaCriptografada[MAX];
    printf("Digite o nome do usu�rio que deseja cadastrar: ");
    scanf(" %[^\n]", nome); // L� nome com acentua��o e espa�os
    criptografar(nome); // Criptografa o nome para busca

    if (usuarioExiste(nome, NULL)) {
        printf("Usu�rio j� existe! Escolha outro nome.\n");
        return;
    }

    // Solicita e criptografa a senha para armazenar
    printf("Digite a senha do usu�rio: ");
    scanf(" %[^\n]", senha); // L� senha com acentua��o e espa�os
    criptografar(senha);

    fprintf(file, "%s %s\n", nome, senha); // Salva o nome e a senha criptografados
    fclose(file);
    printf("Usu�rio cadastrado com sucesso!\n");
}

// Fun��o para realizar o login de um usu�rio e verificar senha
int loginUsuario() {
    char nome[MAX], senha[MAX], senhaCriptografada[MAX];
    printf("Digite seu nome de usu�rio para login: ");
    scanf(" %[^\n]", nome); // L� nome com acentua��o e espa�os
    criptografar(nome); // Criptografa o nome para buscar no arquivo

    if (!usuarioExiste(nome, senhaCriptografada)) {
        printf("Usu�rio n�o encontrado. Verifique o nome ou cadastre-se.\n");
        return 0;
    }

    printf("Digite sua senha: ");
    scanf(" %[^\n]", senha); // L� senha com acentua��o e espa�os
    criptografar(senha); // Criptografa a senha para compara��o

    if (strcmp(senha, senhaCriptografada) == 0) {
        printf("Login realizado com sucesso!\n");
        return 1;
    } else {
        printf("Senha incorreta. Tente novamente.\n");
        return 0;
    }
}

// Fun��o para alterar a senha de um usu�rio logado
void alterarSenha() {
    char nome[MAX], novaSenha[MAX], senhaCriptografada[MAX];
    printf("Digite o nome do usu�rio para alterar a senha: ");
    scanf(" %[^\n]", nome); // L� nome com acentua��o e espa�os
    criptografar(nome); // Criptografa o nome para busca

    if (!usuarioExiste(nome, senhaCriptografada)) {
        printf("Usu�rio n�o encontrado. Verifique o nome.\n");
        return;
    }

    // Confirma��o de login para seguran�a
    printf("Digite sua senha atual: ");
    char senhaAtual[MAX];
    scanf(" %[^\n]", senhaAtual); // L� senha atual com acentua��o e espa�os
    criptografar(senhaAtual);

    if (strcmp(senhaAtual, senhaCriptografada) != 0) {
        printf("Senha atual incorreta. N�o foi poss�vel alterar a senha.\n");
        return;
    }

    // Solicita e criptografa a nova senha
    printf("Digite sua nova senha: ");
    scanf(" %[^\n]", novaSenha); // L� nova senha com acentua��o e espa�os
    criptografar(novaSenha);

    FILE *file = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    char nomeArquivo[MAX], senhaArquivo[MAX];
    while (fscanf(file, "%s %s", nomeArquivo, senhaArquivo) != EOF) {
        if (strcmp(nomeArquivo, nome) == 0) {
            fprintf(tempFile, "%s %s\n", nomeArquivo, novaSenha); // Grava com a nova senha
        } else {
            fprintf(tempFile, "%s %s\n", nomeArquivo, senhaArquivo);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(FILE_NAME);
    rename("temp.txt", FILE_NAME);
    printf("Senha alterada com sucesso!\n");
}

// Fun��o para listar todos os usu�rios (apenas nomes)
void listarUsuarios() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[MAX], senha[MAX];
    printf("Lista de Usu�rios:\n");
    while (fscanf(file, "%s %s", nome, senha) != EOF) {
        criptografar(nome); // Descriptografa o nome para exibir
        printf("- %s\n", nome);
    }
    fclose(file);
}

// Fun��o do menu principal
void menu() {
    int opcao;
    do {
        printf("\n1. Cadastrar Usu�rio\n2. Login\n3. Alterar Senha\n4. Listar Usu�rios\n5. Sair\nEscolha uma op��o: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cadastrarUsuario(); break;
            case 2: if (loginUsuario()) printf("Bem-vindo ao sistema!\n"); break;
            case 3: alterarSenha(); break;
            case 4: listarUsuarios(); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Op��o inv�lida!\n"); break;
        }
    } while(opcao != 5);
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    menu();
    return 0;
}

