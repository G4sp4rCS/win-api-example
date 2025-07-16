#include <windows.h>
#include <stdio.h>

int main() {
    // Nombre del archivo que vamos a crear
    const char* fileName = "demo_archivo.txt";
    const char* content = "Hello! This file was created following the Windows Architecture flow.\n";

    printf("=== DEMO: Windows Architecture Function Call Flow ===\n");
    printf("Creating file: %s\n\n", fileName);

    // PASO 1: Aplicacion de usuario llama a CreateFile (WinAPI)
    // Esta funcion esta en kernel32.dll (Subsystem DLL)
    printf("1. Application calls CreateFile() [kernel32.dll]\n");

    HANDLE hFile = CreateFileA(
        fileName,                    // Nombre del archivo
        GENERIC_WRITE,              // Acceso de escritura
        0,                          // No compartir
        NULL,                       // Atributos de seguridad por defecto
        CREATE_ALWAYS,              // Crear siempre (sobrescribir si existe)
        FILE_ATTRIBUTE_NORMAL,      // Atributos normales
        NULL                        // No hay archivo template
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error: Could not create file. Error code: %lu\n", GetLastError());
        return 1;
    }

    printf("   └─ CreateFile() successful - Handle obtained\n");

    // PASO 2: Internamente, CreateFile llama a NtCreateFile (NTAPI)
    // Esta funcion esta en ntdll.dll - transicion hacia kernel mode
    printf("2. CreateFile() internally calls NtCreateFile() [ntdll.dll]\n");

    // PASO 3: NtCreateFile ejecuta syscall para pasar a kernel mode
    printf("3. NtCreateFile() executes syscall/sysenter to switch to kernel mode\n");

    // PASO 4: El kernel maneja la creacion real del archivo
    printf("4. Kernel (ntoskrnl.exe) handles the actual file creation\n");

    // Escribir contenido al archivo
    printf("\nWriting content to file...\n");

    DWORD bytesWritten;
    BOOL writeResult = WriteFile(
        hFile,                      // Handle del archivo
        content,                    // Contenido a escribir
        strlen(content),            // Número de bytes a escribir
        &bytesWritten,              // Bytes escritos
        NULL                        // No operación asíncrona
    );

    if (writeResult) {
        printf("Content written successfully (%lu bytes)\n", bytesWritten);
    }
    else {
        printf("Error writing to file. Error code: %lu\n", GetLastError());
    }

    // Cerrar el handle del archivo
    CloseHandle(hFile);
    printf("File closed correctly\n");

    printf("\n=== FLOW SUMMARY ===\n");
    printf("User Application → CreateFile() [kernel32.dll] → NtCreateFile() [ntdll.dll] → syscall → Kernel Mode\n");
    printf("\nThe file '%s' has been created following the Windows Architecture flow\n", fileName);

    // Verificar que el archivo existe
    printf("\nVerifying that the file exists...\n");
    DWORD fileAttributes = GetFileAttributesA(fileName);
    if (fileAttributes != INVALID_FILE_ATTRIBUTES) {
        printf("✓ File confirmed - exists in the system\n");
    }
    else {
        printf("✗ Error: Cannot verify file\n");
    }

    return 0;
}