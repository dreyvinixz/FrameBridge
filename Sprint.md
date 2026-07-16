# Roadmap: Fase 2 — Diagnostics & Runtime Analysis

Este documento serve como o rastreador oficial de progresso (Tracker) para a Fase 2 do projeto FrameBridge. O objetivo desta fase é consolidar as ferramentas de diagnóstico e análise estruturada, construindo uma base em C# (.NET 8) sem interferir diretamente na inicialização do jogo (evitando conflitos com Steam, Epic, etc).

Nenhuma telemetria é enviada para a nuvem. Todos os diagnósticos são **locais**.

## Estrutura do Monorepo

```text
FrameBridge/
├── Runtime/                    (C++ - OptiScaler/Wrapper)
├── Installer/                  (Inno Setup)
├── FrameBridge.Common/         (C# - Biblioteca Compartilhada)
├── FrameBridge.Diagnostics/    (C# - Ferramenta CLI)
├── FrameBridge.Launcher/       (C# - Gerenciador Futuro)
└── docs/                       (Documentação)
```

---

## 🏃‍♂️ Sprint 2.1 — FrameBridge.Common
**Objetivo:** Criar a biblioteca base que será consumida por todos os futuros executáveis.

- [ ] Configurar a solution (`FrameBridge.sln`) e o projeto `FrameBridge.Common`.
- [ ] Implementar classe `Logger` (escrita simultânea em texto e console).
- [ ] Implementar classe `Json` (Serializador/Parser estruturado).
- [ ] Implementar classe `Hardware` (Detecção via DXGI/SetupAPI: GPU, VRAM, Driver).
- [ ] Implementar classe `RuntimeInfo` (Detecção da versão do OptiScaler e Injeção).
- [ ] Implementar classe `GameInfo` (Leitura de Hash e Nome do executável).
- [ ] Configurar GitHub Actions para compilar o código C# a cada push.

---

## 🏃‍♂️ Sprint 2.2 — FrameBridge.Diagnostics (Coleta)
**Objetivo:** Criar o executável independente para gerar logs locais estruturados, sem depender do jogo.

- [ ] Criar o projeto Console `FrameBridge.Diagnostics`.
- [ ] Referenciar o `FrameBridge.Common`.
- [ ] Implementar o comando `--collect`.
- [ ] O comando deve gerar a pasta `Logs/` na raiz.
- [ ] O comando deve cuspir os arquivos `FrameBridge.log` (para humanos) e `FrameBridge.json` (para máquinas) com o estado atual do ambiente.

---

## 🏃‍♂️ Sprint 2.3 — Crash Collector (Relatório)
**Objetivo:** Expandir a ferramenta de diagnóstico para empacotar todos os arquivos vitais de suporte em um único ZIP.

- [ ] Implementar o módulo `Compression` no `FrameBridge.Common`.
- [ ] Adicionar o comando `--report` no `FrameBridge.Diagnostics.exe`.
- [ ] O comando deve acionar o `DxDiag.txt` de forma silenciosa (em background).
- [ ] O comando deve gerar o arquivo `FrameBridgeReport.zip`.
- [ ] Garantir que o ZIP contenha: `manifest.json`, `FrameBridge.log`, `FrameBridge.json`, `OptiScaler.log`, `OptiScaler.ini`, `DxDiag.txt`, `Hardware.json` e `Runtime.json`.

---

## 🏃‍♂️ Sprint 2.4 — Integração com Instalador
**Objetivo:** Disponibilizar a ferramenta de diagnóstico para o usuário final de forma fácil e acessível.

- [ ] Atualizar o script de CI/CD para que o binário do `FrameBridge.Diagnostics.exe` seja incluído no release.
- [ ] Modificar o `setup.iss` para extrair a pasta `Diagnostics` junto com o mod.
- [ ] Adicionar no instalador a criação de um atalho no Menu Iniciar/Área de Trabalho chamado: **"Generate Diagnostic Report (FrameBridge)"**.

---

## 🏃‍♂️ Sprint 2.5 — FrameBridge Launcher (Gerenciador)
**Objetivo:** Criar a primeira versão visual (GUI/CLI avançado) para o usuário gerenciar sua instalação, sem necessariamente iniciar o jogo.

- [ ] Criar o projeto `FrameBridge.Launcher`.
- [ ] Reutilizar totalmente as classes do `FrameBridge.Common`.
- [ ] Implementar função: *Abrir pasta do jogo*.
- [ ] Implementar função: *Gerar diagnóstico (chama a Sprint 2.3)*.
- [ ] Implementar função: *Abrir OptiScaler.ini para edição*.
- [ ] Implementar função: *Validar instalação*.
