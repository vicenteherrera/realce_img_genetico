Realce de imágenes en escala de grises mediante algoritmos genéticos usando laplacianos
=======================================================================================

Realizado para la asignatura de Procesamiento de Imágenes Digitales de 4º curso
de Ingeniería Informática en la Universidad de Sevilla.

Por Vicente Herrera, Javier Gallego y Javier Rodriguez durante el curso
2003-2004 con la profesora Rocío González.

Introducción
------------

Este programa basa su trabajo en el paper “[Restoration of gray images based on
a genetic algorithm with Laplacian constraint](<ref_paper/cna99.pdf>)” Y.-W.
Chen et al, I Fuzzy Sets and Systems 103 (1999), 285-293.

En el paper la aplicación buscada para este procesamiento consiste en afinar el
enfoque de un disparador laser sobre el objetivo para iniciar la reacción de un
generador de fusión nuclear.

En el ejemplo aquí programado, se distorsiona artificialmente una imagen de
prueba, para luego mediante sucesivas iteraciones intentar mejorar ésta para que
se parezca cada vez más a la imagen original.

Puedes consultar la [presentación explicativa en este
enlace.](<presentacion/PresenttabrajoPID.pdf>)

Características del desarrollo
------------------------------

Para ello se ha implementado un algoritmo genético clásico en **C** puro, mediante
**Visual Studio v6**, sin emplear ninguna librería adicional (todo el código
construido es de creación propia), como un ejecutable de linea de comandos.
Todos los datos utilizados utilizan **asignación dinámica de memoria y
punteros**, permitiendo aumentar indefinidamente la complejidad del problema
(mientras se tenga memoria disponible), y consiguiendo una velocidad de
ejecución muy grande.

El [código fuente de ese componente ejecutable](<codigo_fuente/GREALCE>) es la
parte más importante de este proyecto:

-   [codigo\_fuente/GREALCE/Genaux.c](<codigo_fuente/GREALCE/Genaux.c>)

-   [codigo\_fuente/GREALCE/Genetic.c](<codigo_fuente/GREALCE/Genetic.c>)

-   [codigo\_fuente/GREALCE/Gengrf.c](<codigo_fuente/GREALCE/Gengrf.c>)

También se implementó un interfaz de usuario como un ejecutable separado
empleando **C++Builder**, que permite establecer estos parámetros y enviarlos al
componente de linea de comandos, lo que hace su utilización muy sencilla.

Ejemplo de ejecución
--------------------

Puedes comprobar una ejecución del programa en [esta página de resultados
generada](<http://htmlpreview.github.io/?https://github.com/vicenteherrera/realce_img_genetico/blob/master/example_run/index.htm>)
por el mismo.

Evaluación
----------

El presente proyecto fue evaluado con una nota de 9,5 sobre 10.
