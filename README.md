Realce de imágenes en escala de grises mediante algoritmo genético
==================================================================

Realizado para la asignatura de Procesamiento de Imágenes Digitales de 4º curso
de Ingeniería Informática.

Este programa basa su trabajo en el paper “[Restoration of gray images based on
a genetic algorithm with Laplacian constraint](<ref_paper/cna99.pdf>)” Y.-W.
Chen et al, I Fuzzy Sets and Systems 103 (1999), 285-293.

En el paper la aplicación de este procesamiento consiste en afinar el enfoque de
un disparador laser sobre el objetivo para iniciar la reacción de un generador
de fisión nuclear.

En el ejemplo aquí programado, se distorsiona artificialmente la imagen, para
luego mediante sucesivas iteraciones intentar mejorar esta para que se parezca
más a la imagen original.

Para ello se ha implementado un algoritmo genético clásico en C++ sin emplear
ninguna libreria adicional (todo el código de creación propia), como un
ejecutable de linea de comandos. Todos los datos utilizados utilizan asignación
dinámica de memoria y punteros, permitiendo aumentar indefinidamente la
complejidad del problema (mientras se tenga memoria disponible), y consiguiendo
una velocidad de ejecución muy grande.

También se implementó un interfaz de usuario empleando C++Builder, que permite
establecer estos parámetros y enviarlos al componente de linea de comandos, lo
que hace su utilización muy sencilla.
