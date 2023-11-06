#ifndef Internet_h
#define Internet_h

#include <Arduino.h> // Asegúrate de incluir la biblioteca Arduino si estás utilizando tipos de datos de Arduino
#include <WiFi.h>
#include <HTTPClient.h>

class Internet {
public:
    Internet(); // Constructor
    void Iniciar();
    void CambiarWifi(String ssid, String password);
    void CambiarEndpoint(String nuevoEnpoint, String tokenAuth = "");
    void Terminar();
    bool ComprobarConexion();

    //funcion propia del projecto
    String LeerUltimoMensaje();


private:
    //valores por defecto
    const char* ssid; 
    const char* password;
    const char* root_ca;
    const char* endpoint;


    HTTPClient http;

    String hacerSolicitud(String query);
    void conectarWiFi(String ssid, String password);
    void conectarEndpoint(String endpoint, String authToken = "");

    bool servidorConectado;
};

#endif
