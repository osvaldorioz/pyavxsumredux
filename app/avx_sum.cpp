#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <immintrin.h> // Para AVX
#include <vector>
#include <stdexcept>

namespace py = pybind11;

// Suma los elementos de un arreglo utilizando AVX
float avx_sum(const float* data, size_t size) {
    if (size == 0) {
        throw std::invalid_argument("El tamaño del arreglo debe ser mayor a 0.");
    }

    __m256 sum = _mm256_setzero_ps(); // Inicializa un vector con ceros

    // Procesar en bloques de 8 elementos
    size_t i = 0;
    for (; i + 7 < size; i += 8) {
        __m256 vec = _mm256_loadu_ps(&data[i]); // Carga 8 valores en el registro AVX
        sum = _mm256_add_ps(sum, vec);         // Suma los 8 valores al acumulador
    }

    // Reducir el registro AVX a un único valor escalar
    alignas(32) float temp[8];
    _mm256_store_ps(temp, sum);
    float result = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];

    // Sumar los elementos restantes (si el tamaño no es múltiplo de 8)
    for (; i < size; ++i) {
        result += data[i];
    }

    return result;
}

// Enlace con Pybind11
py::float_ py_avx_sum(py::array_t<float, py::array::c_style | py::array::forcecast> input_array) {
    py::buffer_info buf = input_array.request();

    if (buf.ndim != 1) {
        throw std::runtime_error("La entrada debe ser un arreglo unidimensional.");
    }

    auto ptr = static_cast<float*>(buf.ptr);
    size_t size = buf.shape[0];

    return avx_sum(ptr, size);
}

PYBIND11_MODULE(avx_module, m) {
    m.doc() = "Módulo para sumar un arreglo usando AVX";
    m.def("avx_sum", &py_avx_sum, "Suma los elementos de un arreglo utilizando AVX");
}
