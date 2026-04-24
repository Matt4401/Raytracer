/*
** EPITECH PROJECT, 2026
** RayTracer
** File description:
** SpherePlugin
*/

extern "C" const char *type(void) {
    return "primitive";
}

extern "C" const char *name(void) {
    return "invalid_data";
}

// Returns a function pointer cast to void* for C compatibility
extern "C" const char *builder(void) {
    return "builder";
}
