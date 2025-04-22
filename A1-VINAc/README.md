### Trabalho A1 - VINAc 

## EDUARDO KALUF - GRR20241770

# Arquivos e Diretórios

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

tests.sh:
    Script de testes que utiliza o diretório tests para validar todos (ou quase todos) os casos possíveis de uso do programa 

Todos esses exceto a main possuem arquivos headers definindo as estruturas, funções, constantes e enumerações utilizadas.

# Algoritmos e Planejamento:

Durante o desenvolvimento do programa duas partes fizeram eu ficar em dúvida qual decisão optar.
A primeira delas foi se eu deveria utilizar uma lista ou um vetor para organizar e modificar os dados do diretório, optei
por utilizar um vetor, devido ao fato que eu gostaria de deixar o diretório ordenado com o conteúdo, além de que 
acho que as operações no vetor possuem uma simplicidade melhor para este programa.

A segunda foi a maneira de mover o conteúdo dentro do diretório.
Tive a ideia de criar uma cópia do .vc, mover o conteúdo dentro do arquivo original e então passar de volta o contúedo da cópia,
porém achei que seria algo com o custo alto demais e a lógica ficaria dificil para alguns casos especifícos.
Sendo assim implementei as movimentações fazendo alterações e shifts dentro do própio archiver.

A única estrutura utilizada é a "dir_member_t", responsável por armazenar as informações de cada membro incluido no archiver.

Toda a lógica do programa se baseia em ler a sessão do diretório para um vetor e a partir dele fazer as movimentações e inclusões
no archiver, consertando os offsets e ordem de inclusão ao longo do caminho.

A maior dificuldade foi a implementação dos testes, pois foram muitos casos para serem implementados e testados, além
de ser um trabalho braçal e minucioso. Porém, acho que cheguei em um resultado satisfatório que garante o bom funcionamento do 
programa.
A partir dos testes fiquei livre para realizar algumas mudanças e aperfeiçoamentos, tendo a certeza de que não iriam 
quebrar nenhum outro lugar do código.
A outra parte difícil foi fazer o planejamento para começar e implementar a primeira opção do VINAc, pois levou tempo
para criar todas as funções auxiliares e ter certeza de que elas estavam funcionando corretamente (ainda mais pelo fato
de nessa etapa do projeto os testes não estarem prontos).

As lógicas e algoritmos estão documentadas nos próprios arquivos de implementação e de header.
Tentei deixar o mais detalhado e sucinto possível.

BUGS CONHECIDOS:
    - UNPREDICTABLE BEHAVIOR:
        - inserir um arquivo com nome maior que 128 caracteres
        - passar um archiver sem a extensão ".vc"
