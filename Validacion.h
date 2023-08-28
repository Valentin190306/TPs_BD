#ifndef VALIDACION_C
#define VALIDACION_C

/*
Instrucciones de uso de la función "EntradaEntera": esta función emite un aviso de reentrada por cada caso de no validación:
    - Si la entrada es decimal (opcional).
    - Si la entrada contiene caracteres alfabeticos.
    - Si la entrada no es natural (opcional).
    - Si la entrada se halla fuera de rango (opcional).

La función retornara un nro. entero válido según si se requiera que sea natural o que se halle dentro de un rango como si no.

1er parámetro (char buffer[]): la instrucción requiere que toda ingreso por teclado a verificar sea proveido dentro de un "char vector[]".
todos los espacios del vector deben estar inicializados con el carácter "\0" antes de asignarle el input.

2do parámetro (int nroNat): parámetro de uso opcional, funciona como un booleano (1 = verdadero, 0 = falso). 
Si se desea que la entrada solo admita nros. naturales se debe poner este parámetro en 1, e.g. "EntradaEntera(entrada, 1, 0, 0)". 
en caso contrario, ponga 0, e.g. "EntradaEntera(entrada, 1, 0, 0)". 

3er parámetro y 4to parámetro (int LimInferior, int LimSuperior): Si se desea, la función puede verificar que la entrada se encuentre 
dentro de un rango dado incluyendo los extremos,  e.g. "EntradaEntera(entrada, 1, 1, 5)". Para inhabilitar esta funcionalidad se debe poner 
ambos limites con un mismo valor, e.g. "EntradaEntera(entrada, 1, 0, 0)". 
*/

int EntradaEntera(char buffer[], int nroNat, int LimInferior, int LimSuperior);

/*
Instrucciones de uso de la función "CadenaValida": esta función retorna un int como booleano (1 = V, 0 = F) dado una cadena de caracteres que se desea verficar 
y otra cadena con los caracteres permitidos (en mayúscula) por el programador, e.g. CadenaValida(entrada, {'a', 'b', 'c', '\0'}, NULL).
Se pueden ingresar tanto cadena de caracteres permitidos prohibidos como permitidos. Si se desea verficar que la cadena tiene caracteres 
no permitidos, ingresar una cedena con los caracteres prohibidos, ejemplo: CadenaValida(entrada, NULL, {'1', ' ', '%'});
Atención: no ingresar dos cadenas de verificación a la vez, ejemplo: CadenaValida(entrada, {'a', 'b', 'c', '\0'}, {'1', ' ', '%'});
*/

int CadenaValida(char entrada[], char permitidos[], char prohibidos[]);

#endif