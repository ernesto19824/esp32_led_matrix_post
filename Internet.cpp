#include "Internet.h"

Internet::Internet(){
    ssid = "tu_ssid";
    password = "tu_contra";

    root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "tu_certificado root ssl\n" \
    "-----END CERTIFICATE-----\n";



    endpoint = "tu_endpoint";

    servidorConectado = false;
}

//publicas
void Internet::Iniciar(){
    Internet::conectarWiFi(String(Internet::ssid), String(Internet::password));
}

void Internet::CambiarWifi(String valorSSID, String valorPassword){
    Internet::conectarWiFi(valorSSID, valorPassword);

    valorSSID.reserve(0);
    valorPassword.reserve(0);   
}

void Internet::CambiarEndpoint(String nuevoEnpoint, String tokenAuth){
    if(tokenAuth.isEmpty()){
        Internet::conectarEndpoint(nuevoEnpoint);
    }
    else{
        Internet::conectarEndpoint(nuevoEnpoint, tokenAuth);
    }
}

bool Internet::ComprobarConexion(){
    return WiFi.status() == WL_CONNECTED;
}

void Internet::Terminar(){
    if(servidorConectado == true){
        http.end();
        servidorConectado = false;
    }
    
    WiFi.disconnect();
}

//privadas
void Internet::conectarWiFi(String valorSSID, String valorPassword){
    Internet::Terminar();

    Serial.print(F("Se conectara a '"));
    Serial.print(valorSSID);
    Serial.println(F("'"));
    Serial.println(F(""));

    WiFi.begin(valorSSID, valorPassword); 
    
    for (int intentos = 0; intentos < 120 && WiFi.status() != WL_CONNECTED; intentos++) {
        delay(1000);
        Serial.print(F("Conectando WiFi '"));
        Serial.print(valorSSID);
        Serial.println(F("'..."));
    }

    Serial.println(F(""));
    if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
        Serial.print(F("Conectado a '"));
        Serial.print(valorSSID);
        Serial.println(F("'!"));

        Internet::conectarEndpoint(endpoint, "Bearer tu_token_esperado");
    }
    else{
        Serial.print(F("Error conectando a '"));
        Serial.print(valorSSID);
        Serial.println(F("'"));
    }

    valorSSID.reserve(0);
    valorPassword.reserve(0);
}

void Internet::conectarEndpoint(String endpoint, String authToken){
    if(servidorConectado == true){
        http.end();
        servidorConectado = false;
    }

    http.begin(endpoint.c_str(), root_ca);
    http.addHeader("Connection", "keep-alive"); // Agrega esta línea
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    if(!authToken.isEmpty()){
        http.addHeader("Authorization", authToken.c_str());
    }

    servidorConectado = true;
}

String Internet::hacerSolicitud(String query){
    if (!(WiFi.status() == WL_CONNECTED)) { //Check the current connection status
        Serial.print(F("Sin coneccion para solicitud: "));
        Serial.println(query);
        return "";
    }

    Serial.print(F("Se hara esta solicitud: "));
    Serial.println(query);
    Serial.println("");
        
    int httpCode = http.POST(query);                                                  //Make the request
    if (httpCode > 0) {

        String payload = http.getString();
        Serial.print(F("Codigo: "));
        Serial.println(httpCode);
        Serial.print(F("Respuesta: "));
        Serial.println(payload);

        query.reserve(0);
        return payload;
    }
    else {
        Serial.print(F("Error en solicitud: "));
        Serial.println(httpCode);

        query.reserve(0);
        return "";
    }
}   




////////////////////////////////////////////////////////////////////////////////////////////////////

String Internet::LeerUltimoMensaje(){
    return Internet::hacerSolicitud("");
}
