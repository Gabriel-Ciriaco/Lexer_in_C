#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int ehDelimitador(char caract)
{
    return (caract == ' ' || caract == '+' || caract == '-'
            || caract == '*' || caract == '/' || caract == ','
            || caract == ';' || caract == '%' || caract == '>'
            || caract == '<' || caract == '=' || caract == '('
            || caract == ')' || caract == '[' || caract == ']'
            || caract == '{' || caract == '}');
}

int ehOperador(char charact)
{
    return (charact == '+' || charact == '-' || charact == '*'
            || charact == '/' || charact == '>' || charact == '<'
            || charact == '=');
}

int ehIdentificadorValido(char * string)
{
    return ((string[0] < 48 || string[0] > 57) && !ehDelimitador(string[0]));
}

int ehPalavraChave(char * string)
{
    const char * palavras_chave[]
        = { "auto",     "break",    "case",     "char",
            "const",    "continue", "default",  "do",
            "double",   "else",     "enum",     "extern",
            "float",    "for",      "goto",     "if",
            "int",      "long",     "register", "return",
            "short",    "signed",   "sizeof",   "static",
            "struct",   "switch",   "typedef",  "union",
            "unsigned", "void",     "volatile", "while" };


    const int QTD_PALAVRAS = 32;

    for (int i = 0; i < QTD_PALAVRAS; i++)
    {
        if (strcmp(string, palavras_chave[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int ehInteiro(char * string)
{
    if (string == NULL || *string == '\0')
    {
        return 0;
    }

    int i = 0;

    while (isdigit(string[i]))
    {
        i++;
    }

    return string[i] == '\0';
}

/*
    Retorna o token o qual é uma substring do nosso input.
*/
char * pegarSubstring(char * string, int ini, int fim)
{
    int tamanhoSubst = fim - ini + 1;

    char * substr = (char *) malloc((tamanhoSubst + 1) * sizeof(char));

    // Copia os caracteres da string para a substring.
    strncpy(substr, string + ini, tamanhoSubst);

    // Finaliza a string.
    substr[tamanhoSubst] = '\0';

    return substr;
}


/*
    Analisa Lexicalmente uma string dada.

    Para isso, utilizamos a técnica de Variable Sliding Window,
     por isso, iniciamos dois ponteiros left e o right.
*/
int analisadorLexico(char * input)
{
    int left = 0;
    int right = 0;

    int tamanho_input = strlen(input);

    while (right <= tamanho_input && left <= right)
    {
        if (!ehDelimitador(input[right]))
        {
            right++;
        }

        if (ehDelimitador(input[right]) && left == right)
        {
            if (ehOperador(input[right]))
            {
                printf("Token: Operador, Valor: %c\n", input[right]);
            }

            right++;
            left = right;

        }
        else if (ehDelimitador(input[right]) && left != right
                 || (right == tamanho_input && left != right))
        {
            char * substr = pegarSubstring(input, left, right - 1);

            if (ehPalavraChave(substr))
            {
                printf("Token: Palavra Chave, Valor: %s\n", substr);
            }

            else if (ehInteiro(substr))
            {
                printf("Token: Inteiro, Valor: %s\n", substr);
            }

            else if (ehIdentificadorValido(substr) && !ehDelimitador(input[right - 1]))
            {
                printf("Token: Identificador, Valor: %s\n", substr);
            }

            else if (!ehIdentificadorValido(substr) && !ehDelimitador(input[right - 1]))
            {
                printf("Token: Nao identificado, Valor: Indefinido");
            }

            // Evitar um memory leak.
            free(substr);

            left = right;
        }
    }

    return 0;
}

void interpretar_arquivo(FILE * arquivo_texto)
{
    char texto[100];

    while (fgets(texto, sizeof(texto), arquivo_texto) != NULL)
    {
        analisadorLexico(texto);
    }
}

void ler_arquivo(char * arquivo_path)
{
  FILE * arquivo_texto;

  arquivo_texto = fopen(arquivo_path, "r");

  if (!arquivo_texto)
  {
    printf("Nao foi possivel abrir o arquivo!");
  }
  else
  {
    interpretar_arquivo(arquivo_texto);
  }

  fclose(arquivo_texto);
}

int main()
{
    char nome_arquivo[30];

    printf("Digite o nome do arquivo: ");

    scanf("%s", nome_arquivo);

    ler_arquivo(nome_arquivo);


    return 0;
}
