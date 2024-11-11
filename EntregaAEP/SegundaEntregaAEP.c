#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>  

#define MAX 100
#define FILE_NAME "usuarios.txt"
#define CHAVE 'X' 

// Função para criptografar e descriptografar usando XOR
void criptografar(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        str[i] ^= CHAVE;
        i++;
    }
}

// Função para verificar se um usuário existe e, se sim, retornar a senha criptografada
int usuarioExiste(const char *nome, char *senha) {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return 0;

    char nomeArquivo[MAX], senhaArquivo[MAX];
    while (fscanf(file, "%s %s", nomeArquivo, senhaArquivo) != EOF) {
        if (strcmp(nomeArquivo, nome) == 0) {
            if (senha) strcpy(senha, senhaArquivo); // Copia a senha criptografada, se solicitada
            fclose(file);
            return 1; // Usuário encontrado
        }
    }
    fclose(file);
    return 0; // Usuário não encontrado
}

// Função para cadastrar um novo usuário com verificação de existência
void cadastrarUsuario() {
    FILE *file = fopen(FILE_NAME, "a");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[MAX], senha[MAX], senhaCriptografada[MAX];
    printf("Digite o nome do usuário que deseja cadastrar: ");
    scanf(" %[^\n]", nome); // Lê nome com acentuação e espaços
    criptografar(nome); // Criptografa o nome para busca

    if (usuarioExiste(nome, NULL)) {
        printf("Usuário já existe! Escolha outro nome.\n");
        return;
    }

    // Solicita e criptografa a senha para armazenar
    printf("Digite a senha do usuário: ");
    scanf(" %[^\n]", senha); // Lê senha com acentuação e espaços
    criptografar(senha);

    fprintf(file, "%s %s\n", nome, senha); // Salva o nome e a senha criptografados
    fclose(file);
    printf("Usuário cadastrado com sucesso!\n");
}

// Função para realizar o login de um usuário e verificar senha
int loginUsuario() {
    char nome[MAX], senha[MAX], senhaCriptografada[MAX];
    printf("Digite seu nome de usuário para login: ");
    scanf(" %[^\n]", nome); // Lê nome com acentuação e espaços
    criptografar(nome); // Criptografa o nome para buscar no arquivo

    if (!usuarioExiste(nome, senhaCriptografada)) {
        printf("Usuário não encontrado. Verifique o nome ou cadastre-se.\n");
        return 0;
    }

    printf("Digite sua senha: ");
    scanf(" %[^\n]", senha); // Lê senha com acentuação e espaços
    criptografar(senha); // Criptografa a senha para comparação

    if (strcmp(senha, senhaCriptografada) == 0) {
        printf("Login realizado com sucesso!\n");
        return 1;
    } else {
        printf("Senha incorreta. Tente novamente.\n");
        return 0;
    }
}

// Função para alterar a senha de um usuário logado
void alterarSenha() {
    char nome[MAX], novaSenha[MAX], senhaCriptografada[MAX];
    printf("Digite o nome do usuário para alterar a senha: ");
    scanf(" %[^\n]", nome); // Lê nome com acentuação e espaços
    criptografar(nome); // Criptografa o nome para busca

    if (!usuarioExiste(nome, senhaCriptografada)) {
        printf("Usuário não encontrado. Verifique o nome.\n");
        return;
    }

    // Confirmação de login para segurança
    printf("Digite sua senha atual: ");
    char senhaAtual[MAX];
    scanf(" %[^\n]", senhaAtual); // Lê senha atual com acentuação e espaços
    criptografar(senhaAtual);

    if (strcmp(senhaAtual, senhaCriptografada) != 0) {
        printf("Senha atual incorreta. Não foi possível alterar a senha.\n");
        return;
    }

    // Solicita e criptografa a nova senha
    printf("Digite sua nova senha: ");
    scanf(" %[^\n]", novaSenha); // Lê nova senha com acentuação e espaços
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

// Função para listar todos os usuários (apenas nomes)
void listarUsuarios() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char nome[MAX], senha[MAX];
    printf("Lista de Usuários:\n");
    while (fscanf(file, "%s %s", nome, senha) != EOF) {
        criptografar(nome); // Descriptografa o nome para exibir
        printf("- %s\n", nome);
    }
    fclose(file);
}

// Função do menu principal
void menu() {
    int opcao;
    do {
        printf("\n1. Cadastrar Usuário\n2. Login\n3. Alterar Senha\n4. Listar Usuários\n5. Sair\nEscolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: cadastrarUsuario(); break;
            case 2: if (loginUsuario()) printf("Bem-vindo ao sistema!\n"); break;
            case 3: alterarSenha(); break;
            case 4: listarUsuarios(); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n"); break;
        }
    } while(opcao != 5);
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    menu();
    return 0;
}

