#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ATTEMPTS 6
#define WORD_LENGTH 20
#define HINT_LENGTH 100
#define MAX_WORDS 100

// Estrutura para armazenar uma palavra e sua dica
typedef struct {
    char word[WORD_LENGTH];
    char hint[HINT_LENGTH];
} Word;

// Função para carregar as palavras e dicas do arquivo
int load_words(const char *filename, Word words[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir o arquivo");
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%[^:]:%[^\n]\n", words[count].word, words[count].hint) == 2) {
        count++;
        if (count >= MAX_WORDS) break;
    }

    fclose(file);
    return count;
}

// Exibe o progresso do jogador
void display_progress(const char *word, const int *correct) {
    printf("Palavra: ");
    for (int i = 0; i < strlen(word); i++) {
        if (correct[i]) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// Exibe o estado do "boneco" da forca
void display_hangman(int attempts) {
    const char *stages[] = {
        "  +---+\n      |\n      |\n      |\n     ===",
        "  +---+\n  O   |\n      |\n      |\n     ===",
        "  +---+\n  O   |\n  |   |\n      |\n     ===",
        "  +---+\n  O   |\n /|   |\n      |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n      |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n /    |\n     ===",
        "  +---+\n  O   |\n /|\\  |\n / \\  |\n     ==="
    };
    printf("%s\n", stages[attempts]);
}

// Verifica se o jogador adivinhou toda a palavra
int check_win(const int *correct, int length) {
    for (int i = 0; i < length; i++) {
        if (!correct[i]) {
            return 0; // Há letras não descobertas
        }
    }
    return 1; // Todas as letras foram descobertas
}

int main() {
    Word words[MAX_WORDS];
    int word_count = load_words("palavras.txt", words);

    if (word_count == 0) {
        printf("Nenhuma palavra encontrada. Certifique-se de que o arquivo 'palavras.txt' existe e tem o formato correto.\n");
        return 1;
    }

    srand(time(NULL));
    int chosen_index = rand() % word_count;
    const char *word = words[chosen_index].word;
    const char *hint = words[chosen_index].hint;
    int length = strlen(word);
    int correct[WORD_LENGTH] = {0};
    int attempts = 0;
    char guess;
    int found;

    printf("Bem-vindo ao Jogo da Forca!\n");
    printf("Dica: %s\n", hint);

    while (attempts < MAX_ATTEMPTS) {
        display_hangman(attempts);
        display_progress(word, correct);
        printf("Tentativas restantes: %d\n", MAX_ATTEMPTS - attempts);
        printf("Digite uma letra: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        found = 0;
        for (int i = 0; i < length; i++) {
            if (word[i] == guess && !correct[i]) {
                correct[i] = 1;
                found = 1;
            }
        }

        if (!found) {
            attempts++;
            printf("Letra incorreta!\n");
        }

        if (check_win(correct, length)) {
            printf("Parabéns! Você adivinhou a palavra: %s\n", word);
            return 0;
        }
    }

    display_hangman(attempts);
    printf("Você perdeu! A palavra era: %s\n", word);
    return 0;
}