from setuptools import setup, Extension
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "avx_module",
        ["app/avx_sum.cpp"],
        extra_compile_args=["-mavx"],  # Habilitar AVX
    ),
]

setup(
    name="avx_module",
    version="1.0",
    author="Osvaldo R",
    description="Módulo para sumar arreglos utilizando AVX",
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)
