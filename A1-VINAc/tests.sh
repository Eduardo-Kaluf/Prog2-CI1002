#!/bin/sh

clear

use_valgrind=false

if [ "$1" = "--valgrind" ]; then
    use_valgrind=true
fi

run_with_valgrind() {
    local label="test_${test_counter}"

    echo "[RUNNING] $label: $*"

    local tmp_log="valgrind_logs/${label}.tmp"
    local final_log="valgrind_logs/${label}.log"

    valgrind --leak-check=full --error-exitcode=1 "$@" > ./out.txt 2> "$tmp_log"

    local error_summary=$(grep -E "ERROR SUMMARY: " "$tmp_log")
    local freed_summary=$(grep "All heap blocks were freed -- no leaks are possible" "$tmp_log")
    local write_uninit_warning=$(grep "Syscall param write(buf) points to uninitialised byte(s)" "$tmp_log")

    local error_count=$(echo "$error_summary" | grep -oE "ERROR SUMMARY: [0-9]+" | awk '{print $3}')

    local should_log=false

    # Caso não tenha a mensagem "All heap blocks were freed", queremos log
    if [ -z "$freed_summary" ]; then
        should_log=true
    fi

    # Se houve erros (> 0), e o erro não for apenas o "write(buf) uninit"
    if [ "$error_count" -ne 0 ]; then
        if [ -z "$write_uninit_warning" ]; then
            should_log=true
        fi
    fi

    if $should_log; then
        mv "$tmp_log" "$final_log"
        echo "[VALGRIND ISSUE DETECTED in ${label}]"
        cat "$final_log"
    else
        rm "$tmp_log"
        echo "[VALGRIND CLEAN] $label"
    fi

    test_counter=$((test_counter + 1))
}

run_test() {
    echo "[RUNNING] test_${test_counter}: $*"
    "$@" > ./out.txt
    test_counter=$((test_counter + 1))
}

run() {
    if $use_valgrind; then
        run_with_valgrind "$@"
    else
        run_test "$@"
    fi
}

clear

test_counter=1
make clean
make homolog

cd tests
mkdir -p valgrind_logs

clear

# Teste 1: Sem parâmetros
run ./VINAc-homologation  > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

# Teste 2: Apenas uma flag aleatória -t
run ./VINAc-homologation  -t > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt

# Teste 3: Flag inválida -12 tamanho diferente que 2
run ./VINAc-homologation  -12 archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

# Teste 4: Flag inválida - tamanho diferente que 2
run ./VINAc-homologation  - archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

# Teste 5: Flag inválida, não possui "-"
run ./VINAc-homologation  12 archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/OPTION_IS_NOT_CORRECT.test ./out.txt
rm ./out.txt

# Teste 6: Archiver com nome inválido
run ./VINAc-homologation -i c > ./out.txt
diff ./out_files/expected_errors/ARCHIVER_IS_NOT_VALID.test ./out.txt
rm ./out.txt

# Teste 7: Archiver sem extensão
run ./VINAc-homologation -i archiver-test > ./out.txt
diff ./out_files/expected_errors/ARCHIVER_IS_NOT_VALID.test ./out.txt
rm ./out.txt

## TESTES -p

### TESTES COM O ARQUIVER INEXISTENTE

#### FORÇANDO ERROS

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 6: Arquivos duplicados
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Guardar apenas 1 arquivo
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Guardar 3 arquivos
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Guardar 5 arquivos
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Guardar 5 arquivos em uma ordem diferente
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt ./in_files/test3.txt ./in_files/test5.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test24351.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Guardar um arquivo muito grande
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

### TESTES COM O ARCHIVER JÁ CRIADO

#### FORÇANDO ERROS

touch ./archiver-test.vc

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/blank.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Guardar apenas 1 arquivo
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 2: Guardar 3 arquivos
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 3: Guardar 5 arquivos
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 4: Guardar 5 arquivos em uma ordem diferente
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt ./in_files/test3.txt ./in_files/test5.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test24351.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc

# Teste 5: Guardar um arquivo muito grande
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 6: Guardar um arquivo muito grande no meio do archiver
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test_giant_file.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test123G45.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

### TESTES COM O ARCHIVER JÁ CRIADO E COM CONTEÚDO

#### FORÇANDO ERROS

touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

