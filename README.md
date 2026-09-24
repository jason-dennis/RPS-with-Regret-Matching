# Rock Paper Scissors — Regret Matching in C++
 
A small learning project built to understand **regret matching**, the core idea behind
Counterfactual Regret Minimization (CFR), the algorithm used by superhuman poker AIs
such as Libratus and Pluribus.
 
The project implements regret matching from scratch in C++ (no ML libraries), verifies it
against known theoretical results, and wraps it in an interactive terminal game where you
can play against a bot that learns from your moves in real time.
 
It is the first step of a larger project: solving **Kuhn Poker** with CFR.
 
---
 
## Screenshots
 
**Varied play: the bot stays close to equilibrium**
 
![Balanced game](photo1)
 
**The player keeps choosing Rock: the bot learns to exploit it**
 
![Bot exploiting the player](photo2)
 
The top panel shows the bot's *current* strategy, which reacts quickly to recent moves.
The bottom panel shows its *average* strategy, which moves slowly and is the quantity that
converges to equilibrium in self-play.
 
---
 
## How regret matching works
 
After every round, the agent asks: *"how much better would I have done if I had played
each other action instead?"* That difference is the **regret** for each action.
 
1. Regrets are accumulated over all rounds.
2. The next strategy picks each action with probability proportional to its **positive**
   cumulative regret. Negative regrets count as zero.
3. If no action has positive regret, the agent plays uniformly (1/3 each).
A key detail: in self-play the *current* strategies of the two agents chase each other in
cycles, but their **average** strategies converge to a Nash equilibrium.
 
---
 
## Experiments
 
### 1. Against a fixed opponent
 
The opponent plays Rock 40%, Paper 30%, Scissors 30%. After 100,000 iterations the agent's
average strategy is:
 
| Rock    | Paper   | Scissors |
|---------|---------|----------|
| 0.0003  | 0.9996  | 0.0001   |
 
The agent correctly learns the best response: always play Paper against an opponent who
over-plays Rock.
 
### 2. Self-play
 
Both players use regret matching and learn against each other, starting from different
initial strategies. After 100,000 iterations:
 
| Player   | Rock  | Paper | Scissors |
|----------|-------|-------|----------|
| Player 1 | 0.336 | 0.333 | 0.330    |
| Player 2 | 0.331 | 0.334 | 0.335    |
 
Both average strategies converge to the Nash equilibrium of 1/3 for each action.
 
### 3. Interactive game
 
You play against the bot in the terminal. The bot starts uniform and updates its regrets
after every round, so repeating the same move gets punished quickly. Because it reacts to
recent history, a player who switches moves can also exploit the bot in turn, which
illustrates why the equilibrium (average) strategy, not the reactive one, is the
unexploitable one.
 
---
 
## Build and run
 
Requirements: a C++17 compiler and CMake.
 
```bash
cmake -B build
cmake --build build
./build/RPS
```
 
Controls: `r` Rock, `p` Paper, `s` Scissors, `q` Quit.
 
On Windows, run the executable in **Windows Terminal** for correct colors and box drawing
characters. The legacy console and some IDE output windows do not support them.

---

## Next step
 
Extend regret matching to sequential games with hidden information: implement **CFR**
on **Kuhn Poker** and verify convergence to the known equilibrium (game value −1/18 for
the first player).
 
## References
 
- Todd Neller, Marc Lanctot — *An Introduction to Counterfactual Regret Minimization* (2013)
- Zinkevich et al. — *Regret Minimization in Games with Incomplete Information* (2007)
