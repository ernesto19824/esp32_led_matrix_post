/*#include "Internet.h"

Internet internet;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("iniciando mensaje matriz leds v1");
  internet.Iniciar();
}

void loop() {
  // put your main code here, to run repeatedly:
  String mensaje = internet.LeerUltimoMensaje();

  Serial.print("Ultimo mensaje: ");
  Serial.println(mensaje);

  delay(2500);
}*/

//Ejemplo para enviar mensajes deslizantes en módulos de matrices LEDs con Arduino basado en MAX7219
#include <MD_MAX72xx.h>
#define HARDWARE_TYPE MD_MAX72XX::DR0CR0RR1_HW
#define NUM_OF_MATRIX 3
#define CLK_PIN   18
#define DATA_PIN  23
#define CS_PIN    4

MD_MAX72XX cartel = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, NUM_OF_MATRIX);

String mensaje = "electronica";

String proximo_mensaje = "";

#include "Internet.h"

Internet internet;

void setup() {
  // inicializar el objeto mx
  cartel.begin();

  // Establecer intencidad a un valor de 5
  cartel.control(MD_MAX72XX::INTENSITY, 10);

  // Desactivar auto-actualizacion
  cartel.control( MD_MAX72XX::UPDATE, false );

  // inicializar puerto Serie a 9600 baudios
  Serial.begin(9600);
  Serial.println("Iniciando led matrix!");
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.println("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  internet.Iniciar();
  Serial.println("Iniciando internet!");
}
void loop() {
  slide_text( 100 );
  actualizar_mensaje();
}

void actualizar_mensaje(){
  proximo_mensaje = internet.LeerUltimoMensaje();
  Serial.print("Proximo mensaje: ");
  Serial.println(proximo_mensaje);
}

void slide_text(int ms_delay){
  int col = 0;
  int last_pos;
  bool completo = false;
  
  cartel.clear();

  if(proximo_mensaje != ""){
    mensaje = proximo_mensaje;
  }

  while( completo == false ){
    last_pos = printText(col, mensaje);
    delay(ms_delay);
    col++;
    if( last_pos > (int)cartel.getColumnCount() )
      completo = true;
  }
}

int printText(int pos, const String text){
  int w;
  
  for( int i = 0; i < text.length(); i++ ){
    // imprimir letra

    w = cartel.setChar( pos, text[i] );
    // la proxima letra empieza donde termina esta
    pos = pos - w; 
    // Se deja una columna entre letras.
    cartel.setColumn(pos, B00000000);
    
    pos = pos - 1;
    
    if( pos < 0 )
      break;
      
  }
  cartel.update();
   
  return pos;
}
