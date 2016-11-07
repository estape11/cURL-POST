#include <QCoreApplication>
#include <stdio.h>
#include <curl/curl.h>
#include <iostream>
#include <cstring>

using namespace std;

struct MemoryStruct{
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size*nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct *)userp;
    mem->memory =(char*) realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* Sin memoria! */
        printf("No hay memoria suficiente memoria \n");
        return 0;
    }
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}

static string sendHTTP(string data,string url){
    CURL *curl;
    CURLcode res;
    struct curl_slist* headers=NULL;
    struct MemoryStruct chunk; //Aca se almacena el dato
    chunk.memory = (char*) malloc(1);
    chunk.size = 0;
    curl_global_init(CURL_GLOBAL_ALL);
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    res = curl_easy_perform(curl); //Ejecuta la accion
    curl_slist_free_all(headers); //Libero memoria
    if(res != CURLE_OK) { // Verifica errores
        fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    else{
        //printf("%lu bytes recibidos\n", (long)chunk.size); //imprimo los bytes que recibí
    }
    curl_easy_cleanup(curl); //SIEMPRE se debe limpiar
    string datoOut(chunk.memory);
    free(chunk.memory); //libero memoria
    return datoOut;
}



int main(int argc, char *argv[]){
    string data = "{\"cantidad\":\"10\"}";
    string url="http://localhost:9080/EvocionadorUniversal/rest/main/poblacion";
    cout<<sendHTTP(data,url)<<endl;
    return 0;
}
