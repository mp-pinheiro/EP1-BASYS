# EP1-BASYS
Primeiro exercício Prático de Programação para a disciplina ACH2096.

## ORGANIZAÇÃO
O projeto está organizado em:
- **src**: arquivos do código fonte.
- **bin**: arquivos compilados.
- **doc**: documentação do software.

## COMPILING
```
$ make clean
$ make
```

## RUNNING
* ### Windows
	```
	$ run.bat
	```

* ### Linux
	```
	$ ./bin/BASYS
	```

## TODO
- [x] **Filas:**
	* Memória
	* Processador
	* Disco
	* Struct
		* Job
		
- [x] **Parâmetros**
	* Tempo entre jobs
	* Memória necessária
	* Tempo de processamento
	* Tempo de IO
	* Quantidade de IOs

- [x] **Job**
	* Id
	* IOs restantes

- [x] **Evento**
	* Id
	* Tempo
	* *Job	

- [x] **System Loop**
	* Evento 1: Chegada do Job.
	* Evento 2: Requisição de Memória.
	* Evento 3: Requisição do CPU.
	* Evento 4: Liberação do CPU para IO.
	* Evento 5: Requisição de IO.
	* Evento 6: Conclusão do IO.
	* Evento 7: Conclusão do Job.
	* Evento 8: Liberação do CPU para swap Round-Robin.
	
- [x] **Testar e Comparar Testes**
	* FCFS
	* Round-Robin
