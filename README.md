# Zombity - Survival Wave Shooter

![Zombity Game Logo](screenshot.png) <!-- Add actual screenshot path -->

## Visão Geral do Jogo
Zombity é um jogo de ação 2D com rolagem paralaxe onde você enfrenta ondas de zumbis cada vez mais desafiadoras. Recursos principais:

- Modo singleplayer e multiplayer local (2 jogadores)
- Sistema progressivo de ondas com 10 fases
- Efeito de parallax em múltiplas camadas
- Mecânicas de tiro e combate corpo-a-corpo

## Como Jogar

### Controles Player 1
- **Movimento**: W, A, S, D
- **Atirar**: ESPAÇO
- **Golpear**: F

### Controles Player 2 (Multiplayer)
- **Movimento**: Teclado Numérico (8,4,2,6)
- **Atirar**: 0 (Numérico)
- **Golpear**: - (Numérico)

## Sistema de Jogo

### Progressão
- 10 ondas de zumbis com dificuldade crescente
- Inimigos ficam mais rápidos a cada onda
- Reabastecimento de munição aleatório

### Personagens
- Vida limitada (10 pontos)
- Munição recarregável
- Dois estados de ataque: ranged e melee

## Requisitos Técnicos
- OpenGL
- GLUT
- Compilador C++

## Estrutura do Código
Principais componentes:
- `Player.cpp`: Lógica do jogador
- `Enemy.cpp`: IA dos zumbis
- `Wave.cpp`: Gerenciador de ondas
- `Layer.cpp`: Sistema de parallax

## Desenvolvedores
[Seu Nome/Nome da Equipe]  
[Contato/Website]

## Licença
Código aberto para fins educacionais. Modifique e distribua livremente.

---

**DICA**: Mantenha-se em movimento e colete munição para sobreviver às ondas finais!
