# Stable Matching Engine in C++

A polished C++ implementation of the **Gale–Shapley stable matching algorithm**, designed to compute stable pairings from ranked preference lists through an iterative process of proposals, tentative selections, and rejections.

This project captures a classic algorithmic idea from matching theory and translates it into a clear executable system that reads structured preference data, simulates deferred acceptance, and exposes the full progression of the matching process round by round.

## Overview

At its core, this program solves a fundamental question in algorithm design:

**How can two groups with competing preferences be matched in a way that guarantees stability?**

Using the Gale–Shapley framework, the program constructs stable pairings between two sets of participants by repeatedly allowing one side to propose while the other side keeps only its most preferred available option. The result is a matching in which no two participants would both prefer each other over their assigned partners.

## Why This Project Matters

This project is more than a simple classroom implementation. It demonstrates how a mathematically rigorous algorithm can be turned into a working computational system that models preference-based decision making.

It is especially relevant to topics such as:

- algorithm design
- discrete mathematics
- game theory
- matching markets
- computational social choice

Because stable matching appears in real-world systems such as residency placement, school choice, and assignment markets, this implementation reflects an algorithm with genuine theoretical and practical significance.

## Core Idea

Each participant has an ordered preference list. The algorithm proceeds in rounds:

1. Each unmatched man proposes to the highest-ranked woman who has not yet rejected him.
2. Each woman reviews all current proposals.
3. She tentatively holds the proposer she prefers most.
4. All other proposers are rejected.
5. Rejected participants continue to their next preference in later rounds.

This continues until the system reaches a stable state.

## Program Features

- Reads structured preference data from an input file
- Represents participants using a dedicated `Person` struct
- Simulates the full Gale–Shapley deferred acceptance process
- Tracks proposals, rejections, and tentative matches across rounds
- Prints intermediate matching states for transparency and debugging
- Uses clean procedural decomposition across multiple helper functions

## Data Model

Each participant is stored as a `Person` structure:

```cpp
struct Person {
    string name;
    string propositions[5];
    string stringPrefs[5];
    int rejectNum = 0;
    bool onHold = false;
    bool proposedTo = false;
};

```

## Field Descriptions

- `name` — the participant’s identifier
- `propositions` — current proposals received
- `stringPrefs` — ranked preferences
- `rejectNum` — number of rejections received so far
- `onHold` — whether the participant is currently being tentatively accepted
- `proposedTo` — whether a participant on the receiving side has received any proposal

## Architectural Components

### `main`

Initializes the program, validates command-line input, loads participant data, prepares the proposal state, launches the matching procedure, and releases allocated memory.

### `read_file`

Parses the input file and builds the participant arrays for each side of the match.

### `calcMatches`

Drives the main execution loop of the algorithm by alternating between proposal rounds and rejection rounds until the termination condition is reached.

### `propose`

Implements the proposal logic for an individual proposer, ensuring they advance through their ranked list only as needed.

### `reject`

Implements the acceptance logic for the receiving side by selecting the strongest available proposal and rejecting the rest.

### `getPreference`

Determines the most preferred proposer among current candidates using the receiver’s ranking.

### `print`

Outputs the current state of the matching process, making the algorithm’s evolution visible at every stage.

## Input Format

The program expects a structured input file containing:

- five men and their ranked preferences
- five women and their ranked preferences

The parsing logic assumes a fixed file layout and a fixed participant count of five per group.
