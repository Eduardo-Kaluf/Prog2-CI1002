#!/bin/sh

clear

make clean
make homolog

cd tests

clear

./VINAc-homologation > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

./VINAc-homologation -t > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

./VINAc-homologation -12 archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

./VINAc-homologation - archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

./VINAc-homologation 12 archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

## TESTES -p

### TESTES COM O ARQUIVER INEXISTENTE

#### FORÇANDO ERROS

rm ./archiver-test.vc

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt ./in_files/test3.txt ./in_files/test5.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test24351.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

### TESTES COM O ARCHIVER JÁ CRIADO

#### FORÇANDO ERROS

touch ./archiver-test.vc

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

#### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt ./in_files/test3.txt ./in_files/test5.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test24351.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test_giant_file.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test123G45.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc


### TESTES COM O ARCHIVER JÁ CRIADO E COM CONTEÚDO

#### FORÇANDO ERROS

touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver será igual ao test1
./VINAc-homologation -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver será igual ao test1
./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# sobreescrever um arquivo com o mesmo conteudo
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# sobreescrever um arquivo com conteudo diferente

touch ./archiver-test.vc
echo "aaa" > ./in_files/test_overwriting.txt
./VINAc-homologation -p archiver-test.vc ./in_files/test_overwriting.txt > ./dump.txt
echo "TEST_OVERWRITING" > ./in_files/test_overwriting.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test_overwriting.txt > ./out.txt
diff ./out_files/expected_outputs/test_overwriting.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#padrao
touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt  > ./dump.txt

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test24135.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -c

#### FORÇANDO ERROS

# Teste 1: Archiver inexistente
# Backup deve ser ativado e archiver será igual ao test1
./VINAc-homologation -c archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt

touch ./archiver-test.vc

./VINAc-homologation -c archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -c archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/WRONG_AMOUNT_OF_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

touch ./archiver-test.vc
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt

./VINAc-homologation -c archiver-test.vc > ./out.txt
diff ./out_files/expected_outputs/test123c.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -m

#### FORÇANDO ERROS

./VINAc-homologation -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt

touch ./archiver-test.vc
./VINAc-homologation -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -m archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/WRONG_AMOUNT_OF_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/MEMBER_NOT_FOUND.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test2.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test12.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test21.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test2.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test13452.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test13425.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test4.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test12435.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test_giant_file.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test_giant_file.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt ./in_files/test_giant_file.txt > ./dump.txt

./VINAc-homologation -m archiver-test.vc ./in_files/test_giant_file.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123G45.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -r

#### FORÇANDO ERROS

./VINAc-homologation -r archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/MEMBER_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc


### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/OK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test2.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test23.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test12.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test135.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./in_files/test_giant_file.txt > ./dump.txt
./VINAc-homologation -r archiver-test.vc ./in_files/test_giant_file.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -x

#### FORÇANDO ERROS

./VINAc-homologation -x archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

touch ./archiver-test.vc
./VINAc-homologation -x archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
./VINAc-homologation -x archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test2_not_found_test1_found.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc


echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
./VINAc-homologation -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./dinamic.txt ./in_files/test3.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

echo "TEST" > ./dinamic.txt
echo "TEST" > ./dinamic2.txt
echo "TEST" > ./static.txt
./VINAc-homologation -p archiver-test.vc ./in_files/test1.txt ./dinamic.txt ./in_files/test3.txt ./dinamic2.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
echo "CHANGED" > ./dinamic2.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt ./dinamic2.txt > ./out.txt
diff ./dinamic.txt ./static.txt
diff ./dinamic2.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./dinamic2.txt
rm ./static.txt

cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
./VINAc-homologation -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

#####

echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
./VINAc-homologation -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

echo "TEST" > ./dinamic.txt
echo "TEST" > ./dinamic2.txt
echo "TEST" > ./dinamic3.txt
echo "TEST" > ./static.txt
./VINAc-homologation -p archiver-test.vc ./dinamic.txt ./dinamic2.txt ./dinamic3.txt  > ./dump.txt
echo "CHANGED" > ./dinamic.txt
echo "CHANGED" > ./dinamic2.txt
echo "CHANGED" > ./dinamic3.txt
./VINAc-homologation -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
diff ./dinamic2.txt ./static.txt
diff ./dinamic3.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./dinamic2.txt
rm ./dinamic3.txt
rm ./static.txt

cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
./VINAc-homologation -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

## TESTES -z com -x

#COMPACTOU E DIMINUIU OS BYTES
cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
./VINAc-homologation -z archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

##TENTOU COMPACTAR E GUARDOU DESCOMPACTADO
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > ./dinamic.txt
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > ./static.txt
./VINAc-homologation -z archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
./VINAc-homologation -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt


rm ./out.txt
rm ./dump.txt
touch ./archiver-test.vc
rm ./archiver-test.vc

cd ..

make clean
