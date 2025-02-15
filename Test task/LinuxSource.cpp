#include "std_testcase.h"
#include <dlfcn.h>

constexpr char const* FILENAME = "/tmp/file.txt";

void CWE114_Process_Control__w32_char_file_01_bad() {
    FILE* file = fopen(FILENAME, "r");

    if (file == nullptr) {
        perror("Cannot open file");
        return;
    }

    char buff[100] = "";
    size_t bytesRead = fread(buff, 1 /* byte */, sizeof(buff) - 1, file);
    buff[bytesRead] = 0; // Ensure zero is in place
    fclose(file);

    char const* data = buff;
    {
        /* POTENTIAL FLAW: If the path to the library is not specified, an attacker may be able to
         * replace his own file with the intended library */
        void* module = dlopen(data, RTLD_LAZY);
        if (module != nullptr) {
            dlclose(module);
            printf("Library loaded and freed successfully\n");
        } else {
            printf("Unable to load library\n");
        }
    }
}


/* goodG2B uses the GoodSource with the BadSink */
constexpr char const* LIB_PATH = "/lib/x86_64-linux-gnu/libc++.so.1";

static void goodG2B() {
    char dataBuffer[100] = "";
    char* data = dataBuffer;
    /* FIX: Specify the full pathname for the library */
    strcpy(data, LIB_PATH);
    {
        /* POTENTIAL FLAW: If the path to the library is not specified, an attacker may be able to
         * replace his own file with the intended library */
        void* module = dlopen(data, RTLD_LAZY);
        if (module != nullptr) {
            dlclose(module);
            printf("Library loaded and freed successfully \n");
        } else {
            printf("Unable to load library\n");
        }
    }
}

void CWE114_Process_Control__w32_char_file_01_good() {
    goodG2B();
}


/* Below is the main(). It is only used when building this testcase on
 * its own for testing or for building a binary to use in testing binary
 * analysis tools. It is not used when compiling all the testcases as one
 * application, which is how source code analysis tools are tested.
 */



int main(int argc, char* argv[]) {
    /* seed randomness */
    srand((unsigned) time(NULL));
    printf("Calling good()... \n");
    CWE114_Process_Control__w32_char_file_01_good();
    printf("Finished good() \n");

    printf("Calling bad()...\n");
    CWE114_Process_Control__w32_char_file_01_bad();
    printf("Finished bad() \n");
    return 0;
}

