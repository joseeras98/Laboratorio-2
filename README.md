# Laboratorio 2: GPIOs, Timers y FSM.

El desarrollo de este proyecto de cruce de semáforos simplificado combina el uso de microcontroladores, LEDs, botones y resistencias para crear un sistema de regulación de tráfico vehicular y peatonal. La simulación a través de Simulide proporciona una herramienta valiosa para verificar su funcionamiento.

El diseño comprende la implementación de resistores, capacitores y un total de seis LEDs para representar visualmente los semáforos. Dos de estos LEDs, denominados LDPV (LED Derecho Peatones Vehiculares) y LDVD (LED Derecho Vehículos) son destinados para la representación del semáforo vehicular, mientras que LDPP (LED Derecho Peatones Pulsador) y LDPD (LED Derecho Peatones Detención) se utilizan para el semáforo peatonal. La inclusión de estos elementos permite una visualización clara de las señales de tráfico para conductores y peatones. Para la interacción del usuario, se han incorporado dos botones, identificados como B1 y B2. Estos botones son fundamentales ya que los usuarios pueden utilizarlos para solicitar la activación de las luces peatonales y permitir un cruce seguro de la calle.


![semaforo(1)](https://github.com/joseeras98/Laboratorio-2/assets/56570687/8401224f-6350-4228-bdbd-ae6c74cc2b35)

![circuito semaforo](https://github.com/joseeras98/Laboratorio-2/assets/56570687/631367bb-efe0-41a1-b69a-28bafbd945bb)


## Guia de uso

1. Clonar el repositorio https://github.com/joseeras98/Laboratorio-2.git.
2. Moverse a la carpeta src.
3. Ejecutar el archivo Makefile.
4. Abrir manualmente el simulador SimulIDE.
5. Cargar el archivo con extensión .simu.
6. En el controlador ATtiny4313, hacer click izquierdo y cargar el firmware, el cual tiene extensión .hex y se crea automáticamente en el paso número 3.
7. Correr la simulación.
