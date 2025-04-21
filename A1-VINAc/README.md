### Trabalho A1 - VINAc 

## EDUARDO KALUF - GRR20241770

# Arquivos

archiver.c:
    - Funções responsáveis para gerenciar o "arquivo.vc"
    Responsável por tratar as opções utilizadas pelo usuário e fazer o gerenciamento do diretório e do conteúdo

content.c:
    - Funções que podem modificar a parte de conteúdo do "arquivo.vc"
    Responsável por alterar, escrever, ler e configurar a parte do conteúdo
    Não necessariamente modificam sempre o conteúdo

dir_member.c:
    - Funções responsáveis por manejar as informações dos membros

directory.c:
    - Funções responsáveis por modificar a parte do diretório do "arquivo.vc"
    Responsável por alterar, escrever, ler e configurar a parte do diretório

files.c:
    - Funções responsáveis por mexer em arquivos genéricos
    Modifica, cria, lê e fornece informações sobre um arquivo genérico, sendo ele um .vc ou não 

logger.c:
    - Funções responsáveis por fornecer informações ao usuário
    Responsável por administrar mensagens de erro 

lz.c:
    - Funções responsáveis pela parte de compressão e descompressão do arquivador

main.c:
    - Arquivo master do projeto
    Contém o fluxo geral do programa e tratamento de erros

utils.c:
    - Funções utilitárias
    Contém funções que não se encaixam em nenhum outro header.
    Seu respectivo header pode ser pensado como um arquivo para declaração de constantes.

makefile:
    Realiza a linkagem e produz o executável do programa

Todos esses exceto a main possuem arquivos headers definindo as estruturas, funções, constantes e enumerações utilizadas.

# Algoritmos e Planejamento:
