#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Definición de los estados
#define A 0 // Paso de vehículos
#define B 1 // Parpadeo de luz de semáforo vehicular
#define C 2 // Paro total de vehículos y peatones
#define D 3 // Paso de peatones
#define E 4 // Parpadeo de luces de semáforos peatonales

// Configuración del timer
void timer_setup() {
  TCCR0A = 0x00; // Modo normal
  TCCR0B = (1 << CS00) | (1 << CS02); // Prescaling de 1024
  TCNT0 = 0;
  TIMSK = (1 << TOIE0); // Habilita la interrupción del timer0
}

// Funciones de salida en cada estado
void paso_vehicular_ON(void) {
  PORTB = (1 << PB3) | (1 << PB7); // Enciende la luz verde del semáforo vehicular (B3) y se enciende la luz roja del semaforo peatonal (B7)
}

void paso_vehicular_WARNING(void) {
  PORTB ^= (1 << PB3); // Parpadeo de la luz verde del semáforo vehicular (B3)
}

void luces_OFF(void) {
  PORTB = 0x00; // Apaga todas las luces
}

void paso_peatonal_ON(void) {
  PORTB = (1 << PB0) | (1 << PB4); // Enciende la luz verde del semáforo peatonal (B0) y se enciende la luz roja del semaforo vehicular (B4)
}

void paso_peatonal_WARNING(void) {
  PORTB ^= (1 << PB0); // Parpadeo de la luz verde del semáforo peatonal (B0)
}

// struct que define los miembros que componen cada estado de la máquina
typedef struct Semaforo {
  void (*state_func_ptr)(void); // Puntero a las funciones de salida de cada estado
  int time; // Tiempo de duración de cada estado
} FSM;

// Se define la máquina de estados y su tiempo de duración
FSM semaforo[5] = {
  {&paso_vehicular_ON, 10},
  {&paso_vehicular_WARNING, 3},
  {&luces_OFF, 1},
  {&paso_peatonal_ON, 10},
  {&paso_peatonal_WARNING, 3},
};

unsigned char state = A; // Estado la máquina
int int_count = 0; // Contador de timer
int TVP = 0; // Contador de 10 segundos
int TI = 0; // Contador de 1 segundo
int pass_flag = 0; // Indicador de paso (vehículos o peatones)

int main(void) {
  DDRB = (1 << PB7) | (1 << PB0) | (1 << PB4) | (1 << PB3); // Configura los pines de salida
  GIMSK = (1 << INT1); // Habilita la interrupción externa en INT1
  MCUCR = (1 << ISC11); // Interrupción generada por flanco de bajada en INT1
  timer_setup(); // Configura el temporizador
  sei(); // Habilita las interrupciones globales

  while (1) {
    // Lógica de la máquina de estados
    switch (state) {
      case A:
        (semaforo[A].state_func_ptr)(); // Llama a la función de salida del estado A
        if ((pass_flag == 1) && (TVP >= semaforo[A].time)) {
          state = B; // Cambia al estado B
          int_count = 0;
          TVP = 0;
          TI = 0;
          pass_flag = 0;
        }
        break;
      case B:
        if (TI == semaforo[B].time) {
          state = C; // Cambia al estado C
          int_count = 0;
          TVP = 0;
          TI = 0;
        }
        break;
      case C:
        (semaforo[C].state_func_ptr)(); // Llama a la función de salida del estado C
        if (TI == semaforo[C].time) {
          int_count = 0;
          TVP = 0;
          TI = 0;
          if (pass_flag == 0) {
            state = D; // Cambia al estado D
          }
          else if (pass_flag == 1) {
            state = A; // Cambia al estado A
          }
        }
        break;
      case D:
        (semaforo[D].state_func_ptr)(); // Llama a la función de salida del estado D
        if (TVP >= semaforo[D].time) {
          state = E; // Cambia al estado E
          int_count = 0;
          TVP = 0;
          TI = 0;
        }
        break;
      case E:
        if (TI == semaforo[E].time) {
          state = C; // Cambia al estado C
          int_count = 0;
          TVP = 0;
          TI = 0;
          pass_flag = 1;
        }
        break;
    }
  }
}

ISR(INT1_vect) {
  pass_flag = 1;
}

ISR(TIMER0_OVF_vect) {
  if ((int_count == 1) || (int_count == 15)) { // Cuenta de medio segundo
    (semaforo[state].state_func_ptr)(); // Llamado a salidas del estado actual
  }
  else if (int_count == 30) { // Cuenta de un segundo
    ++TI;
    int_count = 0;
  }
  if (TI == 10) {
    ++TVP;
  }
  ++int_count;
}