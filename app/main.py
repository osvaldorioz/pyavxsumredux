from fastapi import FastAPI
import avx_module
import numpy as np
import time
from pydantic import BaseModel
from typing import List
import json
import random

def listaAleatorios(n: int):
      lista = [0]  * n
      for i in range(n):
          lista[i] = random.randint(0, 10000)* 0.0001
      return lista

app = FastAPI()

# Definir el modelo para el vector
class VectorF(BaseModel):
    vector: List[float]

@app.post("/avxsum")
async def calculo():
    start = time.time()

    # Crear un gran arreglo de prueba
    data = np.random.rand(4000000).astype(np.float32)

    # Sumar los elementos utilizando el módulo AVX
    result = avx_module.avx_sum(data)
    result2 = np.sum(data)

    #print("Data size:", data.size)
    #print("Suma de los elementos:", result)
    #print("Resultado esperado (NumPy):", result2)

    end = time.time()

    var1 = end - start

    j1 = {
        "Time taken in seconds": var1,
        "Data size": data.size,
        "Resultado esperado (Avx Sum)": result,
        "Resultado esperado (NumPy)": result2
    }
    jj = json.dumps(str(j1))

    return jj