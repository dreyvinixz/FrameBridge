# Upstream Synchronization

O **FrameBridge** utiliza o OptiScaler oficial como seu motor de Runtime principal. 
Nossa filosofia é manter o componente `OptiScaler/` o mais fiel possível ao repositório upstream oficial, permitindo auditorias transparentes e atualizações fáceis.

## Versão Atual Integrada
- **OptiScaler Version:** v0.10.0-pre1 (Nightly/Dev branch)
- **Upstream Repository:** [https://github.com/optiscaler/OptiScaler](https://github.com/optiscaler/OptiScaler)
- **Commit Importado:** `62a45d44c2d74de07cd3bf90b6e23a74a27c2a5d` (branch `release/0.9` que contém as pre-releases 0.10.x).

## Local Patches (Alterações do FrameBridge)
Nós mantemos uma estrita política de *Zero Local Patches*, com as seguintes raras exceções feitas estritamente para manter retrocompatibilidade com o ecossistema do instalador:

1. **`OptiScaler/dllmain.cpp`**
   - **O que mudou:** Adicionado o reconhecimento dos nomes de arquivo `dlss-enabler-upscaler.dll` e `nvngx-upscaler.dll` dentro da função `CheckWorkingMode()`.
   - **Por que mudou:** Para permitir que usuários que utilizaram as versões passadas do instalador (que renomeavam a DLL dessa forma) continuem rodando o OptiScaler sem quebrar a cadeia de carregamento de mods do jogo.

## Procedimento de Atualização
Para futuras atualizações do OptiScaler (ex: v0.11), siga este fluxo:

1. Crie uma nova branch: `git checkout -b feature/optiscaler-v0.11`
2. Certifique-se de ter o remote: `git remote add upstream https://github.com/optiscaler/OptiScaler.git`
3. Atualize o código ignorando o histórico externo: `git fetch upstream && git checkout upstream/master -- OptiScaler/`
4. **Reaplique manualmente** os patches locais descritos acima usando o comando `git diff` da branch principal ou reescrevendo-os no `dllmain.cpp`.
5. Valide todas as configurações do INI novo e realize a Matriz de Testes em um jogo real (ex: FF7 Rebirth) antes do merge para a `main`.
