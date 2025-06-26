#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Disco.h"
#include "Archivos.h"
#include "RAM.h"

int cant_platos = 2;
int cant_pistas = 4;
int cant_sectores = 8;
int cant_bytes_sector = 512;

int main() {
    // 1. Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error: GLFW no pudo inicializarse." << std::endl;
        return -1;
    }

    // 2. Crear ventana con contexto OpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "Configuración de Disco", NULL, NULL);
    if (!window) {
        std::cerr << "Error: No se pudo crear la ventana GLFW." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 3. Inicializar Glad (para OpenGL)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error: No se pudo cargar Glad." << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4. Inicializar ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        std::cerr << "Error: ImGui_ImplGlfw_InitForOpenGL falló." << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!ImGui_ImplOpenGL3_Init("#version 130")) {
        std::cerr << "Error: ImGui_ImplOpenGL3_Init falló." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        // Nueva frame de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Ventana de configuración
        ImGui::Begin("Configuracion del Disco");
        ImGui::Text(" Bienvenido, Configure los parametros del disco:");

        ImGui::InputInt("Platos", &cant_platos);
        ImGui::InputInt("Pistas", &cant_pistas);
        ImGui::InputInt("Sectores", &cant_sectores);
        ImGui::InputInt("Bytes/Sector", &cant_bytes_sector);

        if (ImGui::Button("Iniciar")) {
            Disco disco(cant_platos, cant_pistas, cant_sectores, cant_bytes_sector);
            RAM ram;
            Archivos prueba;
            prueba.Leer_info_sql();
            prueba.Leer_info_csv(disco);
            disco.Rebuild_data();
            disco.get_columnas("TaX", "216.00", ram);
        }
        ImGui::End();

        // Renderizar
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Limpieza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}