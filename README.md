# Makybot
## Introducción

En la enseñanza de la programación, uno de los desafíos recurrentes es hacer que los conceptos abstractos sean comprensibles y atractivos para los estudiantes. Muchos principiantes enfrentan dificultades para asimilar conceptos fundamentales como la lógica de programación, las estructuras de control y el pensamiento algorítmico. Los métodos tradicionales de enseñanza, a menudo basados en la resolución de problemas teóricos y ejercicios lineales, pueden resultar monótonos y poco estimulantes, lo que puede llevar a una baja motivación y a una comprensión superficial de los conceptos.

Para abordar este desafío, hemos creado Makybot, un juego interactivo diseñado para enseñar los fundamentos de la lógica de programación de una manera divertida y accesible. El juego presenta un mapa isométrico en 3D y un mapa en 2D, donde el jugador guía a un automóvil hacia una casilla azul, evitando obstáculos en su camino. La interfaz permite al jugador seleccionar y organizar una serie de instrucciones que el automóvil debe seguir, como girar, avanzar, crear funciones personalizadas, y repetir acciones. Una vez que el jugador ha configurado sus instrucciones, puede presionar "Go" para ver cómo se ejecutan en tiempo real.

Este enfoque interactivo no solo facilita la comprensión de conceptos clave, sino que también mantiene el interés del estudiante al combinar el aprendizaje con elementos de juego.

## Objetivo del Juego

Este enfoque interactivo no solo facilita la comprensión de conceptos clave, sino que también mantiene el interés del estudiante al combinar el aprendizaje con elementos de juego. Para el desarrollo de este proyecto, utilizamos C++ como lenguaje de programación, la biblioteca SFML para la gestión de gráficos y eventos, y GitHub para la gestión del control de versiones y la colaboración en el código.


### Otros objetivos clave:

1. **Desarrollar habilidades de pensamiento algorítmico:**  
   A través de la creación y organización de instrucciones, los jugadores aprenderán a descomponer problemas complejos en pasos más simples, fomentando el desarrollo del pensamiento algorítmico.

2. **Fomentar la comprensión de estructuras de control:**  
   El juego introduce a los jugadores en el uso de estructuras de control fundamentales como bucles y funciones personalizadas, permitiéndoles repetir acciones y optimizar sus soluciones.

3. **Estimular la creatividad y la resolución de problemas:**  
   Los jugadores son incentivados a experimentar con diferentes combinaciones de instrucciones para encontrar la solución más eficiente y creativa, reforzando sus habilidades de resolución de problemas en un entorno lúdico.


## Funcionalidades Principales

1. **Mapa Isométrico y Mapa 2D**:
   - El mapa isométrico simula un entorno en 3D, mientras que el mapa 2D sirve como referencia para que el jugador vea la posición actual del automóvil.
   - ![mapa3D_2D](https://github.com/user-attachments/assets/7772a479-fec6-42c2-9afa-adad02a30b54)

2. **Personaje (Automóvil)**:
   - El automóvil es el personaje principal que el jugador controla mediante una serie de instrucciones predefinidas.
   - ![mapa3D_2D_CARRO](https://github.com/user-attachments/assets/3e681e5b-b33e-4158-b8e5-f71fefff8a55)

3. **Casillas de Instrucciones**:
   - En la esquina superior derecha de la pantalla se encuentra un panel de instrucciones donde el jugador puede seleccionar y ordenar las acciones que desea que el automóvil realice.
   - Las instrucciones disponibles incluyen: girar a la izquierda, girar a la derecha, avanzar una casilla, crear una función personalizada, y establecer un bucle para repetir instrucciones.
   - ![instruciones1 1](https://github.com/user-attachments/assets/042bd362-4083-4e67-8099-6b6c49ffef4a)

4. **Casillas para Instrucciones**:
   - **Arreglo Principal**: Contiene la secuencia de instrucciones que se ejecutará.
   - **Arreglo de Función**: Almacena una serie de instrucciones personalizadas que pueden ser ejecutadas dentro del arreglo principal.
   - **Arreglo de Bucle**: Contiene instrucciones que el jugador desea repetir un número específico de veces.
   - ![instruciones2 1](https://github.com/user-attachments/assets/8c8ff883-1094-48a1-a4a3-dd63e59c7f14)

5. **Ejecución de Instrucciones**:
   - Después de que el jugador selecciona y organiza las instrucciones en las casillas, puede presionar el botón "Go" para ejecutar la secuencia de instrucciones. El automóvil se moverá según las indicaciones dadas.
   - Las instrucciones programadas se guardan en un archivo de texto (`instrucciones.txt`).
   -  ![image](https://github.com/user-attachments/assets/72cf7cad-41ff-4232-8cf6-ec301ae1a6c0)


## Flujo de Juego

1. El jugador observa el mapa isométrico y el mapa 2D para planificar la ruta del automóvil.

2. Selecciona las instrucciones deseadas y las organiza en el orden adecuado dentro de las casillas de instrucciones.

3. Puede crear funciones personalizadas y bucles para simplificar la secuencia de acciones.

4. Presiona el botón "Go" para que el automóvil ejecute las instrucciones en el orden programado.

5. El automóvil se moverá según las instrucciones, y el objetivo es alcanzar la casilla azul sin chocar con los obstáculos.

## Conclusión

Este proyecto combina la lógica y la estrategia en un entorno de juego interactivo donde los jugadores deben pensar en secuencias de movimientos para guiar un automóvil a través de un mapa desafiante. Es un ejercicio que pone a prueba la planificación y el pensamiento lógico de los jugadores, mientras se divierten.
