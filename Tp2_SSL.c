#include <stdio.h>

// ==================================================
// FUNCIONES AUXILIARES
// ==================================================

int esDigito(char c){
    return c >= '0' && c <= '9';
}

int esHexaDigito(char c){
    return (c >= '0' && c <= '9') ||
           (c >= 'A' && c <= 'F') ||
           (c >= 'a' && c <= 'f');
}


// ==================================================
// EJERCICIO 1 - Reconocimiento de constantes enteras
// ==================================================

typedef enum {
    estado_inicio,
    estado_signo,
    estado_decimal,
    estado_cero,
    estado_octal,
    estado_prefijo_hexadecimal,
    estado_hexadecimal,
    estado_error
} Estado;

int contadorDecimal = 0;
int contadorOctal = 0;
int contadorHexadecimal = 0;
int contadorErrores = 0;

Estado analizar_numero(char *num){
    Estado estado = estado_inicio;

    for(int i = 0; num[i] != '\0'; i++){
        char c = num[i];

        switch(estado){

            case estado_inicio:
                if(c == '+' || c == '-'){
                    estado = estado_signo;
                }
                else if(c == '0'){
                    estado = estado_cero;
                }
                else if(c >= '1' && c <= '9'){
                    estado = estado_decimal;
                }
                else{
                    estado = estado_error;
                }
                break;

            case estado_signo:
                if(esDigito(c)){
                    estado = estado_decimal;
                }
                else{
                    estado = estado_error;
                }
                break;

            case estado_decimal:
                if(!esDigito(c)){
                    estado = estado_error;
                }
                break;

            case estado_cero:
                if(c == 'x' || c == 'X'){
                    estado = estado_prefijo_hexadecimal;
                }
                else if(c >= '0' && c <= '7'){
                    estado = estado_octal;
                }
                else{
                    estado = estado_error;
                }
                break;

            case estado_octal:
                if(c >= '0' && c <= '7'){
                    estado = estado_octal;
                }
                else{
                    estado = estado_error;
                }
                break;

            case estado_prefijo_hexadecimal:
                if(esHexaDigito(c)){
                    estado = estado_hexadecimal;
                }
                else{
                    estado = estado_error;
                }
                break;

            case estado_hexadecimal:
                if(!esHexaDigito(c)){
                    estado = estado_error;
                }
                break;

            case estado_error:
                return estado_error;

            default:
                return estado_error;
        }
    }

    if(estado == estado_signo ||
       estado == estado_prefijo_hexadecimal){
        return estado_error;
    }

    if(estado == estado_cero){
        return estado_octal;
    }

    return estado;
}

void analizar_cadena(char *cadena){
    char numero[100];
    int j = 0;

    for(int i = 0; ; i++){
        char c = cadena[i];

        if(c == '@' || c == '\0'){
            numero[j] = '\0';

            Estado estado = analizar_numero(numero);

            if(estado == estado_decimal){
                contadorDecimal++;
            }
            else if(estado == estado_octal){
                contadorOctal++;
            }
            else if(estado == estado_hexadecimal){
                contadorHexadecimal++;
            }
            else{
                contadorErrores++;
                printf("Error lexico en %s\n", numero);
            }

            j = 0;

            if(c == '\0'){
                break;
            }
        }
        else{
            numero[j] = c;
            j++;
        }
    }

    printf("Decimales: %d\n", contadorDecimal);
    printf("Octales: %d\n", contadorOctal);
    printf("Hexadecimales: %d\n", contadorHexadecimal);
}


// ==================================================
// EJERCICIO 2 - Conversion de caracter a entero
// ==================================================

int caracterAEntero(char c){
    return c - '0';
}


// ==================================================
// EJERCICIO 3 - Validacion y evaluacion de expresiones
// ==================================================

typedef enum {
    esperando_numero,
    leyendo_numero,
    esperando_otro_numero,
    expresion_error
} EstadoExpresion;

int validarExpresion(char *expresion){
    EstadoExpresion estado = esperando_numero;

    for(int i = 0; expresion[i] != '\0'; i++){
        char c = expresion[i];

        switch(estado){

            case esperando_numero:
                if(esDigito(c)){
                    estado = leyendo_numero;
                }
                else{
                    estado = expresion_error;
                }
                break;

            case leyendo_numero:
                if(esDigito(c)){
                    estado = leyendo_numero;
                }
                else if(c == '+' || c == '-' || c == '*'){
                    estado = esperando_otro_numero;
                }
                else{
                    estado = expresion_error;
                }
                break;

            case esperando_otro_numero:
                if(esDigito(c)){
                    estado = leyendo_numero;
                }
                else{
                    estado = expresion_error;
                }
                break;

            case expresion_error:
                return 0;
        }
    }

    return estado == leyendo_numero;
}

int evaluarExpresion(char *expresion){
    int i = 0;
    int resultado = 0;
    int termino = 0;

    while(esDigito(expresion[i])){
        termino = termino * 10 + caracterAEntero(expresion[i]);
        i++;
    }

    while(expresion[i] != '\0'){
        char operador = expresion[i];
        i++;

        int numero = 0;

        while(esDigito(expresion[i])){
            numero = numero * 10 + caracterAEntero(expresion[i]);
            i++;
        }

        if(operador == '*'){
            termino = termino * numero;
        }
        else if(operador == '+'){
            resultado += termino;
            termino = numero;
        }
        else if(operador == '-'){
            resultado += termino;
            termino = -numero;
        }
    }

    resultado += termino;

    return resultado;
}


// ==================================================
// PROGRAMA PRINCIPAL
// ==================================================

int main(){

    // Ejercicio 1
    char cadena[100];

    printf("Ingrese una cadena: ");
    scanf("%99s", cadena);

    analizar_cadena(cadena);


    // Ejercicio 2
    char caracter;

    printf("Ingrese un caracter numerico: ");
    scanf(" %c", &caracter);

    printf("Numero entero: %d\n", caracterAEntero(caracter));


    // Ejercicio 3
    char expresion[100];

    printf("Ingrese una expresion: ");
    scanf("%99s", expresion);

    if(validarExpresion(expresion)){
        printf("Expresion valida\n");
        printf("Resultado: %d\n", evaluarExpresion(expresion));
    }
    else{
        printf("Error lexico en la expresion\n");
    }

    return 0;
}