# Teste 1: Sem parâmetros suficientes após o -p
# Backup deve ser ativado e archiver será igual ao test1
run ./VINAc-homologation  -p archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

# Teste 2: Arquivo fornecido não existe
# Backup deve ser ativado e archiver será igual ao test1
run ./VINAc-homologation  -p archiver-test.vc dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 3: Primeiro arquivo válido, segundo não existe
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt dont-exist.txt > ./out.txt
diff ./out_files/expected_errors/FILE_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 4: Arquivo fornecido está em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt

# Teste 5: Primeiro arquivo válido, segundo em branco
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./out_files/expected_outputs/blank.txt > ./out.txt
diff ./out_files/expected_errors/FILE_IS_BLANK.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt

# Teste 6: Arquivos duplicados
# Backup deve ser ativado e archiver permanecerá vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/DUPLICATED_MEMBER.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Sobreescrever um arquivo com o mesmo conteudo
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Sobrescrever um arquivo com conteudo diferente
touch ./archiver-test.vc
echo "aaa" > ./in_files/test_overwriting.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test_overwriting.txt > ./dump.txt
echo "TEST_OVERWRITING" > ./in_files/test_overwriting.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test_overwriting.txt > ./out.txt
diff ./out_files/expected_outputs/test_overwriting.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Sobrescrever o arquivo 1 e escrever os outros 2
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Sobrescrever o arquivo 1 e escrever os outros 4
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Sobrescrever o arquivo 1 e escrever os outros 5 (incluindo um arquivo muito grande)
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt  > ./dump.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 6: Sobrescrever dois arquivos que estão no meio e escrever outros 3
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt  > ./dump.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test24135.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -c

#### FORÇANDO ERROS

# Teste 1: Archive não existe
run ./VINAc-homologation  -c archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt

# Teste 2: Archive é vazio
touch ./archiver-test.vc
run ./VINAc-homologation  -c archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Parametros a mais do que o permitido
touch ./archiver-test.vc
run ./VINAc-homologation  -c archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/WRONG_AMOUNT_OF_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Printar na tela o que foi inserido
touch ./archiver-test.vc
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt
run ./VINAc-homologation  -c archiver-test.vc > ./out.txt
diff ./out_files/expected_outputs/test123c.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -m

#### FORÇANDO ERROS

# Teste 1: Archive não existe
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt

# Teste 2: Archive é vazio
touch ./archiver-test.vc
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Número de parâmetros diferente de 2
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/WRONG_AMOUNT_OF_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Movimentação de um membro que não existe
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/MEMBER_NOT_FOUND.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Mover um membro para o mesmo lugar
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test2.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test12.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Mover um membro pra trás do outro com apenas dois membro no archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test21.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Mover um membro da esquerda para direita para o final do archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test2.txt ./in_files/test5.txt > ./out.txt
diff ./out_files/expected_outputs/test13452.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Mover um membro da esquerda para direita para o meio do archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test13425.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Mover um membro da direta para a esquerda
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test4.txt ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test12435.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 6: Mover um membro muito grande do meio do arquivo
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test_giant_file.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test_giant_file.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test12345G.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 7: Mover um membro muito grande para o final do arquivo
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt ./in_files/test_giant_file.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test_giant_file.txt ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test123G45.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 8: Mover um membro muito grande
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test_giant_file.txt > ./dump.txt
run ./VINAc-homologation  -m archiver-test.vc ./in_files/test_giant_file.txt ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test1G234.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -r

#### FORÇANDO ERROS

# Teste 1: Número de parâmetros insuficiente
run ./VINAc-homologation  -r archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/NOT_ENOUGH_PARAMETERS.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Archive não existe
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Archive é vazio
touch ./archiver-test.vc
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Membro não existe no archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_errors/MEMBER_NOT_FOUND.test ./out.txt
diff ./archiver-test.vc ./out_files/expected_outputs/archiver_with_test1.vc
rm ./out.txt
rm ./archiver-test.vc

### FLUXO ESPERADO

