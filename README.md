# Atividade: Convolução Overlap-and-Add

É uma atividade que se trata da implementação da convolução com método Overlap-and-Add, na linguagem C, para realizar a 
convolução de dois sinais.

## Dependências

Na sua distribuição linux, certifique-se de já ter instalado os pacotes:

- gcc
- make
- octave
- pdflatex

## Instruções de Build e Execução

1. Clone o repositório na sua máquina

```
$ git clone https://github.com/icaro-meneses/convolve-overlap-and-add.git
$ cd convolve-overlap-and-add
```

2. Para realizar a compilação do programa

```
$ make release
```

3. Para executar somente o programa:

```
$ ./conv_ovp_add.x
```

4. Para executar o programa e exportar o arquivo em .pdf:

```
$ make run graph
```

5. Caso precise fazer a limpeza dos arquivos, execute: `make clean`

6. **Extra:** Para realizar a compilação no modo de DEBUG:
```
$ make debug
```

**Nota:** O código ainda requer testes.


