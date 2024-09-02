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


1. **Mapa Isométrico**:
  - El mapa isométrico es el principal en el juego y está diseñado para simular un entorno en 3D, proporcionando una experiencia visual más inmersiva. Este mapa está conformado por una cuadrícula de 8 x 8, lo que permite una representación detallada y atractiva del espacio de juego, donde los objetos y personajes tienen una apariencia tridimensional a pesar de ser un plano bidimensional.
   ![mapa3d](https://github.com/user-attachments/assets/ecece153-816b-4ae7-8962-8dd609669b7d)


2. **Mapa 2D**:
  - El mapa 2D sirve como referencia para el jugador, mostrando de manera clara y sencilla la posición actual del automóvil dentro del entorno del juego. Al igual que el mapa isométrico, el mapa 2D está conformado por una cuadrícula de 8 x 8, pero su principal función es brindar una vista simplificada y directa de la ubicación y movimientos del jugador dentro del espacio de juego.
   ![mapa2d](https://github.com/user-attachments/assets/b6fe8e55-05be-4be3-9c01-6fce336bc479)


3. **Personaje (Automóvil)**:
  - El automóvil es el personaje principal que el jugador controla mediante una serie de instrucciones predefinidas.
   ![mapa3D_2D_CARRO](https://github.com/user-attachments/assets/3e681e5b-b33e-4158-b8e5-f71fefff8a55)


4. **Casillas de Instrucciones**:
  - En la esquina superior derecha de la pantalla se encuentra un **panel de instrucciones** donde el jugador puede seleccionar y ordenar las acciones que desea que el automóvil realice. Las instrucciones disponibles permiten al jugador planificar el movimiento del automóvil dentro del mapa. A continuación, se describen las funciones de cada casillero:
    - **Primer Casillero**: El automóvil avanza una celda hacia adelante.
    - **Segundo Casillero**: El automóvil gira 90 grados en sentido horario.
    - **Tercer Casillero**: El automóvil gira 90 grados en sentido antihorario.
    - **Cuarto Casillero**: Instrucción de función; permite llamar a las instrucciones definidas en el arreglo de función dentro del arreglo principal.
    - **Quinto Casillero**: Casillero para llamar al arreglo de bucle dentro del arreglo principal, permitiendo repetir las instrucciones según lo definido.
   ![instruciones1 1](https://github.com/user-attachments/assets/042bd362-4083-4e67-8099-6b6c49ffef4a)


5. **Casillas para Instrucciones**:


  - En cada casillero de los arreglos, hay dos cuadrados pequeños en la parte izquierda:
     - **Cuadrado Superior (Rojo)**: Este cuadro sirve para eliminar una instrucción del casillero. Al hacer clic en él, se borra la instrucción actualmente asignada, permitiendo al jugador reorganizar o limpiar la secuencia de acciones fácilmente.
     - **Cuadrado Inferior (Verde/Amarillo)**: Este cuadro controla la habilitación de cada casillero para recibir instrucciones. Cuando está de color verde, significa que el casillero está bloqueado y no puede recibir instrucciones. Al hacer clic en este cuadro, cambia a color amarillo, lo que indica que el casillero está desbloqueado y listo para recibir nuevas instrucciones. Si se vuelve a hacer clic, regresa a verde, bloqueando nuevamente el casillero.


   ![boton_rojo_verde](https://github.com/user-attachments/assets/0ca7cdc4-6cce-4530-9fb2-459ed81da142)


  - **Arreglo Principal**: Este arreglo contiene la secuencia completa de instrucciones que se ejecutará en el juego. Es la base sobre la cual se desarrollan las acciones del automóvil y también define el orden de los movimientos.
   ![array_principal](https://github.com/user-attachments/assets/5a9a86fe-86ea-42f2-9354-0566db05ca38)




  - **Arreglo de Función**: Este arreglo permite al jugador almacenar una serie de instrucciones personalizadas que pueden ser llamadas desde el arreglo principal. Es útil para definir rutinas o secuencias de movimientos complejos que se usan repetidamente, optimizando así la gestión de acciones y evitando tener que reescribir las mismas instrucciones múltiples veces.
   ![array_funcion](https://github.com/user-attachments/assets/379f42af-6cdd-468c-92bf-80adc3d8d309)


  - **Arreglo de Bucle**: En este arreglo, el jugador puede colocar instrucciones que desea repetir un número específico de veces, permitiendo la ejecución de bucles de acciones dentro del juego. Es ideal para automatizar movimientos cíclicos, como giros continuos o avances repetidos, proporcionando una forma eficiente de ejecutar patrones sin necesidad de escribir las mismas instrucciones repetidamente.
   ![array_bucle](https://github.com/user-attachments/assets/364ee835-be17-445c-8c55-455e25796a4a)



6. **Ejecución de Instrucciones**:
   - Después de que el jugador selecciona y organiza las instrucciones en las casillas, puede presionar el botón "Go" para ejecutar la secuencia de comandos. El automóvil seguirá las indicaciones dadas, moviéndose por el mapa según las instrucciones programadas.
   - Las instrucciones programadas se guardan en un archivo de texto (`instrucciones.txt`), permitiendo revisar y ajustar las secuencias de movimiento cuando sea necesario.
   - Como ejemplo de ejecución, incluiremos un video en el que se muestra una secuencia específica: primero, se le da al automóvil una instrucción para girar 90 grados en sentido horario, seguida de una instrucción para avanzar una casilla hacia adelante. Luego, se utiliza una instrucción de función que llama a las acciones definidas en el arreglo de función; en este caso, tres flechas que indican avanzar hacia adelante. Al llegar a una casilla azul, el automóvil cambia automáticamente de mapa, avanzando al siguiente nivel del juego.
   
    [Screencast from 01-09-24 21_08_59.webm](https://github.com/user-attachments/assets/9bc84f70-2e84-4d99-96f7-ecfde04423f9)



## Flujo de Juego


1. El jugador observa el mapa isométrico y el mapa 2D para planificar la ruta del automóvil.


2. Selecciona las instrucciones deseadas y las organiza en el orden adecuado dentro de las casillas de instrucciones.


3. Puede crear funciones personalizadas y bucles para simplificar la secuencia de acciones.


4. Presiona el botón "Go" para que el automóvil ejecute las instrucciones en el orden programado.


5. El automóvil se moverá según las instrucciones, y el objetivo es alcanzar la casilla azul sin chocar con los obstáculos.


## Conclusión


Este proyecto combina la lógica y la estrategia en un entorno de juego interactivo donde los jugadores deben pensar en secuencias de movimientos para guiar un automóvil a través de un mapa desafiante. Es un ejercicio que pone a prueba la planificación y el pensamiento lógico de los jugadores, mientras se divierten.
