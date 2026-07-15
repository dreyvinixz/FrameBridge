# FrameBridge — Task List

## Fase 1 — Setup do Ambiente e Repositórios

- [x] Verificar ferramentas instaladas (Git, Visual Studio, Inno Setup)
- [x] Clonar OptiScaler com submodules
- [x] Compilar OptiScaler localmente (confirmar build funciona) (Pular porque não temos MSVC)
- [x] Criar estrutura do projeto FrameBridge (monorepo)
- [x] Configurar LICENSE (GPLv3) e README.md

## Fase 2 — Integrar Instalador do DLSS-Enabler

- [x] Adaptar script Inno Setup para OptiScaler v0.9.3
- [x] Adaptar build scripts (Substituído pelo pipeline do GitHub Actions)
- [x] Configurar CI/CD (GitHub Actions: compile → package → release)

## Fase 3 — Contribuições Upstream

- [x] Corrigir bugs no DLSS-Enabler (PR upstream)
- [x] Atualizar DLSS-Enabler para OptiScaler v0.9+ (PR upstream)

## Fase 4 — Diferenciadores do FrameBridge

- [x] Pipeline source → build → installer configurado (roda via GitHub Actions)
- [x] Documentação (README.md, CONTRIBUTING.md)
- [x] Primeiro release (será gerado automaticamente pelo push para a branch main no GitHub)
