#!/bin/sh

clear

make clean
make homolog

cd tests

clear

./VINAc-homologation > ./out.txt
diff ./NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

./VINAc-homologation -t > ./out.txt
diff ./NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

./VINAc-homologation -12 archiver-test.vc > ./out.txt
diff ./OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

./VINAc-homologation - archiver-test.vc > ./out.txt
diff ./OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

./VINAc-homologation 12 archiver-test.vc > ./out.txt
diff ./OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

# Remove o arquivo de teste de arquivador, se existir
rm ./archiver-test.vc

## TESTES -p

### TESTES COM O ARQUIVER INEXISTENTE

#### FORÇANDO ERROS

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc > ./out.txt
diff ./NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./blank.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc test1.txt test1.txt > ./out.txt
diff ./DUPLICATED_MEMBER.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc test1.txt dont-exist.txt > ./out.txt
diff ./FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc blank.txt > ./out.txt
diff ./FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc test1.txt blank.txt > ./out.txt
diff ./FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./blank.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc test1.txt > ./out.txt
diff ./test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt > ./out.txt
diff ./test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt test4.txt test5.txt > ./out.txt
diff ./test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc test2.txt test4.txt test3.txt test5.txt test1.txt > ./out.txt
diff ./test24351.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt test4.txt test_giant_file.txt test5.txt > ./out.txt
diff ./test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

##./VINAc-homologation -p archiver-test.vc test_other_extension.jpg > ./out.txt
##diff ./testjpg.sol ./out.txt
##rm ./out.txt
##rm ./archiver-test.vc
#
#### TESTES COM O ARCHIVER JÁ CRIADO
#
## Cria um novo arquivo vazio chamado archiver-test.vc
#touch ./archiver-test.vc
#
##### FORÇANDO ERROS
#
## Teste 1: Sem parâmetros suficientes após o -p
## Backup deve ser ativado e archiver permanecerá vazio
#./VINAc-homologation -p archiver-test.vc > ./out.txt
#diff ./NOT_ENOUGH_PARAMETERS.test ./out.txt
#diff ./archiver-test.vc ./blank.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
## Teste 2: Arquivo fornecido não existe
## Backup deve ser ativado e archiver permanecerá vazio
#./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
#diff ./FILE_NOT_FOUND.test ./out.txt
#diff ./archiver-test.vc ./blank.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
## Teste 3: Primeiro arquivo válido, segundo não existe
## Backup deve ser ativado e archiver permanecerá vazio
#./VINAc-homologation -p archiver-test.vc test1.txt dont-exist.txt > ./out.txt
#diff ./FILE_NOT_FOUND.test ./out.txt
#diff ./archiver-test.vc ./blank.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
## Teste 4: Arquivo fornecido está em branco
## Backup deve ser ativado e archiver permanecerá vazio
#./VINAc-homologation -p archiver-test.vc blank.txt > ./out.txt
#diff ./FILE_IS_BLANK.test ./out.txt
#diff ./archiver-test.vc ./blank.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
## Teste 5: Primeiro arquivo válido, segundo em branco
## Backup deve ser ativado e archiver permanecerá vazio
#./VINAc-homologation -p archiver-test.vc test1.txt blank.txt > ./out.txt
#diff ./FILE_IS_BLANK.test ./out.txt
#diff ./archiver-test.vc ./blank.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
##### FLUXO ESPERADO
#
#./VINAc-homologation -p archiver-test.vc test1.txt > ./out.txt
#diff ./test1.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
#./VINAc-homologation -p archiver-test.vc test1.txt test1.txt > ./out.txt
#diff ./test1.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
#./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt > ./out.txt
#diff ./test123.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
#./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt test4.txt test5.txt > ./out.txt
#diff ./test12345.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
#./VINAc-homologation -p archiver-test.vc test2.txt test4.txt test3.txt test5.txt test1.txt > ./out.txt
#diff ./test24351.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
##./VINAc-homologation -p archiver-test.vc test1.txt test2.txt test3.txt test4.txt test5.txt test_giant_file.txt > ./out.txt
##diff ./test12345G.sol ./out.txt
##rm ./out.txt
##rm ./archiver-test.vc
##touch archiver-test.vc
#
#./VINAc-homologation -p archiver-test.vc test_other_extension.jpg > ./out.txt
#diff ./testjpg.sol ./out.txt
#rm ./out.txt
#rm ./archiver-test.vc
#touch archiver-test.vc
#
#
#
#


cd ..

make clean