# Teste 1: Remove 1 membro e deixa o archiver vazio
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_errors/OK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Remove 1 membro do início e deixa o archiver com o outro membro
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test2.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 3: Remove 1 membro do final e deixa o archiver com o outro membro
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 4: Remove 1 membro do início contendo mais de 2 membros no archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test1.txt > ./out.txt
diff ./out_files/expected_outputs/test23.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 5: Remove 1 membro do final contendo mais de 2 membros no archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test3.txt > ./out.txt
diff ./out_files/expected_outputs/test12.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 6: Remove 2 membros do meio do archive
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test2.txt ./in_files/test3.txt ./in_files/test4.txt ./in_files/test5.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test2.txt ./in_files/test4.txt > ./out.txt
diff ./out_files/expected_outputs/test135.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 7: Remove 1 membro muito grande
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./in_files/test_giant_file.txt > ./dump.txt
run ./VINAc-homologation  -r archiver-test.vc ./in_files/test_giant_file.txt > ./out.txt
diff ./out_files/expected_outputs/test1.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

## TESTES -x

#### FORÇANDO ERROS

# Teste 1: Archive não existe
run ./VINAc-homologation  -x archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Archive é vazio
touch ./archiver-test.vc
run ./VINAc-homologation  -x archiver-test.vc > ./out.txt
diff ./out_files/expected_errors/ARCHIVE_IS_BLANK.test ./out.txt
rm ./out.txt
rm ./archiver-test.vc

#### FLUXO ESPERADO

# Teste 1: Extrai o arquivo encontrado e informa o não encontrado
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt > ./dump.txt
run ./VINAc-homologation  -x archiver-test.vc ./in_files/test2.txt > ./out.txt
diff ./out_files/expected_outputs/test2_not_found_test1_found.sol ./out.txt
rm ./out.txt
rm ./archiver-test.vc

# Teste 2: Extrai um arquivo de um archiver que contém somente ele
echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Teste 3: Extrai um arquivo de um archiver que contém mais membros
echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./dinamic.txt ./in_files/test3.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Teste 4: Extrai dois arquivos de um archiver que contém mais membros
echo "TEST" > ./dinamic.txt
echo "TEST" > ./dinamic2.txt
echo "TEST" > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./in_files/test1.txt ./dinamic.txt ./in_files/test3.txt ./dinamic2.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
echo "CHANGED" > ./dinamic2.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt ./dinamic2.txt > ./out.txt
diff ./dinamic.txt ./static.txt
diff ./dinamic2.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./dinamic2.txt
rm ./static.txt

# Teste 5: Extrai um membro muito grande
cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Teste 6: Extrai um membro sem passar argumentos
echo "TEST" > ./dinamic.txt
echo "TEST" > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Teste 7: Extrai todos os membros
echo "TEST" > ./dinamic.txt
echo "TEST" > ./dinamic2.txt
echo "TEST" > ./dinamic3.txt
echo "TEST" > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./dinamic.txt ./dinamic2.txt ./dinamic3.txt  > ./dump.txt
echo "CHANGED" > ./dinamic.txt
echo "CHANGED" > ./dinamic2.txt
echo "CHANGED" > ./dinamic3.txt
run ./VINAc-homologation  -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
diff ./dinamic2.txt ./static.txt
diff ./dinamic3.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./dinamic2.txt
rm ./dinamic3.txt
rm ./static.txt

# Teste 8: Extrai um membro muito grande
cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
run ./VINAc-homologation  -p archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

## TESTES -i com -x

# Teste 1: Insere um membro que é compactado e tem seus bytes diminuidos, extrai esse mesmo membro
cat ./in_files/test_giant_file.txt > ./dinamic.txt
cat ./in_files/test_giant_file.txt > ./static.txt
run ./VINAc-homologation  -i archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Teste 2: Tenta compactar e inserir, porém guarda sem a compactação, extrai esse mesmo membro
##TENTOU COMPACTAR E GUARDOU DESCOMPACTADO
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > ./dinamic.txt
echo "ABCDEFGHIJKLMNOPQRSTUVWXYZ" > ./static.txt
run ./VINAc-homologation  -i archiver-test.vc ./dinamic.txt > ./dump.txt
echo "CHANGED" > ./dinamic.txt
run ./VINAc-homologation  -x archiver-test.vc ./dinamic.txt > ./out.txt
diff ./dinamic.txt ./static.txt
rm ./archiver-test.vc
rm ./dinamic.txt
rm ./static.txt

# Limpa ambiente de testes

touch ./out.txt
rm ./out.txt
touch ./dump.txt
rm ./dump.txt
touch ./archiver-test.vc
rm ./archiver-test.vc

cd ..

make clean
